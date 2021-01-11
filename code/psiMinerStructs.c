/*****************************************************************
 *                      CODE WRITTEN BY                          *
 *                                                               *
 *             Antonio Anastasio Bruto da Costa                  *
 *     Formal Method Laboratory, Dept. of CSE, IIT Kharagpur     *
 *                                                               *
 *****************************************************************/

//#define MEM_DEBUG
//#define SUP_DEBUG
//#define MINER_DEBUG
//#define ASSERT_PRINT_DEBUG
//#define TREE_DEBUG
//#define METHOD_CALL
//#define VERBOSE_LOW
//#define VERBOSE_HIGH
#define VERBOSE_STD

#ifndef MAX_STR_LENGTH
#define MAX_STR_LENGTH 10240
#endif

#define NUM_DIGITS 2
#define SMALL_DBL 1e-2

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
#include <sys/stat.h> /* for file/directory status */

#include "structs.h"
#include "psiMinerStructs.h"
#include "learnedOP.tab.h"
extern int traceCount;
extern int targetBias;
extern int cumulative;
extern struct listOfIntervalListsStruct* coveredSet;
extern struct intervalListStruct* coveredList;
extern struct listOfIntervalListsStruct* intervalSet; //Set of Lists of Intervals for an isolated trace
extern struct listOfIntervalListsStruct** listOfIntervalSets; // Set of Interval Sets for Multiple Traces - One set per trace - All traces have an index
extern double K; // Resolution of time between consecutive sub-expressions
extern int N; //Upperbound for number of sub-expressions in the Sequence Expression
//extern int targetPORV_id; //Index of the target predicate for building the property
extern int assertCount;
extern struct treeNode* decisionTree; //Root of the Decision Tree built to learn properties
extern int numberOfPORVs; // Number of Knowledge (Known) Predicates provided by the User
extern struct nodeList* assertionList; // A list of Decision tree terminal nodes at which a property exists
extern int strict;
extern char* assertFileName;
extern double totalTraceLength; // Sum of trace lengths of all traces
extern double totalTrueLength; // Sum of true interval lengths of target across all traces
extern double totalFalseLength; // // Sum of false interval lengths of target across all traces
extern FILE* logFile;	//File pointer to the MAIN LOG of the tool
extern FILE* predLogFile;			/*	The Predicate Log for logging learned predicates that are used in the decision process*/
static int nodeID = 0;
extern int separationType;
extern double correlationThreshold;
extern double supportThreshold;

extern struct PORV* learnedPORVs;	
extern struct listOfIntervalListsStruct* learnedIntervalSet;
extern int learnedPORVCount;

extern struct file* predicateMap;
extern struct identifier* idList;

extern int learnPredicates;
extern struct predicateDetail* details;

extern FILE* learnedOPin;
extern int learnedOPparse(void);
extern struct config* inputConfig;
extern int continueFlag;
extern int logging;
static int first=0;

extern struct assertionStruct* allAssertions;
struct intervalListStruct** validLists = NULL;
static int superFlag=0;
/*-----------------------IDENTIFIER-----------------------------*/
/*
 * struct identifier* createIdentifier(unsigned int state){
 *	#ifdef METHOD_CALL 
 *	fprintf(logFile,"[createIdentifier] STARTED\n");
 *	#endif
 *	
 *	struct identifier* ID = (struct identifier*)malloc(sizeof(struct identifier));
 *	ID->next = NULL;
 *	ID->state = state;
 *	#ifdef DEBUG_ON 
 *	fprintf(logFile,"[createIdentifier] CRETED NODE\n");
 *	#endif
 *		
 *	return ID;
 * }
 * 
 * struct identifier* addIdentifierToList(struct identifier* root, struct identifier* ID){
 *	#ifdef METHOD_CALL 
 *	fprintf(logFile,"[addIdentifierToList] STARTED\n");
 *	#endif
 *	
 *	struct identifier* temp = root;
 *	
 *	if(root!=NULL){
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addIdentifierToList] ID LIST NOT EMPTY\n");
 *		#endif
 *		
 *		while(temp->next!=NULL){
 *			temp=temp->next;
 *		}
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addIdentifierToList] ADDING TO ID LIST\n");
 *		#endif
 *		
 *		temp->next = ID;
 *	} else {
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addIdentifierToList] ID LIST EMPTY\n");
 *		fprintf(logFile,"[addIdentifierToList] ADDING TO ID LIST\n");
 *		#endif
 *		
 *		root = ID;
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addIdentifierToList] ROOT CHANGED: ROOT->NAME = %s\n",root->name);
 *		#endif
 *		
 *	}
 *	#ifdef DEBUG_ON 
 *	fprintf(logFile,"[addIdentifierToList] ADDED IDENTIFIER [%s]\n",ID->name);
 *	#endif
 *	
 *	return root;
 * }
 * 
 * struct identifier* addToIdentifierList(struct identifier* root, unsigned int state){
 *	#ifdef DEBUG_ON 
 *	fprintf(logFile,"[addToIdentifierList] STARTED\n");
 *	#endif
 *	
 *	struct identifier* ID = (struct identifier*)malloc(sizeof(struct identifier));
 *	struct identifier* temp = root;
 *	ID->next = NULL;
 *	ID->state = state;
 *	#ifdef DEBUG_ON 
 *	fprintf(logFile,"[addToIdentifierList] CRETED NODE\n");
 *	#endif
 *	
 *	if(root!=NULL){
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addToIdentifierList] ID LIST NOT EMPTY\n");
 *		#endif
 *		
 *		while(temp->next!=NULL){
 *			temp=temp->next;
 *		}
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addToIdentifierList] ADDING TO ID LIST\n");
 *		#endif
 *		
 *		temp->next = ID;
 *	} else {
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addToIdentifierList] ID LIST EMPTY\n");
 *		fprintf(logFile,"[addToIdentifierList] ADDING TO ID LIST\n");
 *		#endif
 *		
 *		root = ID;
 *		#ifdef DEBUG_ON 
 *		fprintf(logFile,"[addToIdentifierList] ROOT CHANGED: ROOT->NAME = %s\n",root->name);
 *		#endif
 *		
 *	}
 *	#ifdef DEBUG_ON 
 *	fprintf(logFile,"[addToIdentifierList] ADDED IDENTIFIER [%s]\n",id);
 *	#endif
 *	
 *	return root;
 * }
 * 
 * int getIdentifierID(struct identifier* list,unsigned int state ){
 *	struct identifier* temp = list;
 *	int count=0;
 *	while(temp!=NULL){
 *		count++;
 *		if(temp->state == state){
 *			return count;
 *		}
 *		temp=temp->next;
 *	}	
 *	return 0;
 * }
 * 
 * struct identifier* duplicateIdentifierList(struct identifier* root){
 *	#ifdef METHOD_CALL 
 *	fprintf(logFile,"[duplicateIdentifierList] STARTED\n");
 *	#endif
 *	
 *	struct identifier* temp = root;
 *	struct identifier* newRoot = NULL;
 *	while(temp!=NULL){
 *		newRoot = addToIdentifierList(newRoot, temp->state);
 *		temp = temp->next;
 *	}
 *	#ifdef DEBUG_ON 
 *	fprintf(logFile,"[duplicateIdentifierList] COMPLETED\n");
 *	#endif
 *	
 *	return newRoot;
 * }
 * 
 * void freeIdentifierList(struct identifier* id){
 *	if(id!=NULL){
 *		freeIdentifierList(id->next);
 *		free(id);
 *	}
 * }
 * 
 * void printIdentifier(struct identifier* id){
 *	if(id!=NULL){
 *		//fprintf(logFile,"[%p] %s %s %s ",cond,cond->LHS,operatorMap(cond->op),cond->RHS);
 *		printf("Addrs: %p\n",id);
 *		printf("Name: %s\n",id->state);	
 *		printf("Next: %p\n\n",id->next);
 *	}
 * }
 * 
 * void printIdentifierList(struct identifier* id){
 *	printf("\n\n---------------IDENTIFIER LIST-----------------\n");
 *	while(id!=NULL){	
 *		printIdentifier(id);	
 *		//printf("%s, ",id->name);
 *		id = id->next;
 *	}
 *	printf("\n\n----------------------------------------------\n");
 * }
 * 
 * struct identifier* getIdentifierWithID(struct identifier* list, int id){
 *	while(list){
 *		if(id==1) return list;
 *		id--;
 *		list = list->next;
 *	}
 *	return NULL;
 * }
 * 
 */

struct assertionStruct* createAssertionStruct(){
	struct assertionStruct* temp = (struct assertionStruct*)malloc(sizeof(struct assertionStruct));
	bzero(temp->assertion,sizeof(char)*MAX_STR_LENGTH);
	return temp;
}

struct assertionStruct* addToAsssertionList(struct assertionStruct* assertion, struct assertionStruct* root){
	if(root){
		if(assertion->support > root->support){
			if(assertion->correlation > root->correlation){
				assertion->next = root;
				return assertion;
			}
		} else if((assertion->support == root->support) && (assertion->correlation > root->correlation)){
			assertion->next = root;
			return assertion;
		}
		
		struct assertionStruct* temp = root;
		while(temp->next){
			if(assertion->support > temp->next->support){
				if(assertion->correlation > temp->next->correlation){
					assertion->next = temp->next;
					temp->next = assertion;
					return root;
				}
			} else if((assertion->support == temp->next->support) && (assertion->correlation > temp->next->correlation)){
				assertion->next = temp->next;
				temp->next = assertion;
				return root;
			}
			temp = temp->next;
		}
		temp->next = assertion;
		return root;
	} else {
		return assertion;
	}
}

//Intervals
struct intervalStruct* createIntervalStruct(double l, double r){
	struct intervalStruct* temp = (struct intervalStruct*)malloc(sizeof(struct intervalStruct));
	if(temp){
		temp->l = l;
		temp->r = r;
		return temp;
	} else {
		printf("Problems with Memory\n");
		exit(-1);
	}
}

void printInterval(struct intervalStruct* interval){
	if(interval)
		printf("[%lf : %lf)",interval->l, interval->r);
}

void printIntervalToFilePtr(struct intervalStruct* interval,FILE* fp){
	if(interval)
		fprintf(fp,"[%lf : %lf)",interval->l, interval->r);
}

void deleteIntervalStruct(struct intervalStruct* interval){
	if(interval){
		free(interval);
	}
}

//IntervalLists
struct intervalListStruct* createIntervalList(struct intervalStruct* interval){
	struct intervalListStruct* list = (struct intervalListStruct*)malloc(sizeof(struct intervalListStruct));
	list->interval = interval;
	list->next = NULL;
	return list;
}

void addToIntervalList(struct intervalListStruct** root, struct intervalListStruct* listItem){
	if(*root){
		struct intervalListStruct* iterator = getLastIntervalInList(*root);
		iterator->next = listItem;
	} else {
		*root = listItem;
	}
}

struct intervalListStruct* addIntervalToList(struct intervalListStruct* root, struct intervalListStruct* listItem){
	if(root){
		struct intervalListStruct* iterator = getLastIntervalInList(root);
		iterator->next = listItem;
		return root;
	} else {
		return listItem;
	}	
}

struct intervalListStruct* getLastIntervalInList(struct intervalListStruct* root){
	if(root){
		while(root->next){
			root = root->next;
		}
	}
	return root;
}

void printIntervalList(struct intervalListStruct* list){
	if(list){
		//printf("\n\t{");
		
		while(list){
			printf(" ");
			printInterval(list->interval);
			list=list->next;
		}
		
		//printf(" }\n");
		
	}
}

void printIntervalListToFilePtr(struct intervalListStruct* list,FILE* fp){
	if(list){
		//printf("\n\t{");
		
		while(list){
			//fprintf(fp," ");
			if(list->interval == NULL){
				fprintf(fp,"WHY IS THIS NULL\n");fflush(fp);
			}
			printIntervalToFilePtr(list->interval,fp);
			list=list->next;
		}
		
		//printf(" }\n");
		
	}
}


void deleteIntervalList(struct intervalListStruct* list){
	#ifdef METHOD_CALL
	fprintf(logFile,"[deleteIntervalList] STARTED\n");fflush(logFile);
	#endif
	if(list){
		if(list->next){
			deleteIntervalList(list->next);
		}
		
		deleteIntervalStruct(list->interval);
		free(list);
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[deleteIntervalList] ENDED\n");fflush(logFile);
	#endif
}

struct intervalListStruct* flattenIntervalList(struct intervalListStruct* list){
	#ifdef METHOD_CALL
	fprintf(logFile,"[flattenIntervalList] STARTED\n");fflush(logFile);
	#endif
	
	if(list){
		#ifdef VERBOSE_LOW
		fprintf(logFile,"Flattening: "); printIntervalListToFilePtr(list,logFile);fprintf(logFile,"\n");fflush(logFile);
		#endif
		
		struct intervalListStruct* iterator = list;
		while(iterator->next!=NULL){
			if(iterator->next->interval->l <= iterator->interval->r){ //If there is an overlap
				#ifdef VERBOSE_LOW
				fprintf(logFile,"[flattenIntervalList] Merging: "); printIntervalToFilePtr(iterator->interval,logFile); fprintf(logFile," and "); printIntervalToFilePtr(iterator->next->interval,logFile);fprintf(logFile,"\n");fflush(logFile);
				#endif
				
				iterator->interval->r = (iterator->next->interval->r > iterator->interval->r)? iterator->next->interval->r : iterator->interval->r; //Expand to include the next interval
				
				struct intervalListStruct* temp = iterator->next;
				iterator->next = temp->next;
				
				temp->next = NULL;
				deleteIntervalList(temp);
				
				if(iterator->next == NULL)
					break;
			} else
				iterator = iterator->next;
		}
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[flattenIntervalList] ENDED\n");fflush(logFile);
		#endif
		
		return list;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[flattenIntervalList] ENDED with NULL\n");fflush(logFile);
	#endif
	
	return NULL;
}

struct intervalListStruct* unionIntervalLists(struct intervalListStruct* list1, struct intervalListStruct* list2){
	#ifdef METHOD_CALL
	fprintf(logFile,"[unionIntervalLists] STARTED\n");fflush(logFile);
	#endif
	
	if(list1 && list2){
		#ifdef VERBOSE_LOW
		fprintf(logFile,"Flattening: "); //printIntervalList(list);
		#endif
		
		struct intervalListStruct* list = NULL;
		struct intervalListStruct* iterator1 = list1;
		struct intervalListStruct* iterator2 = list2;
		while(iterator1 || iterator2){
			if(iterator1 == NULL){
				list = addIntervalToList(list,createIntervalList(createIntervalStruct(iterator2->interval->l,iterator2->interval->r)));
				iterator2 = iterator2->next;
			} else if(iterator2 == NULL){
				list = addIntervalToList(list,createIntervalList(createIntervalStruct(iterator1->interval->l,iterator1->interval->r)));
				iterator1 = iterator1->next;
			} else {
				if(iterator1->interval->l < iterator2->interval->l){
					list = addIntervalToList(list,createIntervalList(createIntervalStruct(iterator1->interval->l,iterator1->interval->r)));
					iterator1 = iterator1->next;
				} else {
					list = addIntervalToList(list,createIntervalList(createIntervalStruct(iterator2->interval->l,iterator2->interval->r)));
					iterator2 = iterator2->next;
				}       
			} 
		}
		list = flattenIntervalList(list);
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[unionIntervalLists] ENDED\n");fflush(logFile);
		#endif
		
		return list;
	} else {
		if(list1==NULL && list2==NULL){
			#ifdef METHOD_CALL
			fprintf(logFile,"[unionIntervalLists] ENDED with NULL\n");fflush(logFile);
			#endif
			return NULL;
		}
		
		if(list1){
			return duplicateIntervalList(list1);                        
		} else {
			return duplicateIntervalList(list2);
		}
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[unionIntervalLists] ENDED with NULL\n");fflush(logFile);
	#endif
	
	return NULL;
}

struct intervalListStruct* mergeIntervalLists(struct intervalListStruct* list1, struct intervalListStruct* list2){
	#ifdef METHOD_CALL
	fprintf(logFile,"[unionIntervalLists] STARTED\n");fflush(logFile);
	#endif
	
	if(list1 && list2){
		#ifdef VERBOSE_LOW
		printf("Flattening: "); //printIntervalList(list);
		#endif
		
		struct intervalListStruct* list = list1;
		struct intervalListStruct* iterator1 = list1;
		struct intervalListStruct* iterator2 = list2;
		while(iterator1 || iterator2){
			if(iterator1 == NULL){
				while(iterator2){
					list = addIntervalToList(list,createIntervalList(createIntervalStruct(iterator2->interval->l,iterator2->interval->r)));
					iterator2 = iterator2->next;
				}
			} else if(iterator2 == NULL){
				break;
			} else {
				if(iterator1->interval->l < iterator2->interval->l){
					if(iterator1->next){
						if(iterator1->next->interval->l > iterator2->interval->l){
							struct intervalListStruct* temp = createIntervalList(createIntervalStruct(iterator2->interval->l,iterator2->interval->r));
							temp->next = iterator1->next;
							iterator1->next=temp;
							temp = NULL;                                        
							iterator1 = iterator1->next;
							iterator2 = iterator2->next;
						}
					}
					while(iterator2){
						list = addIntervalToList(list,createIntervalList(createIntervalStruct(iterator2->interval->l,iterator2->interval->r)));
						iterator2 = iterator2->next;
					}
				} else if(iterator1->interval->l >= iterator2->interval->l){
					struct intervalListStruct* temp = createIntervalList(createIntervalStruct(iterator1->interval->l,iterator1->interval->r));
					temp->next = iterator1->next;
					iterator1->interval->l = iterator2->interval->l;
					iterator1->interval->r = iterator2->interval->r;
					iterator1 = iterator1->next;
					iterator2 = iterator2->next;                                        
				}       
			} 
		}
		list = flattenIntervalList(list);
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[unionIntervalLists] ENDED\n");fflush(logFile);
		#endif
		
		return list;
	} else {
		if(list1==NULL && list2==NULL){
			#ifdef METHOD_CALL
			fprintf(logFile,"[unionIntervalLists] ENDED with NULL\n");fflush(logFile);
			#endif
			return NULL;
		}
		
		if(list1){
			return list1;                        
		} else {
			return duplicateIntervalList(list2);
		}
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[unionIntervalLists] ENDED with NULL\n");fflush(logFile);
	#endif
	
	return NULL;
}


struct intervalListStruct* duplicateIntervalList(struct intervalListStruct* root){
	if(root){
		struct intervalListStruct* list = NULL;
		while(root){
			addToIntervalList(&list,createIntervalList(createIntervalStruct(root->interval->l,root->interval->r)));
			root = root->next;
		}
		return list;
	}
}


//List of Interval Lists
struct listOfIntervalListsStruct* createListOfIntervalLists(struct intervalListStruct* trueList, struct intervalListStruct* falseList){
	struct listOfIntervalListsStruct* list = (struct listOfIntervalListsStruct*)malloc(sizeof(struct listOfIntervalListsStruct));
	list->trueList = trueList;
	list->falseList = falseList;
	list->nextList = NULL;
	return list;
}

void addToListOfIntervalLists(struct listOfIntervalListsStruct** root, struct listOfIntervalListsStruct* listItem){
	#ifdef METHOD_CALL
	fprintf(logFile,"[addToListOfIntervalLists] STARTED\n");fflush(logFile);
	#endif
	if(*root){
		struct listOfIntervalListsStruct* iterator = getLastListOfIntervalsInList(*root);
		iterator->nextList = listItem;
	} else {
		*root = listItem;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[addToListOfIntervalLists] ENDED\n");fflush(logFile);
	#endif
}

struct listOfIntervalListsStruct* addListToListOfIntervalLists(struct listOfIntervalListsStruct* root, struct listOfIntervalListsStruct* listItem){
	#ifdef METHOD_CALL
	fprintf(logFile,"[addListToListOfIntervalLists] STARTED\n");fflush(logFile);
	#endif
	if(root){
		struct listOfIntervalListsStruct* iterator = getLastListOfIntervalsInList(root);
		iterator->nextList = listItem;
		#ifdef METHOD_CALL
		fprintf(logFile,"[addListToListOfIntervalLists] ENDED\n");fflush(logFile);
		#endif
		return root;
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[addListToListOfIntervalLists] ENDED\n");fflush(logFile);
		#endif
		return listItem;
	}
}

struct listOfIntervalListsStruct* getLastListOfIntervalsInList(struct listOfIntervalListsStruct* root){
	#ifdef METHOD_CALL
	fprintf(logFile,"[getLastListOfIntervalsInList] STARTED\n");fflush(logFile);
	#endif
	if(root){
		while(root->nextList){
			root = root->nextList;
		}
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[getLastListOfIntervalsInList] ENDED\n");fflush(logFile);
	#endif
	return root;
}

int printLengthOfIntervalLists(struct listOfIntervalListsStruct* list){
	//printf("\n\t{");
	int l=0;
	if(list){
		while(list){
			l++;
			/*printf("\n\t\t{");
			printf("\n\t\t\tTRUE: {");
			printIntervalList(list->trueList);
			printf("}\n");
			printf("\n\t\t\tFALSE: {");
			printIntervalList(list->falseList);
			printf("}\n");*/
			list=list->nextList;
		}
	}
	//printf("\n\t}\n");
	return l;
}

void printListOfIntervalLists(struct listOfIntervalListsStruct* list){
	printf("\n\t{");
	if(list){
		while(list){
			printf("\n\t\t{");
			printf("\n\t\t\tTRUE: {");
			printIntervalList(list->trueList);
			printf("}\n");
			printf("\n\t\t\tFALSE: {");
			printIntervalList(list->falseList);
			printf("}\n");
			list=list->nextList;
		}
	}
	printf("\n\t}\n");
}


void printListOfIntervalListsToFilePtr(struct listOfIntervalListsStruct* list, FILE* fp){
	fprintf(fp,"\n\t{");
	if(list){
		while(list){
			fprintf(fp,"\n\t\t{");
			fprintf(fp,"\n\t\t\tTRUE: {");
			printIntervalListToFilePtr(list->trueList,fp);
			fprintf(fp,"}\n");
			fprintf(fp,"\n\t\t\tFALSE: {");
			printIntervalListToFilePtr(list->falseList,fp);
			fprintf(fp,"}\n");
			list=list->nextList;
		}
	}
	fprintf(fp,"\n\t}\n");
}


void deleteListOfIntervalLists(struct listOfIntervalListsStruct* lists){
	if(lists){
		deleteIntervalList(lists->trueList);
		deleteIntervalList(lists->falseList);
		printf("@\n");
		deleteListOfIntervalLists(lists->nextList);
		if(lists->nextList){
			free(lists->nextList);
		}
		lists->nextList = NULL;
	}
}

struct listOfIntervalListsStruct* duplicateIntervalSet(struct listOfIntervalListsStruct* root){
	
	if(root){
		struct listOfIntervalListsStruct* list = NULL;//(struct listOfIntervalListsStruct*)malloc(sizeof(struct listOfIntervalListsStruct));
		while(root){
			list = addListToListOfIntervalLists(list,createListOfIntervalLists(duplicateIntervalList(root->trueList),duplicateIntervalList(root->falseList)));
			root = root->nextList;
		}
		return list;
	}
	return NULL;
}

struct listOfIntervalListsStruct* unionIntervalSets(struct listOfIntervalListsStruct* list1, struct listOfIntervalListsStruct* list2){
	if(list1 && list2){
		//printf("HERE1\n");
		struct listOfIntervalListsStruct* set = NULL;
		int count1 = 0;
		int count2 = 0;
		
		struct listOfIntervalListsStruct* iterator1 = list1;
		struct listOfIntervalListsStruct* iterator2 = list2;
		
		while(iterator1){
			count1++;
			iterator1=iterator1->nextList;
		}
		
		while(iterator2){
			count2++;
			iterator2=iterator2->nextList;
		}
		
		if(count1!=count2){
			return NULL;
		}
		
		iterator1 = list1;
		iterator2 = list2;
		//printf("HERE2\n");
		while(iterator1 && iterator2){
			set = addListToListOfIntervalLists(set,createListOfIntervalLists(unionIntervalLists(iterator1->trueList,iterator2->trueList),unionIntervalLists(iterator1->falseList,iterator2->falseList)));
			iterator1 = iterator1->nextList;
			iterator2 = iterator2->nextList;
		}
		return set;
		
	} else {
		//printf("list1 = [%p] list2 = [%p]\n",list1,list2);
		if(list1==NULL && list2==NULL)
			return NULL;
		
		struct listOfIntervalListsStruct* temp = NULL;
		
		if(list1!=NULL){
			temp = duplicateIntervalSet(list1);
			
			//printf("PRINTING LIST1\n");
			//printListOfIntervalLists(temp);
		}
		
		if(list2!=NULL){
			temp = duplicateIntervalSet(list2);
			//printf("PRINTING LIST2\n");
			//printListOfIntervalLists(list2);
			//printListOfIntervalLists(temp);
		}
		return temp;
	}
}

struct intervalListStruct* flattenIntervalSet(struct listOfIntervalListsStruct* list){
	struct intervalListStruct* temp = NULL;
	if(list){
		struct listOfIntervalListsStruct* iterator = list;
		while(iterator){
			temp = unionIntervalLists(temp,iterator->falseList);
			temp = unionIntervalLists(temp,iterator->trueList);
			iterator = iterator->nextList;
		}
	}
	return temp;        
}

/*************** Intersection Computation **************/
struct intervalStruct* intersectIntervals(struct intervalStruct* interval1, struct intervalStruct* interval2){
	if(interval1 && interval2){
		if(
			(interval1->l <= interval2->l && 
			(
				(interval2->l < interval1->r && interval1->r <= interval2->r) || 
				(interval1->r >= interval2->r)
			)
			) //Condition 1
			|| 
			(interval1->l >= interval2->l && 
			(
				(interval1->r <= interval2->r) || 
				(interval1->l < interval2->r && interval1->r >= interval2->r)
			)
			)//Condition 2
		){
			struct intervalStruct* interval = createIntervalStruct(
				maxFloat(interval1->l,interval2->l),
																   minFloat(interval1->r,interval2->r)
			);
			return interval;
		}
	}
	return NULL;
}

struct intervalListStruct* intersectIntervalList(struct intervalListStruct* list1, struct intervalListStruct* list2){
	#ifdef METHOD_CALL
	fprintf(logFile,"[intersectIntervalList] STARTED\n");fflush(logFile);
	#endif
	
	if(list1 && list2){
		struct intervalListStruct* list = NULL;
		struct intervalListStruct* iterator1 = list1;
		struct intervalListStruct* iterator2 = NULL;
		struct intervalStruct* t_interval;
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"[intersectIntervalList] Intersecting the lists:\n");fflush(logFile);
		fprintf(logFile,"[intersectIntervalList] List-1 = "); printIntervalListToFilePtr(list1,logFile);fprintf(logFile,"\n");fflush(logFile);
		fprintf(logFile,"[intersectIntervalList] List-2 = "); printIntervalListToFilePtr(list2,logFile);fprintf(logFile,"\n");fflush(logFile);
		#endif
		
		while(iterator1){
			iterator2 = list2;
			
			while(iterator2){
				if(iterator1->interval->r < iterator2->interval->l)
					break;
				
				t_interval = intersectIntervals(iterator1->interval, iterator2->interval);
				if(t_interval){
					addToIntervalList(
						&list,
					   createIntervalList(t_interval)
					);
				} 
				
				iterator2 = iterator2->next;
			}
			
			iterator1 = iterator1->next;
		}
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"[intersectIntervalList] Result = "); fflush(logFile);
		printIntervalListToFilePtr(list,logFile);
		fprintf(logFile,"\n");
		#endif

		#ifdef METHOD_CALL
		fprintf(logFile,"[intersectIntervalList] ENDED\n");fflush(logFile);
		#endif
		
		list = flattenIntervalList(list);
		return list;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[intersectIntervalList] ENDED with NULL\n");fflush(logFile);
	#endif
	
	return NULL;
}

struct listOfIntervalListsStruct* intersectIntervalSet(struct listOfIntervalListsStruct* intervalSet1, struct intervalListStruct* intervalList2){
	#ifdef METHOD_CALL
	fprintf(logFile,"[intersectIntervalSet] STARTED\n");fflush(logFile);
	#endif
	
	if(intervalSet1 && intervalList2){
		struct listOfIntervalListsStruct* tempIntervalSet = NULL;
		struct intervalListStruct* trueList = NULL;
		struct intervalListStruct* falseList = NULL;
		
		while(intervalSet1){
			trueList = intersectIntervalList(intervalSet1->trueList,intervalList2);
			falseList = intersectIntervalList(intervalSet1->falseList,intervalList2);
			trueList = flattenIntervalList(trueList);
			falseList = flattenIntervalList(falseList);
			tempIntervalSet = addListToListOfIntervalLists(tempIntervalSet,createListOfIntervalLists(trueList,falseList));
			intervalSet1 = intervalSet1->nextList;
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"[intersectIntervalSet] ENDED\n");fflush(logFile);
		#endif
		
		return tempIntervalSet;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[intersectIntervalSet] ENDED with NULL\n");fflush(logFile);
	#endif
	
	return NULL;
}

//Truth Assignments
struct truthAssignmentStruct* createTruthAssignment(int predicate_id, int truth, int position){
	struct truthAssignmentStruct* truthAssignment = (struct truthAssignmentStruct*)malloc(sizeof(struct truthAssignmentStruct));
	truthAssignment->predicate_id = predicate_id;
	truthAssignment->truth = truth;
	truthAssignment->position = position;
	truthAssignment->type = 0;
	return truthAssignment;
}


void addToTruthAssignmentList(struct truthAssignmentListStruct** root, struct truthAssignmentListStruct* listItem){
	if(*root){
		struct truthAssignmentListStruct* iterator = getLastListOfTruthAssignmentList(*root);
		iterator->next = listItem;
	} else {
		*root = listItem;
	}
}
struct truthAssignmentListStruct* createAss(){
        
                struct truthAssignmentListStruct* newStruct1 = (struct truthAssignmentListStruct*)malloc(sizeof(struct truthAssignmentListStruct));
                newStruct1->asgmt = createTruthAssignment(2,1,3);
		struct truthAssignmentListStruct* newStruct2 = (struct truthAssignmentListStruct*)malloc(sizeof(struct truthAssignmentListStruct));
		newStruct2->asgmt = createTruthAssignment(3,0,1);
		newStruct1->next = newStruct2;
                newStruct2->next = NULL;
                return newStruct1;
        
        return NULL;
}
struct truthAssignmentListStruct* createTruthListStruct(struct truthAssignmentStruct* truthStruct){
	if(truthStruct){
		struct truthAssignmentListStruct* newStruct = (struct truthAssignmentListStruct*)malloc(sizeof(struct truthAssignmentListStruct));
		newStruct->asgmt = truthStruct;
		newStruct->next = NULL;
		return newStruct;
	}
	return NULL;
}

struct truthAssignmentListStruct* getLastListOfTruthAssignmentList(struct truthAssignmentListStruct* root){
	if(root){
		while(root->next){
			root = root->next;
		}
		return root;
	}
	return NULL;
}

struct truthAssignmentListStruct* duplicateTruthAssignmentList(struct truthAssignmentListStruct* root){
	#ifdef METHOD_CALL
	fprintf(logFile,"[duplicateTruthAssignmentList] STARTED\n");
	#endif
	if(root){
		struct truthAssignmentListStruct* newRoot = NULL;
		while(root){
			struct truthAssignmentListStruct* temp = createTruthListStruct(createTruthAssignment(root->asgmt->predicate_id,root->asgmt->truth,root->asgmt->position));
			temp->asgmt->type = root->asgmt->type;
			addToTruthAssignmentList(&newRoot,temp);
			root = root->next;
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"[duplicateTruthAssignmentList] ENDED\n");
		#endif
		return newRoot;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[duplicateTruthAssignmentList] ENDED\n");
	#endif
	return NULL;
}

int predicateTargetInList(struct truthAssignmentListStruct* root, int predicate, int pos){
	#ifdef METHOD_CALL
	fprintf(logFile,"[predicateInList] STARTED\n");
	#endif
	if(root){
		while(root){
			if((root->asgmt->predicate_id == predicate) && (root->asgmt->position == pos)){
				#ifdef METHOD_CALL
				fprintf(logFile,"[predicateInList] ENDED - IS in list\n");
				#endif
				return 1;
			}
			root = root->next;
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"[predicateInList] ENDED - NOT in list\n");
		#endif
		return 0;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[predicateInList] ENDED\n");
	#endif
	return 0;
}


int predicateInList(struct truthAssignmentListStruct* root, int predicate){
	#ifdef METHOD_CALL
	fprintf(logFile,"[predicateInList] STARTED\n");
	#endif
	if(root){
		while(root){
			if(root->asgmt->predicate_id == predicate){
				#ifdef METHOD_CALL
				fprintf(logFile,"[predicateInList] ENDED - IS in list\n");
				#endif
				return 1;
			}
			root = root->next;
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"[predicateInList] ENDED - NOT in list\n");
		#endif
		return 0;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[predicateInList] ENDED\n");
	#endif
	return 0;
}

void printTruthListToFile(struct truthAssignmentListStruct* root){
	//FILE* fp = fopen("assertions.txt","a");
	FILE* fp;
	if(assertFileName==NULL){
		fp = fopen("assertions.txt","a");
	} else {
		fp = fopen(assertFileName,"a");
	}
	
	printTruthListToFilePtr(root,fp);
	fclose(fp);
}

void printTruthListToFilePtr(struct truthAssignmentListStruct* root, FILE* fp){
	#ifdef METHOD_CALL
	fprintf(logFile,"[printTruthListToFilePtr] STARTED\n");
	#endif
	if(fp){
		if(root){
			//fprintf(fp,"\n------------------------------------------\n");
			fprintf(fp,"{");
			while(root){
				struct PORV* pred =NULL;
				if(root->asgmt->type){
					//printf("SHOULDNOT BE HERE\n");fflush(stdout);
					pred = getPORV(learnedPORVs,root->asgmt->predicate_id);
				}
				
				if(root->asgmt->truth){
					if(root->asgmt->type){
						//printf("SHOULDNOT BE HERE\n");fflush(stdout);
						fprintf(fp,"{ ");printConditionToFilePtr(pred->porv,fp);fprintf(fp," } ");
					}
					fprintf(fp,"%sP%d [%d] ",root->asgmt->type?"L":"K",root->asgmt->predicate_id,root->asgmt->position);
				} else {
					if(root->asgmt->type){
						//printf("SHOULDNOT BE HERE\n");fflush(stdout);
						fprintf(fp,"{ NOT ");printConditionToFilePtr(pred->porv,fp);fprintf(fp,"} ");
					}
					fprintf(fp,"~%sP%d [%d] ",root->asgmt->type?"L":"K",root->asgmt->predicate_id,root->asgmt->position);
				}
				if(root->next){
					fprintf(fp," AND ");
				}
				//fprintf(fp,"Predicate : %d - Truth : %d \n",root->asgmt->predicate_id, root->asgmt->truth);
				root = root->next;
			}
			fprintf(fp,"}\n");
			//fprintf(fp,"\n------------------------------------------\n");
			return;
		}
		fprintf(fp,"Truth List Is Empty\n");fflush(logFile);
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[printTruthListToFilePtr] ENDED\n");
	#endif
	
}


void printTruthList(struct truthAssignmentListStruct* root){
	if(root){
		printf("------------------------------------------\n");
		while(root){
			if(root->asgmt->truth){
				printf("P%d [%d]",root->asgmt->predicate_id,root->asgmt->position);
			} else {
				printf("~P%d [%d]",root->asgmt->predicate_id,root->asgmt->position);
			}
			if(root->next){
				printf(" AND ");
			}
			root = root->next;
		}
		printf("\n------------------------------------------\n");
		return;
	}
	printf("Truth List Is Empty\n");
}

char* getPredicateName(int id){
	if(id>0){
		return getExpressionAtPosition(predicateMap->exprList,id)->id;
	} else return NULL;
}


/* 
 * Prints the list of predicates and their truth at a given index position
 * Returns 0 if there are no predicates at the index position and 1 otherwise
 */
int writeTruthListForIndex(struct truthAssignmentListStruct* root, int index, char* assertion, int andprinter){
	if(root && assertion){
		//Remember if there is anything at the index to print.
		int flag = 0;
		char* fp = (char*)malloc(sizeof(char)*MAX_STR_LENGTH);
		bzero(fp,sizeof(char)*MAX_STR_LENGTH);
		
		while(root){
			if(root->asgmt->position > index) {
				root = root->next; continue;
			} else if(root->asgmt->position == index){
				if(flag==0 && andprinter!=0){
					sprintf(fp," && ");strcat(assertion,fp);	
				}
				char* exprName = NULL;
				if(root->asgmt->type == 0)
					exprName = getExpressionAtPosition(predicateMap->exprList,root->asgmt->predicate_id)->id;
				
				if(root->asgmt->truth){
					if(root->asgmt->type){
						struct PORV* pred = getPORV(learnedPORVs,root->asgmt->predicate_id);
						
						printConditionToString(pred->porv,fp);strcat(assertion,fp);
					} else if(exprName){
						sprintf(fp,"%s",exprName);strcat(assertion,fp);
					} else {
						sprintf(fp,"P%d",root->asgmt->predicate_id);strcat(assertion,fp);
					}
				} else {
					if(root->asgmt->type){
						struct PORV* pred = getPORV(learnedPORVs,root->asgmt->predicate_id);
						sprintf(fp,"!("); strcat(assertion,fp);
						printConditionToString(pred->porv,fp);strcat(assertion,fp);
						sprintf(fp,")");strcat(assertion,fp);
					} else if(exprName){
						sprintf(fp,"!%s",exprName);strcat(assertion,fp);
					} else {
						sprintf(fp,"!P%d",root->asgmt->predicate_id);strcat(assertion,fp);
					}
				}                                
				flag = 1;
			}
			if(root->next && root->asgmt->position == index  && root->next->asgmt->position == index){
				sprintf(fp," && ");strcat(assertion,fp);
			}
			if(root->next && root->next->asgmt->position < index){
				return flag;
			}
			root = root->next;
		}                
		return flag;
	}
	//printf("Truth List Is Empty\n");
}


/* 
 * Prints the list of predicates and their truth at a given index position
 * Returns 0 if there are no predicates at the index position and 1 otherwise
 */
int printTruthListForIndex(struct truthAssignmentListStruct* root, int index, FILE* fp, int andprinter){
	if(root && fp){
		//Remember if there is anything at the index to print.
		int flag = 0;
		
		while(root){
			if(root->asgmt->position > index) {
				root = root->next; continue;
			} else if(root->asgmt->position == index){
				if(flag==0 && andprinter!=0){
					fprintf(fp," && ");	
				}
				char* exprName = NULL;
				if(root->asgmt->type == 0)
					exprName = getExpressionAtPosition(predicateMap->exprList,root->asgmt->predicate_id)->id;
				
				if(root->asgmt->truth){
					if(root->asgmt->type){
						struct PORV* pred = getPORV(learnedPORVs,root->asgmt->predicate_id);
						printConditionToFilePtr(pred->porv,fp);
					} else if(exprName){
						fprintf(fp,"%s",exprName);
					} else {
						fprintf(fp,"P%d",root->asgmt->predicate_id);
					}
				} else {
					if(root->asgmt->type){
						struct PORV* pred = getPORV(learnedPORVs,root->asgmt->predicate_id);
						fprintf(fp,"!("); printConditionToFilePtr(pred->porv,fp);fprintf(fp,")");
					} else if(exprName){
						fprintf(fp,"!%s",exprName);
					} else {
						fprintf(fp,"!P%d",root->asgmt->predicate_id);
					}
				}                                
				flag = 1;
			}
			if(root->next && root->asgmt->position == index  && root->next->asgmt->position == index){
				fprintf(fp," && ");
			}
			if(root->next && root->next->asgmt->position < index){
				return flag;
			}
			root = root->next;
		}                
		return flag;
	}
	//printf("Truth List Is Empty\n");
}

void printSequencePositions(struct treeNode* root){
	if(root){
		//FILE* aFile = fopen("assertions.txt","a");
		FILE* fp;
		if(assertFileName==NULL){
			fp = fopen("assertions.txt","a");
		} else {
			fp = fopen(assertFileName,"a");
		}
		
		//printf("Position: %d for Predicate %d \n",root->targetInfluence, root->splittingPredicate_id);
		fprintf(fp,"Position: %d for Predicate %d \n",root->targetInfluence, root->splittingPredicate_id);
		fclose(fp);
		printSequencePositions(root->parent);
	}
}

void printSequencePositionsToFile(struct treeNode* root, FILE *fp){
	if(root){
		//FILE* aFile = fopen("assertions.txt","a");
		if(fp){
			//printf("Position: %d for Predicate %d \n",root->targetInfluence, root->splittingPredicate_id);
			fprintf(fp,"Position: %d for Predicate %d \n",root->targetInfluence, root->splittingPredicate_id);
			printSequencePositionsToFile(root->parent,fp);
		}
	}
}


/* 
 * Merge Sort Methods for sorting the Constraint Set
 */
void sortTruthAssignmentList(struct truthAssignmentListStruct** root){
	#ifdef METHOD_CALL
	fprintf(logFile,"[SORT] START\n");fflush(logFile);
	#endif
	if(*root){
		struct truthAssignmentListStruct *a, *b;
		struct truthAssignmentListStruct *head = *root;
		
		//Size of list is 0 or 1
		if(head == NULL || head->next == NULL){
			#ifdef VERBOSE_LOW
			fprintf(logFile,"[SORT] 1-4 END\n");fflush(logFile);
			#endif
			return;
		}
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"[SORT] HERE 1\n");fflush(logFile);	
		#endif
		
		
		//Split the list into a and b sublists
		a = head;
		listSplit(head,&b);
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"[SORT] HERE 2\n");fflush(logFile);
		#endif
		
		//Sort each list recursively
		sortTruthAssignmentList(&a);
		sortTruthAssignmentList(&b);
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"[SORT] HERE 3\n");fflush(logFile);
		#endif
		
		//Merge a and b
		*root = auxMergeTruthAssignmentList(a,b);
		#ifdef VERBOSE_LOW
		fprintf(logFile,"[SORT] HERE 4\n");fflush(logFile);
		#endif
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[SORT] END\n");fflush(logFile);
	#endif
}

/* 
 * Merge Sort Methods for sorting the Constraint Set
 */
struct truthAssignmentListStruct* auxMergeTruthAssignmentList(struct truthAssignmentListStruct* a, struct truthAssignmentListStruct* b){
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[AUX] START\n");
	#endif
	struct truthAssignmentListStruct *result = NULL;
	
	if(a==NULL){
		#ifdef METHOD_CALL 
		fprintf(logFile,"[AUX] END\n"); 
		#endif 
		return(b);
	}
	if(b==NULL){
		#ifdef METHOD_CALL
		fprintf(logFile,"[AUX] END\n"); 
		#endif
		return(a);
	}
	
	//printf("HERE1\n");fflush(stdout);
	
	if(a->asgmt->position >= b->asgmt->position){
		result = a;
		result->next = auxMergeTruthAssignmentList(a->next,b);
	} else {
		result = b;
		result->next = auxMergeTruthAssignmentList(a,b->next);
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[AUX] END\n");
	#endif
	return(result);
}


/*
 * This method splits a list into half.
 */
void listSplit(struct truthAssignmentListStruct* root, struct truthAssignmentListStruct** mid){
	#ifdef METHOD_CALL
	fprintf(logFile,"[LIST SPLIT] START\n"); fflush(logFile);
	fprintf(logFile,"**************************** LIST ***************************\n"); 
	printTruthListToFilePtr(root,logFile);
	fprintf(logFile,"**************************************************************\n");
	#endif
	
	struct truthAssignmentListStruct *slow, *fast;
	slow = fast = root;
	
	if(root==NULL || root->next==NULL){
		*mid = NULL;
	} else {
		//printf("\t[LIST SPLIT] - ELSE\n");fflush(stdout);
		while(fast){
			fast = fast->next;
			
			if(fast)
				fast = fast->next;                        
			
			if(fast)
				slow = slow->next;
		}
		*mid = slow->next;
		slow->next = NULL; //This puts an end to the first half of the list.
	}
	#ifdef VERBOSE_LOW
	fprintf(logFile,"[LIST SPLIT] END\n");
	fflush(logFile);
	#endif
}

void purgeTruthList(struct truthAssignmentListStruct* truthList){
	if(truthList){
		fprintf(logFile,"ASSIGNMENT TO BE FREE\n");fflush(logFile);
		free(truthList->asgmt);
		truthList->asgmt=NULL;
		
		fprintf(logFile,"NEXT OF TRUTH LIST TO BE PURGED\n");fflush(logFile);
		purgeTruthList(truthList->next);
		
		fprintf(logFile,"NEXT OF TRUTH LIST TO BE FREE\n");fflush(logFile);
		free(truthList->next);
		truthList->next=NULL;
		
		fprintf(logFile,"DONE WITH NODE [%p]\n",truthList);fflush(logFile);
	}
}

void purgeTruthListForTreeNode(struct treeNode* root){
	if(root){
		fprintf(logFile,"LEFT TO BE FREE\n");fflush(logFile);
		purgeTruthListForTreeNode(root->left);
		
		fprintf(logFile,"RIGHT TO BE FREE\n");fflush(logFile);
		purgeTruthListForTreeNode(root->right);
		
		fprintf(logFile,"TRUTH LIST TO BE FREE\n");fflush(logFile);
		purgeTruthList(root->truthList);
		root->truthList = NULL;
		
		fprintf(logFile,"DONE WITH NODE [%p]\n",root);fflush(logFile);
	}
}

//Index Couples
struct indexCouple* createIndexCouple(int target, int predicate){
	struct indexCouple* index = (struct indexCouple*)malloc(sizeof(struct indexCouple));
	index->target = target;
	index->predicate = predicate;
	index->type = 0;
	index->truth = 0;
	index->next=NULL;
	return index;
}

void addToIndexCoupleList(struct indexCouple** root, struct indexCouple* listItem){
	#ifdef METHOD_CALL
	fprintf(logFile,"[addToIndexCoupleList] STARTED\n");
	#endif
	if(*root){
		struct indexCouple* iterator = getLastOfIndexCoupleList(*root);
		iterator->next = listItem;
		#ifdef METHOD_CALL
		fprintf(logFile,"[addToIndexCoupleList] ENDED - added listItem to the root list\n");fflush(logFile);
		#endif
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[addToIndexCoupleList] ENDED - root was NULL\n");fflush(logFile);
		#endif
		*root = listItem;
	}	
}

struct indexCouple* getLastOfIndexCoupleList(struct indexCouple* root){
	if(root){
		while(root->next){
			root = root->next;
		}
		return root;
	}
	return NULL;
}

void printIndexCoupleList(struct indexCouple* root){
	if(root){
		printf("------------------------------------------\n");
		while(root){
			if(root->truth){
				printf("Predicate : %d True - Target : %d \n",root->predicate, root->target);
			} else {
				printf("Predicate : %d False - Target : %d \n",root->predicate, root->target);
			}
			root = root->next;
		}
		printf("------------------------------------------\n");
		return;
	}
	printf("Index Empty\n");
}

void printIndexCoupleListToFilePtr(struct indexCouple* root,FILE* fp){
	if(fp){
		if(root){
			fprintf(fp,"------------------------------------------\n");
			while(root){
				if(root->truth){
					fprintf(fp,"%s Predicate : %d True - Target : %d \n",root->type?"Learned":"Known", root->predicate, root->target);
				} else {
					fprintf(fp,"%s Predicate : %d False - Target : %d \n",root->type?"Learned":"Known", root->predicate, root->target);
				}
				root = root->next;
			}
			fprintf(fp,"------------------------------------------\n");
			return;
		}
		fprintf(fp,"Index Empty\n");
	}
}


struct indexCouple* duplicateIndexCouple(struct indexCouple* root){
	if(root){
		struct indexCouple* list = NULL;
		while(root){
			struct indexCouple * item = createIndexCouple(root->target,root->predicate); 
			item->type = root->type;
			item->truth = root->truth;
			
			addToIndexCoupleList(&list,item);
			root = root->next;
		}
		return list;
	}
}

//Tree Node
struct treeNode* createTreeNode(
	struct truthAssignmentListStruct* truthList,
	struct listOfIntervalListsStruct** intervalSetList,
	int targetInfluence,
	int splittingPredicate_id,
	double traceLength,
	struct treeNode* left,
	struct treeNode* right){
	
	struct treeNode* node = (struct treeNode*)malloc(sizeof(struct treeNode));
	node->truthList = truthList;
	node->listOfIntervalSets = intervalSetList;
	node->mean = -1;
	node->error = -1;
	node->targetInfluence = targetInfluence;
	node->splittingPredicate_id = splittingPredicate_id;
	node->traceLength = traceLength;
	node->trueFalseFlag = 0;
	node->left = left;
	node->right = right;
	node->parent = NULL;
	node->explored = NULL;
	node->id = nodeID++;
	node->truthValue = 1;
	return node;
}
	
	
// Tree Node List
struct nodeList* createNodeList(struct treeNode* node){
	struct nodeList* list = (struct nodeList*)malloc(sizeof(struct nodeList));
	list->node = node;
	list->next = NULL;
	return list;
}

void addToNodeList(struct nodeList** root, struct nodeList* listItem){
	if(*root){
		struct nodeList* iterator = getLastNodeInList(*root);
		iterator->next = listItem;
	} else {
		*root = listItem;
	}
}

struct nodeList* addNodeToList(struct nodeList* root, struct treeNode* listItem){
	if(root){
		struct nodeList* iterator = getLastNodeInList(root);
		iterator->next = createNodeList(listItem);
		return root;
	} else {
		struct nodeList* node = createNodeList(listItem);
		return node;
	}       
}

struct nodeList* getLastNodeInList(struct nodeList* root){
	if(root){
		while(root->next){
			root = root->next;
		}
	}
	return root;
}


/******************** Support Methods *******************/
double maxFloat(double val1, double val2){
	fflush(logFile);
	if(val1>val2)
		return val1;
	else return val2;
}

double minFloat(double val1, double val2){
	if(val1>val2)
		return val2;
	else return val1;
}

double lengthOfIntervalList(struct intervalListStruct* list){
	#ifdef METHOD_CALL
	fprintf(logFile,"\n[lengthOfIntervalList] STARTED\n");
	fflush(logFile);
	#endif
	if(list){
		double length = 0.0;
		while(list){
			length += list->interval->r - list->interval->l;
			list = list->next;
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"\n[lengthOfIntervalList] ENDED\n");
		fflush(logFile);
		#endif
		return length;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"\n[lengthOfIntervalList] ENDED with NULL LIST returning ZERO\n");
	fflush(logFile);
	#endif
	return 0.0;
}

double mean(double intervalListLength, double traceLength){
	return (double)(intervalListLength/traceLength);
}

double error(double mean){
	return 2.0 * mean * (1 - mean);
}

int countLists(struct listOfIntervalListsStruct* list){
	if(list){
		int count = 0;
		while(list){
			count++;
			list = list->nextList;
		}
		return count;
	}
	return 0;
}


double computeMean(struct listOfIntervalListsStruct** localIntervalSets, int target, double traceLength, struct intervalListStruct** influenceList){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeMean] STARTED target = %d\n",target);
	#endif
	
	if(localIntervalSets){
		struct intervalListStruct* trueList = NULL;
		struct listOfIntervalListsStruct* targetList =  NULL;
		int i=0;
		double meanVal =0.0;
		double targetLength = 0.0;
		int emptyInfluenceFlag=1;
		double influenceLength = 0.0;
		for(i=0;i<traceCount;i++){
			influenceLength += lengthOfIntervalList(influenceList[i]);
			
			fprintf(logFile,"INFLUENCE LIST:");
			printIntervalListToFilePtr(influenceList[i],logFile);fprintf(logFile,"\n");
			
			fprintf(logFile,"PREDICATE TRUTH INTERVAL LIST:");
			printListOfIntervalListsToFilePtr(listOfIntervalSets[i],logFile);fprintf(logFile,"\n");
			
			//printIntervalListToFilePtr(influenceList[i],stdout);
			targetList = getListAtPosition(listOfIntervalSets[i],target);
			
			fprintf(logFile,"TARGET INTERVAL LIST:");
			printIntervalListToFilePtr(targetList->trueList,logFile);fprintf(logFile,"\n");
			printIntervalListToFilePtr(targetList->falseList,logFile);fprintf(logFile,"\n");
			
			fprintf(logFile,"TARGET TRUE LIST:");
			trueList = targetList->trueList;
			
			trueList = intersectIntervalList(trueList,influenceList[i]);
			targetLength += lengthOfIntervalList(trueList);
			#ifdef SUP_DEBUG
			printf("[computeMean Target Length = %lf , Target = %d\n",targetLength,target);
			fprintf(logFile,"[computeMean] Target-%d's Truth Lists for Trace [%d]\n",target,i);
			fprintf(logFile,"Target List [%p]\n",targetList);
			printIntervalListToFilePtr(targetList->trueList,logFile);fprintf(logFile,"\n");
			fprintf(logFile,"[computeMean] Length of End Match for Trace[%d] = [%f]\n",i,lengthOfIntervalList(influenceList[i]));
			fprintf(logFile,"[computeMean] Length of Target[True] for Trace[%d] Under Constraints = %lf\n",i,lengthOfIntervalList(trueList));
			fprintf(logFile,"[computeMean] Cumulative Length of End Match = %lf\n",influenceLength);
			fprintf(logFile,"[computeMean] Cumulative Length of Target[True] Under Constraints = %lf\n",targetLength);
			#endif
			
			if(trueList!=NULL){
				emptyInfluenceFlag = 0;
			}
		}
		
		if(!emptyInfluenceFlag){
			meanVal = mean(targetLength,influenceLength);
			#ifdef SUP_DEBUG
			fprintf(logFile,"[computeMean] Returning Mean [%lf] for Target [%d]\n",meanVal,target);
			#endif
			if(meanVal>1.0 || meanVal<0.0){
				fprintf(logFile,"[computeMean] [ERROR] Mean = [%lf], targetLength = [%lf], traceLength = [%lf]\n",meanVal,targetLength,traceLength);
			}
			return meanVal;
		}
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeMean] Error\n");
		fprintf(logFile,"[computeMean] ENDED\n");
		#endif
		
		return 0.0;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeMean] ENDED with NULL\n");
	#endif
	
	return 0.0;
}


/*
 * We also compute the mean for when the target is false. This is because
 * pseudo-targets do not have complement truth and false lists.
 */
double computeFalseMean(struct listOfIntervalListsStruct** localIntervalSet, int target, double traceLength, struct intervalListStruct** influenceList){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeFalseMean] STARTED target = %d\n",target);
	#endif
	
	if(localIntervalSet){
		struct intervalListStruct* falseList = NULL;
		struct listOfIntervalListsStruct* targetList =  NULL;
		
		int i=0;
		double meanVal =0.0;
		double targetLength = 0.0;
		int emptyInfluenceFlag=1;
		traceLength = 0.0;
		for(i=0;i<traceCount;i++){
			targetList = getListAtPosition(localIntervalSet[i],target);
			falseList = intersectIntervalList(targetList->falseList,influenceList[i]);
			
			targetLength += lengthOfIntervalList(falseList);
			traceLength += lengthOfIntervalList(influenceList[i]);
			
			if(falseList!=NULL){
				emptyInfluenceFlag = 0;
			}
			
			#ifdef SUP_DEBUG
			fprintf(logFile,"[computeFalseMean] Influence List = ");
			printIntervalListToFilePtr(influenceList[i],logFile);fprintf(logFile,"\n");
			fprintf(logFile,"[computeFalseMean] Trace Length = %lf [%lf]\n",traceLength,lengthOfIntervalList(influenceList[i]));
			fprintf(logFile,"[computeFalseMean] Target    List = ");
			printIntervalListToFilePtr(falseList,logFile);fprintf(logFile,"\n");
			fprintf(logFile,"[computeFalseMean] Length of Target[False] Under Constraints = %lf\n",targetLength);
			#endif
		}
		if(!emptyInfluenceFlag){
			meanVal = mean(targetLength,traceLength);
			#ifdef SUP_DEBUG
			fprintf(logFile,"[computeFalseMean] Returning Mean [%lf] for Target [%d]\n",meanVal,target);
			#endif
			if(meanVal>1.0 || meanVal<0.0){
				fprintf(logFile,"[computeFalseMean] [ERROR] Mean = [%lf], targetLength = [%lf], traceLength = [%lf]\n",meanVal,targetLength,traceLength);
			}
			return meanVal;
		}
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeFalseMean] Error\n");
		#endif

		#ifdef METHOD_CALL
		fprintf(logFile,"[computeFalseMean] ENDED\n");
		#endif
		
		return 0.0;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeFalseMean] ENDED with NULL\n");
	#endif
	
	return 0.0;
}

struct intervalListStruct* computeComplimentList(struct intervalListStruct* list, struct intervalListStruct* universe, double traceLength){
	if(universe){
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeComplimentList] STARTED\n");
		#endif
		struct intervalListStruct* trueList = list;
		struct intervalListStruct* falseList = NULL;
		if(trueList){
			
			if(trueList->interval->l>0){
				falseList = addIntervalToList(falseList,createIntervalList(createIntervalStruct(0.0,trueList->interval->l)));
			}
			
			while(trueList->next){
				falseList = addIntervalToList(falseList,createIntervalList(createIntervalStruct(trueList->interval->r,trueList->next->interval->l)));
				trueList = trueList->next;
			}
			
			if(trueList->interval->r < traceLength){
				falseList = addIntervalToList(falseList,createIntervalList(createIntervalStruct(trueList->interval->r,traceLength)));
			}
		} else {
			falseList = addIntervalToList(falseList,createIntervalList(createIntervalStruct(0,traceLength)));
		}
		falseList = intersectIntervalList(falseList,universe);
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeComplimentList] ENDED\n");
		#endif
		return falseList;
	}
}

void computeFalseLists(struct listOfIntervalListsStruct* intervalLists, double traceLength){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeFalseLists] STARTED\n");
	#endif
	if(intervalLists){
		while(intervalLists){
			#ifdef VERBOSE_LOW
			fprintf(logFile,"[computeFalseLists] T [%p]\n",intervalLists->trueList);
			#endif
			struct intervalListStruct* trueList = intervalLists->trueList;
			
			if(trueList){
				
				if(trueList->interval->l>0){
					intervalLists->falseList = addIntervalToList(intervalLists->falseList,createIntervalList(createIntervalStruct(0.0,trueList->interval->l)));
				}
				
				while(trueList->next){
					intervalLists->falseList = addIntervalToList(intervalLists->falseList,createIntervalList(createIntervalStruct(trueList->interval->r,trueList->next->interval->l)));
					trueList = trueList->next;
				}
				
				if(trueList->interval->r < traceLength){
					intervalLists->falseList = addIntervalToList(intervalLists->falseList,createIntervalList(createIntervalStruct(trueList->interval->r,traceLength)));
				}
			} else {
				intervalLists->falseList = addIntervalToList(intervalLists->falseList,createIntervalList(createIntervalStruct(0,traceLength)));
			}
			
			#ifdef VERBOSE_LOW
			fprintf(logFile,"[computeFalseLists] Moving on\n");
			#endif
			intervalLists = intervalLists->nextList;
		}
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeFalseLists] ENDED\n");
	#endif
}

struct listOfIntervalListsStruct* getListAtPosition(struct listOfIntervalListsStruct* intervalLists, int target){
	if(intervalLists){
		struct listOfIntervalListsStruct* iterator =  intervalLists;
		int i = 1;
		while(i < target){
			#ifdef VERBOSE_LOW
			fprintf(logFile,"\n[%d]\nFALSE: ",i);
			printIntervalListToFilePtr(iterator->falseList,logFile);fprintf(logFile,"\nTRUE: ");
			printIntervalListToFilePtr(iterator->trueList,logFile);
			#endif
			iterator = iterator->nextList;
			i++;
		}
		if(superFlag){
			fprintf(logFile,"\nIn GetList\n");
			fprintf(logFile,"\n[%d]\nFALSE: ",i);
			printIntervalListToFilePtr(iterator->falseList,logFile);
			fprintf(logFile,"\nTRUE: ");
			printIntervalListToFilePtr(iterator->trueList,logFile);
			
		}
		return iterator;
	}
	return NULL;
	
}

struct listOfIntervalListsStruct** getListsAtPosition(struct listOfIntervalListsStruct** listOfIntervalSets, int target){
	if(listOfIntervalSets){
		struct listOfIntervalListsStruct** lists = (struct listOfIntervalListsStruct**)malloc(sizeof(struct listOfIntervalListsStruct*)*traceCount);
		bzero(lists,sizeof(struct listOfIntervalListsStruct*)*traceCount);
		
		int i = 0;
		for(i=0;i<traceCount;i++){
			struct listOfIntervalListsStruct* iterator =  listOfIntervalSets[i];
			int j = 1;
			while(j < target){
				iterator = iterator->nextList;
				j++;
			}
			lists[i] = iterator;
		}
		
		return lists;
	}
	return NULL;
	
}

double computeTrueMean2(struct intervalListStruct** target, struct intervalListStruct** endMatchList){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeTrueMean2] STARTED\n");
	#endif
	
	if(endMatchList){
		int i;
		double lengthTargetTrueEM = 0.0;
		double lengthEM = 0.0;
		for(i=0;i<traceCount;i++){
			struct intervalListStruct* trueTargetEndMatchList = intersectIntervalList(target[i],endMatchList[i]);
			lengthTargetTrueEM += lengthOfIntervalList(trueTargetEndMatchList);
			deleteIntervalList(trueTargetEndMatchList);
			lengthEM += lengthOfIntervalList(endMatchList[i]);
		}
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeTrueMean2] Length of Antecedent End Match = %lf\n",lengthEM);
		fprintf(logFile,"[computeTrueMean2] Length of EM Intersection with Target(True) = %lf\n",lengthTargetTrueEM);		
		#endif
		
		double pTrue = lengthEM==0?0.0:lengthTargetTrueEM/lengthEM;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeTrueMean2] pTrue = %lf\n",pTrue);
		#endif
		
		return pTrue;
		
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeTrueMean2] ENDED with NULL\n");
		#endif
		return 0.0;
	}
}

double computeFalseMean2(struct intervalListStruct** target, struct intervalListStruct** endMatchList){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeFalseMean2] STARTED\n");
	#endif
	if(endMatchList){
		int i;
		double lengthTargetFalseEM = 0.0;
		double lengthEM = 0.0;
		for(i=0;i<traceCount;i++){
			struct intervalListStruct* falseTargetEndMatchList = intersectIntervalList(target[i],endMatchList[i]);
			lengthTargetFalseEM += lengthOfIntervalList(falseTargetEndMatchList);
			deleteIntervalList(falseTargetEndMatchList);
			lengthEM += lengthOfIntervalList(endMatchList[i]);
		}
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeFalseMean2] Length of Antecedent End Match = %lf\n",lengthEM);
		fprintf(logFile,"[computeFalseMean2] Length of EM Intersection with Target(False) = %lf\n",lengthTargetFalseEM);		
		#endif
		
		double pFalse = lengthEM==0?0.0:lengthTargetFalseEM/lengthEM;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeFalseMean2] pFalse = %lf\n",pFalse);
		#endif
		
		return pFalse;
		
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeFalseMean2] ENDED with NULL\n");
		#endif
		return 0.0;
	}
}

double computeEndMatchOverlapWithTarget(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList,int truth){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeEndMatchOverlapWithTarget] STARTED\n");
	#endif
	
	if(endMatchList){
		int i;
		double lengthTargetEM = 0.0;
		double lengthEM = 0.0;
		for(i=0;i<traceCount;i++){
			struct intervalListStruct* targetEndMatchList = intersectIntervalList(truth==0?target[i]->falseList:target[i]->trueList,endMatchList[i]);
			lengthTargetEM += lengthOfIntervalList(targetEndMatchList);
			deleteIntervalList(targetEndMatchList);
		}
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeEndMatchOverlapWithTarget] ENDED\n");
		#endif

		return lengthTargetEM;
		
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeEndMatchOverlapWithTarget] ENDED: EndMatchList EMPTY\n");
		#endif
		return 0.0;
	}
}


double computeTrueEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeTrueEntropy] STARTED\n");
	#endif
	
	if(endMatchList){
		int i;
		double lengthTargetTrueEM = 0.0;
		double lengthEM = 0.0;
		for(i=0;i<traceCount;i++){
			struct intervalListStruct* trueTargetEndMatchList = intersectIntervalList(target[i]->trueList,endMatchList[i]);
			lengthTargetTrueEM += lengthOfIntervalList(trueTargetEndMatchList);
			deleteIntervalList(trueTargetEndMatchList);
			lengthEM += lengthOfIntervalList(endMatchList[i]);
		}
		if(superFlag){
			fprintf(logFile,"[computeTrueEntropy] Length of Antecedent End Match = %lf\n",lengthEM);
			fprintf(logFile,"[computeTrueEntropy] Length of EM Intersection with Target(True) = %lf\n",lengthTargetTrueEM);		
			
		}
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeTrueEntropy] Length of Antecedent End Match = %lf\n",lengthEM);
		fprintf(logFile,"[computeTrueEntropy] Length of EM Intersection with Target(True) = %lf\n",lengthTargetTrueEM);		
		#endif
		
		double pTrue = lengthEM==0?0.0:lengthTargetTrueEM/lengthEM;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeTrueEntropy] pTrue = %lf\n",pTrue);
		#endif
		
		double H = pTrue*(pTrue==0.0?0.0:log2(pTrue));
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeTrueEntropy] H = %lf\n\n",(-1.0)*H);
		#endif

		#ifdef METHOD_CALL
		fprintf(logFile,"[computeTrueEntropy] ENDED normally\n");
		#endif
		
		return (-1.0)*H;
		
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeTrueEntropy] ENDED with NULL\n");
		#endif
		return 0.0;
	}
}

double computeFalseEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeFalseEntropy] STARTED\n");
	#endif
	if(endMatchList){
		int i;
		double lengthTargetFalseEM = 0.0;
		double lengthEM = 0.0;
		for(i=0;i<traceCount;i++){
			struct intervalListStruct* falseTargetEndMatchList = intersectIntervalList(target[i]->falseList,endMatchList[i]);
			lengthTargetFalseEM += lengthOfIntervalList(falseTargetEndMatchList);
			deleteIntervalList(falseTargetEndMatchList);
			lengthEM += lengthOfIntervalList(endMatchList[i]);
		}
		if(superFlag){
			fprintf(logFile,"[computeFalseEntropy] Length of Antecedent End Match = %lf\n",lengthEM);
			fprintf(logFile,"[computeFalseEntropy] Length of EM Intersection with Target(False) = %lf\n",lengthTargetFalseEM);		
			
		}
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeFalseEntropy] Length of Antecedent End Match = %lf\n",lengthEM);
		fprintf(logFile,"[computeFalseEntropy] Length of EM Intersection with Target(False) = %lf\n",lengthTargetFalseEM);		
		#endif
		
		double pFalse = lengthEM==0?0.0:lengthTargetFalseEM/lengthEM;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeFalseEntropy] pFalse = %lf\n",pFalse);
		#endif
		
		
		double H = pFalse*(pFalse==0.0?0.0:log2(pFalse));
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeFalseEntropy] H = %lf\n\n",(-1.0)*H);
		#endif

		#ifdef METHOD_CALL
		fprintf(logFile,"[computeFalseEntropy] ENDED normally\n");
		#endif
		
		return (-1.0)*H;
		
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeFalseEntropy] ENDED with NULL\n");
		#endif
		return 0.0;
	}
}

double computeOverlapEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList){
	if(endMatchList){
		int i;
		double lengthOverlapTargetEM = 0.0;
		double lengthEM = 0.0;
		for(i=0;i<traceCount;i++){
			struct intervalListStruct* overlapList = intersectIntervalList(target[i]->falseList,target[i]->trueList);
			struct intervalListStruct* overlapTargetEndMatchList = intersectIntervalList(overlapList,endMatchList[i]);
			lengthOverlapTargetEM += lengthOfIntervalList(overlapTargetEndMatchList);
			deleteIntervalList(overlapTargetEndMatchList);
			deleteIntervalList(overlapList);
			lengthEM += lengthOfIntervalList(endMatchList[i]);
		}
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeOverlapEntropy] Length of Antecedent End Match = %lf\n",lengthEM);
		fprintf(logFile,"[computeOverlapEntropy] Length of EM Intersection with Target(True & False) = %lf\n",lengthOverlapTargetEM);		
		#endif
		
		double pOverlap = lengthEM==0?0.0:lengthOverlapTargetEM/lengthEM;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeOverlapEntropy] pOverlap = %lf\n",pOverlap);
		#endif
		
		
		double H = (pOverlap==0.0?0.0:pOverlap*log2(pOverlap));
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeOverlapEntropy] H = %lf\n\n",(-1.0)*H);
		#endif
		
		return (-1.0)*H;
		
	} else {
		return 0.0;
	}
}

/* 
 * computeEntropy() takes a list of target truth intervals for all traces, and endmatch intervals across all traces and computes the overall entropy
 */
double computeEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList){
	if(endMatchList){
		double H = 0.0;
		double HTE,HFE,HOE;
		/*
		switch(targetBias){
			case -1:
				H = computeFalseEntropy(target,endMatchList);
				break;
				
			case +1:
				H = computeTrueEntropy(target,endMatchList);
				break;
				
			default:*/
				HTE = computeTrueEntropy(target,endMatchList);
				HFE = computeFalseEntropy(target,endMatchList);
				
				if(inputConfig->useOverlap){
					HOE = computeOverlapEntropy(target,endMatchList);
				} else HOE = 0.0;
				
				H = HTE+HFE-HOE;
				/*break;
		}
		*/
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeEntropy] H = %lf\n\n",H);
		#endif
		
		return H;
		
	} else {
		return 0.0;
	}
}


void computeMeanForAllPredicates(struct listOfIntervalListsStruct** localIntervalSet, struct truthAssignmentListStruct* constraintList, double** means, int targetPORV_id, int numTargets, int PORVCount){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeMeanForAllPredicates] STARTED\n");fflush(logFile);
	#endif
	fflush(stdout); fflush(logFile);
	if(localIntervalSet && targetBias!=0){
		//For each predicate and target position compute mean
		int predicate_id, predicateBucket, traceID;
		for(predicate_id=0;predicate_id<PORVCount-numTargets;predicate_id++){
			for(predicateBucket=0;predicateBucket<N;predicateBucket++){
				double m1,m2;
				//Get influence list for constraints when predicate is true
				struct truthAssignmentListStruct* posConstraintList = duplicateTruthAssignmentList(constraintList);
				addToTruthAssignmentList(&posConstraintList,createTruthListStruct(createTruthAssignment(predicate_id+1,1,predicateBucket)));
				
				//Get influence list for constraints when predicate is false
				struct truthAssignmentListStruct* negConstraintList = duplicateTruthAssignmentList(constraintList);
				addToTruthAssignmentList(&negConstraintList,createTruthListStruct(createTruthAssignment(predicate_id+1,0,predicateBucket)));
				
				int smallestBucketID = getSmallestBucketID(posConstraintList);
				
				//Prepare Target List
				int i;
				struct intervalListStruct** posTargetList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
				struct intervalListStruct** negTargetList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
				struct listOfIntervalListsStruct** targetList =  (struct listOfIntervalListsStruct**)malloc(sizeof(struct listOfIntervalListsStruct*)*traceCount);
				for(i=0;i<traceCount;i++){
					targetList[i] = getListAtPosition(listOfIntervalSets[i],smallestBucketID==0?targetPORV_id:numberOfPORVs+smallestBucketID);
					posTargetList[i] = targetList[i]->trueList;
					negTargetList[i] = targetList[i]->falseList;
				}

				struct intervalListStruct** posEndMatches = endMatchesForPrefix(posConstraintList);
				struct intervalListStruct** negEndMatches = endMatchesForPrefix(negConstraintList);
				fflush(logFile);fflush(stdout);
				
				#ifdef VERBOSE_STD
				fprintf(logFile,"For Constraint Set:");
				printTruthListToFilePtr(posConstraintList,logFile);
				fprintf(logFile,"\n");
				for(i=0;i<traceCount;i++){
					fprintf(logFile,"Trace[%d]: \n",i+1);
					fprintf(logFile,"Target:");
					if(posTargetList[i]==NULL) fprintf(logFile,"NULL");
					printIntervalListToFilePtr(posTargetList[i],logFile);fprintf(logFile,"\n");
					fprintf(logFile,"End-Matches:");
					if(posEndMatches[i]==NULL) fprintf(logFile,"NULL");
					printIntervalListToFilePtr(posEndMatches[i],logFile);	
					fprintf(logFile,"\n");				
				}

				fprintf(logFile,"For Constraint Set:");
				printTruthListToFilePtr(negConstraintList,logFile);
				fprintf(logFile,"\n");
				for(i=0;i<traceCount;i++){
					fprintf(logFile,"Trace[%d]: \n",i+1);
					fprintf(logFile,"Target:");
					if(negTargetList[i]==NULL) fprintf(logFile,"NULL");
					printIntervalListToFilePtr(negTargetList[i],logFile);fprintf(logFile,"\n");
					fprintf(logFile,"End-Matches:");
					if(negEndMatches[i]==NULL) fprintf(logFile,"NULL");
					printIntervalListToFilePtr(negEndMatches[i],logFile);fprintf(logFile,"\n");				
				}
				#endif

				if(targetBias>0){
					m1 = computeTrueMean2(posTargetList,posEndMatches);
					m2 = computeTrueMean2(posTargetList,negEndMatches);
					means[predicateBucket][predicate_id] = maxFloat(m1,m2);	
				} else if(targetBias<0){
					m1 = computeFalseMean2(negTargetList,posEndMatches);
					m2 = computeFalseMean2(negTargetList,negEndMatches);
					means[predicateBucket][predicate_id] = maxFloat(m1,m2);
				}
				printf("Predicate ID %d : Position %d : [%lf]\n",predicate_id,predicateBucket,means[predicateBucket][predicate_id]);
				fprintf(logFile,"[computeMeanForAllPredicates] For the following Constraint Lists:\n");
				printTruthListToFilePtr(posConstraintList,logFile);
				printTruthListToFilePtr(negConstraintList,logFile);
				fprintf(logFile,"[computeMeanForAllPredicates] Predicate ID %d : Position %d : [%lf,%lf] (%lf)\n",predicate_id,predicateBucket,m1,m2,means[predicateBucket][predicate_id]);
			}
		} 
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeMeanForAllPredicates] ENDED\n");
	fflush(logFile);
	#endif
}



double computeBinaryEntropy(struct listOfIntervalListsStruct** localIntervalSet, struct truthAssignmentListStruct* constraintList, int predicate_id, int pos, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeBinaryEntropy] STARTED\n");fflush(logFile);
	#endif
	if(localIntervalSet){
		//Get influence list for constraints when predicate is true
		
		struct truthAssignmentListStruct* posConstraintList = duplicateTruthAssignmentList(constraintList);
		addToTruthAssignmentList(&posConstraintList,createTruthListStruct(createTruthAssignment(predicate_id,1,pos)));
		struct intervalListStruct** posContraintIntervalList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
		
		//Get influence list for constraints when predicate is false
		struct truthAssignmentListStruct* negConstraintList = duplicateTruthAssignmentList(constraintList);
		addToTruthAssignmentList(&negConstraintList,createTruthListStruct(createTruthAssignment(predicate_id,0,pos)));
		struct intervalListStruct** negContraintIntervalList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
		
		int i;
		double posLength =0.0;
		double negLength =0.0;
		
		for(i=0;i<traceCount;i++){
			intervalSet = listOfIntervalSets[i];
			//printf("Calling endMatchForPrefix on Constraint List:");printTruthListToFilePtr(posConstraintList,stdout);
			posContraintIntervalList[i] = endMatchForPrefix(posConstraintList);
			//printf("Calling endMatchForPrefix on Constraint List:");printTruthListToFilePtr(negConstraintList,stdout);
			negContraintIntervalList[i] = endMatchForPrefix(negConstraintList);
			posLength += lengthOfIntervalList(posContraintIntervalList[i]);
			negLength += lengthOfIntervalList(negContraintIntervalList[i]);
			
		}
		
		int smallestBucketID = getSmallestBucketID(posConstraintList);
		
		struct listOfIntervalListsStruct** targetList =  (struct listOfIntervalListsStruct**)malloc(sizeof(struct listOfIntervalListsStruct*)*traceCount);
		for(i=0;i<traceCount;i++){
			targetList[i] = getListAtPosition(listOfIntervalSets[i],smallestBucketID==0?targetPORV_id:numberOfPORVs+smallestBucketID);
		}
		
		double totalLength = (posLength+negLength);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeBinaryEntropy] Length of Neg Influence = %lf\n",negLength);
		fprintf(logFile,"[computeBinaryEntropy] Length of Pos Influence = %lf\n",posLength);
		fprintf(logFile,"[computeBinaryEntropy] Length of Sum Influence = %lf\n",totalLength);
		fflush(logFile);
		#endif
		
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeBinaryEntropy] Computing Hpos - predidate is true\n");
		fflush(logFile);
		#endif
		if(predicate_id==4)
			superFlag=1;
		else superFlag = 0;
		double Hpos = computeEntropy(targetList,posContraintIntervalList);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeBinaryEntropy] Computing Hneg - predicate is false\n");
		fflush(logFile);
		#endif
		
		double Hneg = computeEntropy(targetList,negContraintIntervalList);
		superFlag=0;
		double pPos = totalLength==0?0:posLength/totalLength;
		double pNeg = totalLength==0?0:negLength/totalLength;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeBinaryEntropy] Hpos = %lf \n",Hpos);
		fprintf(logFile,"[computeBinaryEntropy] Hneg = %lf \n",Hneg);
		
		fprintf(stdout,"[computeBinaryEntropy] Hpos = %lf \n",Hpos);
		fprintf(stdout,"[computeBinaryEntropy] Hneg = %lf \n",Hneg);
		
		fprintf(logFile,"[computeBinaryEntropy] pPos = %lf \n",pPos);
		fprintf(logFile,"[computeBinaryEntropy] pNeg = %lf \n",pNeg);
		fprintf(logFile,"[computeBinaryEntropy] ENDED\n");
		fflush(logFile);
		#endif
		
		/*
		 * When BIAS is specified, it makes no sense to treat both branches 
		 * of the decision tree equally. On one branch, while entropy may be
		 * very low, due to the consideration of the predicate's oppoosing 
		 * truth on the alternate branch, the entropy it contributes could
		 * be excessively high (specifically when the negation is true for
		 * a larger portion of time
		 */ 
		
		//if(targetBias!=0){	// TODO: Maybe move this higher
			/*if(fabs(minFloat(Hpos,Hneg))==0.0){
			 *				
			 *				if(testCorrelationSupportForTruth(fabs(Hpos)==0.0?posConstraintList:negConstraintList,targetBias?1:0,targetPORV_id) == 0){
			 *					return maxFloat(Hpos,Hneg);
		}
		}*/
		//	return minFloat(Hpos,Hneg);//pPos*Hpos + pNeg*Hneg;//(Hpos + Hneg - Hoverlap);//minFloat(Hpos,Hneg);
		//} else {
			return pPos*Hpos + pNeg*Hneg;//(Hpos + Hneg - Hoverlap);//
		//}
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeBinaryEntropy] ENDED\n");
	fflush(logFile);
	#endif
	return (-1.0)*FLT_MAX;
}

/*
 * Compute the gain for predicate_id (start offset by 1) 
 * for target with error e and position pos
 * 
 */
double computeGain(struct listOfIntervalListsStruct** localIntervalSet, int predicate_id, int target, double e, int pos, int *trueFalseFlag, struct truthAssignmentListStruct* constraintList,int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeGain] STARTED: Predicate [%d], Target [%d], Position [%d]\n",predicate_id,target,pos);fflush(logFile);
	fprintf(stdout,"[computeGain] STARTED: Predicate [%d], Target [%d], Position [%d]\n",predicate_id,target,pos);fflush(stdout);
	#endif
	if(localIntervalSet){
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeGain] Constraint List:");printTruthListToFilePtr(constraintList,logFile);fprintf(logFile,"\n");fflush(logFile);
		#endif
		
		double gain = e; 
		double entropy = computeBinaryEntropy(localIntervalSet,constraintList,predicate_id,pos,targetPORV_id);
		gain -= entropy;
		#ifdef SUP_DEBUG
		fprintf(stdout,"[computeGain] Current Error = %lf\n",e);
		fprintf(stdout,"[computeGain] Entropy of Split [Pred %d Pos %d] = %lf\n",predicate_id,pos,entropy);
		fprintf(stdout,"[computeGain] Gain for Predicate [%d], Target [%d], Position [%d] = %lf\n",predicate_id,target,pos,gain); fprintf(stdout,"\n\n");
		fprintf(logFile,"[computeGain] ENDED\n");fflush(logFile);
		fprintf(stdout,"[computeGain] ENDED\n");fflush(stdout);
		#endif
		
		return gain;
	}
	#ifdef SUP_DEBUG
	fprintf(logFile,"[computeGain] ENDED with NULL\n");fflush(logFile);
	fprintf(stdout,"[computeGain] ENDED with NULL\n");fflush(stdout);
	#endif
	return 0.0;
}

//--------------------------------------------------CUMULATIVE GAIN-------------------------------------------------------------


double computeCumulativeTrueEntropy(struct listOfIntervalListsStruct* localIntervalSet, struct intervalListStruct* endMatchList, int smallestBucketID, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeTrueEntropy] STARTED\n");
	#endif
	
	if(endMatchList){
		struct listOfIntervalListsStruct* targetList =  getListAtPosition(localIntervalSet,targetPORV_id);
		struct intervalListStruct* trueTargetEndMatchList = NULL;
		struct intervalListStruct* partialPseudoList = NULL;
		int i=0;
		double lengthTrueEM = 0.0;
		/*
		 *		for(i=0;i<=smallestBucketID;i++){
		 *			//Compute the intersection of partial pseudo-target with endmatch
		 *			//Compute length of the intersection
		 *			//Compute overall sum
		 *			partialPseudoList = minkowskiDiffList(targetList->trueList,((double)i)*K,((double)smallestBucketID)*K);
		 *			trueTargetEndMatchList = intersectIntervalList(partialPseudoList,endMatchList);
		 *			
		 *			lengthTrueEM = lengthTrueEM + lengthOfIntervalList(trueTargetEndMatchList);
		 *			
		 *			deleteIntervalList(partialPseudoList);partialPseudoList=NULL;
		 *			deleteIntervalList(trueTargetEndMatchList);trueTargetEndMatchList=NULL;
	}
	*/
		
		trueTargetEndMatchList = intersectIntervalList(localIntervalSet->trueList,endMatchList);
		
		lengthTrueEM = lengthTrueEM + lengthOfIntervalList(trueTargetEndMatchList);
		
		double lengthEM = lengthOfIntervalList(endMatchList);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeTrueEntropy] Length True EM = %lf\n",lengthTrueEM);
		fprintf(logFile,"[computeCumulativeTrueEntropy] Length EM = %lf\n",lengthEM);
		#endif
		
		//smallestBucketID++;
		//double pTrue = lengthTrueEM/(smallestBucketID * lengthEM);
		double pTrue = lengthTrueEM/(lengthEM);
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeTrueEntropy] pTrue = %lf\n",pTrue);
		fprintf(stdout,"[computeCumulativeTrueEntropy] pTrue = %lf\n",pTrue);
		#endif
		
		
		double H = pTrue*(pTrue==0.0?0.0:log2(pTrue));
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeTrueEntropy] H = %lf\n\n",(-1.0)*H);
		#endif

		#ifdef METHOD_CALL
		fprintf(logFile,"[computeCumulativeTrueEntropy] ENDED normally\n");
		#endif
		
		return (-1.0)*H;
		
	} else {
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeTrueEntropy] ENDED with NULL\n");
		#endif
		return 0.0;
	}
}

double computeCumulativeFalseEntropy(struct listOfIntervalListsStruct* localIntervalSet, struct intervalListStruct* endMatchList, int smallestBucketID, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeFalseEntropy] STARTED\n");
	#endif
	if(endMatchList){
		struct listOfIntervalListsStruct* targetList =  getListAtPosition(localIntervalSet,targetPORV_id);
		struct intervalListStruct* falseTargetEndMatchList = NULL;
		struct intervalListStruct* partialPseudoList = NULL;
		int i=0;
		double lengthFalseEM = 0.0;
		
		/*
		 * for(i=0;i<=smallestBucketID;i++){
		 *			//Compute the intersection of partial pseudo-target with endmatch
		 *			//Compute length of the intersection
		 *			//Compute overall sum
		 *			partialPseudoList = minkowskiDiffList(targetList->falseList,((double)i)*K,((double)smallestBucketID)*K);
		 *			falseTargetEndMatchList = intersectIntervalList(partialPseudoList,endMatchList);
		 *			
		 *			lengthFalseEM = lengthFalseEM + lengthOfIntervalList(falseTargetEndMatchList);
		 *			
		 *			deleteIntervalList(partialPseudoList);partialPseudoList=NULL;
		 *			deleteIntervalList(falseTargetEndMatchList);falseTargetEndMatchList=NULL;
	}
	*/
		falseTargetEndMatchList = intersectIntervalList(localIntervalSet->falseList,endMatchList);
		lengthFalseEM = lengthFalseEM + lengthOfIntervalList(falseTargetEndMatchList);
		
		double lengthEM = lengthOfIntervalList(endMatchList);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeFalseEntropy] Length False EM = %lf\n",lengthFalseEM);
		fprintf(logFile,"[computeCumulativeFalseEntropy] Length EM = %lf\n",lengthEM);
		#endif
		
		//smallestBucketID++;
		//double pFalse = lengthFalseEM/(smallestBucketID * lengthEM);
		double pFalse = lengthFalseEM/(lengthEM);
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeFalseEntropy] pFalse = %lf\n",pFalse);
		fprintf(stdout,"[computeCumulativeFalseEntropy] pFalse = %lf\n",pFalse);
		#endif
		
		
		double H = pFalse*(pFalse==0.0?0.0:log2(pFalse));
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeFalseEntropy] H = %lf\n\n",(-1.0)*H);
		#endif

		#ifdef METHOD_CALL
		fprintf(logFile,"[computeCumulativeFalseEntropy] ENDED normally\n");
		#endif
		
		return (-1.0)*H;
		
	} else {
		#ifdef METHOD_CALL
		fprintf(logFile,"[computeCumulativeFalseEntropy] ENDED with NULL\n");
		#endif
		return 0.0;
	}
}

double computeCumulativeOverlapEntropy(struct listOfIntervalListsStruct* localIntervalSet, struct intervalListStruct* endMatchList, int smallestBucketID, int targetPORV_id){
	if(endMatchList){
		struct listOfIntervalListsStruct* targetList =  getListAtPosition(localIntervalSet,targetPORV_id);
		struct intervalListStruct* overlapTargetEndMatchList = NULL;
		struct intervalListStruct* partialTruePseudoList = NULL;
		struct intervalListStruct* partialFalsePseudoList = NULL;
		struct intervalListStruct* overlapList = NULL;
		int i=0;
		double lengthOverlapTargetEM = 0.0;
		/*for(i=0;i<=smallestBucketID;i++){
		 *			//Compute the intersection of partial pseudo-target with endmatch
		 *			//Compute length of the intersection
		 *			//Compute overall sum
		 *			partialTruePseudoList = minkowskiDiffList(targetList->trueList,((double)i)*K,((double)smallestBucketID)*K);
		 *			partialFalsePseudoList= minkowskiDiffList(targetList->falseList,((double)i)*K,((double)smallestBucketID)*K);
		 *			overlapList = intersectIntervalList(partialTruePseudoList,partialFalsePseudoList);
		 *			overlapTargetEndMatchList = intersectIntervalList(overlapList,endMatchList);
		 *			
		 *			lengthOverlapTargetEM = lengthOverlapTargetEM + lengthOfIntervalList(overlapTargetEndMatchList);
		 *			
		 *			deleteIntervalList(partialTruePseudoList);partialTruePseudoList=NULL;
		 *			deleteIntervalList(partialFalsePseudoList);partialFalsePseudoList=NULL;
		 *			deleteIntervalList(overlapList);overlapList=NULL;
		 *			deleteIntervalList(overlapTargetEndMatchList);overlapTargetEndMatchList=NULL;
	}
	*/
		overlapList = intersectIntervalList(localIntervalSet->falseList,localIntervalSet->trueList);
		overlapTargetEndMatchList = intersectIntervalList(overlapList,endMatchList);
		lengthOverlapTargetEM = lengthOverlapTargetEM + lengthOfIntervalList(overlapTargetEndMatchList);
		
		double lengthEM = lengthOfIntervalList(endMatchList);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeOverlapEntropy] Length Overlap EM = %lf\n",lengthOverlapTargetEM);
		fprintf(logFile,"[computeCumulativeOverlapEntropy] Length EM = %lf\n",lengthEM);
		#endif
		
		//smallestBucketID++;
		//double pOverlap = lengthOverlapTargetEM/(smallestBucketID * lengthEM);
		double pOverlap = lengthOverlapTargetEM/(lengthEM);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeOverlapEntropy] pOverlap = %lf\n",pOverlap);
		fprintf(stdout,"[computeCumulativeOverlapEntropy] pOverlap = %lf\n",pOverlap);
		#endif
		
		
		double H = (pOverlap==0.0?0.0:pOverlap*log2(pOverlap));
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeOverlapEntropy] H = %lf\n\n",(-1.0)*H);
		#endif
		
		return (-1.0)*H;
		
	} else {
		return 0.0;
	}
}


double computeCumulativeEntropy(struct listOfIntervalListsStruct* localIntervalSet, struct intervalListStruct* endMatchList, int smallestBucketID,int targetPORV_id){
	if(endMatchList){
		
		double H = 0.0;
		switch(targetBias){
			case -1:
				H = computeCumulativeFalseEntropy(localIntervalSet,endMatchList,smallestBucketID,targetPORV_id);
				break;
				
			case +1:
				H = computeCumulativeTrueEntropy(localIntervalSet,endMatchList,smallestBucketID,targetPORV_id);
				break;
				
			default:
				H = computeCumulativeTrueEntropy(localIntervalSet,endMatchList,smallestBucketID,targetPORV_id) + computeCumulativeFalseEntropy(localIntervalSet,endMatchList,smallestBucketID,targetPORV_id) - computeCumulativeOverlapEntropy(localIntervalSet,endMatchList,smallestBucketID,targetPORV_id);
				break;
		}
		// pTrue*(pTrue==0.0?0.0:log2(pTrue)) + pFalse*(pFalse==0.0?0.0:log2(pFalse)) - (pOverlap==0.0?0.0:pOverlap*log2(pOverlap));
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeEntropy] H = %lf\n\n",H);
		#endif
		
		return H;//(-1.0)*H;
		
	} else {
		return 0.0;
	}
}

double computeCumulativeBinaryEntropy(struct listOfIntervalListsStruct* localIntervalSet, struct truthAssignmentListStruct* constraintList, int predicate_id, int pos, double* falseEntropy, double* trueEntropy,int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeCumulativeBinaryEntropy] STARTED\n");fflush(logFile);
	#endif
	if(localIntervalSet){
		fprintf(logFile,"numberOfPORVs = %d\n",numberOfPORVs);fflush(logFile);
		//Get influence list for constraints when predicate is true
		struct truthAssignmentListStruct* posConstraintList = duplicateTruthAssignmentList(constraintList);
		
		fprintf(logFile,"[computeCumulativeBinaryEntropy] ENTROPY 1\n");fflush(logFile);
		
		addToTruthAssignmentList(&posConstraintList,createTruthListStruct(createTruthAssignment(predicate_id,1,pos)));
		
		fprintf(logFile,"[computeCumulativeBinaryEntropy] ENTROPY 2\n");fflush(logFile);
		
		struct intervalListStruct* posContraintIntervalList = endMatchForPrefix(posConstraintList);
		
		fprintf(logFile,"[computeCumulativeBinaryEntropy] ENTROPY 3\n");fflush(logFile);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Positive Influence: ");
		printIntervalListToFilePtr(posContraintIntervalList,logFile); fprintf(logFile,"\n");fflush(logFile);
		#endif		
		
		//Get influence list for constraints when predicate is false
		struct truthAssignmentListStruct* negConstraintList = duplicateTruthAssignmentList(constraintList);
		addToTruthAssignmentList(&negConstraintList,createTruthListStruct(createTruthAssignment(predicate_id,0,pos)));
		struct intervalListStruct* negContraintIntervalList = endMatchForPrefix(negConstraintList);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeBinaryEntropy] NegativeInfluence: ");
		printIntervalListToFilePtr(negContraintIntervalList,logFile); fprintf(logFile,"\n");fflush(logFile);
		#endif
		
		int smallestBucketID = getSmallestBucketID(posConstraintList);
		
		double posLength = lengthOfIntervalList(posContraintIntervalList);
		double negLength = lengthOfIntervalList(negContraintIntervalList);
		
		double totalLength = (posLength+negLength);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Length of Neg Influence = %lf\n",negLength);
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Length of Pos Influence = %lf\n",posLength);
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Length of Sum Influence = %lf\n",totalLength);
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Computing Hpos - predidate is true\n");
		fflush(logFile);
		#endif
		
		double Hpos = computeCumulativeEntropy(localIntervalSet,posContraintIntervalList,smallestBucketID,targetPORV_id);
		*trueEntropy = Hpos;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Computing Hneg - predicate is false\n");
		fflush(logFile);
		#endif
		
		double Hneg = computeCumulativeEntropy(localIntervalSet,negContraintIntervalList,smallestBucketID,targetPORV_id);
		*falseEntropy = Hneg;
		
		double pPos = posLength/totalLength;
		double pNeg = negLength/totalLength;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Hpos = %lf \n",Hpos);
		fprintf(logFile,"[computeCumulativeBinaryEntropy] Hneg = %lf \n",Hneg);
		fprintf(logFile,"[computeCumulativeBinaryEntropy] pPos = %lf \n",pPos);
		fprintf(logFile,"[computeCumulativeBinaryEntropy] pNeg = %lf \n",pNeg);
		#endif

		#ifdef METHOD_CALL
		fprintf(logFile,"[computeCumulativeBinaryEntropy] ENDED\n");
		fflush(logFile);
		#endif
		
		return pPos*Hpos + pNeg*Hneg;
		
	}
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeCumulativeBinaryEntropy] ENDED\n");
	fflush(logFile);
	#endif
	
	return (-1.0)*FLT_MAX;
}

/*
 * Compute the gain for predicate_id (start offset by 1) 
 * for target with error e and position pos
 * 
 */
double computeCumulativeGain(struct listOfIntervalListsStruct* localIntervalSet, int predicate_id, int target, double e, int pos, int *trueFalseFlag, struct truthAssignmentListStruct* constraintList, double* falseEntropy, double* trueEntropy,int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeCumulativeGain] STARTED: Predicate [%d], Target [%d], Position [%d]\n",predicate_id,target,pos);fflush(logFile);
	fprintf(stdout,"[computeCumulativeGain] STARTED: Predicate [%d], Target [%d], Position [%d]\n",predicate_id,target,pos);fflush(stdout);
	#endif

	if(localIntervalSet){
		#ifdef SUP_DEBUG
		fprintf(logFile,"[computeCumulativeGain] Constraint List:");printTruthListToFilePtr(constraintList,logFile);fprintf(logFile,"\n");fflush(logFile);
		#endif
		
		double gain = e; 
		double entropy = computeCumulativeBinaryEntropy(localIntervalSet,constraintList,predicate_id,pos,falseEntropy,trueEntropy,targetPORV_id);
		gain -= entropy;
		#ifdef SUP_DEBUG
		fprintf(stdout,"[computeCumulativeGain] Current Error = %lf\n",e);
		fprintf(stdout,"[computeCumulativeGain] Entropy of Split [Pred %d Pos %d] = %lf\n",predicate_id,pos,e);
		fprintf(stdout,"[computeCumulativeGain] Gain for Predicate [%d], Target [%d], Position [%d] = %lf\n",predicate_id,target,pos,gain); fprintf(stdout,"\n\n");
		#endif 

		#ifdef METHOD_CALL
		fprintf(logFile,"[computeCumulativeGain] ENDED\n");fflush(logFile);
		fprintf(stdout,"[computeCumulativeGain] ENDED\n");fflush(stdout);
		#endif
		
		return gain;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeCumulativeGain] ENDED with NULL\n");fflush(logFile);
	fprintf(stdout,"[computeCumulativeGain] ENDED with NULL\n");fflush(stdout);
	#endif
	return 0.0;
}

//------------------------------------------------------------------------------------------------------------------------------


void computeAllGains(struct listOfIntervalListsStruct** localIntervalSet, int target, int *trueFalseFlag, double* bestGain, int i, int j, int PORVCount, struct treeNode* currentNode, double* falseEntropy, double* trueEntropy, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeAllGains] STARTED: Predicate [%d]\n",j+1);fflush(logFile);
	#endif
	
	if(localIntervalSet){
		if(cumulative==0)
			*bestGain = computeGain(localIntervalSet,j+1,i==0?target:PORVCount+i,currentNode->error,i,trueFalseFlag,currentNode->truthList,targetPORV_id);
		//else
		//	*bestGain = computeCumulativeGain(localIntervalSet,j+1,i==0?target:PORVCount+i,currentNode->error,i,trueFalseFlag,currentNode->truthList,falseEntropy,trueEntropy);
		
	}
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeAllGains] ENDED Gain = %lf\n",*bestGain);fflush(logFile);
	#endif	
}

/*
 * findBestGain : Computes the tree node corresponding to 
 * the choice of a predicate and its truth
 * 
 * Also populates the means and errors statistics for the node
 * When a predicate is identified, the splittingPredicate_id field
 * is updated in the current node.
 * 
 */
struct treeNode* findBestGain(struct listOfIntervalListsStruct** localIntervalSets,struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int PORVCount,int numTargets, int N, struct treeNode* currentNode, int targetPORV_id){
	//Compute gain for all PORVs and all targets
	//N is the number of delays to be considered (0 included , it would be 0 to N-1)
	//PORVCount is the number of PORVs in the system.
	#ifdef METHOD_CALL
	fprintf(logFile,"[findBestGain] STARTED\n");
	printTruthListToFilePtr(currentNode->truthList,logFile);
	#endif
	
	if(localIntervalSets && currentNode){
		//Get list of intervals where the current constraints have influence
		int smallestBucketID = getSmallestBucketID(currentNode->truthList);
		
		struct intervalListStruct** endMatchIntervalList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
		bzero(endMatchIntervalList,sizeof(struct intervalListStruct*)*traceCount);
		
		int i;
		double endMatchTraceLength[traceCount];
		double traceLengthTargetTrue = 0.0;
		double traceLengthTargetFalse = 0.0;
		double totalEndMatchLength = 0.0;
		for(i=0;i<traceCount;i++){
			intervalSet = listOfIntervalSets[i];
			//printf("Calling endMatchForPrefix on Constraint List:");printTruthListToFilePtr(currentNode->truthList,stdout);printf("\n");printListOfIntervalLists(intervalSet);
			endMatchIntervalList[i] = endMatchForPrefix(currentNode->truthList);//printf("End Match:");printIntervalListToFilePtr(endMatchIntervalList[i],stdout);if(!endMatchIntervalList[i])printf("NULL\n"); else printf("\n");
			endMatchTraceLength[i] = lengthOfIntervalList(endMatchIntervalList[i]);//printf("LENGTH=%lf\n",endMatchTraceLength[i]);
			totalEndMatchLength+=endMatchTraceLength[i];
			
			//Get the begin match for the target
			//TODO: This is a conservative for now - but may be made optimistic later
			struct listOfIntervalListsStruct* temp =  getListAtPosition(localIntervalSets[i],smallestBucketID==0?targetPORV_id:numberOfPORVs+smallestBucketID);
			
			struct intervalListStruct* influenceListTargetTrue = intersectIntervalList(endMatchIntervalList[i],temp->trueList);
			
			//fprintf(stdout,"HERE FG 2\n");fflush(stdout);
			struct intervalListStruct* influenceListTargetFalse = intersectIntervalList(endMatchIntervalList[i],temp->falseList);
			
			//fprintf(stdout,"HERE FG 3\n");fflush(stdout);
			
			#ifdef VERBOSE_HIGH
			fprintf(logFile,"[findBestGain] CONSTRAINT LIST = ");printTruthListToFilePtr(currentNode->truthList,logFile);fprintf(logFile,"\n");		
			fprintf(logFile,"[findBestGain] INFLUENCE  LIST TARGET TRUE = ");printIntervalListToFilePtr(influenceListTargetTrue,logFile);fprintf(logFile,"\n");
			fprintf(logFile,"[findBestGain] INFLUENCE  LIST TARGET FALSE= ");printIntervalListToFilePtr(influenceListTargetFalse,logFile);fprintf(logFile,"\n");
			#endif
			
			//Trace length applicable for the present interval set
			traceLengthTargetTrue += lengthOfIntervalList(influenceListTargetTrue);
			//fprintf(stdout,"HERE FG 4\n");fflush(stdout);
			traceLengthTargetFalse += lengthOfIntervalList(influenceListTargetFalse);
			//fprintf(stdout,"HERE FG 5\n");fflush(stdout);
		}
		currentNode->traceLength = totalEndMatchLength;
		if(totalEndMatchLength==0){
			return currentNode;
		}
		double currentMeanTrue = totalTrueLength/totalEndMatchLength;
		double currentMeanFalse = totalFalseLength/totalEndMatchLength;
		
		//---------------------------------------THE CODE ABOVE SEEMS UNNECESSARY---------------------------------//
		
		
		//fprintf(stdout,"HERE FG 6\n");fflush(stdout);
		//Computing Gain from known predicates
		
		double modGains[N][PORVCount];
		double entropy[PORVCount][N*2];
		//double means[N][PORVCount];
		double **means = (double**)malloc(sizeof(double*)*N);
		for(i=0;i<N;i++){
			means[i] = (double*)malloc(sizeof(double)*PORVCount);
		}
		bzero(entropy,(N*2*PORVCount)*sizeof(double));	//Zero'ing things out so no segfaults take place
		bzero(modGains,(N*PORVCount)*sizeof(double));	//Zero'ing things out so no segfaults take place
		//bzero(means,(N*PORVCount)*sizeof(double));		//Zero'ing things out so no segfaults take place
		int bestTarget = -1;                           	//The winning gain IDs for [0]: [1]: 
		int bestPORV = -1;
		int bestTruthType = -1;
		double bestVal = (-1.0)*FLT_MAX;		//Winning gain value (This is maximized)
		int bestTrueFalseFlag = 0;			/*When the error is zero, this flag is a 0 if the 
		*winning position has one or no contributing 
		*assertion. Else it is 1 if both the false and true
		computeBucketSeparation							 *target have a zero error.
		*/
		#ifdef VERBOSE_HIGH
		fprintf(logFile,"[findBestGain] PORVCount = %d, N = %d\n",PORVCount,N);
		#endif
		//For each predicate, target position, place it in the constraint set 
		//and compute a tentative endmatch. Use this for mean and error
		
		//Initialization - Compute Mean and Error for the target at each of the N delay positions
		i = 0;
		int j = 0;
		int bi,bj,bt;
		double bg = (-1.0)*FLT_MAX;
		bi=0;
		bj=0;
		bt=0;
		printf("Computing all means\n");
		computeMeanForAllPredicates(localIntervalSets,currentNode->truthList,means,targetPORV_id,numTargets,PORVCount);
		printf("Done Computing all means\n");
		int updateBestPredicate = 1;
		int updatedBestPredicate = 0;
		if(learnPredicates<=2){
			//Evaluate Knowledge
			//Loop through all predicate - temporal positions and determine which combination produces the best gain.
			for(i=0;i<N;i++){//For each potential target
				for(j=0;j<PORVCount-numTargets;j++){
					/*
					 * For each PORV compute gain
					 * 
					 * j+1 is the PORV being considered in the list
					 * printf("Index In List [%d,%d] = %d\n",i,j+1,indexInList(currentNode->explored,j+1,i));
					 * printIndexCoupleList(currentNode->explored);
					 * 
					 */				
					/*
					 * This line prevents the target from being used in causal checks
					 * It also prevents a causing predicate that has already been 
					 * considered from being used.
					 * However this should be modified sometime to check for the pair 
					 * <Predicate,temporal position> and eliminate from consideration 
					 * based on in which relation the causality has already been explored
					 */
					
					//printf("currentNode->truthList = [%p]\n",currentNode->truthList);
					
					//Change this to if((i==0 && j==target-1) || predicateInList(currentNode->truthList,j+1)) continue;
					
					//if(j==target-1) continue;
					
					//predicate is j
					//target position is i
					//False Target
					//entropy[j][2*i] = 
					//True Target
					//entropy[j][2*i+1] = 
					
					
					if(j==targetPORV_id-1 || predicateTargetInList(currentNode->truthList,j+1,i) || ignorePredicate(j)) continue; 	//TODO: Make more specific
					
					if(indexInList(currentNode->explored,j+1,i)==1) continue;
					//printf("buck=%d pred=%d\n",i,j+1);
					#ifdef VERBOSE_LOW
					fprintf(logFile,"[findBestGain] Computing Gain For PORV P[%d] with Target at position [%d] \n",j+1,i);fflush(logFile);
					#endif
					
					int trueFalseFlag = 0;
					
					//Compute gain for target position i and PORV j = two gain computations: one for 2*i (true) and 2*i+1 (false)
					computeAllGains(localIntervalSets, target, &trueFalseFlag, &modGains[i][j], i, j, PORVCount, currentNode,&entropy[j][2*i],&entropy[j][2*i+1],targetPORV_id);
					
					#ifdef VERBOSE_STD
					fprintf(logFile,"[findBestGain] Computed Gain: P-%d , Target Position: %d =  %lf\n",j+1,i,modGains[i][j]);fflush(logFile);
					#endif
					
					if(targetBias!=0){
						updateBestPredicate = (means[i][j]-currentNode->mean)>0;
					}
					if(inputConfig->objective && updateBestPredicate){
						if(modGains[i][j]>bg){
							bg = modGains[i][j];
							bi = i;
							bj = j;
						}
						updatedBestPredicate = 1;

						#ifdef VERBOSE_STD
						//#ifdef VERBOSE_LOW
						fprintf(logFile,"[findBestGain] : PORV %d TARGET POSITION %d : Gain %lf, (Best Gain [%lf])\n",j+1,i,modGains[i][j],bg);fflush(logFile);
						#endif
					}
					
				}
				
			}
			//If not possible to improve both gain and mean, then first improve the mean
			if(!updatedBestPredicate) bg = 0.0;

			if(targetBias!=0 && !updatedBestPredicate){
				#ifdef VERBOSE_STD
					//#ifdef VERBOSE_LOW
					fprintf(logFile,"[findBestGain] : Not possible to improve gain and mean.\n");fflush(logFile);
				#endif

				double M=currentNode->mean;
				//Loop through all predicate - temporal positions and determine which combination produces the best gain.
				for(i=0;i<N;i++){//For each potential target
					for(j=0;j<PORVCount-numTargets;j++){
						if(j==targetPORV_id-1 || predicateTargetInList(currentNode->truthList,j+1,i) || ignorePredicate(j)) continue; 	//TODO: Make more specific
						if(indexInList(currentNode->explored,j+1,i)==1) continue;
						if(means[i][j]>=M){
							bi = i;
							bj = j;
							M = means[i][j];
							bg = 1.0;
						}
					}
				}
			}

			//Evaluate all predicate gain curves and choose predicate with the highest total gain
			if(inputConfig->objective==0){
				double totalGain[PORVCount];
				int predHighestGain = 0;
				double highestTotalGain = 0.0;
				for(j=0;j<PORVCount;j++){//For each PORV compute gain
					totalGain[j] = 0.0;
					for(i=0;i<N;i++){//For each potential target
						totalGain[j] += modGains[i][j];
					}
					if(highestTotalGain<totalGain[j]){
						predHighestGain = j;
						highestTotalGain = totalGain[j];
					}
				}
				
				bg=0.0; bj=predHighestGain;
				for(i=0;i<N;i++){//For each potential target
					if(modGains[i][bj] > bg){
						bg = modGains[i][bj];
						bi = i;
					}
				}
			}
			
			if(bj==targetPORV_id-1){
				bg=-1;
			}
			
			//printf("Smallest BucketId = %d\n",smallestBucketID);
			/*for(i=0;i<N;i++){
			 *				printf("Gain[%d][2] = %lf\n",i,modGains[i][2]);
		}*/
			//copyMatrix(currentNode->gains,N*2,PORVCount,gainVal);
			#ifdef VERBOSE_STD
			fprintf(logFile,"[findBestGain] Gain Table\n");
			printFloat2DArraryToFile(N,PORVCount,modGains,logFile);fflush(logFile);
			//choicePause();
			fprintf(logFile,"[findBestGain] Outside: i=%d,j=%d and bi=%d\n",i,j,bi);
			fprintf(logFile,"[findBestGain] Under the Constraint List:");printTruthListToFilePtr(currentNode->truthList,logFile);//fprintf(logFile,"\n");
			fprintf(logFile,"[findBestGain] Best Gain[%lf]: Position[%d] PORV [%d].\n",bg,bi,bj+1);
			fprintf(logFile,"[findBestGain] GAINS computed:\n");
			fflush(logFile);
			#endif
			
		}
		
		
		/*
		 *		struct intervalListStruct** buckets = computeBuckets(currentNode);
		 *		
		 *		int bucketCount = -1;
		 *		if(currentNode->truthList)
		 *			bucketCount = currentNode->truthList->asgmt->position;
		 *		
		 *		struct intervalListStruct* trueList = getListAtPosition(localIntervalSets[0],target)->trueList;
		 *		//fprintf(logFile,"YO-4-HERE-1-1-1\n");fflush(logFile);
		 *		if(predicateMap){
		 *			//fprintf(logFile,"YO-HERE-1-1-1\n");fflush(logFile);
		 *			printRequestForNewPredicate(inputConfig,predicateMap->varList,buckets,bucketCount,trueList);
	}
	static int testDepth=5;
	
	if(currentNode && currentNode->truthList && testDepth ==0){
		//exit(0);
	} else if(currentNode && currentNode->truthList){
		testDepth--;
	}
	*/
		//fprintf(stdout,"HERE FG 7\n");fflush(stdout);
		//If best gain from knowledge is zero (Level 0) or if the learn level is non-zero
		//if(bg<=0.0 || learnPredicates>0){ 
		if((bg<=0.0 && learnPredicates==1) || learnPredicates>1){
			//LEARN SOMETHING NEW :-)
			
			//Initialize Objects
			deletePredicateDetails(details);
			details = NULL;
			
			struct intervalListStruct** buckets = computeBuckets(currentNode);
			//fprintf(logFile,"YO-1-HERE-1-1-1\n");fflush(logFile);
			//File from which to read predicates
			if(learnedOPin){
				fprintf(logFile,"INSIDE\n");fflush(logFile);
				fclose(learnedOPin);
				
			}
			//fprintf(logFile,"YO-2-HERE-1-1-1\n");fflush(logFile);
			
			int bucketCount = -1;
			if(currentNode->truthList)
				bucketCount = currentNode->truthList->asgmt->position;
			
			//fprintf(logFile,"YO-3-HERE-1-1-1\n");fflush(logFile);
			/*
			 *			struct intervalListStruct* trueList = getListAtPosition(localIntervalSet,target)->trueList;
			 *			//fprintf(logFile,"YO-4-HERE-1-1-1\n");fflush(logFile);
			 *			if(predicateMap){
			 *				//fprintf(logFile,"YO-HERE-1-1-1\n");fflush(logFile);
			 *				printRequestForNewPredicate(inputConfig,predicateMap->varList,buckets,bucketCount,trueList);
		}
		//fprintf(logFile,"YO-5-HERE-1-1-1\n");fflush(logFile);
		
		learnNewPredicates();
		
		#ifdef SUP_DEBUG
		printPredicateDetailListToFilePtr(details,logFile);
		#endif
		*/
			details = NULL;
			if(details){
				//Is what was learned worthwhile? If so advise to make a decision on the learned predicate.
				/*if( details->gain > 0.0 && ( (learnPredicates>=2) || (learnPredicates==1 && bg==0.0) ) ){//details->gain>bg) ) ){
					int atomID = getIdentifierID(idList,details->predicate->porv->LHS);
					if(atomID==0){
						printf("ERROR: No variable with name [%s] declared\n",details->predicate->porv->LHS);
						exit(0);
					}
					learnedPORVCount++;
					struct PORV* learnedPred = createPORV(duplicateConditionList(details->predicate->porv),learnedPORVCount,atomID);
					learnedPORVs = addPORVToList(learnedPORVs,learnedPred);
					learnedIntervalSet = addListToListOfIntervalLists(learnedIntervalSet,createListOfIntervalLists(duplicateIntervalList(details->trueList),computeComplimentList(details->trueList,createIntervalList(createIntervalStruct(0,totalEndMatchLength)),totalEndMatchLength)));
					
					fprintf(predLogFile,"Added Predicate: LP%d = {",learnedPred->id);
					printConditionToFilePtr(learnedPred->porv,predLogFile);
					fprintf(predLogFile,"}\nTruthList = {");
					printIntervalListToFilePtr(details->trueList,predLogFile); fprintf(predLogFile,"\n");
					fflush(predLogFile);
					
					currentNode->targetInfluence = details->bucket;
					currentNode->splittingPredicate_id = learnedPORVCount;
					currentNode->predType = 1;
					
					endMatchIntervalList = endMatchForPrefix(currentNode->truthList);
					currentNode->traceLength = lengthOfIntervalList(endMatchIntervalList);
					
					struct indexCouple* pred_target = createIndexCouple(currentNode->targetInfluence,currentNode->splittingPredicate_id);
					pred_target->type = 1;		// To indicate that this predicate was learned.
					addToIndexCoupleList(&(currentNode->explored),pred_target);
					
				}*/
			} else if(learnPredicates<=2) {
				currentNode->targetInfluence = bi;
				currentNode->splittingPredicate_id = bj+1;
				currentNode->predType = 0;
				//endMatchIntervalList = endMatchForPrefix(currentNode->truthList);
				//currentNode->traceLength = lengthOfIntervalList(endMatchIntervalList);
				
				addToIndexCoupleList(&(currentNode->explored),createIndexCouple(currentNode->targetInfluence,currentNode->splittingPredicate_id));
				
			} else {// Nothing new to be learned  - learnPredicates == 3
				currentNode->splittingPredicate_id = -1;
				return currentNode;
			}
		} else {//No need for learning.
			fprintf(logFile,"No learning\n");fflush(logFile);
			if(bg==-1){//We should never enter here anymore. May consider removing this code eventually.
				currentNode->targetInfluence = 0;
				currentNode->splittingPredicate_id = -1;
				currentNode->predType = 0;
				
				fprintf(logFile,"No further gain improvement is possible at this node [ID = %d].\n",currentNode->id);
				
			} else if(bg!=0.0){
				currentNode->targetInfluence = bi;
				currentNode->splittingPredicate_id = bj+1;
				currentNode->predType = 0;
				//endMatchIntervalList = endMatchForPrefix(currentNode->truthList);
				//currentNode->traceLength = lengthOfIntervalList(endMatchIntervalList);
				
				addToIndexCoupleList(&(currentNode->explored),createIndexCouple(currentNode->targetInfluence,currentNode->splittingPredicate_id));
			}
		}
		//fprintf(logFile,"HERE FG 9\n");fflush(logFile);
		//printFloat2DArraryToFile(N*2,PORVCount,gainVal,logFile);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[findBestGain] NODE STATS: \n\t[Mean = %lf]\n\t[Error = %lf]\n\tTarget = %d\n\tSplitting On Predicate = %d\n",currentNode->mean,currentNode->error,currentNode->targetInfluence,currentNode->splittingPredicate_id);fflush(logFile);
		printTruthListToFilePtr(currentNode->truthList,logFile);
		#endif
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[findBestGain] ENDED\n");fflush(logFile);
		#endif
		
		return currentNode;
	}
	#ifdef METHOD_CALL
	printTruthListToFilePtr(currentNode->truthList,logFile);
	fprintf(logFile,"[findBestGain] ENDED\n");fflush(logFile);
	#endif

	return currentNode;
	
}

//Minkowski Methods
struct intervalStruct* minkowskiSum(struct intervalStruct* interval, double l, double r){
	if(interval){
		return createIntervalStruct(interval->l+l,interval->r+r);
	}
	return NULL;
}

struct intervalStruct* minkowskiDiff(struct intervalStruct* interval, double l, double r){	
	if(interval){
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"interval->l = [%lf] \t interval->r = [%lf]\n",interval->l, interval->r);
		fprintf(logFile,"l = [%lf] \t r = [%lf] \n",l,r);
		fprintf(logFile,"Left = [%lf] \t Right = [%lf]\n",((interval)->l)-r,((interval)->r)-l);
		#endif
		
		return createIntervalStruct(
			(((interval)->l-r)>=0.0)?((interval)->l)-r:0.0,
									(((interval)->r-l)>=0.0)?((interval)->r)-l:0.0
		);
	}
	return NULL;
}

struct intervalStruct* minmaxDiff(struct intervalStruct* interval, double l, double r){        
	if(interval){
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"interval->l = [%lf] \t interval->r = [%lf]\n",interval->l, interval->r);
		fprintf(logFile,"l = [%lf] \t r = [%lf] \n",l,r);
		fprintf(logFile,"Left = [%lf] \t Right = [%lf]\n",((interval)->l)-l,((interval)->r)-r);
		#endif
		
		return createIntervalStruct(
			(((interval)->l-l)>=0.0)?((interval)->l)-l:0.0,
									((interval)->l-l) > (((interval)->r)-r)?
									((((interval)->l-l)>=0.0)?((interval)->l)-l:0.0):
									((((interval)->r-r)>=0.0)?((interval)->r)-r:0.0));
		
		
	}
	return NULL;
}


struct intervalStruct* minkowskiIntervalSum(struct intervalStruct* interval1, struct intervalStruct* interval2){
	if(interval1 && interval2){
		return minkowskiSum(interval1,interval2->l,interval2->r);
	}
	return NULL;
}

struct intervalStruct* minkowskiIntervalDiff(struct intervalStruct* interval1, struct intervalStruct* interval2){
	if(interval1 && interval2){
		return minkowskiDiff(interval1,interval2->l,interval2->r);
	}
	return NULL;	
}

struct intervalListStruct* minkowskiSumList(struct intervalListStruct* list, double l, double r){
	if(list){
		struct intervalListStruct* sum = NULL;
		while(list){
			sum = addIntervalToList(sum,createIntervalList(minkowskiSum(list->interval,l,r)));
			list = list->next;
		}
		return sum;
	}
	return NULL;
}

struct intervalListStruct* minkowskiDiffList(struct intervalListStruct* list, double l, double r){
	if(list){
		struct intervalListStruct* diff = NULL;
		while(list){
			diff = addIntervalToList(diff,createIntervalList(minkowskiDiff(list->interval,l,r)));
			list = list->next;
		}
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"\n");printIntervalListToFilePtr(diff,logFile);fprintf(logFile,"\n");
		#endif
		
		return flattenIntervalList(diff);
	}
	return NULL;
}
//edited
_Bool is_stop(struct treeNode* root, int target,struct listOfIntervalListsStruct** listOfIntervalSets)
{
    printTruthList(root->truthList);
    printf("initial mean=%lf\n",root->mean);
    struct intervalListStruct** endMatchIntervalList = endMatchesForPrefix(root->truthList);
	/*for(int i=0;i<traceCount;i++){
			printListOfIntervalLists(endMatchIntervalList[i]);
		}*/
    int mininmumBucketID = getSmallestBucketID(root->truthList);
    int targetID = mininmumBucketID==0?target:(numberOfPORVs+mininmumBucketID);
    double trueMean, falseMean, Htrue, Hfalse,mean, error;
    double intervalLength = 0.0;
    if(endMatchIntervalList){
        intervalLength = lengthOfIntervalLists(endMatchIntervalList);
    }
	//printf("IL=%lf\n",intervalLength);
    int dominantTruth;
    trueMean = computeMean(listOfIntervalSets,targetID,intervalLength,endMatchIntervalList);
    falseMean = computeFalseMean(listOfIntervalSets,targetID,intervalLength,endMatchIntervalList);
    Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList);
    Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList);
    dominantTruth = Htrue<=Hfalse?1:0;//error(trueMean)<error(falseMean)?1:0;
    mean = dominantTruth==0?falseMean:trueMean;
    error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList));
	
	root->error = error;
	root->mean = mean;
	
    if(root->traceLength>0.0 && (mean == 0.0 || mean == 1.0) && fabs(error) == 0.0){
		//printf("IL=%lf updated mean=%lf\n",intervalLength,mean);
		//if(mean == 0.0) return -1;
		return 1;
		
    }
    return 0;
}
_Bool is_leaf(struct treeNode *root,int depth)
{
    if(depth==0 || (root != NULL && root->left == NULL && root->right == NULL) ) return 1;
    return 0;
}
//int amsDupMine(struct treeNode* , struct treeNode* , int , int , int , int );
void updateLeaf(struct treeNode* root,struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int numberOfPORVs,int numTargets, int N)
{
    root = findBestGain(root->listOfIntervalSets,pseudoTargetLists,target,numberOfPORVs,numTargets,N,root,target);
}

int amsMine2(struct treeNode* original, struct treeNode* duproot,struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int numberOfPORVs,int numTargets, int N, int origDepth, int dupDepth,int isroot,struct listOfIntervalListsStruct** listOfIntervalSets){
    
    if(isroot==0){
	    int ret;
	    if(ret=is_stop(duproot,target,listOfIntervalSets))
	    {
			//report assertion
			FILE* fp;
			if(assertFileName==NULL){
				fp = fopen("assertions.txt","a");
			} else {
				//printf("filename = [%s]\n");
				fp = fopen(assertFileName,"a");
			}
			printTruthList(duproot->truthList);
			//printf("ret=%d\n",ret);
			printf("assertion found at origDepth=%d dupDepth=%d target=%d\n",origDepth,dupDepth,target);
			printAssertions(duproot,fp,target);
			return 1;
	    }
	    
	    if(is_leaf(original,origDepth)){//CHANGED: duproot to original
			//original->originalToDupSubtree = duproot;
			//printf("reached to leaf node\n");
			//printf("spid=%d",duproot->splittingPredicate_id);
			//printf("before updating spid=%d\n",duproot->splittingPredicate_id);
			
			updateLeaf(duproot, pseudoTargetLists,target, numberOfPORVs,numTargets, N);
			
			//printf("after updating spid=%d\n",duproot->splittingPredicate_id);
			//printf("expanding the tree\n");
			
			amsMine(duproot,pseudoTargetLists, target, numberOfPORVs,numTargets, N, dupDepth,target);
			
			return 1;
		}
		
		//update the node according to the target
		printf("updating the duplicate tree node for target=%d at depth=%d\n",target,origDepth);
		struct intervalListStruct** endMatchIntervalList = endMatchesForPrefix(duproot->truthList);
		int mininmumBucketID = getSmallestBucketID(duproot->truthList);
		int targetID = mininmumBucketID==0?target:(numberOfPORVs+mininmumBucketID);
		double trueMean, falseMean, Htrue, Hfalse,mean, error;
		int dominantTruth;
	    double intervalLength = 0.0;
	    if(endMatchIntervalList){
		intervalLength = lengthOfIntervalLists(endMatchIntervalList);
	    }
		//printf("iL=%lf\n",intervalLength);
	    trueMean = computeMean(listOfIntervalSets,targetID,intervalLength,endMatchIntervalList);
	    falseMean = computeFalseMean(listOfIntervalSets,targetID,intervalLength,endMatchIntervalList);
	    Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList);
	    Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList);
	    dominantTruth = Htrue<=Hfalse?1:0;//error(trueMean)<error(falseMean)?1:0;
	    
		duproot->mean = dominantTruth==0?falseMean:trueMean;
	    duproot->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList));
	    
		duproot->dupToOriginal = original;
    }
    
    amsMine2(original->left, duproot->left,pseudoTargetLists, target, numberOfPORVs, numTargets, N, origDepth-1, dupDepth,0,listOfIntervalSets);
	
	amsMine2(original->right, duproot->right,pseudoTargetLists, target, numberOfPORVs, numTargets, N, origDepth-1, dupDepth,0,listOfIntervalSets);
}

int amsMine(struct treeNode* root,struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int numberOfPORVs,int numTargets, int N, int depth,int targetPORV_id){
 	fflush(logFile);
	#ifdef METHOD_CALL
	fprintf(logFile,"\n[amsMine] STARTED - Depth %d\n",depth);
	printTruthListToFilePtr(root->truthList,logFile);
	fflush(logFile);
	#endif
	
	//TODO: Cleanup all the dumps of interval sets to the assertion file. Its pointless.
	if(root && depth>0){
		
		#ifdef VERBOSE_LOW
		if(root->parent == NULL)
			fprintf(logFile,"[amsMine] Current Node is ROOT [ID=%d]: Target %d (Now Splitting On P-%d)\n",root->id,target,root->splittingPredicate_id);
		else
			fprintf(logFile,"[amsMine] Current Node is INTERNAL [ID=%d]: Target %d (Context On P-%d)\n",root->id,target,root->parent->splittingPredicate_id);
		
		printTreeNodeToFilePtr(root,logFile,targetPORV_id);
		fprintf(logFile,"[amsMine] >>> Checking for Termination\n");
		fflush(logFile);
		#endif
		
		//Get list of zero error positions
		
		if(root->traceLength>0.0 && (root->mean == 0.0 || root->mean == 1.0) && fabs(root->error) == 0.0){
			#ifdef VERBOSE_LOW
			fprintf(logFile,"\n-----------------------------------------------------------------\nAssertion Found @1: Consequent - %s\n",root->truthValue==0?(root->mean==0.0?"true":"false"):(root->mean==1.0?"true":"false"));
			#endif
			FILE* fp;
			if(assertFileName==NULL){
				fp = fopen("assertions.txt","a");
			} else {
				//printf("filename = [%s]\n");
				fp = fopen(assertFileName,"a");
			}
			
			//fprintf(fp,"\n-----------------------------------------------------------------\n");//fprintf(fp,"Assertion Found @1: Consequent - %s\n",root->truthValue==0?(root->mean==0.0?"true":"false"):(root->mean==1.0?"true":"false"));
			//fprintf(fp,"Continue = %d\n",continueFlag);
			//printTruthListToFilePtr(root->truthList,fp);
			printAssertions(root,fp,targetPORV_id);
			//fprintf(fp,"-----------------------------------------------------------------\n");
			fflush(fp);
			fclose(fp);
			
			#ifdef VERBOSE_LOW
			printTruthListToFilePtr(root->truthList,logFile);
			fprintf(logFile,"\n------------ Printing Sequence For Assertion: -----------\n");
			printSequencePositionsToFile(root->parent,logFile);
			fprintf(logFile,"\n---------------------------------------------------------\n");
			fprintf(logFile,"\nAdding to Cov:\n");
			#endif
			
			assertCount++;
			
			#ifdef VERBOSE_LOW
			printIntervalListToFilePtr(coveredList,logFile);
			fprintf(logFile,"\n");
			#endif
			assertionList = addNodeToList(assertionList,root);
			
			if(continueFlag == 0){ return 1;}
		}
		fflush(logFile);
		
		if(root->traceLength==0.0){			
			return 1;
		}
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] >>> No Termination\n");
		#endif
		
		if(depth == 0 || root->splittingPredicate_id == -1){
			#ifdef VERBOSE_LOW
			fprintf(logFile,"[amsMine] %s - ENDING HERE\n\n",depth==0?"Depth Expired":"No further improvement in gain. (Splitting Predicate is -1)");
			#endif
			return 1;
		}
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] Computing TruthLists\n");
		#endif
		
		struct truthAssignmentListStruct* newTruthList_false = duplicateTruthAssignmentList(root->truthList);
		struct truthAssignmentListStruct* newTruthList_true = duplicateTruthAssignmentList(root->truthList);
		
		struct truthAssignmentStruct* asgmtFalse = createTruthAssignment(root->splittingPredicate_id,0,root->targetInfluence);
		struct truthAssignmentStruct* asgmtTrue = createTruthAssignment(root->splittingPredicate_id,1,root->targetInfluence);
		
		//Mark the type of predicate as 0 (from current knowledge) or 1 (from learned knowledge)
		asgmtFalse->type = root->predType;	
		asgmtTrue->type = root->predType;
		
		addToTruthAssignmentList(&newTruthList_false,createTruthListStruct(asgmtFalse));
		addToTruthAssignmentList(&newTruthList_true,createTruthListStruct(asgmtTrue));
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] Truth List -- FALSE \n");
		printTruthListToFilePtr(newTruthList_false,logFile); 
		fprintf(logFile,"\n[amsMine] --------------------\n");
		fprintf(logFile,"[amsMine] Truth List -- TRUE \n");
		printTruthListToFilePtr(newTruthList_true,logFile);
		fprintf(logFile,"\n[amsMine] --------------------\n");
		#endif
		
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] Computing Constrained Interval Sets\n");
		#endif
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] Computing Predicate List for P[%d] : False\n",root->splittingPredicate_id);
		#endif
		
		//Get the influence lists for each partial prefix.
		struct intervalListStruct** endMatchIntervalList_false = endMatchesForPrefix(newTruthList_false);	//TODO: Change Name of list to represent that it contains intervals
		
		struct intervalListStruct** endMatchIntervalList_true = endMatchesForPrefix(newTruthList_true);	//TODO: Change Name of list to represent that it contains intervals
		
		//Compute influence lengths
		double falseLength = 0.0;
		if(endMatchIntervalList_false){
			falseLength = lengthOfIntervalLists(endMatchIntervalList_false);
		}
		
		double trueLength = 0.0;
		if(endMatchIntervalList_true){
			trueLength = lengthOfIntervalLists(endMatchIntervalList_true);
		}
		
		#ifdef VERBOSE_LOW
		fprintf(logFile,"[amsMine] Position 3 - no use case after this\n");
		printTruthListToFilePtr(newTruthList_false,logFile);
		printTruthListToFilePtr(newTruthList_true,logFile);
		#endif
		
		char choice = 'N';
		int leftGainFlag = 1;
		int rightGainFlag = 1;
		//Decide the children and on what PORV they will be split on
		//do{	
		/*
		 * When a child node is computed, we first compute the nodes mean 
		 * to determine if an assertion is found, if so further splitting 
		 * is redundant and exploration stops.
		 * 
		 * Flags exploreLeft and exploreRight are individually disabled to
		 * stop further exploration of false and true branches respectively.
		 * 
		 * The left and right gain flags determine if gain should be computed 
		 * or not. If the parent node found that there was no gain to be had, 
		 * the splitting predicate id will be negative and gain won't be 
		 * computed for that node.
		 */
		int exploreLeft = 1; int exploreRight = 1;
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] Left (FALSE) Node, Now Preparing\n");
		#endif
		//if(first){//REMOVE	
		if(root->left==NULL){
			#ifdef MINER_DEBUG
			fprintf(logFile,"[amsMine] Computing Left (FALSE) Node\n");
			printTruthListToFilePtr(newTruthList_false,logFile);
			fprintf(stdout,"[amsMine] Computing Left (FALSE) Node\n");
			printTruthListToFilePtr(newTruthList_false,stdout);				
			fprintf(stdout,"[amsMine] End Match:\n");
			int k=0;
			for(k=0;k<traceCount;k++){
				printf("Trace[%d]:\t",k+1);
				printIntervalListToFilePtr(endMatchIntervalList_false[k],stdout);
				printf("\n");					
			}
			
			fflush(stdout);
			#endif
			root->left = createTreeNode( 
			newTruthList_false,
			listOfIntervalSets,
			-1,
			-1,
			falseLength,
			NULL,
			NULL	
			);
			
			int mininmumBucketID = getSmallestBucketID(newTruthList_false);
			int targetID = mininmumBucketID==0?target:(numberOfPORVs+mininmumBucketID);
			double trueMean, falseMean, Htrue, Hfalse;
			int dominantTruth;
			root->left->trueFalseFlag=0;
			
			if(targetBias == -1){
				falseMean = computeFalseMean(listOfIntervalSets,targetID,falseLength,endMatchIntervalList_false);
				//Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_false);//cumulative?computeCumulativeFalseEntropy(intervalSet,endMatchIntervalList_false,mininmumBucketID):computeFalseEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_false);
				dominantTruth = 0;
				root->left->mean = falseMean;
				root->left->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList_false));;//Hfalse;
			} else if(targetBias == +1){
				trueMean = computeMean(listOfIntervalSets,targetID,falseLength,endMatchIntervalList_false);
				//Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_false);//cumulative?computeCumulativeTrueEntropy(intervalSet,endMatchIntervalList_false,mininmumBucketID):computeTrueEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_false);
				dominantTruth = 1;
				root->left->mean = trueMean;
				root->left->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList_false));//Htrue;
			} else {
				trueMean = computeMean(listOfIntervalSets,targetID,falseLength,endMatchIntervalList_false);
				falseMean = computeFalseMean(listOfIntervalSets,targetID,falseLength,endMatchIntervalList_false);
				Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_false);//cumulative?computeCumulativeTrueEntropy(intervalSet,endMatchIntervalList_false,mininmumBucketID):computeTrueEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_false);
				Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_false);//cumulative?computeCumulativeFalseEntropy(intervalSet,endMatchIntervalList_false,mininmumBucketID):computeFalseEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_false);
				if(fabs(Htrue) == 0.0 && fabs(Hfalse)==0.0)
					root->left->trueFalseFlag=1;
				
				dominantTruth = Htrue<=Hfalse?1:0;//error(trueMean)<error(falseMean)?1:0;
				root->left->mean = dominantTruth==0?falseMean:trueMean;
				root->left->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList_false));//cumulative?computeCumulativeEntropy(intervalSet,endMatchIntervalList_false,mininmumBucketID):computeEntropy(getListAtPosition(intervalSet,targetID),(endMatchIntervalList_false));//error(root->left->mean);
			} 
				
			root->left->parent = root;
			root->left->explored = duplicateIndexCouple(root->explored);
			root->left->truthValue = dominantTruth;
			
			#ifdef MINER_DEBUG
				fprintf(logFile,"[amsMine] After creating Left (FALSE) Node\nThe Constraint Set");
				printTruthListToFilePtr(newTruthList_false,logFile);
				fprintf(stdout,"[amsMine] After creating Left (FALSE) Node\n");
				printTreeNodeToFilePtr(root->left,stdout,targetPORV_id);
				printTreeNodeToFilePtr(root->left,logFile,targetPORV_id);
			#endif
			
		} else {
			if(root->left->splittingPredicate_id == -1)
				leftGainFlag = 0;
		}
		
		//if(N<=1 && (root->left->traceLength==0 || (root->left->mean == 0 || root->left->mean == 1) || root->error == 0)){
		if(N<=1 && (root->left->traceLength>0.0 && (fabs(root->left->mean) == 0.0 || root->left->mean == 1.0) && ((float)(((int)(fabs(root->left->error)*10e5))/10e5) == 0.0) )){
			
			root->left->targetInfluence = 0;
			#ifdef MINER_DEBUG
			fprintf(logFile,"Assertion Found : Consequent - %s\n",root->left->truthValue==0?(root->left->mean==0.0?"true":"false"):(root->left->mean==1.0?"true":"false"));
			#endif
			
			FILE* fp;
			if(assertFileName==NULL){
				fp = fopen("assertions.txt","a");
			} else {
				fp = fopen(assertFileName,"a");
			}
			
			//FILE* fp = fopen("assertions.txt","a");
			//fprintf(fp,"\n-----------------------------------------------------------------\n");//fprintf(fp,"Assertion Found @2A: Consequent - %s\n",root->left->truthValue==0?(root->left->mean==0.0?"true":"false"):(root->left->mean==1.0?"true":"false"));
			//fprintf(fp,"Continue = %d\n",continueFlag);
			//printTruthListToFilePtr(root->left->truthList,fp);
			printAssertions(root->left,fp, targetPORV_id);
			//fprintf(fp,"-----------------------------------------------------------------\n");
			assertionList = addNodeToList(assertionList,root->left);
			fclose(fp);
			//printTruthListToFile(root->left->truthList);
			
			#ifdef MINER_DEBUG
			printTruthListToFilePtr(root->left->truthList,logFile);
			fprintf(logFile,"\n------------ Printing Sequence : -----------\n");
			printSequencePositionsToFile(root->left,logFile);
			fprintf(logFile,"\n--------------------------------------------\n");
			#endif
			//coveredSet = unionIntervalSets(coveredSet,root->left->intervalList);
			//coveredList = unionIntervalLists(coveredList,computeForwardInfluenceForAnAssertion(root->left,target,K));
			
			exploreLeft = 0;
			assertCount++;
			
			//if(continueFlag==0) return 1;
		} else {
			if(leftGainFlag){
				root->left = findBestGain(root->left->listOfIntervalSets,pseudoTargetLists,target,numberOfPORVs,numTargets,N,root->left,targetPORV_id);
				
				if(root->left->splittingPredicate_id == -1)
					leftGainFlag = 0;
			}
		}
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] Left (FALSE) Node - Prepared\n");
		fprintf(stdout,"[amsMine] Left (FALSE) Node - Prepared\n");
		fprintf(logFile,"[amsMine] Right (TRUE) Node, Now Preparing\n");
		#endif
		//}//REMOVE
		if(root->right==NULL){
			if(root->left->id == 3){
				fprintf(logFile,"\nCHECK WHATS HAPPENING HERE --- ABC ---\n");
			}
			#ifdef MINER_DEBUG
			fprintf(logFile,"[amsMine] Computing Right (TRUE) Node\n");
			fprintf(stdout,"[amsMine] Computing Right (TRUE) Node\n");
			printTruthListToFilePtr(newTruthList_true,stdout);
			fprintf(stdout,"[amsMine] End Match:\n");
			int k=0;
			for(k=0;k<traceCount;k++){
				printf("Trace[%d]:\t",k+1);
				printIntervalListToFilePtr(endMatchIntervalList_true[k],stdout);
				printf("\n");
			}
			fflush(stdout);
			#endif
			
			int mininmumBucketID = getSmallestBucketID(newTruthList_true);
			int targetID = mininmumBucketID==0?target:(numberOfPORVs+mininmumBucketID);
				
			root->right = createTreeNode( newTruthList_true,
							listOfIntervalSets,
							-1,
							-1,
							trueLength,
							NULL,
							NULL	
			);
				
				double trueMean, falseMean, Htrue, Hfalse;
				int dominantTruth;
				
			root->right->trueFalseFlag=0;
				
			if(targetBias == -1){
				falseMean = computeFalseMean(listOfIntervalSets,targetID,trueLength,endMatchIntervalList_true);
				//Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_true);//cumulative?computeCumulativeFalseEntropy(intervalSet,endMatchIntervalList_true,mininmumBucketID):computeFalseEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_true);
				dominantTruth = 0;
				root->right->mean = falseMean;
				root->right->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList_true));//Hfalse;
			} else if(targetBias == +1){
				trueMean = computeMean(listOfIntervalSets,targetID,trueLength,endMatchIntervalList_true);
				//Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_true);//cumulative?computeCumulativeTrueEntropy(intervalSet,endMatchIntervalList_true,mininmumBucketID):computeTrueEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_true);
				dominantTruth = 1;
				root->right->mean = trueMean;
				root->right->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList_true));//Htrue;
			} else {
				trueMean = computeMean(listOfIntervalSets,targetID,trueLength,endMatchIntervalList_true);
				falseMean = computeFalseMean(listOfIntervalSets,targetID,trueLength,endMatchIntervalList_true);
				Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_true);//cumulative?computeCumulativeTrueEntropy(intervalSet,endMatchIntervalList_true,mininmumBucketID):computeTrueEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_true);
				Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetID),endMatchIntervalList_true);//cumulative?computeCumulativeFalseEntropy(intervalSet,endMatchIntervalList_true,mininmumBucketID):computeFalseEntropy(getListAtPosition(intervalSet,targetID),endMatchIntervalList_true);
				
				if(fabs(Htrue) == 0.0 && fabs(Hfalse)==0.0)
					root->right->trueFalseFlag=1;
				
				dominantTruth = Htrue<=Hfalse?1:0;//error(trueMean)<error(falseMean)?1:0;
				root->right->mean = dominantTruth==0?falseMean:trueMean;
				root->right->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetID),(endMatchIntervalList_true));//cumulative?computeCumulativeEntropy(intervalSet,endMatchIntervalList_true,mininmumBucketID):computeEntropy(getListAtPosition(intervalSet,targetID),(endMatchIntervalList_true));//error(root->left->mean);
			} 
				
			root->right->parent = root;
			root->right->truthValue = dominantTruth;
			root->right->explored = duplicateIndexCouple(root->explored);
			
			
			#ifdef MINER_DEBUG
			fprintf(logFile,"[amsMine] Right (TRUE) Node Created.\n");
			#endif
			#ifdef MINER_DEBUG
			fprintf(logFile,"[amsMine] After creating Right (TRUE) Node\n");
			printTruthListToFilePtr(newTruthList_true,logFile);
			fprintf(stdout,"[amsMine] After creating Right (TRUE) Node\n");
			printTreeNodeToFilePtr(root->right,stdout,targetPORV_id);
			printTreeNodeToFilePtr(root->right,logFile,targetPORV_id);
			#endif	
					
		} else {
			if(root->right->splittingPredicate_id == -1)
				rightGainFlag = 0;
		}
		
		#ifdef MINER_DEBUG
		printTreeNodeToFilePtr(root->right,logFile,targetPORV_id);
		fprintf(logFile,"[amsMine] N = %d\n",N);
		#endif
		//fprintf(logFile,"[amsMine] traceLength = %lf\n",root->right->traceLength);
		//fprintf(logFile,"[amsMine] (fabs(root->right->mean) == 0.0 || root->right->mean == 1.0) = %d\n",(fabs(root->right->mean) == 0.0 || root->right->mean == 1.0));
		//fprintf(logFile,"[amsMine] fabs(root->right->error) == 0.0 = %d\n",fabs(root->right->error) == 0.0);
		//fprintf(logFile,"[amsMine] fabs(root->right->error) = %lf\n",fabs(root->right->error) );
		//fprintf(logFile,"[amsMine] fabs(root->right->error) = %10.10lf\n",(double)(((int)(fabs(root->right->error)*10e5))/10e5));
			
		if(N<=1 && (root->right->traceLength>0.0 && (fabs(root->right->mean) == 0.0 || root->right->mean == 1.0) && ((float)(((int)(fabs(root->right->error)*10e5))/10e5) == 0.0))){
			root->right->error = 0.0;
			root->right->targetInfluence = 0;
			
			//#ifdef VERBOSE_LOW
			printf("Assertion Found : Consequent - %s\n",root->right->truthValue==0?(root->right->mean==0.0?"true":"false"):(root->right->mean==1.0?"true":"false"));
			//#endif
			
			FILE* fp;
			if(assertFileName==NULL){
				fp = fopen("assertions.txt","a");
			} else {
				fp = fopen(assertFileName,"a");
			}
			
			//FILE* fp = fopen("assertions.txt","a");
			//fprintf(fp,"\n-----------------------------------------------------------------\n");//fprintf(fp,"Assertion Found @2B: Consequent - %s\n",root->right->truthValue==0?(root->right->mean==0.0?"true":"false"):(root->right->mean==1.0?"true":"false"));
			//fprintf(fp,"Continue = %d\n",continueFlag);
			//printTruthListToFilePtr(root->right->truthList,fp);
			printAssertions(root->right,fp, targetPORV_id);
			//fprintf(fp,"-----------------------------------------------------------------\n");
			assertionList = addNodeToList(assertionList,root->right);
			fclose(fp);
			//printTruthListToFile(root->right->truthList);
			
			#ifdef MINER_DEBUG
			printTruthListToFilePtr(root->right->truthList,logFile);
			fprintf(logFile,"\n------------ Printing Sequence : -----------\n");
			printSequencePositionsToFile(root->right,logFile);
			fprintf(logFile,"\n--------------------------------------------\n");
			#endif
			
			//printAssertion(root->truthList);
			//printListOfIntervalLists(root->right->intervalList);
			//coveredSet = unionIntervalSets(coveredSet,root->right->intervalList);
			//                                 coveredList = unionIntervalLists(coveredList,computeForwardInfluenceForAnAssertion(root->right,target,K));
			//                                 
			//                                 #ifdef VERBOSE_LOW
			// 				printIntervalListToFilePtr(coveredList,logFile);fprintf(logFile,"\n");
			//                                 #endif
			
			exploreRight = 0;
			assertCount++;
			
			//if(continueFlag==0) return 1;
		} else { 
			if(rightGainFlag)
			{	fprintf(logFile,"[amsMine] Compute Gain for Right Node\n");
				root->right = findBestGain(root->right->listOfIntervalSets,pseudoTargetLists,target,numberOfPORVs,numTargets,N,root->right,targetPORV_id);
				if(root->right->splittingPredicate_id == -1)
					rightGainFlag = 0;
			}
		}
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"[amsMine] Right (TRUE) Node Prepared\n");
		fprintf(stdout,"[amsMine] Right (TRUE) Node - Prepared\n");
		fprintf(logFile,"[amsMine] Going Deeper Node ID [%d]\n",root->id);fflush(logFile);
		#endif
		
		int test1 = 1; int test2 = 1;
		
		#ifdef MINER_DEBUG
		fprintf(logFile,"State of exploration at Node-%d for children [Node-%d ; Node-%d] = [%d,%d]\n", root->id, root->left?root->left->id:-1, root->right?root->right->id:-1, exploreLeft, exploreRight);fflush(logFile);
		#endif 
		fflush(logFile);
		
		//if(first && exploreLeft && root->left && root->left->traceLength>0.0){
		if(exploreLeft && root->left && root->left->traceLength>0.0){
			//pause();
			#ifdef MINER_DEBUG
			fprintf(logFile,"[amsMine] @Current Depth = %d : Exploring CHILD NODE - LEFT \n",depth);
			#endif
			//fprintf(stdout,"[amsMine] Mining Left Node , DEPTH = %d\n",depth-1);
			test1 = amsMine(root->left,pseudoTargetLists,target,numberOfPORVs,numTargets,N,depth-1,targetPORV_id);
			//fprintf(stdout,"[amsMine] Left Node Mined , DEPTH = %d\n",depth-1);
		}
		//first = 1;
		if(exploreRight && root->right && root->right->traceLength>0.0){
			//pause();
			#ifdef MINER_DEBUG
			fprintf(logFile,"[amsMine] @Current Depth = %d : Exploring CHILD NODE - RIGHT\n",depth);
			#endif
			//fprintf(stdout,"[amsMine] Mining Right Node , DEPTH = %d\n",depth-1);
			test2 = amsMine(root->right,pseudoTargetLists,target,numberOfPORVs,numTargets,N,depth-1,targetPORV_id);
			//fprintf(stdout,"[amsMine] Right Node Mined , DEPTH = %d\n",depth-1);
		}
		//fprintf(stdout,"[amsMine] Mined from Children, DEPTH = %d\n",depth-1);
		//return;
		//}
		fflush(stdout);
		fflush(stdin);
		if(depth==0 || (leftGainFlag == 0 && rightGainFlag == 0)) {
			#ifdef MINER_DEBUG
			fprintf(logFile,"[amsMine] ENDED - depth 0 or left/right gain flag is 0\n");
			#endif
			return 1;
		}
		
		if(test1==0 && test2==0){
			//#ifdef MINER_DEBUG
			#ifdef VERBOSE_LOW
			fprintf(logFile,"[amsMine] Both Child Nodes Test Negative --- ENDED\n");
			#endif
			return 0;
		}
		//printf("End? (Y/N) : ");
		//scanf("%c",&choice);
		
		//}while(choice!='Y' && choice!='y');
		fflush(logFile);
	} else if(root && depth == 0){
		fprintf(logFile,"[amsMine] AT MAX DEPTH with NON NULL Node\n");
		//if(root->traceLength==0 || (root->mean == 0 || root->mean == 1) || root->error == 0){
		if(root->traceLength>0.0 && (root->mean == 0.0 || root->mean == 1.0) || root->error == 0.0){
			fprintf(logFile,"[amsMine] INSIDE IF\n");
			fflush(logFile);
			#ifdef VERBOSE_LOW
			fprintf(logFile,"\n-----------------------------------------------------------------\nAssertion Found : Consequent - %s\n",root->truthValue==0?(root->mean==0.0?"true":"false"):(root->mean==1.0?"true":"false"));
			#endif
			
			FILE* fp;
			if(assertFileName==NULL){
				fp = fopen("assertions.txt","a");
			} else {
				fp = fopen(assertFileName,"a");
			}
			
			//FILE* fp = fopen("assertions.txt","a");
			//fprintf(fp,"\n-----------------------------------------------------------------\n");fprintf(fp,"Assertion Found @0 : Consequent - %s\n",root->truthValue==0?(root->mean==0.0?"true":"false"):(root->mean==1.0?"true":"false"));
			//fprintf(fp,"Continue = %d\n",continueFlag);
			fprintf(logFile,"[amsMine] PRINTING ASSERTIONS\n");
			fflush(logFile);
			//printTruthListToFilePtr(root->truthList,fp);
			printAssertions(root,fp, targetPORV_id);
			//fprintf(fp,"-----------------------------------------------------------------\n");
			fprintf(logFile,"[amsMine] ADDING NODE TO LIST OF ASSERTION NODES\n");
			fflush(logFile);
			assertionList = addNodeToList(assertionList,root);
			fclose(fp);
			fprintf(logFile,"[amsMine] ADDED\n");
			//printTruthListToFile(root->truthList);
			
			#ifdef VERBOSE_LOW
			printTruthListToFilePtr(root->truthList,logFile);
			fprintf(logFile,"\n------------ Printing Sequence : -----------\n");
			printSequencePositionsToFile(root->parent,logFile);
			fprintf(logFile,"\n--------------------------------------------\n");
			#endif
			
			//coveredSet = unionIntervalSets(coveredSet,root->intervalList);
			//                         coveredList = unionIntervalLists(coveredList,computeForwardInfluenceForAnAssertion(root,target,K));
			//                         
			//                         #ifdef VERBOSE_LOW
			// 				printIntervalListToFilePtr(coveredList,logFile);fprintf(logFile,"\n");
			//                         #endif
			
			assertCount++;
			//return 1;
		} else {
			//fprintf(logFile,"[amsMine] INSIDE ELSE\n");
			fflush(logFile);
			
			printf("\n Further exploration needed\n");
		}
	} 
	fflush(logFile);
	#ifdef METHOD_CALL
	fprintf(logFile,"[amsMine] ENDED\n");
	fflush(logFile);
	#endif
	return 1;
}

void printTreeNode(struct treeNode* node,int targetPORV_id){
	if(node){
		printf("---------------------TREE NODE ID [%d]----------------------\n",node->id);
		if(node->parent){
			printf("PARENT NODE [%d]\n",node->parent->id);
		}
		
		printf("CONSTRAINT SET  : \n");
		printTruthList(node->truthList);
		
		//printf("INTERVAL SET: \n");
		//printListOfIntervalLists(node->intervalList);
		//printf("\n");
		
		//printAssertions(node,stdout);
		printf("\n");
		printf("TARGET INFLUENCE   = %d\n",node->targetInfluence);
		printf("NEXT SPLIT ON PORV = %d\n",node->splittingPredicate_id);
		printf("TRACE LENGTH       = %lf\n",node->traceLength);
		printf("MEAN         = %lf\n",node->mean);
		printf("ERROR        = %lf\n",node->error);
		printf("For Truth Type    = %d\n",node->truthValue);        
		printf("EXPLORED LIST  : \n");
		printIndexCoupleList(node->explored);
		printf("\n");
		
		printf("---------------------------------------------------\n");
	}
}

void printTreeNodeToFile(struct treeNode* node,int depth,int targetPORV_id){
	if(node){
		FILE* fp = getDTreeFilePtr();
		if(fp){
			fprintf(fp,"---------------------TREE NODE ID [%d] at depth [%d] ----------------------\n",node->id,depth);
			if(node->parent){
				fprintf(fp,"PARENT NODE [%d]\n",node->parent->id);
			}
			
			fprintf(fp,"TRUTH LIST  : \n");
			printTruthListToFilePtr(node->truthList,fp);
			
			//fprintf(fp,"INTERVAL SET: \n");
			//printListOfIntervalListsToFilePtr(node->intervalList,fp);
			//fprintf(fp,"\n");
			
			printAssertions(node,fp,targetPORV_id);
			fprintf(fp,"\n");
			fprintf(fp,"TARGET INFLUENCE   = %d\n",node->targetInfluence);
			fprintf(fp,"NEXT SPLIT ON PORV = %d\n",node->splittingPredicate_id);
			fprintf(fp,"TRACE LENGTH       = %lf\n",node->traceLength);                        
			fprintf(fp,"MEAN         = %lf\n",node->mean);			
			fprintf(fp,"ERROR        = %lf\n",node->error);
			fprintf(fp,"For Truth Type    = %d\n",node->truthValue);
			fprintf(fp,"TRUE-FALSE-FLAG    = %d\n",node->trueFalseFlag);
			fprintf(fp,"EXPLORED LIST  : \n");
			printIndexCoupleListToFilePtr(node->explored,fp);
			fprintf(fp,"\n");
			
			
			fprintf(fp,"------------------------------------------------------------\n");
			fflush(fp);
			//fclose(fp);
		}
	}
}

void printTree(struct treeNode* node,int targetPORV_id){
	if(node){
		printTreeNodeToFilePtr(node,getDTreeFilePtr(),targetPORV_id);
		printTree(node->left,targetPORV_id);
		printTree(node->right,targetPORV_id);
	}
}

void printTreeToFilePtr(struct treeNode* node, FILE *dTree, int targetPORV_id){
	printTreeNodeToFilePtr(node,dTree,targetPORV_id);
	if(node){
		fprintf(dTree,"\nLEFT CHILD:\n");
		printTreeToFilePtr(node->left,dTree,targetPORV_id);
		fprintf(dTree,"\nRIGHT CHILD:\n");
		printTreeToFilePtr(node->right,dTree,targetPORV_id);
	}
}

void printTreeNodeToFilePtr(struct treeNode* node, FILE* fp,int targetPORV_id){
	if(node){               
		if(fp){
			fprintf(fp,"---------------------TREE NODE ID [%d]----------------------\n",node->id);
			//fprintf(fp,"INTERVAL SET: \n");
			//printListOfIntervalListsToFilePtr(node->intervalList,fp);
			//fprintf(fp,"\n");
			
			fprintf(fp,"TRUTH LIST  : \n");
			printTruthListToFilePtr(node->truthList,fp);
			//printAssertions(node,fp);
			fprintf(fp,"\n");
			fprintf(fp,"TARGET INFLUENCE\t= %d\n",node->targetInfluence);
			fprintf(fp,"ON PORV\t= %d\n",node->splittingPredicate_id);
			fprintf(fp,"TRACE LENGTH\t= %lf\n",node->traceLength);
			fprintf(fp,"MEAN\t= %lf\n",node->mean);
			fprintf(fp,"ERROR\t= %lf\n",node->error);
			
			/*
			int i =0;
			struct truthAssignmentListStruct* temp = node->truthList;
			
			while(temp){
				i++;
				temp = temp->next;
			}
			
			if(i==4){
				struct intervalListStruct** endMatches = endMatchesForPrefix(node->truthList);
				int smallestBucketID = getSmallestBucketID(node->truthList);
				struct listOfIntervalListsStruct** targetLists = getListsAtPosition(listOfIntervalSets,smallestBucketID==0?targetPORV_id:numberOfPORVs+smallestBucketID);
				int j=0;
				for(j=0;j<traceCount;j++){
					printListOfIntervalListsToFilePtr(listOfIntervalSets[j],fp);
					double traceLength = lengthOfIntervalList(endMatches[j]);
					fprintf(fp,"\nEnd Match = ");printIntervalListToFilePtr(endMatches[j],fp);
					double targetTrue = lengthOfIntervalList(intersectIntervalList(endMatches[j],targetLists[j]->trueList));
					fprintf(fp,"\nTarget True = ");printIntervalListToFilePtr(intersectIntervalList(endMatches[j],targetLists[j]->trueList),fp);fprintf(fp,"\n");
					double targetFalse = lengthOfIntervalList(intersectIntervalList(endMatches[j],targetLists[j]->trueList));
					
					fprintf(fp,"[%d] traceLength = [%lf], targetTrue = [%lf], targetFalse = [%lf]\n",j,traceLength,targetTrue,targetFalse);
				}
			}
			*/
			
			//fprintf(fp,"Gains:\n");
			//printFloat2DArraryToFile(N*2,numberOfPORVs,node->gains,fp);
			fprintf(fp,"TARGET TRUTH\t= %d\n",node->truthValue);
			fprintf(fp,"LEARN TYPE\t= %d\n",node->predType);
			fprintf(fp,"LIST OF EXPLORED DECISIONS\t: \n");
			printIndexCoupleListToFilePtr(node->explored,fp);
			fprintf(fp,"\n");
			
			
			fprintf(fp,"--------------------------------------------------------\n");                        
			fflush(fp);
		} else printf("\nfp is null\n");
	} else {
		if(fp){
			fprintf(fp,"Node is NULL\n"); fflush(fp);
		} else printf("\nfp is null\n");
	}
}

/*
 * predicate is the 0 relative id + 1
 * target is the position in the sequence
 */
int indexInList(struct indexCouple* root, int predicate, int target){
	#ifdef METHOD_CALL
	fprintf(logFile,"[indexInList] STARTED\n");
	#endif
	if(root){
		while(root){
			if(root->predicate == predicate && root->target == target && root->type==0){
				#ifdef SUP_DEBUG
				fprintf(logFile,"[indexInList] ENDED - IS in list\n");
				#endif
				return 1;
			}
			root = root->next;
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"[indexInList] ENDED - NOT in list\n");
		#endif
		return 0;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[indexInList] ENDED\n");
	#endif
	return 0;
}


//-------------------- SUPPORT METHODS -------------------------

/*
 * Process and VALIDATE the inputs to AMS-Miner
 */
FILE* processInput(int argc, char* argv[], int* N, double* K, int* depth, int repeatFlag){        
	if(argc!=5){
		printf("\nINCORRECT SYNTAX\n");
		
		printf("\nExpected Syntax: CMD <File Name> <Assertion Length> <Time Seperation> <Decision-Tree Depth>\n\n");
		exit(0);
	}
	if(!repeatFlag){
		*N = (double)atoi(argv[2]);
		*K = (double)atof(argv[3]);
		*depth = (int)atof(argv[4]);
	}
	
	
	FILE* intervalListin =  fopen(argv[1], "r");
	if(intervalListin==0){
		printf("[ERROR] File \"%s\" does not exist.\n",argv[1]);
		exit(0);
	}
	return intervalListin;
}


FILE* processConfig(int argc, char* argv[]){
	FILE* configin = NULL;
	switch(argc){
		case 2:
			configin =  fopen(argv[1], "r");
			if(configin==0){
				printf("[ERROR] File \"%s\" does not exist.\n",argv[1]);
				exit(0);
			}
			return configin;
			break;
			
		case 3:
			configin =  fopen(argv[1], "r");
			if(configin==0){
				printf("[ERROR] File \"%s\" does not exist.\n",argv[1]);
				exit(0);
			}
			targetBias = atoi(argv[2]);
			
			return configin;
			break;
			
		default:
			printf("\nINCORRECT SYNTAX\n");
			
			printf("\nExpected Syntax: CMD <Config File Name> [<explanation-bias>]\n\n");
			exit(0);
	}
	
	
}

/*
 * Get the target PORV
 */
int getTarget(int numberOfPORVs){
	int target = 0;
	do{
		printf("Target PORV ID [1:%d]: ",numberOfPORVs);
		scanf("%d",&target);
		
	}while(!(target<=numberOfPORVs && target>0));        
	return target;
}


/*
 * Get the trace Length
 */
double getTraceLength(){
	int i;
	double length = 0.0;
	for(i=0;i<traceCount;i++){
		length += getMaxIntervalSet(listOfIntervalSets[i]);
	}
	/*
	 *		double traceLength = 0.0;
	 *		printf("Trace Length: ");
	 *		scanf("%lf",&traceLength);
	 */
	return length;
}
//edited
void prepareBackwardInfluenceTraces2(struct listOfIntervalListsStruct** localIntervalSets,struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int N, double K, int strict,int numberOfPORVs,int numTargets){
	#ifdef METHOD_CALL
		fprintf(logFile,"[prepareBackwardInfluenceTraces] STARTED\n");
	#endif
	
	int setCount = 0;
	struct listOfIntervalListsStruct* backwardInfluence = NULL;
	for(setCount=0;setCount<traceCount;setCount++){
		
		struct listOfIntervalListsStruct* targetList = getListAtPosition(localIntervalSets[setCount],target);
		int i;
		for(i=1;i<N;i++){
			backwardInfluence = addListToListOfIntervalLists(
							backwardInfluence,
							createListOfIntervalLists(
								minkowskiDiffList(targetList->trueList,strict==0?0:((double)i)*K,((double)i)*K),
								minkowskiDiffList(targetList->falseList,strict==0?0:((double)i)*K,((double)i)*K)
							)
						);
		}
		printf("%d\n",setCount);
		//localIntervalSets[setCount] = addListToListOfIntervalLists(localIntervalSets[setCount],backwardInfluence);
		pseudoTargetLists[setCount][target- (numberOfPORVs - numTargets + 1)] = backwardInfluence;
		backwardInfluence = NULL;
	}
	
        //printListOfIntervalLists(backwardInfluence);        
        #ifdef METHOD_CALL
		fprintf(logFile,"[prepareBackwardInfluenceTraces] ENDED\n");
	#endif        
}
/*
 * Prepare Backward Influence Trace Truth for Target PORV
 */
void prepareBackwardInfluenceTraces(struct listOfIntervalListsStruct** localIntervalSets, int target, int N, double K, int strict){
	#ifdef METHOD_CALL
	fprintf(logFile,"[prepareBackwardInfluenceTraces] STARTED\n");
	#endif
	
	int setCount = 0;
	struct listOfIntervalListsStruct* backwardInfluence = NULL;
	for(setCount=0;setCount<traceCount;setCount++){
		
		struct listOfIntervalListsStruct* targetList = getListAtPosition(localIntervalSets[setCount],target);
		int i;
		for(i=1;i<N;i++){
			backwardInfluence = addListToListOfIntervalLists(
				backwardInfluence,
				createListOfIntervalLists(
					minkowskiDiffList(targetList->trueList,strict==0?0:((double)i)*K,((double)i)*K),
										  minkowskiDiffList(targetList->falseList,strict==0?0:((double)i)*K,((double)i)*K)
				)
			);
		}
		
		localIntervalSets[setCount] = addListToListOfIntervalLists(localIntervalSets[setCount],backwardInfluence);
		backwardInfluence = NULL;
	}
	
	//printListOfIntervalLists(backwardInfluence);        
	#ifdef METHOD_CALL
	fprintf(logFile,"[prepareBackwardInfluenceTraces] ENDED\n");
	#endif        
}
/*
 * Prepare the root node of the Decision Tree
 */
void prepareRoot(struct treeNode* root,struct listOfIntervalListsStruct** localIntervalSet,struct listOfIntervalListsStruct*** pseudoTargetLists, int targetPORV_id, int numberOfPORVs,int numTargets,int N){
	#ifdef METHOD_CALL
	fprintf(logFile,"[prepareRoot] STARTED\n");fflush(logFile);
	#endif
	
	root->listOfIntervalSets = localIntervalSet;
	
	double trueMean, falseMean, Htrue, Hfalse;
	int dominantTruth;
	validLists = createValidLists();
	
	if(targetBias == -1){
		falseMean = computeFalseMean(localIntervalSet,targetPORV_id,root->traceLength,validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		//if(cumulative){
		//	Hfalse = computeCumulativeFalseEntropy(localIntervalSet,influenceLists);//createIntervalList(createIntervalStruct(0,root->traceLength)),0);
		//} else {
		//Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetPORV_id),validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		//}
		dominantTruth = 0;
		root->mean = falseMean;
		//root->error = Hfalse;
		root->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetPORV_id),validLists);
		root->truthValue = 0;
	} else if(targetBias == +1){
		trueMean = computeMean(localIntervalSet,targetPORV_id,root->traceLength,validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		// 		if(cumulative){
		// 			Htrue = computeCumulativeTrueEntropy(localIntervalSet,createIntervalList(createIntervalStruct(0,root->traceLength)),0);
		// 		} else {
		Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetPORV_id),validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		//}
		dominantTruth = 1;
		root->mean = trueMean;
		//root->error = Htrue;
		root->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetPORV_id),validLists);
		root->truthValue = 1;
	} else {
		trueMean = computeMean(listOfIntervalSets,targetPORV_id,root->traceLength,validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		falseMean = computeFalseMean(listOfIntervalSets,targetPORV_id,root->traceLength,validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		
		// 		if(cumulative){
		// 			Htrue = computeCumulativeTrueEntropy(localIntervalSet,createIntervalList(createIntervalStruct(0,root->traceLength)),0);
		// 			Hfalse = computeCumulativeFalseEntropy(localIntervalSet,createIntervalList(createIntervalStruct(0,root->traceLength)),0);
		// 			root->error = computeCumulativeEntropy(localIntervalSet,createIntervalList(createIntervalStruct(0,root->traceLength)),0);
		// 		} else {
		Htrue = computeTrueEntropy(getListsAtPosition(listOfIntervalSets,targetPORV_id),validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		Hfalse = computeFalseEntropy(getListsAtPosition(listOfIntervalSets,targetPORV_id),validLists);//createIntervalList(createIntervalStruct(0,root->traceLength)));
		root->error = computeEntropy(getListsAtPosition(listOfIntervalSets,targetPORV_id),validLists);//endMatchForPrefix(NULL));
		// 		}
		//printf("here\n");fflush(stdout);fflush(logFile);
		
		dominantTruth = Htrue<=Hfalse?1:0;//error(trueMean)<error(falseMean)?1:0;
		root->mean = dominantTruth==0?falseMean:trueMean;
		
		root->truthValue = dominantTruth;
	} 
	fflush(logFile);
	root = findBestGain(localIntervalSet,pseudoTargetLists,targetPORV_id,numberOfPORVs,numTargets,N,root,targetPORV_id);
	
	
	#ifdef VERBOSE_LOW
	fprintf(logFile,"[prepareRoot] Index Couple List \n{ -- \n");
	printIndexCoupleListToFilePtr(root->explored,logFile);
	fprintf(logFile,"\n[prepareRoot] --  } \n\n");
	//printf(">>> [MAIN] BEST GAIN [%d:%d] Error [Mean = %lf Error = %lf]\n",\
	root->targetInfluence,root->splittingPredicate_id,root->mean,root->error);
	#endif
	
	#ifdef MAIN_DEBUG
	fprintf(logFile,"[prepareRoot] Printing Truth Set\n");
	printListOfIntervalListsToFilePtr(localIntervalSet,logFile);
	fprintf(logFile,"[prepareRoot] Done\n");
	#endif
	//exit(0);
	//printf("root->parent = %p, root = %p\n",root->parent,root);
	//return depth; 
	#ifdef SUP_DEBUG
	fprintf(logFile,"[prepareRoot] ENDED\n");
	#endif
	
}

void choicePause(){
	fflush(stdin);
	char choice = 'n';
	printf("Continue?\n");
	fflush(stdin);
	scanf(" %c",&choice);
	if(choice=='y' || choice == 'Y'){
		return;
	} 
	exit(0);
}

struct intervalListStruct* forwardInfluence(struct intervalListStruct* source, int i, double k, struct intervalListStruct* target){
	if(strict==0){
		if(source)
			return intersectIntervalList(minkowskiSumList(source,(double)0,((double)i)*k),target);
		else return NULL;
	} else {
		if(source)
			return intersectIntervalList(minkowskiSumList(source,((double)i)*k,((double)i)*k),target);
		else return NULL;
	}
}
/*
 * //Compute forward influence for an assertion given target truth
 * struct intervalListStruct* computeForwardInfluenceForAnAssertionForTruth(struct treeNode* assertion,int targetID,double k,int truth){
 *	#ifdef SUP_DEBUG
 *	fprintf(logFile,"[computeForwardInfluenceForAnAssertion] BEGIN\n");
 *	#endif
 *	if(assertion){
 *		if(assertion->truthList && intervalSet){
 *			sortTruthAssignmentList(&assertion->truthList);
 *			struct truthAssignmentListStruct* bucket = assertion->truthList;
 *			
 *			struct intervalListStruct* conjunct0 = NULL;
 *			int lastBucketID = 0;
 *			struct intervalListStruct* conjunct1 = NULL;
 *			struct intervalListStruct* influence = NULL;
 *			
 *			int bucketID = bucket->asgmt->position;
 *			lastBucketID = bucketID;
 *			
 *			//struct listOfIntervalListsStruct* target = getListAtPosition(assertion->intervalList,targetID);
 *			struct listOfIntervalListsStruct* target = getListAtPosition(intervalSet,targetID);
 *			struct intervalListStruct* targetList = truth==0?target->falseList:target->trueList;
 *			
 *			while(bucket){
 *				//bucket->asgmt->position // i
 *				//bucket->asgmt->predicate_id //porv
 *				//bucket->asgmt->truth    //true or false list
 *				//assertion->mean         //target truth
 *				
 *				struct listOfIntervalListsStruct* source = getListAtPosition(assertion->intervalList,bucket->asgmt->predicate_id);
 *				struct intervalListStruct* bucketList = bucket->asgmt->truth==0?source->falseList:source->trueList;
 *				
 *				if(bucket->asgmt->position == bucketID){
 *					if(conjunct1 == NULL){  // The first predicate in the conjunct
 *						conjunct1 = bucketList;
 *					} else {                // No the first predicate in the conjunct
 *						conjunct1 = intersectIntervalList(conjunct1,bucketList);
 *					} 
 *				} else {
 *					if(conjunct0 && conjunct1){
 *						conjunct0 = forwardInfluence(conjunct0,lastBucketID-bucketID,k,conjunct1);                                                
 *					} else {
 *						conjunct0 = conjunct1;
 *					}
 *					conjunct1 = bucketList;
 *					lastBucketID = bucketID;
 *					bucketID = bucket->asgmt->position;
 *				}                             
 *				bucket = bucket->next;
 *			}
 *			if(conjunct0 && conjunct1){
 *				conjunct0 = forwardInfluence(conjunct0,lastBucketID-bucketID,k,conjunct1);
 *				influence = forwardInfluence(conjunct0,bucketID,k,targetList);
 *				
 *			} else if(conjunct1 && conjunct0==NULL){
 *				influence = forwardInfluence(conjunct1,bucketID,k,targetList);
 *			} else if(conjunct1 == NULL){
 *				influence = targetList;
 *			} 
 *			#ifdef SUP_DEBUG
 *			fprintf(logFile,"[computeForwardInfluenceForAnAssertion] ENDED\n");
 *			#endif
 *			return influence;
 *		}
 *	}
 *	#ifdef SUP_DEBUG
 *	fprintf(logFile,"[computeForwardInfluenceForAnAssertion] ENDED\n");
 *	#endif
 *	return NULL;
 * }
 */

/*
 * struct intervalListStruct* computeForwardInfluenceForAnAssertionForTruth(struct treeNode* assertion,int targetID,double k,int truth){
 *	#ifdef SUP_DEBUG
 *	fprintf(logFile,"[computeForwardInfluenceForAnAssertionForTruth] BEGIN\n");
 *	#endif
 *	if(assertion){
 *		if(assertion->truthList && assertion->intervalList){
 *			sortTruthAssignmentList(&assertion->truthList);
 *			struct truthAssignmentListStruct* bucket = assertion->truthList;
 *			
 *			struct intervalListStruct* conjunct0 = NULL;
 *			int lastBucketID = 0;
 *			struct intervalListStruct* conjunct1 = NULL;
 *			struct intervalListStruct* influence = NULL;
 *			
 *			int bucketID = bucket->asgmt->position;
 *			lastBucketID = bucketID;
 *			
 *			//struct listOfIntervalListsStruct* target = getListAtPosition(assertion->intervalList,targetID);
 *			struct listOfIntervalListsStruct* target = getListAtPosition(intervalSet,targetID);
 *			struct intervalListStruct* targetList = truth==0?target->falseList:target->trueList;
 *			int smallestBucketID = getSmallestBucketID(assertion->truthList);
 *			
 *			while(bucket){
 *				if(bucket->asgmt->position == smallestBucketID){
 *				
 *					struct listOfIntervalListsStruct* source = getListAtPosition(intervalSet,bucket->asgmt->predicate_id);
 *					struct intervalListStruct* bucketList = bucket->asgmt->truth==0?source->falseList:source->trueList;
 *					
 *					influence = forwardInfluence(bucketList,smallestBucketID,k,targetList);
 *				}
 *				//bucket->asgmt->position // i
 *				//bucket->asgmt->predicate_id //porv
 *				//bucket->asgmt->truth    //true or false list
 *				//assertion->mean         //target truth
 *				
 *				bucket = bucket->next;
 *			}
 *			#ifdef SUP_DEBUG
 *			fprintf(logFile,"[computeForwardInfluenceForAnAssertionForTruth] ENDED\n");
 *			#endif
 *			return influence;
 *		}
 *	}
 *	#ifdef SUP_DEBUG
 *	fprintf(logFile,"[computeForwardInfluenceForAnAssertionForTruth] ENDED\n");
 *	#endif
 *	return NULL;
 * }
 * 
 */

struct intervalListStruct** computeForwardInfluenceForAnAssertion(struct treeNode* assertion,int targetID,double k){
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeForwardInfluenceForAnAssertion] STARTED\n");
	#endif
	if(assertion){
		if(assertion->truthList && listOfIntervalSets){
			int smallestBucketID = getSmallestBucketID(assertion->truthList);
			
			struct intervalListStruct** endMatchIntervalList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
			bzero(endMatchIntervalList,sizeof(struct intervalListStruct*)*traceCount);
			
			struct intervalListStruct** forwardInfluenceIntervalList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
			bzero(forwardInfluenceIntervalList,sizeof(struct intervalListStruct*)*traceCount);
			struct listOfIntervalListsStruct* target = NULL;
			struct intervalListStruct* targetList = NULL;
			int i;
			for(i=0;i<traceCount;i++){
				intervalSet = listOfIntervalSets[i];
				//printf("Computing endMatchForPrefix over constraint list:");printTruthListToFilePtr(assertion->truthList,stdout);printf("\n");
				endMatchIntervalList[i] = endMatchForPrefix(assertion->truthList);
				target = getListAtPosition(intervalSet,targetID);
				targetList = (assertion->truthValue==0?(assertion->mean==0.0?target->trueList:target->falseList):(assertion->mean==1.0?target->trueList:target->falseList));
				
				forwardInfluenceIntervalList[i] = forwardInfluence(endMatchIntervalList[i],smallestBucketID,k,targetList);
			}
			
			#ifdef METHOD_CALL
			fprintf(logFile,"[computeForwardInfluenceForAnAssertion] ENDED\n");
			#endif
			
			return forwardInfluenceIntervalList;                              
		}
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[computeForwardInfluenceForAnAssertion] ENDED\n");
	#endif
	return NULL;
}

/*
 * Compute sum of lengths over multiple interval lists
 */
double lengthOfIntervalLists(struct intervalListStruct **intervalLists){
	#ifdef METHOD_CALL
	fprintf(logFile,"[lengthOfIntervalLists] STARTED\n");
	#endif
	if(intervalLists){
		int i;
		double totalLengthOfLists = 0.0;
		for(i=0;i<traceCount;i++){
			totalLengthOfLists+=lengthOfIntervalList(intervalLists[i]);
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"[lengthOfIntervalLists] ENDED\n");
		#endif
		return totalLengthOfLists;
	}
	#ifdef METHOD_CALL
	fprintf(logFile,"[lengthOfIntervalLists] ENDED\n");
	#endif
	return 0.0;
}

/*
 * Compute End Matches over all traces
 */
struct intervalListStruct** endMatchesForPrefix(struct truthAssignmentListStruct* prefix){
	if(prefix){
		struct intervalListStruct** endMatchIntervalList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
		bzero(endMatchIntervalList,sizeof(struct intervalListStruct*)*traceCount);
		
		int i;
		for(i=0;i<traceCount;i++){
			intervalSet = listOfIntervalSets[i];
			endMatchIntervalList[i] = endMatchForPrefix(prefix);
			
			#ifdef VERBOSE_LOW
			fprintf(logFile,"\n[endMatchesForPrefix] EM for Trace [%d]: ",i);
			if(endMatchIntervalList[i]){
				printIntervalListToFilePtr(endMatchIntervalList[i],logFile);
			} else { fprintf(logFile,"EMPTY"); }
			fprintf(logFile,"\n");
			#endif
			
		}
		return endMatchIntervalList;	
	}
	return NULL;
}



/*
 * Returns the forward influence interval for a given set of constraints
 */
struct intervalListStruct* endMatchForPrefix(struct truthAssignmentListStruct* tempConstraintList){
	#ifdef METHOD_CALL
	fprintf(logFile,"[endMatchForPrefix] STARTED\n");fflush(logFile);
	fprintf(logFile,"Constraint List:");
	printTruthListToFilePtr(tempConstraintList,logFile);
	fflush(logFile);
	#endif
	
	//printf("[endMatchForPrefix] BEGIN\n");
	//printf("Constraint List:\n");
	//printTruthListToFilePtr(tempConstraintList,stdout);
	//printListOfIntervalListsToFilePtr(intervalSet,stdout);
	
	if(tempConstraintList){
		if(tempConstraintList && intervalSet){
			
			#ifdef VERBOSE_LOW
			fprintf(logFile,"\n[endMatchForPrefix] List of Interval Lists:\n");
			printListOfIntervalListsToFilePtr(intervalSet,logFile);
			#endif

			//Prepare a sorted list of prefix constraints
			struct truthAssignmentListStruct* constraintList = duplicateTruthAssignmentList(tempConstraintList);
			sortTruthAssignmentList(&constraintList);
			//fprintf(logFile,"Yo yo\n");fflush(logFile);
			
			//Compute the forward influence
			struct truthAssignmentListStruct* bucket = constraintList;
			
			struct intervalListStruct* conjunct0 = NULL;
			struct intervalListStruct* conjunct1 = NULL;
			struct intervalListStruct* influence = NULL;
			
			int bucketID = bucket->asgmt->position;
			
			int lastBucketID = bucketID;
			
			while(bucket){ 
				//bucket->asgmt->position // i
				//bucket->asgmt->predicate_id //porv
				//bucket->asgmt->truth    //true or false list
				//assertion->mean         //target truth
				
				//fprintf(logFile,"Predicate %s%d - Truth = %d \n",bucket->asgmt->type?"LP":"KP",bucket->asgmt->predicate_id,bucket->asgmt->truth);fflush(logFile);
				
				// Intersect all constraints in the same bucket
				struct listOfIntervalListsStruct* source = getListAtPosition(bucket->asgmt->type?learnedIntervalSet:intervalSet,bucket->asgmt->predicate_id);
				struct intervalListStruct* intervalList = bucket->asgmt->truth==0?source->falseList:source->trueList;
				if(intervalList == NULL){ return NULL; }
				
				//Does the predicate fall in the bucket under consideration? <-bucketId->
				if(bucket->asgmt->position == bucketID){ 
					//YES
					if(conjunct1 == NULL){  // The first predicate in the conjunct
						conjunct1 = intervalList;
					} else {                // No the first predicate in the conjunct
						conjunct1 = intersectIntervalList(conjunct1,intervalList);
						if(conjunct1==NULL){
							return NULL;
						}
					}
				} else {//NO
					if(conjunct1!=NULL){// If the last sub-expression has a non-null match
						if(conjunct0==NULL){//When one bucket has been processed
							conjunct0 = conjunct1;
						} else {//Two sub-expressions are processed and must be merged
							conjunct0 = forwardInfluence(conjunct0,lastBucketID-bucketID,K,conjunct1);
						}
						//Update the bucket id under consideration - remember last bucket that was processed
						lastBucketID = bucketID;
						bucketID = bucket->asgmt->position;
						conjunct1 = intervalList;
					} else {//Conjunct 1 - the last processed sub-expression - 
						//has no match
						influence = NULL;
						return NULL;
					}
					
				}
				//At this point either one bucket exists <-bucketID-> 	[CASE 1]
				//Or two buckets exist, one bucket that is in the past <-lastBucketID-> 
				//and one under consideration <-bucketID->.		[CASE 2]
				bucket = bucket->next;
				//fprintf(logFile,"bucketID = %d \n",bucketID);fflush(logFile);
				
			}
			//Merge everything
			//fprintf(logFile,"About to go to Case 2\n");fflush(logFile);
			
			if(conjunct0!=NULL && conjunct1!=NULL){ //[CASE 2]
				conjunct0 = forwardInfluence(conjunct0,lastBucketID-bucketID,K,conjunct1);
				influence = conjunct0;//minkowskiSumList(conjunct0,(double)0,((double)bucketID)*K);
			} else if(conjunct1!=NULL && conjunct0==NULL){//[CASE 1]
				influence = conjunct1;//minkowskiSumList(conjunct1,(double)0,((double)bucketID)*K);
			} else if(conjunct1 == NULL){
				influence = NULL;
				//fprintf(logFile,"Houston we have a problem... Expected Conjunct1 to be NON-NULL, thus fwd inf is 0\n");
				//exit(0);
			} 
			#ifdef METHOD_CALL
			printTruthListToFilePtr(tempConstraintList,logFile);
			fprintf(logFile,"[endMatchForPrefix] ENDED\n");fflush(logFile);
			#endif
			//printf("[endMatchForPrefix] Influence Computed to be:");
			//printIntervalListToFilePtr(influence,stdout);
			//fprintf(stdout,"\n[endMatchForPrefix] ENDED\n");
			return influence;
		}
	} else {
		/*
		 * When the constraint list is empty, the influence is the entire trace
		 */
		#ifdef METHOD_CALL
		fprintf(logFile,"[endMatchForPrefix] ENDED\n");fflush(logFile);
		#endif
		return createIntervalList(createIntervalStruct(0,getMaxIntervalSet(intervalSet)));
	}
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[endMatchForPrefix] ENDED\n");fflush(logFile);
	#endif
	return NULL;
}


/*
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
int cfileexists(const char * filename){
	/* try to open file to read */
	FILE *file;
	if (file = fopen(filename, "r")){
		fclose(file);
		return 1;
	}
	return 0;
}


//Belief and Correlation

double getCorrelation(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[getCorrelation] STARTED\n");
	#endif
	
	if(truthList && targetList){
		double correlation = 0.0;
		int smallestBucketID = getSmallestBucketID(truthList);
		
		struct intervalListStruct* endMatch = endMatchForPrefix(truthList);
		struct intervalListStruct* forwardInfList = forwardInfluence(endMatch,smallestBucketID,K,targetList);
		
		struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(intervalSet,targetPORV_id)->falseList,forwardInfList);
		
		correlation=maxFloat(lengthOfIntervalList(overlap)/totalTrueLength,lengthOfIntervalList(overlap)/totalFalseLength);
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[getCorrelation] ENDED\n");
		#endif
		
		return correlation;
		
	} 
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[getCorrelation] ENDED\n");
	#endif
	
	return 0;
}

double getCorrelationForTruth(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int truth, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[getCorrelationForTruth] STARTED\n");
	#endif
	
	if(truthList && targetList){
		double correlation = 0.0;
		int smallestBucketID = getSmallestBucketID(truthList);
		
		struct intervalListStruct* endMatch = endMatchForPrefix(truthList);
		struct intervalListStruct* forwardInfList = forwardInfluence(endMatch,smallestBucketID,K,targetList);
		
		struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(intervalSet,targetPORV_id)->falseList,forwardInfList);
		
		correlation= truth?lengthOfIntervalList(overlap)/totalTrueLength:lengthOfIntervalList(overlap)/totalFalseLength;
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[getCorrelationForTruth] ENDED\n");
		#endif
		
		return correlation;
		
	} 
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[getCorrelationForTruth] ENDED\n");
	#endif
	
	return 0;
}


double getSupport(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[getSupport] STARTED\n");
	#endif
	
	if(truthList && targetList){
		double support = 0.0;
		
		int smallestBucketID = getSmallestBucketID(truthList);
		
		struct intervalListStruct* endMatch = endMatchForPrefix(truthList);
		struct intervalListStruct* forwardInfList = forwardInfluence(endMatch,smallestBucketID,K,targetList);
		
		struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(intervalSet,targetPORV_id)->falseList,forwardInfList);
		
		support = lengthOfIntervalList(overlap)/totalTraceLength;
		
		#ifdef METHOD_CALL
		fprintf(logFile,"[getSupport] ENDED\n");
		#endif
		
		return support;
	} 
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[getSupport] ENDED\n");
	#endif
	
	return 0;
}

int testCorrelationSupport(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[testCorrelationSupport] STARTED\n");
	#endif
	
	if(truthList && targetList){
		double support = 0.0;
		double correlation = 0.0;
		int smallestBucketID = getSmallestBucketID(truthList);
		
		struct intervalListStruct* endMatch = endMatchForPrefix(truthList);
		struct intervalListStruct* forwardInfList = forwardInfluence(endMatch,smallestBucketID,K,targetList);
		
		struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(intervalSet,targetPORV_id)->falseList,forwardInfList);
		
		correlation=maxFloat(lengthOfIntervalList(overlap)/totalTrueLength,lengthOfIntervalList(overlap)/totalFalseLength);
		support = lengthOfIntervalList(overlap)/totalTraceLength;
		
		if(correlation>=correlationThreshold && support>=supportThreshold){
			#ifdef METHOD_CALL
			fprintf(logFile,"[testCorrelationSupport] ENDED\n");
			#endif
			
			return 1;
		}
		
	} 
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[testCorrelationSupport] ENDED\n");
	#endif
	
	return 0;
}

int testCorrelationSupportForTruth(struct truthAssignmentListStruct* truthList, int truth, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[testCorrelationSupport] STARTED\n");
	#endif
	
	if(truthList){
		struct listOfIntervalListsStruct* targetLists = getListAtPosition(intervalSet,targetPORV_id);
		struct intervalListStruct* targetList = truth?targetLists->trueList:targetLists->falseList;
		double support = 0.0;
		double correlation = 0.0;
		int smallestBucketID = getSmallestBucketID(truthList);
		
		struct intervalListStruct* endMatch = endMatchForPrefix(truthList);
		struct intervalListStruct* forwardInfList = forwardInfluence(endMatch,smallestBucketID,K,targetList);
		
		struct intervalListStruct* overlap = intersectIntervalList(targetList,forwardInfList);
		
		correlation=truth?lengthOfIntervalList(overlap)/totalTrueLength:lengthOfIntervalList(overlap)/totalFalseLength;
		support = lengthOfIntervalList(overlap)/totalTraceLength;
		
		//fprintf(stdout,"\nthresh = %lf\n",supportThreshold);
		printTruthListToFilePtr(truthList,stdout);
		fprintf(stdout,"correlation = %lf, support = %lf\n",correlation,support);
		if(correlation>=correlationThreshold && support>=supportThreshold){
			#ifdef METHOD_CALL
			fprintf(logFile,"[testCorrelationSupport] ENDED\n");
			#endif
			
			return 1;
		}
		
	} 
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[testCorrelationSupport] ENDED\n");
	#endif
	
	return 0;
}

//Pretty Printers for Assertions
/*
 * Compute the buckets of time-shifted truth intervals
 */
struct intervalListStruct** computeBuckets(struct treeNode* node){
	if(node){
		fprintf(logFile,"0-HERE-1-1-1\n");fflush(logFile);
		sortTruthAssignmentList(&node->truthList);
		fprintf(logFile,"1-HERE-1-1-1\n");fflush(logFile);
		struct truthAssignmentListStruct* constraintList = node->truthList;
		int bucketCount = 0;
		
		if(constraintList!=NULL){
			bucketCount = constraintList->asgmt->position;
			fprintf(logFile,"HERE-1-1-1\n");fflush(logFile);
			struct intervalListStruct** bucket = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*(bucketCount+1));
			bzero(bucket,sizeof(struct intervalListStruct*)*(bucketCount+1));
			fprintf(logFile,"HERE-1-1-2\n");fflush(logFile);
			constraintList = node->truthList;
			int i = 0;
			int j=0;
			
			for(j=0;j<traceCount;j++){
				printf("TRACE[%d]\n",j);
				while(constraintList){
					fprintf(logFile,"HERE-1-2-1\n");fflush(logFile);
					
					
					struct listOfIntervalListsStruct* currentIntervalSet = listOfIntervalSets[j];
					
					struct listOfIntervalListsStruct* tempList = getListAtPosition(constraintList->asgmt->type?learnedIntervalSet:currentIntervalSet,constraintList->asgmt->predicate_id);
					i = constraintList->asgmt->position;
					if(bucket[i]){
						bucket[i] = intersectIntervalList(bucket[i],constraintList->asgmt->truth?tempList->trueList:tempList->falseList);
					} else {
						bucket[i] = duplicateIntervalList((constraintList->asgmt->truth)?tempList->trueList:tempList->falseList);
					}
					
					fprintf(logFile,"HERE-1-2-2\n");fflush(logFile);
					constraintList = constraintList->next;
				}
				i = bucketCount;
				
				for(i=bucketCount;i>=0;i--){
					fprintf(stdout,"\nBucket[%d]:\n",i);
					printIntervalListToFilePtr(bucket[i],stdout);
					fprintf(stdout,"\n");
					bucket[i]=NULL;
				}
				
			}
			fprintf(logFile,"HERE-1-1-3\n");fflush(logFile);
			//Print Buckets
			#ifdef SUP_DEBUG
			
			#endif
			fprintf(logFile,"END-HERE-1\n");fflush(logFile);
			return bucket;
		} else {
			fprintf(logFile,"END-HERE-2\n");fflush(logFile);
			return NULL;
			
		}
	}
}

/* Compute tight temporal separation interval between the target and a given bucket = list1-list2
 */
/*
 * struct intervalStruct* computeTightSeparation(struct intervalListStruct* list1,struct intervalListStruct* list2, int pos){
 *		if(list1 && list2){
 *				struct intervalListStruct* forwardInfInterval = minkowskiSumList(list1,(double)0,((double)pos)*K);
 *				struct intervalListStruct* targetIntervalList = intersectIntervalList(forwardInfInterval,list2);
 *				
 *				#ifdef SUP_DEBUG
 *						printf("------- forward list pos [%d] with k [%lf] ----------\n",pos,K);
 *						printIntervalList(forwardInfInterval);
 *						printf("\n------------------------------\n\n");
 *						printf("------- target Initial----------\n");
 *						printIntervalList(list2);
 *						printf("\n------------------------------\n\n");
 *						
 *						printf("-------- target list----------\n");
 *						printIntervalList(targetIntervalList);
 *						printf("\n------------------------------\n\n");
 *				#endif
 *				
 *				struct intervalListStruct* bucketInterval = list1;
 *				
 *				struct intervalListStruct* separations = NULL;
 *				struct intervalListStruct* dummyList = createIntervalList(NULL);
 *				struct intervalListStruct* intersectList = NULL;
 *				struct intervalStruct* maxInterval = createIntervalStruct(0,pos*K);
 *				
 *		//For each bucket interval, For each 
 *				while(forwardInfInterval && bucketInterval){
 *			//Get the next widened bucket (taken forward and possibly merged)
 *						dummyList->interval = forwardInfInterval->interval;
 *						
 *			//Get all the target intervals that the forward taken bucket influences 
 *						intersectList = intersectIntervalList(targetIntervalList,dummyList);
 *						struct intervalStruct* partInterval = NULL;
 *						while(intersectList){
 *				//For each target intervals infuenced by the current expanded & merged bucket
 *				//Compute the min-max difference with the current bucket.
 *								partInterval = minmaxDiff(intersectList->interval,bucketInterval->interval->l,bucketInterval->interval->r);
 *								
 *								#ifdef SUP_DEBUG
 *										printf("PART INTERVAL: [%lf:%lf] - [%lf:%lf]: ",intersectList->interval->l,intersectList->interval->r,bucketInterval->interval->l,bucketInterval->interval->r);printInterval(partInterval);printf("\n"); 
 *								#endif
 *										
 *								partInterval = intersectIntervals(partInterval,maxInterval);
 *								
 *								if( partInterval && (partInterval->r - partInterval->l) >=0 ) {
 *										//printf("HERE 1\n");
 *										separations = addIntervalToList(separations,createIntervalList(partInterval));
 *								} else {
 *										//printf("HERE 2\n");
 *										delete(partInterval);
 *										partInterval = NULL;
 *								}
 *								
 *								intersectList = intersectList->next;
 *								
 *						}
 *						
 *						//BOTH SHOULD NOT MOVE AT THE SAME SPEED BECAUSE THE FWDINFINTERVAL may 
 *						//be a merge of multiple buckets. 
 *						//This may lead to missing some separation windows.
 *						forwardInfInterval = forwardInfInterval->next;
 *						bucketInterval = bucketInterval->next;
 *						
 *				}
 *				
 *				return widenIntervalSet(separations);
 *		}
 *		return NULL;
 * }
 */

/* Compute tight temporal separation interval between the target and a given bucket = list1-list2
 */
struct intervalStruct* computeTightSeparation(struct intervalListStruct* list1,struct intervalListStruct* list2, int distance){
	fprintf(logFile,"[computeTightSeparation] STARTED: List1[%p] List2 [%p] Distance[%d].\n",list1,list2,distance);
	fflush(logFile);
	if(list1 && list2){
		struct intervalListStruct* forwardInfInterval = minkowskiSumList(list1,(double)0,((double)distance)*K);
		struct intervalListStruct* targetIntervalList = intersectIntervalList(forwardInfInterval,list2);
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"------- forward list pos [%d] with k [%lf] ----------\n",distance,K);
		printIntervalListToFilePtr(forwardInfInterval,logFile);
		fprintf(logFile,"\n------------------------------\n\n");
		fprintf(logFile,"------- target Initial----------\n");
		printIntervalListToFilePtr(list2,logFile);
		fprintf(logFile,"\n------------------------------\n\n");
		
		fprintf(logFile,"-------- target list----------\n");
		printIntervalListToFilePtr(targetIntervalList,logFile);
		fprintf(logFile,"\n------------------------------\n\n");
		#endif
		fflush(logFile);
		struct intervalListStruct* bucketInterval = list1;
		struct intervalListStruct* listOfSeparations = NULL;
		struct intervalListStruct* dummyList = createIntervalList(NULL);
		struct intervalListStruct* intersectList = NULL;
		struct intervalStruct* maxInterval = createIntervalStruct(0,distance*K);
		
		//For each bucket interval
		while(bucketInterval){
			//Get the next widened bucket (taken forward and possibly merged)
			dummyList->interval = minkowskiSum(bucketInterval->interval,(double)0,((double)distance)*K);//forwardInfInterval->interval;
			
			//Get all the target intervals that the forward taken bucket influences 
			intersectList = intersectIntervalList(targetIntervalList,dummyList);
			
			/* 
			 * partInterval is a delay interval for bucketInterval in list1 and 
			 * a bucketInterval influenced in list2.
			 */
			struct intervalStruct* partInterval = NULL;
			
			while(intersectList){
				//For each target intervals infuenced by the current expanded & merged bucket
				//Compute the min-max difference with the current bucket.
				//partInterval = minmaxDiff(intersectList->interval,bucketInterval->interval->l,bucketInterval->interval->r);
				partInterval = minkowskiDiff(intersectList->interval,bucketInterval->interval->l,bucketInterval->interval->r);
				
				#ifdef SUP_DEBUG
				fprintf(logFile,"PART INTERVAL: [%lf:%lf] - [%lf:%lf]: ",intersectList->interval->l,intersectList->interval->r,bucketInterval->interval->l,bucketInterval->interval->r);printIntervalToFilePtr(partInterval,logFile);fprintf(logFile,"\n"); fflush(logFile);
				#endif
				
				//Make sure the part interval is only within the window of influence.
				//This is achieved by intersecting it with a max window interval.
				partInterval = intersectIntervals(partInterval,maxInterval);
				
				if( partInterval && (partInterval->r - partInterval->l) >=0 ) {
					//printf("HERE 1\n");
					listOfSeparations = addIntervalToList(listOfSeparations,createIntervalList(partInterval));
				} else {
					//printf("HERE 2\n");
					free(partInterval);
					partInterval = NULL;
				}
				
				intersectList = intersectList->next;				
			}
			
			bucketInterval = bucketInterval->next;
			
		}
		fprintf(logFile,"[computeTightSeparation] ENDED: Calling widenIntervalSet\n");
		fflush(logFile);
		return widenIntervalSet(listOfSeparations);
	}
	fprintf(logFile,"[computeTightSeparation] ENDED: NULL\n");
	return NULL;
}

/*
 * This method computes the separation between a bucket and the next bucket or
 * a bucket and the target.
 */
struct intervalStruct* computeSeparationAnyMatch(struct intervalListStruct** bucket,int bucketPos,struct intervalListStruct** targetList, int targetPos, int smallestBucketID){
	//Potential for SEGFAULT
	fprintf(logFile,"[computeSeparationAnyMatch] STARTED : Bucket Postition = %d\n",bucketPos);
	
	if(targetList && bucketPos>=targetPos){
		/*
		 * Compute an any-match separation between the bucket at position "pos" and the next.
		 * bucketCount+1 * tractCount
		 */
		struct intervalListStruct* list1 = *((bucket+bucketPos*traceCount)+0);
		struct intervalListStruct* list2 = NULL;
		int position = 0;
		int nextBucketPos = 0;
		if(bucketPos==smallestBucketID || bucketPos==0){
			//Compute distance to target at targetPos.
			position = bucketPos;
		} else {
			//Find the next non-empty bucket
			int i,j;
			for(i=bucketPos-1;i>=0;i--){
				for(j=0;j<traceCount;j++){
					if(*((bucket+i*traceCount)+j)){//For i(th) bucket, j(th) trace
						nextBucketPos = i;
						break;
					}
				}
			}
			
			fprintf(logFile,"Next bucket position = %d\n",nextBucketPos);
			
			//Compute Difference
			position = bucketPos-nextBucketPos;
		}
		
		struct intervalStruct* separation = NULL;
		int i;
		for(i=0;i<traceCount;i++){
			list1 = *((bucket+bucketPos*traceCount)+i);
			list2 = nextBucketPos==0?targetList[i]:*((bucket+nextBucketPos*traceCount)+i);
			struct intervalStruct* temp = computeTightSeparation(list1, list2, position); 
			if(temp){
				struct intervalListStruct* tempList = createIntervalList(temp);
				if(separation) tempList->next = createIntervalList(separation);
				separation = widenIntervalSet(tempList);
			}
		}
		
		fprintf(logFile,"[computeSeparationAnyMatch] ENDED - Calling computeTightSeparation\n");
		return separation;
	}
	fprintf(logFile,"[computeSeparationAnyMatch] ENDED: NULL\n");
	return NULL;
}

/*
 * Wrapper to compute separation. Presently computes a wide separation.
 */
struct intervalStruct* computeBucketSeparation(struct intervalListStruct** bucket, struct intervalListStruct** list2, int pos1, int pos2, int smallestBucketID){
	switch(separationType){
		case 1: return computeSeparationAnyMatch(bucket, pos1, list2, pos2, smallestBucketID);
	}
}

/* 
 * Compute tight temporal separation interval between the target and a given bucket = list1-list2
 */
/*
 * struct intervalStruct* computeSeparation(struct intervalListStruct* list1,struct intervalListStruct* list2, int pos){
 *		return computeTightSeparation(list1,list2,pos);
 *		if(list1 && list2){
 *				struct intervalListStruct* forwardInfInterval = minkowskiSumList(list1,(double)0,((double)pos)*K);
 *				struct intervalListStruct* targetIntervalList = intersectIntervalList(forwardInfInterval,list2);
 *				
 *				#ifdef SUP_DEBUG
 *						printf("------- forward list pos [%d] with k [%lf] ----------\n",pos,K);
 *						printIntervalList(forwardInfInterval);
 *						printf("\n------------------------------\n\n");
 *						printf("------- target Initial----------\n");
 *						printIntervalList(list2);
 *						printf("\n------------------------------\n\n");
 *						
 *						printf("-------- target list----------\n");
 *						printIntervalList(targetIntervalList);
 *						printf("\n------------------------------\n\n");
 *				#endif
 *				
 *				struct intervalListStruct* bucketInterval = list1;
 *				
 *				struct intervalListStruct* separations = NULL;
 *				struct intervalListStruct* dummyList = createIntervalList(NULL);
 *				struct intervalListStruct* intersectList = NULL;
 *				struct intervalStruct* maxInterval = createIntervalStruct(0,pos*K);
 *				
 *				while(forwardInfInterval && bucketInterval){
 *						dummyList->interval = forwardInfInterval->interval;
 *						
 *						intersectList = intersectIntervalList(targetIntervalList,dummyList);
 *						struct intervalStruct* partInterval = NULL;
 *						while(intersectList){
 *								partInterval = minkowskiDiff(intersectList->interval,bucketInterval->interval->l,bucketInterval->interval->r);
 *								
 *								#ifdef SUP_DEBUG
 *										printf("PART INTERVAL: [%lf:%lf] - [%lf:%lf]: ",intersectList->interval->l,intersectList->interval->r,bucketInterval->interval->l,bucketInterval->interval->r);printInterval(partInterval);printf("\n"); 
 *								#endif
 *										
 *								partInterval = intersectIntervals(partInterval,maxInterval);
 *								if( (partInterval->r - partInterval->l) >0 ) {
 *										separations = addIntervalToList(separations,createIntervalList(partInterval));
 *								} else {
 *										delete(partInterval);
 *										partInterval = NULL;
 *								}
 *								intersectList = intersectList->next;
 *						}
 *						
 *						
 *						forwardInfInterval = forwardInfInterval->next;
 *						bucketInterval = bucketInterval->next;
 *						
 *				}
 *				
 *				return widenIntervalSet(separations);
 *		}
 *		return NULL;
 * }
 */


struct intervalStruct* widenIntervalSet(struct intervalListStruct* list){
	#ifdef METHOD_CALL
	fprintf(logFile,"[widenIntervalSet] STARTED\n");
	#endif
	fflush(logFile);
	if(list){
		double left,right;
		left = DBL_MAX;
		right = -1.0 * (DBL_MAX);
		//printf("[%e] \n [%e]\n",left,right);
		
		struct intervalListStruct* item = list;
		while(item){
			#ifdef SUP_DEBUG
			fprintf(logFile,"[widenIntervalSet] item [%p][%p]\n",item,item->next);
			#endif
			fflush(logFile);
			if(item->interval->l < left){
				left = item->interval->l;
			}
			
			if(item->interval->r > right){
				right = item->interval->r;
			}
			
			item = item->next;
		}
		#ifdef METHOD_CALL
		fprintf(logFile,"[widenIntervalSet] ENDED\n");
		#endif
		fflush(logFile);
		return createIntervalStruct(left,right);
	}
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[widenIntervalSet] ENDED - NULL\n");
	#endif
	fflush(logFile);
	return NULL;
}

void writeAssertionWithTruthToStruct(struct assertionStruct* assertion, struct intervalListStruct** targetList, struct intervalListStruct** bucket, int bucketCount, int truth, struct treeNode* node,int position, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[writeAssertionWithTruthToStruct] STARTED : bucketCount = %d\n",bucketCount);
	//printTreeNodeToFilePtr(node,fp);
	#endif
	char* tempStr = (char*)malloc(sizeof(char)*MAX_STR_LENGTH);
	bzero(tempStr,sizeof(char)*MAX_STR_LENGTH);
	
	fflush(logFile);
	if(assertion && targetList && bucket && (bucketCount>=0) && node){
		//Compute Seperations
		//Widen(B+[0:pos*k] intersect Target - B)
		//fprintf(fp,"Internal (1)\n");
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[writeAssertionWithTruthToStruct] TargetPos[%d] Truth = %d\n",position,truth);            
		fprintf(logFile,"[writeAssertionWithTruthToStruct] Constraint Set\n");
		printTruthListToFilePtr(node->truthList,logFile);
		//fprintf(fp,"[writeAssertionWithTruthToStruct] Bucket Count = %d\n",bucketCount);                
		fprintf(logFile,"[writeAssertionWithTruthToStruct] Bucket Count = %d\n",bucketCount); 
		#endif
		
		double support = 0.0;
		double correlation = 0.0;
		
		if(truth==0){
			//fprintf(fp,"Internal (2) : Position %d\n",position);
			//Compute forward influence interval list and use that!!!
			//printTruthListToFilePtr(node->truthList,fp);fprintf(fp,"\n");	
			struct intervalListStruct** endMatch = endMatchesForPrefix(node->truthList);
			
			struct intervalListStruct** forwardInfList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
			bzero(forwardInfList,sizeof(struct intervalListStruct*)*traceCount);
			
			int i;
			double totalEvidence = 0.0;
			double totalOverlap = 0.0;
			for(i=0;i<traceCount;i++){
				forwardInfList[i] = forwardInfluence(endMatch[i],position,K,targetList[i]);
				totalEvidence += lengthOfIntervalList(forwardInfList[i]);
				
				struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(listOfIntervalSets[i],targetPORV_id)->falseList,forwardInfList[i]);
				totalOverlap += lengthOfIntervalList(overlap);
			}
			
			correlation = totalOverlap/totalFalseLength;
			support = totalEvidence/totalTraceLength;
			
		} else {
			//fprintf(fp,"Internal (3) : Position %d\n",position);
			//printTruthListToFilePtr(node->truthList,fp);fprintf(fp,"\n");	
			struct intervalListStruct** endMatch = endMatchesForPrefix(node->truthList);
			struct intervalListStruct** forwardInfList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
			bzero(forwardInfList,sizeof(struct intervalListStruct*)*traceCount);
			
			int i;
			double totalEvidence = 0.0;
			double totalOverlap = 0.0;
			for(i=0;i<traceCount;i++){
				forwardInfList[i] = forwardInfluence(endMatch[i],position,K,targetList[i]);
				totalEvidence += lengthOfIntervalList(forwardInfList[i]);
				
				struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(listOfIntervalSets[i],targetPORV_id)->trueList,forwardInfList[i]);
				totalOverlap += lengthOfIntervalList(overlap);
			}
			
			correlation = totalOverlap/totalTrueLength;
			support = totalEvidence/totalTraceLength;
		}		
		
		fprintf(logFile,"SUPPORT = [%lf]\n",support*100.0);
		fprintf(logFile,"CORRELATION = [%lf]\n",correlation*100.0);
		
		assertion->support = support*100.0;
		assertion->correlation = correlation*100.0;
		
		if(correlation == 0.0){ return; }
		if(bucketCount==0){
			//printTruthListForIndex(node->truthList,0,fp,0);
			if(node->truthList == NULL){	//For a VALID or UNSAT target
				strcat(assertion->assertion,"TRUE ");
			}
			char *targetName = getPredicateName(targetPORV_id);
			
			if(!truth){
				if(targetName){
					sprintf(tempStr,"|=> !%s\n",targetName);strcat(assertion->assertion,tempStr);
				} else {
					sprintf(tempStr,"|=> !P%d\n",targetPORV_id);strcat(assertion->assertion,tempStr);
				}
			} else {
				if(targetName){
					sprintf(tempStr,"|=> %s\n",targetName);strcat(assertion->assertion,tempStr);
				} else {
					sprintf(tempStr,"|=> P%d\n",targetPORV_id);strcat(assertion->assertion,tempStr);
				}
			}
			
			#ifdef SUP_DEBUG
			fprintf(logFile,"[writeAssertionWithTruthToStruct] ENDED\n");
			#endif
			
			return;
		}
		
		//Compute Bucket Separation Intervals
		struct intervalStruct* bucketSepIntervals[bucketCount];	//Store separation intervals
		bzero(bucketSepIntervals,sizeof(struct intervalStruct*)*bucketCount);
		int i;
		
		int smallestBucketID = getSmallestBucketID(node->truthList);
		
		for(i=bucketCount;i>=0;i--){
			//Compute the seperataion
			bucketSepIntervals[i] = computeBucketSeparation(bucket,targetList,i,position,smallestBucketID);
			
			#ifdef SUP_DEBUG
			fprintf(logFile,"\n[writeAssertionWithTruthToStruct] Bucket %d\n",i);
			fflush(logFile);
			if(bucket[i]) {
				fprintf(logFile,"\n[writeAssertionWithTruthToStruct] Bucket Interval List: ");
				printIntervalListToFilePtr(bucket[i],logFile);
				fprintf(logFile,"\n");
			}
			fflush(logFile);
			if(bucketSepIntervals[i]!=NULL){
				fprintf(logFile,"[writeAssertionWithTruthToStruct] For bucket %d Seperation [%lf:%lf]\n",i,bucketSepIntervals[i]->l,bucketSepIntervals[i]->r);                                
			} else fprintf(logFile,"[writeAssertionWithTruthToStruct] For bucket %d Seperation [0 : 0]\n",i);
			#endif
		}      
		fflush(logFile);
		
		i = bucketCount;
		int flag = 0;
		int allNull = 1;
		
		while(i>=0){
			if(bucketSepIntervals[i]==NULL){
				if(i==0){
					writeTruthListForIndex(node->truthList,0,assertion->assertion,0);
				} else {
					i--;continue;
				}
				
			} else if(bucketSepIntervals[i]!=NULL && i==smallestBucketID){
				allNull = 0;
				writeTruthListForIndex(node->truthList,i,assertion->assertion,0);
				break;
			} else if(bucketSepIntervals[i]!=NULL){
				allNull = 0;
				writeTruthListForIndex(node->truthList,i,assertion->assertion,0);
				sprintf(tempStr," ##[ %e : %e ] ",bucketSepIntervals[i]->l,bucketSepIntervals[i]->r);strcat(assertion->assertion,tempStr);
				
			} 
			
			i--;
		}
		fprintf(logFile,"YY %d allNull =[%d]\n",smallestBucketID,allNull);fflush(logFile);
		
		char *targetName = getPredicateName(targetPORV_id);
		char newTargetName[MAX_STR_LENGTH]; 
		if(targetName){
			sprintf(newTargetName,"%s",targetName);
		} else {
			sprintf(newTargetName,"P%d\n",targetPORV_id);
		}
		
		if(smallestBucketID>0 && allNull==0){
			if(!truth){
				sprintf(tempStr,"|=> ##[ %e : %e ] !%s\n",bucketSepIntervals[smallestBucketID]->l,bucketSepIntervals[smallestBucketID]->r,newTargetName);strcat(assertion->assertion,tempStr);
			} else {
				sprintf(tempStr,"|=> ##[ %e : %e ] %s\n",bucketSepIntervals[smallestBucketID]->l,bucketSepIntervals[smallestBucketID]->r,newTargetName);strcat(assertion->assertion,tempStr);
			}
			
		} else {
			if(!truth){
				sprintf(tempStr,"|=> !%s\n",newTargetName);strcat(assertion->assertion,tempStr);
			} else {
				sprintf(tempStr,"|=> %s\n",newTargetName);strcat(assertion->assertion,tempStr);
			}
		}
		fprintf(logFile,"YY %d allNull =[%d]\n",smallestBucketID,allNull);fflush(logFile);
		
	}
	
	#ifdef SUP_DEBUG
	fprintf(logFile,"[writeAssertionWithTruthToStruct] ENDED\n");
	#endif
	fflush(logFile);
}


void printAssertionWithTruthToFile(FILE* fp, struct intervalListStruct** targetList, struct intervalListStruct** bucket, int bucketCount, int truth, struct treeNode* node,int position, int targetPORV_id){
	#ifdef SUP_DEBUG
	fprintf(logFile,"[printAssertionWithTruthToFile] STARTED : bucketCount = %d\n",bucketCount);
	//printTreeNodeToFilePtr(node,fp);
	#endif
	fflush(logFile);
	if(fp && targetList && bucket && (bucketCount>=0) && node){
		//Compute Seperations
		//Widen(B+[0:pos*k] intersect Target - B)
		//fprintf(fp,"Internal (1)\n");
		#ifdef ASSERT_PRINT_DEBUG
		fprintf(fp,"[printAssertionWithTruthToFile] TargetPos[%d] Truth = %d\n",position,truth);                
		fprintf(fp,"[printAssertionWithTruthToFile] Constraint Set:\n");
		printTruthListToFilePtr(node->truthList,fp);
		#endif
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"[printAssertionWithTruthToFile] TargetPos[%d] Truth = %d\n",position,truth);            
		fprintf(logFile,"[printAssertionWithTruthToFile] Constraint Set\n");
		printTruthListToFilePtr(node->truthList,logFile);
		//fprintf(fp,"[printAssertionWithTruthToFile] Bucket Count = %d\n",bucketCount);                
		fprintf(logFile,"[printAssertionWithTruthToFile] Bucket Count = %d\n",bucketCount); 
		#endif
		
		double support = 0.0;
		double correlation = 0.0;
		int smallestBucketID = getSmallestBucketID(node->truthList);
		struct intervalListStruct** endMatch = endMatchesForPrefix(node->truthList);
		struct intervalListStruct** forwardInfList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
		bzero(forwardInfList,sizeof(struct intervalListStruct*)*traceCount);
		
		//Correcting truth
		float trueMean = computeTrueMean2(targetList,endMatch);
		float falseMean = computeFalseMean2(targetList,endMatch);
		if(trueMean==1.0 || falseMean==1.0){
			if(truth==0 && falseMean!=1.0 && trueMean==1.0){
				truth=1;
				//Update TargetList;
				int i;
				struct listOfIntervalListsStruct** targetTrueFalseLists=getListsAtPosition(listOfIntervalSets,smallestBucketID);
				for(i=0;i<traceCount;i++){
					targetList[i]=targetTrueFalseLists[i]->trueList;
				}
			} 
			if(truth==1 && trueMean!=1.0 && falseMean==1.0){
				truth=0;
				//Update TargetList;
				int i;
				struct listOfIntervalListsStruct** targetTrueFalseLists=getListsAtPosition(listOfIntervalSets,smallestBucketID);
				for(i=0;i<traceCount;i++){
					targetList[i]=targetTrueFalseLists[i]->trueList;
				}
			}
		}
		
		if(truth==0){
			int i;
			double totalEvidence = 0.0;
			double totalOverlap = 0.0;
			for(i=0;i<traceCount;i++){
				//fprintf(fp,"Trace[%d]: End matches at:\n",i);printIntervalListToFilePtr(endMatch[i],fp);fprintf(fp,"\n");
				//fprintf(fp,"Target List for trace [%d]:\n",i);printIntervalListToFilePtr(targetList[i],fp);fprintf(fp,"\n");
				forwardInfList[i] = forwardInfluence(endMatch[i],position,K,targetList[i]);
				//fprintf(fp,"Forward Influence for trace [%d]:\n",i);printIntervalListToFilePtr(forwardInfList[i],fp);fprintf(fp,"\n");
				totalEvidence += lengthOfIntervalList(forwardInfList[i]);
				
				struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(listOfIntervalSets[i],targetPORV_id)->falseList,forwardInfList[i]);
				
				totalOverlap += lengthOfIntervalList(overlap);
			}
			
			#ifdef ASSERT_PRINT_DEBUG
				fprintf(fp,"\n-----------------DEBUG---------------------\n");
				fprintf(fp,"Length of Evidence = [%f]\n",totalEvidence);
				fprintf(fp,"Length of Overlap = [%f]\n",totalOverlap);
				fprintf(fp,"\n---------------DEBUG ENDS------------------\n");
			#endif
			
			correlation = totalOverlap/totalFalseLength;
			support = totalEvidence/totalTraceLength;
		} else {
			int i;
			double totalEvidence = 0.0;
			double totalOverlap = 0.0;
			for(i=0;i<traceCount;i++){
				//fprintf(fp,"Trace[%d]: End matches at:\n",i);printIntervalListToFilePtr(endMatch[i],fp);fprintf(fp,"\n");
				//fprintf(fp,"Target List for trace [%d]:\n",i);printIntervalListToFilePtr(targetList[i],fp);fprintf(fp,"\n");
				forwardInfList[i] = forwardInfluence(endMatch[i],position,K,targetList[i]);
				//fprintf(fp,"Forward Influence for trace [%d]:\n",i);printIntervalListToFilePtr(forwardInfList[i],fp);fprintf(fp,"\n");
				totalEvidence += lengthOfIntervalList(forwardInfList[i]);
				
				struct intervalListStruct* overlap = intersectIntervalList(getListAtPosition(listOfIntervalSets[i],targetPORV_id)->trueList,forwardInfList[i]);
				totalOverlap += lengthOfIntervalList(overlap);
			}
			
			#ifdef ASSERT_PRINT_DEBUG
				fprintf(fp,"\n-----------------DEBUG---------------------\n");
				fprintf(fp,"Length of Evidence = [%f]\n",totalEvidence);
				fprintf(fp,"Length of Overlap = [%f]\n",totalOverlap);
				fprintf(fp,"\n---------------DEBUG ENDS------------------\n");
			#endif

			correlation = totalOverlap/totalTrueLength;
			support = totalEvidence/totalTraceLength;
		}
		
		fprintf(logFile,"SUPPORT = [%lf]\n",support*100.0);
		fprintf(logFile,"CORRELATION = [%lf]\n",correlation*100.0);
		
		
		#ifdef ASSERT_PRINT_DEBUG
		fprintf(fp,"\n-----------------DEBUG---------------------\n");
		fprintf(fp,"SUPPORT = [%lf]\n",support*100.0);
		fprintf(fp,"CORRELATION = [%lf]\n",correlation*100.0);
		fprintf(fp,"\n---------------DEBUG ENDS------------------\n");

		#endif
		
		if(correlation == 0.0){ return; }
		if(bucketCount==0){
			printTruthListForIndex(node->truthList,0,fp,0);
			if(node->truthList == NULL){	//For a VALID or UNSAT target
				fprintf(fp,"TRUE ");
			}
			char *targetName = getPredicateName(targetPORV_id);
			
			if(!truth){
				if(targetName){
					fprintf(fp,"|=> !%s\n",targetName);
				} else {
					fprintf(fp,"|=> !P%d\n",targetPORV_id);
				}
			} else {
				if(targetName){
					fprintf(fp,"|=> %s\n",targetName);
				} else {
					fprintf(fp,"|=> P%d\n",targetPORV_id);
				}
			}
			
			fprintf(fp,"SUPPORT\t\t= [%lf]\n",support*100.0);
			fprintf(fp,"CORRELATION\t= [%lf]\n",correlation*100.0);
			
			#ifdef SUP_DEBUG
			fprintf(logFile,"[printAssertionWithTruthToFile] ENDED\n");
			#endif
			
			return;
		}
		
		//Compute Bucket Separation Intervals
		struct intervalStruct* bucketSepIntervals[bucketCount];	//Store separation intervals
		bzero(bucketSepIntervals,sizeof(struct intervalStruct*)*bucketCount);
		int i;
		
		//printTruthListToFilePtr(node->truthList,logFile);
		
		//int smallestBucketID = getSmallestBucketID(node->truthList);
		//fprintf(logFile,"SBI=%d\n",smallestBucketID);
		//int deltaPosition = smallestBucketID-position;
		//if(deltaPosition<0)
		for(i=bucketCount;i>=0;i--){
			//Compute the seperataion
			bucketSepIntervals[i] = computeBucketSeparation(bucket,targetList,i,position,smallestBucketID);
			
			#ifdef SUP_DEBUG
			fprintf(logFile,"\n[printAssertionWithTruthToFile] Bucket %d\n",i);
			fflush(logFile);
			if(bucket[i]) {
				fprintf(logFile,"\n[printAssertionWithTruthToFile] Bucket Interval List: ");
				printIntervalListToFilePtr(bucket[i],logFile);
				fprintf(logFile,"\n");
			}
			fflush(logFile);
			if(bucketSepIntervals[i]!=NULL){
				fprintf(logFile,"[printAssertionWithTruthToFile] For bucket %d Seperation [%lf:%lf]\n",i,bucketSepIntervals[i]->l,bucketSepIntervals[i]->r);                                
			} else fprintf(logFile,"[printAssertionWithTruthToFile] For bucket %d Seperation [0 : 0]\n",i);
			#endif
		}
		fflush(logFile);
		
		i = bucketCount;
		int flag = 0;
		int allNull = 1;
		
		while(i>=0){
			if(bucketSepIntervals[i]==NULL){
				if(i==0){
					printTruthListForIndex(node->truthList,0,fp,0);
				} else {
					i--;continue;
				}
			} else if(bucketSepIntervals[i]!=NULL && i==smallestBucketID){
				//fprintf(logFile,"\nNot-Null for Bucket [%i] - smallest\n",i);
				allNull = 0;
				printTruthListForIndex(node->truthList,i,fp,0);
				break;
			} else if(bucketSepIntervals[i]!=NULL){
				//fprintf(logFile,"\nNot-Null for Bucket [%i]\n",i);
				allNull = 0;
				printTruthListForIndex(node->truthList,i,fp,0);
				//fprintf(fp," ##[ %e : %e ] ",bucketSepIntervals[i]->l,bucketSepIntervals[i]->r);
				fprintf(fp," ##[ ");
				if(bucketSepIntervals[i]->l>0.0 && bucketSepIntervals[i]->l < SMALL_DBL){
					fprintf(fp,"%e : ",bucketSepIntervals[i]->l);
				} else {
					fprintf(fp,"%lf : ",newPrecision(bucketSepIntervals[i]->l,NUM_DIGITS));
				}
				if(bucketSepIntervals[i]->r>0.0 && bucketSepIntervals[i]->r < SMALL_DBL){
					fprintf(fp,"%e",bucketSepIntervals[i]->r);
				} else {
					fprintf(fp,"%lf",newPrecision(bucketSepIntervals[i]->r,NUM_DIGITS));
				}
				fprintf(fp," ] ");
				
// 				if(i>=1){
//                                         int j = i-1;
// 					while(bucketSepIntervals[j]==NULL && j>0){
//                                                 j--;                                                
//                                         }
//                                         if(j==0){
// 						fprintf(fp," ##[ %lf : %lf ] ",bucketSepIntervals[i]->l,bucketSepIntervals[i]->r);
//                                         } else {
// 						struct intervalStruct* temp = minkowskiIntervalDiff(bucketSepIntervals[i],bucketSepIntervals[j]);
//                                                 fprintf(fp," ##[ %lf : %lf ] ",temp->l,temp->r);
//                                         }
//                                         
// 					
//                                 }
                        } 
                        
			
			
			//flag = printTruthListForIndex(node->truthList,i,fp,flag) || flag;
			
			//if(i==smallestBucketID) {
			//	fprintf(fp," ##[ %lf : %lf ] ",bucketSepIntervals[i]->l,bucketSepIntervals[i]->r);
			//}
                        i--;
                }
                fprintf(logFile,"YY %d allNull =[%d]\n",smallestBucketID,allNull);fflush(logFile);
		
		char *targetName = getPredicateName(targetPORV_id);
		char newTargetName[MAX_STR_LENGTH]; 
		if(targetName){
			sprintf(newTargetName,"%s",targetName);
		} else {
			sprintf(newTargetName,"P%d\n",targetPORV_id);
		}
                
		if(smallestBucketID>0 && allNull==0){
			fprintf(fp,"|=> ##[ ");
			if(bucketSepIntervals[smallestBucketID]->l>0.0 && bucketSepIntervals[smallestBucketID]->l < SMALL_DBL){
				fprintf(fp,"%e : ",bucketSepIntervals[i]->l);
			} else {
				fprintf(fp,"%lf : ",newPrecision(bucketSepIntervals[smallestBucketID]->l,NUM_DIGITS));
			}
			if(bucketSepIntervals[smallestBucketID]->r>0.0 && bucketSepIntervals[smallestBucketID]->r < SMALL_DBL){
				fprintf(fp,"%e",bucketSepIntervals[smallestBucketID]->r);
			} else {
				fprintf(fp,"%lf",newPrecision(bucketSepIntervals[smallestBucketID]->r,NUM_DIGITS));
			}
			fprintf(fp," ] ");
			
			if(!truth)
				fprintf(fp,"!%s\n",newTargetName);
			else
				fprintf(fp,"%s\n",newTargetName);
			
			fflush(fp);
			fprintf(fp,"SUPPORT\t\t= [%lf]\n",support*100.0);
			fprintf(fp,"CORRELATION\t= [%lf]\n",correlation*100.0);
		} else {
			if(!truth)
				fprintf(fp,"|=> !%s\n",newTargetName);
			else
				fprintf(fp,"|=> %s\n",newTargetName);
			
			fflush(fp);
			fprintf(fp,"SUPPORT\t\t= [%lf]\n",support*100.0);
			fprintf(fp,"CORRELATION\t= [%lf]\n",correlation*100.0);
		}
		fprintf(logFile,"YY %d allNull =[%d]\n",smallestBucketID,allNull);fflush(logFile);
		
	}
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[printAssertionWithTruthToFile] ENDED\n");
	#endif
	fflush(logFile);
}


/* 
 * Prints Assertions in ASCII text as SVA sequences with dense intervals and PORVs
 */
void printAssertions(struct treeNode* node, FILE* fp, int targetPORV_id){
	#ifdef METHOD_CALL
	fprintf(logFile,"[printAssertions] STARTED\n");
	#endif
	if(node->id==3){
		fprintf(logFile,"\nCHECK WHATS HAPPENING HERE --- ABC ---\n");
	}
	
	if(node && fp){
		#ifdef SUP_DEBUG
		printTreeNodeToFilePtr(node, logFile,targetPORV_id);
		fflush(logFile);
		#endif
		
		sortTruthAssignmentList(&node->truthList);
		
		struct truthAssignmentListStruct* constraintList = node->truthList;
		int bucketCount = 0;
		
		#ifdef SUP_DEBUG
		fprintf(logFile,"*******************CONSTRAINT LIST PTR [%p]***********************\n",constraintList);
		printTruthListToFilePtr(constraintList,logFile);
		//REMOVING THIS FFLUSH CAN CAUSE A SEGFAULT: AND I HAVE NO IDEA WHY :-( 
		fflush(logFile);
		#endif
		
		#ifdef ASSERT_PRINT_DEBUG
			fprintf(fp,"[printAssertions] Constraint List PTR = [%p]\n",constraintList);
		#endif
		
		//Compute Buckets
		if(constraintList!=NULL){
			//fprintf(fp,"here\n");
			bucketCount = constraintList->asgmt->position;
			int truth = 0;
			//Mean and Error Check
			int smallestBucketID = getSmallestBucketID(constraintList);
			
			//Check if error non-zero
			#ifdef ASSERT_PRINT_DEBUG
				fprintf(fp,"[printAssertions] (Node Error = [%lf]) ==0? [ %s ]\n",node->error,node->error == 0.0?"Yes":"No");
			#endif
			if(node->error!=0.0){
				//fprintf(fp,"here - error not zero L4769\n");
				#ifdef SUP_DEBUG
				fprintf(logFile,"[printAssertions] Non-zero Node Error of [%lf]\n",node->error);
				printTreeNodeToFilePtr(node,logFile,targetPORV_id);
				#endif
				
				return;
			}
			
			/* Compute Bucket Interval Lists  */ 
			struct intervalListStruct* bucket[bucketCount+1][traceCount];
			bzero(bucket, traceCount*(bucketCount+1)*sizeof(struct intervalListStruct*));
			
			constraintList = node->truthList;
			int i = 0;
			int j = 0;
			fflush(logFile);
			
			while(constraintList){
				i = constraintList->asgmt->position;
				
				for(j=0;j<traceCount;j++){
					struct listOfIntervalListsStruct* tempList = getListAtPosition(constraintList->asgmt->type?learnedIntervalSet:listOfIntervalSets[j],constraintList->asgmt->predicate_id);
					
					if(bucket[i][j]){
						bucket[i][j] = intersectIntervalList(bucket[i][j],constraintList->asgmt->truth?tempList->trueList:tempList->falseList);
					} else {
						bucket[i][j] = duplicateIntervalList((constraintList->asgmt->truth)?tempList->trueList:tempList->falseList);
					}
				}
				constraintList = constraintList->next;
			}
			
			
			#ifdef SUP_DEBUG
			//Print Buckets
			i = bucketCount;
			for(i=bucketCount;i>=0;i--){
				for(j=0;j<traceCount;j++){
					fprintf(logFile,"\n[printAssertions] Bucket[%d][trace<%d>]:\n",i,j);
					printIntervalListToFilePtr(bucket[i][j],logFile);
					fprintf(logFile,"\n");fflush(logFile);
				}
			}
			#endif
			
			struct listOfIntervalListsStruct** targetTrueFalseList = getListsAtPosition(listOfIntervalSets,targetPORV_id);
			
			struct intervalListStruct** targetList = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
			
			//fprintf(fp,"here - before printing assertions\n");
			struct intervalListStruct** endMatch = endMatchesForPrefix(node->truthList); 
			double Htrue = computeTrueEntropy(targetTrueFalseList,endMatch);
			double TrueOverlap = computeEndMatchOverlapWithTarget(targetTrueFalseList,endMatch,1);
			double Hfalse = computeFalseEntropy(targetTrueFalseList,endMatch);
			double FalseOverlap = computeEndMatchOverlapWithTarget(targetTrueFalseList,endMatch,0);
			fprintf(logFile,"[writingAssertions] Htrue = %lf, Hfalse = %lf\n",Htrue, Hfalse);
			//if((float)(((int)(fabs(Htrue)*10e5))/10e5) == 0.0 && (float)(((int)(fabs(Hfalse)*10e5))/10e5) == 0.0){
			if(newPrecision(fabs(Htrue),5)==0.0 && TrueOverlap>0.0 && newPrecision(fabs(Hfalse),5)==0.0 && FalseOverlap>0.0){
				fprintf(logFile,"Target is both True and False\n");
				fprintf(fp,"Target entropy is zero in both True and False states.\n");
				#ifdef ASSERT_PRINT_DEBUG
				fprintf(fp,"HERE If - Entropy is close to 0\n");fflush(fp);
				fprintf(fp,"here (1) - sending control to printAssertionWithTruthToFile\n");
				fprintf(fp,"\n");
				#endif
				
				for(i=0;i<traceCount;i++){
					targetList[i] = targetTrueFalseList[i]->trueList;
				}
				
				struct assertionStruct* assertion1 = createAssertionStruct();
				writeAssertionWithTruthToStruct(assertion1,targetList,(struct intervalListStruct**)bucket,bucketCount,1,node,smallestBucketID,targetPORV_id);
				allAssertions=addToAsssertionList(assertion1,allAssertions);
				
				printAssertionWithTruthToFile(fp,targetList,(struct intervalListStruct**)bucket,bucketCount,1,node,smallestBucketID,targetPORV_id);
				fprintf(fp,"\n");
				for(i=0;i<traceCount;i++){
					targetList[i] = targetTrueFalseList[i]->falseList;
				}
				
				struct assertionStruct* assertion2 = createAssertionStruct();
				writeAssertionWithTruthToStruct(assertion2,targetList,(struct intervalListStruct**)bucket,bucketCount,0,node,smallestBucketID,targetPORV_id);
				allAssertions=addToAsssertionList(assertion2,allAssertions);
				
				printAssertionWithTruthToFile(fp,targetList,(struct intervalListStruct**)bucket,bucketCount,0,node,smallestBucketID,targetPORV_id);        
			} else {
				if(fabs(node->error) == 0.0){
					#ifdef ASSERT_PRINT_DEBUG
					fprintf(fp,"here (2) - sending control to printAssertionWithTruthToFile\n");
					#endif
					
					if(node->truthValue == 0){//Target is false
						for(i=0;i<traceCount;i++){
							targetList[i] = targetTrueFalseList[i]->falseList;
						}
						struct assertionStruct* assertion = createAssertionStruct();
						writeAssertionWithTruthToStruct(assertion,targetList,(struct intervalListStruct**)bucket,bucketCount,0,node,smallestBucketID,targetPORV_id);
						allAssertions=addToAsssertionList(assertion,allAssertions);
						
						printAssertionWithTruthToFile(fp,targetList,(struct intervalListStruct**)bucket,bucketCount,0,node,smallestBucketID,targetPORV_id);
						//}
					} else {//Target is true
						for(i=0;i<traceCount;i++){
							targetList[i] = targetTrueFalseList[i]->trueList;
						}
						
						struct assertionStruct* assertion = createAssertionStruct();
						writeAssertionWithTruthToStruct(assertion,targetList,(struct intervalListStruct**)bucket,bucketCount,1,node,smallestBucketID,targetPORV_id);
						allAssertions=addToAsssertionList(assertion,allAssertions);
						
						printAssertionWithTruthToFile(fp,targetList,(struct intervalListStruct**)bucket,bucketCount,1,node,smallestBucketID,targetPORV_id);						
					}
				}
			}
		} else if(fabs(node->error) == 0.0){
			int targetInfluence = 0;
			int foundTrue = 0;	//Found a valid assertion for target true
			int foundFalse = 0;	//Found a valid assertion for target false
			//The flags are used to stop the search at the least value of delay found.
			for(targetInfluence=0;targetInfluence<N;targetInfluence++){
				
				double trueMean = computeMean(listOfIntervalSets,targetInfluence?numberOfPORVs+targetInfluence:targetPORV_id,node->traceLength,validLists);//createIntervalList(createIntervalStruct(0.0,totalTraceLength)));
				double falseMean = computeFalseMean(listOfIntervalSets,targetInfluence?numberOfPORVs+targetInfluence:targetPORV_id,node->traceLength,validLists);//createIntervalList(createIntervalStruct(0.0,totalTraceLength)));
				
				if(trueMean == 1.0 && !(foundTrue==0)){
					fprintf(fp,"TRUE |=>");
					fprintf(fp," ##[ %lf : %lf ] P%d\n",(double)0,K*(targetInfluence),targetPORV_id);
					foundTrue = 1;
				}
				
				if(falseMean == 1.0 && !(foundFalse==0)){
					fprintf(fp,"TRUE |=>");
					fprintf(fp," ##[ %lf : %lf ] !P%d\n",(double)0,K*(targetInfluence),targetPORV_id);
					foundFalse = 1;
				}
			}
		} else {
				fflush(fp);
				fprintf(fp,"ERROR: False Alarm - No Assertion Here.\n");
		}
	} else {
		fflush(fp);
		printf("ERROR: The node is empty. This should not have happened.\n");
	}
	fflush(fp);
	#ifdef METHOD_CALL
	fprintf(logFile,"[printAssertions] ENDED\n");
	//fprintf(fp,"[printAssertions] ENDED\n");
	#endif
}


void printFloatArrayToFile(double* array, int size, FILE* fp){
	if(array && fp && (size%2==0)){
		int i = 0;
		fprintf(fp,"{ ");
		for(i=0;i<size;i = i+2){
			fprintf(fp,"[ %lf, %lf ] ",array[i],array[i+1]);                        
		}
		fprintf(fp,"}\n");
	}
}


void printFloat2DArraryToFile(int size1, int size2, double array[][size2], FILE* fp){
	fprintf(logFile,"[printFloat2DArraryToFile] STARTED size-1 = [%d] ; size-2 = [%d].\n",size1,size2);
	if(array && fp){
		int i = 0;
		int j = 0;
		fprintf(fp,"{\n");
		for(i=0;i<size2;i++){//Next predicate
			fprintf(fp,"[%d] : ",i);//Predicate ID
			
			for(j=0;j<size1;j++){//For target in its true form and false
				fprintf(fp,"  (%d)[ %lf ] , ",j,array[j][i]);
				
			}
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n}\n");
	}	
}

/*
 * void printFloat2DArraryToFile(int size1, int size2, double array[][size2], FILE* fp){
 * fprintf(logFile,"[printFloat2DArraryToFile] STARTED size-1 = [%d] ; size-2 = [%d].\n",size1,size2);
 * if(array && fp && (size1%2==0)){
 *	int i = 0;
 *	int j = 0;
 *	fprintf(fp,"{\n");
 *	for(i=0;i<size2;i++){//Next predicate
 *		fprintf(fp,"[%d] : ",i);//Predicate ID
 *	
 *		for(j=0;j<size1;){//For target in its true form and false
 *			fprintf(fp,"  (%d)[ %lf, %lf ] , ",j/2,array[j][i],array[j+1][i]);
 *			j+=2;
 *		}
 *		fprintf(fp,"\n");
 *	}
 *	fprintf(fp,"\n}\n");
 * }	
 * } */

void copyMatrix(double ** target, int m, int n, double source[][n]){
	fprintf(logFile,"[copyMatrix] STARTED\n");
	int i,j;
	
	target = (double **) malloc(m * sizeof(double *));
	for (i = 0; i < m; i++)
		target[i] = (double *)malloc(n * sizeof(double));
	
	for(i=0; i < m; i++){
		for(j=0; j < n; j++){
			target[i][j] = source[i][j];
		}
	}
	fprintf(logFile,"[copyMatrix] ENDED\n");
}


/*
 * Returns list of target positions and truths where error is zero.
 * This is done so as to track all assertions with all delays where 
 * the target error is zero. It is always expected that for a pseudo-
 * target at a delay of 'k' having zero error, will force all pseudo-
 * targets at delays of >'k' to have zero erro at the same node.
 * 
 * Each pair is <target position, truth>
 * mapping to parameters [target, predicate] in the indexCouple structure.
 * 
 * Yes Bad Design, I know, this needs to be changed. It was done a long while
 * back and it never changed. Maybe change the names to <key,val> or something
 * of the sort.
 */
/*struct indexCouple* findError(struct treeNode* node){ 
 *	#ifdef SUP_DEBUG
 *	fprintf(logFile,"[findError] STARTED\n");
 *	#endif
 *	if(node){
 *		struct indexCouple* list = NULL;
 *		double* errors = node->errors;
 *		int i = N*2-1;
 *		
 *		#ifdef SUP_DEBUG
 *		//FILE* testout = fopen(assertFileName?assertFileName:"assertions.txt","a");
 *		fprintf(logFile,"ERRORS:");
 *		printFloatArrayToFile(errors,N*2,logFile);
 *		//fclose(testout);
 *		#endif
 *		
 *		while(errors && i>=0){
 *			if(errors[i] == 0.0){
 *				//Target Position, Truth
 *				addToIndexCoupleList(&list,createIndexCouple(i/2,1-i%2));
 *				
 *			}
 *			i--;
 *		}
 *		
 *		#ifdef SUP_DEBUG
 *		printIndexCoupleListToFilePtr(list,logFile);
 *		fprintf(logFile,"[findError] ENDED\n");
 *		#endif
 *		return list;
 *	}
 *	#ifdef SUP_DEBUG
 *	fprintf(logFile,"[findError] ENDED\n");
 *	#endif
 *	return NULL;
 * }
 */
int countCombinations(struct listOfIntervalListsStruct* truthSet, int* predList, int len){
	#ifdef METHOD_CALL
	fprintf(logFile,"[countCombinations] STARTED\n");
	#endif
	
	
	
	#ifdef METHOD_CALL
	fprintf(logFile,"[countCombinations] ENDED\n");
	#endif
}

double getMax(double val1, double val2){
	return val1>val2?val1:val2;
}

double getMaxIntervalList(struct intervalListStruct* list){
	if(list){
		double max=-1.0;
		while(list){
			double temp = getMax((double)list->interval->l,(double)list->interval->r);
			if(max<temp){
				max = temp;
			}
			list = list->next;
		}
		return max;
	}
	return -1.0;	
}

double getMaxIntervalSet(struct listOfIntervalListsStruct* set){
	if(set){
		double max = -1.0;
		while(set){
			double temp = getMaxIntervalList(set->trueList);
			if(max < temp){
				max = temp;
			}
			set = set->nextList;
		}
		return max;
	}
	return -1.0;	
}

int getSmallestBucketID(struct truthAssignmentListStruct* truthtList){
	if(truthtList){
		int id = N;
		while(truthtList){
			if(truthtList->asgmt->position < id){ 
				id = truthtList->asgmt->position;
				
			}
			truthtList = truthtList->next;
		}
		return id;
	}
	return 0;
}

FILE* getDTreeFilePtr(){
	static int opened=0;
	if(!opened){
		opened = 1;
		return fopen("logs/dtree.txt","w");
	} else {
		return fopen("logs/dtree.txt","a");
	}
}

FILE* getDTreeFilePtrWithName(char* name){
	static int opened=0;
	if(name){
		char fileNewName[15+strlen(name)];
		sprintf(fileNewName,"logs/dtree-%s.txt",name);
		
		if(!opened){
			opened = 1;
			return fopen(fileNewName,"w");
		} else {
			return fopen(fileNewName,"a");
		}
	} else {
		return fopen("logs/dtree-no-name-targets.txt","a");
	}
}

//Predicate Detail

struct predicateDetail* createPredicateDetail(struct PORV* predicate, double gain, int bucket, struct intervalListStruct* trueList){
	struct predicateDetail* detail = (struct predicateDetail*)malloc(sizeof(struct predicateDetail));
	detail->predicate = predicate;
	detail->gain = gain;
	detail->bucket = bucket;
	detail->trueList = trueList;	
	detail->next = NULL;
}

struct predicateDetail* duplicatePredicateDetail(struct predicateDetail* root){
	if(root){
		struct predicateDetail* newRoot = NULL;
		newRoot = createPredicateDetail(duplicatePORVList(root->predicate),root->gain,root->bucket,duplicateIntervalList(root->trueList));
		root = root->next;
		struct predicateDetail* temp = newRoot;
		while(root){
			newRoot->next = createPredicateDetail(duplicatePORVList(root->predicate),root->gain,root->bucket,duplicateIntervalList(root->trueList));
			root = root->next;
			newRoot = newRoot->next;
		}
		newRoot->next = NULL;
		return temp;
	}
	
}


struct predicateDetail* getLastPredicateDetail(struct predicateDetail* root){
	if(root){
		while(root->next){
			root = root->next;
		}
		return root;
	}
	return NULL;
}

struct predicateDetail* addPredicateDetailToList(struct predicateDetail* root, struct predicateDetail* detail){
	if(root){
		struct predicateDetail* iterator = getLastPredicateDetail(root);
		iterator->next = detail;
		return root;
	} else {
		return detail;
	}	
}

void printPredicateDetailToFilePtr(struct predicateDetail* detail,FILE* fp){
	if(detail && fp){
		fprintf(fp,"Predicate: ");
		printPORVToFilePtr(detail->predicate,fp); fprintf(fp,"\n");
		fprintf(fp,"Interval List: "); printIntervalListToFilePtr(detail->trueList,fp); fprintf(fp,"\n");
		fprintf(fp,"Gain: %lf\n",detail->gain);
		fprintf(fp,"Bucket Position: %d\n",detail->bucket);
		fflush(fp);
	}
}

void printPredicateDetailListToFilePtr(struct predicateDetail* list, FILE* fp){
	if(list && fp){
		while(list){
			printPredicateDetailToFilePtr(list,fp);
			fprintf(fp,"\n");
			fflush(fp);
			list = list->next;
		}
	}
}

void printRequestForNewPredicate(struct config* configuration, struct identifier* varList,  struct intervalListStruct** buckets, int bucketCount, struct intervalListStruct* targetTrueList){
	if(configuration && varList){
		FILE* fp = fopen("sim_learn_config.cfg","w");
		
		printConfigToFilePtr(configuration, fp);
		/*
		 *		fprintf(fp,"dataset_file = %s\n",configuration->traceFileName);
		 *		fprintf(fp,"m = %d\n",configuration->bestGainCount);
		 *		fprintf(fp,"k = %lf\n",configuration->K);
		 *		fprintf(fp,"tmax = %lf\n",configuration->tmax);
		 *		fprintf(fp,"tmin = %lf\n",configuration->tmin);
		 *		fprintf(fp,"trace_length = %lf\n",totalTraceLength);
		 *		fprintf(fp,"n = %d\n",configuration->N);
		 *		
		 */
		
		fprintf(fp,"bias = %d\n",targetBias);
		fprintf(fp,"tschedule = linear\n");
		printVariableListToFilePtr(idList,fp);
		
		/*
		 *		fprintf(fp,"\nstart\n");
		 *		varList = idList;
		 *		while(varList){
		 *			fprintf(fp,"%s %d %d\n",varList->name,varList->col,varList->timeCol);
		 *			varList = varList->next;
	}
	fprintf(fp,"end\n");
	*/
		
		fprintf(fp,"\ninfluence_list_start\n");
		if(bucketCount>=0){
			int i = bucketCount;
			for(;i>=0;i--){
				if(buckets[i]){
					fprintf(fp,"%d {",i);
					printIntervalListToFilePtr(buckets[i],fp);
					fprintf(fp,"}\n");
				}
			}
		}
		fprintf(fp,"influence_list_end\n");
		
		fprintf(fp,"\ntarget_start\n");
		fprintf(fp,"{");
		printIntervalListToFilePtr(targetTrueList,fp);
		fprintf(fp,"}");
		fprintf(fp,"\ntarget_end\n");
		
		fclose(fp);
	}
}


void learnNewPredicates(){
	FILE* fp = fopen("learnedOutput.txt","a");
	if(!fp){
		fprintf(stdout,"We have a proble opening the log file.\n"); exit(0);
	}
	
	pid_t pyPID=fork();	
	
	if (pyPID==0)
	{ //child process 
		if(fp){
			dup2(fileno(fp),1);
			fclose(fp);
		}
		execlp("python","python","bpg_constrained.py","sim_learn_config.cfg",(char*)NULL);
		fclose(fp);
		printf("ERROR: FAILED TO RUN PytholearnNewPredicatesn Script bpg_constrained.py\n");
		exit(127); // only if execv fails 
	}
	else
	{ // pid!=0; parent process 
		
		waitpid(pyPID,0,0); // wait for child to exit
		fclose(fp);
		learnedOPin = fopen("best_predicates.txt","r");		
		learnedOPparse();
		fclose(learnedOPin);
		learnedOPin = NULL;
	}
	
}

void deletePredicateDetails(struct predicateDetail* node){
	if(node){
		if(node->next){
			deletePredicateDetails(node->next);
		}
		deletePORVs(node->predicate);
		deleteIntervalList(node->trueList);
		free(node);
	}
}

char* convertToDatFileName(char* temp){
	if(temp){
		char* datFileName = (char*)malloc(sizeof(char)*strlen(temp));
		strcpy(datFileName,temp);
		char* tempF = datFileName;
		while(*tempF!='\0' && *tempF!='.'){
			tempF++;
		}
		if(*tempF == '.'){
			tempF++;
			sprintf(tempF,"dat");
		}
		return datFileName;
	}
}

struct intervalListStruct** createValidLists(){
	if(listOfIntervalSets){
		struct intervalListStruct** validLists = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
		bzero(validLists,sizeof(struct intervalListStruct*)*traceCount);
		int i;
		for(i=0;i<traceCount;i++){
			validLists[i] = createIntervalList(createIntervalStruct(0,getMaxIntervalSet(listOfIntervalSets[i])));
		}
		return validLists;
	}
	return NULL;
}

/*
 * Checks whether the logging folder exists, if it does not, it is created
 */
int checkCreateLogDir(){
	struct stat st = {0};
	if(stat("logs",&st)==-1){
		if(mkdir("logs",0700)!=0){
			printf("Unable to create log directory. Exiting\n");
			exit(0);
		}
	}
}
int ignorePredicate(int predicateID){
	int traceID =0;
	double trueLength=0.0;
	double falseLength=0.0;
	for(traceID=0;traceID<traceCount;traceID++){
		intervalSet = listOfIntervalSets[traceID];
		struct listOfIntervalListsStruct* source = getListAtPosition(intervalSet,predicateID);
		trueLength+=lengthOfIntervalList(source->trueList);
		falseLength+=lengthOfIntervalList(source->falseList);
	}
	if(trueLength ==0.0 || falseLength == 0.0){ 
		fprintf(logFile,"[ignorePredicate] Ignoring KP[%d]\n",predicateID);
		return 1;
	}
	return 0;
}

double newPrecision(double n, double i) 
{ 
    return floor(pow(10,i)*n)/pow(10,i); 
}
