#define yylval predToPylval
#define yy_create_buffer predToPy_create_buffer
#define yy_delete_buffer predToPy_delete_buffer
#define yy_flex_debug predToPy_flex_debug
#define yy_init_buffer predToPy_init_buffer
#define yy_flush_buffer predToPy_flush_buffer
#define yy_load_buffer_state predToPy_load_buffer_state
#define yy_switch_to_buffer predToPy_switch_to_buffer
#define yyin predToPyin
#define yyleng predToPyleng
#define yylex predToPylex
#define yylineno predToPylineno
#define yyout predToPyout
#define yyrestart predToPyrestart
#define yytext predToPytext
#define yywrap predToPywrap
#define yyalloc predToPyalloc
#define yyrealloc predToPyrealloc
#define yyfree predToPyfree

/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         predToPyparse
#define yylex           predToPylex
#define yyerror         predToPyerror
#define yydebug         predToPydebug
#define yynerrs         predToPynerrs

#define yylval          predToPylval
#define yychar          predToPychar

/* Copy the first part of user declarations.  */
#line 1 "predToPy.y" /* yacc.c:339  */

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

#line 125 "predToPy.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "predToPy.tab.h".  */
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
#line 59 "predToPy.y" /* yacc.c:355  */

	char string[MAX_STR_LENGTH];
	struct identifier* id;
	struct PORV* porvType;
	//struct eventType* event;
	struct expressionList* exprList;
	struct expression* expr;
	struct term* disjnct;
	struct config* inputs;
	int code;

#line 211 "predToPy.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE predToPylval;

int predToPyparse (void);

#endif /* !YY_PREDTOPY_PREDTOPY_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 228 "predToPy.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  30
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   101

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  112

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    83,    83,   100,   113,   125,   135,   145,   155,   165,
     175,   185,   195,   208,   218,   229,   239,   249,   259,   269,
     279,   296,   305,   313,   317,   345,   374,   382,   390,   398,
     406,   414,   423,   423,   444,   444,   467,   476,   483,   491,
     501,   509,   521,   527,   535,   585,   592,   599,   605,   611,
     647,   653,   662,   679,   696,   703,   712,   715,   718,   721,
     726,   733,   737,   742,   745,   750
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TRACEFILE", "TRACECOUNT", "SEQLENGTH",
  "DELAYRES", "DEPTH", "BESTCOUNT", "TMAX", "TMIN", "START", "PBEGIN",
  "PEND", "OPENROUND", "CLOSEROUND", "ATPOSEDGE", "ATNEGEDGE", "ATANYEDGE",
  "RATIONAL", "ARITHOP", "EQ", "LEQ", "GEQ", "LT", "GT", "SEMICOLON",
  "DOLLARTIME", "ATOM", "BAND", "BOR", "EEQ", "PREDLEARN", "COMMA",
  "$accept", "configSpec", "inputList", "learningType", "traceCT",
  "traceFile", "traceFileList", "sequenceLength", "delayResolution",
  "treeDepth", "bestGainCount", "tempMax", "tempMin", "predicateSpec",
  "$@1", "$@2", "startExpr", "variableMapList", "variableMap", "rational",
  "predicateList", "porv", "ineq", "arithExpr", "arithStatement",
  "expressList", "namedExpressionLine", "expressionLine", "disjunct",
  "endPList", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288
};
# endif

#define YYPACT_NINF -54

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-54)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      17,   -23,     1,     3,    26,    27,    36,    38,    39,    40,
      48,     8,   -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,
     -54,    44,    35,    35,    35,    35,    35,    35,    35,    35,
     -54,    35,    35,   -54,   -54,   -54,   -54,   -54,   -54,   -54,
     -54,   -54,   -54,    45,    45,   -54,   -54,    42,   -54,    55,
     -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,    35,
      45,    64,   -54,    49,   -54,   -54,    66,    51,   -54,    51,
      41,    18,   -54,    18,   -54,   -54,   -54,   -54,   -54,    32,
     -54,   -54,    19,    19,    10,   -54,   -54,   -54,   -54,    60,
      35,    50,    19,   -54,    53,    19,   -54,    32,   -54,    15,
      69,   -54,    53,    71,   -54,   -54,    35,    53,    35,   -54,
      20,   -54
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    19,    20,    12,    13,    14,    18,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     0,     0,    11,     3,     4,     5,     6,    10,     7,
       8,     9,     2,     0,    34,    38,    25,    23,    40,     0,
      22,    26,    27,    28,    29,    30,    31,    21,    36,     0,
      32,     0,    37,     0,    41,    39,     0,     0,    24,     0,
       0,     0,    43,     0,    49,    47,    48,    45,    46,     0,
      65,    42,     0,     0,     0,    55,    52,    54,    44,    51,
       0,     0,    35,    59,    57,    33,    53,     0,    64,     0,
       0,    58,    56,     0,    50,    62,     0,    60,     0,    63,
       0,    61
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -54,   -54,   -54,    76,    77,    78,   -54,    79,    80,    81,
      82,    83,    84,   -54,   -54,   -54,   -54,    54,   -16,   -22,
      29,   -30,   -54,    -1,   -54,    16,   -53,   -39,    -8,    28
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    10,    11,    12,    13,    14,    47,    15,    16,    17,
      18,    19,    20,    42,    66,    61,    43,    44,    45,    87,
      71,    72,    79,    88,    89,    92,    93,    94,    99,    82
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      50,    51,    52,    53,    54,    55,    56,    57,    21,    58,
      59,     1,     2,     3,     4,     5,     6,     7,     8,    31,
       1,     2,     3,     4,     5,     6,     7,     8,    62,    64,
     105,    80,    22,    90,    23,   111,    32,    65,    96,   101,
       9,    81,   101,    81,    62,   106,    70,    91,    30,     9,
     106,    48,    84,   102,    48,    49,   102,    24,    25,    85,
      86,   107,    74,    75,    76,    77,    78,    26,    98,    27,
      28,    29,    46,    32,    64,    63,    67,    68,    69,    70,
      97,   100,   103,    90,   109,   108,    98,    33,    34,    35,
      36,    37,    38,    39,    40,    41,   104,    60,    73,    95,
     110,    83
};

static const yytype_uint8 yycheck[] =
{
      22,    23,    24,    25,    26,    27,    28,    29,    31,    31,
      32,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       3,     4,     5,     6,     7,     8,     9,    10,    44,    19,
      15,    13,    31,    14,    31,    15,    28,    59,    28,    92,
      32,    71,    95,    73,    60,    30,    28,    28,     0,    32,
      30,    19,    20,    92,    19,    20,    95,    31,    31,    27,
      28,   100,    21,    22,    23,    24,    25,    31,    90,    31,
      31,    31,    28,    28,    19,    33,    12,    28,    12,    28,
      20,    31,    29,    14,   106,    14,   108,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    97,    43,    69,    83,
     108,    73
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    32,
      35,    36,    37,    38,    39,    41,    42,    43,    44,    45,
      46,    31,    31,    31,    31,    31,    31,    31,    31,    31,
       0,    11,    28,    37,    38,    39,    41,    42,    43,    44,
      45,    46,    47,    50,    51,    52,    28,    40,    19,    20,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      51,    49,    52,    33,    19,    53,    48,    12,    28,    12,
      28,    54,    55,    54,    21,    22,    23,    24,    25,    56,
      13,    55,    63,    63,    20,    27,    28,    53,    57,    58,
      14,    28,    59,    60,    61,    59,    28,    20,    53,    62,
      31,    60,    61,    29,    57,    15,    30,    61,    14,    53,
      62,    15
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    34,    35,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    37,    38,    39,    40,    40,    41,    42,    43,    44,
      45,    46,    48,    47,    49,    47,    50,    51,    51,    52,
      53,    53,    54,    54,    55,    56,    56,    56,    56,    56,
      57,    57,    58,    58,    58,    58,    59,    59,    59,    59,
      60,    61,    61,    62,    62,    63
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     0,     7,     0,     6,     2,     2,     1,     3,
       1,     2,     2,     1,     3,     1,     1,     1,     1,     1,
       3,     1,     1,     2,     1,     1,     2,     1,     2,     1,
       3,     5,     3,     3,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 83 "predToPy.y" /* yacc.c:1646  */
    { 
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
#line 1396 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 100 "predToPy.y" /* yacc.c:1646  */
    {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							if(traceCount>0){
								printf("Trace Count was defined multiple times, considering the latest\n");
							}
							inputConfig->traceCount = atoi((yyvsp[0].string));
							traceCount = atoi((yyvsp[0].string));
						}
#line 1414 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 113 "predToPy.y" /* yacc.c:1646  */
    {	
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
#line 1431 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 125 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-sequenceLength statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->N = atoi((yyvsp[0].string));
						}
#line 1446 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 135 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-delayResolution statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->K = atof((yyvsp[0].string));
						}
#line 1461 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 145 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-bestGainCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->bestGainCount = atoi((yyvsp[0].string));
						}
#line 1476 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 155 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-tempMax statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->tmax = atof((yyvsp[0].string));
							
						}
#line 1491 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 165 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-tempMin statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->tmin = atof((yyvsp[0].string));
							
						}
#line 1506 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 175 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-treeDepth statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->maxTreeDepth = atoi((yyvsp[0].string));
							
						}
#line 1521 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 185 "predToPy.y" /* yacc.c:1646  */
    {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-learningType statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->learnType = atoi((yyvsp[0].string));
						
						}
#line 1536 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 195 "predToPy.y" /* yacc.c:1646  */
    {	
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
#line 1554 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 208 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-sequenceLength statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->N = atoi((yyvsp[0].string));
						}
#line 1569 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 218 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-delayResolution statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->K = atof((yyvsp[0].string));
							
						}
#line 1585 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 229 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-bestGainCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->bestGainCount = atoi((yyvsp[0].string));
						}
#line 1600 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 239 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-tempMax statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->tmax = atof((yyvsp[0].string));
						}
#line 1615 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 249 "predToPy.y" /* yacc.c:1646  */
    {	
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched lone-input-tempMin statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							inputConfig->tmin = atof((yyvsp[0].string));
						}
#line 1630 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 259 "predToPy.y" /* yacc.c:1646  */
    {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-treeDepth statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->maxTreeDepth = atoi((yyvsp[0].string));
							
						}
#line 1645 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 269 "predToPy.y" /* yacc.c:1646  */
    {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-learningType statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							inputConfig->learnType = atoi((yyvsp[0].string));
						
						}
#line 1660 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 279 "predToPy.y" /* yacc.c:1646  */
    {
							#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched input-traceCount statement\n");
							#endif
							if(!inputConfig){
								inputConfig = createConfig();
							} 
							
							if(traceCount>0){
								printf("Trace Count was defined multiple times, considering the latest\n");
							}
							inputConfig->traceCount = atoi((yyvsp[0].string));
							traceCount = atoi((yyvsp[0].string));;
						
						}
#line 1680 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 296 "predToPy.y" /* yacc.c:1646  */
    {
					        #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched traceFile statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1692 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 305 "predToPy.y" /* yacc.c:1646  */
    {
												#ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched traceCount statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1704 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 313 "predToPy.y" /* yacc.c:1646  */
    {
						strcpy((yyval.string),(yyvsp[0].string));
					}
#line 1712 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 318 "predToPy.y" /* yacc.c:1646  */
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
							inputConfig->traceFileNames = createIdentifier((yyvsp[0].string));
						} else {
							inputConfig->traceFileNames = addToIdentifierList(inputConfig->traceFileNames,(yyvsp[0].string));
						}
						traceCount--;
						strcpy((yyval.string),(yyvsp[0].string));
					}
#line 1744 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 345 "predToPy.y" /* yacc.c:1646  */
    {
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
							inputConfig->traceFileNames = createIdentifier((yyvsp[0].string));
						} else {
							inputConfig->traceFileNames = addToIdentifierList(inputConfig->traceFileNames,(yyvsp[0].string));
						}
						traceCount--;
						//printf("[%s]\n",$2);
						strcpy((yyval.string),(yyvsp[0].string));
					}
#line 1777 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 374 "predToPy.y" /* yacc.c:1646  */
    {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched sequenceLength statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1789 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 382 "predToPy.y" /* yacc.c:1646  */
    {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched delayResolution statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1801 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 390 "predToPy.y" /* yacc.c:1646  */
    {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched treeDepth statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1813 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 398 "predToPy.y" /* yacc.c:1646  */
    {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched bestGainCount statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1825 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 406 "predToPy.y" /* yacc.c:1646  */
    {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched tempMax statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1837 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 414 "predToPy.y" /* yacc.c:1646  */
    {
						#ifdef YACC_DEBUG_ON 
								printf("PARSER: Matched tempMin statement\n");
						#endif
						//printf("[%s]\n",$2);
						strcpy((yyval.string),(yyvsp[0].string)); 
					}
#line 1849 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 423 "predToPy.y" /* yacc.c:1646  */
    { idList = (yyvsp[0].id);}
#line 1855 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 423 "predToPy.y" /* yacc.c:1646  */
    {      
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Matched File\n");
                                                                #endif
                                                                if(!predicateMap)
																	predicateMap = createFile();
                                                                predicateMap->start = (yyvsp[-6].code);
                                                                predicateMap->varList = (yyvsp[-5].id);
                                                                predicateMap->porvList = (yyvsp[-2].porvType);
                                                                predicateMap->exprList = (yyvsp[0].exprList);
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
#line 1881 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 444 "predToPy.y" /* yacc.c:1646  */
    { idList = (yyvsp[0].id);}
#line 1887 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 444 "predToPy.y" /* yacc.c:1646  */
    {      
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Matched File\n");
                                                                #endif
                                                                if(!predicateMap)
																	predicateMap = createFile();
                                                                predicateMap->start = 0;
                                                                predicateMap->varList = (yyvsp[-5].id);
                                                                predicateMap->porvList = (yyvsp[-2].porvType);
                                                                predicateMap->exprList = (yyvsp[0].exprList);
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
#line 1913 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 467 "predToPy.y" /* yacc.c:1646  */
    {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched start statement\n");
                                                #endif
                                                //printf("[%s]\n",$2);
                                                (yyval.code) = atoi((yyvsp[0].string));
                                        }
#line 1925 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 476 "predToPy.y" /* yacc.c:1646  */
    {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched variableMapList\n");
                                                #endif
                                                (yyval.id)=addIdentifierToList((yyvsp[-1].id),(yyvsp[0].id));
                                        }
#line 1936 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 483 "predToPy.y" /* yacc.c:1646  */
    {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched variableMapList\n");
                                                #endif
                                                (yyval.id)=(yyvsp[0].id);
                                        }
#line 1947 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 491 "predToPy.y" /* yacc.c:1646  */
    {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched variableMap\n");
                                                #endif
                                                (yyval.id)=createIdentifier((yyvsp[-2].string));
                                                (yyval.id)->col = atoi((yyvsp[-1].string));
                                                (yyval.id)->timeCol = atoi((yyvsp[0].string));
                                        }
#line 1960 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 501 "predToPy.y" /* yacc.c:1646  */
    {       
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Linear Expr is a Rational [%s]\n",(yyvsp[0].string));
                                                #endif
                                                
                                                //$$=createCondition($1,EF_dummy,-1);
                                                strcpy((yyval.string),(yyvsp[0].string));
                                        }
#line 1973 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 510 "predToPy.y" /* yacc.c:1646  */
    {       
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Linear Expr is a Rational[%s%s]\n",(yyvsp[-1].string),(yyvsp[0].string));
                                                #endif
                                                
                                                //$$=createCondition($1,EF_dummy,-1);
                                                sprintf((yyval.string),"%s%s",(yyvsp[-1].string),(yyvsp[0].string));
                                                //strcpy($$,$1);
                                        }
#line 1987 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 521 "predToPy.y" /* yacc.c:1646  */
    {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched predicateList\n");
                                                #endif                                                                                                
                                                (yyval.porvType)=addPORVToList((yyvsp[-1].porvType),(yyvsp[0].porvType));
                                        }
#line 1998 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 527 "predToPy.y" /* yacc.c:1646  */
    {
                                                #ifdef YACC_DEBUG_ON 
                                                        printf("PARSER: Matched predicateList\n");
                                                #endif
                                                (yyval.porvType)=(yyvsp[0].porvType);
                                        }
#line 2009 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 535 "predToPy.y" /* yacc.c:1646  */
    {
											#ifdef YACC_DEBUG_ON 
													printf("PARSER: porv: ATOM ineq arithExpr\n");
											#endif
											if(idList==NULL){
													printf("ERROR: No variables declared\n");
													exit(0);
											}
											int atomID = getIdentifierID(idList,(yyvsp[-2].string));
											if(atomID==0){
													printf("ERROR: No variable with name [%s] declared\n",(yyvsp[-2].string));
													exit(0);
											}
											char temp[MAX_STR_LENGTH];
											int col = getIdentifierCol(idList,atomID);
											if(col==-1){
													printf("ERROR: col val is -1: atomID value mismatch\n");
													exit(0);
											}
											sprintf(temp,"float(row[%d])",col-1);                                                        
											(yyval.porvType) = createPORV(createCondition(temp,(yyvsp[0].string),(yyvsp[-1].code)),porvID++,atomID); 
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
#line 2048 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 585 "predToPy.y" /* yacc.c:1646  */
    {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FLT [  %s  ]\n",(yyvsp[0].string));
                                                        #endif
                                                       // yyerror("A '<' inequality is not allowed\n");
                                                        (yyval.code) =4;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
#line 2060 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 592 "predToPy.y" /* yacc.c:1646  */
    {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FGT [  %s  ]\n",(yyvsp[0].string));
                                                        #endif
                                                        //yyerror("A '>' inequality is not allowed\n");
                                                        (yyval.code) = 5;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
#line 2072 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 599 "predToPy.y" /* yacc.c:1646  */
    {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FLEQ [  %s  ]\n",(yyvsp[0].string));
                                                        #endif
                                                        (yyval.code) = 1;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
#line 2083 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 605 "predToPy.y" /* yacc.c:1646  */
    {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FGEQ [  %s  ]\n",(yyvsp[0].string));
                                                        #endif
                                                        (yyval.code) = 3;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
#line 2094 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 611 "predToPy.y" /* yacc.c:1646  */
    {       
                                                        #ifdef YACC_DEBUG_ON 
                                                                printf("PARSER: ineq: FEQ [  %s  ]\n",(yyvsp[0].string));
                                                        #endif
                                                        (yyval.code) = 0;//$$=createCondition($1->name,EF_dummy,-1);//strcpy($$,$1);
                                                }
#line 2105 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 647 "predToPy.y" /* yacc.c:1646  */
    {
													#ifdef YACC_DEBUG_ON 
															printf("PARSER: Complex Arith Expr\n"); 
													#endif
													sprintf((yyval.string),"%s %s %s",(yyvsp[-2].string),(yyvsp[-1].string),(yyvsp[0].string));
												}
#line 2116 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 653 "predToPy.y" /* yacc.c:1646  */
    {
													#ifdef YACC_DEBUG_ON 
															printf("PARSER: Simple Arith Statement\n");
													#endif
													strcpy((yyval.string),(yyvsp[0].string));
												}
#line 2127 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 662 "predToPy.y" /* yacc.c:1646  */
    {
													#ifdef YACC_DEBUG_ON 
															printf("PARSER: Linear Expr is an ID\n");
													#endif
													int atomID = getIdentifierID(idList,(yyvsp[0].string));
													if(atomID==0){
															printf("ERROR: No variable with name [%s] declared\n",(yyvsp[0].string));
															exit(0);
													}
													int col = getIdentifierCol(idList,atomID);
													if(col==-1){
															printf("ERROR: col val is -1: atomID value mismatch\n");
															exit(0);
													}
													sprintf((yyval.string),"float(row[%d])",col-1);
													//strcpy($$,$1);                                                               
												}
#line 2149 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 679 "predToPy.y" /* yacc.c:1646  */
    {
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is an ID\n");
                                                                //strcpy($2,$1);//strcpy($$,$1);
                                                                #endif
                                                                int atomID = getIdentifierID(idList,(yyvsp[0].string));
                                                                if(atomID==0){
                                                                        printf("ERROR: No variable with name [%s] declared\n",(yyvsp[0].string));
                                                                        exit(0);
                                                                }
                                                                int col = getIdentifierCol(idList,atomID);
                                                                if(col==-1){
                                                                        printf("ERROR: col val is -1: atomID value mismatch\n");
                                                                        exit(0);
                                                                }
                                                                sprintf((yyval.string),"%sfloat(row[%d])",(yyvsp[-1].string),col-1);
                                                        }
#line 2171 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 696 "predToPy.y" /* yacc.c:1646  */
    {       
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is a Rational\n");
                                                                #endif
                                                                
                                                                strcpy((yyval.string),(yyvsp[0].string));
                                                        }
#line 2183 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 703 "predToPy.y" /* yacc.c:1646  */
    {       
                                                                #ifdef YACC_DEBUG_ON 
                                                                        printf("PARSER: Linear Expr is $TIME\n");
                                                                #endif
                                                                
                                                                strcpy((yyval.string),(yyvsp[0].string));
                                                        }
#line 2195 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 712 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.exprList) = addToExpressionList((yyvsp[-1].exprList),(yyvsp[0].expr));
							}
#line 2203 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 715 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.exprList) = createExpressionList((yyvsp[0].expr));	
							}
#line 2211 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 718 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.exprList) = addExpressionListToExpressionList((yyvsp[-1].exprList),(yyvsp[0].exprList));
							}
#line 2219 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 721 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.exprList) = (yyvsp[0].exprList);
							}
#line 2227 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 726 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.exprList) = createExpressionList((yyvsp[0].expr));
								(yyval.exprList)->id = (char*)malloc(sizeof(char)*MAX_STR_LENGTH);
								strcpy((yyval.exprList)->id,(yyvsp[-2].string));
							}
#line 2237 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 734 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.expr) = addTermToExpression((yyvsp[-4].expr),(yyvsp[-1].disjnct));
                                                        }
#line 2245 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 737 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.expr) = createExpression((yyvsp[-1].disjnct));
							}
#line 2253 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 742 "predToPy.y" /* yacc.c:1646  */
    {
								(yyval.disjnct) = addPORVToTerm((yyvsp[-2].disjnct),atoi((yyvsp[0].string)));
							}
#line 2261 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 745 "predToPy.y" /* yacc.c:1646  */
    {	
								(yyval.disjnct) = createTerm(atoi((yyvsp[0].string)));	
							}
#line 2269 "predToPy.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 750 "predToPy.y" /* yacc.c:1646  */
    {	
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
#line 2290 "predToPy.tab.c" /* yacc.c:1646  */
    break;


#line 2294 "predToPy.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 767 "predToPy.y" /* yacc.c:1906  */



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
