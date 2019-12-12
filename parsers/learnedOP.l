/*****************************************************************
 *                      CODE WRITTEN BY                          *
 *                                                               *
 *             Antonio Anastasio Bruto da Costa                  *
 *     Formal Method Laboratory, Dept. of CSE, IIT Kharagpur     *
 *                                                               *
 *****************************************************************/

%{
        //#define LEX_DEBUG_ON
        
        #ifndef MAX_STR_LENGTH
                #define MAX_STR_LENGTH 10240
        #endif
        
        #include "learnedOP.tab.h"
        #include <stdlib.h>
        #include <string.h>
        
        void yyerror(char *);
        int learnedOP_lineNo = 1;
        int learnedOP_charPos, learnedOP_oldPos;
        int learnedOP_multiLine = 0;
        char learnedOP_line[MAX_STR_LENGTH] = {0};         
        char oldLine[MAX_STR_LENGTH] = {0};
        
%}

%%

[\n\r]                  {
                                #ifdef LEX_DEBUG_ON
                                printf("lex: NEWLINE\n");
                                #endif
                                
                                if(*yytext=='\n') ++learnedOP_lineNo;
                                learnedOP_oldPos=learnedOP_charPos;
                                learnedOP_charPos = 0;
                                if(learnedOP_multiLine==0){
					strcpy(oldLine,learnedOP_line);
					bzero(learnedOP_line,sizeof(learnedOP_line));
					learnedOP_multiLine = 1;
				}
                                //return NEWLINE;
                        }

[ \t]+                  {
                                learnedOP_charPos+=yyleng;
				strcat(learnedOP_line,yytext); 
                                /*if(*yytext!='\t') 
                                        strcat(line,yytext); 
                                else {
                                        int len = strlen(line);
                                        line[len++] = ' ';
                                        line[len] = '\0';
                                }
                                */
                                //printf("**********************************   STRLEN = [%d], yyleng = [%d] for text [%s]\n",strlen(yytext),yyleng,yytext);
                                //if(*yytext=='\t'){printf("Tab Found\n");}
                                #ifdef LEX_DEBUG_ON
                                printf("lex: Whitespace found\n");
                                #endif
                                
                        }
                        
"begin_pred"		{       learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [begin_pred]\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return BEGINPRED;
                        }
                        
"end_pred"		{       learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [end_pred]\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return ENDPRED;
                        }

"gain"			{       learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [gain]\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return GAIN;
                        }
                        
"bucket"		{       learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [bucket]\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return BUCKET;
                        }
                        
"intervals"		{       learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [intervals]\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return INTERVALS;
                        }

"{"		{	
				#ifdef LEX_DEBUG_ON
				printf("lex: LIST_BEGIN\n");
				#endif
				learnedOP_charPos+=yyleng;
				strcat(learnedOP_line,yytext);
				strcpy(yylval.string,yytext);
				return LIST_BEGIN;
			}

"}"			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: LIST_END\n");
				#endif
				learnedOP_charPos+=yyleng;
				strcat(learnedOP_line,yytext);
				strcpy(yylval.string,yytext);
				return LIST_END;
			}

"["			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: INTERVAL_BEGIN\n");
				#endif
				learnedOP_charPos+=yyleng;
				strcat(learnedOP_line,yytext);
				strcpy(yylval.string,yytext);
				return INTERVAL_BEGIN;
			}

")"			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: INTERVAL_END\n");
				#endif
				learnedOP_charPos+=yyleng;
				strcat(learnedOP_line,yytext);
				strcpy(yylval.string,yytext);
				return INTERVAL_END;
			}

":"			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: SEPARATOR\n");
				#endif
				learnedOP_charPos+=yyleng;
				strcat(learnedOP_line,yytext);
				strcpy(yylval.string,yytext);
				return SEPARATOR;
			}


"=="                    {       learnedOP_multiLine=0;
                                learnedOP_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: EQ\n");
                                #endif
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return EQ;
                        }
                        
"<="                    {	learnedOP_multiLine=0;
                                learnedOP_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: LEQ\n");
                                #endif
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return LEQ;
                        }
                        
">="                    {	learnedOP_multiLine=0;
                                learnedOP_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: GEQ\n");
                                #endif
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return GEQ;
                        }
                        
"<"                     {	learnedOP_multiLine=0;
                                learnedOP_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: LT\n");
                                #endif
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return LT;
                        }
                        
                        
">"                     {       learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: GT\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return GT;
                        }			
			
\-?((([0-9]*)\.([0-9]+))|([0-9]+))((([eE])[\+\-]?[0-9]+)?) {	
				#ifdef LEX_DEBUG_ON
				printf("lex: RATIONAL\n");
				#endif
				learnedOP_charPos+=yyleng;
				strcat(learnedOP_line,yytext);
				strcpy(yylval.string,yytext);
				return RATIONAL;
			}
                        
"="                    {	learnedOP_multiLine=0;
                                learnedOP_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: EEQ\n");
                                #endif
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return EEQ;
                        }
                        
(([_a-zA-Z]+)([_a-zA-Z0-9\.]*)) {   
				learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: ATOM\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return ATOM;
                        }
                        
\+|\-|\*|\/             {       learnedOP_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: ARITHOP\n");
                                #endif
                                learnedOP_charPos+=yyleng;
                                strcat(learnedOP_line,yytext);
                                strcpy(yylval.string,yytext);
                                return ARITHOP;
                        }                        

.                       {       
                                printf("Lexical Error at Line : [%d], Character Position : [%d], Token = [%s], \n",learnedOP_lineNo,learnedOP_charPos+1,yytext);                            
                                //exit(0);
                        }
%%


int yywrap(void) {
    return 1;
}





