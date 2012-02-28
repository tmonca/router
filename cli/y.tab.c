
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 3 "cli_parser.y"


#include <string.h>              /* memcpy(), memmove(), memset()     */
#include "cli.h"                 /* implementation of CLI methods     */
#include "cli_help.h"            /* cli_help_t, cli_send_help()       */
#include "cli_scanner.h"         /* for yylex                         */
#include "cli_parser.h"          /* parser external API               */
#include "helper.h"

/** buffer to feed to the parser */
static char cmd_buf[CLI_MAX_BUF+2];

/** number of bytes used in buf */
static unsigned cmd_blen;

/**
 * Manual Error Handler.  If not NULL, then yyerror will not print an error
 * message (the user is manually handling the error).  This points to the error
 * desc text to use in yyerror.  (Syntax Error: <meh_desc>).  If meh_desc is
 * NULL, and yyerror is called, then the argument passed to yyerror is used.
 */
const char* meh_desc;
int meh_has_usage;
int meh_ignore;
int meh_force;

/** Handles printing a nicely formatted error. */
void yyerror( const char* desc );
static int has_error;
char token_err[128];

/* convenience */
#define ERR(desc) parse_error(desc);
#define ERR_TMI   ERR("expected end-of-command (newline)")
#define ERR_WRONG ERR("unexpected token")
#define ERR_IP    ERR("expected IP address")
#define ERR_MAC   ERR("expected MAC address")
#define ERR_INTF  ERR("expected interface name")
#define ERR_NO_USAGE(desc) parse_error(desc); meh_force = 1; meh_has_usage = 0; meh_ignore = 0;
#define ERR_IGNORE meh_ignore = 1;

/* We only want to call a command once it is completely parsed (with the
   exception of help commands which fire on errors and can be safely executed
   immediately as actions).  This is a gross, but these structs hold the parsed
   data until it is time to execute a command. */
gross_object_t gobj;
gross_arp_t garp;
gross_intf_t gintf;
gross_route_t grt;
gross_ip_t gip;
gross_ip_int_t giip;
gross_option_t gopt;
#define SETC_FUNC0(func)      gobj.func_do0=func; gobj.func_do1=NULL; gobj.data=NULL
#define SETC_FUNC1(func)      gobj.func_do0=NULL; gobj.func_do1=(void (*)(void*))func; gobj.data=NULL
#define SETC_ARP_IP(func,xip)  SETC_FUNC1(func); gobj.data=&garp; garp.ip=xip
#define SETC_ARP(func,ip,xmac) SETC_ARP_IP(func,ip); memcpy(garp.mac, xmac, 6);
#define SETC_INTF(func,name)  SETC_FUNC1(func); gobj.data=&gintf; gintf.intf_name=name
#define SETC_INTF_SET(func,name,xip,sm) SETC_INTF(func,name); gintf.ip=xip; gintf.subnet_mask=sm
#define SETC_RT(func,xdest,xmask) SETC_FUNC1(func); gobj.data=&grt; grt.dest=xdest; grt.mask=xmask
#define SETC_RT_ADD(func,dest,xgw,mask,intf) SETC_RT(func,dest,mask); grt.gw=xgw; grt.intf_name=intf
#define SETC_IP(func,xip) SETC_FUNC1(func); gobj.data=&gip; gip.ip=xip
#define SETC_IP_INT(func,xip,xn) SETC_FUNC1(func); gobj.data=&giip; giip.ip=xip; giip.count=xn
#define SETC_OPT(func) SETC_FUNC1(func); gobj.data=&gopt

/** Clears out any previous command */
static void clear_command();

/** Run the command stored in gobj, if any. */
static void run_command();

/**
 * Convenience method for when a help command is called (directly or by an
 * error handler).
 */
#define HELP(h) cli_send_help(h); clear_command(); YYABORT;



/* Line 189 of yacc.c  */
#line 152 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 81 "cli_parser.y"

    int intVal;
    uint32_t ip;
    uint8_t mac[6];
    char string[MAX_STR_LEN];



/* Line 214 of yacc.c  */
#line 279 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 291 "y.tab.c"

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
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

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

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   342

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  215
/* YYNRULES -- Number of states.  */
#define YYNSTATES  238

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

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
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     8,    10,    12,    14,    16,    17,
      20,    21,    24,    25,    28,    31,    34,    37,    40,    42,
      43,    45,    48,    50,    53,    55,    58,    60,    63,    65,
      66,    68,    71,    73,    76,    78,    81,    83,    84,    86,
      89,    91,    94,    96,    98,    99,   102,   104,   105,   108,
     109,   111,   114,   116,   119,   121,   124,   126,   129,   131,
     134,   137,   140,   143,   146,   148,   151,   154,   156,   159,
     162,   166,   169,   173,   175,   176,   179,   180,   184,   187,
     191,   193,   194,   197,   199,   202,   204,   205,   208,   211,
     212,   217,   221,   226,   229,   233,   236,   240,   242,   244,
     247,   249,   252,   254,   257,   260,   262,   265,   268,   271,
     273,   274,   277,   278,   282,   283,   288,   289,   295,   300,
     306,   308,   309,   312,   313,   317,   320,   324,   327,   330,
     332,   334,   336,   338,   340,   341,   344,   346,   349,   353,
     357,   362,   365,   367,   368,   371,   373,   376,   378,   381,
     383,   386,   388,   391,   393,   396,   397,   402,   405,   409,
     414,   419,   424,   429,   433,   438,   443,   448,   452,   457,
     461,   466,   471,   475,   480,   485,   490,   495,   498,   502,
     507,   512,   517,   523,   529,   533,   538,   543,   547,   552,
     557,   562,   567,   572,   575,   578,   581,   584,   587,   590,
     594,   597,   601,   602,   606,   608,   610,   613,   615,   617,
     619,   621,   624,   625,   627,   630
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    -1,    44,    45,     5,    -1,    47,    -1,
      57,    -1,    78,    -1,    90,    -1,    -1,    46,     1,    -1,
      -1,     3,    48,    -1,    -1,    17,    49,    -1,    12,    50,
      -1,    16,    51,    -1,     7,    56,    -1,    35,    93,    -1,
      89,    -1,    -1,    26,    -1,    26,    52,    -1,    15,    -1,
      15,    52,    -1,    14,    -1,    14,    52,    -1,    13,    -1,
      13,    52,    -1,    54,    -1,    -1,    15,    -1,    15,    52,
      -1,    14,    -1,    14,    52,    -1,    13,    -1,    13,    52,
      -1,    54,    -1,    -1,    18,    -1,    18,    52,    -1,    11,
      -1,    11,    52,    -1,    54,    -1,    89,    -1,    -1,    53,
       1,    -1,    89,    -1,    -1,    55,     1,    -1,    -1,    10,
      -1,    10,    52,    -1,    11,    -1,    11,    52,    -1,     8,
      -1,     8,    52,    -1,     9,    -1,     9,    52,    -1,    54,
      -1,    12,    58,    -1,    15,    59,    -1,    14,    65,    -1,
      16,    68,    -1,    13,    69,    -1,    54,    -1,    19,    60,
      -1,    20,    63,    -1,    23,    -1,    23,    52,    -1,    23,
      25,    -1,    23,    25,    52,    -1,    23,    24,    -1,    23,
      24,    52,    -1,    89,    -1,    -1,    61,     1,    -1,    -1,
      39,    62,     1,    -1,    39,    40,    -1,    39,    40,    52,
      -1,    89,    -1,    -1,    64,     1,    -1,    39,    -1,    39,
      52,    -1,    89,    -1,    -1,    66,     1,    -1,    41,    54,
      -1,    -1,    41,    39,    67,     1,    -1,    41,    39,    39,
      -1,    41,    39,    39,    52,    -1,    41,    21,    -1,    41,
      21,    52,    -1,    41,    22,    -1,    41,    22,    52,    -1,
      54,    -1,    21,    -1,    21,    52,    -1,    22,    -1,    22,
      52,    -1,    54,    -1,    19,    70,    -1,    20,    75,    -1,
      23,    -1,    23,    25,    -1,    23,    24,    -1,    23,    52,
      -1,    89,    -1,    -1,    71,     1,    -1,    -1,    39,    72,
       1,    -1,    -1,    39,    39,    73,     1,    -1,    -1,    39,
      39,    39,    74,     1,    -1,    39,    39,    39,    41,    -1,
      39,    39,    39,    41,    52,    -1,    89,    -1,    -1,    76,
       1,    -1,    -1,    39,    77,     1,    -1,    39,    39,    -1,
      39,    39,    52,    -1,    27,    79,    -1,    28,    81,    -1,
      83,    -1,    84,    -1,    85,    -1,    86,    -1,    89,    -1,
      -1,    80,     1,    -1,    39,    -1,    32,    39,    -1,    32,
      39,    52,    -1,    32,    38,    39,    -1,    32,    38,    39,
      52,    -1,    39,    52,    -1,    89,    -1,    -1,    82,     1,
      -1,    39,    -1,    39,    52,    -1,    37,    -1,    37,    52,
      -1,    30,    -1,    30,    52,    -1,    31,    -1,    31,    52,
      -1,    89,    -1,    89,     6,    -1,    -1,    89,     6,    87,
       1,    -1,    89,     3,    -1,    89,     3,    17,    -1,    89,
       3,    17,    26,    -1,    89,     3,    17,    15,    -1,    89,
       3,    17,    14,    -1,    89,     3,    17,    13,    -1,    89,
       3,    12,    -1,    89,     3,    12,    15,    -1,    89,     3,
      12,    14,    -1,    89,     3,    12,    13,    -1,    89,     3,
      35,    -1,    89,     3,    35,    36,    -1,    89,     3,    16,
      -1,    89,     3,    16,    18,    -1,    89,     3,    16,    11,
      -1,    89,     3,     7,    -1,    89,     3,     7,    10,    -1,
      89,     3,     7,    11,    -1,    89,     3,     7,     8,    -1,
      89,     3,     7,     9,    -1,    89,    12,    -1,    89,    12,
      15,    -1,    89,    12,    15,    19,    -1,    89,    12,    15,
      20,    -1,    89,    12,    15,    23,    -1,    89,    12,    15,
      23,    25,    -1,    89,    12,    15,    23,    24,    -1,    89,
      12,    14,    -1,    89,    12,    14,    22,    -1,    89,    12,
      14,    21,    -1,    89,    12,    13,    -1,    89,    12,    13,
      19,    -1,    89,    12,    13,    20,    -1,    89,    12,    13,
      23,    -1,    89,    12,    13,    25,    -1,    89,    12,    13,
      24,    -1,    89,    37,    -1,    89,    30,    -1,    89,    27,
      -1,    89,    31,    -1,    89,    28,    -1,    89,    33,    -1,
      89,    33,    36,    -1,    89,    34,    -1,    89,    34,    36,
      -1,    -1,    89,    88,     1,    -1,    29,    -1,     4,    -1,
      91,    92,    -1,    33,    -1,    34,    -1,    89,    -1,    36,
      -1,    36,    52,    -1,    -1,    36,    -1,    36,    52,    -1,
      54,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   104,   104,   104,   107,   108,   109,   110,   111,   111,
     112,   115,   118,   119,   120,   121,   122,   123,   124,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   139,
     140,   141,   142,   143,   144,   145,   146,   149,   150,   151,
     152,   153,   154,   157,   158,   158,   161,   162,   162,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   177,
     180,   181,   182,   183,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   197,   198,   198,   199,   199,   200,   201,
     204,   205,   205,   206,   207,   210,   211,   211,   212,   213,
     213,   214,   215,   216,   217,   218,   219,   222,   223,   224,
     225,   226,   229,   230,   231,   232,   233,   234,   235,   238,
     239,   239,   240,   240,   241,   241,   242,   242,   243,   244,
     247,   248,   248,   249,   249,   250,   251,   254,   255,   256,
     257,   258,   259,   262,   263,   263,   264,   265,   266,   267,
     268,   269,   272,   273,   273,   274,   275,   278,   279,   282,
     283,   286,   287,   290,   291,   292,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   338,   341,   342,   345,   348,   349,   352,
     353,   354,   357,   358,   359,   360
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_SHOW", "T_QUESTION", "T_NEWLINE",
  "T_ALL", "T_VNS", "T_USER", "T_VHOST", "T_LHOST", "T_TOPOLOGY", "T_IP",
  "T_ROUTE", "T_INTF", "T_ARP", "T_OSPF", "T_HW", "T_NEIGHBORS", "T_ADD",
  "T_DEL", "T_UP", "T_DOWN", "T_PURGE", "T_STATIC", "T_DYNAMIC", "T_ABOUT",
  "T_PING", "T_TRACE", "T_HELP", "T_EXIT", "T_SHUTDOWN", "T_FLOOD",
  "T_SET", "T_UNSET", "T_OPTION", "T_VERBOSE", "T_DATE", "TAV_INT",
  "TAV_IP", "TAV_MAC", "TAV_STR", "$accept", "Start", "$@1", "Command",
  "$@2", "ShowCommand", "ShowType", "ShowTypeHW", "ShowTypeIP",
  "ShowTypeOSPF", "TMIorQ", "$@3", "WrongOrQ", "$@4", "ShowTypeVNS",
  "ManipCommand", "ManipTypeIP", "ManipTypeIPARP", "AddTransOrQ", "$@5",
  "$@6", "DelTransOrQ", "$@7", "ManipTypeIPInterface", "$@8", "$@9",
  "ManipTypeIPOSPF", "ManipTypeIPRoute", "RouteAddOrQ", "$@10", "$@11",
  "$@12", "$@13", "RouteDelOrQ", "$@14", "$@15", "ActionCommand",
  "ActionPing", "$@16", "ActionTrace", "$@17", "ActionDate", "ActionExit",
  "ActionShutdown", "ActionHelp", "$@18", "$@19", "HelpOrQ",
  "OptionCommand", "OptionAction", "Option", "ShowTypeOption", 0
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
     295,   296
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    44,    43,    45,    45,    45,    45,    46,    45,
      45,    47,    48,    48,    48,    48,    48,    48,    48,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    50,
      50,    50,    50,    50,    50,    50,    50,    51,    51,    51,
      51,    51,    51,    52,    53,    52,    54,    55,    54,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    57,
      58,    58,    58,    58,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    61,    60,    62,    60,    60,    60,
      63,    64,    63,    63,    63,    65,    66,    65,    65,    67,
      65,    65,    65,    65,    65,    65,    65,    68,    68,    68,
      68,    68,    69,    69,    69,    69,    69,    69,    69,    70,
      71,    70,    72,    70,    73,    70,    74,    70,    70,    70,
      75,    76,    75,    77,    75,    75,    75,    78,    78,    78,
      78,    78,    78,    79,    80,    79,    79,    79,    79,    79,
      79,    79,    81,    82,    81,    81,    81,    83,    83,    84,
      84,    85,    85,    86,    86,    87,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    88,    86,    89,    89,    90,    91,    91,    92,
      92,    92,    93,    93,    93,    93
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     0,     2,
       0,     2,     0,     2,     2,     2,     2,     2,     1,     0,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     0,
       1,     2,     1,     2,     1,     2,     1,     0,     1,     2,
       1,     2,     1,     1,     0,     2,     1,     0,     2,     0,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       2,     2,     2,     2,     1,     2,     2,     1,     2,     2,
       3,     2,     3,     1,     0,     2,     0,     3,     2,     3,
       1,     0,     2,     1,     2,     1,     0,     2,     2,     0,
       4,     3,     4,     2,     3,     2,     3,     1,     1,     2,
       1,     2,     1,     2,     2,     1,     2,     2,     2,     1,
       0,     2,     0,     3,     0,     4,     0,     5,     4,     5,
       1,     0,     2,     0,     3,     2,     3,     2,     2,     1,
       1,     1,     1,     1,     0,     2,     1,     2,     3,     3,
       4,     2,     1,     0,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     0,     4,     2,     3,     4,
       4,     4,     4,     3,     4,     4,     4,     3,     4,     3,
       4,     4,     3,     4,     4,     4,     4,     2,     3,     4,
       4,     4,     5,     5,     3,     4,     4,     3,     4,     4,
       4,     4,     4,     2,     2,     2,     2,     2,     2,     3,
       2,     3,     0,     3,     1,     1,     2,     1,     1,     1,
       1,     2,     0,     1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     8,     1,    12,   205,     0,   134,   143,   204,
      44,    44,   207,   208,    44,     0,     0,     4,     5,     6,
     129,   130,   131,   132,   153,     7,     0,    47,    29,    37,
      19,    47,    11,    18,    47,    86,    47,    47,    59,     0,
      44,   127,     0,   133,    44,   128,     0,   142,   150,     0,
      43,   152,   148,     3,     9,   157,   154,   177,   195,   197,
     194,   196,   198,   200,   193,     0,    44,   209,   206,    44,
      44,    44,    44,    58,     0,    16,    46,    34,    32,    30,
      14,    36,    40,    38,    15,    42,    26,    24,    22,    20,
      13,    28,    44,   215,    17,   110,   121,    44,   102,    63,
      47,    61,     0,    85,    74,    81,    44,    64,    60,    44,
      44,    97,    62,     0,    44,   141,   135,   146,   144,    45,
     172,   163,   169,   158,   167,     0,   187,   184,   178,   199,
     201,   203,   211,    55,    57,    51,    53,    48,    35,    33,
      31,    41,    39,    27,    25,    23,    21,   214,   112,   103,
       0,   109,   123,   104,     0,   120,   107,   106,   108,    44,
      44,    89,    88,    87,    76,    65,     0,    73,    44,    66,
       0,    80,    44,    44,    68,    99,   101,    44,   138,   175,
     176,   173,   174,   166,   165,   164,   171,   170,   162,   161,
     160,   159,   168,   156,   188,   189,   190,   192,   191,   186,
     185,   179,   180,   181,   114,     0,   111,    44,     0,   122,
      94,    96,    44,     0,    44,     0,    75,    84,    82,    72,
      70,   140,   183,   182,   116,     0,   113,   126,   124,    92,
      90,    79,    77,    44,     0,   115,   119,   117
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    15,    16,    17,    32,    90,    80,    84,
      48,    49,    73,    74,    75,    18,    38,   108,   165,   166,
     215,   169,   170,   101,   102,   213,   112,    99,   149,   150,
     205,   225,   234,   153,   154,   208,    19,    41,    42,    45,
      46,    20,    21,    22,    23,   125,    65,    50,    25,    26,
      68,    94
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -27
static const yytype_int16 yypact[] =
{
     -27,     8,   140,   -27,   125,   -27,   322,     2,    18,   -27,
     176,   221,   -27,   -27,   224,    -4,    11,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,    88,   -27,   102,   205,   175,   135,
     171,    97,   -27,   -27,   136,     1,   204,   106,   -27,   -24,
     227,   -27,    22,   -27,   231,   -27,    51,   -27,   -27,    55,
     -27,   -27,   -27,   -27,   -27,   195,    69,   308,   -27,   -27,
     -27,   -27,   -26,    33,   -27,    84,   241,   -27,   -27,   243,
     247,   250,   253,   -27,    87,   -27,   -27,     3,     3,     3,
     -27,   -27,     3,     3,   -27,   -27,     3,     3,     3,     3,
     -27,   -27,   257,   -27,   -27,    24,    44,   213,   -27,   -27,
      58,   -27,    95,   -27,    45,    78,   215,   -27,   -27,   259,
     261,   -27,   -27,   -14,   263,   -27,   -27,   -27,   -27,   -27,
     331,   312,    -9,   180,    72,   110,   292,    43,    31,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,    73,   -27,
     119,   -27,    85,   -27,   122,   -27,   -27,   -27,   -27,   269,
     273,    91,   -27,   -27,    94,   -27,   146,   -27,   276,   -27,
     149,   -27,   279,   289,   -27,   -27,   -27,   291,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,    68,   112,   157,   -27,   295,   162,   -27,
     -27,   -27,   299,   177,   302,   181,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   142,   186,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   305,   190,   -27,   -27,   -27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -11,   -27,   -10,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,     9,   -27,   -27,
     -27,   -27
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -214
static const yytype_int16 yytable[] =
{
      51,    53,   186,    52,   -44,     5,     5,     5,     3,   187,
     129,    24,    54,    33,   113,   114,    43,    47,    81,    85,
      91,    93,     5,   116,    98,   177,   107,   111,     5,   115,
       9,     9,     9,   117,    39,    67,    76,    76,    76,    76,
      76,    40,   100,    76,   103,    76,    76,     9,     5,     5,
     201,   202,   118,     9,   203,   132,   119,    44,   133,   134,
     135,   136,     5,   148,   199,   200,   138,   139,   140,   130,
    -155,   141,   142,     9,     9,   143,   144,   145,   146,   159,
     160,   147,     5,   152,   164,   131,   158,     9,   137,  -202,
     162,    55,   222,   223,    56,   174,   163,   161,   175,   176,
      57,     5,  -212,   178,   151,   155,     5,     9,   192,    76,
       5,   193,   204,   167,   171,    58,    59,   168,    60,    61,
     206,    62,    63,   209,   207,    64,     9,   109,   110,     5,
     212,     9,    27,    92,   214,     9,   -47,    28,    66,     5,
       5,    29,    30,     4,     5,   -10,    82,   216,   210,   211,
     218,   224,     6,    83,     9,    95,    96,   217,   226,    97,
      31,   219,   220,   228,     9,     9,   221,     7,     8,     9,
      10,    11,   -47,    12,    13,     5,   -47,    14,   230,     5,
       5,  -149,   232,   233,    86,    87,    88,   235,    77,    78,
      79,   237,     0,   188,   189,   190,   227,    89,     0,     0,
       9,   229,   120,   231,     9,     9,   191,   121,     5,     5,
     -49,   122,   123,    69,    70,    71,    72,     5,  -105,     5,
     -67,     0,   236,   104,   105,     5,  -151,   106,     5,  -147,
     124,     5,  -136,     9,     9,     5,  -145,   156,   157,   172,
     173,     0,     9,     0,     9,     5,  -210,     5,   -54,     0,
       9,     5,   -56,     9,     5,   -50,     9,     5,   -52,     0,
       9,     5,  -213,     5,   -98,     5,  -100,     5,  -137,     0,
       9,     0,     9,     5,   -93,     0,     9,     5,   -95,     9,
       5,   -83,     9,     5,   -71,     0,     9,     0,     9,     0,
       9,     0,     9,     5,   -69,     5,  -139,     0,     9,     5,
    -125,     0,     9,     5,   -91,     9,     5,   -78,     9,     5,
    -118,   194,   195,     0,     0,   196,   197,   198,     9,     0,
       9,   126,   127,   128,     9,   183,   184,   185,     9,     0,
       0,     9,     0,     0,     9,    34,    35,    36,    37,   179,
     180,   181,   182
};

static const yytype_int16 yycheck[] =
{
      11,     5,    11,    14,     1,     4,     4,     4,     0,    18,
      36,     2,     1,     4,    38,    39,     7,     8,    28,    29,
      30,    31,     4,     1,    34,    39,    36,    37,     4,    40,
      29,    29,    29,    44,    32,    26,    27,    28,    29,    30,
      31,    39,    41,    34,    35,    36,    37,    29,     4,     4,
      19,    20,     1,    29,    23,    66,     1,    39,    69,    70,
      71,    72,     4,    39,    21,    22,    77,    78,    79,    36,
       1,    82,    83,    29,    29,    86,    87,    88,    89,    21,
      22,    92,     4,    39,    39,     1,    97,    29,     1,     1,
     100,     3,    24,    25,     6,   106,     1,    39,   109,   110,
      12,     4,     5,   114,    95,    96,     4,    29,    36,   100,
       4,     1,    39,   104,   105,    27,    28,    39,    30,    31,
       1,    33,    34,     1,    39,    37,    29,    21,    22,     4,
      39,    29,     7,    36,    40,    29,     1,    12,    36,     4,
       4,    16,    17,     3,     4,     5,    11,     1,   159,   160,
       1,    39,    12,    18,    29,    19,    20,   168,     1,    23,
      35,   172,   173,     1,    29,    29,   177,    27,    28,    29,
      30,    31,     1,    33,    34,     4,     1,    37,     1,     4,
       4,     5,     1,    41,    13,    14,    15,     1,    13,    14,
      15,     1,    -1,    13,    14,    15,   207,    26,    -1,    -1,
      29,   212,     7,   214,    29,    29,    26,    12,     4,     4,
       5,    16,    17,     8,     9,    10,    11,     4,     5,     4,
       5,    -1,   233,    19,    20,     4,     5,    23,     4,     5,
      35,     4,     5,    29,    29,     4,     5,    24,    25,    24,
      25,    -1,    29,    -1,    29,     4,     5,     4,     5,    -1,
      29,     4,     5,    29,     4,     5,    29,     4,     5,    -1,
      29,     4,     5,     4,     5,     4,     5,     4,     5,    -1,
      29,    -1,    29,     4,     5,    -1,    29,     4,     5,    29,
       4,     5,    29,     4,     5,    -1,    29,    -1,    29,    -1,
      29,    -1,    29,     4,     5,     4,     5,    -1,    29,     4,
       5,    -1,    29,     4,     5,    29,     4,     5,    29,     4,
       5,    19,    20,    -1,    -1,    23,    24,    25,    29,    -1,
      29,    13,    14,    15,    29,    13,    14,    15,    29,    -1,
      -1,    29,    -1,    -1,    29,    13,    14,    15,    16,     8,
       9,    10,    11
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    43,    44,     0,     3,     4,    12,    27,    28,    29,
      30,    31,    33,    34,    37,    45,    46,    47,    57,    78,
      83,    84,    85,    86,    89,    90,    91,     7,    12,    16,
      17,    35,    48,    89,    13,    14,    15,    16,    58,    32,
      39,    79,    80,    89,    39,    81,    82,    89,    52,    53,
      89,    52,    52,     5,     1,     3,     6,    12,    27,    28,
      30,    31,    33,    34,    37,    88,    36,    89,    92,     8,
       9,    10,    11,    54,    55,    56,    89,    13,    14,    15,
      50,    54,    11,    18,    51,    54,    13,    14,    15,    26,
      49,    54,    36,    54,    93,    19,    20,    23,    54,    69,
      41,    65,    66,    89,    19,    20,    23,    54,    59,    21,
      22,    54,    68,    38,    39,    52,     1,    52,     1,     1,
       7,    12,    16,    17,    35,    87,    13,    14,    15,    36,
      36,     1,    52,    52,    52,    52,    52,     1,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    39,    70,
      71,    89,    39,    75,    76,    89,    24,    25,    52,    21,
      22,    39,    54,     1,    39,    60,    61,    89,    39,    63,
      64,    89,    24,    25,    52,    52,    52,    39,    52,     8,
       9,    10,    11,    13,    14,    15,    11,    18,    13,    14,
      15,    26,    36,     1,    19,    20,    23,    24,    25,    21,
      22,    19,    20,    23,    39,    72,     1,    39,    77,     1,
      52,    52,    39,    67,    40,    62,     1,    52,     1,    52,
      52,    52,    24,    25,    39,    73,     1,    52,     1,    52,
       1,    52,     1,    41,    74,     1,    52,     1
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
        case 2:

/* Line 1455 of yacc.c  */
#line 104 "cli_parser.y"
    { clear_command(); }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 104 "cli_parser.y"
    { run_command(); YYACCEPT; }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 111 "cli_parser.y"
    {ERR_NO_USAGE("Unknown Command")}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 118 "cli_parser.y"
    { SETC_FUNC0(cli_show_all); }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 124 "cli_parser.y"
    { HELP(HELP_SHOW); }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 127 "cli_parser.y"
    { SETC_FUNC0(cli_show_hw); }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 128 "cli_parser.y"
    { SETC_FUNC0(cli_show_hw_about); }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 129 "cli_parser.y"
    { HELP(HELP_SHOW_HW_ABOUT); }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 130 "cli_parser.y"
    { SETC_FUNC0(cli_show_hw_arp); }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 131 "cli_parser.y"
    { HELP(HELP_SHOW_HW_ARP); }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 132 "cli_parser.y"
    { SETC_FUNC0(cli_show_hw_intf); }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 133 "cli_parser.y"
    { HELP(HELP_SHOW_HW_INTF); }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 134 "cli_parser.y"
    { SETC_FUNC0(cli_show_hw_route); }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 135 "cli_parser.y"
    { HELP(HELP_SHOW_HW_ROUTE); }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 136 "cli_parser.y"
    { HELP(HELP_SHOW_HW); }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 139 "cli_parser.y"
    { SETC_FUNC0(cli_show_ip); }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 140 "cli_parser.y"
    { SETC_FUNC0(cli_show_ip_arp); }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 141 "cli_parser.y"
    { HELP(HELP_SHOW_IP_ARP); }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 142 "cli_parser.y"
    { SETC_FUNC0(cli_show_ip_intf); }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 143 "cli_parser.y"
    { HELP(HELP_SHOW_IP_INTF); }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 144 "cli_parser.y"
    { SETC_FUNC0(cli_show_ip_route); }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 145 "cli_parser.y"
    { HELP(HELP_SHOW_IP_ROUTE); }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 146 "cli_parser.y"
    { HELP(HELP_SHOW_IP); }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 149 "cli_parser.y"
    { SETC_FUNC0(cli_show_ospf); }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 150 "cli_parser.y"
    { SETC_FUNC0(cli_show_ospf_neighbors); }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 151 "cli_parser.y"
    { HELP(HELP_SHOW_OSPF_NEIGHBORS); }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 152 "cli_parser.y"
    { SETC_FUNC0(cli_show_ospf_topo); }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 153 "cli_parser.y"
    { HELP(HELP_SHOW_OSPF_TOPOLOGY); }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 154 "cli_parser.y"
    { HELP(HELP_SHOW_OSPF); }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 158 "cli_parser.y"
    {ERR_TMI}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 162 "cli_parser.y"
    {ERR_WRONG}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 165 "cli_parser.y"
    { SETC_FUNC0(cli_show_vns); }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 166 "cli_parser.y"
    { SETC_FUNC0(cli_show_vns_lhost); }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 167 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_LHOST); }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 168 "cli_parser.y"
    { SETC_FUNC0(cli_show_vns_topo); }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 169 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_TOPOLOGY); }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 170 "cli_parser.y"
    { SETC_FUNC0(cli_show_vns_user); }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 171 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_USER); }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 172 "cli_parser.y"
    { SETC_FUNC0(cli_show_vns_vhost); }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 173 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_VHOST); }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 174 "cli_parser.y"
    { HELP(HELP_SHOW_VNS); }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 186 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP); }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 189 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_arp_purge_all); }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 190 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_PURGE_ALL); }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 191 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_arp_purge_dyn); }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 192 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_PURGE_DYN); }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 193 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_arp_purge_sta); }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 194 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_PURGE_STA); }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 197 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_ADD); }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 198 "cli_parser.y"
    {ERR_IP}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 198 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_ADD); }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 199 "cli_parser.y"
    {ERR_MAC}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 199 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_ADD); }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 200 "cli_parser.y"
    { SETC_ARP(cli_manip_ip_arp_add,(yyvsp[(1) - (2)].ip),(yyvsp[(2) - (2)].mac)); }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 201 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_ADD); }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 204 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_DEL); }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 205 "cli_parser.y"
    {ERR_IP}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 205 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_DEL); }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 206 "cli_parser.y"
    { SETC_ARP_IP(cli_manip_ip_arp_del,(yyvsp[(1) - (1)].ip)); }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 207 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_DEL); }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 210 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF); }
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 211 "cli_parser.y"
    {ERR_INTF}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 211 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF); }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 212 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF); }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 213 "cli_parser.y"
    {ERR_IP}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 213 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF_SET); }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 214 "cli_parser.y"
    { SETC_INTF_SET(cli_manip_ip_intf_set,(yyvsp[(1) - (3)].string),(yyvsp[(2) - (3)].ip),(yyvsp[(3) - (3)].ip)); }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 215 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF_SET); }
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 216 "cli_parser.y"
    { SETC_INTF(cli_manip_ip_intf_up,(yyvsp[(1) - (2)].string)); }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 217 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF_UP); }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 218 "cli_parser.y"
    { SETC_INTF(cli_manip_ip_intf_down,(yyvsp[(1) - (2)].string)); }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 219 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF_DOWN); }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 222 "cli_parser.y"
    { HELP(HELP_MANIP_IP_OSPF);           }
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 223 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_ospf_up);   }
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 224 "cli_parser.y"
    { HELP(HELP_MANIP_IP_OSPF_UP);        }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 225 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_ospf_down); }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 226 "cli_parser.y"
    { HELP(HELP_MANIP_IP_OSPF_DOWN);      }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 229 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE); }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 232 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_route_purge_all); }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 233 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_route_purge_dyn); }
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 234 "cli_parser.y"
    { SETC_FUNC0(cli_manip_ip_route_purge_sta); }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 235 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_PURGE_ALL); }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 238 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_ADD); }
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 239 "cli_parser.y"
    {ERR_IP}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 239 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_ADD); }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 240 "cli_parser.y"
    {ERR_IP}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 240 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_ADD); }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 241 "cli_parser.y"
    {ERR_IP}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 241 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_ADD); }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 242 "cli_parser.y"
    {ERR_INTF}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 242 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_ADD); }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 243 "cli_parser.y"
    { SETC_RT_ADD(cli_manip_ip_route_add,(yyvsp[(1) - (4)].ip),(yyvsp[(2) - (4)].ip),(yyvsp[(3) - (4)].ip),(yyvsp[(4) - (4)].string)); }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 244 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_ADD); }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 247 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_DEL); }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 248 "cli_parser.y"
    {ERR_IP}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 248 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_DEL); }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 249 "cli_parser.y"
    {ERR_IP}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 249 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_DEL); }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 250 "cli_parser.y"
    { SETC_RT(cli_manip_ip_route_del,(yyvsp[(1) - (2)].ip),(yyvsp[(2) - (2)].ip)); }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 251 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_DEL); }
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 262 "cli_parser.y"
    { HELP(HELP_ACTION_PING); }
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 263 "cli_parser.y"
    {ERR_IP}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 263 "cli_parser.y"
    { HELP(HELP_ACTION_PING); }
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 264 "cli_parser.y"
    { SETC_IP(cli_ping,(yyvsp[(1) - (1)].ip)); }
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 265 "cli_parser.y"
    { SETC_IP_INT(cli_ping_flood,(yyvsp[(2) - (2)].ip),100); }
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 266 "cli_parser.y"
    { HELP(HELP_ACTION_PING); }
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 267 "cli_parser.y"
    { SETC_IP_INT(cli_ping_flood,(yyvsp[(3) - (3)].ip),(yyvsp[(2) - (3)].intVal)); }
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 268 "cli_parser.y"
    { HELP(HELP_ACTION_PING); }
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 269 "cli_parser.y"
    { HELP(HELP_ACTION_PING); }
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 272 "cli_parser.y"
    { HELP(HELP_ACTION_TRACE); }
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 273 "cli_parser.y"
    {ERR_IP}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 273 "cli_parser.y"
    { HELP(HELP_ACTION_TRACE); }
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 274 "cli_parser.y"
    { SETC_IP(cli_traceroute,(yyvsp[(1) - (1)].ip)); }
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 275 "cli_parser.y"
    { HELP(HELP_ACTION_TRACE); }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 278 "cli_parser.y"
    { SETC_FUNC0(cli_date); }
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 279 "cli_parser.y"
    { HELP(HELP_ACTION_DATE); }
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 282 "cli_parser.y"
    { SETC_FUNC0(cli_exit); }
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 283 "cli_parser.y"
    { HELP(HELP_ACTION_EXIT); }
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 286 "cli_parser.y"
    { SETC_FUNC0(cli_shutdown); }
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 287 "cli_parser.y"
    { HELP(HELP_ACTION_SHUTDOWN); }
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 290 "cli_parser.y"
    { HELP(HELP_ACTION_HELP); }
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 291 "cli_parser.y"
    { HELP(HELP_ALL);         }
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 292 "cli_parser.y"
    {ERR_IGNORE}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 292 "cli_parser.y"
    { HELP(HELP_ALL); }
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 293 "cli_parser.y"
    { HELP(HELP_SHOW ); }
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 294 "cli_parser.y"
    { HELP(HELP_SHOW_HW); }
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 295 "cli_parser.y"
    { HELP(HELP_SHOW_HW_ABOUT); }
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 296 "cli_parser.y"
    { HELP(HELP_SHOW_HW_ARP); }
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 297 "cli_parser.y"
    { HELP(HELP_SHOW_HW_INTF); }
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 298 "cli_parser.y"
    { HELP(HELP_SHOW_HW_ROUTE); }
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 299 "cli_parser.y"
    { HELP(HELP_SHOW_IP); }
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 300 "cli_parser.y"
    { HELP(HELP_SHOW_IP_ARP); }
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 301 "cli_parser.y"
    { HELP(HELP_SHOW_IP_INTF); }
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 302 "cli_parser.y"
    { HELP(HELP_SHOW_IP_ROUTE); }
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 303 "cli_parser.y"
    { HELP(HELP_SHOW_OPT); }
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 304 "cli_parser.y"
    { HELP(HELP_SHOW_OPT_VERBOSE); }
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 305 "cli_parser.y"
    { HELP(HELP_SHOW_OSPF); }
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 306 "cli_parser.y"
    { HELP(HELP_SHOW_OSPF_NEIGHBORS); }
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 307 "cli_parser.y"
    { HELP(HELP_SHOW_OSPF_TOPOLOGY); }
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 308 "cli_parser.y"
    { HELP(HELP_SHOW_VNS); }
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 309 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_LHOST); }
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 310 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_TOPOLOGY); }
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 311 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_USER); }
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 312 "cli_parser.y"
    { HELP(HELP_SHOW_VNS_VHOST); }
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 313 "cli_parser.y"
    { HELP(HELP_MANIP_IP); }
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 314 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP); }
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 315 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_ADD); }
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 316 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_DEL); }
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 317 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_PURGE_ALL); }
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 318 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_PURGE_DYN); }
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 319 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ARP_PURGE_STA); }
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 320 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF); }
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 321 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF_DOWN); }
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 322 "cli_parser.y"
    { HELP(HELP_MANIP_IP_INTF_UP); }
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 323 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE); }
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 324 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_ADD); }
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 325 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_DEL); }
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 326 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_PURGE_ALL); }
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 327 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_PURGE_DYN); }
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 328 "cli_parser.y"
    { HELP(HELP_MANIP_IP_ROUTE_PURGE_STA); }
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 329 "cli_parser.y"
    { HELP(HELP_ACTION_DATE); }
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 330 "cli_parser.y"
    { HELP(HELP_ACTION_EXIT); }
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 331 "cli_parser.y"
    { HELP(HELP_ACTION_PING); }
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 332 "cli_parser.y"
    { HELP(HELP_ACTION_SHUTDOWN); }
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 333 "cli_parser.y"
    { HELP(HELP_ACTION_TRACE); }
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 334 "cli_parser.y"
    { HELP(HELP_OPT); }
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 335 "cli_parser.y"
    { HELP(HELP_OPT_VERBOSE); }
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 336 "cli_parser.y"
    { HELP(HELP_OPT); }
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 337 "cli_parser.y"
    { HELP(HELP_OPT_VERBOSE); }
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 338 "cli_parser.y"
    {ERR_IGNORE}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 338 "cli_parser.y"
    { HELP(HELP_ACTION_HELP); }
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 348 "cli_parser.y"
    { gopt.on = 1; }
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 349 "cli_parser.y"
    { gopt.on = 0; }
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 352 "cli_parser.y"
    { HELP(HELP_OPT); }
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 353 "cli_parser.y"
    { SETC_OPT(cli_opt_verbose); }
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 354 "cli_parser.y"
    { HELP(HELP_OPT_VERBOSE); }
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 357 "cli_parser.y"
    { SETC_FUNC0(cli_show_opt); }
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 358 "cli_parser.y"
    { SETC_FUNC0(cli_show_opt_verbose); }
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 359 "cli_parser.y"
    { HELP(HELP_SHOW_OPT_VERBOSE); }
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 360 "cli_parser.y"
    { HELP(HELP_SHOW_OPT); }
    break;



/* Line 1455 of yacc.c  */
#line 3044 "y.tab.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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
      if (yyn != YYPACT_NINF)
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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



/* Line 1675 of yacc.c  */
#line 362 "cli_parser.y"


static void clear_command() {
    gobj.func_do0 = NULL;
    gobj.func_do1 = NULL;
    gobj.data     = NULL;
    has_error     = 0;
    token_err[0] = '\0';
}

static void run_command() {
    if( has_error ) {
        if( token_err[0] != '\0' )
            cli_send_parse_error( 2, "Unexecpted Token Error: ", token_err );

        clear_command();
        return;
    }

    if( gobj.func_do0 ) {
        true_or_die( gobj.data == NULL, "Error: func_do0 and data set" );
        true_or_die( gobj.func_do1 == NULL, "Error: func_do1 set at same time as func_do0" );
        gobj.func_do0();
    }
    else if( gobj.func_do1 ) {
        true_or_die( gobj.data != NULL, "Error: func_do1 set but data is NULL" );
        gobj.func_do1( gobj.data );
    }

    clear_command();
}

void parse_error( const char* desc ) {
    if( !meh_desc ) meh_desc = desc;
    meh_has_usage = 1;
    meh_ignore = 0;
    meh_force = 0;
    has_error = 1;
}

void cli_parser_init() {
    yydebug = 0;
    meh_desc = NULL;
    has_error = meh_has_usage = meh_ignore = meh_force = 0;
    token_err[0] = '\0';
}

int cli_parser_handle_client( cli_client_t* client ) {
    YY_BUFFER_STATE yystate;
    char* sbuf;
    char* sbuf_loc;
    char* pch;
    int num;

    sbuf = client->state.chbuf;

    /* copy the command(s) */
    pch = strchr( sbuf, '\n' );
    true_or_die( pch!=NULL,
                 "Error: cli_parser_handle_client called when sbuf has no newline char" );
    do {
        cmd_blen = pch - sbuf + 1;
        memcpy( cmd_buf, sbuf, cmd_blen );

        /* add the two NUL bytes lex requires at the end */
        memset( cmd_buf + cmd_blen, '\0', 2 );

        /* give the buffer to lex (+2 => include NULs) */
        yystate = yy_scan_buffer( cmd_buf, cmd_blen + 2 );
        true_or_die( yystate != NULL,
                     "Error: the last 2 bytes are not NUL for flex?" );

        /* process the command(s) received */
        cli_scanner_init(); /* reset the line/col numbers */
        yyparse();
        yy_delete_buffer( yystate );

        /* bail out if socket closed or router shutting down */
        if( !cli_focus_is_alive() || cli_is_time_to_shutdown() ) {
            return 0; /* cleanup the connection */
        }

        /* continue searching from char after pch */
        sbuf_loc = pch + 1;
        pch = strchr( sbuf_loc, '\n' );
    }
    while( pch );

    /* move any extra bytes leftover to the front of the sbuf */
    num = client->state.used - client->state.needle_offset - 1;
    if( num > 0 ) {
        memmove( sbuf, sbuf+client->state.needle_offset, num );
        client->state.used = num;
    }
    else
        client->state.used = 0;

    /* we pulled all the needles out, so keep searching what is left */
    client->state.search_offset = 0;
    client->state.needle_offset = client->state.capacity + 1;

    cli_send_prompt();
    return cli_focus_is_alive(); /* true if client socket is still connected */
}

void yyerror( const char* desc ) {
    if( !meh_ignore ) {
        /* only print yyerror message if the manual error handler flag is unset */
        if( meh_desc ) {
            token_err[0] = '\0'; /* ignore any previous token errors */

            if( !meh_force && (strchr(yytext, '?') || strstr(yytext, "help")) )
                return; /* no-op: user handled it */
            else {
                cli_send_parse_error( 4,
                                      scanner_get_str_to_underline_error(STRLEN_PROMPT),
                                      "Syntax Error: ",
                                      meh_desc,
                                      meh_has_usage ? "\n  usage: " : "\n" );
            }
        }
        else
            strncpy( token_err, yytext, 128 );

        has_error = 1;
    }

    /* reset the manual error handler flag and desc */
    meh_desc = NULL;
    meh_ignore = 0;
    meh_force = 0;
}

