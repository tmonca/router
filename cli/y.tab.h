
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_SHOW = 258,
     T_QUESTION = 259,
     T_NEWLINE = 260,
     T_ALL = 261,
     T_VNS = 262,
     T_USER = 263,
     T_VHOST = 264,
     T_LHOST = 265,
     T_TOPOLOGY = 266,
     T_IP = 267,
     T_ROUTE = 268,
     T_INTF = 269,
     T_ARP = 270,
     T_OSPF = 271,
     T_HW = 272,
     T_NEIGHBORS = 273,
     T_ADD = 274,
     T_DEL = 275,
     T_UP = 276,
     T_DOWN = 277,
     T_PURGE = 278,
     T_STATIC = 279,
     T_DYNAMIC = 280,
     T_ABOUT = 281,
     T_PING = 282,
     T_TRACE = 283,
     T_HELP = 284,
     T_EXIT = 285,
     T_SHUTDOWN = 286,
     T_FLOOD = 287,
     T_SET = 288,
     T_UNSET = 289,
     T_OPTION = 290,
     T_VERBOSE = 291,
     T_DATE = 292,
     TAV_INT = 293,
     TAV_IP = 294,
     TAV_MAC = 295,
     TAV_STR = 296
   };
#endif
/* Tokens.  */
#define T_SHOW 258
#define T_QUESTION 259
#define T_NEWLINE 260
#define T_ALL 261
#define T_VNS 262
#define T_USER 263
#define T_VHOST 264
#define T_LHOST 265
#define T_TOPOLOGY 266
#define T_IP 267
#define T_ROUTE 268
#define T_INTF 269
#define T_ARP 270
#define T_OSPF 271
#define T_HW 272
#define T_NEIGHBORS 273
#define T_ADD 274
#define T_DEL 275
#define T_UP 276
#define T_DOWN 277
#define T_PURGE 278
#define T_STATIC 279
#define T_DYNAMIC 280
#define T_ABOUT 281
#define T_PING 282
#define T_TRACE 283
#define T_HELP 284
#define T_EXIT 285
#define T_SHUTDOWN 286
#define T_FLOOD 287
#define T_SET 288
#define T_UNSET 289
#define T_OPTION 290
#define T_VERBOSE 291
#define T_DATE 292
#define TAV_INT 293
#define TAV_IP 294
#define TAV_MAC 295
#define TAV_STR 296




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 81 "cli_parser.y"

    int intVal;
    uint32_t ip;
    uint8_t mac[6];
    char string[MAX_STR_LEN];



/* Line 1676 of yacc.c  */
#line 143 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


