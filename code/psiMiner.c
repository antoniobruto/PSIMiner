/*****************************************************************
 *                      CODE WRITTEN BY                          *
 *                                                               *
 *             Antonio Anastasio Bruto da Costa                  *
 *     Formal Method Laboratory, Dept. of CSE, IIT Kharagpur     *
 *                                                               *
 *****************************************************************/

#define MAIN_DEBUG

//#define COV_TEST

#ifdef COV_TEST
		#define MAIN_DEBUG
#endif

#ifndef MAX_STR_LENGTH
	#define MAX_STR_LENGTH 10240
#endif

#define RECREATE_COUNT 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "psiMinerStructs.h"
#include "structs.h"

#include "predToPy.tab.h"
#include "intervalList.tab.h"
#include "learnedOP.tab.h"


extern FILE* intervalListin;
extern int intervalListparse(void);

extern FILE* predToPyin;
extern int predToPyparse(void);

struct file* predicateMap = NULL;
struct identifier* idList = NULL;
struct config* inputConfig = NULL;

struct PORV* learnedPORVs = NULL;
//struct listOfIntervalListsStruct** learnedIntervalSets = NULL;
struct listOfIntervalListsStruct* learnedIntervalSet = NULL;
int learnedPORVCount = 0;
int continueFlag = 0;
int assertCount = 0;
int traceID = 0;
struct listOfIntervalListsStruct** listOfIntervalSets = NULL;
struct listOfIntervalListsStruct* intervalSet = NULL;
struct listOfIntervalListsStruct* coveredSet = NULL;
struct intervalListStruct* coveredList = NULL;
struct treeNode* decisionTree;
struct nodeList* assertionList;
struct identifier* traceFileNames;
int traceCount;
double* traceLengths;
//FOR READING BEST PREDICATES LIST
/*
learnedOPin = fopen("best_predicates.txt","r");
learnedOPparse();
// printPredicateDetailListToFilePtr(details,stdout);
*/
double tl; //Adding because the pointer doesn't work for moving data between this file and the parser
extern FILE* learnedOPin;
extern int learnedOPparse(void);

struct predicateDetail* details = NULL;
//struct predicateDetail* allDetails = NULL;	//Sorted Accumulator of predicates learned
int logging = 1;	//Logging is 1 if logs are enabled and 0 otherwise

//----------------------------------------

char* assertFileName;
int numberOfPORVs = 0;
int numTargets =0;
double K;
int N;
//int targetPORV_id;
int strict = 0;				//TODO: Implement strict pseudo targets
double totalTraceLength = 0.0;
double totalTrueLength = 0.0;
double totalFalseLength = 0.0;
int separationType = 1;

double correlationThreshold = 1.0;
double supportThreshold = 0.001;

int targetBias = 0;		/*	
						targetBias biases the algorithm to learn 
						for specific target truths. 
							-1 -> 	a negatedd target, 
							+1 -> 	a true target, 
							0  -> 	no bias.
					*/
int learnPredicates = 0;		/*	
						learnPredicates indicates the level at which
						learning is performed. 
							0 ->	Knowledge Only:
								Given a Predicate Alphabet,
								only timing is learned.
							1 ->	Learn on zero gain:
								Predicates are learned if gain
								is zero for the current alphabet
								in all temporal positions
							2 ->	Best of Both Worlds:
								Learn a new predicate list at
								every decision node. Pick the best
								predicate and temporal position
								among domain knowledge and new
								knowledge.
							3 ->	Ignorance is bliss:
								Only use learned predicates	
						*/

FILE* logFile;				/*	The Log File used for logging function calls	*/
FILE* predLogFile;			/*	The Predicate Log for logging learned predicates that are used in the decision process*/

int cumulative = 0;			//	Determine how gain is computed.
struct assertionStruct* allAssertions=NULL;
void copyContents(struct treeNode* root, struct treeNode* newRoot){
	newRoot->truthList = duplicateTruthAssignmentList(root->truthList);
	newRoot->listOfIntervalSets = root->listOfIntervalSets;
	newRoot->mean = root->mean;
	newRoot->error = root->error;
	newRoot->targetInfluence = root->targetInfluence;
	newRoot->splittingPredicate_id = root->splittingPredicate_id;
	newRoot->traceLength = root->traceLength;
	newRoot->id = root->id;
	newRoot->truthValue = root->truthValue;
	newRoot->predType = root->predType;
	newRoot->learnedPredicates =  root->learnedPredicates;
}

struct treeNode* duplicateTree(struct treeNode* root, int ind, struct listOfIntervalListsStruct** learnedIntervalSets){
	if(root == NULL) return NULL;
	//printf("@ ");
	struct treeNode* duproot = createTreeNode(
                                                  NULL,//Truth List
                                                  listOfIntervalSets,//IntervalSet
												  learnedIntervalSets,
                                                  0,0,totalTraceLength,NULL,NULL);
	//root->originalToDup[ind] = newroot;
	duproot->dupToOriginal = root;
	copyContents(root,duproot);
	duproot->left = duplicateTree(root->left,ind, learnedIntervalSets);
	duproot->right = duplicateTree(root->right,ind, learnedIntervalSets);
	return duproot;
}

void deleteExtraLists(struct listOfIntervalListsStruct* lists, int numberOfPORVs)
{
	int ptr_ind=1;
	//printf("before=%d\n",printLengthOfIntervalLists(lists));
	while(ptr_ind<numberOfPORVs) 
	{
		ptr_ind++;
		lists = lists->nextList;
	}
	//if(i==2) printf("@\n");	   
	lists->nextList = NULL;
	//deleteListOfIntervalLists(lists);
	//printf("after=%d\n",printLengthOfIntervalLists(listOfIntervalSets[0]));
}

int main(int argc, char *argv[]) {
	//Declarations
	struct listOfIntervalListsStruct** learnedIntervalSets = NULL;
	
	checkCreateLogDir();
	clock_t begin_process_input,end_process_input,begin_gen_tree,end_gen_tree;
	
	int targetPORV_id = 1;
        N = 0;              //Number of parts of the sequence 
        K = 0.0;                //Maximum delay seperation
        int i = 0;
        coveredSet = NULL;
        int repeatFlag = 0;
        int depthOrig = 0,depthDup=0;
        struct intervalListStruct* bcov = NULL;
        FILE* fcov = fopen("logs/cov.txt","w");
        int assertCountOld = 0;
        assertionList = NULL;
	
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	
	char log_filename[MAX_STR_LENGTH];
	sprintf(log_filename,"logs/log_%d%d%d%d%d%d.txt", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	char predlog_filename[MAX_STR_LENGTH];
	sprintf(predlog_filename,"logs/predLog_%d%d%d%d%d%d.txt", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	
	logFile = fopen(log_filename,"w");
	predLogFile = fopen(predlog_filename,"w");
	
	/*
		FILE* aFile = fopen("assertions.txt","w");
		fprintf(aFile,"ASSERTION LIST: \n\n");
		fclose(aFile);
	*/
	FILE* aFile=NULL;
	
	#ifdef COV_TEST
	while(1){
	#endif
		begin_process_input=clock();
		//-------Read the Config into an "inputConfig" structure---------
		predToPyin = processConfig(argc,argv);
		predToPyparse();
		printConfig(inputConfig);
		//--------------------------------------------------------------
		
		learnPredicates = getLearnType(inputConfig);
		printf("Learning Mode = %d\n",learnPredicates);
		
		N = inputConfig->N+1;
		K = inputConfig->K;

		traceCount = inputConfig->traceCount;
		strict = inputConfig->strict;
		
		listOfIntervalSets = (struct listOfIntervalListsStruct**)malloc(sizeof(struct listOfIntervalListsStruct*)*(inputConfig->traceCount));
		bzero(listOfIntervalSets,sizeof(struct listOfIntervalListsStruct*)*(inputConfig->traceCount));
		
		learnedIntervalSets = (struct listOfIntervalListsStruct**)malloc(sizeof(struct listOfIntervalListsStruct*)*(traceCount));
		bzero(learnedIntervalSets,sizeof(struct listOfIntervalListsStruct*)*(traceCount));
		
		traceLengths = (double*)malloc(sizeof(double)*inputConfig->traceCount);

		depthOrig = getDepth(inputConfig);
		depthDup = getExDepth(inputConfig);
		char* tempF;
		assertFileName = (char*)malloc(sizeof(char)*(strlen(argv[1])+50));
		strcpy(assertFileName,argv[1]);
		tempF = assertFileName;
		
		while(*tempF!='\0' && *tempF!='.'){
			tempF++;
		}
		
		if(*tempF == '.'){
			*tempF = '\0';
		}
		
		//---------------------------------Parsing Interval Sets--------------------------------------
		#ifdef MAIN_DEBUG
			fprintf(logFile,"[PSIMiner] Parsing Intervals.\n");fflush(logFile);
		#endif
		
		setbuf(stdout, NULL);
		
		struct identifier* traceFileNames = duplicateTraceFileNames(inputConfig);//Do not call duplicateIdentifiers because pointers one hierarchy in, created by other files, are not accessible from this file.
		struct identifier* id = traceFileNames;//inputConfig->traceFileNames;
		
		traceID = 0;
		if(!id){
			printf("\nNo traces to analyze! Gimme something to chew on :) \n");
			exit(0);//printIdentifierList(traceFileNames);
		}
	
		while(id){
			char* traceName = id->name;
			char* datFileName = convertToDatFileName(traceName);
			intervalListin =  fopen(datFileName,"r");
			if(intervalListin==0){
				printf("[ERROR] File \"%s\" does not exist.\n",inputConfig->intervalSetFileName);
				exit(0);
			}
			intervalListparse();//Read Interval Lists from Trace DAT File (generated by processing config and traces).
			if(listOfIntervalSets[traceID]==NULL){
				printf("\n Trace %d [%s] is empty\n",traceID, datFileName);
			} else {
				printListOfIntervalLists(listOfIntervalSets[traceID]);
			}
			//printIntervalList(intervalSet[traceID]->trueList);
			//printf("\n[%d]\n",traceID);
			///printListOfIntervalListsToFilePtr(intervalSet[traceID],stdout);fflush(stdout);
			traceLengths[traceID] = tl;
			traceID++;
			//printf("traceLength = %lf\n",traceLengths[traceID]);
			id = getNextIdentifier(id);
		}
		//exit(0);
		traceCount = inputConfig->traceCount;
// 		intervalListin =  fopen(inputConfig->intervalSetFileName,"r");
// 		if(intervalListin==0){
// 			printf("[ERROR] File \"%s\" does not exist.\n",inputConfig->intervalSetFileName);
// 			exit(0);
// 		}
//                 intervalListparse();
		//-------------------------------------------------------------------------------------------
		
                
//		#ifdef MAIN_DEBUG
//		fprintf(logFile,"[PSIMiner] intervalSet = %p\n",intervalSet);
//		fprintf(logFile,"[PSIMiner] From the Main Codebase:\n");fflush(logFile);
//		printListOfIntervalListsToFilePtr(intervalSet,logFile);
//		fflush(logFile);
//		#endif
                
                //Count the number of truth sets
		numberOfPORVs = countLists(listOfIntervalSets[0]);
                
                #ifdef MAIN_DEBUG
		fprintf(logFile,"[PSIMiner] Number of PORVs = %d\n",numberOfPORVs);
                #endif
		
		printPredicateList(predicateMap);
		

		end_process_input = clock();
		
		//----------------------------------Choose a target PORV-------------------------------------
                printf("NUMBER = %d\n",numberOfPORVs);
		//edited
        //target = getTarget(numberOfPORVs);        //REMOVE LATER
        //targetPORV_id = target;			//TODO: Merge this and the previous lines

		//#ifdef MAIN_DEBUG
		//fprintf(logFile,"[PSIMiner] Target PORV ID: P-%d\n",targetPORV_id);
		//#endif
                //--------------------------------------------------------------------------------------------
			
		sprintf(assertFileName,"%s-assertions.txt",assertFileName);
		//strcat(assertFileName,"-assert.txt");
		aFile = fopen(assertFileName,"w");
		fprintf(aFile,"ASSERTION LIST: \n\n");
		fclose(aFile);	
			
		
                //----Compute the traceLength - must be larger that largest constant in the interval set-----
                
		
		double traceLength = 0.0;
		for(i=0;i<traceCount;i++){
			traceLength += traceLengths[i];
		}
		
		totalTraceLength = traceLength;//tempTraceLength>traceLength?tempTraceLength:traceLength;
		setTraceLengthForConfig(inputConfig,totalTraceLength);
		printf("Total Trace Length = %lf\n",totalTraceLength);
		
		//-------------------Compute the Set of Intervals when FALSE for each PORV--------------------
                for(i=0;i<traceCount;i++)
			computeFalseLists(listOfIntervalSets[i],traceLengths[i]);
		//--------------------------------------------------------------------------------------------
		/*
		totalTrueLength = 0.0;
		for(i=0;i<traceCount;i++)
			totalTrueLength += lengthOfIntervalList(getListAtPosition(listOfIntervalSets[i],targetPORV_id)->trueList);
		totalFalseLength = totalTraceLength-totalTrueLength;
		   */
         
		//Prepare Decision Tree Root Node
		struct treeNode* root = createTreeNode(
												NULL,//Truth List
												listOfIntervalSets,learnedIntervalSets,//IntervalSets
												0,0,totalTraceLength,NULL,NULL);
			
		decisionTree = root; //TODO: This and the previous line may be combined.
		
		//printf("after=%d\n",printLengthOfIntervalLists(lists));  
                // At this point target, and numberOfPORVs+1 to numberOfPORVs+(N-1) are the target interval lists
               
		end_process_input = clock();	
		struct intervalListStruct* trueList = NULL;
		struct listOfIntervalListsStruct* targetList =  NULL;
		//edited (store targetList PORVID )

		struct expressionList* tempList = predicateMap->targetList;
		int ind=0,j;
		numTargets=0;
		while(tempList != NULL)
		{
		    numTargets++;
		    tempList = tempList->next;
		}
		
		double totalTrueLengthList[numTargets], totalFalseLengthList[numTargets];
		tempList = predicateMap->targetList;
		int targetPORVID[numTargets];
	
		struct listOfIntervalListsStruct*** pseudoTargetLists = (struct listOfIntervalListsStruct***)malloc(sizeof(struct listOfIntervalListsStruct**)*traceCount);
		for(int k=0;k<traceCount;k++) {
			pseudoTargetLists[k] = (struct listOfIntervalListsStruct**)malloc(sizeof(struct listOfIntervalListsStruct*)*numTargets);
		}
		struct treeNode* listTree[numTargets][numTargets];
		while(tempList != NULL)
		{
		    targetPORVID[ind++]=tempList->expr->disjunct->PORVID;
		    tempList = tempList->next;
		}

		for(i=0;i<numTargets;i++) 
		{
			totalTrueLengthList[i] = 0.0;
			for(int k=0;k<traceCount;k++)
				totalTrueLengthList[i] += lengthOfIntervalList(getListAtPosition(listOfIntervalSets[k],targetPORVID[i])->trueList);
			totalFalseLengthList[i] = totalTraceLength-totalTrueLengthList[i];
			//printf("%d\n",targetPORVID[i]);
			//prepareBackwardInfluenceTraces2(listOfIntervalSets,pseudoTargetLists,targetPORVID[i],N,K,strict,numberOfPORVs,numTargets);
		}
		/*printListOfIntervalLists(pseudoTargetLists[0][0]);
		prepareBackwardInfluenceTraces(listOfIntervalSets,6,N,K,strict);
		printf("0\n");
		printListOfIntervalLists(listOfIntervalSets[0]);*/
		/*struct treeNode* newroot = createTreeNode(
		                                                NULL,//Truth List
		                                                listOfIntervalSets,//IntervalSet
		                                                0,0,totalTraceLength,NULL,NULL); */
		int recreate_iterator = 0;
		FILE* dTree = NULL;
		for(recreate_iterator=0;recreate_iterator<RECREATE_COUNT;recreate_iterator++)
		{
			begin_gen_tree=clock();
			for(i=0;i<numTargets;i++)
			{
				totalTrueLength=totalTrueLengthList[i];
				totalFalseLength=totalFalseLengthList[i];
				struct treeNode* newroot = createTreeNode(
														NULL,//Truth List
														listOfIntervalSets,//IntervalSet
														learnedIntervalSets,
														0,0,totalTraceLength,NULL,NULL);
				printf("Total true Length = %lf\n",totalTrueLength);
				prepareBackwardInfluenceTraces(listOfIntervalSets,targetPORVID[i],N,K,strict);
				/*struct truthAssignmentListStruct* truthList = createAss();
				printTruthList(truthList);
				struct intervalListStruct** endMatchIntervalList = endMatchesForPrefix(truthList);
				for(int i=0;i<traceCount;i++){
					printf("i=%d\n",i);
					printIntervalList(endMatchIntervalList[i]);
				}
				//exit(0);
				int mininmumBucketID = getSmallestBucketID(truthList);
				int targetID = mininmumBucketID==0?7:(numberOfPORVs+mininmumBucketID);
				double trueMean, falseMean, Htrue, Hfalse,mean, error;
				double intervalLength = 0.0;
				if(endMatchIntervalList){
					intervalLength = lengthOfIntervalLists(endMatchIntervalList);
				}
				printf("IL=%lf\n",intervalLength);
				int dominantTruth;
				trueMean = computeMean(listOfIntervalSets,targetID,intervalLength,endMatchIntervalList);
				falseMean = computeFalseMean(listOfIntervalSets,targetID,intervalLength,endMatchIntervalList);
				Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList);
				Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList);
				dominantTruth = Htrue<=Hfalse?1:0;//error(trueMean)<error(falseMean)?1:0;
				mean = dominantTruth==0?falseMean:trueMean;
				error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList));
				printf("mean=%lf\n",mean);
				exit(0);*/
				prepareRoot(newroot,listOfIntervalSets,learnedIntervalSets,pseudoTargetLists,targetPORVID[i],numberOfPORVs,numTargets,N);
				printf("[PSIMiner] spid=%d\n",newroot->splittingPredicate_id);
				//exit(0);
				printf("[PSIMiner] Calling PSI-Miner for target= %d\n",targetPORVID[i]);
				
				FILE* aFile = fopen(assertFileName,"a");
				char *targetName = getPredicateName(targetPORVID[i]);
				char newTargetName[MAX_STR_LENGTH]; 
				if(targetName){
					sprintf(newTargetName,"%s",targetName);
				} else {
					sprintf(newTargetName,"P%d",targetPORVID[i]);
				}
				
				fprintf(aFile,"\n****************** TARGET [%s] ********************\n",newTargetName);
				fclose(aFile);
				amsMine(newroot,pseudoTargetLists,targetPORVID[i],numberOfPORVs,numTargets,N,depthOrig,targetPORVID[i],learnedIntervalSets);
				printf("[PSIMiner] Completed PSI-Mining for target= %d\n",targetPORVID[i]);
				
				dTree = getDTreeFilePtrWithName(targetName);
				fprintf(dTree,"___________________________________________________\n\n                   DECISION TREE\n                   [%s]\n________________________________________________________\n\n",targetName);
				printTreeToFilePtr(newroot,dTree,targetPORVID[i]);
				fflush(dTree); fclose(dTree);
				//exit(0);
				printTruthList(newroot->right->truthList);
				for(int k=0;k<traceCount;k++)
				{
					//printf("before=%d for trace=%d\n",printLengthOfIntervalLists(listOfIntervalSets[k]),k);
					deleteExtraLists(listOfIntervalSets[k],numberOfPORVs);
					//printf("after=%d for trace=%d\n",printLengthOfIntervalLists(listOfIntervalSets[k]),k);
				}
				//newRoot-> = (struct treeNode**)malloc(sizeof(struct treeNode*)*(numTargets));
				listTree[i][i] = newroot;
				//printTruthList(newroot);
				for(j=0;j<numTargets;j++)
				{
					if(j != i)
					{
						struct treeNode* duproot;
						prepareBackwardInfluenceTraces(listOfIntervalSets,targetPORVID[j],N,K,strict);
						//newroot->originalToDup = (struct treeNode**)malloc(sizeof(struct treeNode*)*(numTargets));
						//printf("1\n");
						duproot = duplicateTree(newroot, j,learnedIntervalSets); //maintain correspondence here
						totalTrueLength=totalTrueLengthList[j];
						totalFalseLength=totalFalseLengthList[j];
						//printf("1\n");
						listTree[i][j] = duproot;
						//printf("1\n");
							//struct intervalListStruct** endMatchIntervalList = endMatchesForPrefix(root->truthList);
						/*for(int k=0;k<traceCount;k++){
							printIntervalList(endMatchIntervalList[k]);
							}*/
						printf("[PSIMiner] ------------------------ [ Calling amsMine2 for target= %d ] --------------------------\n",targetPORVID[j]);
						
						FILE* aFile = fopen(assertFileName,"a");
						
						char *targetName = getPredicateName(targetPORVID[j]);
						char newTargetName[MAX_STR_LENGTH]; 
						if(targetName){
							sprintf(newTargetName,"%s",targetName);
						} else {
							sprintf(newTargetName,"P%d",targetPORVID[j]);
						}

						fprintf(aFile,"\n******** RELATED ASSERTIONS FOR TARGET [%s] ********\n",newTargetName);
						fclose(aFile);
					
						amsMine2(newroot, duproot,pseudoTargetLists, targetPORVID[j], numberOfPORVs,numTargets, N, depthOrig, depthDup,1,listOfIntervalSets,learnedIntervalSets);
						for(int k=0;k<traceCount;k++)
						{
							//printf("before=%d for trace=%d\n",printLengthOfIntervalLists(listOfIntervalSets[k]),k);
							deleteExtraLists(listOfIntervalSets[k],numberOfPORVs);
							//printf("after=%d for trace=%d\n",printLengthOfIntervalLists(listOfIntervalSets[k]),k);
						}
						//exit(0);
					}
				}
				purgeTruthListForTreeNode(newroot); 
			}
			end_gen_tree=clock();
		}

                /*
		
                printListOfIntervalLists(coveredSet);
                struct listOfIntervalListsStruct* cov = getListAtPosition(coveredSet,target);
                struct intervalListStruct* covList = unionIntervalLists(cov->falseList,cov->trueList);
                printf("\nCOV = %lf\n",lengthOfIntervalList(covList)/traceLength);
                
                if(!bcov)
                        bcov = flattenIntervalSet(coveredSet);
                else 
                        bcov = unionIntervalLists(bcov,flattenIntervalSet(coveredSet));
                
                printf("\nCUMULATIVE COVERAGE \n");
                printIntervalList(bcov);
                printf("\nBCOV = %lf\n",lengthOfIntervalList(bcov)/traceLength);
                */
		
		
                fflush(stdout);
                fflush(stdin);
                
                //printListOfIntervalLists(coveredSet);
                //printf("\nCoverage List \n");
                //printIntervalList(coveredList);
// 		fprintf(fcov,"\nCOVERAGE:\n");
//                 fprintf(fcov,"\nAssertion Length (N) = %d\n",N);
//                 fprintf(fcov,"\nResolution (K) = %lf\n",K);
//                 fprintf(fcov,"\nTarget (Consequent) = %d\n",target);
//                 fprintf(fcov,"\nCoverage List :\n");
//                 printIntervalListToFilePtr(coveredList,fcov);
//                 fprintf(fcov,"Coverage = %lf\n", lengthOfIntervalList(coveredList)/totalTraceLength);
//                 fprintf(fcov,"\nCumulative Coverage = %lf\n", lengthOfIntervalList(coveredList)/totalTraceLength);
//                 fprintf(fcov,"\nNew Assertions Found = %d\n", assertCount-assertCountOld);
//                 fprintf(fcov,"\nCumulative Assertions Found = %d\n", assertCount);
//                 assertCountOld = assertCount;
                fflush(fcov);
                
        #ifdef COV_TEST
                pause();
                
                printf("N = ");
                scanf("%d",&N);
                printf("K = ");
                scanf("%lf",&K);
                
                repeatFlag = 1;
        
        }
        #endif
        fclose(fcov);
	fflush(logFile);
	fclose(logFile);
	fflush(predLogFile);
	fclose(predLogFile);
	
	fprintf(stdout,"\nList of Assertions - (from internal data-structures)\n");
	struct assertionStruct* temp = allAssertions;
	FILE* tempAssertFile = fopen("psi-properties.txt","w");
	while(temp){		
		if(((int)strlen(temp->assertion))>0){
			fprintf(stdout,"\n%s",temp->assertion);
			fprintf(stdout,"SUPPORT = %lf\n",temp->support);
			fprintf(stdout,"CORRELATION = %lf\n",temp->correlation);
			
			fprintf(tempAssertFile,"\n%s",temp->assertion);
			fprintf(tempAssertFile,"SUPPORT = %lf\n",temp->support);
			fprintf(tempAssertFile,"CORRELATION = %lf\n",temp->correlation);
		}
		temp = temp->next;
	}
	fprintf(tempAssertFile,"\n Process Pseudo-Targets = %lf\n",(double)(end_process_input - begin_process_input)/CLOCKS_PER_SEC);
	fprintf(tempAssertFile,"\n Process Decision Tree = %lf\n",(double)(end_gen_tree - begin_gen_tree)/CLOCKS_PER_SEC);
	printf("\nAssertions are listed in the file \"%s\"\nCoverage information is in the file \"cov.txt\"\n",assertFileName);
	return 0;
}
