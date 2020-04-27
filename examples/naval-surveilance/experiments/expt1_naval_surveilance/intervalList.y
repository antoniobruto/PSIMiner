%{
	#define YACC_DEBUG_ON
	
	#ifndef MAX_STR_LENGTH
		#define MAX_STR_LENGTH 10240
	#endif
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <float.h>
	#include "psiMinerStructs.h"
	
	//Function Declarations
	void yyerror(char *);
	
	//External Objects
	extern int yylex();
	extern int lineNo,charPos,yyleng,yylineno,yychar,oldPos;
	extern char line[MAX_STR_LENGTH],oldLine[MAX_STR_LENGTH];
	extern FILE *yyin;
	extern char* yytext;
	extern struct listOfIntervalListsStruct** listOfIntervalSets;
	extern int traceID;
	extern double tl;
	
	//Global Objects
	int errNo,err1No,err2No,err3No,err4No,err5No,err6No;
	int currentMatch;
	char F_dummy[1] = {'\0'};
	
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
%token <string> LIST_BEGIN LIST_END INTERVAL_BEGIN INTERVAL_END SEPARATOR RATIONAL;

//Start Production Rule
%start file

//Data Types Used for Tokens and Reduction Rules
%union {
		char string[MAX_STR_LENGTH];
		struct intervalStruct* intval;
		struct intervalListStruct* intervalList;
		struct listOfIntervalListsStruct* listOfIntervalLists;
	}

%type <intval> interval;
%type <intervalList> intervalContent intervalList;
%type <listOfIntervalLists> file setOfIntervals;

%%
file:
	setOfIntervals RATIONAL			{	
							tl = atof($2);
							printf("length = %lf",atof$2);
						};


setOfIntervals:
	setOfIntervals intervalList			{	//printf("HERE (2)\n");
							$$ = addListToListOfIntervalLists($1,createListOfIntervalLists($2,NULL));
							listOfIntervalSets[traceID] = $$;
							
							#ifdef YACC_DEBUG_ON
                                                                printf("FILE (2) INTERVAL LIST \n");
                                                                printListOfIntervalLists($$);
                                                                //printf("NEXT\n");
                                                        #endif
						}
	| intervalList				{
							$$ = createListOfIntervalLists($1,NULL);
							listOfIntervalSets[traceID] = $$;
							
							#ifdef YACC_DEBUG_ON
								printf("FILE (1) INTERVAL LIST \n");
                                                                printListOfIntervalLists($$);
                                                                //printf("HERE (1)\n");
                                                        #endif
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
						}
	;

intervalContent: 
	intervalContent interval 		{       
                                                        #ifdef YACC_DEBUG_ON
                                                                printf("[intervalContent] List\n");fflush(stdout);
                                                        #endif
                                                        
							$$ = addIntervalToList($1,createIntervalList($2));
							
							#ifdef YACC_DEBUG_ON
                                                                printf("Printing Interval List: ");
                                                                printIntervalList($$);
                                                                printf("\nEND\n");
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
                                                                printf("[interval] [%s:%s)\n",$2,$4);fflush(stdout);
							#endif
							
							$$ = createIntervalStruct(atof($2),atof($4));
							
							#ifdef YACC_DEBUG_ON
                                                                printInterval($$);
                                                                printf("\n");
							#endif
							
						};
%%


void yyerror(char *s) {

	char errorMsg[MAX_STR_LENGTH];
	int flag = 0;
	switch(errNo){
		/*case BLOCK_NO_NEWLINE:	sprintf(errorMsg,"New statements must fall on a new line.");
					currentMatch = charPos;
					break;*/
		case IF_NO_TEST:	sprintf(errorMsg,"Expected a logical condition expression AFTER \"if\".");
					break;
		case IF_NO_COLON:	sprintf(errorMsg,"Expected a \":\" after the condition expression.");
					flag = 1;
					break;
		case TEST_EQ_EXPR:	sprintf(errorMsg,"The condition expression can't contain an assignment operator. Use Logical Operators only.");
					break;
		case WHILE_NO_COLON:	sprintf(errorMsg,"Expected a \":\" after the condition expression.");
					flag = 1;
					break;
		case WHILE_NO_TEST:	sprintf(errorMsg,"Expected a logical condition expression AFTER \"while\".");
					break;
		case PRINT_INVALID :	sprintf(errorMsg,"Invalid print statement syntax.");
					break;
		case TEST_NO_LOGICAL:	sprintf(errorMsg,"Expected a logical operator between the two expressions.");
					flag = 1;
					break;
		case EXPR_INVALID:	sprintf(errorMsg,"Invalid Expression Format.");
					break;
		default:		sprintf(errorMsg,"Hey Didn't expect to see this here.");
					flag = 1;
					break;
	}
	if(strlen(line)==0){
		strcpy(line,oldLine);
		charPos = oldPos;
		lineNo--;
	}
	if(flag==1){
		currentMatch = charPos+1;
	}
	//fprintf(stderr, "%s at Line %d ; %s Match [%d], line :[%s],  ERROR CODES: [%d,%d,%d,%d,%d,%d,%d]\n", s,lineNo,errorMsg,currentMatch,line,errNo,err1No,err2No,err3No,err4No,err5No,err6No);
	fprintf(stderr, "[IntervalListParser] %s at Line %d : %s \n", s,lineNo,errorMsg);
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
	yyin = fopen(argv[1], "r");
	setbuf(stdout, NULL);
	yyparse();
}

*/
