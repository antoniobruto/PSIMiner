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

#include "amsMinerStructs.h"
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
printPredicateDetailListToFilePtr(details,stdout);
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
double K;
int N;
int targetPORV_id;
int strict = 0;				//TODO: Implement strict pseudo targets
double totalTraceLength = 0.0;
double totalTrueLength = 0.0;
double totalFalseLength = 0.0;
int separationType = 1;

double correlationThreshold = 1.0;
double supportThreshold = 0.001;

int targetBias = 0;			/*	
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
int main(int argc, char *argv[]) {
	checkCreateLogDir();
	int target = 1;
        N = 0;              //Number of parts of the sequence 
        K = 0.0;                //Maximum delay seperation
        int i = 0;
        coveredSet = NULL;
        int repeatFlag = 0;
        int depth = 0;
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
        FILE* aFile = getDTreeFilePtr();
        fprintf(aFile,"___________________________________________________\n\n                   DECISION TREE\n________________________________________________________\n\n");
        fclose(aFile);
        
        #ifdef COV_TEST
        while(1){
        #endif
		//-------Read the Config into an "inputConfig" structure---------
		predToPyin = processConfig(argc,argv);
		printf("HERE\n");fflush(stdout);
		predToPyparse();
		printf("THERE\n");fflush(stdout);
		printConfig(inputConfig);
		
		//--------------------------------------------------------------
		
		learnPredicates = getLearnType(inputConfig);
		printf("Learning Mode = %d\n",learnPredicates);
		
		N = inputConfig->N+1;
		K = inputConfig->K;
		
		listOfIntervalSets = (struct listOfIntervalListsStruct**)malloc(sizeof(struct listOfIntervalListsStruct*)*(inputConfig->traceCount));
		bzero(listOfIntervalSets,sizeof(struct listOfIntervalListsStruct*)*(inputConfig->traceCount));
		printf("HERE\n");fflush(stdout);
		traceLengths = (double*)malloc(sizeof(double)*inputConfig->traceCount);
		
		depth = getDepth(inputConfig);
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
		fprintf(logFile,"[AMSMiner] Parsing Intervals.\n");fflush(logFile);
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
			intervalListparse();
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
//		fprintf(logFile,"[AMSMiner] intervalSet = %p\n",intervalSet);
//		fprintf(logFile,"[AMSMiner] From the Main Codebase:\n");fflush(logFile);
//		printListOfIntervalListsToFilePtr(intervalSet,logFile);
//		fflush(logFile);
//		#endif
                
                //Count the number of truth sets
		numberOfPORVs = countLists(listOfIntervalSets[0]);
                
                #ifdef MAIN_DEBUG
		fprintf(logFile,"[AMSMiner] Number of PORVs = %d\n",numberOfPORVs);
                #endif
		
		printPredicateList(predicateMap);
		
		//----------------------------------Choose a target PORV-------------------------------------
                printf("NUMBER = %d\n",numberOfPORVs);
                target = getTarget(numberOfPORVs);        //REMOVE LATER
                targetPORV_id = target;			//TODO: Merge this and the previous lines
                
		#ifdef MAIN_DEBUG
		fprintf(logFile,"[AMSMiner] Target PORV ID: P-%d\n",target);
		#endif
                //--------------------------------------------------------------------------------------------
			
		sprintf(assertFileName,"%s-assert-%d.txt",assertFileName,targetPORV_id);
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
		
		totalTrueLength = 0.0;
		for(i=0;i<traceCount;i++)
			totalTrueLength += lengthOfIntervalList(getListAtPosition(listOfIntervalSets[i],targetPORV_id)->trueList);
		totalFalseLength = totalTraceLength-totalTrueLength;
		    
                //Prepare Decision Tree Root Node
                struct treeNode* root = createTreeNode(
                                                        NULL,//Truth List
                                                        listOfIntervalSets,//IntervalSet
                                                        0,0,totalTraceLength,NULL,NULL);

                decisionTree = root; //TODO: This and the previous line may be combined.
                
                //Compute Additional Interval Lists for Sequence Generation: Backward Influence for the Target
                //Compute Pseudo-Targets
                //struct listOfIntervalListsStruct* backwardInfluence = prepareBackwardInfluenceTraces(listOfIntervalSets,target,N,K,strict);
                prepareBackwardInfluenceTraces(listOfIntervalSets,target,N,K,strict);        
                // At this point target, and numberOfPORVs+1 to numberOfPORVs+(N-1) are the target interval lists
                
		printTreeNodeToFilePtr(root,logFile);
		fflush(logFile);
		
		int recreate_iterator = 0;
		for(recreate_iterator=0;recreate_iterator<RECREATE_COUNT;recreate_iterator++){
			prepareRoot(root,listOfIntervalSets,target,numberOfPORVs,N);                
			amsMine(root,target,numberOfPORVs,N,depth);
			printTree(root);
			purgeTruthListForTreeNode(root);
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
	
	struct assertionStruct* temp = allAssertions;
	while(temp){
		
		if(((int)strlen(temp->assertion))>0){
			fprintf(stdout,"\n%s",temp->assertion);
			fprintf(stdout,"SUPPORT = %lf\n",temp->support);
			fprintf(stdout,"CORRELATION = %lf\n",temp->correlation);
		}
		temp = temp->next;
	}
	
        printf("\nAssertions are listed in the file \"%s\"\nCoverage information is in the file \"cov.txt\"\n",assertFileName);
	return 0;
	
}
