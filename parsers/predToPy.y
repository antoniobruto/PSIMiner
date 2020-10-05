%{
	//#define YACC_DEBUG_ON
	
	#ifndef MAX_STR_LENGTH
			#define MAX_STR_LENGTH 10240
	#endif
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <float.h>
	#include "structs.h"
	
	//Function Declarations
	void yyerror(char *);
	
	//External Objects
	extern int yylex();
	extern int predToPy_lineNo,predToPy_charPos,yyleng,yylineno,yychar,predToPy_oldPos;
	extern char line[MAX_STR_LENGTH],oldLine[MAX_STR_LENGTH];
	extern FILE *yyin;
	extern char* yytext;
	extern int predToPy_multiLine;
	extern struct file* predicateMap;
	extern struct identifier* idList;
	extern struct config* inputConfig;
	extern struct identifier* traceFileNames;
	
	//Local Objects
	char* traceFileName = NULL;
	int errNo,err1No,err2No,err3No,err4No,err5No,err6No;
	int currentMatch;
	int porvID = 1;
	char EF_dummy[1] = {'\0'};
	char predicateLine[MAX_STR_LENGTH];
	int traceCount = 0;
	
	//Error Types : Will be eventually useful when the parsers show detailed, helpful parse error messages
	enum errorType {
		//BLOCK_NO_NEWLINE = 1,
		IF_NO_TEST = 2,
		IF_NO_COLON = 3,
		TEST_EQ_EXPR = 4,
		WHILE_NO_COLON = 5,
		WHILE_NO_TEST = 6,
		PRINT_INVALID = 7,
		TEST_NO_LOGICAL = 8,
		EXPR_INVALID = 9
	};
%}

//Lexical Tokens
%token <string> TRACEFILE TRACECOUNT SEQLENGTH DELAYRES DEPTH BESTCOUNT TMAX TMIN START PBEGIN PEND OPENROUND CLOSEROUND ATPOSEDGE ATNEGEDGE ATANYEDGE RATIONAL ARITHOP EQ LEQ GEQ LT GT SEMICOLON DOLLARTIME ATOM BAND BOR EEQ PREDLEARN COMMA STRICT OBJECTIVE USEOVERLAP;

//Start Production Rule
%start configSpec

//Data Types Used for Tokens and Reduction Rules
%union {
	char string[MAX_STR_LENGTH];
	struct identifier* id;
	struct PORV* porvType;
	//struct eventType* event;
	struct expressionList* exprList;
	struct expression* expr;
	struct term* disjnct;
	struct config* inputs;
	int code;
}

%type <string> rational arithExpr arithStatement traceFile traceFileList sequenceLength delayResolution bestGainCount tempMax tempMin treeDepth learningType traceCT strictness objectiveFunction useOverlap;
%type <id> variableMapList variableMap;
//%type <event> eventExpr;
%type <porvType> porv predicateList;
%type <code> ineq  startExpr; //eventType
%type <exprList> expressList namedExpressionLine;
%type <expr> expressionLine;
%type <disjnct> disjunct;
%type <inputs> inputList;
%%

configSpec: 
		inputList predicateSpec 	{ 
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: END\n");
							
							struct identifier* id = inputConfig->traceFileNames;
							
							while(id){
								printf("NAME = [%s]\n",id->name);
								id = id->next;
							}
							#endif
							traceFileNames = inputConfig->traceFileNames;
						}
						
	;

inputList:
		inputList traceCT {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							if(traceCount>0){
								printf("Trace Count was defined multiple times, considering the latest\n");
							}
							inputConfig->traceCount = atoi($2);
							traceCount = atoi($2);
						}
	|	inputList traceFile		{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceFile statement\n");
							#endif
							if(!inputConfig){
								printf("ERROR[Creating Trace File List]: Config is NULL\n");
							} 
							
							//bzero(inputConfig->traceFileName,sizeof(char)*MAX_STR_LENGTH);
							//strcpy(inputConfig->traceFileName,$2);
							//traceFileName = inputConfig->traceFileName;
						}
	|	inputList sequenceLength  	{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-sequenceLength statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->N = atoi($2);
						}
	|	inputList delayResolution 	{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-delayResolution statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->K = atof($2);
						}
	|	inputList bestGainCount  	{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-bestGainCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->bestGainCount = atoi($2);
						}
	|	inputList tempMax 		{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-tempMax statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->tmax = atof($2);
							
						}
	|	inputList tempMin 		{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-tempMin statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->tmin = atof($2);
							
						}						
	|	inputList treeDepth 		{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-treeDepth statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->maxTreeDepth = atoi($2);
							
						}
	|	inputList learningType		{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-learningType statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->learnType = atoi($2);
						
						}
	|	inputList strictness		{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-learningType statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->strict = atoi($2);
						}
	|	inputList objectiveFunction		{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-learningType statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->objective = atoi($2);
						}
	|	inputList useOverlap		{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-learningType statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->useOverlap = atoi($2);
						}
	|	traceFile			{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-traceFile statement\n");
							#endif
							if(!inputConfig){
								printf("ERROR[Creating Trace File List]: Config is NULL\n");
							} 
							
							//bzero(inputConfig->traceFileName,sizeof(char)*MAX_STR_LENGTH);
							//strcpy(inputConfig->traceFileName,$1);
							//traceFileName = inputConfig->traceFileName;							
							
						}
	|	sequenceLength  		{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-sequenceLength statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->N = atoi($1);
						}
	|	delayResolution 		{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-delayResolution statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->K = atof($1);
							
						}
	|	bestGainCount  			{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-bestGainCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->bestGainCount = atoi($1);
						}
	|	tempMax 			{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-tempMax statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->tmax = atof($1);
						}
	|	tempMin 			{	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-tempMin statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->tmin = atof($1);
						}
	|	treeDepth 			{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-treeDepth statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->maxTreeDepth = atoi($1);
							
						}
	|	learningType			{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-learningType statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->learnType = atoi($1);
						
						}
	| 	traceCT			{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							if(traceCount>0){
								printf("Trace Count was defined multiple times, considering the latest\n");
							}
							inputConfig->traceCount = atoi($1);
							traceCount = atoi($1);;
						
						}
	| 	strictness		{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->strict = atoi($1);
						}
	| 	objectiveFunction		{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->objective = atoi($1);
						}
	| 	useOverlap		{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->useOverlap = atoi($1);
						}
	;

learningType: PREDLEARN EEQ rational	{
					        #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched traceFile statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                strcpy($$,$3); 
					};
					

traceCT:	TRACECOUNT EEQ rational	{
												#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched traceCount statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                strcpy($$,$3); 
					};

strictness:	STRICT EEQ rational	{
												#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched strictness statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                strcpy($$,$3); 
					};

objectiveFunction:	OBJECTIVE EEQ rational	{
												#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched strictness statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                strcpy($$,$3); 
					};
					
useOverlap:	USEOVERLAP EEQ rational	{
												#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched strictness statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                strcpy($$,$3); 
					};
					
traceFile: TRACEFILE EEQ traceFileList	{
						strcpy($$,$3);
					};
					
traceFileList: traceFileList COMMA ATOM 
					{
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched traceFileList statement\n");
						#endif

						if(!inputConfig || inputConfig->traceCount<=0){
							printf("\nError in the configuration. A positive trace count is required\n");
							exit(0);
						}
						
						if(!inputConfig){
							inputConfig = createConfig();
						}
						
						if(traceCount == 0){
							printf("\nError, you have more traces than the count you have specified\n");
							exit(0);
						}
						
						if(inputConfig->traceFileNames == NULL){
							inputConfig->traceFileNames = createIdentifier($3);
						} else {
							inputConfig->traceFileNames = addToIdentifierList(inputConfig->traceFileNames,$3);
						}
						traceCount--;
						strcpy($$,$3);
					}
		| ATOM 	{
						#ifdef YACC_DEBUG_ON 
							printf("PARSER: Matched traceFile statement\n");
						#endif
						
						if(!inputConfig || inputConfig->traceCount<=0){
						printf("\nError in the configuration. A positive trace count is required\n");
						exit(0);
						}
						
						if(!inputConfig){
							inputConfig = createConfig();
						}
						
						if(traceCount == 0){
							printf("\nError, you have more traces than the count you have specified\n");
							exit(0);
						}
						
						if(inputConfig->traceFileNames == NULL){
							inputConfig->traceFileNames = createIdentifier($1);
						} else {
							inputConfig->traceFileNames = addToIdentifierList(inputConfig->traceFileNames,$1);
						}
						traceCount--;
						//printf("[%s]\n",$2);
						strcpy($$,$1);
					};
	
sequenceLength: SEQLENGTH EEQ rational {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched sequenceLength statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy($$,$3); 
					};
	
delayResolution: DELAYRES EEQ rational {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched delayResolution statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy($$,$3); 
					};
	
treeDepth: DEPTH EEQ rational 		{
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched treeDepth statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy($$,$3); 
					};

bestGainCount: BESTCOUNT EEQ rational 	{
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched bestGainCount statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy($$,$3); 
					};
	
tempMax:  TMAX EEQ rational 		{
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched tempMax statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy($$,$3); 
					};
	
tempMin: TMIN EEQ rational 		{
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched tempMin statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy($$,$3); 
					};
	
predicateSpec:
        startExpr variableMapList { idList = $2;} PBEGIN predicateList endPList expressList {      
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Matched File\n");
                                                                #endif
                                                                if(!predicateMap)
																	predicateMap = createFile();
                                                                predicateMap->start = $1;
                                                                predicateMap->varList = $2;
                                                                predicateMap->porvList = $5;
                                                                predicateMap->exprList = $7;
                                                                //printPredicateMap(predicateMap);
                                                                struct identifier* id = inputConfig->traceFileNames;
                                                                while(id){
																	preparePy(predicateMap,id->name);
																	booleanize();
																	id = id->next;
                                                                }
                                                                //preparePy(predicateMap,traceFileName);
                                                                //booleanize();
                                                                //printf("\nInterval Sets are In: \"%s.dat\"\n",traceFileName);
                                                            }
        |variableMapList { idList = $1;} PBEGIN predicateList endPList expressList {      
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Matched File\n");
                                                                #endif
                                                                if(!predicateMap)
																	predicateMap = createFile();
                                                                predicateMap->start = 0;
                                                                predicateMap->varList = $1;
                                                                predicateMap->porvList = $4;
                                                                predicateMap->exprList = $6;
                                                                //printPredicateMap(predicateMap);                                                                
                                                                struct identifier* id = inputConfig->traceFileNames;
                                                                while(id){
																	preparePy(predicateMap,id->name);
																	booleanize();
																	id = id->next;
                                                                }
                                                                //preparePy(predicateMap,traceFileName);
                                                                //booleanize();
                                                                //printf("\nInterval Sets are In: \"%s.dat\"\n",traceFileName);
                                                            }
        ;

startExpr: START rational               {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched start statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                $$ = atoi($2);
                                        }
        ;

variableMapList: variableMapList variableMap {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched variableMapList\n");
                                                #endif
                                                $$=addIdentifierToList($1,$2);
                                        }

        |variableMap                    {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched variableMapList\n");
                                                #endif
                                                $$=$1;
                                        }
        ;
variableMap:
        ATOM rational rational          {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched variableMap\n");
                                                #endif
                                                $$=createIdentifier($1);
                                                $$->col = atoi($2);
                                                $$->timeCol = atoi($3);
                                        }
        ;
        
rational: RATIONAL                     {       
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Linear Expr is a Rational [%s]\n",$1);
                                                #endif
                                                
                                                //$$=createCondition($1,EF_dummy,-1);
                                                strcpy($$,$1);
                                        }
        | ARITHOP RATIONAL
                                        {       
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Linear Expr is a Rational[%s%s]\n",$1,$2);
                                                #endif
                                                
                                                //$$=createCondition($1,EF_dummy,-1);
                                                sprintf($$,"%s%s",$1,$2);
                                                //strcpy($$,$1);
                                        }
        ;
        
predicateList: predicateList porv       {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched predicateList\n");
                                                #endif                                                                                                
                                                $$=addPORVToList($1,$2);
                                        }
       | porv                           {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched predicateList\n");
                                                #endif
                                                $$=$1;
                                        }
      ;

porv:   ATOM ineq arithExpr             {
											#ifdef YACC_DEBUG_ON 
													printf("PARSER: porv: ATOM ineq arithExpr\n");
											#endif
											if(idList==NULL){
													printf("ERROR: No variables declared\n");
													exit(0);
											}
											int atomID = getIdentifierID(idList,$1);
											if(atomID==0){
													printf("ERROR: No variable with name [%s] declared\n",$1);
													exit(0);
											}
											char temp[MAX_STR_LENGTH];
											int col = getIdentifierCol(idList,atomID);
											if(col==-1){
													printf("ERROR: col val is -1: atomID value mismatch\n");
													exit(0);
											}
											sprintf(temp,"float(row[%d])",col-1);                                                        
											$$ = createPORV(createCondition(temp,$3,$2),porvID++,atomID); 
											if(!predicateMap)
												predicateMap = createFile();

											strcpy(predicateLine,oldLine);
											if(strlen(trim(predicateLine))>0){
												//printf("PORV ----- [%s]\n",predicateLine);
												struct identifier* tempID = createIdentifier(predicateLine);
												tempID->col = porvID-1;
												tempID->timeCol = 0;
												predicateMap->predicates = addIdentifierToList(predicateMap->predicates,tempID);
												tempID = NULL;
											}
										}
        ;

/*
eventExpr:
        eventType OPENROUND porv CLOSEROUND   {
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: eventExpr: eventType FOPENROUND porv FCLOSEROUND\n");
                                                        #endif

                                                        $$=createEvent($1,$3);
                                                        //$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        ;
*/

ineq:
        LT                                     {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FLT [  %s  ]\n",$1);
                                                        #endif
                                                       // yyerror("A '<' inequality is not allowed\n");
                                                        $$ =4;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        |GT                                    {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FGT [  %s  ]\n",$1);
                                                        #endif
                                                        //yyerror("A '>' inequality is not allowed\n");
                                                        $$ = 5;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        |LEQ                                   {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FLEQ [  %s  ]\n",$1);
                                                        #endif
                                                        $$ = 1;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        |GEQ                                   {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FGEQ [  %s  ]\n",$1);
                                                        #endif
                                                        $$ = 3;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        |EQ                                    {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FEQ [  %s  ]\n",$1);
                                                        #endif
                                                        $$ = 0;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        ;

/*
eventType:
        ATPOSEDGE                              {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: eventType: FATPOSEDGE [  %s  ]\n",$1);
                                                        #endif
                                                        $$ = 1;
                                                        //$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        |ATNEGEDGE                             {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: eventType: FATNEGEDGE [  %s  ]\n",$1);
                                                        #endif
                                                        $$ = -1;
                                                        //$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        |ATANYEDGE                             {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: eventType: FATANYEDGE [  %s  ]\n",$1);
                                                        #endif

                                                        $$ = 0;
                                                        //$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        ;
*/

arithExpr:
        arithStatement ARITHOP arithExpr		{
													#ifdef YACC_DEBUG_ON 
															printf("PARSER: Complex Arith Expr\n"); 
													#endif
													sprintf($$,"%s %s %s",$1,$2,$3);
												}
        | arithStatement						{
													#ifdef YACC_DEBUG_ON 
															printf("PARSER: Simple Arith Statement\n");
													#endif
													strcpy($$,$1);
												}
        ;

arithStatement:
		ATOM									{
													#ifdef YACC_DEBUG_ON 
															printf("PARSER: Linear Expr is an ID\n");
													#endif
													int atomID = getIdentifierID(idList,$1);
													if(atomID==0){
															printf("ERROR: No variable with name [%s] declared\n",$1);
															exit(0);
													}
													int col = getIdentifierCol(idList,atomID);
													if(col==-1){
															printf("ERROR: col val is -1: atomID value mismatch\n");
															exit(0);
													}
													sprintf($$,"float(row[%d])",col-1);
													//strcpy($$,$1);                                                               
												}
        | ARITHOP ATOM                                {
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is an ID\n");
                                                                //strcpy($2,$1);//strcpy($$,$1);
                                                                #endif
                                                                int atomID = getIdentifierID(idList,$2);
                                                                if(atomID==0){
                                                                        printf("ERROR: No variable with name [%s] declared\n",$2);
                                                                        exit(0);
                                                                }
                                                                int col = getIdentifierCol(idList,atomID);
                                                                if(col==-1){
                                                                        printf("ERROR: col val is -1: atomID value mismatch\n");
                                                                        exit(0);
                                                                }
                                                                sprintf($$,"%sfloat(row[%d])",$1,col-1);
                                                        }
        | rational                                      {       
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is a Rational\n");
                                                                #endif
                                                                
                                                                strcpy($$,$1);
                                                        }
        | DOLLARTIME                                   	{       
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is $TIME\n");
                                                                #endif
                                                                
                                                                strcpy($$,$1);
                                                        }
        ;

expressList: expressList expressionLine        		{
								$$ = addToExpressionList($1,$2);
							}
	| expressionLine				{
								$$ = createExpressionList($1);	
							}
	| expressList namedExpressionLine        	{
								$$ = addExpressionListToExpressionList($1,$2);
							}
	| namedExpressionLine				{
								$$ = $1;
							}
	;
        
namedExpressionLine: ATOM EEQ expressionLine		{
								$$ = createExpressionList($3);
								$$->id = (char*)malloc(sizeof(char)*MAX_STR_LENGTH);
								strcpy($$->id,$1);
							}
	;
							
expressionLine: expressionLine BAND OPENROUND disjunct CLOSEROUND
							{
								$$ = addTermToExpression($1,$4);
                                                        }
		| OPENROUND disjunct CLOSEROUND		{
								$$ = createExpression($2);
							}
		;

disjunct: disjunct BOR rational				{
								$$ = addPORVToTerm($1,atoi($3));
							}
	| rational					{	
								$$ = createTerm(atoi($1));	
							}
	;
	
endPList: PEND				{	
								if(predToPy_multiLine){
									if(!predicateMap)
										predicateMap = createFile();

									strcpy(predicateLine,oldLine);
									if(strlen(trim(predicateLine))>0){
										printf("PORV ----- [%s]\n",predicateLine);
										struct identifier* tempID = createIdentifier(predicateLine);
										tempID->col = porvID-1;
										tempID->timeCol = 0;
										predicateMap->predicates = addIdentifierToList(predicateMap->predicates,tempID);
										tempID = NULL;                   
									}
								}
							}
							;
%%


void yyerror(char *s) {

        char errorMsg[MAX_STR_LENGTH];
        int flag = 0;
        switch(errNo){
                /*case BLOCK_NO_NEWLINE:        sprintf(errorMsg,"New statements must fall on a new line.");
                                        currentMatch = predToPy_charPos;
                                        break;*/
                case IF_NO_TEST:        sprintf(errorMsg,"Expected a logical condition expression AFTER \"if\".");
                                        break;
                case IF_NO_COLON:       sprintf(errorMsg,"Expected a \":\" after the condition expression.");
                                        flag = 1;
                                        break;
                case TEST_EQ_EXPR:      sprintf(errorMsg,"The condition expression can't contain an assignment operator. Use Logical Operators only.");
                                        break;
                case WHILE_NO_COLON:    sprintf(errorMsg,"Expected a \":\" after the condition expression.");
                                        flag = 1;
                                        break;
                case WHILE_NO_TEST:     sprintf(errorMsg,"Expected a logical condition expression AFTER \"while\".");
                                        break;
                case PRINT_INVALID :    sprintf(errorMsg,"Invalid print statement syntax.");
                                        break;
                case TEST_NO_LOGICAL:   sprintf(errorMsg,"Expected a logical operator between the two expressions.");
                                        flag = 1;
                                        break;
                case EXPR_INVALID:      sprintf(errorMsg,"Invalid Expression Format.");
                                        break;
                default:                sprintf(errorMsg,"Hey Didn't expect to see this here.");
                                        flag = 1;
                                        break;
        }
        if(strlen(line)==0){
                strcpy(line,oldLine);
                predToPy_charPos = predToPy_oldPos;
                predToPy_lineNo--;
        }
        if(flag==1){
                currentMatch = predToPy_charPos+1;
        }
        //fprintf(stderr, "%s at Line %d ; %s Match [%d], line :[%s],  ERROR CODES: [%d,%d,%d,%d,%d,%d,%d]\n", s,predToPy_lineNo,errorMsg,currentMatch,line,errNo,err1No,err2No,err3No,err4No,err5No,err6No);
        fprintf(stderr, "[ConfigParser] %s at Line %d : %s \n", s,predToPy_lineNo,errorMsg);
        fprintf(stderr, "%s\n",line);
        int i=0;
        for(i=1;i<currentMatch;i++){
                printf(" ");
        }
        printf("^\n");
        exit(0);
        
        
}

/*
int main(int argc, char *argv[]) {
        if(argc<3){
                
                printf("\nINCORRECT SYNTAX\n");
                
                printf("\nExpected Syntax: Booleanize <Config File Name> <CSV File Name>\n\n");
                
                exit(0);
        }
                
        yyin = fopen(argv[1], "r");
        traceFileName = argv[2];
        setbuf(stdout, NULL);
        yyparse();
}
*/
