/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_PREDTOPY_PREDTOPY_TAB_H_INCLUDED
# define YY_PREDTOPY_PREDTOPY_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int predToPydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TRACEFILE = 258,
    TRACECOUNT = 259,
    SEQLENGTH = 260,
    DELAYRES = 261,
    DEPTH = 262,
    BESTCOUNT = 263,
    TMAX = 264,
    TMIN = 265,
    START = 266,
    PBEGIN = 267,
    PEND = 268,
    OPENROUND = 269,
    CLOSEROUND = 270,
    ATPOSEDGE = 271,
    ATNEGEDGE = 272,
    ATANYEDGE = 273,
    RATIONAL = 274,
    ARITHOP = 275,
    EQ = 276,
    LEQ = 277,
    GEQ = 278,
    LT = 279,
    GT = 280,
    SEMICOLON = 281,
    DOLLARTIME = 282,
    ATOM = 283,
    BAND = 284,
    BOR = 285,
    EEQ = 286,
    PREDLEARN = 287,
    COMMA = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 59 "predToPy.y" /* yacc.c:1909  */

	char string[MAX_STR_LENGTH];
	struct identifier* id;
	struct PORV* porvType;
	//struct eventType* event;
	struct expressionList* exprList;
	struct expression* expr;
	struct term* disjnct;
	struct config* inputs;
	int code;

#line 100 "predToPy.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE predToPylval;

int predToPyparse (void);

#endif /* !YY_PREDTOPY_PREDTOPY_TAB_H_INCLUDED  */
