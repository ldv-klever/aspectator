/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse ldv_yyparse
#define yylex   ldv_yylex
#define yyerror ldv_yyerror
#define yylval  ldv_yylval
#define yychar  ldv_yychar
#define yydebug ldv_yydebug
#define yynerrs ldv_yynerrs
#define yylloc ldv_yylloc

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LDV_FILE = 258,
     LDV_BODY = 259,
     LDV_ID = 260,
     LDV_INT_NUMB = 261,
     LDV_TYPEDEF = 262,
     LDV_EXTERN = 263,
     LDV_STATIC = 264,
     LDV_AUTO = 265,
     LDV_REGISTER = 266,
     LDV_VOID = 267,
     LDV_CHAR = 268,
     LDV_INT = 269,
     LDV_FLOAT = 270,
     LDV_DOUBLE = 271,
     LDV_BOOL = 272,
     LDV_COMPLEX = 273,
     LDV_SHORT = 274,
     LDV_LONG = 275,
     LDV_SIGNED = 276,
     LDV_UNSIGNED = 277,
     LDV_STRUCT = 278,
     LDV_UNION = 279,
     LDV_ENUM = 280,
     LDV_TYPEDEF_NAME = 281,
     LDV_CONST = 282,
     LDV_RESTRICT = 283,
     LDV_VOLATILE = 284,
     LDV_INLINE = 285,
     LDV_AND = 286,
     LDV_ANY_PARAMS = 287,
     LDV_ELLIPSIS = 288,
     LDV_OR = 289
   };
#endif
/* Tokens.  */
#define LDV_FILE 258
#define LDV_BODY 259
#define LDV_ID 260
#define LDV_INT_NUMB 261
#define LDV_TYPEDEF 262
#define LDV_EXTERN 263
#define LDV_STATIC 264
#define LDV_AUTO 265
#define LDV_REGISTER 266
#define LDV_VOID 267
#define LDV_CHAR 268
#define LDV_INT 269
#define LDV_FLOAT 270
#define LDV_DOUBLE 271
#define LDV_BOOL 272
#define LDV_COMPLEX 273
#define LDV_SHORT 274
#define LDV_LONG 275
#define LDV_SIGNED 276
#define LDV_UNSIGNED 277
#define LDV_STRUCT 278
#define LDV_UNION 279
#define LDV_ENUM 280
#define LDV_TYPEDEF_NAME 281
#define LDV_CONST 282
#define LDV_RESTRICT 283
#define LDV_VOLATILE 284
#define LDV_INLINE 285
#define LDV_AND 286
#define LDV_ANY_PARAMS 287
#define LDV_ELLIPSIS 288
#define LDV_OR 289




/* Copy the first part of user declarations.  */
#line 25 "ldv-aspect-parser.y"


/* For ISALPHA, ISDIGIT, etc. functions. */
#include <safe-ctype.h>

/* General gcc core types and functions. */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"

/* For error functions. */
#include "diagnostic-core.h"
#include "toplev.h"

#include "ldv-aspect-parser.h"
#include "ldv-aspect-types.h"
#include "ldv-core.h"
#include "ldv-cpp-pointcut-matcher.h"
#include "ldv-io.h"


#define LDV_BODY_PATTERN_ARG                "arg"
#define LDV_BODY_PATTERN_ARG_TYPE           "arg_type"
#define LDV_BODY_PATTERN_ARG_SIZE           "arg_size"
#define LDV_BODY_PATTERN_ARG_VALUE          "arg_value"
#define LDV_BODY_PATTERN_ASPECT_FUNC_NAME   "aspect_func_name"
#define LDV_BODY_PATTERN_FUNC_NAME          "func_name"
#define LDV_BODY_PATTERN_PROCEED            "proceed"
#define LDV_BODY_PATTERN_RES                "res"
#define LDV_BODY_PATTERN_RET_TYPE           "ret_type"

#define ldv_set_first_line(val)    yylloc.first_line = (val); ldv_print_info (LDV_INFO_LEX, "%d: first line is \"%d\"", __LINE__, yylloc.first_line);
#define ldv_set_last_line(val)     yylloc.last_line = (val); ldv_print_info (LDV_INFO_LEX, "%d: last line is \"%d\"", __LINE__, yylloc.last_line);
#define ldv_set_first_column(val)  yylloc.first_column = (val); ldv_print_info (LDV_INFO_LEX, "%d: first column is \"%d\"", __LINE__, yylloc.first_column);
#define ldv_set_last_column(val)   yylloc.last_column = (val); ldv_print_info (LDV_INFO_LEX, "%d: last column is \"%d\"", __LINE__, yylloc.last_column);
#define ldv_set_file_name(val)     yylloc.file_name = (val); ldv_print_info (LDV_INFO_LEX, "%d: file name is \"%s\"", __LINE__, yylloc.file_name);


/* Specify own location tracking type. */
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
  const char *file_name;
} YYLTYPE;
#define yyltype YYLTYPE
#define YYLTYPE_IS_DECLARED 1

/* Relation between a keyword and a corresponding token. */
struct ldv_keyword_token
{
  const char *keyword;
  int token;
};

/* Map between keywords and tokens. */
struct ldv_keyword_token ldv_keyword_token_map [] = {
    {"typedef", LDV_TYPEDEF}
  , {"extern", LDV_EXTERN}
  , {"static", LDV_STATIC}
  , {"auto", LDV_AUTO}
  , {"register", LDV_REGISTER}
  , {"void", LDV_VOID}
  , {"char", LDV_CHAR}
  , {"int", LDV_INT}
  , {"float", LDV_FLOAT}
  , {"double", LDV_DOUBLE}
  , {"_Bool", LDV_BOOL}
  , {"_Complex", LDV_COMPLEX}
  , {"short", LDV_SHORT}
  , {"long", LDV_LONG}
  , {"signed", LDV_SIGNED}
  , {"unsigned", LDV_UNSIGNED}
  , {"struct", LDV_STRUCT}
  , {"union", LDV_UNION}
  , {"enum", LDV_ENUM}
  , {"const", LDV_CONST}
  , {"restrict", LDV_RESTRICT}
  , {"volatile", LDV_VOLATILE}
  , {"inline", LDV_INLINE}
  , {NULL, 0}
};

/* The named pointcuts list. */
static ldv_list_ptr ldv_n_pointcut_list = NULL;
/* Flag true if some type specifier was parsed and false otherwise.
 * It becomes false when declaration specifiers are parsed. */
static bool ldv_istype_spec = true;


static void ldv_check_pp_semantics (ldv_pp_ptr);
static ldv_cp_ptr ldv_create_c_pointcut (void);
static ldv_pps_ptr ldv_create_pp_signature (void);
static int ldv_get_id_kind (char *id);
static unsigned int ldv_parse_id (char **id);
static unsigned int ldv_parse_unsigned_integer (unsigned int *integer);
static void ldv_print_info_location (yyltype, const char *, const char *, ...) ATTRIBUTE_PRINTF_3;
static ldv_pps_ptr_quals_ptr ldv_set_ptr_quals (ldv_pps_declspecs_ptr);
static void yyerror (char const *, ...);
static int yylex (void);



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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 145 "ldv-aspect-parser.y"
{
  ldv_list_ptr list;
  ldv_file_ptr file;
  ldv_ab_ptr body;
  ldv_id_ptr id;
  ldv_int_ptr integer;
  ldv_np_ptr n_pointcut;
  ldv_adef_ptr a_definition;
  ldv_adecl_ptr a_declaration;
  ldv_cp_ptr c_pointcut;
  ldv_pp_ptr p_pointcut;
  ldv_pps_ptr pp_signature;
  ldv_pps_macro_ptr pps_macro;
  ldv_pps_file_ptr pps_file;
  ldv_pps_decl_ptr pps_declaration;
  ldv_pps_decl_ptr pps_decl;
  ldv_pps_declspecs_ptr pps_declspecs;
}
/* Line 187 of yacc.c.  */
#line 297 "ldv-aspect-parser.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 322 "ldv-aspect-parser.c"

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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   109

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNRULES -- Number of states.  */
#define YYNSTATES  130

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    39,     2,     2,     2,     2,
      37,    38,    43,     2,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    36,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    41,     2,    42,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    15,    18,    22,    24,
      26,    29,    33,    37,    41,    46,    48,    50,    53,    54,
      56,    58,    63,    65,    67,    71,    73,    75,    78,    81,
      84,    87,    90,    91,    93,    94,    95,    99,   101,   103,
     105,   107,   109,   111,   113,   115,   117,   119,   121,   123,
     125,   127,   129,   131,   132,   136,   137,   141,   142,   146,
     148,   150,   152,   154,   156,   159,   161,   165,   170,   175,
     176,   178,   181,   185,   186,   188,   189,   191,   193,   196,
     197,   200,   204,   206,   210,   212,   215,   218,   220,   223,
     224,   226,   230,   235,   240,   241,   243,   244
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,    -1,    45,    47,    -1,    45,    46,    -1,
       5,     5,    36,    49,    -1,    48,     4,    -1,     5,    36,
      49,    -1,     5,    -1,    50,    -1,    35,    49,    -1,    49,
      34,    49,    -1,    49,    31,    49,    -1,    37,    49,    38,
      -1,     5,    37,    51,    38,    -1,    53,    -1,    54,    -1,
      52,    56,    -1,    -1,    39,    -1,     5,    -1,     5,    37,
      55,    38,    -1,     3,    -1,     5,    -1,    55,    40,     5,
      -1,    57,    -1,    60,    -1,    60,    70,    -1,    63,    59,
      -1,    64,    59,    -1,    68,    59,    -1,    69,    59,    -1,
      -1,    58,    -1,    -1,    -1,    61,    58,    62,    -1,     7,
      -1,     8,    -1,     9,    -1,    10,    -1,    11,    -1,    12,
      -1,    13,    -1,    14,    -1,    15,    -1,    16,    -1,    17,
      -1,    18,    -1,    19,    -1,    20,    -1,    21,    -1,    22,
      -1,    -1,    23,    65,     5,    -1,    -1,    24,    66,     5,
      -1,    -1,    25,    67,     5,    -1,    26,    -1,    27,    -1,
      28,    -1,    29,    -1,    30,    -1,    72,    71,    -1,     5,
      -1,    37,    70,    38,    -1,    71,    41,    74,    42,    -1,
      71,    37,    77,    38,    -1,    -1,    73,    -1,    43,    75,
      -1,    43,    75,    73,    -1,    -1,     6,    -1,    -1,    76,
      -1,    68,    -1,    76,    68,    -1,    -1,    79,    78,    -1,
      79,    40,    33,    -1,    80,    -1,    79,    40,    80,    -1,
      32,    -1,    60,    70,    -1,    60,    82,    -1,    73,    -1,
      72,    83,    -1,    -1,    81,    -1,    37,    81,    38,    -1,
      84,    41,    74,    42,    -1,    83,    37,    85,    38,    -1,
      -1,    83,    -1,    -1,    77,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   261,   261,   263,   268,   274,   306,   328,   367,   401,
     417,   433,   451,   469,   477,   537,   551,   565,   584,   586,
     589,   606,   625,   641,   649,   657,   663,   682,   743,   753,
     763,   773,   786,   789,   795,   795,   795,   801,   813,   825,
     837,   849,   863,   876,   889,   902,   915,   928,   941,   954,
     972,   990,  1008,  1026,  1026,  1040,  1040,  1054,  1054,  1068,
    1084,  1096,  1108,  1122,  1136,  1144,  1161,  1167,  1192,  1211,
    1214,  1220,  1237,  1256,  1259,  1266,  1269,  1275,  1279,  1291,
    1291,  1303,  1320,  1338,  1357,  1370,  1389,  1410,  1416,  1425,
    1434,  1440,  1446,  1472,  1491,  1494,  1501,  1504
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LDV_FILE", "LDV_BODY", "LDV_ID",
  "LDV_INT_NUMB", "LDV_TYPEDEF", "LDV_EXTERN", "LDV_STATIC", "LDV_AUTO",
  "LDV_REGISTER", "LDV_VOID", "LDV_CHAR", "LDV_INT", "LDV_FLOAT",
  "LDV_DOUBLE", "LDV_BOOL", "LDV_COMPLEX", "LDV_SHORT", "LDV_LONG",
  "LDV_SIGNED", "LDV_UNSIGNED", "LDV_STRUCT", "LDV_UNION", "LDV_ENUM",
  "LDV_TYPEDEF_NAME", "LDV_CONST", "LDV_RESTRICT", "LDV_VOLATILE",
  "LDV_INLINE", "\"&&\"", "\"..\"", "\"...\"", "\"||\"", "'!'", "':'",
  "'('", "')'", "'#'", "','", "'['", "']'", "'*'", "$accept", "input",
  "named_pointcut", "advice_definition", "advice_declaration",
  "composite_pointcut", "primitive_pointcut",
  "primitive_pointcut_signature", "hash_opt",
  "primitive_pointcut_signature_macro",
  "primitive_pointcut_signature_file", "macro_param",
  "primitive_pointcut_signature_declaration", "c_declaration",
  "c_declaration_specifiers", "c_declaration_specifiers_opt",
  "c_declaration_specifiers_aux", "@1", "@2", "c_storage_class_specifier",
  "c_type_specifier", "@3", "@4", "@5", "c_type_qualifier",
  "c_function_specifier", "c_declarator", "c_direct_declarator",
  "c_pointer_opt", "c_pointer", "int_opt", "c_type_qualifier_list_opt",
  "c_type_qualifier_list", "c_parameter_type_list", "@6",
  "c_parameter_list", "c_parameter_declaration", "c_abstract_declarator",
  "c_abstract_declarator_opt", "c_direct_abstract_declarator",
  "c_direct_abstract_declarator_opt", "c_parameter_type_list_opt", 0
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
     285,   286,   287,   288,   289,    33,    58,    40,    41,    35,
      44,    91,    93,    42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    45,    45,    46,    47,    48,    49,    49,
      49,    49,    49,    49,    50,    51,    51,    51,    52,    52,
      53,    53,    54,    55,    55,    56,    57,    57,    58,    58,
      58,    58,    59,    59,    61,    62,    60,    63,    63,    63,
      63,    63,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    65,    64,    66,    64,    67,    64,    64,
      68,    68,    68,    69,    70,    71,    71,    71,    71,    72,
      72,    73,    73,    74,    74,    75,    75,    76,    76,    78,
      77,    77,    79,    79,    80,    80,    80,    81,    81,    82,
      82,    83,    83,    83,    84,    84,    85,    85
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     4,     2,     3,     1,     1,
       2,     3,     3,     3,     4,     1,     1,     2,     0,     1,
       1,     4,     1,     1,     3,     1,     1,     2,     2,     2,
       2,     2,     0,     1,     0,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     0,     3,     0,     3,     1,
       1,     1,     1,     1,     2,     1,     3,     4,     4,     0,
       1,     2,     3,     0,     1,     0,     1,     1,     2,     0,
       2,     3,     1,     3,     1,     2,     2,     1,     2,     0,
       1,     3,     4,     4,     0,     1,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     4,     3,     0,     0,     0,     6,
       0,     8,     0,     0,     7,     9,     5,    18,    10,     0,
       0,     0,    22,    20,    19,     0,    34,    15,    16,    13,
      12,    11,     0,    14,    17,    25,    69,     0,    23,     0,
      75,    27,     0,    70,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    55,    57,    59,    60,    61,    62,    63,    35,    32,
      32,    32,    32,    21,     0,    77,    71,    76,    65,    69,
      64,     0,     0,     0,    36,    33,    28,    29,    30,    31,
      24,    72,    78,     0,    34,    73,    54,    56,    58,    66,
      84,    69,     0,    79,    82,    74,     0,    85,    94,    70,
      90,    86,    68,    34,    80,    67,    69,    88,     0,    81,
      83,     0,    34,    73,    91,    97,     0,     0,    93,    92
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     4,     5,     6,    14,    15,    25,    26,    27,
      28,    39,    34,    35,    85,    86,   101,    37,    84,    69,
      70,    81,    82,    83,    71,    72,    93,    80,    42,    43,
     106,    76,    77,   102,   114,   103,   104,   110,   111,   117,
     118,   126
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -94
static const yytype_int8 yypact[] =
{
     -94,    24,   -94,     2,   -94,   -94,    40,     9,    -2,   -94,
      -2,    17,    -2,    -2,    12,   -94,    12,     1,   -94,   -17,
      -2,    -2,   -94,    19,   -94,    21,   -94,   -94,   -94,   -94,
     -94,    29,    56,   -94,   -94,   -94,   -13,    59,   -94,    -4,
      20,   -94,     0,   -94,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,    59,
      59,    59,    59,   -94,    57,   -94,    47,    20,   -94,    47,
     -10,    58,    86,    87,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,   -94,   -94,    26,    61,    88,   -94,   -94,   -94,   -94,
     -94,   -25,    60,    55,   -94,   -94,    54,   -94,     5,    15,
     -94,   -94,   -94,    25,   -94,   -94,    47,    -9,    62,   -94,
     -94,    63,   -16,    88,   -94,   -94,    64,    65,   -94,   -94
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -94,   -94,   -94,   -94,   -94,    -1,   -94,   -94,   -94,   -94,
     -94,   -94,   -94,   -94,    67,   -20,    71,   -94,   -94,   -94,
     -94,   -94,   -94,   -94,   -38,   -94,   -36,   -94,   -93,   -75,
     -24,   -94,   -94,   -22,   -94,   -94,    -8,    -7,   -94,   -94,
     -94,   -94
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -97
static const yytype_int16 yytable[] =
{
      41,    91,    75,    11,    22,    78,    23,     7,   108,    16,
      78,    18,    19,   -89,    20,   -89,   100,    21,    40,    30,
      31,    29,   -96,   108,     2,   -26,   109,    94,   122,     3,
      40,    95,   -95,    12,    73,    13,    74,    79,     8,    92,
      24,   109,   116,    20,     9,    10,    21,    64,    65,    66,
      87,    88,    89,   -87,    17,   -87,    32,   100,   119,    33,
      20,    38,    90,    96,    99,   107,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      40,    97,    98,   100,   105,   113,   115,    36,   112,   127,
     125,   124,   128,   123,    68,   120,     0,   129,     0,   121
};

static const yytype_int8 yycheck[] =
{
      36,    76,    40,     5,     3,     5,     5,     5,   101,    10,
       5,    12,    13,    38,    31,    40,    32,    34,    43,    20,
      21,    38,    38,   116,     0,    38,   101,    37,    37,     5,
      43,    41,    41,    35,    38,    37,    40,    37,    36,    77,
      39,   116,    37,    31,     4,    36,    34,    27,    28,    29,
      70,    71,    72,    38,    37,    40,    37,    32,    33,    38,
      31,     5,     5,     5,    38,   101,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      43,     5,     5,    32,     6,    40,    42,    26,    38,   123,
     122,    38,    38,    41,    37,   113,    -1,    42,    -1,   116
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    45,     0,     5,    46,    47,    48,     5,    36,     4,
      36,     5,    35,    37,    49,    50,    49,    37,    49,    49,
      31,    34,     3,     5,    39,    51,    52,    53,    54,    38,
      49,    49,    37,    38,    56,    57,    60,    61,     5,    55,
      43,    70,    72,    73,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    58,    63,
      64,    68,    69,    38,    40,    68,    75,    76,     5,    37,
      71,    65,    66,    67,    62,    58,    59,    59,    59,    59,
       5,    73,    68,    70,    37,    41,     5,     5,     5,    38,
      32,    60,    77,    79,    80,     6,    74,    70,    72,    73,
      81,    82,    38,    40,    78,    42,    37,    83,    84,    33,
      80,    81,    37,    41,    38,    77,    85,    74,    38,    42
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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif


  /* User initialization code.  */
#line 247 "ldv-aspect-parser.y"
{
  /* Initialize the beginning location and aspect file. */
  yylloc.first_line = yylloc.last_line = 1;
  ldv_set_first_line (yylloc.first_line);
  ldv_set_last_line (yylloc.last_line);
  yylloc.first_column = yylloc.last_column = 1;
  ldv_set_first_column (yylloc.first_column);
  ldv_set_last_column (yylloc.last_column);
  yylloc.file_name = ldv_aspect_fname;
  ldv_set_file_name (yylloc.file_name);
}
/* Line 1069 of yacc.c.  */
#line 1508 "ldv-aspect-parser.c"
  yylsp[0] = yylloc;
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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 264 "ldv-aspect-parser.y"
    {
      /* Add advice definition from corresponding rule to the advice definitions list. */
      ldv_list_push_back (&ldv_adef_list, (yyvsp[(2) - (2)].a_definition));
    ;}
    break;

  case 4:
#line 269 "ldv-aspect-parser.y"
    {
      ldv_list_push_back (&ldv_n_pointcut_list, (yyvsp[(2) - (2)].n_pointcut));
    ;}
    break;

  case 5:
#line 275 "ldv-aspect-parser.y"
    {
      ldv_np_ptr n_pointcut_new = NULL;
      char *p_keyword = NULL;

      /* Set pointcut keyword from lexer identifier. */
      p_keyword = ldv_get_id_name ((yyvsp[(1) - (4)].id));

      if (strcmp ("pointcut", p_keyword))
        {
          ldv_print_info_location ((yylsp[(1) - (4)]), LDV_ERROR_BISON, "incorrect keyword \"%s\" was used", p_keyword);
          LDV_FATAL_ERROR ("use one of the following keywords: \"pointcut\", \"before\", \"after\", \"around\", \"new\"");
        }

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[(1) - (4)].id));

      n_pointcut_new = XCNEW (ldv_named_pointcut);
      ldv_print_info (LDV_INFO_MEM, "named pointcut memory was released");

      /* Set a pointcut name from a lexer identifier. */
      n_pointcut_new->p_name = ldv_get_id_name ((yyvsp[(2) - (4)].id));

      /* Set a composite pointcut of a named pointcut from a corresponding rule. */
      n_pointcut_new->c_pointcut = (yyvsp[(4) - (4)].c_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed named pointcut");

      (yyval.n_pointcut) = n_pointcut_new;
    ;}
    break;

  case 6:
#line 307 "ldv-aspect-parser.y"
    {
      ldv_adef_ptr a_definition_new = NULL;

      a_definition_new = XCNEW (ldv_advice_definition);
      ldv_print_info (LDV_INFO_MEM, "advice definition memory was released");

      /* Set an advice definition declaration from a corresponding rule. */
      a_definition_new->a_declaration = (yyvsp[(1) - (2)].a_declaration);

      /* Set and advice definition body from a lexer body. */
      a_definition_new->a_body = (yyvsp[(2) - (2)].body);

      /* Set that an advice wasn't used at the beginning. */
      a_definition_new->use_counter = 0;

      ldv_print_info (LDV_INFO_BISON, "bison parsed advice definition");

      (yyval.a_definition) = a_definition_new;
    ;}
    break;

  case 7:
#line 329 "ldv-aspect-parser.y"
    {
      char *a_kind = NULL;
      ldv_adecl_ptr a_declaration = NULL;

      /* Set an advice declaration kind from a lexer identifier. */
      a_kind = ldv_get_id_name ((yyvsp[(1) - (3)].id));

      a_declaration = XCNEW (ldv_advice_declaration);
      ldv_print_info (LDV_INFO_MEM, "advice declaration memory was released");

      /* Set a corresponding advice kind. */
      if (!strcmp ("after", a_kind))
        a_declaration->a_kind = LDV_A_AFTER;
      else if (!strcmp ("before", a_kind))
        a_declaration->a_kind = LDV_A_BEFORE;
      else if (!strcmp ("around", a_kind))
        a_declaration->a_kind = LDV_A_AROUND;
      else if (!strcmp ("new", a_kind))
        a_declaration->a_kind = LDV_A_NEW;
      else
        {
          ldv_print_info_location ((yylsp[(1) - (3)]), LDV_ERROR_BISON, "incorrect advice declaration kind \"%s\" was used", a_kind);
          LDV_FATAL_ERROR ("use \"after\", \"around\", \"before\", \"new\" advice declaration kind");
        }

      /* Set a composite pointcut from a corresponding rule. */
      a_declaration->c_pointcut = (yyvsp[(3) - (3)].c_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" advice declaration", a_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[(1) - (3)].id));

      (yyval.a_declaration) = a_declaration;
    ;}
    break;

  case 8:
#line 368 "ldv-aspect-parser.y"
    {
      ldv_np_ptr n_pointcut = NULL;
      ldv_list_ptr n_pointcut_list = NULL;
      char *p_name = NULL;
      ldv_cp_ptr c_pointcut = NULL;

      /* Set a pointcut name from a lexer identifier. */
      p_name = ldv_get_id_name ((yyvsp[(1) - (1)].id));

      c_pointcut = NULL;

      /* Walk through a named pointcuts list to find matches. */
      for (n_pointcut_list = ldv_n_pointcut_list; n_pointcut_list ; n_pointcut_list = ldv_list_get_next (n_pointcut_list))
        {
          n_pointcut = (ldv_np_ptr) ldv_list_get_data (n_pointcut_list);

          if (!strcmp(n_pointcut->p_name, p_name))
            {
              ldv_print_info (LDV_INFO_MATCH, "pointcut with name \"%s\" was matched", p_name);
              c_pointcut = n_pointcut->c_pointcut;
              break;
            }
        }

      ldv_print_info (LDV_INFO_BISON, "bison parsed named composite pointcut");

      if (c_pointcut)
        (yyval.c_pointcut) = c_pointcut;
      else
        {
          LDV_FATAL_ERROR ("undefined pointcut with name \"%s\" was used", p_name);
        }
    ;}
    break;

  case 9:
#line 402 "ldv-aspect-parser.y"
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_PRIMITIVE;

      /* Set a primitive pointcut from a corresponding rule. */
      c_pointcut->p_pointcut = (yyvsp[(1) - (1)].p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"primitive\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    ;}
    break;

  case 10:
#line 418 "ldv-aspect-parser.y"
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_NOT;

      /* Set a composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = (yyvsp[(2) - (2)].c_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"not\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    ;}
    break;

  case 11:
#line 434 "ldv-aspect-parser.y"
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_OR;

      /* Set a first composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = (yyvsp[(1) - (3)].c_pointcut);
      /* Set a second composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_second = (yyvsp[(3) - (3)].c_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"or\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    ;}
    break;

  case 12:
#line 452 "ldv-aspect-parser.y"
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_AND;

      /* Set a first composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = (yyvsp[(1) - (3)].c_pointcut);
      /* Set a second composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_second = (yyvsp[(3) - (3)].c_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"and\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    ;}
    break;

  case 13:
#line 470 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed composite pointcut of associativity");

      (yyval.c_pointcut) = (yyvsp[(2) - (3)].c_pointcut);
    ;}
    break;

  case 14:
#line 478 "ldv-aspect-parser.y"
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ((yyvsp[(1) - (4)].id));

      p_pointcut = XCNEW (ldv_primitive_pointcut);
      ldv_print_info (LDV_INFO_MEM, "primitive pointcut memory was released");

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("call", pp_kind))
        p_pointcut->pp_kind = LDV_PP_CALL;
      else if (!strcmp ("define", pp_kind))
        p_pointcut->pp_kind = LDV_PP_DEFINE;
      else if (!strcmp ("execution", pp_kind))
        p_pointcut->pp_kind = LDV_PP_EXECUTION;
      else if (!strcmp ("file", pp_kind))
        p_pointcut->pp_kind = LDV_PP_FILE;
      else if (!strcmp ("get", pp_kind))
        p_pointcut->pp_kind = LDV_PP_GET;
      else if (!strcmp ("get_global", pp_kind))
        p_pointcut->pp_kind = LDV_PP_GET_GLOBAL;
      else if (!strcmp ("get_local", pp_kind))
        p_pointcut->pp_kind = LDV_PP_GET_LOCAL;
      else if (!strcmp ("incall", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INCALL;
      else if (!strcmp ("infile", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INFILE;
      else if (!strcmp ("infunc", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INFUNC;
      else if (!strcmp ("introduce", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INTRODUCE;
      else if (!strcmp ("set", pp_kind))
        p_pointcut->pp_kind = LDV_PP_SET;
      else if (!strcmp ("set_global", pp_kind))
        p_pointcut->pp_kind = LDV_PP_SET_GLOBAL;
      else if (!strcmp ("set_local", pp_kind))
        p_pointcut->pp_kind = LDV_PP_SET_LOCAL;
      else
        {
          ldv_print_info_location ((yylsp[(1) - (4)]), LDV_ERROR_BISON, "incorrect primitive pointcut kind \"%s\" was used", pp_kind);
          LDV_FATAL_ERROR ("use \"call\", \"define\", \"execution\", \"file\", \"get\", \"get_global\", \"get_local\", \"incall\", \"infile\", \"infunc\", \"introduce\", \"set\", \"set_global\", \"set_local\" primitive pointcut kind");
        }

      /* Set a primitive pointcut signature from a corresponding rule. */
      p_pointcut->pp_signature = (yyvsp[(3) - (4)].pp_signature);

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[(1) - (4)].id));

      (yyval.p_pointcut) = p_pointcut;
    ;}
    break;

  case 15:
#line 538 "ldv-aspect-parser.y"
    {
      ldv_pps_ptr pp_signature = NULL;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a macro signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_DEFINE;

      /* Specify a macro signature. */
      pp_signature->pps_macro = (yyvsp[(1) - (1)].pps_macro);

      (yyval.pp_signature) = pp_signature;
    ;}
    break;

  case 16:
#line 552 "ldv-aspect-parser.y"
    {
      ldv_pps_ptr pp_signature = NULL;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a file signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_FILE;

      /* Specify a file signature. */
      pp_signature->pps_file = (yyvsp[(1) - (1)].pps_file);

      (yyval.pp_signature) = pp_signature;
    ;}
    break;

  case 17:
#line 566 "ldv-aspect-parser.y"
    {
      ldv_pps_ptr pp_signature = NULL;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a declaration signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_DECL;

      /* Specify a declaration signature. */
      pp_signature->pps_declaration = (yyvsp[(2) - (2)].pps_decl);

      (yyval.pp_signature) = pp_signature;
    ;}
    break;

  case 20:
#line 590 "ldv-aspect-parser.y"
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);
      ldv_print_info (LDV_INFO_MEM, "macro primitive pointcut signature memory was released");

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = (yyvsp[(1) - (1)].id);

      /* Specify that there is no macro parameters at all. */
      macro->macro_param_list = NULL;

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro signature");

      (yyval.pps_macro) = macro;
    ;}
    break;

  case 21:
#line 607 "ldv-aspect-parser.y"
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);
      ldv_print_info (LDV_INFO_MEM, "macro primitive pointcut signature memory was released");

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = (yyvsp[(1) - (4)].id);

      /* Set a macro function parameters from a corresponding rule. */
      macro->macro_param_list = (yyvsp[(3) - (4)].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro function signature");

      (yyval.pps_macro) = macro;
    ;}
    break;

  case 22:
#line 626 "ldv-aspect-parser.y"
    {
      ldv_pps_file_ptr file = NULL;

      file = XCNEW (ldv_primitive_pointcut_signature_file);
      ldv_print_info (LDV_INFO_MEM, "file primitive pointcut signature memory was released");

      /* Set a function name from a lexer identifier. */
      file->file_name = (yyvsp[(1) - (1)].file);

      ldv_print_info (LDV_INFO_BISON, "bison parsed file signature");

      (yyval.pps_file) = file;
    ;}
    break;

  case 23:
#line 642 "ldv-aspect-parser.y"
    {
      ldv_list_ptr macro_param_list = NULL;

      ldv_list_push_back (&macro_param_list, (yyvsp[(1) - (1)].id));

      (yyval.list) = macro_param_list;
    ;}
    break;

  case 24:
#line 650 "ldv-aspect-parser.y"
    {
      ldv_list_push_back (&(yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].id));

      (yyval.list) = (yyvsp[(1) - (3)].list);
    ;}
    break;

  case 25:
#line 658 "ldv-aspect-parser.y"
    {
      (yyval.pps_decl) = (yyvsp[(1) - (1)].pps_decl);
    ;}
    break;

  case 26:
#line 664 "ldv-aspect-parser.y"
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = (yyvsp[(1) - (1)].pps_declspecs);

      /* Specify an empty declarator, i.e. no name is declared. */
      pps_decl->pps_declarator = NULL;

      /* Specify a kind of a declaration, a type. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_TYPE;

      ldv_print_info (LDV_INFO_BISON, "bison parsed type declaration signature pointcut");

      (yyval.pps_decl) = pps_decl;
    ;}
    break;

  case 27:
#line 683 "ldv-aspect-parser.y"
    {
      ldv_pps_decl_ptr decl = NULL;
      ldv_pps_declarator_ptr declarator = NULL;
      ldv_list_ptr declarator_list = NULL;
      bool isdecl_kind_specified = false;

      decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers and a declarator chain. */
      decl->pps_declspecs = (yyvsp[(1) - (2)].pps_declspecs);
      decl->pps_declarator = (yyvsp[(2) - (2)].list);

      /* Specify a kind of a declaration, a function or a variable. */
      for (declarator_list = (yyvsp[(2) - (2)].list)
        ;
        ; declarator_list = ldv_list_get_next (declarator_list))
        {
          declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

          /* We deal with a variable declaration in this case. */
          if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_ID)
            {
              decl->pps_decl_kind = LDV_PPS_DECL_VAR;
              isdecl_kind_specified = true;

              ldv_print_info (LDV_INFO_BISON, "bison parsed variable declaration signature pointcut");

              (yyval.pps_decl) = decl;

              break;
            }
          /* We deal with a function declaration in this case and a following
             declarator in a declarator chain must be a function name
             declaration. */
          else if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_FUNC)
            {
              declarator_list = ldv_list_get_next (declarator_list);
              declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

              if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_ID)
                {
                  decl->pps_decl_kind = LDV_PPS_DECL_FUNC;
                  isdecl_kind_specified = true;

                  ldv_print_info (LDV_INFO_BISON, "bison parsed function declaration signature pointcut");

                  (yyval.pps_decl) = decl;

                  break;
                }
            }
        }

      if (!isdecl_kind_specified)
        {
          LDV_FATAL_ERROR ("declaration kind can't be determined");
        }
    ;}
    break;

  case 28:
#line 744 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ((yyvsp[(1) - (2)].pps_declspecs), (yyvsp[(2) - (2)].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 29:
#line 754 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ((yyvsp[(1) - (2)].pps_declspecs), (yyvsp[(2) - (2)].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 30:
#line 764 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ((yyvsp[(1) - (2)].pps_declspecs), (yyvsp[(2) - (2)].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 31:
#line 774 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ((yyvsp[(1) - (2)].pps_declspecs), (yyvsp[(2) - (2)].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 32:
#line 786 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = NULL;
    ;}
    break;

  case 33:
#line 790 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = (yyvsp[(1) - (1)].pps_declspecs);
    ;}
    break;

  case 34:
#line 795 "ldv-aspect-parser.y"
    { ldv_istype_spec = false; ;}
    break;

  case 35:
#line 795 "ldv-aspect-parser.y"
    { ldv_istype_spec = true; ;}
    break;

  case 36:
#line 796 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = (yyvsp[(2) - (3)].pps_declspecs);
    ;}
    break;

  case 37:
#line 802 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"typedef\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 38:
#line 814 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isextern = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"extern\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 39:
#line 826 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstatic = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"static\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 40:
#line 838 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isauto = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"auto\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 41:
#line 850 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isregister = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"register\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 42:
#line 864 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvoid = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"void\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 43:
#line 877 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->ischar = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"char\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 44:
#line 890 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isint = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"int\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 45:
#line 903 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isfloat = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"float\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 46:
#line 916 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isdouble = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"double\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 47:
#line 929 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isbool = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"bool\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 48:
#line 942 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->iscomplex = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"complex\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 49:
#line 955 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isshort = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      /* Assume that 'int' presents if 'short' is encountered. Don't do this if
         'double' type specifier was already parsed. */
      if (!pps_declspecs->isdouble)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"short\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 50:
#line 973 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->islong = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      /* Assume that 'int' presents if 'long' is encountered. Don't do this if
       * 'double' type specifier was already parsed. */
      if (!pps_declspecs->isdouble)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"long\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 51:
#line 991 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->issigned = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      /* Assume that 'int' presents if 'signed' is encountered. Don't do this if
       * 'double' or 'char' type specifier was already parsed. */
      if (!pps_declspecs->isdouble && !pps_declspecs->ischar)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"signed\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 52:
#line 1009 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isunsigned = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      /* Assume that 'int' presents if 'unsigned' is encountered. Don't do this if
       * 'double' or 'char' type specifier was already parsed. */
      if (!pps_declspecs->isdouble && !pps_declspecs->ischar)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"unsigned\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 53:
#line 1026 "ldv-aspect-parser.y"
    { ldv_istype_spec = true; ;}
    break;

  case 54:
#line 1027 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstruct = true;
      pps_declspecs->type_name = (yyvsp[(3) - (3)].id);
      pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"struct\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 55:
#line 1040 "ldv-aspect-parser.y"
    { ldv_istype_spec = true; ;}
    break;

  case 56:
#line 1041 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isunion = true;
      pps_declspecs->type_name = (yyvsp[(3) - (3)].id);
      pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"union\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 57:
#line 1054 "ldv-aspect-parser.y"
    { ldv_istype_spec = true; ;}
    break;

  case 58:
#line 1055 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isenum = true;
      pps_declspecs->type_name = (yyvsp[(3) - (3)].id);
      pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"enum\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 59:
#line 1069 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef_name = true;
      pps_declspecs->type_name = (yyvsp[(1) - (1)].id);
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed typedef name \"%s\" type specifier", ldv_get_id_name ((yyvsp[(1) - (1)].id)));

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 60:
#line 1085 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isconst = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"const\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 61:
#line 1097 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isrestrict = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"restrict\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 62:
#line 1109 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvolatile = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"volatile\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 63:
#line 1123 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isinline = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"inline\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 64:
#line 1137 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed declarator");

      (yyval.list) = ldv_list_splice ((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].list));
    ;}
    break;

  case 65:
#line 1145 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;
      ldv_list_ptr declarator_list = NULL;

      declarator_new = ldv_create_declarator ();

      declarator_list = ldv_list_push_front (NULL, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ID;

      declarator_new->declarator_name = (yyvsp[(1) - (1)].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed identifier direct declarator");

      (yyval.list) = declarator_list;
    ;}
    break;

  case 66:
#line 1162 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct declarator of associativity");

      (yyval.list) = (yyvsp[(2) - (3)].list);
    ;}
    break;

  case 67:
#line 1168 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[(1) - (4)].list) = ldv_list_push_front ((yyvsp[(1) - (4)].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ARRAY;

      declarator_new->pps_array_size = ldv_create_pps_array_size ();

      /* If an array size was specified then save it. */
      if ((yyvsp[(3) - (4)].integer))
        {
          declarator_new->pps_array_size->pps_array_size = ldv_get_int ((yyvsp[(3) - (4)].integer));
          declarator_new->pps_array_size->issize_specified = true;
        }
      else
        declarator_new->pps_array_size->issize_specified = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed array direct declarator");

      (yyval.list) = (yyvsp[(1) - (4)].list);
    ;}
    break;

  case 68:
#line 1193 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[(1) - (4)].list) = ldv_list_push_front ((yyvsp[(1) - (4)].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = (yyvsp[(3) - (4)].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct declarator");

      (yyval.list) = (yyvsp[(1) - (4)].list);
    ;}
    break;

  case 69:
#line 1211 "ldv-aspect-parser.y"
    {
      (yyval.list) = NULL;
    ;}
    break;

  case 70:
#line 1215 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[(1) - (1)].list);
    ;}
    break;

  case 71:
#line 1221 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;
      ldv_list_ptr declarator_list = NULL;

      declarator_new = ldv_create_declarator ();

      declarator_list = ldv_list_push_front (NULL, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      declarator_new->pps_ptr_quals = ldv_set_ptr_quals ((yyvsp[(2) - (2)].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison parsed pointer declarator");

      (yyval.list) = declarator_list;
    ;}
    break;

  case 72:
#line 1238 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[(3) - (3)].list) = ldv_list_push_front ((yyvsp[(3) - (3)].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      declarator_new->pps_ptr_quals = ldv_set_ptr_quals ((yyvsp[(2) - (3)].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison parsed pointer declarator");

      (yyval.list) = (yyvsp[(3) - (3)].list);
    ;}
    break;

  case 73:
#line 1256 "ldv-aspect-parser.y"
    {
      (yyval.integer) = NULL;
    ;}
    break;

  case 74:
#line 1260 "ldv-aspect-parser.y"
    {
      (yyval.integer) = (yyvsp[(1) - (1)].integer);
    ;}
    break;

  case 75:
#line 1266 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = NULL;
    ;}
    break;

  case 76:
#line 1270 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = (yyvsp[(1) - (1)].pps_declspecs);
    ;}
    break;

  case 77:
#line 1276 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = (yyvsp[(1) - (1)].pps_declspecs);
    ;}
    break;

  case 78:
#line 1280 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ((yyvsp[(1) - (2)].pps_declspecs), (yyvsp[(2) - (2)].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison merged type qualifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    ;}
    break;

  case 79:
#line 1291 "ldv-aspect-parser.y"
    { /* It's a hack!!! It's needed to alow a correct processing of typedefs names inside a parameter list. */ ldv_istype_spec = true; ;}
    break;

  case 80:
#line 1292 "ldv-aspect-parser.y"
    {
      ldv_pps_func_arg_ptr func_arg = NULL;

      func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data ((yyvsp[(1) - (2)].list));

      func_arg->isva = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter list");

      (yyval.list) = (yyvsp[(1) - (2)].list);
    ;}
    break;

  case 81:
#line 1304 "ldv-aspect-parser.y"
    {
      ldv_pps_func_arg_ptr func_arg = NULL;
      ldv_list_ptr func_arg_list = NULL;

      func_arg_list = ldv_list_get_last ((yyvsp[(1) - (3)].list));

      func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (func_arg_list);

      func_arg->isva = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter list of variable length");

      (yyval.list) = (yyvsp[(1) - (3)].list);
    ;}
    break;

  case 82:
#line 1321 "ldv-aspect-parser.y"
    {
      ldv_pps_func_arg_ptr pps_func_arg_new = NULL;
      ldv_list_ptr func_arg_list = NULL;

      pps_func_arg_new = ldv_create_pps_func_arg ();

      pps_func_arg_new->pps_func_arg = (yyvsp[(1) - (1)].pps_decl);

      pps_func_arg_new->isva = false;

      /* It's a hack!!! It's needed to alow a correct processing of typedefs names inside a parameter list. */
      ldv_istype_spec = false;

      ldv_list_push_back (&func_arg_list, pps_func_arg_new);

      (yyval.list) = func_arg_list;
    ;}
    break;

  case 83:
#line 1339 "ldv-aspect-parser.y"
    {
      ldv_pps_func_arg_ptr pps_func_arg_new = NULL;

      pps_func_arg_new = ldv_create_pps_func_arg ();

      pps_func_arg_new->pps_func_arg = (yyvsp[(3) - (3)].pps_decl);

      ldv_list_push_back (&(yyvsp[(1) - (3)].list), pps_func_arg_new);

      pps_func_arg_new->isva = false;

      /* It's a hack!!! It's needed to alow a correct processing of typedefs names inside a parameter list. */
      ldv_istype_spec = false;

      (yyval.list) = (yyvsp[(1) - (3)].list);
    ;}
    break;

  case 84:
#line 1358 "ldv-aspect-parser.y"
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify that any parameters may correspond to this 'declaration'. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_ANY_PARAMS;

      ldv_print_info (LDV_INFO_BISON, "bison parsed any parameters wildcard");

      (yyval.pps_decl) = pps_decl;
    ;}
    break;

  case 85:
#line 1371 "ldv-aspect-parser.y"
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify a kind of a declaration. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_PARAM;

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = (yyvsp[(1) - (2)].pps_declspecs);

      /* Specify a declarator. */
      pps_decl->pps_declarator = (yyvsp[(2) - (2)].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter declaration");

      (yyval.pps_decl) = pps_decl;
    ;}
    break;

  case 86:
#line 1390 "ldv-aspect-parser.y"
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify a kind of a declaration. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_PARAM;

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = (yyvsp[(1) - (2)].pps_declspecs);

      /* Specify an abstact declarator. */
      pps_decl->pps_declarator = (yyvsp[(2) - (2)].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter declaration");

      (yyval.pps_decl) = pps_decl;
    ;}
    break;

  case 87:
#line 1411 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      (yyval.list) = (yyvsp[(1) - (1)].list);
    ;}
    break;

  case 88:
#line 1417 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      (yyval.list) = ldv_list_splice ((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].list));
    ;}
    break;

  case 89:
#line 1425 "ldv-aspect-parser.y"
    {
      ldv_list_ptr declarator_list = NULL;

      declarator_list = ldv_list_push_front (NULL, ldv_create_declarator ());

      ldv_print_info (LDV_INFO_BISON, "bison parsed empty abstract declarator");

      (yyval.list) = declarator_list;
    ;}
    break;

  case 90:
#line 1435 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[(1) - (1)].list);
    ;}
    break;

  case 91:
#line 1441 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct abstract declarator of associativity");

      (yyval.list) = (yyvsp[(2) - (3)].list);
    ;}
    break;

  case 92:
#line 1447 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[(1) - (4)].list) = ldv_list_push_front ((yyvsp[(1) - (4)].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ARRAY;

      declarator_new->pps_array_size = ldv_create_pps_array_size ();

      /* If an array size was specified then save it. */
      if ((yyvsp[(3) - (4)].integer))
        {
          declarator_new->pps_array_size->pps_array_size = ldv_get_int ((yyvsp[(3) - (4)].integer));
          declarator_new->pps_array_size->issize_specified = true;
        }
      else
        declarator_new->pps_array_size->issize_specified = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed array direct abstract declarator");

      (yyval.list) = (yyvsp[(1) - (4)].list);
    ;}
    break;

  case 93:
#line 1473 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[(1) - (4)].list) = ldv_list_push_front ((yyvsp[(1) - (4)].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = (yyvsp[(3) - (4)].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct abstract declarator");

      (yyval.list) = (yyvsp[(1) - (4)].list);
    ;}
    break;

  case 94:
#line 1491 "ldv-aspect-parser.y"
    {
      (yyval.list) = NULL;
    ;}
    break;

  case 95:
#line 1495 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[(1) - (1)].list);
    ;}
    break;

  case 96:
#line 1501 "ldv-aspect-parser.y"
    {
      (yyval.list) = NULL;
    ;}
    break;

  case 97:
#line 1505 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[(1) - (1)].list);
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 3154 "ldv-aspect-parser.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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


#line 1509 "ldv-aspect-parser.y"


void
ldv_aspect_parser (void)
{
  ldv_print_info (LDV_INFO, "begin parse no comment aspect file");

  /* Enable debugging. */
  if (ldv_isinfo_bison)
    yydebug = 1;

  /* Call a main parsing procedure. */
  yyparse();

  ldv_print_info (LDV_INFO, "finish parse no comment aspect file");
}

void
ldv_check_pp_semantics (ldv_pp_ptr p_pointcut)
{
  ldv_ppk pp_kind;
  ldv_ppsk pps_kind;

  pp_kind = p_pointcut->pp_kind;
  pps_kind = p_pointcut->pp_signature->pps_kind;

  /* There are following relations between a primitive pointcut signature kind
     and a primitive pointcut kind:
       LDV_PPS_DEFINE - LDV_PP_DEFINE
       LDV_PPS_FILE - LDV_PP_FILE
       LDV_PPS_FILE - LDV_PP_INFILE
       LDV_PPS_DECL - LDV_PP_CALL
       LDV_PPS_DECL - LDV_PP_EXECUTION
       LDV_PPS_DECL - LDV_PP_GET
       LDV_PPS_DECL - LDV_PP_GET_GLOBAL
       LDV_PPS_DECL - LDV_PP_GET_LOCAL
       LDV_PPS_DECL - LDV_PP_INCALL
       LDV_PPS_DECL - LDV_PP_INFUNC
       LDV_PPS_DECL - LDV_PP_INTRODUCE
       LDV_PPS_DECL - LDV_PP_SET
       LDV_PPS_DECL - LDV_PP_SET_GLOBAL
       LDV_PPS_DECL - LDV_PP_SET_LOCAL. */
  switch (pps_kind)
    {
    case LDV_PPS_DEFINE:
      if (pp_kind == LDV_PP_DEFINE)
        return ;

      break;

    case LDV_PPS_FILE:
      if (pp_kind == LDV_PP_FILE || pp_kind == LDV_PP_INFILE)
        return ;

      break;

    case LDV_PPS_DECL:
      if (pp_kind == LDV_PP_CALL || pp_kind == LDV_PP_EXECUTION
        || pp_kind == LDV_PP_GET || pp_kind == LDV_PP_GET_GLOBAL
        || pp_kind == LDV_PP_GET_LOCAL || pp_kind == LDV_PP_INCALL
        || pp_kind == LDV_PP_INFUNC || pp_kind == LDV_PP_INTRODUCE
        || pp_kind == LDV_PP_SET || pp_kind == LDV_PP_SET_GLOBAL
        || pp_kind == LDV_PP_SET_LOCAL)
        return ;

      break;

    default:
      LDV_FATAL_ERROR ("incorrect primitive pointcut signature kind \"%d\" is used", pps_kind);
    }

  LDV_FATAL_ERROR ("incorrect primitive pointcut kind \"%d\" is used with primitive pointcut signature kind \"%d\"", pp_kind, pps_kind);
}

ldv_cp_ptr
ldv_create_c_pointcut (void)
{
  ldv_cp_ptr c_pointcut = NULL;

  c_pointcut = XCNEW (ldv_composite_pointcut);
  ldv_print_info (LDV_INFO_MEM, "composite pointcut memory was released");

  c_pointcut->cp_kind = LDV_CP_NONE;

  return c_pointcut;
}

ldv_pps_ptr
ldv_create_pp_signature (void)
{
  ldv_pps_ptr pp_signature = NULL;

  pp_signature = XCNEW (ldv_primitive_pointcut_signature);
  ldv_print_info (LDV_INFO_MEM, "primitive pointcut signature memory was released");

  pp_signature->pps_kind = LDV_PPS_NONE;

  return pp_signature;
}

int
ldv_get_id_kind (char *id)
{
  int i;

  /* Check whether an identifier is a C declaration specifier keyword. */
  for (i = 0; ldv_keyword_token_map[i].keyword; i++)
    {
      if (!strcmp(id, ldv_keyword_token_map[i].keyword))
        {
          ldv_print_info (LDV_INFO_LEX, "lex parsed keyword \"%s\"", id);
          return ldv_keyword_token_map[i].token;
        }
    }

  /* A typedef name is encountered when type specifiers are parsed and a
     nonkeyword identifier was met. */
  if (!ldv_istype_spec)
    {
      ldv_istype_spec = true;

      ldv_print_info (LDV_INFO_LEX, "lex parsed typedef name \"%s\"", id);
      return LDV_TYPEDEF_NAME;
    }

  ldv_print_info (LDV_INFO_LEX, "lex parsed identifier \"%s\"", id);
  return LDV_ID;
}

/* Correct identifier or C keyword begins with any character (in a low or an
   upper case) or '_'.  Subsequent identifier symbols are characters, digits and
   '_'. */
unsigned int
ldv_parse_id (char **id)
{
  int c;
  unsigned int byte_count = 0;
  ldv_str_ptr str = NULL;

  c = ldv_getc (LDV_ASPECT_STREAM);

  if (ISIDST (c))
    {
      str = ldv_create_string ();

      /* Get the rest of an identifier. */
      do
        {
          byte_count++;
          ldv_putc_string (c, str);
          c = ldv_getc (LDV_ASPECT_STREAM);
        }
      while (ISIDNUM (c));


    }

  /* Push back a first nonidentifier character. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);

  if (str)
    {
      /* Assign read identifier to identifier passed through parameter. */
      *id = ldv_get_str (str);
      return byte_count;
    }

  /* That is identifier wasn't read. */
  return 0;
}

unsigned int
ldv_parse_unsigned_integer (unsigned int *integer)
{
  unsigned int unsigned_integer_read;
  int matches;

  errno = 0;

  matches = fscanf(LDV_ASPECT_STREAM, "%u", &unsigned_integer_read);

  if (matches == 1)
    {
      /* Assign read from stream integer to integer passed through parameter. */
      *integer = unsigned_integer_read;
      /* Count the number of bytes read and return it. */
      return strlen(ldv_itoa(*integer));
    }
  else if (errno != 0)
    {
      LDV_FATAL_ERROR ("can%'t read aspect stream: %m");
    }

  /* Don't assign any value to integer passed through parameter. 0 bytes were
     read from stream. */
  return 0;
}

void
ldv_print_info_location (yyltype loc, const char *info_kind, const char *format, ...)
{
  va_list ap;

  if (!strcmp (info_kind, LDV_INFO_BISON))
    {
      if (!ldv_isinfo_bison)
        return;
    }
  else if (!strcmp (info_kind, LDV_INFO_LEX))
    {
      if (!ldv_isinfo_lex)
        return;
    }
  else if (strcmp (info_kind, LDV_ERROR_BISON) && strcmp (info_kind, LDV_ERROR_LEX))
    {
      LDV_FATAL_ERROR ("don't use location tracking beyond lex or bison information");
    }

  va_start (ap, format);
  /* Use a previous last line since a current value points to a following
     character. */
  fprintf (LDV_INFO_STREAM, "%s: %s:%d.%d-%d.%d: ", info_kind, loc.file_name, loc.first_line, loc.first_column, loc.last_line, (loc.last_column - 1));
  vfprintf (LDV_INFO_STREAM, format, ap);
  va_end (ap);

  fputc ('\n', LDV_INFO_STREAM);
}

ldv_pps_ptr_quals_ptr
ldv_set_ptr_quals (ldv_pps_declspecs_ptr declspecs)
{
  ldv_pps_ptr_quals_ptr ptr_quals = NULL;

  ptr_quals = ldv_create_ptr_quals ();

  /* In that case when there is no declaration specifiers at all. */
  if (!declspecs)
    {
      ptr_quals->isconst = false;
      ptr_quals->isrestrict = false;
      ptr_quals->isvolatile = false;

      return ptr_quals;
    }

  /* Set type qualifiers from declaration specifiers to pointer qualifiers. */
  ptr_quals->isconst = declspecs->isconst;
  ptr_quals->isrestrict = declspecs->isrestrict;
  ptr_quals->isvolatile = declspecs->isvolatile;

  return ptr_quals;
}

void
yyerror (char const *format, ...)
{
  va_list ap;

  va_start (ap, format);

  ldv_print_info (LDV_ERROR_BISON, format, ap);

  va_end (ap);

  ldv_print_info_location (yylloc, LDV_ERROR_BISON, "aspect file processed has incorrect syntax");

  LDV_FATAL_ERROR ("terminate work after syntax error happened");
}

int
yylex (void)
{
  int c;
  int c_next;
  unsigned int line_numb;
  ldv_str_ptr file_name;
  int brace_count = 0;
  ldv_ab_ptr body = NULL;
  ldv_file_ptr file = NULL;
  char *str = NULL;
  ldv_id_ptr id = NULL;
  unsigned int i;
  ldv_int_ptr integer = NULL;
  unsigned int arg_numb;
  ldv_ab_arg_ptr ab_arg_new = NULL;
  ldv_ab_general_ptr ab_general_new = NULL;
  unsigned int byte_count;

  /* Skip nonsignificant whitespaces and move a current position. */
  while (1)
    {
      c = ldv_getc (LDV_ASPECT_STREAM);

      switch (c)
        {
        case ' ':
        case '\t':
          ldv_set_last_column (yylloc.last_column + 1);
          continue;

        case '\n':
          ldv_set_last_line (yylloc.last_line + 1);
          ldv_set_last_column (1);
          continue;

        default:
          /* Push back a first nonwhitespace character. */
          ldv_ungetc (c, LDV_ASPECT_STREAM);
        }

      break;
    }

  /* Examine whether a C or C++ comment is encountered. Skip a corresponding
     comment if so and continue parsing. */
  while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
    {
      /* A possible comment beginning. */
      if (c == '/')
        {
          /* Define a comment type (C or C++) by means of a following
             character. */
          c_next = ldv_getc (LDV_ASPECT_STREAM);

          /* Drop a C++ comment '//...\n' from '//' up to the end of a line.
             Don't track a current file position. */
          if (c_next != EOF && c_next == '/')
            {
              while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
                {
                  if (c == '\n')
                    {
                      ldv_ungetc (c, LDV_ASPECT_STREAM);
                      break;
                    }

                  ldv_print_info (LDV_INFO_IO, "dropped C++ comment character \"%c\"", ldv_end_of_line (c));
                }

              return yylex ();
            }
          /* Drop a C comment '/_*...*_/' from '/_*' up to '*_/'. Track a
             current file position, since '*_/' can be placed at another line
             and there may be other symbols after it. */
          else if (c_next != EOF && c_next == '*')
            {
              ldv_set_last_column (yylloc.last_column + 2);

              while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
                {
                  /* See whether a C comment end is reached. */
                  if (c == '*')
                    {
                      c_next = ldv_getc (LDV_ASPECT_STREAM);

                      if (c_next != EOF && c_next == '/')
                        {
                          ldv_set_last_column (yylloc.last_column + 2);
                          return yylex ();
                        }
                      else
                        {
                          ldv_set_last_column (yylloc.last_column + 1);
                          ldv_ungetc (c_next, LDV_ASPECT_STREAM);
                        }
                    }
                  /* If the end of line is encountered, enlarge a current line
                     position. */
                  else if (c == '\n')
                    {
                      ldv_set_last_line (yylloc.last_line + 1);
                      ldv_set_last_column (1);
                    }
                  else
                    ldv_set_last_column (yylloc.last_column + 1);

                  ldv_print_info (LDV_INFO_IO, "dropped C comment character \"%c\"", ldv_end_of_line (c));
                }
            }
          else
            {
              ldv_ungetc (c, LDV_ASPECT_STREAM);

              if (c_next != EOF)
                ldv_ungetc (c_next, LDV_ASPECT_STREAM);
            }
        }
      else
        {
          ldv_ungetc (c, LDV_ASPECT_STREAM);
          break;
        }
    }

  /* Examine whether a special preprocessor line is encountered. This line has
     the following format:
       # \d+ "[^"]+" \d+
     where the first number denotes the following line number in the file
     specified inside quotes. There are may be less or more numbers at the end
     of such lines. A current position isn't tracked while processing such
     lines. */
  while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
    {
      if (c == '#')
        {
          line_numb = 0;

          while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
            {
              /* Read a file name specified and change a current file name
                 respectively to report error locations correctly. */
              if (c == '"')
                {
                  file_name = ldv_create_string ();

                  while ((c_next = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
                   {
                     ldv_print_info (LDV_INFO_IO, "dropped preprocessor character \"%c\"", ldv_end_of_line (c_next));

                     if (c_next == '"')
                       {
                          break;
                       }

                     ldv_putc_string (c_next, file_name);
                   }

                  ldv_set_file_name (ldv_get_str(file_name));
                }

              /* Update the current line with respect to a special line. */
              if (!line_numb
                && (ldv_parse_unsigned_integer (&line_numb) > 0))
                {
                  ldv_set_last_line (line_numb);
                  ldv_set_last_column (1);
                }

              if (c == '\n')
                {
                  /* Don't ungetc the end of line to avoid line enlarging. */
                  break;
                }

              ldv_print_info (LDV_INFO_IO, "dropped preprocessor character \"%c\"", ldv_end_of_line (c));
            }

          return yylex ();
        }
      else
        {
          ldv_ungetc (c, LDV_ASPECT_STREAM);
          break;
        }
    }

  /* Initialize a first line and a first column. */
  ldv_set_first_line (yylloc.last_line);
  ldv_set_first_column (yylloc.last_column);

  /* Get a first character of a token. */
  c = ldv_getc (LDV_ASPECT_STREAM);

  /* Reach the end of a file. So teh aspect parser must finish work. */
  if (c == EOF)
    {
      ldv_print_info (LDV_INFO_LEX, "lex reached the end of file");
      return 0;
    }

  /* Parse an advice body. The lexer isn't interested in its contents, except
     for correct determinition of the advice body end (that is '}') and body
     patterns. */
  if (c == '{')
    {
      body = ldv_create_body ();

      do
        {
          /* Process the end of line inside a body. */
          if (c == '\n')
            {
              ldv_set_last_line (yylloc.last_line + 1);
              ldv_set_last_column (1);
              /* Reset initial position as well. */
              ldv_set_first_line (yylloc.last_line);
              ldv_set_first_column (yylloc.last_column);
            }

          /* Process special patterns inside a body. */
          if (c == '$')
            {
              /* Move first column pointer to the beginning of pattern. */
              ldv_set_first_column (yylloc.last_column);

              id = ldv_create_id ();

              while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
                {
                  /* Stop and process an obtained pattern when an identifier is
                     complited. */
                  if (!ISIDNUM (c))
                    {
                      /* Compare an initial part of a pattern with
                         'arg'. '\d+' must foolow 'arg'. */
                      if (!strncmp (ldv_get_id_name (id), LDV_BODY_PATTERN_ARG, strlen (LDV_BODY_PATTERN_ARG))
                        && ISDIGIT (*(ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG))))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern argument");

                          /* The rest part of a pattern must have format '\d+'. */
                          arg_numb = atoi (ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG));

                          ab_arg_new = ldv_create_body_arg ();

                          ab_arg_new->arg_numb = arg_numb;
                          ab_arg_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_arg, ab_arg_new);
                        }
                      /* Compare an initial part of a pattern with
                         'arg_type'. '\d+' must foolow 'arg_type'. */
                      else if (!strncmp (ldv_get_id_name (id), LDV_BODY_PATTERN_ARG_TYPE, strlen (LDV_BODY_PATTERN_ARG_TYPE))
                        && ISDIGIT (*(ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG_TYPE))))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern argument type");

                          /* The rest part of a pattern must have format '\d+'. */
                          arg_numb = atoi (ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG_TYPE));

                          ab_arg_new = ldv_create_body_arg ();

                          ab_arg_new->arg_numb = arg_numb;
                          ab_arg_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_arg_type, ab_arg_new);
                        }
                      /* Compare an initial part of a pattern with
                         'arg_size'. '\d+' must foolow 'arg_size'. */
                      else if (!strncmp (ldv_get_id_name (id), LDV_BODY_PATTERN_ARG_SIZE, strlen (LDV_BODY_PATTERN_ARG_SIZE))
                        && ISDIGIT (*(ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG_SIZE))))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern argument size");

                          /* The rest part of a pattern must have format '\d+'. */
                          arg_numb = atoi (ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG_SIZE));

                          ab_arg_new = ldv_create_body_arg ();

                          ab_arg_new->arg_numb = arg_numb;
                          ab_arg_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_arg_size, ab_arg_new);
                        }
                      /* Compare an initial part of a pattern with
                         'arg_value'. '\d+' must foolow 'arg_value'. */
                      else if (!strncmp (ldv_get_id_name (id), LDV_BODY_PATTERN_ARG_VALUE, strlen (LDV_BODY_PATTERN_ARG_VALUE))
                        && ISDIGIT (*(ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG_VALUE))))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern argument ");

                          /* The rest part of a pattern must have format '\d+'. */
                          arg_numb = atoi (ldv_get_id_name (id) + strlen (LDV_BODY_PATTERN_ARG_VALUE));

                          ab_arg_new = ldv_create_body_arg ();

                          ab_arg_new->arg_numb = arg_numb;
                          ab_arg_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_arg_value, ab_arg_new);
                        }
                      /* Compare a pattern with 'aspect_func_name'. */
                      else if (!strcmp (ldv_get_id_name (id), LDV_BODY_PATTERN_ASPECT_FUNC_NAME))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern aspect function name");

                          ab_general_new = ldv_create_body_general ();

                          ab_general_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_aspect_func_name, ab_general_new);
                        }
                      /* Compare a pattern with 'func_name'. */
                      else if (!strcmp (ldv_get_id_name (id), LDV_BODY_PATTERN_FUNC_NAME))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern function name");

                          ab_general_new = ldv_create_body_general ();

                          ab_general_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_func_name, ab_general_new);
                        }
                      /* Compare a pattern with 'proceed'. */
                      else if (!strcmp (ldv_get_id_name (id), LDV_BODY_PATTERN_PROCEED))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern proceed");

                          ab_general_new = ldv_create_body_general ();

                          ab_general_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_proceed, ab_general_new);
                        }
                      /* Compare a pattern with 'res'. */
                      else if (!strcmp (ldv_get_id_name (id), LDV_BODY_PATTERN_RES))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body pattern result");

                          ab_general_new = ldv_create_body_general ();

                          ab_general_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_res, ab_general_new);
                        }
                      /* Compare a pattern with 'ret_type'. */
                      else if (!strcmp (ldv_get_id_name (id), LDV_BODY_PATTERN_RET_TYPE))
                        {
                          ldv_print_info (LDV_INFO_LEX, "lex parsed body return type to be weaved \"%s\"", ldv_get_id_name (id));

                          ab_general_new = ldv_create_body_general ();

                          ab_general_new->arg_place = LDV_STR_OFFSET (ldv_get_body_text (body));

                          ldv_list_push_back (&body->ab_ret_type, ab_general_new);
                        }
                      else
                        {
                          ldv_print_info_location (yylloc, LDV_ERROR_LEX, "incorrect body pattern \"%s\" was used", ldv_get_id_name (id));
                          LDV_FATAL_ERROR ("use \"argN\", \"arg_typeN\", \"arg_sizeN\", \"arg_valueN\", \"func_name\", \"aspect_func_name\", \"proceed\", \"res\", \"ret_type\" body patterns");
                        }

                      break;
                    }

                  ldv_set_last_column (yylloc.last_column + 1);
                  ldv_putc_id (c, id);
                }
            }

          ldv_putc_body (c, body);

          /* Increase/decrease a brace counter to skip '{...}' construction
             inside a body. */
          if (c == '{')
            brace_count++;
          else if (c == '}')
            brace_count--;

          if (c != '\n')
            ldv_set_last_column (yylloc.last_column + 1);

          if (c == '}' && !brace_count)
            break;
        }
      while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF);

      /* Set a corresponding semantic value. */
      yylval.body = body;

      ldv_print_info (LDV_INFO_LEX, "lex parsed body \"%s\"", ldv_get_body_text (body));
      return LDV_BODY;
    }

  /* Parse a file name that is in the form "file_name" */
  if (c == '"')
    {
      ldv_set_last_column (yylloc.last_column + 1);
      file = ldv_create_file ();

      do
        {
          c = ldv_getc (LDV_ASPECT_STREAM);
          ldv_set_last_column (yylloc.last_column + 1);

          if (c == '"')
            {
              if (!strcmp (ldv_get_file_name (file), "$this"))
                file->isthis = true;
              else
                file->isthis = false;

              break;
            }

        ldv_putc_file (c, file);

        if (c == '\n')
          {
            LDV_FATAL_ERROR ("file path isn't terminated with quote - it's terminated with the end of line");
          }
        }
      while (c != EOF);

      /* Set a corresponding semantic value. */
      yylval.file = file;

      ldv_print_info (LDV_INFO_LEX, "lex parsed file \"%s\"", ldv_get_file_name (file));
      return LDV_FILE;
    }

  /* Parse some identifier or a C keyword. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);
  if ((byte_count = ldv_parse_id (&str)))
    {
      /* Move current position properly. */
      ldv_set_last_column (yylloc.last_column + byte_count);

      /* Initialize corresponding internal structure. */
      id = ldv_create_id ();
      ldv_puts_id (str, id);

      /* Set a corresponding semantic value. */
      yylval.id = id;

      return ldv_get_id_kind (str);
    }

  /* Parse some integer number. It consists of digits. */
  if ((byte_count = ldv_parse_unsigned_integer (&i)))
    {
      /* Move current position properly. */
      ldv_set_last_column (yylloc.last_column + byte_count);

      /* Initialize corresponding internal structure. */
      integer = ldv_create_int ();
      integer->numb = i;

      /* Set a corresponding semantic value. */
      yylval.integer = integer;

      ldv_print_info (LDV_INFO_LEX, "lex parsed integer number \"%d\"", ldv_get_int (integer));

      return LDV_INT_NUMB;
    }

  /* After all parse multicharacter tokens. */
  c = ldv_getc (LDV_ASPECT_STREAM);
  ldv_set_last_column (yylloc.last_column + 1);
  if (c == '|' || c == '&' || c == '.')
    {
      c_next = ldv_getc (LDV_ASPECT_STREAM);

      /* Or boolean operator '||' */
      if (c == '|' && c_next == '|')
        {
          ldv_set_last_column (yylloc.last_column + 1);
          ldv_print_info (LDV_INFO_LEX, "lex parsed or boolean operator");

          return LDV_OR;
        }

      /* And boolean operator '&&' */
      if (c == '&' && c_next == '&')
        {
          ldv_set_last_column (yylloc.last_column + 1);
          ldv_print_info (LDV_INFO_LEX, "lex parsed and boolean operator");

          return LDV_AND;
        }

      /* Either C ellipsis '...' or ldv any parameters '..'. */
      if (c == '.' && c_next == '.')
        {
          ldv_set_last_column (yylloc.last_column + 1);

          /* See on a next character. */
          c_next = ldv_getc (LDV_ASPECT_STREAM);

          if (c_next == '.')
            {
              ldv_set_last_column (yylloc.last_column + 1);
              ldv_print_info (LDV_INFO_LEX, "lex parsed ellipsis");

              return LDV_ELLIPSIS;
            }
          else
            {
              ldv_ungetc (c_next, LDV_ASPECT_STREAM);
              ldv_print_info (LDV_INFO_LEX, "lex parsed any parameters wildcard");

              return LDV_ANY_PARAMS;
            }
        }

      ldv_ungetc (c, LDV_ASPECT_STREAM);
    }

  /* Just return a symbol. */
  return c;
}

