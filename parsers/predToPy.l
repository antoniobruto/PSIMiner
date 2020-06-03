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
        
        #include "predToPy.tab.h"
        #include <stdlib.h>
        #include <string.h>
        
        void yyerror(char *);
        int predToPy_lineNo = 1;
        int predToPy_charPos, predToPy_oldPos;
        int predToPy_multiLine = 0;
        char line[MAX_STR_LENGTH];
        char oldLine[MAX_STR_LENGTH];
%}

%%

[\n\r]                  {
                                #ifdef LEX_DEBUG_ON
                                printf("lex: NEWLINE\n");
                                #endif
                                
                                if(*yytext=='\n') ++predToPy_lineNo;
                                predToPy_oldPos=predToPy_charPos;
                                predToPy_charPos = 0;
                                if(predToPy_multiLine==0){
					strcpy(oldLine,line);
					bzero(line,sizeof(line));
					predToPy_multiLine = 1;
				}
                                //return NEWLINE;
                        }

[ \t]+                  {
                                predToPy_charPos+=yyleng;
				strcat(line,yytext); 
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
                        
"traceFile"		{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [traceFile]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return TRACEFILE;
                        }
                        
"seqLength"		{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [seqLength]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return SEQLENGTH;
                        }

"delayRes"		{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [delayRes]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return DELAYRES;
                        }
                        
"depth"			{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [depth]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return DEPTH;
                        }
                        
"bestPredCount"		{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [bestPredCount]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return BESTCOUNT;
                        }         

"tmin"			{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [tmin]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return TMIN;
                        }

"tmax"			{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [tmax]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return TMAX;
                        }


"start"			{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [start]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return START;
                        }
                        
                        
"begin"                 {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [begin]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return PBEGIN;
                        }

"end"                   {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [end]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return PEND;
                        }

"learnMode"		{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [learnMode]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return PREDLEARN;
                        }
                        
"traceCount"		{       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: keyword [traceCount]\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return TRACECOUNT;
                        }
                        
"timePrecision"			{		predToPy_multiLine=0;
								#ifdef LEX_DEBUG_ON
									printf("lex: keyword [traceCount]\n");
								#endif
								predToPy_charPos+=yyleng;
								strcat(line,yytext);
								strcpy(yylval.string,yytext);
								return TIMEPRECISION;
                        }

"("                     {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: (\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return OPENROUND;
                        }

")"                     {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: )\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return CLOSEROUND;
                        }

"@+"                    {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: @+\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return ATPOSEDGE;
                        }

"@-"                    {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: @-\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return ATNEGEDGE;
                        }

"@"                     {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: @\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return ATANYEDGE;
                        }


((([0-9]*)\.([0-9]+))|([0-9]+))((([eE])[\+\-]?[0-9]+)?) {       
				predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: RATIONAL\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return RATIONAL;
                        }
                        
"&&"                    {       predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: AND\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return BAND;
                        }
                        
"||"                    {       predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: OR\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return BOR;
                        }
                        
\+|\-|\*|\/             {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: ARITHOP\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return ARITHOP;
                        }

"=="                    {       predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: EQ\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return EQ;
                        }
                        
"<="                    {	predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: LEQ\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return LEQ;
                        }
                        
">="                    {	predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: GEQ\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return GEQ;
                        }
                        
"<"                     {	predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: LT\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return LT;
                        }
                        
                        
">"                     {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: GT\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return GT;
                        }

";"                     {       predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: SEMICOLON\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return SEMICOLON;
                        }
                        
"="                    {	predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: EEQ\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return EEQ;
                        }
                        
","                    {	predToPy_multiLine=0;
                                predToPy_charPos+=yyleng;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: COMMA\n");
                                #endif
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return COMMA;
                        }
                        
                        
"$time"                 {	predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: $time\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return DOLLARTIME;
                        }

(([_a-zA-Z]+)([-_a-zA-Z0-9\.]*)) {   
				predToPy_multiLine=0;
                                #ifdef LEX_DEBUG_ON
                                printf("lex: ATOM\n");
                                #endif
                                predToPy_charPos+=yyleng;
                                strcat(line,yytext);
                                strcpy(yylval.string,yytext);
                                return ATOM;
                        }

.                       {       
                                printf("Lexical Error at Line : [%d], Character Position : [%d], Token = [%s], \n",predToPy_lineNo,predToPy_charPos+1,yytext);                            
                                //exit(0);
                        }
%%


int yywrap(void) {
    return 1;
}





