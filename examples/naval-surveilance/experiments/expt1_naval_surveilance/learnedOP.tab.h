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

#ifndef YY_LEARNEDOP_LEARNEDOP_TAB_H_INCLUDED
# define YY_LEARNEDOP_LEARNEDOP_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int learnedOPdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    GAIN = 258,
    INTERVALS_BEGIN = 259,
    INTERVALS_END = 260,
    BUCKET = 261,
    BEGINPRED = 262,
    ENDPRED = 263,
    ATOM = 264,
    EEQ = 265,
    LIST_BEGIN = 266,
    LIST_END = 267,
    SEPARATOR = 268,
    RATIONAL = 269,
    OPENROUND = 270,
    CLOSEROUND = 271,
    ARITHOP = 272,
    EQ = 273,
    LEQ = 274,
    GEQ = 275,
    LT = 276,
    GT = 277,
    INTERVAL_BEGIN = 278,
    INTERVAL_END = 279
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 56 "learnedOP.y" /* yacc.c:1909  */

	char string[MAX_STR_LENGTH];
	double decimal;
	struct PORV* porvType;
	struct predicateDetail* predDetail;
	struct intervalStruct* intval;
	struct intervalListStruct* intervalList;
	struct intervalListStruct** traceIntervalSet;
	int code;

#line 90 "learnedOP.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE learnedOPlval;

int learnedOPparse (void);

#endif /* !YY_LEARNEDOP_LEARNEDOP_TAB_H_INCLUDED  */
