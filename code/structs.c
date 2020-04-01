/*****************************************************************
 *                      CODE WRITTEN BY                          *
 *                                                               *
 *             Antonio Anastasio Bruto da Costa                  *
 *     Formal Method Laboratory, Dept. of CSE, IIT Kharagpur     *
 *                                                               *
 *****************************************************************/

#ifndef MAX_STR_LENGTH
	#define MAX_STR_LENGTH 10240
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <float.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/time.h> /* for time */

#include "structs.h"

#define epsilon 0.0001

int timeCol = 0;
/*-----------------------Operator Maps-------------------------*/

char* operatorMap(int op){	
	char *p=(char *)malloc(sizeof(char)*3);
	switch(op){
		case 0: sprintf(p,"==");break;
		case 1: sprintf(p,"<=");break;
		case 2: sprintf(p,":=");break;
		case 3: sprintf(p,">=");break;
		case 4: sprintf(p,"<");break;
		case 5: sprintf(p,">");break;
		default:sprintf(p,"==");break;
		
	}
	
	return p;
}

/*------------------------CONFIG------------------------------*/
struct config* createConfig(){
	struct config* inputConfigTemp = (struct config*)malloc(sizeof(struct config));
	inputConfigTemp->K = 0;
	inputConfigTemp->N = 0;
	inputConfigTemp->maxTreeDepth = 1;
	inputConfigTemp->tmin = 10;
	inputConfigTemp->tmax = 20;
	inputConfigTemp->bestGainCount = 5;
	inputConfigTemp->learnType = 0;
	inputConfigTemp->traceFileNames = NULL;
	bzero(inputConfigTemp->traceFileName,sizeof(char)*MAX_STR_LENGTH);
	bzero(inputConfigTemp->intervalSetFileName,sizeof(char)*MAX_STR_LENGTH);
	return inputConfigTemp;
}

void printConfig(struct config* inputConfig){
	if(inputConfig){
		printf("----------------------CONFIG------------------------\n");
		printf("Sequence Length (N) = %d\n",inputConfig->N);
		printf("Delay Resolution(K) = %lf\n",inputConfig->K);
		printf("Decision Tree Depth = %d\n",inputConfig->maxTreeDepth);
		printf("Predicates # Learnd = %d\n",inputConfig->bestGainCount);
		printf("Max Temperature     = %lf\n",inputConfig->tmax);
		printf("Min Temperature     = %lf\n",inputConfig->tmin);
		printf("Trace Count         = %d\n",inputConfig->traceCount);
		printf("Trace File Names    = "); printIdentifierList(inputConfig->traceFileNames);printf("\n");
		printf("Data File Name      = %s\n",inputConfig->intervalSetFileName);
		printf("Learn Type          = %d\n",inputConfig->learnType);
		printf("----------------------------------------------------\n");
	}
}

struct config* redirectConfig(struct config* inputConfig){
	printf("Data File Name      = %s\n",inputConfig->intervalSetFileName);
	return inputConfig;
};

struct identifier* duplicateTraceFileNames(struct config* inputConfig){
	if(inputConfig && inputConfig->traceFileNames){
		return duplicateIdentifierList(inputConfig->traceFileNames);
	}
}

int getDepth(struct config* inputConfig){
	if(inputConfig){
		return inputConfig->maxTreeDepth;
	}
	return 0;
}


int getLearnType(struct config* inputConfig){
	if(inputConfig){
		return inputConfig->learnType;
	}
	return 0;
}


void printConfigToFilePtr(struct config* configuration, FILE* fp){
	if(configuration && fp){
		fprintf(fp,"dataset_file = %s\n",configuration->traceFileName);
		fprintf(fp,"m = %d\n",configuration->bestGainCount);
		fprintf(fp,"k = %lf\n",configuration->K);
		fprintf(fp,"tmax = %d\n",(int)configuration->tmax);
		fprintf(fp,"tmin = %d\n",(int)configuration->tmin);
		fprintf(fp,"trace_length = %lf\n",configuration->traceLength);
		fprintf(fp,"n = %d\n",configuration->N);
	}
}

void setTraceLengthForConfig(struct config* configuration, double length){
	if(configuration){
		configuration->traceLength = length;
	}
}
/*-----------------------CONDITION----------------------------*/

struct condition* createCondition(char *LHS, char* RHS, int op){
	#ifdef DEBUG_ON 
		printf("[createCondition] STARTED\n");
	#endif
	struct condition* cond = (struct condition*)malloc(sizeof(struct condition));
		
	if(LHS!=NULL)
		strcpy(cond->LHS,LHS);
	if(RHS!=NULL)
		strcpy(cond->RHS,RHS);
	cond->op = op;
	cond->next = NULL;
	#ifdef DEBUG_ON 
		printf("[createCondition] CRETED NODE\n");
	#endif
	return cond;
}

struct condition* addConditionToList(struct condition* root, struct condition* cond){
	#ifdef DEBUG_ON 
		printf("[addConditionToList] STARTED\n");	
		printf("ROOT = %p ; cond = %p\n",root,cond);
	#endif
	struct condition* temp = root;
		
	if(root!=NULL){
		#ifdef DEBUG_ON 
			printf("[addConditionToList] CONDITION LIST NOT EMPTY\n");
		#endif
		
		while(temp->next!=NULL){
			if(temp == cond){
				return root;
			}
			temp=temp->next;
		}
		if(temp == cond){
			return root;
		}
		
		#ifdef DEBUG_ON 
			printf("[addConditionToList] ADDING TO CONDITION LIST\n");
			printf("temp = %p ; cond = %p\n",temp ,cond);
		#endif
			
		temp->next = cond;
	} else {
		#ifdef DEBUG_ON 
			printf("[addConditionToList] CONDITION LIST EMPTY\n");
			printf("[addConditionToList] ADDING TO CONDITION LIST\n");
		#endif
		
		root = cond;
		#ifdef DEBUG_ON 
			printf("[addConditionToList] ROOT CHANGED\n");
		#endif
		
	}
	#ifdef DEBUG_ON 
		printf("[addConditionToList] ADDED CONDITION\n");
	#endif
	
	return root;
}

struct condition* addToConditionList(struct condition* root, char *LHS, char* RHS, int op){
	#ifdef DEBUG_ON 
	printf("[addToConditionList] STARTED\n");
	#endif
	
	struct condition* cond = (struct condition*)malloc(sizeof(struct condition));
	struct condition* temp = root;
	
	strcpy(cond->LHS,LHS);
	strcpy(cond->RHS,RHS);
	cond->op = op;
	cond->next = NULL;
	#ifdef DEBUG_ON 
	printf("[addToConditionList] CRETED NODE\n");
	#endif
	
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
		printf("[addToConditionList] CONDITION LIST NOT EMPTY\n");
		#endif
		
		while(temp->next!=NULL){
			temp=temp->next;
		}
		#ifdef DEBUG_ON 
		printf("[addToConditionList] ADDING TO CONDITION LIST\n");
		#endif
		
		temp->next = cond;
		
	} else {
		#ifdef DEBUG_ON 
		printf("[addToConditionList] CONDITION LIST EMPTY\n");
		printf("[addToConditionList] ADDING TO CONDITION LIST\n");
		#endif
		
		root = cond;
		
		#ifdef DEBUG_ON 
		printf("[addToConditionList] ROOT CHANGED\n");
		printf("[addToConditionList] ROOT = %p, NEXT = %p\n",root,root->next);
		#endif
		
	}
	#ifdef DEBUG_ON 
	printf("[addToConditionList] ADDED CONDITION %s %d %s\n",LHS,op,RHS);
	#endif
	
	return root;
}

struct condition* duplicateConditionList(struct condition* root){
	#ifdef DEBUG_ON 
	printf("[duplicateCondition] STARTED\n");
	#endif
	
	struct condition* temp = root;
	struct condition* newRoot = NULL;
	#ifdef DEBUG_ON 
	printf("[root = %p] %p \n",root,root!=NULL?root->next:NULL);
	#endif
	
	while(temp!=NULL){
		newRoot = addToConditionList(newRoot, temp->LHS, temp->RHS, temp->op);
		#ifdef DEBUG_ON 
		printf("[duplicateCondition] DUPLICATING NEXT CONDITION [%p] %s %s %d [%p]\n",temp,temp->LHS,temp->RHS,temp->op,temp->next);
		printf("[root = %p]\n",root);
		//if(temp == temp->next) temp->next=NULL;//exit(0);
		#endif
		
		temp = temp->next;
	}
	#ifdef DEBUG_ON 
	printf("[duplicateCondition] COMPLETED\n");
	#endif
	
	return newRoot;
}

void deleteConditionList(struct condition* node){
	if(node){
		if(node->next){
			deleteConditionList(node->next);
		}
		free(node);
	}
}

void printCondition(struct condition* cond){
	if(cond!=NULL){
		//printf("[%p] %s %s %s ",cond,cond->LHS,operatorMap(cond->op),cond->RHS);	
		printf("%s %s %s ",cond->LHS,operatorMap(cond->op),cond->RHS);	
	}
}


void printConditionToFilePtr(struct condition* cond,FILE* fp){
	if(cond!=NULL && fp){
		//printf("[%p] %s %s %s ",cond,cond->LHS,operatorMap(cond->op),cond->RHS);	
		fprintf(fp,"%s %s %s ",cond->LHS,operatorMap(cond->op),cond->RHS);	
	}
}


void printConditionToString(struct condition* cond, char* fp){
	if(cond!=NULL && fp){
		//printf("[%p] %s %s %s ",cond,cond->LHS,operatorMap(cond->op),cond->RHS);	
		char* temp=(char*)malloc(sizeof(char)*MAX_STR_LENGTH);
		sprintf(temp,"%s %s %s ",cond->LHS,operatorMap(cond->op),cond->RHS);strcat(fp,temp);
	}
}


void printConditionList(struct condition* cond){
	printf("\n\n---------------CONDITION LIST-----------------\n");
	while(cond!=NULL){
		printf("\n");
		printCondition(cond);
		cond = cond->next;
	}
	printf("\n\n----------------------------------------------\n");
}

void printConditionListToFilePtr(struct condition* cond, FILE* fp){
	printf("\n\n---------------CONDITION LIST-----------------\n");
	while(cond!=NULL){
		fprintf(fp,"\n");
		printConditionToFilePtr(cond,fp);
		cond = cond->next;
	}
	printf("\n\n----------------------------------------------\n");
}

struct condition* reverseConditionList(struct condition* root){

	if(root==NULL){
		#ifdef DEBUG_ON 
			printf("[reverseConditionList] STARTED AND ENDED - NULL ROOT\n");
		#endif
	
		return NULL;
	}
	#ifdef DEBUG_ON 
		printf("[reverseConditionList] STARTED\n");
	#endif
	struct condition *prev, *current, *next;
	
	current = root;
	next=root->next;
	root->next = NULL;
	
	while(next!=NULL){
		prev = current;
		current = next;
		next = current->next;
		current->next = prev;
	}
	#ifdef DEBUG_ON 
		printf("[reverseConditionList] ENDED - returning current = %d\n",current);
	#endif
	
	return current;
}


/*
 * Compares if a condition has a specified LHS with a specified RHS
 */
int conditionCompare(struct condition* cond, char* LHS, char* RHS){
        #ifdef DEBUG_ON 
        printf("[conditionCompare] STARTED\n");
        #endif

        if(cond){
                if(LHS){
                        if(strcmp(LHS,cond->LHS)==0){
                                if(RHS){
                                        if(strcmp(RHS,cond->RHS)==0)
                                                return 1;
                                }
                        }
                }
        }
        
        return 0;
        
        #ifdef DEBUG_ON 
        printf("[conditionCompare] STARTED\n");
        #endif
}


/*
 * Checks if a given LHS,RHS pair is present in the list of conditions
 */
int inConditionList(struct condition* list, char* LHS, char* RHS){
        #ifdef DEBUG_ON 
        printf("[inConditionList] STARTED\n");
        #endif

        while(list){
                if(conditionCompare(list,LHS,RHS)){
                        return 1;
                }
                list = list->next;
        }
        
        return 0;
        
        #ifdef DEBUG_ON 
        printf("[inConditionList] STARTED\n");
        #endif
}

int removeConditionFromList(struct condition** root, char* LHS){
        #ifdef DEBUG_ON 
        printf("[removeConditionFromList] STARTED\n");
        #endif
        
        if(*root && LHS){
                //printConditionList(*root);
                struct condition* prev = *root;
                struct condition* curr = *root;
                while(curr){
                        //printf("Comparing [%s] with [%s]\n",curr->LHS,LHS);
                        if(strncmp(curr->LHS,LHS,strlen(LHS))==0){
                                //printf("FOUND\n");
                                if(curr == *root){
                                        //printf("CURRENT IS ROOT\n");
                                        *root = curr->next;
                                        free(curr);
                                } else {
                                        prev->next = curr->next;
                                        free(curr);
                                }
                                break;
                        }
                        prev = curr;
                        curr = curr->next;
                }
                
                //printf("\nNow it is: \n");
                //printConditionList(*root);
        }
        
        #ifdef DEBUG_ON 
        printf("[removeConditionFromList] ENDED\n");
        #endif
}

/*-----------------------IDENTIFIER-----------------------------*/
struct identifier* createIdentifier(char *id){
	#ifdef DEBUG_ON 
	printf("[createIdentifier] STARTED\n");
	#endif
	
	struct identifier* ID = (struct identifier*)malloc(sizeof(struct identifier));
	ID->next = NULL;
	strcpy(ID->name,id);
	#ifdef DEBUG_ON 
	printf("[createIdentifier] CRETED NODE\n");
	#endif
	
	
	return ID;
}

struct identifier* addIdentifierToList(struct identifier* root, struct identifier* ID){
	#ifdef DEBUG_ON 
	printf("[addIdentifierToList] STARTED\n");
		#endif
	
	struct identifier* temp = root;
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
		printf("[addIdentifierToList] ID LIST NOT EMPTY\n");
		#endif
		
		while(temp->next!=NULL){
			temp=temp->next;
		}
		#ifdef DEBUG_ON 
		printf("[addIdentifierToList] ADDING TO ID LIST\n");
		#endif
		
		temp->next = ID;
	} else {
		#ifdef DEBUG_ON 
		printf("[addIdentifierToList] ID LIST EMPTY\n");
		printf("[addIdentifierToList] ADDING TO ID LIST\n");
		#endif
		
		root = ID;
		#ifdef DEBUG_ON 
		printf("[addIdentifierToList] ROOT CHANGED: ROOT->NAME = %s\n",root->name);
		#endif
		
	}
	#ifdef DEBUG_ON 
	printf("[addIdentifierToList] ADDED IDENTIFIER [%s]\n",ID->name);
	#endif
	
	return root;
}

struct identifier* addToIdentifierList(struct identifier* root, char *id){
	#ifdef DEBUG_ON 
	printf("[addToIdentifierList] STARTED\n");
	#endif
	
	struct identifier* ID = (struct identifier*)malloc(sizeof(struct identifier));
	struct identifier* temp = root;
	ID->next = NULL;
	strcpy(ID->name,id);
	#ifdef DEBUG_ON 
	printf("[addToIdentifierList] CRETED NODE\n");
	#endif
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
		printf("[addToIdentifierList] ID LIST NOT EMPTY\n");
		#endif
		
		while(temp->next!=NULL){
			temp=temp->next;
		}
		#ifdef DEBUG_ON 
		printf("[addToIdentifierList] ADDING TO ID LIST\n");
		#endif
		
		temp->next = ID;
	} else {
		#ifdef DEBUG_ON 
		printf("[addToIdentifierList] ID LIST EMPTY\n");
		printf("[addToIdentifierList] ADDING TO ID LIST\n");
		#endif
		
		root = ID;
		#ifdef DEBUG_ON 
		printf("[addToIdentifierList] ROOT CHANGED: ROOT->NAME = %s\n",root->name);
		#endif
		
	}
	#ifdef DEBUG_ON 
	printf("[addToIdentifierList] ADDED IDENTIFIER [%s]\n",id);
	#endif
	
	return root;
}

int getIdentifierID(struct identifier* list, char *name){
	struct identifier* temp = list;
	int count=0;
	while(temp!=NULL){
		count++;
		if(strcmp(temp->name,name)==0){
			return count;
		}
		temp=temp->next;
	}	
	return 0;
}

struct identifier* duplicateIdentifierList(struct identifier* root){
	#ifdef DEBUG_ON 
	printf("[duplicateIdentifierList] STARTED\n");
	#endif
	
	struct identifier* temp = root;
	struct identifier* newRoot = NULL;
	while(temp!=NULL){
		newRoot = addToIdentifierList(newRoot, temp->name);
		temp = temp->next;
	}
	#ifdef DEBUG_ON 
	printf("[duplicateIdentifierList] COMPLETED\n");
	#endif
	
	return newRoot;
}

void freeIdentifierList(struct identifier* id){
	if(id!=NULL){
		freeIdentifierList(id->next);
		free(id);
	}
}

char* containsIdentifier(struct identifier* list1, struct identifier* list2){
	struct identifier* temp = list1;
	while(temp){
		if(getIdentifierID(list2,temp->name)==0){
			char* err = (char*)malloc(sizeof(char)*MAX_STR_LENGTH);
			sprintf(err,"Identifier [%s] not declared in the the signature of the module.\n",temp->name);
			return err;			
		}
		temp = temp->next;
	}
	return NULL;
}


void printIdentifier(struct identifier* id){
	if(id!=NULL){
		//printf("[%p] %s %s %s ",cond,cond->LHS,operatorMap(cond->op),cond->RHS);
		printf("Addrs: %p\n",id);
		printf("Name: %s\n",id->name);	
		printf("Next: %p\n\n",id->next);
	}
}

void printIdentifierList(struct identifier* id){
	printf("\n\n---------------IDENTIFIER LIST-----------------\n");
	while(id!=NULL){	
		printIdentifier(id);	
		//printf("%s, ",id->name);
		id = id->next;
	}
	printf("----------------------------------------------\n");
}

struct identifier* getIdentifierWithID(struct identifier* list, int id){
        while(list){
                if(id==1) return list;
                id--;
                list = list->next;
        }
        return NULL;
}


int getIdentifierCol(struct identifier* list, int id){
        while(list){
                if(id==1) return list->col;
                id--;
                list = list->next;
        }
        return -1;
}

int getIdentifierTimeCol(struct identifier* list, int id){
        while(list){
                if(id==1) return list->timeCol;
                id--;
                list = list->next;
        }
        return -1;
}

struct identifier* getNextIdentifier(struct identifier* temp){
	if(temp){
		return temp->next;
	}
}

/*-----------------------MISCELLANEOUS-----------------------------*/

int charInList(char *c, char *list, int count){
	int i=0;
	for(i=0;i<count;i++){
		if(*c==list[i])
			return 1;
	}
	return 0;
	
}


/*-----------------------Event-------- ---------------------*/

struct eventType* createEvent(int type, struct PORV* porv){
	#ifdef DEBUG_ON 
		printf("[createEvent] STARTED\n");
	#endif
	struct eventType* event = (struct eventType*)malloc(sizeof(struct eventType));
	
	event->type = type;
	event->porv = porv;
	#ifdef DEBUG_ON 
		printf("[createEvent] CRETED NODE\n");
	#endif
		
	return event;
}


/*-----------------------PORV-------- ---------------------*/

struct PORV* createPORV(struct condition* cond, int id, int atomID){
	#ifdef DEBUG_ON 
		printf("[createPORV] STARTED\n");
	#endif
	struct PORV* porv = (struct PORV*)malloc(sizeof(struct PORV));
	
	porv->porv = cond;
	porv->id = id;
        porv->atomID = atomID;
	porv->next = NULL;
	#ifdef DEBUG_ON 
		printf("[createPORV] CRETED NODE\n");
	#endif
	
	return porv;
}

struct PORV* addPORVToList(struct PORV* root, struct PORV* porv){
	#ifdef DEBUG_ON 
		printf("[addPORVToList] STARTED\n");
	#endif
	
	struct PORV* temp = root;
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
			printf("[addPORVToList] PORV LIST NOT EMPTY\n");
		#endif
		while(temp->next!=NULL){
			temp=temp->next;
		}
		#ifdef DEBUG_ON 
			printf("[addPORVToList] ADDING TO PORV LIST\n");
		#endif
		temp->next = porv;
	} else {
		#ifdef DEBUG_ON 
			printf("[addPORVToList] PORV LIST EMPTY\n");
			printf("[addPORVToList] ADDING TO PORV LIST\n");
		#endif
		root = porv;
		#ifdef DEBUG_ON 
			printf("[addPORVToList] ROOT CHANGED\n");
		#endif
		
	}
	#ifdef DEBUG_ON 
		printf("[addPORVToList] ADDED PORV\n");
	#endif
	return root;
}

struct PORV* duplicatePORVList(struct PORV* root){
	#ifdef DEBUG_ON 
		printf("[duplicatePORVList] STARTED\n");
	#endif
	
	struct PORV* temp = root;
	struct PORV* newRoot = NULL;
	#ifdef DEBUG_ON 
		printf("[root = %p] %p \n",root,root->next);
	#endif
	
	while(temp!=NULL){
		struct PORV* newPORV = (struct PORV*)malloc(sizeof(struct PORV));
		newPORV->id = temp->id;
		newPORV->next = NULL;
		newPORV->porv = duplicateConditionList(temp->porv);
		
		newRoot = addPORVToList (newRoot, newPORV);
		#ifdef DEBUG_ON 
			printf("[duplicatePORVList] DUPLICATING NEXT PORV [%p]\n",temp->next);
			printf("[root = %p]\n",root);
		//if(temp == temp->next) temp->next=NULL;//exit(0);
		#endif
		
		temp = temp->next;
	}
	#ifdef DEBUG_ON 
		printf("[duplicatePORVList] COMPLETED\n");
	#endif
	
	return newRoot;
}

int getPORVCount(struct PORV* root){
        int N=0;
        while(root){
                N++;
                root = root->next;                
        }
        return N;
}

struct PORV* getPORV(struct PORV* root, int id){
	while(root){
		if(root->id == id)
			return root;			
		root = root->next;
	}
	return NULL;
}

void deletePORVs(struct PORV* node){
	if(node){
		if(node->next){
			deletePORVs(node->next);
		} 
		deleteConditionList(node->porv);
		free(node);
	}
}

//-------------------------- File ----------------------------------
struct file* createFile(){
        struct file* tempFile = (struct file*)malloc(sizeof(struct file));
	tempFile->predicates = NULL;
	return tempFile;
}

/*--------------------------- Print Methods ------------------------ */

char* featureOperatorMap(int op){	
	char *p=(char *)malloc(sizeof(char)*3);
	switch(op){
		case 0: sprintf(p,"==");break;
		case 1: sprintf(p,"<=");break;
		case 2: sprintf(p,"=");break;
		case 3: sprintf(p,">=");break;
		case 4: sprintf(p,"<");break;
		case 5: sprintf(p,">");break;
		default:sprintf(p,"==");break;
		
	}
	//printf("%s",p);
	return p;
}

void printFeatureCondition(struct condition* cond){
	if(cond!=NULL){
		printf(" %s %s %s ",cond->LHS,featureOperatorMap(cond->op),cond->RHS);	
	}
	/*if(cond->next!=NULL){
		printf("CAUGHT IT\n");exit(0);
	}*/
}

void printAssignmentList(struct condition* assignments){
	if(assignments!=NULL){
		while(assignments->next!=NULL){
			printFeatureCondition(assignments);
			printf(", ");
			assignments = assignments->next;
		}
		printFeatureCondition(assignments);
	}
}

char* getPORVAsString(struct PORV* porv){
        if(porv!=NULL){
                char* porvAsStr = (char*)malloc(sizeof(char)*MAX_STR_LENGTH);
                sprintf(porvAsStr,"%s %s %s ",porv->porv->LHS,operatorMap(porv->porv->op),porv->porv->RHS);
                return porvAsStr;
        }
        return NULL;
}

void printPORV(struct PORV* porv){
	if(porv!=NULL){
		printf("[%d]",porv->id);
                printCondition(porv->porv);
	}
}

void printPORVToFilePtr(struct PORV* porv,FILE* fp){
	if(porv!=NULL && fp){
		//fprintf(fp,"[%d]",porv->id);
		printConditionToFilePtr(porv->porv,fp);
	}
}


void printPORVConjunct(struct PORV* conjunct){
	if(conjunct!=NULL){
		while(conjunct->next!=NULL){
			printPORV(conjunct);
			printf("&&");
			conjunct = conjunct->next;
		}
		printPORV(conjunct);
	}
}

void printEvent(struct eventType* event){
	if(event!=NULL){
		printf(" @");
		switch(event->type){
			case 1: printf("+");break;
			case 2: printf("-");break;
			default:break;
		}
		printf("(");
		printPORV(event->porv);
		printf(") ");
	}
}

struct interval* createInterval(){
	return createIntervalInit(0.0,0.0,0);
}

struct interval* createIntervalInit(double left, double right, int id){
	struct interval* range = (struct interval*)malloc(sizeof(struct interval));
	range->left = left;
	range->right = right;
	range->id = id;
	return range;
}


//Terms - Disjuncts
struct term* createTerm(int id){
	#ifdef DEBUG_ON 
		printf("[createTerm] STARTED\n");
	#endif
	struct term* disjunct = (struct term*)malloc(sizeof(struct term));
	
	disjunct->PORVID = id;
	disjunct->next = NULL;
	#ifdef DEBUG_ON 
		printf("[createTerm] CRETED NODE\n");
	#endif
	
	return disjunct;	
}


struct term* addPORVToTerm(struct term* root, int id){
	#ifdef DEBUG_ON 
		printf("[addPORVToTerm] STARTED\n");
	#endif
	
	struct term* temp = root;
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
			printf("[addPORVToTerm] LIST NOT EMPTY\n");
		#endif
		while(temp->next!=NULL){
			temp=temp->next;
		}
		#ifdef DEBUG_ON 
			printf("[addPORVToTerm] ADDING TO LIST\n");
		#endif
		temp->next = createTerm(id);
	} else {
		#ifdef DEBUG_ON 
			printf("[addPORVToTerm] LIST EMPTY\n");
			printf("[addPORVToTerm] ADDING TO LIST\n");
		#endif
		root = createTerm(id);
		#ifdef DEBUG_ON 
			printf("[addPORVToTerm] ROOT CHANGED\n");
		#endif
		
	}
	#ifdef DEBUG_ON 
		printf("[addPORVToTerm] ADDED PORV to TERM\n");
	#endif
	return root;


}

void printTerm(struct term* disjunct, struct identifier* predicates){
	if(disjunct){
		struct identifier* id = getIdentifierWithID(predicates,disjunct->PORVID);
		printf("( %s ",id->name);
		
		disjunct = disjunct->next;
		while(disjunct && disjunct->next){
			id = getIdentifierWithID(predicates,disjunct->PORVID);
			
			printf("|| %s ",id->name);
			disjunct = disjunct->next;
		}
		if(disjunct){
			id = getIdentifierWithID(predicates,disjunct->PORVID);
			printf("|| %s ",id->name);
		} else printf(") ");
		
		/*
		printf("( %d ",disjunct->PORVID);
		disjunct = disjunct->next;
		while(disjunct && disjunct->next){
			printf("|| %d ",disjunct->PORVID);
			disjunct = disjunct->next;
		}
		if(disjunct)
			printf("|| %d ) ",disjunct->PORVID);
		else printf(") ");
		*/
	}
}

char* getTermAsString(struct term* disjunct, struct file* predicateMap){
	if(disjunct){
		char* strTerm = (char*)malloc(MAX_STR_LENGTH*sizeof(char));
		bzero(strTerm,MAX_STR_LENGTH*sizeof(char));
		
		struct PORV* temp = getPORV(predicateMap->porvList,disjunct->PORVID);
		
		if(!temp){
			printf("ERROR : PORV ID [%d] does not exist.\n",disjunct->PORVID);
		}
		//printf("HERE 3 temp = [%p]\n",temp);
		
		sprintf(strTerm,"( ( %s )",getPORVAsString(temp));
		//printf("HERE 4 \n");
		disjunct = disjunct->next;
		while(disjunct && disjunct->next){
			temp = getPORV(predicateMap->porvList,disjunct->PORVID);
			if(!temp){
				printf("ERROR : PORV ID [%d] does not exist.\n",disjunct->PORVID);
			}
			sprintf(strTerm,"%s or ( %s )",strTerm,getPORVAsString(temp));
			disjunct = disjunct->next;
		}
		//printf("HERE 5\n");
		if(disjunct){
			temp = getPORV(predicateMap->porvList,disjunct->PORVID);
			if(!temp){
				sprintf(strTerm,"ERROR : PORV ID [%d] does not exist.\n",disjunct->PORVID);
			}			
			sprintf(strTerm,"%s or ( %s ) ) ",strTerm,getPORVAsString(temp));
		} else sprintf(strTerm,"%s ) ",strTerm);
		return strTerm;
	}
	return NULL;
}

//Expressions
struct expression* createExpression(struct term* disjunct){
	#ifdef DEBUG_ON 
		printf("[createExpression] STARTED\n");
	#endif
	struct expression* expr = (struct expression*)malloc(sizeof(struct expression));
	
	expr->disjunct = disjunct;
	expr->next = NULL;
	#ifdef DEBUG_ON 
		printf("[createExpression] CRETED NODE\n");
	#endif
	
	return expr;		
	
}

struct expression* addTermToExpression(struct expression* root, struct term* disjunct){
	#ifdef DEBUG_ON 
		printf("[addTermToExpression] STARTED\n");
	#endif
	
	struct expression* temp = root;
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
			printf("[addTermToExpression] LIST NOT EMPTY\n");
		#endif
		while(temp->next!=NULL){
			temp=temp->next;
		}
		#ifdef DEBUG_ON 
			printf("[addTermToExpression] ADDING TO LIST\n");
		#endif
		temp->next = createExpression(disjunct);
	} else {
		#ifdef DEBUG_ON 
			printf("[addTermToExpression] LIST EMPTY\n");
			printf("[addTermToExpression] ADDING TO LIST\n");
		#endif
		root = createExpression(disjunct);
		#ifdef DEBUG_ON 
			printf("[addTermToExpression] ROOT CHANGED\n");
		#endif
		
	}
	#ifdef DEBUG_ON 
		printf("[addTermToExpression] ADDED TERM to EXPRESSION\n");
	#endif
	return root;	
}

void printExpression(struct expression* expr, struct identifier* predicates){
	if(expr){
		struct expression* tempExpr = expr;
		while(tempExpr){		
			printf("(");
			printTerm(tempExpr->disjunct,predicates);
			printf(")");
			tempExpr = tempExpr->next;
		}   		
	}
}

char* getExprAsString(struct expression* expr, struct file* predicateMap){
	if(expr){
		struct expression* tempExpr = expr;
		char* strExpr = (char*)malloc(MAX_STR_LENGTH*sizeof(char));
		bzero(strExpr,MAX_STR_LENGTH*sizeof(char));
		//printf("HERE1\n");
		strExpr = strcat(strExpr,getTermAsString(tempExpr->disjunct,predicateMap));
		tempExpr = tempExpr->next;
		//printf("HERE2\n");
		while(tempExpr){		
			strExpr = strcat(strExpr," and ");
			strExpr = strcat(strExpr,getTermAsString(tempExpr->disjunct,predicateMap));
			tempExpr = tempExpr->next;
		}  
		return strExpr;
	}
	return NULL;
}

//Expressions
struct expressionList* createExpressionList(struct expression* expr){
	#ifdef DEBUG_ON 
	printf("[createExpressionList] STARTED\n");
	#endif
	struct expressionList* exprList = (struct expressionList*)malloc(sizeof(struct expressionList));
	exprList->id = NULL;
	exprList->expr = expr;
	exprList->next = NULL;
	#ifdef DEBUG_ON 
	printf("[createExpressionList] CRETED NODE\n");
	#endif
	
	return exprList;
	
}


struct expressionList* addToExpressionList(struct expressionList* root, struct expression* expr){
	#ifdef DEBUG_ON 
	printf("[addToExpressionList] STARTED\n");
	#endif
	
	struct expressionList* temp = root;
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
		printf("[addToExpressionList] LIST NOT EMPTY\n");
		#endif
		
		while(temp->next!=NULL){
			temp=temp->next;
		}
		
		#ifdef DEBUG_ON 
		printf("[addToExpressionList] ADDING TO LIST\n");
		#endif
		
		temp->next = createExpressionList(expr);
	} else {
		#ifdef DEBUG_ON 
		printf("[addToExpressionList] LIST EMPTY\n");
		printf("[addToExpressionList] ADDING TO LIST\n");
		#endif
		
		root = createExpressionList(expr);
		
		#ifdef DEBUG_ON 
		printf("[addToExpressionList] ROOT CHANGED\n");
		#endif
		
	}
	#ifdef DEBUG_ON 
	printf("[addToExpressionList] ADDED TERM to EXPRESSION\n");
	#endif
	
	return root;	
}


struct expressionList* addExpressionListToExpressionList(struct expressionList* root, struct expressionList* list){
	#ifdef DEBUG_ON 
	printf("[addExpressionListToExpressionList] STARTED\n");
	#endif
	
	struct expressionList* temp = root;
	
	if(root!=NULL){
		#ifdef DEBUG_ON 
		printf("[addExpressionListToExpressionList] LIST NOT EMPTY\n");
		#endif
		
		while(temp->next!=NULL){
			temp=temp->next;
		}
		
		#ifdef DEBUG_ON 
		printf("[addExpressionListToExpressionList] ADDING TO LIST\n");
		#endif
		
		temp->next = list;
	} else {
		#ifdef DEBUG_ON 
		printf("[addExpressionListToExpressionList] LIST EMPTY\n");
		printf("[addExpressionListToExpressionList] ADDING TO LIST\n");
		#endif
		
		root = list;
		
		#ifdef DEBUG_ON 
		printf("[addExpressionListToExpressionList] ROOT CHANGED\n");
		#endif
		
	}
	#ifdef DEBUG_ON 
	printf("[addExpressionListToExpressionList] ADDED TERM to EXPRESSION\n");
	#endif
	
	return root;	
}

int getExpressionCount(struct expressionList* exprList){
	if(exprList){
		int count = 0;
		while(exprList){
			count++;
			exprList = exprList->next;
		}
		return count;
	}
	return 0;
}

struct expressionList* getExpressionAtPosition(struct expressionList *exprList, int position){
	if(exprList){
		int count = 0;
		while(exprList){
			count++;
			if(count==position)
				return exprList;
			exprList = exprList->next;
		}
		return NULL;
	}
}

//-----------------------SUPPORT METHODS --------------------------------

char* strToUpper(char* str){
        int len = strlen(str);
        char* newStr = (char*)malloc(sizeof(char)*(len+1));
        
        for(int i=0;i<len;i++){
                newStr[i] = toupper(str[i]);
        }
        newStr[len] = '\0';
        return newStr;
}

char* trim(char *str) {
          char *end;

        // Trim leading space
        while(isspace(*str)) str++;

        if(*str == 0)  // All spaces?
                return str;

        // Trim trailing space
        end = str + strlen(str) - 1;
        while(end > str && isspace(*end)) end--;

        // Write new null terminator
        *(end+1) = 0;

        return str;
}

int fileCopy(const char* source, const char* dest){
	if(source!=NULL && dest!=NULL){
		pid_t cpPID=fork();
		if(cpPID == 0){
			execlp("cp", "cp", source, dest,(char*)NULL);
			printf("ERROR: FAILED TO COPY %s  to %s\n",source,dest);
			exit(127);
			
		} else {
			waitpid(cpPID,0,0); // wait for child to exit 
			return access( dest, F_OK );
		}
	}
}


void printPredicateList(struct file* predMap){
	if(predMap){
		int i=1;
		struct PORV* pred = predMap->porvList;
		struct expressionList* cnf = predMap->exprList;
		while(cnf){
			if(cnf->id){
				printf("[%s] : ",cnf->id);i++;
			} else {	
				printf("[P%d] : ",i++);
			}
			struct expression* dterm = cnf->expr;
			printExpression(dterm,predMap->predicates);
			printf("\n");
			cnf = cnf->next;
		}
		//getPORV(predMap->porvList,)
	}
}

void printPredicates(struct identifier* predicates){
	while(predicates){
		printf("[%d] [%s]\n",predicates->col,predicates->name);
		predicates=predicates->next;
	}
}

void printVariableListToFilePtr(struct identifier* list, FILE* fp){
	if(list && fp){
		fprintf(fp,"\nstart\n");
		struct identifier* varList = list;
		while(varList){
			fprintf(fp,"%s %d %d\n",varList->name,varList->col,varList->timeCol);
			varList = varList->next;
		}
		fprintf(fp,"end\n");
	}
}
/*
void printPredicateMap(struct file* predMap){
        printf("\n---------------------PREDICATE MAP---------------------\n");
        if(predMap){
                
                printf("\nSTART AT ROW %d\n",predMap->start);
                
                printf("\nVariable List: {\n");
                struct identifier* id = predMap->varList;
                while(id){
                        printf("\n%s @ Col [%d] TimeRef [%d]",id->name,id->col,id->timeCol);
                        id = id->next;                        
                }
                printf("\n}\n");
                
                struct PORV* porvList = predMap->porvList;
                printf("\nPredicate List: {\n");
                while(porvList){
                        printf("\n[ATOM %d]",porvList->atomID);
                        printPORV(porvList);
                        porvList = porvList->next;
                }                
                
                struct expressionList* exprList = predMap->exprList;
		printf("\nExpression List: {\n");
		while(exprList){
			printExpression(exprList->expr);
			exprList = exprList->next;
			printf("\n");
		}                
		
        }
        printf("\n-------------------------------------------------------\n");
}
*/
void preparePy(struct file* predicateMap,char *csvFile){
        if(predicateMap && csvFile){
                FILE* fp = fopen("translate.py","w");
                if(fp==NULL){
                        printf("ERROR: Couldn't write python script.\n");
                        exit(0);
                }
                //printf("Writing to file.\n");
                printImports(fp);
                printCSV(fp,predicateMap,csvFile);                
                fclose(fp);
        }
        //printf("Writing to file predicateMap = [%p] csvFile = [%p].\n",predicateMap,csvFile);
}

void printImports(FILE* fp){
        if(fp){
                fprintf(fp,"import sys\n");
                fprintf(fp,"import csv\n");
		fflush(fp);
        }
}

void printConditionToFile(FILE* fp, struct PORV* predicate, int id, int timeCol){
        //printf("[%p] [%p] [%d] [%d]\n",fp,predicate,id,timeCol);
        
        if(fp && predicate && id>=0 && timeCol>=0){
                //printf("IN\n");
                char* cond = getPORVAsString(predicate);
                fprintf(fp,"\t\t\tif %s:\n",cond);
                fprintf(fp,"\t\t\t\tif switch[%d] == 0:\n",id);
                fprintf(fp,"\t\t\t\t\tswitch[%d] = 1\n",id);
                fprintf(fp,"\t\t\t\t\tl[%d] = float(row[%d])\n",id, timeCol-1);
                fprintf(fp,"\t\t\t\telse:\n");
                fprintf(fp,"\t\t\t\t\tr[%d] = float(row[%d])\n",id, timeCol-1);
                
                fprintf(fp,"\t\t\telse:\n");
                fprintf(fp,"\t\t\t\tif switch[%d] == 1:\n",id);
                fprintf(fp,"\t\t\t\t\tr[%d] = float(row[%d])\n",id,timeCol-1);
                fprintf(fp,"\t\t\t\t\tswitch[%d] = 0\n",id);
                fprintf(fp,"\t\t\t\t\tintervalSet[%d] = intervalSet[%d] + [[l[%d],r[%d]]]\n",id,id,id,id);
                fprintf(fp,"\t\t\t\t\tl[%d] = -1.0\n",id);
                fprintf(fp,"\t\t\t\t\tr[%d] = -1.0\n",id);
        }   
                
}


void printExpressionToFile(FILE* fp, struct file* predicateMap, struct expression* expr, int id){
	//printf("[%p] [%p] [%d] [%p]\n",fp,predicateMap,id,expr);
	
	if(fp && predicateMap && expr && id>=0){
		//printf("IN\n");
		timeCol = getIdentifierTimeCol(predicateMap->varList,predicateMap->porvList->atomID);
		char* cond = getExprAsString(expr,predicateMap);
		//printf("HERE\n");
		fprintf(fp,"\t\t\tif %s:\n",cond);
		fprintf(fp,"\t\t\t\tif switch[%d] == 0:\n",id);
		fprintf(fp,"\t\t\t\t\tswitch[%d] = 1\n",id);
		fprintf(fp,"\t\t\t\t\tl[%d] = float(row[%d])\n",id, timeCol-1);
		fprintf(fp,"\t\t\t\t\tr[%d] = float(row[%d])+%f\n",id, timeCol-1,epsilon);
		fprintf(fp,"\t\t\t\telse:\n");
		fprintf(fp,"\t\t\t\t\tr[%d] = float(row[%d])\n",id, timeCol-1);
		
		fprintf(fp,"\t\t\telse:\n");
		fprintf(fp,"\t\t\t\tif switch[%d] == 1:\n",id);
		fprintf(fp,"\t\t\t\t\tr[%d] = float(row[%d])\n",id,timeCol-1);
		fprintf(fp,"\t\t\t\t\tswitch[%d] = 0\n",id);
		fprintf(fp,"\t\t\t\t\tintervalSet[%d] = intervalSet[%d] + [[l[%d],r[%d]]]\n",id,id,id,id);
		fprintf(fp,"\t\t\t\t\tl[%d] = -1.0\n",id);
		fprintf(fp,"\t\t\t\t\tr[%d] = -1.0\n",id);
		fflush(fp);
	}   
	
}


void printCSV(FILE* fp,struct file* predicateMap,char* csvFile){
        if(fp && predicateMap && csvFile){
                fprintf(fp,"with open('%s','r') as f:\n",csvFile);
                char* prefix = (char*)malloc(sizeof(char)*MAX_STR_LENGTH);
                char* datFileName = (char*)malloc(sizeof(char)*strlen(csvFile));
		
		strcpy(datFileName,csvFile);
		char* tempF = datFileName;
		while(*tempF!='\0' && *tempF!='.'){
			tempF++;
		}
		if(*tempF == '.'){
			*tempF = '\0';
		}
		
                sprintf(prefix,"\t");
                
		fprintf(fp,"%s",prefix);
		fprintf(fp,"maxTime = 0.0\n");
		
		
                fprintf(fp,"%s",prefix);
                fprintf(fp,"reader = csv.reader(f,delimiter=',')\n");
                
                //Print Row Lower Bound
                fprintf(fp,"%s",prefix); fprintf(fp,"i = 1\n");
                fprintf(fp,"%s",prefix); fprintf(fp,"bound = %d\n",predicateMap->start);

                //Print interval list structures
                fprintf(fp,"%s",prefix);
                fprintf(fp,"l = [");
		int porvCount = getExpressionCount(predicateMap->exprList);//getPORVCount(predicateMap->porvList);
                int iterator = porvCount;
                while(iterator>1){
                        fprintf(fp,"-1.0,");
                        iterator--;
                }
                fprintf(fp,"-1.0]\n");
                
                fprintf(fp,"%s",prefix);
                fprintf(fp,"r = [");
                iterator = porvCount;
                while(iterator>1){
                        fprintf(fp,"-1.0,");
                        iterator--;
                }
                fprintf(fp,"-1.0]\n");
                
                //Switches
                fprintf(fp,"%s",prefix);
                fprintf(fp,"switch = [");
                iterator = porvCount;
                while(iterator>1){
                        fprintf(fp,"0,");
                        iterator--;
                }
                fprintf(fp,"0]\n");
                
                //Intervals
                fprintf(fp,"%s",prefix);
                fprintf(fp,"intervalSet = [");
                iterator = porvCount;
                while(iterator>1){
                        fprintf(fp,"[],");
                        iterator--;
                }
                fprintf(fp,"[]]\n");
                
                //Open Interval File
                fprintf(fp,"%s",prefix);
                fprintf(fp,"listFile= open(\"%s.dat\",\"w+\")\n",datFileName);
                
                //For Each Row in the CSV file
                fprintf(fp,"%s",prefix);
                fprintf(fp,"for row in reader:\n");
                
                sprintf(prefix,"%s\t",prefix);
                //Get to Row beyond bound
                fprintf(fp,"%sif i>bound:\n",prefix);
                
                //Each condition comes here
                sprintf(prefix,"%s\t",prefix);
                
                
		/*
		int porvIpreparePyD = 0;*
		struct PORV* porvList = predicateMap->porvList;
                while(porvList){
                        printConditionToFile(fp,porvList,porvID,getIdentifierTimeCol(predicateMap->varList,porvList->atomID));
                        porvID++;
                        porvList = porvList->next;
                }
                */
		
		fflush(fp);
		int exprID = 0;
		struct expressionList* exprList = predicateMap->exprList;
		while(exprList){
			printExpressionToFile(fp,predicateMap,exprList->expr,exprID);
			exprID++;
			exprList = exprList->next;
		}
		
		fprintf(fp,"\n\t\t\tmaxTime = float(row[0])\n");
                fprintf(fp,"\n\t\ti=i+1\n");
                
                //Open Closed Switches
                for(iterator=0;iterator<porvCount;iterator++){
                        fprintf(fp,"\tif switch[%d] == 1:\n",iterator);
                        fprintf(fp,"\t\tintervalSet[%d] = intervalSet[%d] + [[l[%d],r[%d]]]\n",iterator,iterator,iterator,iterator);
                }
                
                fprintf(fp,"\n\tfor intervalList in intervalSet:\n");
                fprintf(fp,"\n\t\tlistFile.write(\"{\")\n");
                fprintf(fp,"\n\t\tfor interval in intervalList:\n");
                fprintf(fp,"\n\t\t\tlistFile.write(\"[\")\n");
                fprintf(fp,"\n\t\t\tid = 1\n");
                fprintf(fp,"\n\t\t\tfor val in interval:\n");
                fprintf(fp,"\n\t\t\t\tif id == 1:\n");
                fprintf(fp,"\n\t\t\t\t\tlistFile.write(\"%%s:\" %% repr(val))\n");
                fprintf(fp,"\n\t\t\t\t\tid = 2\n");
                fprintf(fp,"\n\t\t\telse:\n");
                fprintf(fp,"\n\t\t\t\tlistFile.write(\"%%s\" %% repr(val))\n");
                fprintf(fp,"\n\t\t\tlistFile.write(\")\")\n");
                fprintf(fp,"\n\t\tlistFile.write(\"}\\n\")\n");
		fprintf(fp,"\n\tlistFile.write(\"%%s\" %% repr(maxTime))\n");
                fprintf(fp,"\n\tlistFile.close\n");
                
        }
}

void booleanize(){
        pid_t pyPID=fork();
        if (pyPID==0)
        { //child process 
                execlp("python3","python3","translate.py",(char*)NULL);
                printf("ERROR: FAILED TO RUN Python Script translate.py\n");
                exit(127); // only if execv fails 
        }
        else
        { // pid!=0; parent process 
                
                waitpid(pyPID,0,0); // wait for child to exit 
                
        }

}
