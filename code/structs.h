/*****************************************************************
 *                      CODE WRITTEN BY                          *
 *                                                               *
 *             Antonio Anastasio Bruto da Costa                  *
 *     Formal Method Laboratory, Dept. of CSE, IIT Kharagpur     *
 *                                                               *
 *****************************************************************/

//#define DEBUG_ON

#ifndef MAX_STR_LENGTH
	#define MAX_STR_LENGTH 10240
#endif


#ifndef STRUCTS_H

#define STRUCTS_H
struct condition{	//Condition Structure : used to represent assignments, invariants, derivatives, and resets.
	char LHS[MAX_STR_LENGTH];	//Left-hand side of operator
	char RHS[MAX_STR_LENGTH];	//Right-hand side of operator
	int op;		//Operator
			// 0 : ==
			// 1 : <=
			// 2 : := or =
			// 3 : >=
			// 4 : <
			// 5 : >
	struct condition* next; //Next in the Linked List
};


struct identifier{		//Identifier Structure : Linked List used to store any generic list of identifiers
	char name[MAX_STR_LENGTH];		//Identifier Name
	int col;						//CSV Column - Offset of 1
	int timeCol;					//Time CSV Column
	struct identifier* next;//Pointer to the next Identifier in the list or Null(If this is the last Identifier)
};

struct PORV{				//List of PORV's conjuncted together
	struct condition* porv;
	int id;
	int atomID;
	struct PORV* next;
};

struct eventType{
	int type;			//Event Type - posedge(1), negedge(2), either(0)
	struct PORV* porv;		//Event Condition
};

struct interval{
	double left;
	double right;
	int id;
};

struct term{
	int PORVID;
	struct term* next;
};

struct expression{
	struct term* disjunct;
	struct expression* next;
};

struct expressionList{
	char* id;
	struct expression* expr;
	struct expressionList* next;
};

struct file{
        int start;
        struct identifier* varList;
        struct PORV* porvList;
		struct identifier* predicates;
        struct expressionList* exprList;
};

struct config{
	int N;
	double K;
	int traceCount;
	char traceFileName[MAX_STR_LENGTH];
	struct identifier* traceFileNames;
	char intervalSetFileName[MAX_STR_LENGTH];
	int bestGainCount;
	double tmin;
	double tmax;
	int maxTreeDepth;
	double traceLength;
	int learnType;
};

/*---------------------DECLARATIONS-----------------------*/
char* operatorMap(int op);


//Config
struct config* createConfig();
void printConfig(struct config* inputConfig);
void printConfigToFilePtr(struct config* inputConfig, FILE* fp);
struct config* redirectConfig();
int getDepth(struct config* inputConfig);
void setTraceLengthForConfig(struct config* configuration, double length);
int getLearnType(struct config* inputConfig);

//Conditions
struct condition* createCondition(char *LHS, char* RHS, int op);
struct condition* addConditionToList(struct condition* root, struct condition* cond);
struct condition* addToConditionList(struct condition* root, char *LHS, char* RHS, int op);
struct condition* duplicateConditionList(struct condition* root);

void printCondition(struct condition* cond);
void printConditionList(struct condition* cond);
void printConditionToFilePtr(struct condition* cond,FILE* fp);
void printConditionToString(struct condition* cond, char* fp);
void printConditionListToFilePtr(struct condition* cond, FILE* fp);

struct condition* reverseConditionList(struct condition* root);
int conditionCompare(struct condition* cond, char* LHS, char* RHS);
int inConditionList(struct condition* list, char* LHS, char* RHS);
int removeConditionFromList(struct condition** root, char* LHS);
void deleteConditionList(struct condition* node);

//Identifiers
struct identifier* createIdentifier(char *id);
struct identifier* addIdentifierToList(struct identifier* root, struct identifier* ID);
struct identifier* addToIdentifierList(struct identifier* root, char *id);
int getIdentifierID(struct identifier* list, char *name);
struct identifier* duplicateIdentifierList(struct identifier* root);
void freeIdentifierList(struct identifier* id);
char* containsIdentifier(struct identifier* list1, struct identifier* list2);
void printIdentifier(struct identifier* id);	
void printIdentifierList(struct identifier* id);
struct identifier* getIdentifierWithID(struct identifier* list, int id);
int getIdentifierCol(struct identifier* list, int id);
int getIdentifierTimeCol(struct identifier* list, int id);
struct identifier* getNextIdentifier(struct identifier* temp);

//Events
struct eventType* createEvent(int type, struct PORV* porv);

//PORVs
struct PORV* createPORV(struct condition* porv, int id, int atomID);
struct PORV* addPORVToList(struct PORV* root, struct PORV* porv);
struct PORV* duplicatePORVList(struct PORV* root);
int getPORVCount(struct PORV* root);
struct PORV* getPORV(struct PORV* root, int id);
void deletePORVs(struct PORV* node);

//Print Methods
void printFeatureCondition(struct condition* cond);
void printAssignmentList(struct condition* assignments);
void printPORV(struct PORV* porv);
void printPORVToFilePtr(struct PORV* porv,FILE* fp);
void printPORVConjunct(struct PORV* conjunct);
void printEvent(struct eventType* event);
void printPredicates(struct identifier* predicates);
//void printPredicateMap(struct file* predMap);
void printPredicateList(struct file* predMap);
void printVariableListToFilePtr(struct identifier* list, FILE* fp);

//Intervals
struct interval* createInterval();
struct interval* createIntervalInit(double left, double right, int id);

//File
struct file* createFile();

//Terms - Disjuncts
struct term* createTerm(int id);
struct term* addPORVToTerm(struct term* root, int id);
void printTerm(struct term* disjunct, struct identifier* predicates);

//Expressions
struct expression* createExpression(struct term* disjunct);
struct expression* addTermToExpression(struct expression* root, struct term* disjunct);
void printExpression(struct expression* expr,struct identifier* predicates);

//Expression List
struct expressionList* createExpressionList(struct expression* expr);
struct expressionList* addToExpressionList(struct expressionList* root, struct expression* expr);
struct expressionList* addExpressionListToExpressionList(struct expressionList* root, struct expressionList* list);
struct expressionList* getExpressionAtPosition(struct expressionList *root, int position);

//Config Support Methods
struct identifier* duplicateTraceFileNames(struct config* inputConfig);


//Support Methods
int charInList(char *c, char *list, int count);
char* strToUpper(char* str);
char* trim(char *str);

int fileCopy(const char* source, const char* dest);

void preparePy(struct file* predicateMap,char *csvFile);
void printImports(FILE* fp);
void printCSV(FILE* fp,struct file* predicateMap,char* csvFile);

void booleanize();
#endif
