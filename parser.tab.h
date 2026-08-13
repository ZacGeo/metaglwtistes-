/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
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
/* Line 2058 of yacc.c  */
#line 65 "parser.y"

    long long ival;
    double fval;
    char *sval;
    Type *type;
    AstNode *node;
    DimList *dims;
    DeclItem *decl;


/* Line 2058 of yacc.c  */
#line 128 "parser.tab.h"
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
