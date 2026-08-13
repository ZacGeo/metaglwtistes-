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
#include "ast.h"
#include "semantic.h"
#include "types.h"
#include "parser_types.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;
extern void yyrestart(FILE *input_file);
void lexer_set_trace(int enabled);

void yyerror(const char *message);

static char *pending_function_name = NULL;
static char *pending_method_name = NULL;
static Type *pending_method_owner = NULL;

typedef struct PendingParameter {
    char *name;
    Type *type;
    PassMode mode;
    struct PendingParameter *next;
} PendingParameter;

static PendingParameter *pending_parameters = NULL;
static PendingParameter *pending_parameters_tail = NULL;
static Type *pending_function_return_type = NULL;

static Type *declaration_base_type = NULL;
static long long next_enum_value = 0;

static void declare_enum_value(char *name, AstNode *initializer);
static void discard_text(char *text);
static void enter_named_scope(const char *kind, const char *name);
static void remember_function(char *name);
static void enter_function_scope(void);
static void finish_function_scope(void);
static void remember_parameter(char *name, Type *type, PassMode mode);
static void declare_pending_parameters(void);
static void clear_pending_parameters(void);
static void register_pending_function(int definition);
static int implicit_reference(Type *type);
static DimList *dim_new(long long length);
static DimList *dim_append(DimList *list, DimList *item);
static Type *apply_declarator(Type *base, int is_list, DimList *dimensions, int line);
static void dim_free(DimList *dimensions);
static DeclItem *decl_new(char *name, int is_list, DimList *dimensions, int line);
static DeclItem *decl_append(DeclItem *list, DeclItem *item);
static AstNode *process_declarations(Type *base, DeclItem *items, SymbolKind kind,
                                     StorageClass storage);

/* Line 371 of yacc.c  */
#line 129 "parser.tab.c"

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
#line 65 "parser.y"

    long long ival;
    double fval;
    char *sval;
    Type *type;
    AstNode *node;
    DimList *dims;
    DeclItem *decl;


/* Line 387 of yacc.c  */
#line 243 "parser.tab.c"
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
#line 271 "parser.tab.c"

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
#define YYLAST   736

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  99
/* YYNRULES -- Number of rules.  */
#define YYNRULES  194
/* YYNRULES -- Number of states.  */
#define YYNSTATES  351

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
      46,    48,    50,    51,    54,    55,    59,    62,    63,    69,
      73,    75,    80,    82,    86,    90,    92,    94,    98,   100,
     104,   106,   110,   112,   116,   118,   122,   124,   128,   130,
     133,   136,   139,   142,   144,   147,   149,   154,   159,   161,
     165,   169,   171,   176,   180,   185,   188,   190,   194,   196,
     200,   202,   204,   205,   207,   209,   211,   213,   217,   218,
     224,   228,   232,   237,   240,   243,   244,   245,   251,   256,
     261,   264,   265,   269,   272,   275,   278,   281,   282,   284,
     286,   288,   290,   294,   298,   300,   304,   305,   310,   314,
     318,   321,   323,   325,   327,   330,   333,   338,   342,   346,
     348,   351,   355,   359,   360,   366,   370,   374,   376,   379,
     381,   383,   386,   387,   391,   392,   396,   401,   406,   411,
     414,   418,   420,   424,   429,   433,   437,   441,   444,   446,
     448,   449,   452,   454,   459,   464,   469,   471,   472,   475,
     477,   479,   481,   483,   485,   487,   489,   491,   494,   497,
     499,   502,   505,   511,   519,   520,   527,   528,   539,   541,
     542,   546,   551,   556,   560,   562,   564,   568,   570,   572,
     574,   578,   582,   583,   587
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      62,     0,    -1,    63,   157,    -1,    63,    64,    -1,    -1,
      65,    -1,    71,    -1,    92,    -1,    97,    -1,   120,    -1,
     122,    -1,   125,    -1,     1,    45,    -1,     3,    66,    68,
      58,    69,    45,    -1,    67,    -1,    58,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,    13,    -1,    17,    -1,
      -1,    69,    70,    -1,    -1,    50,    54,    51,    -1,    50,
      51,    -1,    -1,     8,    66,    72,    73,    45,    -1,    73,
      47,    74,    -1,    74,    -1,    58,    69,    48,    75,    -1,
      77,    -1,    52,    76,    53,    -1,    76,    47,    75,    -1,
      75,    -1,    78,    -1,    78,    31,    79,    -1,    79,    -1,
      79,    32,    80,    -1,    80,    -1,    80,    33,    81,    -1,
      81,    -1,    81,    34,    82,    -1,    82,    -1,    82,    35,
      83,    -1,    83,    -1,    83,    36,    84,    -1,    84,    -1,
      37,    84,    -1,    35,    84,    -1,    30,    84,    -1,    38,
      86,    -1,    85,    -1,    86,    38,    -1,    86,    -1,    86,
      43,    89,    44,    -1,    25,    43,    87,    44,    -1,    90,
      -1,    43,    87,    44,    -1,    43,    67,    44,    -1,    91,
      -1,    86,    50,    87,    51,    -1,    86,    46,    58,    -1,
      59,    43,    87,    44,    -1,   139,    58,    -1,    29,    -1,
      87,    47,    88,    -1,    88,    -1,    86,    48,    88,    -1,
      77,    -1,    87,    -1,    -1,    56,    -1,    54,    -1,    55,
      -1,    57,    -1,    50,    89,    51,    -1,    -1,    16,    58,
      93,    94,    45,    -1,    52,    95,    53,    -1,    52,     1,
      53,    -1,    95,    47,    58,    96,    -1,    58,    96,    -1,
      48,    75,    -1,    -1,    -1,     9,    58,    98,    99,    45,
      -1,   100,    52,   101,    53,    -1,   100,    52,     1,    53,
      -1,    49,    58,    -1,    -1,   101,   102,   103,    -1,   102,
     103,    -1,    10,    49,    -1,    11,    49,    -1,    12,    49,
      -1,    -1,   104,    -1,   113,    -1,   105,    -1,   108,    -1,
      66,   106,    45,    -1,   106,    47,   107,    -1,   107,    -1,
      68,    58,    69,    -1,    -1,    15,   109,   110,    45,    -1,
      52,   111,    53,    -1,    52,     1,    53,    -1,   111,   112,
      -1,   112,    -1,   105,    -1,   114,    -1,   115,    45,    -1,
     119,    45,    -1,   116,    43,   117,    44,    -1,    66,    68,
      58,    -1,   117,    47,   118,    -1,   118,    -1,    66,    42,
      -1,    66,    68,    69,    -1,   116,    43,    44,    -1,    -1,
      15,    58,   121,   110,    45,    -1,    66,   123,    45,    -1,
     123,    47,   124,    -1,   124,    -1,   107,    96,    -1,   114,
      -1,   126,    -1,   129,   135,    -1,    -1,   134,   127,   135,
      -1,    -1,   119,   128,   135,    -1,   130,    43,   132,    44,
      -1,   116,    43,   132,    44,    -1,    66,    68,   131,    58,
      -1,    58,    39,    -1,   132,    47,   133,    -1,   133,    -1,
      66,    42,    58,    -1,    66,    68,    58,    69,    -1,   130,
      43,    44,    -1,    52,   136,    53,    -1,    52,     1,    53,
      -1,   137,   140,    -1,   137,    -1,   140,    -1,    -1,   137,
     138,    -1,   138,    -1,   139,    66,   106,    45,    -1,   139,
      66,     1,    45,    -1,   139,    66,   106,     1,    -1,    14,
      -1,    -1,   140,   141,    -1,   141,    -1,   142,    -1,   143,
      -1,   144,    -1,   146,    -1,   149,    -1,   150,    -1,   156,
      -1,    18,    45,    -1,    19,    45,    -1,    45,    -1,     1,
      45,    -1,    87,    45,    -1,    20,    43,    87,    44,   141,
      -1,    20,    43,    87,    44,   141,    21,   141,    -1,    -1,
      22,    43,    87,    44,   145,   141,    -1,    -1,    23,    43,
     148,    45,   148,    45,   148,    44,   147,   141,    -1,    87,
      -1,    -1,    24,   148,    45,    -1,    26,    40,   151,    45,
      -1,    27,    41,   153,    45,    -1,   151,    40,   152,    -1,
     152,    -1,    86,    -1,   153,    41,   154,    -1,   154,    -1,
      87,    -1,    52,    -1,   155,   136,    53,    -1,   155,     1,
      53,    -1,    -1,   159,   158,   135,    -1,     5,    28,    43,
      44,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   118,   118,   122,   123,   127,   128,   129,   130,   131,
     132,   133,   134,   142,   152,   153,   161,   162,   163,   164,
     165,   169,   170,   174,   175,   179,   180,   185,   184,   190,
     191,   195,   210,   211,   215,   216,   222,   226,   228,   232,
     234,   238,   240,   244,   246,   250,   252,   256,   258,   262,
     264,   266,   267,   269,   273,   275,   276,   277,   279,   280,
     281,   283,   287,   289,   294,   299,   304,   308,   309,   313,
     314,   318,   319,   323,   328,   329,   330,   338,   344,   343,
     361,   362,   370,   374,   381,   382,   387,   386,   401,   402,
     410,   421,   425,   426,   430,   431,   432,   433,   437,   438,
     442,   443,   447,   452,   453,   457,   466,   466,   471,   472,
     480,   481,   485,   489,   493,   499,   508,   512,   521,   522,
     526,   528,   538,   543,   542,   558,   563,   564,   568,   573,
     574,   578,   589,   588,   604,   603,   621,   627,   636,   663,
     670,   671,   675,   679,   689,   693,   695,   704,   705,   706,
     707,   711,   712,   716,   721,   727,   739,   740,   744,   745,
     749,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,   768,   773,   779,   790,   789,   802,   801,   815,   816,
     820,   824,   826,   831,   832,   836,   844,   845,   849,   853,
     860,   866,   877,   876,   896
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
  "listspec", "dims", "dim", "const_declaration", "$@1", "constdefs",
  "constdef", "init_value", "init_values", "expression", "or_expression",
  "and_expression", "equality_expression", "relational_expression",
  "additive_expression", "multiplicative_expression", "unary_expression",
  "postfix_expression", "variable", "general_expression", "assignment",
  "expression_list", "constant", "listexpression", "enum_declaration",
  "$@2", "enum_body", "id_list", "initializer", "class_declaration", "$@3",
  "class_body", "parent", "members_methods", "access", "member_or_method",
  "member", "var_declaration", "variabledefs", "variabledef",
  "anonymous_union", "$@4", "union_body", "fields", "field", "method",
  "short_func_declaration", "short_par_func_header", "func_header_start",
  "parameter_types", "prototype_parameter", "nopar_func_header",
  "union_declaration", "$@5", "global_var_declaration",
  "init_variabledefs", "init_variabledef", "func_declaration",
  "full_func_declaration", "$@6", "$@7", "full_par_func_header",
  "class_func_header_start", "func_class", "parameter_list",
  "named_parameter", "nopar_class_func_header", "function_body",
  "decl_statements", "declarations", "local_declaration", "decltype",
  "statements", "statement", "expression_statement", "if_statement",
  "while_statement", "$@8", "for_statement", "$@9", "optexpr",
  "return_statement", "io_statement", "in_list", "in_item", "out_list",
  "out_item", "compound_open", "comp_statement", "main_function", "$@10",
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
      67,    68,    68,    69,    69,    70,    70,    72,    71,    73,
      73,    74,    75,    75,    76,    76,    77,    78,    78,    79,
      79,    80,    80,    81,    81,    82,    82,    83,    83,    84,
      84,    84,    84,    84,    85,    85,    85,    85,    85,    85,
      85,    85,    86,    86,    86,    86,    86,    87,    87,    88,
      88,    89,    89,    90,    90,    90,    90,    91,    93,    92,
      94,    94,    95,    95,    96,    96,    98,    97,    99,    99,
     100,   100,   101,   101,   102,   102,   102,   102,   103,   103,
     104,   104,   105,   106,   106,   107,   109,   108,   110,   110,
     111,   111,   112,   113,   114,   114,   115,   116,   117,   117,
     118,   118,   119,   121,   120,   122,   123,   123,   124,   125,
     125,   126,   127,   126,   128,   126,   129,   129,   130,   131,
     132,   132,   133,   133,   134,   135,   135,   136,   136,   136,
     136,   137,   137,   138,   138,   138,   139,   139,   140,   140,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   142,   143,   143,   145,   144,   147,   146,   148,   148,
     149,   150,   150,   151,   151,   152,   153,   153,   154,   155,
     156,   156,   158,   157,   159
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     2,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     2,     0,     3,     2,     0,     5,     3,
       1,     4,     1,     3,     3,     1,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     2,
       2,     2,     2,     1,     2,     1,     4,     4,     1,     3,
       3,     1,     4,     3,     4,     2,     1,     3,     1,     3,
       1,     1,     0,     1,     1,     1,     1,     3,     0,     5,
       3,     3,     4,     2,     2,     0,     0,     5,     4,     4,
       2,     0,     3,     2,     2,     2,     2,     0,     1,     1,
       1,     1,     3,     3,     1,     3,     0,     4,     3,     3,
       2,     1,     1,     1,     2,     2,     4,     3,     3,     1,
       2,     3,     3,     0,     5,     3,     3,     1,     2,     1,
       1,     2,     0,     3,     0,     3,     4,     4,     4,     2,
       3,     1,     3,     4,     3,     3,     3,     2,     1,     1,
       0,     2,     1,     4,     4,     4,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       2,     2,     5,     7,     0,     6,     0,    10,     1,     0,
       3,     4,     4,     3,     1,     1,     3,     1,     1,     1,
       3,     3,     0,     3,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     1,     0,     0,    16,    17,    18,    19,
       0,     0,    20,     0,     0,    15,     3,     5,    22,    14,
       6,     7,     8,   129,     0,     0,   134,     9,    10,    11,
     130,     0,     0,   132,     2,   192,    12,    17,    22,     0,
      27,    86,   123,    78,    21,     0,    85,     0,   127,   114,
       0,   115,     0,     0,   131,     0,     0,     0,     0,     0,
       0,    91,     0,     0,    24,     0,   157,   128,   125,    22,
     122,    22,     0,   119,     0,   141,   135,     0,   156,     0,
       0,     0,     0,     0,   157,     0,     0,     0,    66,   157,
     157,   157,   157,   157,   169,    72,   189,    74,    75,    73,
      76,     0,    70,    36,    38,    40,    42,    44,    46,    48,
      53,    55,     0,    68,    58,    61,     0,     0,   152,     0,
       0,   159,   160,   161,   162,   163,   164,   165,     0,   166,
     144,    22,     0,   133,   193,    24,   194,    24,     0,    30,
       0,     0,     0,     0,     0,     0,     0,   139,   105,   138,
     157,    84,    32,    55,     0,     0,   126,   120,    24,   116,
       0,   137,     0,   170,   146,   167,   168,   157,   157,   157,
     178,     0,   157,   157,   157,    51,    50,    49,    52,     0,
       0,    71,     0,   157,   157,   157,   157,   157,   157,   157,
      54,    72,     0,   157,   157,   171,   157,   145,     0,   151,
       0,    65,     0,   158,     0,     0,     0,     0,   136,     0,
       0,    28,     0,    90,    87,     0,     0,    22,   112,     0,
     111,   124,     0,    85,     0,    79,     0,    23,    35,     0,
      65,    24,   142,    24,   121,    22,   118,   140,     0,     0,
       0,   180,     0,   185,     0,   184,   188,     0,   187,    60,
      59,    77,     0,    37,    39,    41,    43,    45,    47,     0,
      63,    69,     0,    67,     0,     0,   104,   191,   190,    13,
     157,    29,     0,     0,     0,     0,    97,     0,   109,     0,
     108,   110,    81,    83,     0,    80,    26,     0,   157,    33,
     143,   120,    24,     0,   174,   157,    57,   157,   181,   157,
     182,    64,    56,    62,   154,   155,   153,    22,    31,    89,
      94,    95,    96,    88,     0,   106,    22,    93,    98,   100,
     101,    99,   113,     0,     0,   102,    85,    25,    34,   172,
       0,     0,   183,   186,   103,    92,     0,     0,     0,    82,
       0,   175,   157,     0,    24,   173,     0,   107,   176,     0,
     177
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    16,    17,   131,    19,   155,   148,   227,
      20,    60,   138,   139,   151,   229,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   182,   114,
     115,    21,    63,   146,   224,    67,    22,    61,   141,   142,
     276,   277,   317,   318,   218,   279,   266,   320,   336,   144,
     219,   220,   321,   322,    24,   323,    72,    73,   324,    27,
      62,    28,    47,    48,    29,    30,    56,    52,    31,    32,
      65,    74,    75,    33,    54,   116,   117,   118,   154,   120,
     121,   122,   123,   124,   330,   125,   349,   171,   126,   127,
     244,   245,   247,   248,   128,   129,    34,    57,    35
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -214
static const yytype_int16 yypact[] =
{
    -214,    63,    21,  -214,    47,   180,  -214,    85,  -214,  -214,
     180,    62,  -214,    70,    92,  -214,  -214,  -214,   138,  -214,
    -214,  -214,  -214,  -214,   117,   113,   144,  -214,  -214,  -214,
    -214,   142,   169,  -214,  -214,  -214,  -214,  -214,   138,   179,
    -214,  -214,  -214,  -214,  -214,   177,   197,   -12,  -214,  -214,
      65,  -214,   142,   263,  -214,   147,   142,   142,   190,   208,
     213,   217,   232,   243,   104,   238,   543,  -214,  -214,   138,
    -214,     0,    38,  -214,   129,  -214,  -214,    41,  -214,   258,
     265,   261,   268,   271,   576,   280,   284,   285,  -214,   677,
     677,   677,    67,   510,  -214,   609,  -214,  -214,  -214,  -214,
    -214,   289,  -214,   296,   301,   302,   300,   304,   305,  -214,
    -214,   259,    95,  -214,  -214,  -214,   287,   324,  -214,   236,
     145,  -214,  -214,  -214,  -214,  -214,  -214,  -214,   383,  -214,
    -214,    56,   134,  -214,  -214,  -214,  -214,  -214,   176,  -214,
     294,   310,   293,    61,   311,    22,   312,  -214,   308,  -214,
     543,  -214,  -214,   201,   306,   307,  -214,   313,   314,  -214,
     180,  -214,   180,  -214,  -214,  -214,  -214,   677,   677,   576,
     316,   315,   677,    67,   677,  -214,  -214,  -214,   111,   322,
     166,   316,   317,   677,   677,   677,   677,   677,   677,   677,
    -214,   609,   327,   677,   677,  -214,   677,  -214,   325,  -214,
     429,    42,    44,  -214,    52,   320,   313,   314,  -214,    79,
     224,  -214,   213,  -214,  -214,    45,   333,   138,  -214,   202,
    -214,  -214,   338,   197,    74,  -214,    -7,  -214,  -214,    78,
    -214,  -214,  -214,  -214,   308,    66,  -214,  -214,   206,   210,
     330,  -214,   212,   111,    99,  -214,   316,   151,  -214,  -214,
    -214,  -214,   214,   301,   302,   300,   304,   305,  -214,   348,
    -214,  -214,    98,  -214,   349,    30,  -214,  -214,  -214,  -214,
     543,  -214,   340,   346,   350,   351,   220,   221,  -214,   233,
    -214,  -214,  -214,  -214,   353,  -214,  -214,   347,   543,  -214,
     308,  -214,  -214,   475,  -214,   576,  -214,    67,  -214,   677,
    -214,  -214,  -214,  -214,  -214,  -214,  -214,   138,  -214,  -214,
    -214,  -214,  -214,  -214,   221,  -214,   138,  -214,  -214,  -214,
    -214,  -214,  -214,   361,   144,  -214,   197,  -214,  -214,   393,
     475,   370,  -214,  -214,  -214,  -214,   232,   358,    65,  -214,
     475,  -214,   644,   372,   376,  -214,   378,  -214,  -214,   475,
    -214
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -214,  -214,  -214,  -214,  -214,    -2,   331,   -17,  -126,  -214,
    -214,  -214,  -214,   211,  -140,  -214,   -59,  -214,   241,   242,
     245,   247,   244,   -71,  -214,   -51,   -79,    69,   253,  -214,
    -214,  -214,  -214,  -214,  -214,  -210,  -214,  -214,  -214,  -214,
    -214,   153,   131,  -214,  -213,   248,   -16,  -214,  -214,   110,
    -214,   246,  -214,   455,  -214,   458,  -214,   303,   459,  -214,
    -214,  -214,  -214,   399,  -214,  -214,  -214,  -214,  -214,  -214,
    -214,   407,   309,  -214,    54,   341,  -214,   356,   -41,   360,
    -116,  -214,  -214,  -214,  -214,  -214,  -214,  -163,  -214,  -214,
    -214,   173,  -214,   181,  -214,  -214,  -214,  -214,  -214
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -180
static const yytype_int16 yytable[] =
{
      18,    45,    46,    38,   203,   170,   240,   152,    40,   209,
     228,   210,   119,   283,   180,   153,   181,    44,   175,   176,
     177,    58,     4,   222,     5,     6,     7,     8,     9,    10,
      11,   305,   234,    68,    12,    69,    13,    14,   153,   153,
     153,   178,   157,   -15,   286,   264,   272,   287,    71,   -97,
     -97,   -97,   -97,    46,   158,   273,   274,   275,   -97,   -15,
     -97,    44,   216,     3,   319,     6,    37,     8,     9,     6,
      37,     8,     9,    44,    12,   306,   119,   307,    12,    15,
     223,    78,   159,    44,   203,   160,   163,   119,   238,   239,
     170,   152,    36,   242,   164,   246,    88,   163,   206,   153,
     -15,   319,   -22,   -97,   252,   267,    76,   290,   291,    70,
     133,   134,   181,    39,   207,   262,   339,   202,   258,    15,
      41,   284,   243,    15,   269,   288,   101,   285,    42,   226,
     308,   289,   331,   153,   153,   153,   153,   153,   153,   297,
     195,   217,   196,   147,   298,   196,   198,  -117,   328,   303,
      43,     6,    37,     8,     9,    44,    50,   192,   235,    78,
      12,   194,    49,    79,    80,    81,   234,    82,    83,    84,
      85,    86,    87,   161,    88,    89,   162,   329,   208,   346,
      90,   162,    91,    92,     6,    37,     8,     9,    93,    51,
      94,   130,   299,    12,    53,    95,   300,    96,  -149,    97,
      98,    99,   100,  -157,   101,    15,     6,    37,     8,     9,
     250,   152,    55,   196,   341,    12,   170,   217,   292,   153,
     246,   211,    59,   212,   345,     6,    37,     8,     9,   152,
     273,   274,   275,   350,    12,    64,   315,   153,    15,   190,
       6,    37,     8,     9,   191,    66,   243,   192,   135,    12,
     293,   194,   136,   196,   294,   280,   296,   196,   301,   196,
      15,   196,   261,   170,    77,   263,   140,  -157,  -157,  -157,
    -157,   137,   270,   313,   226,   316,  -157,    78,   325,    15,
     307,    79,    80,    81,   143,    82,    83,    84,    85,    86,
      87,   334,    88,    89,   201,   145,   149,   190,    90,   337,
      91,    92,   191,   165,   167,   192,    93,   193,    94,   194,
     166,   168,   316,    95,   169,    96,  -150,    97,    98,    99,
     100,  -157,   101,   172,   173,   198,   174,   184,  -157,  -157,
    -157,  -157,   183,   185,   187,   186,   235,  -157,    78,   188,
     197,   189,    79,    80,    81,   215,    82,    83,    84,    85,
      86,    87,   213,    88,    89,   214,   221,   225,   226,    90,
     241,    91,    92,   196,   230,   231,   249,    93,   251,    94,
     163,   232,   233,   268,    95,   295,    96,  -148,    97,    98,
      99,   100,  -157,   101,   204,   260,   278,  -157,  -157,  -157,
    -157,   282,   302,   309,   304,   310,  -157,    78,   327,   311,
     312,    79,    80,    81,   338,    82,    83,    84,    85,    86,
      87,   326,    88,    89,   340,   342,   344,   347,    90,  -117,
      91,    92,   348,   271,   179,   253,    93,   254,    94,   314,
     198,   255,   257,    95,   256,    96,  -150,    97,    98,    99,
     100,  -157,   101,    78,   259,   335,   343,    79,    80,    81,
     265,    82,    83,    84,    85,    86,    87,    23,    88,    89,
      25,    26,   132,   236,    90,   281,    91,    92,   156,   205,
     332,   237,    93,   199,    94,     0,   198,   200,     0,    95,
     333,    96,  -147,    97,    98,    99,   100,  -157,   101,    78,
       0,     0,     0,    79,    80,    81,     0,    82,    83,    84,
      85,    86,    87,     0,    88,    89,     0,     0,     0,     0,
      90,     0,    91,    92,     6,    37,     8,     9,    93,     0,
      94,     0,     0,    12,    78,    95,     0,    96,     0,    97,
      98,    99,   100,  -157,   101,    85,     0,     0,     0,    88,
      89,     0,     0,     0,     0,    90,     0,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,    78,     0,     0,
      95,     0,     0,     0,    97,    98,    99,   100,    85,   101,
       0,     0,    88,    89,     0,     0,     0,     0,    90,     0,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
      78,     0,     0,    95,     0,   150,     0,    97,    98,    99,
     100,    85,   101,     0,     0,    88,    89,     0,     0,     0,
       0,    90,     0,    91,    92,     0,     0,     0,     0,    93,
       0,  -179,     0,    78,     0,     0,    95,     0,     0,     0,
      97,    98,    99,   100,    85,   101,     0,     0,    88,    89,
       0,     0,     0,     0,    90,     0,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,    78,    95,
       0,     0,     0,    97,    98,    99,   100,  -157,   101,    85,
       0,     0,     0,    88,    89,     0,     0,     0,     0,    90,
       0,    91,    92,     0,     0,     0,     0,    93,  -179,     0,
       0,    78,     0,     0,    95,     0,     0,     0,    97,    98,
      99,   100,    85,   101,     0,     0,    88,    89,     0,     0,
       0,     0,    90,     0,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,    97,    98,    99,   100,     0,   101
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-214)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,    18,    18,     5,   120,    84,   169,    66,    10,   135,
     150,   137,    53,   223,    93,    66,    95,    17,    89,    90,
      91,    38,     1,     1,     3,     4,     5,     6,     7,     8,
       9,     1,   158,    45,    13,    47,    15,    16,    89,    90,
      91,    92,    42,     1,    51,     1,     1,    54,    50,     4,
       5,     6,     7,    69,    71,    10,    11,    12,    13,    17,
      15,    17,     1,     0,   277,     4,     5,     6,     7,     4,
       5,     6,     7,    17,    13,    45,   117,    47,    13,    58,
      58,    14,    44,    17,   200,    47,    45,   128,   167,   168,
     169,   150,    45,   172,    53,   174,    29,    45,    42,   150,
      58,   314,    58,    58,   183,    53,    52,   233,    42,    44,
      56,    57,   191,    28,   131,   194,   326,   119,   189,    58,
      58,    47,   173,    58,    45,    47,    59,    53,    58,    50,
     270,    53,   295,   184,   185,   186,   187,   188,   189,    40,
      45,   143,    47,    39,    45,    47,     1,    43,   288,    51,
      58,     4,     5,     6,     7,    17,    43,    46,   160,    14,
      13,    50,    45,    18,    19,    20,   292,    22,    23,    24,
      25,    26,    27,    44,    29,    30,    47,   293,    44,   342,
      35,    47,    37,    38,     4,     5,     6,     7,    43,    45,
      45,    44,    41,    13,    52,    50,    45,    52,    53,    54,
      55,    56,    57,    58,    59,    58,     4,     5,     6,     7,
      44,   270,    43,    47,   330,    13,   295,   219,   235,   270,
     299,    45,    43,    47,   340,     4,     5,     6,     7,   288,
      10,    11,    12,   349,    13,    58,    15,   288,    58,    38,
       4,     5,     6,     7,    43,    48,   297,    46,    58,    13,
      44,    50,    44,    47,    44,    53,    44,    47,    44,    47,
      58,    47,   193,   342,     1,   196,    49,     4,     5,     6,
       7,    58,    48,    53,    50,   277,    13,    14,    45,    58,
      47,    18,    19,    20,    52,    22,    23,    24,    25,    26,
      27,   307,    29,    30,    58,    52,    58,    38,    35,   316,
      37,    38,    43,    45,    43,    46,    43,    48,    45,    50,
      45,    43,   314,    50,    43,    52,    53,    54,    55,    56,
      57,    58,    59,    43,    40,     1,    41,    31,     4,     5,
       6,     7,    43,    32,    34,    33,   338,    13,    14,    35,
      53,    36,    18,    19,    20,    52,    22,    23,    24,    25,
      26,    27,    58,    29,    30,    45,    45,    45,    50,    35,
      45,    37,    38,    47,    58,    58,    44,    43,    51,    45,
      45,    58,    58,    53,    50,    45,    52,    53,    54,    55,
      56,    57,    58,    59,     1,    58,    53,     4,     5,     6,
       7,    53,    44,    53,    45,    49,    13,    14,    51,    49,
      49,    18,    19,    20,    43,    22,    23,    24,    25,    26,
      27,    58,    29,    30,    21,    45,    58,    45,    35,    43,
      37,    38,    44,   212,    93,   184,    43,   185,    45,   276,
       1,   186,   188,    50,   187,    52,    53,    54,    55,    56,
      57,    58,    59,    14,   191,   314,   336,    18,    19,    20,
     202,    22,    23,    24,    25,    26,    27,     2,    29,    30,
       2,     2,    55,   160,    35,   219,    37,    38,    69,   128,
     297,   162,    43,   117,    45,    -1,     1,   117,    -1,    50,
     299,    52,    53,    54,    55,    56,    57,    58,    59,    14,
      -1,    -1,    -1,    18,    19,    20,    -1,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,     4,     5,     6,     7,    43,    -1,
      45,    -1,    -1,    13,    14,    50,    -1,    52,    -1,    54,
      55,    56,    57,    58,    59,    25,    -1,    -1,    -1,    29,
      30,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    14,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    56,    57,    25,    59,
      -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      14,    -1,    -1,    50,    -1,    52,    -1,    54,    55,    56,
      57,    25,    59,    -1,    -1,    29,    30,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,
      -1,    45,    -1,    14,    -1,    -1,    50,    -1,    -1,    -1,
      54,    55,    56,    57,    25,    59,    -1,    -1,    29,    30,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    14,    50,
      -1,    -1,    -1,    54,    55,    56,    57,    58,    59,    25,
      -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      -1,    14,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      56,    57,    25,    59,    -1,    -1,    29,    30,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    59
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    62,    63,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    13,    15,    16,    58,    64,    65,    66,    67,
      71,    92,    97,   114,   115,   116,   119,   120,   122,   125,
     126,   129,   130,   134,   157,   159,    45,     5,    66,    28,
      66,    58,    58,    58,    17,    68,   107,   123,   124,    45,
      43,    45,   128,    52,   135,    43,   127,   158,    68,    43,
      72,    98,   121,    93,    58,   131,    48,    96,    45,    47,
      44,    66,   117,   118,   132,   133,   135,     1,    14,    18,
      19,    20,    22,    23,    24,    25,    26,    27,    29,    30,
      35,    37,    38,    43,    45,    50,    52,    54,    55,    56,
      57,    59,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    90,    91,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   146,   149,   150,   155,   156,
      44,    66,   132,   135,   135,    58,    44,    58,    73,    74,
      49,    99,   100,    52,   110,    52,    94,    39,    69,    58,
      52,    75,    77,    86,   139,    68,   124,    42,    68,    44,
      47,    44,    47,    45,    53,    45,    45,    43,    43,    43,
      87,   148,    43,    40,    41,    84,    84,    84,    86,    67,
      87,    87,    89,    43,    31,    32,    33,    34,    35,    36,
      38,    43,    46,    48,    50,    45,    47,    53,     1,   138,
     140,    58,    66,   141,     1,   136,    42,    68,    44,    69,
      69,    45,    47,    58,    45,    52,     1,    66,   105,   111,
     112,    45,     1,    58,    95,    45,    50,    70,    75,    76,
      58,    58,    58,    58,    69,    66,   118,   133,    87,    87,
     148,    45,    87,    86,   151,   152,    87,   153,   154,    44,
      44,    51,    87,    79,    80,    81,    82,    83,    84,    89,
      58,    88,    87,    88,     1,   106,   107,    53,    53,    45,
      48,    74,     1,    10,    11,    12,   101,   102,    53,   106,
      53,   112,    53,    96,    47,    53,    51,    54,    47,    53,
      69,    42,    68,    44,    44,    45,    44,    40,    45,    41,
      45,    44,    44,    51,    45,     1,    45,    47,    75,    53,
      49,    49,    49,    53,   102,    15,    66,   103,   104,   105,
     108,   113,   114,   116,   119,    45,    58,    51,    75,   141,
     145,   148,   152,   154,   107,   103,   109,    68,    43,    96,
      21,   141,    45,   110,    58,   141,   148,    45,    44,   147,
     141
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
      case 31: /* OROP */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1610 "parser.tab.c"
        break;
      case 32: /* ANDOP */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1617 "parser.tab.c"
        break;
      case 33: /* EQUOP */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1624 "parser.tab.c"
        break;
      case 34: /* RELOP */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1631 "parser.tab.c"
        break;
      case 35: /* ADDOP */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1638 "parser.tab.c"
        break;
      case 36: /* MULOP */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1645 "parser.tab.c"
        break;
      case 37: /* NOTOP */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1652 "parser.tab.c"
        break;
      case 38: /* INCDEC */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1659 "parser.tab.c"
        break;
      case 56: /* CCONST */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1666 "parser.tab.c"
        break;
      case 57: /* SCONST */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1673 "parser.tab.c"
        break;
      case 58: /* ID */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1680 "parser.tab.c"
        break;
      case 59: /* LISTFUNC */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1687 "parser.tab.c"
        break;
      case 131: /* func_class */
/* Line 1398 of yacc.c  */
#line 109 "parser.y"
        { free(((*yyvaluep).sval)); };
/* Line 1398 of yacc.c  */
#line 1694 "parser.tab.c"
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
#line 135 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 143 "parser.y"
    {
          Type *alias = apply_declarator((yyvsp[(2) - (6)].type), (int)(yyvsp[(3) - (6)].ival), (yyvsp[(5) - (6)].dims), yylineno);
          semantic_declare((yyvsp[(4) - (6)].sval), SYMBOL_TYPE, alias, STORAGE_NONE, yylineno, 1);
          dim_free((yyvsp[(5) - (6)].dims));
          discard_text((yyvsp[(4) - (6)].sval));
      }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 154 "parser.y"
    {
          (yyval.type) = semantic_resolve_type((yyvsp[(1) - (1)].sval), yylineno);
          discard_text((yyvsp[(1) - (1)].sval));
      }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 161 "parser.y"
    { (yyval.type) = type_char(); }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 162 "parser.y"
    { (yyval.type) = type_int(); }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 163 "parser.y"
    { (yyval.type) = type_float(); }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 164 "parser.y"
    { (yyval.type) = type_string(); }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 165 "parser.y"
    { (yyval.type) = type_void(); }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 169 "parser.y"
    { (yyval.ival) = 1; }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 170 "parser.y"
    { (yyval.ival) = 0; }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 174 "parser.y"
    { (yyval.dims) = dim_append((yyvsp[(1) - (2)].dims), (yyvsp[(2) - (2)].dims)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 175 "parser.y"
    { (yyval.dims) = NULL; }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 179 "parser.y"
    { (yyval.dims) = dim_new((yyvsp[(2) - (3)].ival)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 180 "parser.y"
    { (yyval.dims) = dim_new(-1); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 185 "parser.y"
    { declaration_base_type = (yyvsp[(2) - (2)].type); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 196 "parser.y"
    {
          Type *type = apply_declarator(declaration_base_type, 0, (yyvsp[(2) - (4)].dims), yylineno);
          Symbol *symbol;
          semantic_validate_initializer(type, (yyvsp[(4) - (4)].node), (yyvsp[(1) - (4)].sval), yylineno);
          symbol = semantic_declare((yyvsp[(1) - (4)].sval), SYMBOL_CONSTANT, type,
                                    STORAGE_NONE, yylineno, 1);
          semantic_store_constant(symbol, (yyvsp[(4) - (4)].node), yylineno);
          dim_free((yyvsp[(2) - (4)].dims));
          ast_free((yyvsp[(4) - (4)].node));
          discard_text((yyvsp[(1) - (4)].sval));
      }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 211 "parser.y"
    { (yyval.node) = ast_init_list((yyvsp[(2) - (3)].node), yylineno); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 215 "parser.y"
    { (yyval.node) = ast_append((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 216 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 222 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 227 "parser.y"
    { (yyval.node) = semantic_binary((yyvsp[(2) - (3)].sval), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); free((yyvsp[(2) - (3)].sval)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 228 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 233 "parser.y"
    { (yyval.node) = semantic_binary((yyvsp[(2) - (3)].sval), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); free((yyvsp[(2) - (3)].sval)); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 234 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 239 "parser.y"
    { (yyval.node) = semantic_binary((yyvsp[(2) - (3)].sval), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); free((yyvsp[(2) - (3)].sval)); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 240 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 245 "parser.y"
    { (yyval.node) = semantic_binary((yyvsp[(2) - (3)].sval), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); free((yyvsp[(2) - (3)].sval)); }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 246 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 251 "parser.y"
    { (yyval.node) = semantic_binary((yyvsp[(2) - (3)].sval), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); free((yyvsp[(2) - (3)].sval)); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 252 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 257 "parser.y"
    { (yyval.node) = semantic_binary((yyvsp[(2) - (3)].sval), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); free((yyvsp[(2) - (3)].sval)); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 258 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 263 "parser.y"
    { (yyval.node) = semantic_unary((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].node), yylineno); free((yyvsp[(1) - (2)].sval)); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 265 "parser.y"
    { (yyval.node) = semantic_unary((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].node), yylineno); free((yyvsp[(1) - (2)].sval)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 266 "parser.y"
    { (yyval.node) = semantic_unary("sizeof", (yyvsp[(2) - (2)].node), yylineno); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 268 "parser.y"
    { (yyval.node) = semantic_unary((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].node), yylineno); free((yyvsp[(1) - (2)].sval)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 269 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 274 "parser.y"
    { (yyval.node) = semantic_unary((yyvsp[(2) - (2)].sval), (yyvsp[(1) - (2)].node), yylineno); free((yyvsp[(2) - (2)].sval)); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 275 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 276 "parser.y"
    { (yyval.node) = semantic_call((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node), yylineno); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 278 "parser.y"
    { (yyval.node) = semantic_length((yyvsp[(3) - (4)].node), yylineno); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 279 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 280 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 282 "parser.y"
    { (yyval.node) = ast_new(AST_EMPTY, yylineno, "type-expression"); (yyval.node)->type = (yyvsp[(2) - (3)].type); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 283 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 288 "parser.y"
    { (yyval.node) = semantic_index((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node), yylineno); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 290 "parser.y"
    {
          (yyval.node) = semantic_member((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].sval), yylineno);
          discard_text((yyvsp[(3) - (3)].sval));
      }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 295 "parser.y"
    {
          (yyval.node) = semantic_list_function((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].node), yylineno);
          discard_text((yyvsp[(1) - (4)].sval));
      }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 300 "parser.y"
    {
          (yyval.node) = semantic_identifier((yyvsp[(2) - (2)].sval), yylineno);
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 304 "parser.y"
    { (yyval.node) = semantic_this(yylineno); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 308 "parser.y"
    { (yyval.node) = ast_append((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 309 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 313 "parser.y"
    { (yyval.node) = semantic_assignment((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 314 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 318 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 319 "parser.y"
    { (yyval.node) = NULL; }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 324 "parser.y"
    {
          (yyval.node) = ast_text(AST_CHAR, (yyvsp[(1) - (1)].sval), type_char(), yylineno);
          discard_text((yyvsp[(1) - (1)].sval));
      }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 328 "parser.y"
    { (yyval.node) = ast_int((yyvsp[(1) - (1)].ival), yylineno); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 329 "parser.y"
    { (yyval.node) = ast_float((yyvsp[(1) - (1)].fval), yylineno); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 331 "parser.y"
    {
          (yyval.node) = semantic_string_literal((yyvsp[(1) - (1)].sval), yylineno);
          discard_text((yyvsp[(1) - (1)].sval));
      }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 339 "parser.y"
    { (yyval.node) = semantic_list_literal((yyvsp[(2) - (3)].node), yylineno); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 344 "parser.y"
    {
          Type *type = type_named(TYPE_ENUM, (yyvsp[(2) - (2)].sval));
          semantic_declare((yyvsp[(2) - (2)].sval), SYMBOL_TYPE, type, STORAGE_NONE, yylineno, 1);
          enter_named_scope("enum", (yyvsp[(2) - (2)].sval));
          symtab_set_scope_type(symtab_current_scope(), type);
          symtab_set_current_access(ACCESS_PUBLIC);
          declaration_base_type = type;
          next_enum_value = 0;
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 355 "parser.y"
    {
          symtab_leave_scope();
      }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 363 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 371 "parser.y"
    {
          declare_enum_value((yyvsp[(3) - (4)].sval), (yyvsp[(4) - (4)].node));
      }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 375 "parser.y"
    {
          declare_enum_value((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].node));
      }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 381 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (2)].node); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 382 "parser.y"
    { (yyval.node) = NULL; }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 387 "parser.y"
    {
          Type *type = type_named(TYPE_CLASS, (yyvsp[(2) - (2)].sval));
          semantic_declare((yyvsp[(2) - (2)].sval), SYMBOL_TYPE, type, STORAGE_NONE, yylineno, 1);
          enter_named_scope("class", (yyvsp[(2) - (2)].sval));
          symtab_set_scope_type(symtab_current_scope(), type);
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 395 "parser.y"
    {
          symtab_leave_scope();
      }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 403 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 411 "parser.y"
    {
          Type *derived = symtab_current_owner_type();
          Type *base = semantic_resolve_type((yyvsp[(2) - (2)].sval), yylineno);
          if (!derived || derived->kind != TYPE_CLASS || base->kind != TYPE_CLASS)
              semantic_error(yylineno, "class inheritance requires a class base type");
          if (derived == base || type_is_derived_from(base, derived))
              semantic_error(yylineno, "cyclic class inheritance involving '%s'", derived->name);
          symtab_set_class_parent(derived, base, yylineno);
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 430 "parser.y"
    { symtab_set_current_access(ACCESS_PRIVATE); }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 431 "parser.y"
    { symtab_set_current_access(ACCESS_PROTECTED); }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 432 "parser.y"
    { symtab_set_current_access(ACCESS_PUBLIC); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 448 "parser.y"
    { ast_free(process_declarations((yyvsp[(1) - (3)].type), (yyvsp[(2) - (3)].decl), SYMBOL_FIELD, STORAGE_MEMBER)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 452 "parser.y"
    { (yyval.decl) = decl_append((yyvsp[(1) - (3)].decl), (yyvsp[(3) - (3)].decl)); }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 453 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (1)].decl); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 458 "parser.y"
    {
          (yyval.decl) = decl_new((yyvsp[(2) - (3)].sval), (int)(yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].dims), yylineno);
      }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 466 "parser.y"
    { symtab_begin_anonymous_union(); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 467 "parser.y"
    { symtab_end_anonymous_union(); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 473 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 494 "parser.y"
    {
          register_pending_function(0);
          free(pending_function_name);
          pending_function_name = NULL;
      }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 500 "parser.y"
    {
          register_pending_function(0);
          free(pending_function_name);
          pending_function_name = NULL;
      }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 513 "parser.y"
    {
          pending_function_return_type = (yyvsp[(2) - (3)].ival) ? type_list((yyvsp[(1) - (3)].type)) : (yyvsp[(1) - (3)].type);
          remember_function((yyvsp[(3) - (3)].sval));
          discard_text((yyvsp[(3) - (3)].sval));
      }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 527 "parser.y"
    { remember_parameter(NULL, (yyvsp[(1) - (2)].type), PASS_REFERENCE); }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 529 "parser.y"
    {
          Type *type = apply_declarator((yyvsp[(1) - (3)].type), (int)(yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].dims), yylineno);
          remember_parameter(NULL, type,
              implicit_reference(type) ? PASS_REFERENCE : PASS_VALUE);
          dim_free((yyvsp[(3) - (3)].dims));
      }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 543 "parser.y"
    {
          Type *type = type_named(TYPE_UNION, (yyvsp[(2) - (2)].sval));
          semantic_declare((yyvsp[(2) - (2)].sval), SYMBOL_TYPE, type, STORAGE_NONE, yylineno, 1);
          enter_named_scope("union", (yyvsp[(2) - (2)].sval));
          symtab_set_scope_type(symtab_current_scope(), type);
          symtab_set_current_access(ACCESS_PUBLIC);
          discard_text((yyvsp[(2) - (2)].sval));
      }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 552 "parser.y"
    {
          symtab_leave_scope();
      }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 559 "parser.y"
    { ast_free(process_declarations((yyvsp[(1) - (3)].type), (yyvsp[(2) - (3)].decl), SYMBOL_VARIABLE, STORAGE_GLOBAL)); }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 563 "parser.y"
    { (yyval.decl) = decl_append((yyvsp[(1) - (3)].decl), (yyvsp[(3) - (3)].decl)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 564 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (1)].decl); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 569 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (2)].decl); (yyval.decl)->initializer = (yyvsp[(2) - (2)].node); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 579 "parser.y"
    {
          if (diagnostics_syntax_count() == 0) {
              semantic_check_function_body((yyvsp[(2) - (2)].node), yylineno);
              printf("\n=== ABSTRACT SYNTAX TREE: %s ===\n", pending_function_name);
              ast_print(stdout, (yyvsp[(2) - (2)].node), 0);
          }
          ast_free((yyvsp[(2) - (2)].node));
          finish_function_scope();
      }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 589 "parser.y"
    {
          register_pending_function(1);
          enter_function_scope();
      }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 594 "parser.y"
    {
          if (diagnostics_syntax_count() == 0) {
              semantic_check_function_body((yyvsp[(3) - (3)].node), yylineno);
              printf("\n=== ABSTRACT SYNTAX TREE: %s ===\n", pending_function_name);
              ast_print(stdout, (yyvsp[(3) - (3)].node), 0);
          }
          ast_free((yyvsp[(3) - (3)].node));
          finish_function_scope();
      }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 604 "parser.y"
    {
          register_pending_function(1);
          enter_function_scope();
      }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 609 "parser.y"
    {
          if (diagnostics_syntax_count() == 0) {
              semantic_check_function_body((yyvsp[(3) - (3)].node), yylineno);
              printf("\n=== ABSTRACT SYNTAX TREE: %s ===\n", pending_function_name);
              ast_print(stdout, (yyvsp[(3) - (3)].node), 0);
          }
          ast_free((yyvsp[(3) - (3)].node));
          finish_function_scope();
      }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 622 "parser.y"
    {
          register_pending_function(1);
          enter_function_scope();
          declare_pending_parameters();
      }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 628 "parser.y"
    {
          register_pending_function(1);
          enter_function_scope();
          declare_pending_parameters();
      }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 637 "parser.y"
    {
          Type *owner = semantic_resolve_type((yyvsp[(3) - (4)].sval), yylineno);
          size_t length = strlen((yyvsp[(3) - (4)].sval)) + strlen((yyvsp[(4) - (4)].sval)) + 3;
          char *qualified = malloc(length);
          if (!qualified) {
              fprintf(stderr, "Out of memory\n");
              exit(EXIT_FAILURE);
          }
          if (owner->kind != TYPE_CLASS)
              semantic_error(yylineno, "'%s' is not a class", (yyvsp[(3) - (4)].sval));
          snprintf(qualified, length, "%s::%s", (yyvsp[(3) - (4)].sval), (yyvsp[(4) - (4)].sval));
          pending_function_return_type = (yyvsp[(2) - (4)].ival) ? type_list((yyvsp[(1) - (4)].type)) : (yyvsp[(1) - (4)].type);
          remember_function(qualified);
          pending_method_owner = owner;
          pending_method_name = strdup((yyvsp[(4) - (4)].sval));
          if (!pending_method_name) {
              fprintf(stderr, "Out of memory\n");
              exit(EXIT_FAILURE);
          }
          free(qualified);
          discard_text((yyvsp[(3) - (4)].sval));
          discard_text((yyvsp[(4) - (4)].sval));
      }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 664 "parser.y"
    {
          (yyval.sval) = (yyvsp[(1) - (2)].sval);
      }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 676 "parser.y"
    {
          remember_parameter((yyvsp[(3) - (3)].sval), (yyvsp[(1) - (3)].type), PASS_REFERENCE);
      }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 680 "parser.y"
    {
          Type *type = apply_declarator((yyvsp[(1) - (4)].type), (int)(yyvsp[(2) - (4)].ival), (yyvsp[(4) - (4)].dims), yylineno);
          remember_parameter((yyvsp[(3) - (4)].sval), type,
              implicit_reference(type) ? PASS_REFERENCE : PASS_VALUE);
          dim_free((yyvsp[(4) - (4)].dims));
      }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 694 "parser.y"
    { (yyval.node) = ast_new(AST_BLOCK, yylineno, NULL); (yyval.node)->child[0] = (yyvsp[(2) - (3)].node); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 696 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          (yyval.node) = ast_new(AST_BLOCK, yylineno, "syntax-error");
      }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 704 "parser.y"
    { (yyval.node) = ast_append((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 705 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 706 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 707 "parser.y"
    { (yyval.node) = NULL; }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 711 "parser.y"
    { (yyval.node) = ast_append((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 712 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 717 "parser.y"
    {
          (yyval.node) = process_declarations((yyvsp[(2) - (4)].type), (yyvsp[(3) - (4)].decl), SYMBOL_VARIABLE,
                                    (yyvsp[(1) - (4)].ival) ? STORAGE_STATIC_LOCAL : STORAGE_STACK);
      }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 722 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          (yyval.node) = ast_new(AST_EMPTY, yylineno, "syntax-error-declaration");
      }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 728 "parser.y"
    {
          /* Common error-production rule: a missing semicolon after a
             local declaration. The unexpected lookahead is retained so
             parsing can continue with the next declaration/statement. */
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          (yyval.node) = ast_new(AST_EMPTY, yylineno, "syntax-error-declaration");
      }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 739 "parser.y"
    { (yyval.ival) = 1; }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 740 "parser.y"
    { (yyval.ival) = 0; }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 744 "parser.y"
    { (yyval.node) = ast_append((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node)); }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 745 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 749 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 750 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 751 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 752 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 753 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 754 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 755 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 756 "parser.y"
    { (yyval.node) = semantic_loop_jump(AST_CONTINUE, yylineno); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 757 "parser.y"
    { (yyval.node) = semantic_loop_jump(AST_BREAK, yylineno); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 758 "parser.y"
    { (yyval.node) = ast_new(AST_EMPTY, yylineno, NULL); }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 760 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          (yyval.node) = ast_new(AST_EMPTY, yylineno, "syntax-error");
      }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 769 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 774 "parser.y"
    {
          (yyval.node) = ast_new(AST_IF, yylineno, NULL);
          (yyval.node)->child[0] = semantic_condition((yyvsp[(3) - (5)].node), yylineno);
          (yyval.node)->child[1] = (yyvsp[(5) - (5)].node);
      }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 780 "parser.y"
    {
          (yyval.node) = ast_new(AST_IF, yylineno, NULL);
          (yyval.node)->child[0] = semantic_condition((yyvsp[(3) - (7)].node), yylineno);
          (yyval.node)->child[1] = (yyvsp[(5) - (7)].node);
          (yyval.node)->child[2] = (yyvsp[(7) - (7)].node);
      }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 790 "parser.y"
    { semantic_enter_loop(); }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 792 "parser.y"
    {
          semantic_leave_loop();
          (yyval.node) = ast_new(AST_WHILE, yylineno, NULL);
          (yyval.node)->child[0] = semantic_condition((yyvsp[(3) - (6)].node), yylineno);
          (yyval.node)->child[1] = (yyvsp[(6) - (6)].node);
      }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 802 "parser.y"
    { semantic_enter_loop(); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 804 "parser.y"
    {
          semantic_leave_loop();
          (yyval.node) = ast_new(AST_FOR, yylineno, NULL);
          (yyval.node)->child[0] = (yyvsp[(3) - (10)].node);
          (yyval.node)->child[1] = (yyvsp[(5) - (10)].node) ? semantic_condition((yyvsp[(5) - (10)].node), yylineno) : NULL;
          (yyval.node)->child[2] = (yyvsp[(7) - (10)].node);
          (yyval.node)->child[3] = (yyvsp[(10) - (10)].node);
      }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 815 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 816 "parser.y"
    { (yyval.node) = NULL; }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 820 "parser.y"
    { (yyval.node) = semantic_return((yyvsp[(2) - (3)].node), yylineno); }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 825 "parser.y"
    { (yyval.node) = ast_new(AST_INPUT, yylineno, NULL); (yyval.node)->child[0] = (yyvsp[(3) - (4)].node); }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 827 "parser.y"
    { (yyval.node) = ast_new(AST_OUTPUT, yylineno, NULL); (yyval.node)->child[0] = (yyvsp[(3) - (4)].node); }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 831 "parser.y"
    { (yyval.node) = ast_append((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 832 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 837 "parser.y"
    {
          if (!(yyvsp[(1) - (1)].node)->is_lvalue) semantic_error(yylineno, "cin item is not an lvalue");
          (yyval.node) = (yyvsp[(1) - (1)].node);
      }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 844 "parser.y"
    { (yyval.node) = ast_append((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 845 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 849 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 854 "parser.y"
    {
          symtab_enter_scope("compound block");
      }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 861 "parser.y"
    {
          symtab_leave_scope();
          (yyval.node) = ast_new(AST_BLOCK, yylineno, NULL);
          (yyval.node)->child[0] = (yyvsp[(2) - (3)].node);
      }
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 867 "parser.y"
    {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          symtab_leave_scope();
          (yyval.node) = ast_new(AST_BLOCK, yylineno, "syntax-error");
      }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 877 "parser.y"
    {
          semantic_declare_function("main", type_function(type_int(), NULL), yylineno, 1);
          symtab_enter_scope("main");
          semantic_begin_function(type_int());
      }
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 883 "parser.y"
    {
          if (diagnostics_syntax_count() == 0) {
              semantic_check_function_body((yyvsp[(3) - (3)].node), yylineno);
              puts("\n=== ABSTRACT SYNTAX TREE: main ===");
              ast_print(stdout, (yyvsp[(3) - (3)].node), 0);
          }
          ast_free((yyvsp[(3) - (3)].node));
          semantic_end_function();
          symtab_leave_scope();
      }
    break;


/* Line 1792 of yacc.c  */
#line 3146 "parser.tab.c"
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
#line 899 "parser.y"


static void declare_enum_value(char *name, AstNode *initializer)
{
    if (name != NULL) {
        long long value = initializer
            ? semantic_require_integer_constant(initializer, "enum initializer", yylineno)
            : next_enum_value;
        Symbol *local = semantic_declare(
            name, SYMBOL_ENUM_VALUE,
            declaration_base_type ? declaration_base_type : type_error(),
            STORAGE_NONE, yylineno, 1);
        symtab_set_constant_value(local, CONST_VALUE_INT, value, 0.0, NULL);
        next_enum_value = value + 1;
        ast_free(initializer);
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
    free(pending_method_name);
    pending_method_name = NULL;
    pending_method_owner = NULL;

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
    if (pending_method_owner)
        semantic_begin_method(pending_method_owner,
                              pending_function_return_type ? pending_function_return_type : type_error());
    else
        semantic_begin_function(pending_function_return_type
                                ? pending_function_return_type : type_error());
}

static void finish_function_scope(void)
{
    semantic_end_function();
    symtab_leave_scope();
    free(pending_function_name);
    pending_function_name = NULL;
    free(pending_method_name);
    pending_method_name = NULL;
    pending_method_owner = NULL;
}

static void remember_parameter(char *name, Type *type, PassMode mode)
{
    PendingParameter *parameter;

    parameter = malloc(sizeof(*parameter));
    if (parameter == NULL) {
        fprintf(stderr, "Out of memory\n");
        free(name);
        exit(EXIT_FAILURE);
    }

    parameter->name = name;
    parameter->type = type;
    parameter->mode = mode;
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
        if (parameter->name != NULL) {
            StorageClass storage = parameter->mode == PASS_REFERENCE
                ? STORAGE_PARAMETER_REFERENCE : STORAGE_PARAMETER_VALUE;
            semantic_declare(parameter->name, SYMBOL_PARAMETER, parameter->type,
                             storage, yylineno, 1);
        }
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

static void register_pending_function(int definition)
{
    PendingParameter *pending;
    Parameter *parameters = NULL;
    Type *function_type;

    for (pending = pending_parameters; pending; pending = pending->next) {
        parameters = parameter_append(parameters,
            parameter_new(pending->name, pending->type, pending->mode));
    }

    function_type = type_function(
        pending_function_return_type ? pending_function_return_type : type_error(),
        parameters);
    parameter_free(parameters);

    if (pending_method_owner)
        semantic_declare_method(pending_method_owner,
            pending_method_name ? pending_method_name : "<anonymous>",
            function_type, yylineno, definition);
    else
        semantic_declare_function(
            pending_function_name ? pending_function_name : "<anonymous>",
            function_type, yylineno, definition);
}

static int implicit_reference(Type *type)
{
    return type && (type->kind == TYPE_STRING || type->kind == TYPE_ARRAY ||
                    type->kind == TYPE_LIST || type->kind == TYPE_CLASS ||
                    type->kind == TYPE_UNION);
}

static DimList *dim_new(long long length)
{
    DimList *dimension = calloc(1, sizeof(*dimension));
    if (!dimension) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    dimension->length = length;
    return dimension;
}

static DimList *dim_append(DimList *list, DimList *item)
{
    DimList *tail;
    if (!list) return item;
    for (tail = list; tail->next; tail = tail->next) {}
    tail->next = item;
    return list;
}

static Type *apply_dimensions_recursive(Type *base, DimList *dimensions)
{
    Type *inner;
    if (!dimensions) return base;
    inner = apply_dimensions_recursive(base, dimensions->next);
    return type_array(inner, dimensions->length);
}

static Type *apply_declarator(Type *base, int is_list,
                              DimList *dimensions, int line)
{
    Type *type;
    DimList *dimension;
    int position = 0;
    if (is_list && dimensions) {
        semantic_error(line,
            "a list marker and array dimensions cannot coexist in one declarator");
    }
    for (dimension = dimensions; dimension; dimension = dimension->next, position++) {
        if (dimension->length == 0)
            semantic_error(line, "array dimensions must be greater than zero");
        if (dimension->length < 0 && position != 0)
            semantic_error(line, "only the first array dimension may be omitted");
    }
    type = is_list ? type_list(base) : base;
    return apply_dimensions_recursive(type, dimensions);
}

static void dim_free(DimList *dimensions)
{
    while (dimensions) {
        DimList *next = dimensions->next;
        free(dimensions);
        dimensions = next;
    }
}

static DeclItem *decl_new(char *name, int is_list,
                          DimList *dimensions, int line)
{
    DeclItem *item = calloc(1, sizeof(*item));
    if (!item) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    item->name = name;
    item->is_list = is_list;
    item->dimensions = dimensions;
    item->line = line;
    return item;
}

static DeclItem *decl_append(DeclItem *list, DeclItem *item)
{
    DeclItem *tail;
    if (!list) return item;
    for (tail = list; tail->next; tail = tail->next) {}
    tail->next = item;
    return list;
}

static AstNode *process_declarations(Type *base, DeclItem *items,
                                     SymbolKind kind, StorageClass storage)
{
    AstNode *declarations = NULL;
    while (items) {
        DeclItem *next = items->next;
        Type *type = apply_declarator(base, items->is_list,
                                      items->dimensions, items->line);
        Symbol *symbol;
        AstNode *declaration;

        semantic_validate_initializer(type, items->initializer,
                                      items->name, items->line);

        symbol = semantic_declare(items->name, kind, type, storage, items->line, 1);
        semantic_store_initial_value(symbol, items->initializer);
        declaration = ast_new(AST_DECL, items->line, items->name);
        declaration->type = type;
        declaration->child[0] = items->initializer;
        items->initializer = NULL;
        declarations = ast_append(declarations, declaration);
        dim_free(items->dimensions);
        free(items->name);
        free(items);
        items = next;
    }
    return declarations;
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
    types_init();
    symtab_init();
    semantic_init();

    parse_result = yyparse();

    if (symtab_current_scope()) symtab_print_scope(symtab_current_scope());
    if (parse_result == 0 && diagnostics_lexical_count() == 0 &&
        diagnostics_syntax_count() == 0)
        semantic_verify_all_prototypes();
    symtab_finish();

    success = parse_result == 0 &&
              diagnostics_lexical_count() == 0 &&
              diagnostics_syntax_count() == 0 &&
              semantic_error_count() == 0;

    printf("\n=== ANALYSIS SUMMARY ===\n");
    printf("Lexical errors : %d\n", diagnostics_lexical_count());
    printf("Syntax errors  : %d\n", diagnostics_syntax_count());
    printf("Semantic errors: %d\n", semantic_error_count());
    printf("Result         : %s\n", success ? "ACCEPTED" : "REJECTED");

    clear_pending_parameters();
    free(pending_function_name);
    pending_function_name = NULL;
    free(pending_method_name);
    pending_method_name = NULL;
    fclose(yyin);
    diagnostics_free_source();
    semantic_finish();
    types_finish();

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
