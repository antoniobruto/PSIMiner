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
        #include "psiMinerStructs.h"
        
        //Function Declarations
        void yyerror(char *);
        
        //External Objects
        extern int yylex();
        extern int learnedOP_lineNo,learnedOP_charPos,yyleng,yylineno,yychar,learnedOP_oldPos;
        extern char learnedOP_line[MAX_STR_LENGTH],oldLine[MAX_STR_LENGTH];
        extern FILE *yyin;
        extern char* yytext;
		extern int learnedOP_multiLine;
		extern int traceCount;
        //Global Objects
        extern struct predicateDetail* details;
        
        int errNo,err1No,err2No,err3No,err4No,err5No,err6No;
        int currentMatch;
        char predicateLine[MAX_STR_LENGTH];
        
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
        
        struct intervalListStruct** listOfTraceIntervalLists = NULL;
%}

//Lexical Tokens
%token <string> GAIN INTERVALS_BEGIN INTERVALS_END BUCKET BEGINPRED ENDPRED ATOM EEQ LIST_BEGIN LIST_END SEPARATOR RATIONAL OPENROUND CLOSEROUND ARITHOP EQ LEQ GEQ LT GT INTERVAL_BEGIN INTERVAL_END;

//Start Production Rule
%start detailList

//Data Types Used for Tokens and Reduction Rules
%union {
	char string[MAX_STR_LENGTH];
	double decimal;
	struct PORV* porvType;
	struct predicateDetail* predDetail;
	struct intervalStruct* intval;
	struct intervalListStruct* intervalList;
	struct intervalListStruct** traceIntervalSet;
	int code;
}

%type <string> rational arithExpr arithStatement;
%type <decimal> gainExpr;
%type <porvType> porv;
%type <predDetail> detailList detail;
%type <intval> interval;
%type <intervalList> intervalContent intervalList;
%type <traceIntervalSet> traceIntervalLists traceIntervalList intervalSet;
%type <code> ineq  bucketExpr;

%%

detailList:	%empty		{	
							#ifdef YACC_DEBUG_ON 
								fprintf(stdout,"\nNo details found\n");
							#endif
							
							details = NULL;
						}
		|detailList detail 		{ 
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: END\n");
							#endif
							
						}
		/*| detail 		{ 
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: END\n");
							#endif
							
						}*/
		
	;

detail:
		BEGINPRED porv gainExpr bucketExpr intervalSet ENDPRED
						{
							#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Predicate Deatail\n");
							#endif
							
							struct predicateDetail* x = createPredicateDetail($2,$3,$4,$5);
							details = addPredicateDetailToList(details,x);
							
						};
        
gainExpr:	GAIN EEQ rational			{
							#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Gain Rational [%s]\n",$3);
							#endif
							
							$$ = atof($3);
						};
						
bucketExpr:	BUCKET EEQ rational			{
							#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Bucket Rational [%s]\n",$3);
							#endif
							
							$$ = atoi($3);
						};

intervalSet: INTERVALS_BEGIN traceIntervalLists INTERVALS_END {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Interval Set\n");
							#endif
							$$ = listOfTraceIntervalLists;
							listOfTraceIntervalLists = NULL;
						};

traceIntervalLists: traceIntervalList traceIntervalLists {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Trace Interval Lists\n");
							#endif
							
						} 
				| traceIntervalList 
						{
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Trace Interval Lists\n");
							#endif
						};

traceIntervalList: rational intervalList {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Trace Interval List\n");
							#endif
							if(listOfTraceIntervalLists==NULL){
								listOfTraceIntervalLists = (struct intervalListStruct**)malloc(sizeof(struct intervalListStruct*)*traceCount);
								
								bzero(listOfTraceIntervalLists,sizeof(struct intervalListStruct*)*traceCount);
								
								#ifdef YACC_DEBUG_ON 
									printf("PARSER: Created TraceIntervalList Array of Trace Interval Lists\n");
								#endif
								
							}
							int traceID = atoi($1);
							if(traceID>=traceCount){
								printf("Error: The traceID exceeds the number of traced in the configuration.\n");
								exit(0);
							} else {
								#ifdef YACC_DEBUG_ON 
									printf("PARSER: Adding to list of interval lists\n");
								#endif
								listOfTraceIntervalLists[traceID] = $2;
							}
							
							
						};

rational: RATIONAL					 	{
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
                                        };

porv:   ATOM ineq arithExpr			{
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: porv: ATOM ineq arithExpr\n");
                                                        #endif

                                                        $$ = createPORV(createCondition($1,$3,$2),0,0);
						}
        ;

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
        |GEQ					{       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FGEQ [  %s  ]\n",$1);
                                                        #endif
                                                        $$ = 3;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        |EQ					{       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FEQ [  %s  ]\n",$1);
                                                        #endif
                                                        $$ = 0;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
        ;

arithExpr:
        arithStatement ARITHOP arithExpr               {       
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Complex Arith Expr\n"); 
                                                                #endif
                                                                sprintf($$,"%s %s %s",$1,$2,$3);
                                                                
                                                                //strcat($2,$3);strcat($1,$2);strcpy($$,$1);
                                                        }
        | arithStatement                                {
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Simple Arith Statement\n");
                                                                #endif
                                                                
                                                                strcpy($$,$1);
                                                                
                                                                //strcpy($$,$1);
                                                        }
        
        ;

arithStatement:
          ATOM                                         {       
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is an ID\n");
                                                               
                                                                #endif
                                                                strcpy($$,$1);                                                               
                                                        }
        | ARITHOP ATOM                                	{
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is an ID\n");
                                                                #endif
                                                                sprintf($$,"%s%s",$1,$2);
                                                        }
        | rational                                      {       
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is a Rational\n");
                                                                #endif
                                                                
                                                                strcpy($$,$1);
                                                        }
        ;
        

intervalList: 
	LIST_BEGIN intervalContent LIST_END 	{
                                                        #ifdef YACC_DEBUG_ON
                                                                printf("[intervalList]\n");
							#endif
							
							$$ = $2;
						}
	|LIST_BEGIN LIST_END 	{
                                                        #ifdef YACC_DEBUG_ON
                                                                printf("[intervalList] ************************          Empty\n");
							#endif
							
							$$ = NULL;        
				};
				
intervalContent: 
	intervalContent interval 		{       
                                                        #ifdef YACC_DEBUG_ON
                                                                printf("[intervalContent] List\n");
                                                        #endif
                                                        
							$$ = addIntervalToList($1,createIntervalList($2));
							
							#ifdef YACC_DEBUG_ON
                                                                printf("Printing Interval List: ");
                                                                printIntervalList($$);
                                                                printf("\n");
							#endif
						}
	|interval 				{	
                                                        #ifdef YACC_DEBUG_ON
                                                                printf("[intervalContent] Singular\n");
							#endif
							
							$$ = createIntervalList($1);
							
							#ifdef YACC_DEBUG_ON
                                                                printIntervalList($$);
							#endif
							
						};
						
interval: INTERVAL_BEGIN RATIONAL SEPARATOR RATIONAL INTERVAL_END
						{	
                                                        #ifdef YACC_DEBUG_ON
                                                                printf("[interval] [%s:%s)\n",$2,$4);
							#endif
							
							$$ = createIntervalStruct(atof($2),atof($4));
							
							#ifdef YACC_DEBUG_ON
                                                                printInterval($$);
                                                                printf("\n");
							#endif
							
						};
%%


void yyerror(char *s) {

        char errorMsg[MAX_STR_LENGTH]; bzero(errorMsg,sizeof(char)*MAX_STR_LENGTH);
        int flag = 0;
        switch(errNo){
                /*case BLOCK_NO_NEWLINE:        sprintf(errorMsg,"New statements must fall on a new line.");
                                        currentMatch = learnedOP_charPos;
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
        if(strlen(learnedOP_line)==0){
                strcpy(learnedOP_line,oldLine);
                learnedOP_charPos = learnedOP_oldPos;
                learnedOP_lineNo--;
        }
        if(flag==1){
                currentMatch = learnedOP_charPos+1;
        }
        //fprintf(stderr, "%s at Line %d ; %s Match [%d], line :[%s],  ERROR CODES: [%d,%d,%d,%d,%d,%d,%d]\n", s,learnedOP_lineNo,errorMsg,currentMatch,line,errNo,err1No,err2No,err3No,err4No,err5No,err6No);
        fprintf(stderr, "[LearningPredicates] %s at Line %d : %s \n", s,learnedOP_lineNo,errorMsg);
        fprintf(stderr, "%s\n",learnedOP_line);
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
