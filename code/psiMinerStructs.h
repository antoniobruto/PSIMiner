/*****************************************************************
 *                      CODE WRITTEN BY                          *
 *                                                               *
 *             Antonio Anastasio Bruto da Costa                  *
 *     Formal Method Laboratory, Dept. of CSE, IIT Kharagpur     *
 *                                                               *
 *****************************************************************/

/* 
 * amsMinerStructs.h : Where all the Interval Arithmetic structures, 
 * and supporting methods are defined.
 */

//#define DEBUG_ON

#ifndef MAX_STR_LENGTH
	#define MAX_STR_LENGTH 10240
#endif


#ifndef AMS_STRUCTS_H
        #define AMS_STRUCTS_H

#include "structs.h"       

//A Truth Assignment
struct truthAssignmentStruct{
	int predicate_id;
	int position;
	int truth;
	int type;		/*
					Type0: Knowledge Predicates
					Type1: Learned Predicates
				*/
};

struct indexCouple{
	int target;
	int predicate;
	int type;
	int truth;
	struct indexCouple* next;
};

//List of Truth Assignments
struct truthAssignmentListStruct{
	struct truthAssignmentStruct* asgmt;
	struct truthAssignmentListStruct* next;
};

//A Left Inclusive, Right Exclusive Interval
struct intervalStruct{
	double l;
	double r;
};

//List of Intervals
struct intervalListStruct{
	struct intervalStruct* interval;
	struct intervalListStruct* next;
};

// List of Interval Lists
struct listOfIntervalListsStruct{
	struct intervalListStruct* trueList;
	struct intervalListStruct* falseList;
	struct listOfIntervalListsStruct* nextList;
};

struct treeNode{					// --------- DECISION BUCKET ----------
	struct truthAssignmentListStruct* truthList;	//List of Predicate Truths
	struct listOfIntervalListsStruct** listOfIntervalSets;	//True & False Lists for each predicate
	struct listOfIntervalListsStruct** learnedIntervalSets;	//True & False Lists for each predicate
	double **gains;					//All gain pairs <target-position,predicateID>
	double mean;
        double error;
	int targetInfluence;				//Target Position Context for the node: 0 if the target has no temporal shift.
	int splittingPredicate_id;			//Predicate over which the current node will be split for its children.
	double traceLength;				//Length of Trace
	int trueFalseFlag;				//Indicates if the dual mean also contributes an assertion when the error is zero.
	struct treeNode* left;
	struct treeNode* right;
	struct treeNode* parent;
	//duplicate node pointers
	struct treeNode* dupToOriginal; 
	struct treeNode** originalToDup; //should be an array
	struct indexCouple* explored;
        int id;                                         //To keep track of the number of nodes created
        int truthValue;
	int predType;					//Type of predicate 0 if learned by knowledge, 1 otherwise.
	struct predicateDetail* learnedPredicates;	//Predicates learned by the predicate learning tool via Sim-Annealing
};

struct nodeList{
        struct treeNode* node;
        struct nodeList* next;
};

struct predicateDetail{
	struct PORV* predicate;
	double gain;
	int bucket;
	struct intervalListStruct** trueLists;	
	struct predicateDetail* next;	
};

struct assertionStruct{
	char assertion[MAX_STR_LENGTH];
	double correlation;
	double support;
	struct assertionStruct* next;
};

struct assertionStruct* createAssertionStruct();
struct assertionStruct* addToAsssertionList(struct assertionStruct* assertion, struct assertionStruct* root);

/*
struct iidentifier{		//Identifier Structure : Linked List used to store any generic list of identifiers
	unsigned int state;						//CSV Column - Offset of 1
	struct iidentifier* next;//Pointer to the next Identifier in the list or Null(If this is the last Identifier)
};
*/

/******************************Methods**********************************/

/*
//Identifiers
struct iidentifier* createIdentifier(unsigned int state);
struct iidentifier* addIdentifierToList(struct identifier* root, struct identifier* ID);
struct identifier* addToIdentifierList(struct identifier* root, unsigned int state);
int getIdentifierID(struct identifier* list, unsigned int state);
struct identifier* duplicateIdentifierList(struct identifier* root);
void freeIdentifierList(struct identifier* id);
char* containsIdentifier(struct identifier* list1, struct identifier* list2);
void printIdentifier(struct identifier* id);	
void printIdentifierList(struct identifier* id);
struct identifier* getIdentifierWithID(struct identifier* list, int id);
*/

//Truth Assignments
struct truthAssignmentListStruct* createAss();
struct truthAssignmentStruct* createTruthAssignment(int predicate_id, int truth, int position);
void addToTruthAssignmentList(struct truthAssignmentListStruct** root, struct truthAssignmentListStruct* listItem);
struct truthAssignmentListStruct* getLastListOfTruthAssignmentList(struct truthAssignmentListStruct* root);
struct truthAssignmentListStruct* duplicateTruthAssignmentList(struct truthAssignmentListStruct* root);
int predicateInList(struct truthAssignmentListStruct* root, int predicate);
void printTruthList(struct truthAssignmentListStruct* root);
int  printTruthListForIndex(struct truthAssignmentListStruct* root, int index, FILE* fp, int andprinter);
char* getPredicateName(int id);
int  writeTruthListForIndex(struct truthAssignmentListStruct* root, int index, char* str, int andprinter);
void printTruthListToFile(struct truthAssignmentListStruct* root);
void printTruthListToFilePtr(struct truthAssignmentListStruct* root, FILE* fp);
void sortTruthAssignmentList(struct truthAssignmentListStruct** root);
struct truthAssignmentListStruct* auxMergeTruthAssignmentList(struct truthAssignmentListStruct* a, struct truthAssignmentListStruct* b);
void listSplit(struct truthAssignmentListStruct* root, struct truthAssignmentListStruct** mid);
void purgeTruthList(struct truthAssignmentListStruct* root);
void purgeTruthListForTreeNode(struct treeNode* root);


//Index Couples
struct indexCouple* createIndexCouple(int target, int predicate);
void addToIndexCoupleList(struct indexCouple** root, struct indexCouple* listItem);
struct indexCouple* getLastOfIndexCoupleList(struct indexCouple* root);
int indexInList(struct indexCouple* root, int predicate, int target);
void printIndexCoupleList(struct indexCouple* root);
void printIndexCoupleListToFilePtr(struct indexCouple* root, FILE* fp);
struct indexCouple* duplicateIndexCouple(struct indexCouple* root);

//Intervals
struct intervalStruct* createIntervalStruct(double l, double r);
void printInterval(struct intervalStruct* interval);
void printIntervalToFilePtr(struct intervalStruct* interval,FILE* fp);
void deleteIntervalStruct(struct intervalStruct* interval);

//IntervalLists
struct intervalListStruct* createIntervalList(struct intervalStruct* interval);
void addToIntervalList(struct intervalListStruct** root, struct intervalListStruct* listItem);
struct intervalListStruct* addIntervalToList(struct intervalListStruct* root, struct intervalListStruct* listItem);
struct intervalListStruct* getLastIntervalInList(struct intervalListStruct* root);
void printIntervalList(struct intervalListStruct* list);
void printIntervalListToFilePtr(struct intervalListStruct* list, FILE* fp);
void deleteIntervalList(struct intervalListStruct* list);
struct intervalListStruct* flattenIntervalList(struct intervalListStruct* list);
struct intervalListStruct* duplicateIntervalList(struct intervalListStruct* list);
struct intervalListStruct* unionIntervalLists(struct intervalListStruct* list1, struct intervalListStruct* list2);
struct intervalListStruct* mergeIntervalLists(struct intervalListStruct* list1, struct intervalListStruct* list2);

//List of Interval Lists
struct listOfIntervalListsStruct* createListOfIntervalLists(struct intervalListStruct* trueList, struct intervalListStruct* falseList);
void addToListOfIntervalLists(struct listOfIntervalListsStruct** root, struct listOfIntervalListsStruct* listItem);
struct listOfIntervalListsStruct* addListToListOfIntervalLists(struct listOfIntervalListsStruct* root, struct listOfIntervalListsStruct* listItem);
struct listOfIntervalListsStruct* getLastListOfIntervalsInList(struct listOfIntervalListsStruct* root);
void printListOfIntervalLists(struct listOfIntervalListsStruct* list);

int printLengthOfIntervalLists(struct listOfIntervalListsStruct* list);
void printListOfIntervalListsToFilePtr(struct listOfIntervalListsStruct* list, FILE* fp);
void deleteListOfIntervalLists(struct listOfIntervalListsStruct* lists);
struct listOfIntervalListsStruct* duplicateIntervalSet(struct listOfIntervalListsStruct* root);
struct listOfIntervalListsStruct* unionIntervalSets(struct listOfIntervalListsStruct* list1, struct listOfIntervalListsStruct* list2);
struct intervalListStruct* flattenIntervalSet(struct listOfIntervalListsStruct* list);

//Intersection Computation
struct intervalStruct* intersectIntervals(struct intervalStruct* interval1, struct intervalStruct* interval2);
struct intervalListStruct* intersectIntervalList(struct intervalListStruct* list1, struct intervalListStruct* list2);
struct listOfIntervalListsStruct* intersectIntervalSet(struct listOfIntervalListsStruct* intervalSet1, struct intervalListStruct* intervalList2);
double computeEndMatchOverlapWithTarget(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList,int truth);

//Tree Node
struct treeNode* createTreeNode(
	struct truthAssignmentListStruct* truthList,
	struct listOfIntervalListsStruct** listOfintervalSets,
	struct listOfIntervalListsStruct** learnedIntervalSets,
	int targetInfluence,
	int splittingPredicate_id,
	double traceLength,
	struct treeNode* left,
	struct treeNode* right);

//Node List
struct nodeList* createNodeList(struct treeNode* node);
void addToNodeList(struct nodeList** root, struct nodeList* listItem);
struct nodeList* addNodeToList(struct nodeList* root, struct treeNode* listItem);
struct nodeList* getLastNodeInList(struct nodeList* root);


//Support Methods
double maxFloat(double val1, double val2);
double minFloat(double val1, double val2);
double lengthOfIntervalList(struct intervalListStruct* list);
int countLists(struct listOfIntervalListsStruct* list);
void computeFalseLists(struct listOfIntervalListsStruct* list, double traceLength);
struct listOfIntervalListsStruct* getListAtPosition(struct listOfIntervalListsStruct* intervalLists, int target);
struct listOfIntervalListsStruct** getListsAtPosition(struct listOfIntervalListsStruct** listOfIntervalSets, int target);

//Mean
double mean(double intervalListLength, double traceLength);
double computeMean(struct listOfIntervalListsStruct** localIntervalSets, int target, double traceLength, struct intervalListStruct** influenceLists);
double computeFalseMean(struct listOfIntervalListsStruct** localIntervalSets, int target, double traceLength, struct intervalListStruct** influenceLists);

//Entropy and Error
double error(double mean);
double computeEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList);
double computeTrueEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList);
double computeFalseEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList);
double computeOverlapEntropy(struct listOfIntervalListsStruct** target, struct intervalListStruct** endMatchList);
double computeBinaryEntropy(struct listOfIntervalListsStruct** localIntervalSet, struct  listOfIntervalListsStruct** learnedIntervalSets, struct truthAssignmentListStruct* constraintList, int predicate_id, int pos, int targetPORV_id);

//Gain
double computeGain(struct listOfIntervalListsStruct** localIntervalSet, struct  listOfIntervalListsStruct** learnedIntervalSets, int predicate_id, int target, double e, int pos, int *trueFalseFlag, struct truthAssignmentListStruct* constraintList,int targetPORV_id);
void computeAllGains(struct listOfIntervalListsStruct** localIntervalSet, struct listOfIntervalListsStruct** learnedIntervalSets, int target, int *trueFalseFlag, double* bestGain, int i, int j, int PORVCount, struct treeNode* currentNode, double* falseEntropy, double* trueEntropy,int targetPORV_id);
struct treeNode* findBestGain(struct listOfIntervalListsStruct** localIntervalSets, struct listOfIntervalListsStruct** learnedIntervalSets, struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int PORVCount, int numTargets, int N, struct treeNode* currentNode, int targetPORV_id);
struct listOfIntervalListsStruct* computeConstrainedIntervalSet(struct listOfIntervalListsStruct* list, int predicate_id, int truth, struct truthAssignmentListStruct* constraintList);

//Minkowski Methods
struct intervalStruct* minkowskiSum(struct intervalStruct* interval, double l, double r);
struct intervalStruct* minkowskiDiff(struct intervalStruct* interval, double l, double r);

struct intervalStruct* minkowskiIntervalSum(struct intervalStruct* interval1, struct intervalStruct* interval2);
struct intervalStruct* minkowskiIntervalDiff(struct intervalStruct* interval1, struct intervalStruct* interval2);

struct intervalListStruct* minkowskiSumList(struct intervalListStruct* list, double l, double r);
struct intervalListStruct* minkowskiDiffList(struct intervalListStruct* list, double l, double r);

int amsMine(struct treeNode* root, struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int numberOfPORVs, int numTargets, int N, int depth,int targetPORV_id, struct listOfIntervalListsStruct** learnedIntervalSets);

void printTreeNode(struct treeNode* node,int targetPORV_id);
void printTreeNodeToFile(struct treeNode* node,int depth,int targetPORV_id);
void printTreeNodeToFilePtr(struct treeNode* root,FILE* fp,int targetPORV_id);
void printTree(struct treeNode* node, int targetPORV_id);
void printTreeToFilePtr(struct treeNode* node, FILE *dTree, int targetPORV_id);

FILE* processInput(int argc, char* argv[], int* N, double* K, int* depth, int repeatFlag);
FILE* processConfig(int argc, char* argv[]);

int getTarget(int numberOfPORVs);
double getTraceLength();
void prepareBackwardInfluenceTraces(struct listOfIntervalListsStruct** localIntervalSets, int target, int N, double K, int strict);
void prepareBackwardInfluenceTraces2(struct listOfIntervalListsStruct** localIntervalSets, struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int N, double K, int strict,int numberOfPORVs,int numTargets);
void prepareRoot(struct treeNode* root, struct listOfIntervalListsStruct** intervalSet, struct listOfIntervalListsStruct** learnedIntervalSets, struct listOfIntervalListsStruct*** pseudoTargetLists, int target,int numTargets, int numberOfPORVs,int N);
void choicePause();

struct intervalListStruct* forwardInfluence(struct intervalListStruct* source, int i, double k, struct intervalListStruct* target);
struct intervalListStruct** computeForwardInfluenceForAnAssertion(struct treeNode* assertion,int targetID,double k);
struct intervalListStruct* computeForwardInfluenceForAnAssertionForTruth(struct treeNode* assertion,int targetID,double k,int truth);

int cfileexists(const char * filename);

//Support and Correlation
double getCorrelation(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int targetPORV_id);
double getCorrelationForTruth(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int truth, int targetPORV_id);

double getSupport(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int targetPORV_id);

int testCorrelationSupport(struct truthAssignmentListStruct* truthList, struct intervalListStruct* targetList, int targetPORV_id);	//Returns 0 or 1 depending on whether the prefix has support and correlation quantities above thresholds.
int testCorrelationSupportForTruth(struct truthAssignmentListStruct* truthList, int truth, int targetPORV_id);

//Pretty Printers for Assertions
struct intervalListStruct** computeBuckets(struct treeNode* node);
struct intervalStruct* computeTightSeparation(struct intervalListStruct* list1,struct intervalListStruct* list2, int pos);
struct intervalStruct* computeSeparationAnyMatch(struct intervalListStruct** bucket,int bucketPos,struct intervalListStruct** targetList, int targetPos, int smallestBucketID);
struct intervalStruct* computeBucketSeparation(struct intervalListStruct** bucket,struct intervalListStruct** list2, int pos1, int pos2, int smallestBucketID);
struct intervalStruct* widenIntervalSet(struct intervalListStruct* list);
//void printAssertionWithTruthToFile(FILE* fp, struct intervalListStruct* targetList, struct intervalListStruct* bucket[], int bucketCount, int truth, struct treeNode* node);
void printAssertionWithTruthToFile(struct listOfIntervalListsStruct** learnedIntervalSets, FILE* fp, struct intervalListStruct** targetList, struct intervalListStruct** bucket, int bucketCount, int truth, struct treeNode* node,int position, int targetPORV_id);
void writeAssertionWithTruthToStruct(struct assertionStruct* assertion, struct listOfIntervalListsStruct** learnedIntervalSets, struct intervalListStruct** targetList, struct intervalListStruct** bucket, int bucketCount, int truth, struct treeNode* node,int position, int targetPORV_id);
void printAssertions(struct listOfIntervalListsStruct** learnedIntervalSets, struct treeNode* node, FILE* fp, int targetPORV_id);

void printFloatArrayToFile(double* array, int size, FILE* fp);
void printFloat2DArraryToFile(int size1, int size2, double array[][size2], FILE* fp);
void copyMatrix(double ** target, int m, int n, double source[][n]);

struct listOfIntervalListsStruct* getAllIntervalsFromFile(FILE* fp);
//struct indexCouple* findError(struct treeNode* node);

double getMaxIntervalList(struct intervalListStruct* list);
double getMaxIntervalSet(struct listOfIntervalListsStruct* set);

int getSmallestBucketID(struct truthAssignmentListStruct* truthtList);

FILE* getDTreeFilePtr();
FILE* getDTreeFilePtrWithName(char* name);
struct intervalListStruct* endMatchForPrefix(struct truthAssignmentListStruct* constraintList);

//Predicate Detail
struct predicateDetail* createPredicateDetail(struct PORV* predicate, double gain, int bucket, struct intervalListStruct** trueLists);
struct predicateDetail* addPredicateDetailToList(struct predicateDetail* root, struct predicateDetail* detail);
void printPredicateDetailToFilePtr(struct predicateDetail* detail,int traceCount,FILE* fp);
void printPredicateDetailListToFilePtr(struct predicateDetail* list,int traceCount,FILE* fp);
void deletePredicateDetails(struct predicateDetail* node);
int prepareToLearn(struct predicateDetail* details, struct treeNode* currentNode, struct config* configuration, struct identifier* varList, int bucketCount, int traceCount, int targetPORV_id, struct intervalListStruct*** buckets,struct listOfIntervalListsStruct** learnedIntervalSets);
void updateLearnedIntervalSet(struct predicateDetail* detail, int traceCount, struct listOfIntervalListsStruct** learnedIntervalSets);
struct predicateDetail* duplicatePredicateDetail(struct predicateDetail* root);
void printRequestForNewPredicate(struct config* configuration, struct identifier* varList,  struct intervalListStruct** buckets, int bucketCount, struct intervalListStruct* targetTrueList);
void printLearnedIntervalSet(int traceCount, FILE* fp, struct  listOfIntervalListsStruct** learnedIntervalSets);

void learnNewPredicates();
char* convertToDatFileName(char* temp);

double lengthOfIntervalLists(struct intervalListStruct **intervalLists);
struct intervalListStruct** endMatchesForPrefix(struct truthAssignmentListStruct* prefix, struct listOfIntervalListsStruct** learnedIntervalSets);
struct intervalListStruct** createValidLists();
struct intervalListStruct** getBucketsForTrace(struct listOfIntervalListsStruct** learnedIntervalSets, struct truthAssignmentListStruct* truthList,int bucketCount, int traceID);
//edited- added methods
_Bool is_stop(struct treeNode* root, int target,struct listOfIntervalListsStruct** listOfIntervalSets, struct listOfIntervalListsStruct** learnedIntervalSets);
_Bool is_leaf(struct treeNode *root,int depth);
void updateLeaf(struct treeNode* root,struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int numberOfPORVs,int numTargets, int N, struct listOfIntervalListsStruct** learnedIntervalSets);
int amsMine2(struct treeNode* original, struct treeNode* duproot,struct listOfIntervalListsStruct*** pseudoTargetLists, int target, int numberOfPORVs,int numTargets, int N, int origDepth, int dupDepth,int isroot,struct listOfIntervalListsStruct** listOfIntervalSets, struct listOfIntervalListsStruct** learnedIntervalSets);


int checkCreateLogDir();
int ignorePredicate(int predicateID);
double newPrecision(double n, double i);

//Methods from Code-Refactor
FILE* getAssertionFile();
void processAssertionNode(struct listOfIntervalListsStruct** learnedIntervalSets, struct treeNode* node, int targetPORV_id);
void printAssertionsToFile(struct listOfIntervalListsStruct** learnedIntervalSets, struct treeNode* node, int targetPORV_id);
_Bool is_assertion_node(struct treeNode* root);
_Bool stop_learn(int depth,struct treeNode* root);
struct truthAssignmentListStruct* createTruthListFalse(struct truthAssignmentListStruct* src, int predicateID, int bucket, int predicateType);
struct truthAssignmentListStruct* createTruthListTrue(struct truthAssignmentListStruct* src, int predicateID, int bucket, int predicateType);
void prepareTreeNode(struct treeNode** root,struct truthAssignmentListStruct* constraintList, struct listOfIntervalListsStruct **listOfIntervalSets,  struct listOfIntervalListsStruct **learnedIntervalSets, double traceLength, int targetPredicateID, struct intervalListStruct **endMatchIntervalList, struct indexCouple *exploredList);
#endif
