%{
	#define LEX_DEBUG_ON
	
	#ifndef MAX_STR_LENGTH
		#define MAX_STR_LENGTH 10240
	#endif
	
	#include "intervalList.tab.h"
	#include <stdlib.h>
	#include <string.h>
	
	void yyerror(char *);
	int lineNo = 1;
	int charPos, oldPos;
	char line[MAX_STR_LENGTH];
	char oldLine[MAX_STR_LENGTH];
%}

%%

[\n\r]			{
				#ifdef LEX_DEBUG_ON
				printf("lex: NEWLINE\n");
				#endif
				if(*yytext=='\n') ++lineNo;
				oldPos=charPos;
				charPos = 0;
				strcpy(oldLine,line);
				bzero(line,sizeof(line));
				
				//return NEWLINE;
			}

[ \t]+			{
				charPos+=yyleng;
				if(strlen(line)>=(MAX_STR_LENGTH-yyleng)) line[0]='\0';
				
				if(*yytext!='\t') 
					strcat(line,yytext); 
				else {
					int len = strlen(line);
					line[len++] = ' ';
					line[len] = '\0';
				}
				//printf("**********************************   STRLEN = [%d], yyleng = [%d] for text [%s]\n",strlen(yytext),yyleng,yytext);
				//if(*yytext=='\t'){printf("Tab Found\n");}
				#ifdef LEX_DEBUG_ON
				printf("lex: Whitespace found\n");
				#endif
				
			}
			
"{"		{	
				#ifdef LEX_DEBUG_ON
				printf("lex: LIST_BEGIN\n");
				#endif
				if(strlen(line)>=(MAX_STR_LENGTH-yyleng)) line[0]='\0';
				charPos+=yyleng;
				strcat(line,yytext);
				strcpy(yylval.string,yytext);
				return LIST_BEGIN;
			}

"}"			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: LIST_END\n");
				#endif
				if(strlen(line)>=(MAX_STR_LENGTH-yyleng)) line[0]='\0';
				charPos+=yyleng;
				strcat(line,yytext);
				strcpy(yylval.string,yytext);
				return LIST_END;
			}

"["			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: INTERVAL_BEGIN\n");
				#endif
				if(strlen(line)>=(MAX_STR_LENGTH-yyleng)) line[0]='\0';
				charPos+=yyleng;
				strcat(line,yytext);
				strcpy(yylval.string,yytext);
				return INTERVAL_BEGIN;
			}

")"			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: INTERVAL_END\n");
				#endif
				if(strlen(line)>=(MAX_STR_LENGTH-yyleng)) line[0]='\0';
				charPos+=yyleng;
				strcat(line,yytext);
				strcpy(yylval.string,yytext);
				return INTERVAL_END;
			}

":"			{	
				#ifdef LEX_DEBUG_ON
				printf("lex: SEPARATOR\n");
				#endif
				if(strlen(line)>=(MAX_STR_LENGTH-yyleng)) line[0]='\0';
				charPos+=yyleng;
				strcat(line,yytext);
				strcpy(yylval.string,yytext);
				return SEPARATOR;
			}
			
\-?((([0-9]*)\.([0-9]+))|([0-9]+))((([eE])[\+\-]?[0-9]+)?) {	
				#ifdef LEX_DEBUG_ON
				printf("lex: RATIONAL\n");
				#endif
				if(strlen(line)>=(MAX_STR_LENGTH-yyleng)) line[0]='\0';
				charPos+=yyleng;
				strcat(line,yytext);
				strcpy(yylval.string,yytext);
				return RATIONAL;
			}

.			{	
				printf("Lexical Error at Line : [%d], Character Position : [%d], Token = [%s], \n",lineNo,charPos+1,yytext);				
				//exit(0);
			}
%%


int yywrap(void) {
    return 1;
}





