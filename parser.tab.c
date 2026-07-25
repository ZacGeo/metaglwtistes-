/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "parser.y"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diagnostics.h"
#include "symtab.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;
extern void yyrestart(FILE *input_file);
void lexer_set_trace(int enabled);

void yyerror(const char *message);

static char *pending_function_name = NULL;

typedef struct PendingParameter {
    char *name;
    struct PendingParameter *next;
} PendingParameter;

static PendingParameter *pending_parameters = NULL;
static PendingParameter *pending_parameters_tail = NULL;

static void declare_identifier(char *name);
static void discard_text(char *text);
static void enter_named_scope(const char *kind, const char *name);
static void remember_function(char *name);
static void enter_function_scope(void);
static void finish_function_scope(void);
static void remember_parameter(char *name);
static void declare_pending_parameters(void);
static void clear_pending_parameters(void);

/* Line 371 of yacc.c  */
#line 107 "parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TYPEDEF = 258,
     CHAR = 259,
     INT = 260,
     FLOAT = 261,
     STRING = 262,
     CONST = 263,
     CLASS = 264,
     PRIVATE = 265,
     PROTECTED = 266,
     PUBLIC = 267,
     VOID = 268,
     STATIC = 269,
     UNION = 270,
     ENUM = 271,
     LIST = 272,
     CONTINUE = 273,
     BREAK = 274,
     IF = 275,
     ELSE = 276,
     WHILE = 277,
     FOR = 278,
     RETURN = 279,
     LENGTH = 280,
     CIN = 281,
     COUT = 282,
     MAIN = 283,
     THIS = 284,
     SIZEOP = 285,
     OROP = 286,
     ANDOP = 287,
     EQUOP = 288,
     RELOP = 289,
     ADDOP = 290,
     MULOP = 291,
     NOTOP = 292,
     INCDEC = 293,
     METH = 294,
     INP = 295,
     OUT = 296,
     REFER = 297,
     LPAREN = 298,
     RPAREN = 299,
     SEMI = 300,
     DOT = 301,
     COMMA = 302,
     ASSIGN = 303,
     COLON = 304,
     LBRACK = 305,
     RBRACK = 306,
     LBRACE = 307,
     RBRACE = 308,
     ICONST = 309,
     FCONST = 310,
     CCONST = 311,
     SCONST = 312,
     ID = 313,
     LISTFUNC = 314,
     LOWER_THAN_ELSE = 315
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 43 "parser.y"

    long long ival;
    double fval;
    char *sval;


/* Line 387 of yacc.c  */
#line 217 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 245 "parser.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

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
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   737

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  191
/* YYNRULES -- Number of states.  */
#define YYNSTATES  350

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   315

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    10,    12,    14,    16,    18,
      20,    22,    24,    27,    34,    36,    38,    40,    42,    44,
      46,    48,    50,    51,    54,    55,    59,    62,    67,    71,
      73,    78,    80,    84,    88,    90,    92,    96,    98,   102,
     104,   108,   110,   114,   116,   120,   122,   126,   128,   131,
     134,   137,   140,   142,   145,   147,   152,   157,   159,   163,
     167,   169,   174,   178,   183,   186,   188,   192,   194,   198,
     200,   202,   203,   205,   207,   209,   211,   215,   216,   222,
     226,   230,   235,   238,   241,   242,   243,   249,   254,   259,
     262,   263,   267,   270,   273,   276,   279,   280,   282,   284,
     286,   288,   292,   296,   298,   302,   306,   310,   314,   317,
     319,   321,   323,   326,   329,   334,   338,   343,   346,   348,
     351,   355,   356,   362,   366,   370,   372,   375,   377,   379,
     382,   383,   387,   388,   392,   397,   402,   407,   410,   415,
     418,   420,   423,   427,   431,   435,   439,   442,   444,   446,
     447,   450,   452,   457,   462,   467,   469,   470,   473,   475,
     477,   479,   481,   483,   485,   487,   489,   492,   495,   497,
     500,   503,   509,   517,   523,   533,   535,   536,   540,   545,
     550,   554,   556,   558,   562,   564,   566,   568,   572,   576,
     577,   581
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      62,     0,    -1,    63,   154,    -1,    63,    64,    -1,    -1,
      65,    -1,    71,    -1,    91,    -1,    96,    -1,   118,    -1,
     120,    -1,   123,    -1,     1,    45,    -1,     3,    66,    68,
      58,    69,    45,    -1,    67,    -1,    58,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,    13,    -1,    17,    -1,
      -1,    69,    70,    -1,    -1,    50,    54,    51,    -1,    50,
      51,    -1,     8,    66,    72,    45,    -1,    72,    47,    73,
      -1,    73,    -1,    58,    69,    48,    74,    -1,    76,    -1,
      52,    75,    53,    -1,    75,    47,    74,    -1,    74,    -1,
      77,    -1,    77,    31,    78,    -1,    78,    -1,    78,    32,
      79,    -1,    79,    -1,    79,    33,    80,    -1,    80,    -1,
      80,    34,    81,    -1,    81,    -1,    81,    35,    82,    -1,
      82,    -1,    82,    36,    83,    -1,    83,    -1,    37,    83,
      -1,    35,    83,    -1,    30,    83,    -1,    38,    85,    -1,
      84,    -1,    85,    38,    -1,    85,    -1,    85,    43,    88,
      44,    -1,    25,    43,    86,    44,    -1,    89,    -1,    43,
      86,    44,    -1,    43,    67,    44,    -1,    90,    -1,    85,
      50,    86,    51,    -1,    85,    46,    58,    -1,    59,    43,
      86,    44,    -1,   138,    58,    -1,    29,    -1,    86,    47,
      87,    -1,    87,    -1,    85,    48,    87,    -1,    76,    -1,
      86,    -1,    -1,    56,    -1,    54,    -1,    55,    -1,    57,
      -1,    50,    88,    51,    -1,    -1,    16,    58,    92,    93,
      45,    -1,    52,    94,    53,    -1,    52,     1,    53,    -1,
      94,    47,    58,    95,    -1,    58,    95,    -1,    48,    74,
      -1,    -1,    -1,     9,    58,    97,    98,    45,    -1,    99,
      52,   100,    53,    -1,    99,    52,     1,    53,    -1,    49,
      58,    -1,    -1,   100,   101,   102,    -1,   101,   102,    -1,
      10,    49,    -1,    11,    49,    -1,    12,    49,    -1,    -1,
     103,    -1,   111,    -1,   104,    -1,   107,    -1,    66,   105,
      45,    -1,   105,    47,   106,    -1,   106,    -1,    68,    58,
      69,    -1,    15,   108,    45,    -1,    52,   109,    53,    -1,
      52,     1,    53,    -1,   109,   110,    -1,   110,    -1,   104,
      -1,   112,    -1,   113,    45,    -1,   117,    45,    -1,   114,
      43,   115,    44,    -1,    66,    68,    58,    -1,   115,    47,
      66,   116,    -1,    66,   116,    -1,    42,    -1,    68,    69,
      -1,   114,    43,    44,    -1,    -1,    15,    58,   119,   108,
      45,    -1,    66,   121,    45,    -1,   121,    47,   122,    -1,
     122,    -1,   106,    95,    -1,   112,    -1,   124,    -1,   127,
     134,    -1,    -1,   133,   125,   134,    -1,    -1,   117,   126,
     134,    -1,   128,    43,   130,    44,    -1,   114,    43,   130,
      44,    -1,    66,    68,   129,    58,    -1,    58,    39,    -1,
     130,    47,    66,   131,    -1,    66,   131,    -1,   132,    -1,
      42,    58,    -1,    68,    58,    69,    -1,   128,    43,    44,
      -1,    52,   135,    53,    -1,    52,     1,    53,    -1,   136,
     139,    -1,   136,    -1,   139,    -1,    -1,   136,   137,    -1,
     137,    -1,   138,    66,   105,    45,    -1,   138,    66,     1,
      45,    -1,   138,    66,   105,     1,    -1,    14,    -1,    -1,
     139,   140,    -1,   140,    -1,   141,    -1,   142,    -1,   143,
      -1,   144,    -1,   146,    -1,   147,    -1,   153,    -1,    18,
      45,    -1,    19,    45,    -1,    45,    -1,     1,    45,    -1,
      86,    45,    -1,    20,    43,    86,    44,   140,    -1,    20,
      43,    86,    44,   140,    21,   140,    -1,    22,    43,    86,
      44,   140,    -1,    23,    43,   145,    45,   145,    45,   145,
      44,   140,    -1,    86,    -1,    -1,    24,   145,    45,    -1,
      26,    40,   148,    45,    -1,    27,    41,   150,    45,    -1,
     148,    40,   149,    -1,   149,    -1,    85,    -1,   150,    41,
     151,    -1,   151,    -1,    86,    -1,    52,    -1,   152,   135,
      53,    -1,   152,     1,    53,    -1,    -1,   156,   155,   134,
      -1,     5,    28,    43,    44,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    77,    77,    81,    82,    86,    87,    88,    89,    90,
      91,    92,    93,   101,   108,   109,   116,   117,   118,   119,
     120,   124,   125,   129,   130,   134,   135,   139,   143,   144,
     148,   155,   156,   160,   161,   167,   171,   172,   176,   177,
     181,   182,   186,   187,   191,   192,   196,   197,   201,   202,
     203,   204,   205,   209,   210,   211,   212,   213,   214,   215,
     216,   220,   221,   225,   229,   233,   237,   238,   242,   243,
     247,   248,   252,   256,   257,   258,   265,   270,   269,   282,
     283,   291,   295,   302,   303,   308,   307,   320,   321,   329,
     333,   337,   338,   342,   343,   344,   345,   349,   350,   354,
     355,   359,   363,   364,   368,   377,   381,   382,   390,   391,
     395,   399,   403,   408,   416,   420,   429,   430,   434,   435,
     439,   444,   443,   456,   460,   461,   465,   469,   470,   474,
     479,   478,   487,   486,   497,   502,   510,   519,   526,   527,
     531,   532,   539,   546,   550,   551,   559,   560,   561,   562,
     566,   567,   571,   572,   577,   588,   589,   593,   594,   598,
     599,   600,   601,   602,   603,   604,   605,   606,   607,   608,
     616,   620,   621,   625,   629,   633,   634,   638,   642,   643,
     647,   648,   652,   656,   657,   661,   665,   672,   676,   686,
     685,   696
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TYPEDEF", "CHAR", "INT", "FLOAT",
  "STRING", "CONST", "CLASS", "PRIVATE", "PROTECTED", "PUBLIC", "VOID",
  "STATIC", "UNION", "ENUM", "LIST", "CONTINUE", "BREAK", "IF", "ELSE",
  "WHILE", "FOR", "RETURN", "LENGTH", "CIN", "COUT", "MAIN", "THIS",
  "SIZEOP", "OROP", "ANDOP", "EQUOP", "RELOP", "ADDOP", "MULOP", "NOTOP",
  "INCDEC", "METH", "INP", "OUT", "REFER", "LPAREN", "RPAREN", "SEMI",
  "DOT", "COMMA", "ASSIGN", "COLON", "LBRACK", "RBRACK", "LBRACE",
  "RBRACE", "ICONST", "FCONST", "CCONST", "SCONST", "ID", "LISTFUNC",
  "LOWER_THAN_ELSE", "$accept", "program", "global_declarations",
  "global_declaration", "typedef_declaration", "typename", "standard_type",
  "listspec", "dims", "dim", "const_declaration", "constdefs", "constdef",
  "init_value", "init_values", "expression", "or_expression",
  "and_expression", "equality_expression", "relational_expression",
  "additive_expression", "multiplicative_expression", "unary_expression",
  "postfix_expression", "variable", "general_expression", "assignment",
  "expression_list", "constant", "listexpression", "enum_declaration",
  "$@1", "enum_body", "id_list", "initializer", "class_declaration", "$@2",
  "class_body", "parent", "members_methods", "access", "member_or_method",
  "member", "var_declaration", "variabledefs", "variabledef",
  "anonymous_union", "union_body", "fields", "field", "method",
  "short_func_declaration", "short_par_func_header", "func_header_start",
  "parameter_types", "pass_list_dims", "nopar_func_header",
  "union_declaration", "$@3", "global_var_declaration",
  "init_variabledefs", "init_variabledef", "func_declaration",
  "full_func_declaration", "$@4", "$@5", "full_par_func_header",
  "class_func_header_start", "func_class", "parameter_list",
  "pass_variabledef", "parameter_variabledef", "nopar_class_func_header",
  "function_body", "decl_statements", "declarations", "local_declaration",
  "decltype", "statements", "statement", "expression_statement",
  "if_statement", "while_statement", "for_statement", "optexpr",
  "return_statement", "io_statement", "in_list", "in_item", "out_list",
  "out_item", "compound_open", "comp_statement", "main_function", "$@6",
  "main_header", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    63,    64,    64,    64,    64,    64,
      64,    64,    64,    65,    66,    66,    67,    67,    67,    67,
      67,    68,    68,    69,    69,    70,    70,    71,    72,    72,
      73,    74,    74,    75,    75,    76,    77,    77,    78,    78,
      79,    79,    80,    80,    81,    81,    82,    82,    83,    83,
      83,    83,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    85,    85,    85,    85,    85,    86,    86,    87,    87,
      88,    88,    89,    89,    89,    89,    90,    92,    91,    93,
      93,    94,    94,    95,    95,    97,    96,    98,    98,    99,
      99,   100,   100,   101,   101,   101,   101,   102,   102,   103,
     103,   104,   105,   105,   106,   107,   108,   108,   109,   109,
     110,   111,   112,   112,   113,   114,   115,   115,   116,   116,
     117,   119,   118,   120,   121,   121,   122,   123,   123,   124,
     125,   124,   126,   124,   127,   127,   128,   129,   130,   130,
     131,   131,   132,   133,   134,   134,   135,   135,   135,   135,
     136,   136,   137,   137,   137,   138,   138,   139,   139,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     141,   142,   142,   143,   144,   145,   145,   146,   147,   147,
     148,   148,   149,   150,   150,   151,   152,   153,   153,   155,
     154,   156
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     2,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     2,     0,     3,     2,     4,     3,     1,
       4,     1,     3,     3,     1,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     2,     2,
       2,     2,     1,     2,     1,     4,     4,     1,     3,     3,
       1,     4,     3,     4,     2,     1,     3,     1,     3,     1,
       1,     0,     1,     1,     1,     1,     3,     0,     5,     3,
       3,     4,     2,     2,     0,     0,     5,     4,     4,     2,
       0,     3,     2,     2,     2,     2,     0,     1,     1,     1,
       1,     3,     3,     1,     3,     3,     3,     3,     2,     1,
       1,     1,     2,     2,     4,     3,     4,     2,     1,     2,
       3,     0,     5,     3,     3,     1,     2,     1,     1,     2,
       0,     3,     0,     3,     4,     4,     4,     2,     4,     2,
       1,     2,     3,     3,     3,     3,     2,     1,     1,     0,
       2,     1,     4,     4,     4,     1,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     2,
       2,     5,     7,     5,     9,     1,     0,     3,     4,     4,
       3,     1,     1,     3,     1,     1,     1,     3,     3,     0,
       3,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     1,     0,     0,    16,    17,    18,    19,
       0,     0,    20,     0,     0,    15,     3,     5,    22,    14,
       6,     7,     8,   127,     0,     0,   132,     9,    10,    11,
     128,     0,     0,   130,     2,   189,    12,    17,    22,     0,
       0,    85,   121,    77,    21,     0,    84,     0,   125,   112,
       0,   113,     0,     0,   129,     0,     0,     0,     0,     0,
      24,     0,    29,    90,     0,     0,    24,     0,   156,   126,
     123,    22,   120,    22,     0,     0,   133,     0,   155,     0,
       0,     0,     0,     0,   156,     0,     0,     0,    65,   156,
     156,   156,   156,   156,   168,    71,   186,    73,    74,    72,
      75,     0,    69,    35,    37,    39,    41,    43,    45,    47,
      52,    54,     0,    67,    57,    60,     0,     0,   151,     0,
       0,   158,   159,   160,   161,   162,   163,   164,     0,   165,
     143,    22,     0,   131,   190,    24,   191,     0,    27,     0,
       0,     0,     0,     0,     0,     0,     0,   137,   104,   136,
     156,    83,    31,    54,     0,     0,   124,   118,    24,   117,
     139,   140,   114,     0,   135,     0,   169,   145,   166,   167,
     156,   156,   156,   175,     0,   156,   156,   156,    50,    49,
      48,    51,     0,     0,    70,     0,   156,   156,   156,   156,
     156,   156,   156,    53,    71,     0,   156,   156,   170,   156,
     144,     0,   150,     0,    64,     0,   157,     0,     0,     0,
       0,   134,     0,   156,     0,    23,    28,    89,    86,     0,
       0,    22,   110,     0,   109,   122,     0,    84,     0,    78,
      34,     0,    64,    24,   141,    24,   119,    22,    22,     0,
       0,     0,   177,     0,   182,     0,   181,   185,     0,   184,
      59,    58,    76,     0,    36,    38,    40,    42,    44,    46,
       0,    62,    68,     0,    66,     0,     0,   103,   188,   187,
      13,    30,    26,     0,     0,     0,     0,     0,    96,     0,
     107,     0,   106,   108,    80,    82,     0,    79,   156,    32,
     142,   118,    24,   116,   138,     0,     0,   156,    56,   156,
     178,   156,   179,    63,    55,    61,   153,   154,   152,    22,
      25,    88,    93,    94,    95,    87,     0,     0,    22,    92,
      97,    99,   100,    98,   111,     0,     0,   101,    84,    33,
     171,   173,     0,   180,   183,   102,    91,     0,     0,     0,
      81,     0,   156,   105,    24,    22,   172,     0,     0,   174
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    16,    17,   221,    19,   155,   148,   215,
      20,    61,    62,   151,   231,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   185,   114,   115,
      21,    65,   146,   228,    69,    22,    63,   141,   142,   278,
     279,   319,   320,   222,   281,   267,   322,   144,   223,   224,
     323,   324,    24,   325,    74,   159,   326,    27,    64,    28,
      47,    48,    29,    30,    56,    52,    31,    32,    67,    75,
     160,   161,    33,    54,   116,   117,   118,   154,   120,   121,
     122,   123,   124,   125,   174,   126,   127,   245,   246,   248,
     249,   128,   129,    34,    57,    35
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -249
static const yytype_int16 yypact[] =
{
    -249,    29,    58,  -249,   -15,   118,  -249,    18,  -249,  -249,
     118,   -13,  -249,    -1,    61,  -249,  -249,  -249,    70,  -249,
    -249,  -249,  -249,  -249,    65,    68,    75,  -249,  -249,  -249,
    -249,    76,   101,  -249,  -249,  -249,  -249,  -249,    70,   113,
     106,  -249,  -249,  -249,  -249,   131,   147,    82,  -249,  -249,
     206,  -249,    76,   265,  -249,   224,    76,    76,   151,   164,
    -249,   143,  -249,   166,   172,   173,    87,   158,   544,  -249,
    -249,    70,  -249,     5,   150,   152,  -249,   -12,  -249,   194,
     204,   189,   231,   232,   577,   237,   246,   240,  -249,   678,
     678,   678,    40,   511,  -249,   610,  -249,  -249,  -249,  -249,
    -249,   261,  -249,   266,   273,   278,   299,   300,   298,  -249,
    -249,   105,   210,  -249,  -249,  -249,   283,   325,  -249,   134,
     148,  -249,  -249,  -249,  -249,  -249,  -249,  -249,   384,  -249,
    -249,    59,   170,  -249,  -249,  -249,  -249,   248,  -249,   106,
     282,   296,   290,    19,   301,    17,   311,  -249,   307,  -249,
     544,  -249,  -249,   315,   306,   308,  -249,   309,   313,  -249,
    -249,  -249,  -249,   118,  -249,   118,  -249,  -249,  -249,  -249,
     678,   678,   577,   312,   324,   678,    40,   678,  -249,  -249,
    -249,   100,   328,   179,   312,   322,   678,   678,   678,   678,
     678,   678,   678,  -249,   610,   316,   678,   678,  -249,   678,
    -249,   331,  -249,   430,    26,    27,  -249,    37,   333,   309,
     313,  -249,   107,   544,   187,  -249,  -249,  -249,  -249,   241,
     334,    70,  -249,   254,  -249,  -249,   339,   147,    25,  -249,
    -249,    53,  -249,  -249,  -249,  -249,   307,    63,    59,   196,
     218,   348,  -249,   229,   100,   114,  -249,   312,   124,  -249,
    -249,  -249,  -249,   262,   273,   278,   299,   300,   298,  -249,
     350,  -249,  -249,   133,  -249,   351,    41,  -249,  -249,  -249,
    -249,  -249,  -249,   344,   346,   352,   356,   363,    28,    45,
    -249,   280,  -249,  -249,  -249,  -249,   342,  -249,   544,  -249,
     307,  -249,  -249,  -249,  -249,   476,   476,   577,  -249,    40,
    -249,   678,  -249,  -249,  -249,  -249,  -249,  -249,  -249,    70,
    -249,  -249,  -249,  -249,  -249,  -249,    45,   172,    70,  -249,
    -249,  -249,  -249,  -249,  -249,   372,    75,  -249,   147,  -249,
     395,  -249,   373,  -249,  -249,  -249,  -249,   375,   359,   206,
    -249,   476,   645,  -249,   380,    63,  -249,   381,   476,  -249
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -249,  -249,  -249,  -249,  -249,    -2,   335,   -17,   -56,  -249,
    -249,  -249,   285,  -143,  -249,   -53,  -249,   239,   242,   243,
     245,   255,   -80,  -249,   -55,   -79,   117,   251,  -249,  -249,
    -249,  -249,  -249,  -249,  -215,  -249,  -249,  -249,  -249,  -249,
     155,   135,  -249,  -248,   253,   -16,  -249,   130,  -249,   238,
    -249,   460,  -249,   461,  -249,   227,   464,  -249,  -249,  -249,
    -249,   398,  -249,  -249,  -249,  -249,  -249,  -249,  -249,   415,
     233,  -249,  -249,    52,   353,  -249,   355,   -34,   357,  -114,
    -249,  -249,  -249,  -249,  -155,  -249,  -249,  -249,   177,  -249,
     178,  -249,  -249,  -249,  -249,  -249
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -177
static const yytype_int16 yytable[] =
{
      18,    45,    46,    38,   137,   173,   206,   230,    40,   178,
     179,   180,   285,   153,   183,   152,   184,   241,   226,   119,
     220,    58,    44,     6,    37,     8,     9,   -15,   265,     3,
      36,   321,    12,   166,   153,   153,   153,   181,   275,   276,
     277,   167,   307,   -15,    44,    41,    39,   157,    73,     6,
      37,     8,     9,   131,    78,    46,   158,    42,    12,     4,
     317,     5,     6,     7,     8,     9,    10,    11,   321,    88,
     271,    12,   286,    13,    14,   227,    44,    15,   287,   212,
      44,   315,   166,   119,   -15,   -22,   308,    44,   309,   206,
     268,   239,   240,   173,   119,   153,   243,   152,   247,   101,
     288,   209,   236,    15,    76,   291,   289,   253,   133,   134,
      49,    50,   259,   340,   210,   184,    15,   205,   263,    43,
      51,   244,     6,    37,     8,     9,   147,    70,    53,    71,
    -115,    12,   153,   153,   153,   153,   153,   153,     6,    37,
       8,     9,   332,   193,    55,   329,   195,    12,   194,   201,
     197,   195,   270,   196,   299,   197,    59,   214,   153,   300,
     152,   237,    78,   238,    60,   301,    79,    80,    81,   302,
      82,    83,    84,    85,    86,    87,    15,    88,    89,   290,
     199,   330,   331,    90,   305,    91,    92,   347,   138,    66,
     139,    93,   204,    94,   162,    68,   164,   163,    95,   165,
      96,  -148,    97,    98,    99,   100,  -156,   101,   136,   135,
       6,    37,     8,     9,   211,   140,   149,   165,   173,    12,
     292,   210,   247,   251,   143,   145,   199,   346,     6,    37,
       8,     9,   170,   153,   349,   152,   236,    12,   272,   168,
     295,   273,   274,   199,   244,   -96,   -96,   -96,   -96,   169,
      72,   275,   276,   277,   -96,   198,   -96,   199,     6,    37,
       8,     9,   296,   173,    15,   199,    77,    12,   130,  -156,
    -156,  -156,  -156,   298,   171,   172,   199,   318,  -156,    78,
     175,   177,    15,    79,    80,    81,   176,    82,    83,    84,
      85,    86,    87,   335,    88,    89,   213,   187,   214,   -96,
      90,   338,    91,    92,   186,   188,   303,   282,    93,   199,
      94,   189,    15,   262,   318,    95,   264,    96,  -149,    97,
      98,    99,   100,  -156,   101,   327,   201,   309,   292,  -156,
    -156,  -156,  -156,   190,   192,   191,   200,   345,  -156,    78,
     217,   218,   219,    79,    80,    81,   225,    82,    83,    84,
      85,    86,    87,   193,    88,    89,   229,   214,   194,   199,
      90,   195,    91,    92,   232,   197,   233,   234,    93,   242,
      94,   235,   250,   252,   261,    95,   166,    96,  -147,    97,
      98,    99,   100,  -156,   101,   207,   269,   280,  -156,  -156,
    -156,  -156,   284,   297,   304,   310,   306,  -156,    78,   311,
     328,   312,    79,    80,    81,   313,    82,    83,    84,    85,
      86,    87,   314,    88,    89,   339,   341,   344,   342,    90,
     343,    91,    92,  -115,   216,   348,   254,    93,   182,    94,
     255,   201,   256,   316,    95,   257,    96,  -149,    97,    98,
      99,   100,  -156,   101,    78,   260,   258,   337,    79,    80,
      81,   336,    82,    83,    84,    85,    86,    87,   266,    88,
      89,   283,    23,    25,   293,    90,    26,    91,    92,   156,
     132,   294,   202,    93,   203,    94,   333,   201,     0,   334,
      95,   208,    96,  -146,    97,    98,    99,   100,  -156,   101,
      78,     0,     0,     0,    79,    80,    81,     0,    82,    83,
      84,    85,    86,    87,     0,    88,    89,     0,     0,     0,
       0,    90,     0,    91,    92,     6,    37,     8,     9,    93,
       0,    94,     0,     0,    12,    78,    95,     0,    96,     0,
      97,    98,    99,   100,  -156,   101,    85,     0,     0,     0,
      88,    89,     0,     0,     0,     0,    90,     0,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,    78,     0,
       0,    95,     0,     0,     0,    97,    98,    99,   100,    85,
     101,     0,     0,    88,    89,     0,     0,     0,     0,    90,
       0,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,    78,     0,     0,    95,     0,   150,     0,    97,    98,
      99,   100,    85,   101,     0,     0,    88,    89,     0,     0,
       0,     0,    90,     0,    91,    92,     0,     0,     0,     0,
      93,     0,  -176,     0,    78,     0,     0,    95,     0,     0,
       0,    97,    98,    99,   100,    85,   101,     0,     0,    88,
      89,     0,     0,     0,     0,    90,     0,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,    78,
      95,     0,     0,     0,    97,    98,    99,   100,  -156,   101,
      85,     0,     0,     0,    88,    89,     0,     0,     0,     0,
      90,     0,    91,    92,     0,     0,     0,     0,    93,  -176,
       0,     0,    78,     0,     0,    95,     0,     0,     0,    97,
      98,    99,   100,    85,   101,     0,     0,    88,    89,     0,
       0,     0,     0,    90,     0,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,    97,    98,    99,   100,     0,   101
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-249)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,    18,    18,     5,    60,    84,   120,   150,    10,    89,
      90,    91,   227,    68,    93,    68,    95,   172,     1,    53,
       1,    38,    17,     4,     5,     6,     7,     1,     1,     0,
      45,   279,    13,    45,    89,    90,    91,    92,    10,    11,
      12,    53,     1,    17,    17,    58,    28,    42,    50,     4,
       5,     6,     7,    55,    14,    71,    73,    58,    13,     1,
      15,     3,     4,     5,     6,     7,     8,     9,   316,    29,
     213,    13,    47,    15,    16,    58,    17,    58,    53,   135,
      17,    53,    45,   117,    58,    58,    45,    17,    47,   203,
      53,   170,   171,   172,   128,   150,   175,   150,   177,    59,
      47,    42,   158,    58,    52,    42,    53,   186,    56,    57,
      45,    43,   192,   328,   131,   194,    58,   119,   197,    58,
      45,   176,     4,     5,     6,     7,    39,    45,    52,    47,
      43,    13,   187,   188,   189,   190,   191,   192,     4,     5,
       6,     7,   297,    38,    43,   288,    46,    13,    43,     1,
      50,    46,    45,    48,    40,    50,    43,    50,   213,    45,
     213,   163,    14,   165,    58,    41,    18,    19,    20,    45,
      22,    23,    24,    25,    26,    27,    58,    29,    30,   235,
      47,   295,   296,    35,    51,    37,    38,   342,    45,    58,
      47,    43,    58,    45,    44,    48,    44,    47,    50,    47,
      52,    53,    54,    55,    56,    57,    58,    59,    44,    58,
       4,     5,     6,     7,    44,    49,    58,    47,   297,    13,
     237,   238,   301,    44,    52,    52,    47,   341,     4,     5,
       6,     7,    43,   288,   348,   288,   292,    13,    51,    45,
      44,    54,     1,    47,   299,     4,     5,     6,     7,    45,
      44,    10,    11,    12,    13,    45,    15,    47,     4,     5,
       6,     7,    44,   342,    58,    47,     1,    13,    44,     4,
       5,     6,     7,    44,    43,    43,    47,   279,    13,    14,
      43,    41,    58,    18,    19,    20,    40,    22,    23,    24,
      25,    26,    27,   309,    29,    30,    48,    31,    50,    58,
      35,   318,    37,    38,    43,    32,    44,    53,    43,    47,
      45,    33,    58,   196,   316,    50,   199,    52,    53,    54,
      55,    56,    57,    58,    59,    45,     1,    47,   345,     4,
       5,     6,     7,    34,    36,    35,    53,   339,    13,    14,
      58,    45,    52,    18,    19,    20,    45,    22,    23,    24,
      25,    26,    27,    38,    29,    30,    45,    50,    43,    47,
      35,    46,    37,    38,    58,    50,    58,    58,    43,    45,
      45,    58,    44,    51,    58,    50,    45,    52,    53,    54,
      55,    56,    57,    58,    59,     1,    53,    53,     4,     5,
       6,     7,    53,    45,    44,    51,    45,    13,    14,    53,
      58,    49,    18,    19,    20,    49,    22,    23,    24,    25,
      26,    27,    49,    29,    30,    43,    21,    58,    45,    35,
      45,    37,    38,    43,   139,    44,   187,    43,    93,    45,
     188,     1,   189,   278,    50,   190,    52,    53,    54,    55,
      56,    57,    58,    59,    14,   194,   191,   317,    18,    19,
      20,   316,    22,    23,    24,    25,    26,    27,   205,    29,
      30,   223,     2,     2,   237,    35,     2,    37,    38,    71,
      55,   238,   117,    43,   117,    45,   299,     1,    -1,   301,
      50,   128,    52,    53,    54,    55,    56,    57,    58,    59,
      14,    -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,     4,     5,     6,     7,    43,
      -1,    45,    -1,    -1,    13,    14,    50,    -1,    52,    -1,
      54,    55,    56,    57,    58,    59,    25,    -1,    -1,    -1,
      29,    30,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    14,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    56,    57,    25,
      59,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    14,    -1,    -1,    50,    -1,    52,    -1,    54,    55,
      56,    57,    25,    59,    -1,    -1,    29,    30,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,
      43,    -1,    45,    -1,    14,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    56,    57,    25,    59,    -1,    -1,    29,
      30,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    14,
      50,    -1,    -1,    -1,    54,    55,    56,    57,    58,    59,
      25,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    -1,    14,    -1,    -1,    50,    -1,    -1,    -1,    54,
      55,    56,    57,    25,    59,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    59
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    62,    63,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    13,    15,    16,    58,    64,    65,    66,    67,
      71,    91,    96,   112,   113,   114,   117,   118,   120,   123,
     124,   127,   128,   133,   154,   156,    45,     5,    66,    28,
      66,    58,    58,    58,    17,    68,   106,   121,   122,    45,
      43,    45,   126,    52,   134,    43,   125,   155,    68,    43,
      58,    72,    73,    97,   119,    92,    58,   129,    48,    95,
      45,    47,    44,    66,   115,   130,   134,     1,    14,    18,
      19,    20,    22,    23,    24,    25,    26,    27,    29,    30,
      35,    37,    38,    43,    45,    50,    52,    54,    55,    56,
      57,    59,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    89,    90,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   146,   147,   152,   153,
      44,    66,   130,   134,   134,    58,    44,    69,    45,    47,
      49,    98,    99,    52,   108,    52,    93,    39,    69,    58,
      52,    74,    76,    85,   138,    68,   122,    42,    68,   116,
     131,   132,    44,    47,    44,    47,    45,    53,    45,    45,
      43,    43,    43,    86,   145,    43,    40,    41,    83,    83,
      83,    85,    67,    86,    86,    88,    43,    31,    32,    33,
      34,    35,    36,    38,    43,    46,    48,    50,    45,    47,
      53,     1,   137,   139,    58,    66,   140,     1,   135,    42,
      68,    44,    69,    48,    50,    70,    73,    58,    45,    52,
       1,    66,   104,   109,   110,    45,     1,    58,    94,    45,
      74,    75,    58,    58,    58,    58,    69,    66,    66,    86,
      86,   145,    45,    86,    85,   148,   149,    86,   150,   151,
      44,    44,    51,    86,    78,    79,    80,    81,    82,    83,
      88,    58,    87,    86,    87,     1,   105,   106,    53,    53,
      45,    74,    51,    54,     1,    10,    11,    12,   100,   101,
      53,   105,    53,   110,    53,    95,    47,    53,    47,    53,
      69,    42,    68,   116,   131,    44,    44,    45,    44,    40,
      45,    41,    45,    44,    44,    51,    45,     1,    45,    47,
      51,    53,    49,    49,    49,    53,   101,    15,    66,   102,
     103,   104,   107,   111,   112,   114,   117,    45,    58,    74,
     140,   140,   145,   149,   151,   106,   102,   108,    68,    43,
      95,    21,    45,    45,    58,    66,   140,   145,    44,   140
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 56: /* CCONST */
/* Line 1398 of yacc.c  */
#line 68 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1580 "parser.tab.c"
        break;
      case 57: /* SCONST */
/* Line 1398 of yacc.c  */
#line 68 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1587 "parser.tab.c"
        break;
      case 58: /* ID */
/* Line 1398 of yacc.c  */
#line 68 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1594 "parser.tab.c"
        break;
      case 59: /* LISTFUNC */
/* Line 1398 of yacc.c  */
#line 68 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1601 "parser.tab.c"
        break;

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

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
      yychar = YYLEX;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 12:
/* Line 1792 of yacc.c  */
#line 94 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 102 "parser.y"
    {
          declare_identifier((yyvsp[(4) - (6)].sval));
      }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 110 "parser.y"
    {
          discard_text((yyvsp[(1) - (1)].sval));
      }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 149 "parser.y"
    {
          declare_identifier((yyvsp[(1) - (4)].sval));
      }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 222 "parser.y"
    {
          discard_text((yyvsp[(3) - (3)].sval));
      }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 226 "parser.y"
    {
          discard_text((yyvsp[(1) - (4)].sval));
      }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 230 "parser.y"
    {
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 253 "parser.y"
    {
          discard_text((yyvsp[(1) - (1)].sval));
      }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 259 "parser.y"
    {
          discard_text((yyvsp[(1) - (1)].sval));
      }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 270 "parser.y"
    {
          symtab_declare((yyvsp[(2) - (2)].sval));
          enter_named_scope("enum", (yyvsp[(2) - (2)].sval));
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 276 "parser.y"
    {
          symtab_leave_scope();
      }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 284 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 292 "parser.y"
    {
          declare_identifier((yyvsp[(3) - (4)].sval));
      }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 296 "parser.y"
    {
          declare_identifier((yyvsp[(1) - (2)].sval));
      }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 308 "parser.y"
    {
          symtab_declare((yyvsp[(2) - (2)].sval));
          enter_named_scope("class", (yyvsp[(2) - (2)].sval));
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 314 "parser.y"
    {
          symtab_leave_scope();
      }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 322 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 330 "parser.y"
    {
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 369 "parser.y"
    {
          declare_identifier((yyvsp[(2) - (3)].sval));
      }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 383 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 404 "parser.y"
    {
          free(pending_function_name);
          pending_function_name = NULL;
      }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 409 "parser.y"
    {
          free(pending_function_name);
          pending_function_name = NULL;
      }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 421 "parser.y"
    {
          symtab_declare((yyvsp[(3) - (3)].sval));
          remember_function((yyvsp[(3) - (3)].sval));
          discard_text((yyvsp[(3) - (3)].sval));
      }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 444 "parser.y"
    {
          symtab_declare((yyvsp[(2) - (2)].sval));
          enter_named_scope("union", (yyvsp[(2) - (2)].sval));
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 450 "parser.y"
    {
          symtab_leave_scope();
      }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 475 "parser.y"
    {
          finish_function_scope();
      }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 479 "parser.y"
    {
          enter_function_scope();
      }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 483 "parser.y"
    {
          finish_function_scope();
      }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 487 "parser.y"
    {
          enter_function_scope();
      }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 491 "parser.y"
    {
          finish_function_scope();
      }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 498 "parser.y"
    {
          enter_function_scope();
          declare_pending_parameters();
      }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 503 "parser.y"
    {
          enter_function_scope();
          declare_pending_parameters();
      }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 511 "parser.y"
    {
          symtab_declare((yyvsp[(4) - (4)].sval));
          remember_function((yyvsp[(4) - (4)].sval));
          discard_text((yyvsp[(4) - (4)].sval));
      }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 520 "parser.y"
    {
          discard_text((yyvsp[(1) - (2)].sval));
      }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 533 "parser.y"
    {
          remember_parameter((yyvsp[(2) - (2)].sval));
      }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 540 "parser.y"
    {
          remember_parameter((yyvsp[(2) - (3)].sval));
      }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 552 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 573 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 578 "parser.y"
    {
          /* Common error-production rule: a missing semicolon after a
             local declaration. The unexpected lookahead is retained so
             parsing can continue with the next declaration/statement. */
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 609 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 666 "parser.y"
    {
          symtab_enter_scope("compound block");
      }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 673 "parser.y"
    {
          symtab_leave_scope();
      }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 677 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          symtab_leave_scope();
      }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 686 "parser.y"
    {
          symtab_enter_scope("main");
      }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 690 "parser.y"
    {
          symtab_leave_scope();
      }
    break;


/* Line 1792 of yacc.c  */
#line 2278 "parser.tab.c"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 699 "parser.y"


static void declare_identifier(char *name)
{
    if (name != NULL) {
        symtab_declare(name);
        free(name);
    }
}

static void discard_text(char *text)
{
    free(text);
}

static void enter_named_scope(const char *kind, const char *name)
{
    size_t size = strlen(kind) + strlen(name) + 3;
    char *label = malloc(size);

    if (label == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    snprintf(label, size, "%s %s", kind, name);
    symtab_enter_scope(label);
    free(label);
}

static void remember_function(char *name)
{
    clear_pending_parameters();
    free(pending_function_name);
    pending_function_name = NULL;

    if (name != NULL) {
        pending_function_name = strdup(name);
        if (pending_function_name == NULL) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
}

static void enter_function_scope(void)
{
    enter_named_scope("function",
                      pending_function_name != NULL ? pending_function_name : "<anonymous>");
}

static void finish_function_scope(void)
{
    symtab_leave_scope();
    free(pending_function_name);
    pending_function_name = NULL;
}

static void remember_parameter(char *name)
{
    PendingParameter *parameter;

    if (name == NULL) {
        return;
    }

    parameter = malloc(sizeof(*parameter));
    if (parameter == NULL) {
        fprintf(stderr, "Out of memory\n");
        free(name);
        exit(EXIT_FAILURE);
    }

    parameter->name = name;
    parameter->next = NULL;

    if (pending_parameters_tail == NULL) {
        pending_parameters = parameter;
    } else {
        pending_parameters_tail->next = parameter;
    }
    pending_parameters_tail = parameter;
}

static void declare_pending_parameters(void)
{
    PendingParameter *parameter = pending_parameters;

    while (parameter != NULL) {
        symtab_declare(parameter->name);
        parameter = parameter->next;
    }

    clear_pending_parameters();
}

static void clear_pending_parameters(void)
{
    PendingParameter *parameter = pending_parameters;

    while (parameter != NULL) {
        PendingParameter *next = parameter->next;
        free(parameter->name);
        free(parameter);
        parameter = next;
    }

    pending_parameters = NULL;
    pending_parameters_tail = NULL;
}

void yyerror(const char *message)
{
    if (!diagnostics_too_many_syntax_errors()) {
        diagnostics_syntax_error(yylineno, message);
    }
}

static void print_usage(const char *program_name)
{
    fprintf(stderr,
            "Usage: %s [-q|--quiet-tokens] <source.cpp>\n"
            "  -q, --quiet-tokens   do not print every token returned by the lexer\n",
            program_name);
}

int main(int argc, char **argv)
{
    const char *filename = NULL;
    int trace_tokens = 1;
    int index;
    int parse_result;
    int success;

    for (index = 1; index < argc; index++) {
        if (strcmp(argv[index], "-q") == 0 ||
            strcmp(argv[index], "--quiet-tokens") == 0) {
            trace_tokens = 0;
        } else if (filename == NULL) {
            filename = argv[index];
        } else {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (filename == NULL) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (diagnostics_load_source(filename) != 0) {
        fprintf(stderr, "Cannot preload source file '%s' for diagnostics.\n", filename);
        return EXIT_FAILURE;
    }

    yyin = fopen(filename, "r");
    if (yyin == NULL) {
        perror(filename);
        diagnostics_free_source();
        return EXIT_FAILURE;
    }

    yylineno = 1;
    yyrestart(yyin);
    lexer_set_trace(trace_tokens);
    symtab_init();

    parse_result = yyparse();

    /* Also closes/prints any still-open scopes after an aborted parse. */
    symtab_finish();

    success = parse_result == 0 &&
              diagnostics_lexical_count() == 0 &&
              diagnostics_syntax_count() == 0;

    printf("\n=== ANALYSIS SUMMARY ===\n");
    printf("Lexical errors : %d\n", diagnostics_lexical_count());
    printf("Syntax errors  : %d\n", diagnostics_syntax_count());
    printf("Result         : %s\n", success ? "ACCEPTED" : "REJECTED");

    clear_pending_parameters();
    free(pending_function_name);
    pending_function_name = NULL;
    fclose(yyin);
    diagnostics_free_source();

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
