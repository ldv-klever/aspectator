/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         ldv_yyparse
#define yylex           ldv_yylex
#define yyerror         ldv_yyerror
#define yydebug         ldv_yydebug
#define yynerrs         ldv_yynerrs

#define yylval          ldv_yylval
#define yychar          ldv_yychar
#define yylloc          ldv_yylloc

/* Copy the first part of user declarations.  */
#line 25 "ldv-aspect-parser.y" /* yacc.c:339  */


/* For ISALPHA, ISDIGIT, etc. functions. */
#include <safe-ctype.h>

/* General gcc core types and functions. */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hashtab.h"

/* For error functions. */
#include "diagnostic-core.h"
#include "toplev.h"

#include "ldv-aspect-parser.h"
#include "ldv-aspect-types.h"
#include "ldv-core.h"
#include "ldv-cpp-converter.h"
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
  char *file_name;
} YYLTYPE;
#define yyltype YYLTYPE
#define YYLTYPE_IS_DECLARED 1

/* Flag says whether it's parsed macro signature or not. */
static bool ldv_ismacro = false;
/* Flag says whether it's parsed declaration or not. */
static bool ldv_isdecl = false;
/* Flag is true if some type specifier was parsed and false otherwise.
 * It becomes false when declaration specifiers are parsed. */
static bool ldv_istype_spec = false;
/* Flag is true if universal type specifier was parsed and false otherwise.
 * It becomes false when declaration specifiers are parsed. */
static bool ldv_isuniversal_type_spec = false;
static htab_t ldv_called_func_names;


/* TODO: remove parameter names from prototypes. */
static void ldv_check_pp_semantics (ldv_pp_ptr);
static ldv_cp_ptr ldv_create_c_pointcut (void);
static ldv_pps_ptr ldv_create_pp_signature (void);
static int ldv_get_id_kind (char *id);
static void ldv_hash_add_name (char *name);
static void ldv_hash_recursive_composite_pointcut (ldv_cp_ptr c_pointcut);
static void ldv_hash_string_del (void *s_p);
static int ldv_hash_string_eq (const void *s1_p, const void *s2_p);
static hashval_t ldv_hash_string_hash (const void *s_p);
static unsigned int ldv_parse_advice_body (ldv_ab_ptr *body);
static ldv_aspect_pattern_ptr ldv_parse_aspect_pattern (void);
static ldv_aspect_pattern_param_ptr ldv_parse_aspect_pattern_param (void);
static unsigned int ldv_parse_aspect_pattern_param_str (char **str);
static ldv_list_ptr ldv_parse_aspect_pattern_params (void);
static int ldv_parse_aspect_pattern_known_value (char **str);
static ldv_text_ptr ldv_parse_comments (void);
static unsigned int ldv_parse_file_name (char **file_name);
static unsigned int ldv_parse_id (char **id, bool *isany_chars, bool isaspect_pattern);
static int ldv_parse_preprocessor_directives (void);
static unsigned int ldv_parse_unsigned_integer (unsigned int *integer);
static void ldv_parse_whitespaces (void);
static void ldv_print_info_location (yyltype, const char *, const char *, ...) ATTRIBUTE_PRINTF_3;
static ldv_pps_ptr_quals_ptr ldv_set_ptr_quals (ldv_pps_declspecs_ptr);
static void yyerror (char const *, ...);
static int yylex (void);


#line 170 "ldv-aspect-parser.c" /* yacc.c:339  */

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


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int ldv_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LDV_FILE = 258,
    LDV_BODY = 259,
    LDV_ID = 260,
    LDV_INT_NUMB = 261,
    LDV_MACRO_POINTCUT = 262,
    LDV_FILE_POINTCUT = 263,
    LDV_TYPEDEF = 264,
    LDV_EXTERN = 265,
    LDV_STATIC = 266,
    LDV_AUTO = 267,
    LDV_REGISTER = 268,
    LDV_VOID = 269,
    LDV_CHAR = 270,
    LDV_INT = 271,
    LDV_FLOAT = 272,
    LDV_DOUBLE = 273,
    LDV_BOOL = 274,
    LDV_COMPLEX = 275,
    LDV_SHORT = 276,
    LDV_LONG = 277,
    LDV_SIGNED = 278,
    LDV_UNSIGNED = 279,
    LDV_STRUCT = 280,
    LDV_UNION = 281,
    LDV_ENUM = 282,
    LDV_TYPEDEF_NAME = 283,
    LDV_UNIVERSAL_TYPE_SPECIFIER = 284,
    LDV_CONST = 285,
    LDV_RESTRICT = 286,
    LDV_VOLATILE = 287,
    LDV_INLINE = 288,
    LDV_AND = 289,
    LDV_ANY_PARAMS = 290,
    LDV_ELLIPSIS = 291,
    LDV_OR = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 134 "ldv-aspect-parser.y" /* yacc.c:355  */

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

#line 264 "ldv-aspect-parser.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE ldv_yylval;
extern YYLTYPE ldv_yylloc;
int ldv_yyparse (void);



/* Copy the second part of user declarations.  */

#line 293 "ldv-aspect-parser.c" /* yacc.c:358  */

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   116

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  136

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,     2,     2,     2,     2,     2,
      40,    41,    45,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    44,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   255,   255,   257,   262,   268,   312,   334,   385,   428,
     467,   487,   512,   541,   549,   587,   625,   700,   720,   742,
     759,   762,   768,   776,   782,   793,   803,   813,   823,   829,
     848,   908,   908,   908,   914,   932,   949,   966,   983,   991,
    1002,  1005,  1011,  1024,  1036,  1048,  1060,  1074,  1086,  1098,
    1110,  1122,  1134,  1146,  1158,  1175,  1192,  1209,  1226,  1239,
    1252,  1265,  1278,  1292,  1304,  1316,  1330,  1344,  1352,  1369,
    1375,  1403,  1422,  1425,  1431,  1448,  1467,  1470,  1475,  1487,
    1490,  1496,  1500,  1519,  1544,  1560,  1589,  1608,  1662,  1668,
    1677,  1686,  1692,  1698,  1724,  1743,  1746,  1753,  1756
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LDV_FILE", "LDV_BODY", "LDV_ID",
  "LDV_INT_NUMB", "LDV_MACRO_POINTCUT", "LDV_FILE_POINTCUT", "LDV_TYPEDEF",
  "LDV_EXTERN", "LDV_STATIC", "LDV_AUTO", "LDV_REGISTER", "LDV_VOID",
  "LDV_CHAR", "LDV_INT", "LDV_FLOAT", "LDV_DOUBLE", "LDV_BOOL",
  "LDV_COMPLEX", "LDV_SHORT", "LDV_LONG", "LDV_SIGNED", "LDV_UNSIGNED",
  "LDV_STRUCT", "LDV_UNION", "LDV_ENUM", "LDV_TYPEDEF_NAME",
  "LDV_UNIVERSAL_TYPE_SPECIFIER", "LDV_CONST", "LDV_RESTRICT",
  "LDV_VOLATILE", "LDV_INLINE", "\"&&\"", "\"..\"", "\"...\"", "\"||\"",
  "'!'", "':'", "'('", "')'", "','", "'['", "']'", "'*'", "$accept",
  "input", "named_pointcut", "advice_definition", "advice_declaration",
  "composite_pointcut", "primitive_pointcut",
  "primitive_pointcut_signature_macro",
  "primitive_pointcut_signature_file", "macro_param_opt", "macro_param",
  "primitive_pointcut_signature_declaration", "c_declaration",
  "c_declaration_specifiers", "$@1", "$@2", "c_declaration_specifiers_aux",
  "c_declaration_specifiers_opt", "c_storage_class_specifier",
  "c_type_specifier", "c_type_qualifier", "c_function_specifier",
  "c_declarator", "c_direct_declarator", "c_pointer_opt", "c_pointer",
  "int_opt", "c_type_qualifier_list_opt", "c_type_qualifier_list",
  "c_parameter_type_list", "c_parameter_list", "c_parameter_declaration",
  "c_abstract_declarator", "c_abstract_declarator_opt",
  "c_direct_abstract_declarator", "c_direct_abstract_declarator_opt",
  "c_parameter_type_list_opt", YY_NULLPTR
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
     285,   286,   287,   288,   289,   290,   291,   292,    33,    58,
      40,    41,    44,    91,    93,    42
};
# endif

#define YYPACT_NINF -98

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-98)))

#define YYTABLE_NINF -98

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -98,    13,   -98,     5,   -98,   -98,     7,   -17,    -2,   -98,
      -2,   -23,    -7,     1,    -2,    -2,     0,   -98,     0,   -98,
      71,    85,   -98,   -22,    -2,    -2,    49,   -98,   -16,    38,
      51,    52,   -98,    53,   -98,   -98,    55,   -98,    48,   -98,
      -1,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,    87,    90,
      91,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
      38,    38,    38,    38,     4,   -98,   -98,   -98,    54,    48,
     -98,    54,    -8,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,    61,   -98,    57,    58,   -98,   -98,    60,   -98,
      79,   -98,   -98,     9,   -98,   -18,    62,    63,   -98,   -98,
     -98,    64,    66,   -98,   -98,     2,    45,   -98,   -98,   -98,
     -98,   -98,   -98,    54,    32,    67,   -98,    65,    68,    79,
     -98,   -98,    70,    69,   -98,   -98
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     4,     3,     0,     0,     0,     6,
       0,     8,     0,     0,     0,     0,     7,     9,     5,    31,
       0,     0,    10,     0,     0,     0,     0,    28,    72,     0,
      17,     0,    19,     0,    13,    12,    11,    16,    79,    30,
       0,    73,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,     0,     0,
       0,    61,    62,    63,    64,    65,    66,    38,    39,    32,
      40,    40,    40,    40,    20,    14,    15,    81,    74,    80,
      68,    72,    67,    58,    59,    60,    33,    41,    34,    35,
      36,    37,    22,    24,     0,    21,    75,    82,     0,    31,
      76,    25,    18,     0,    69,    72,     0,    83,    84,    78,
      77,     0,    23,    26,    86,    95,    73,    91,    87,    71,
      31,    70,    27,    72,    89,     0,    85,     0,    31,    76,
      92,    98,     0,     0,    94,    93
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -98,   -98,   -98,   -98,   -98,     6,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,    88,   -98,   -98,    75,    10,   -98,   -98,
     -36,   -98,   -28,   -98,   -97,   -77,   -15,   -98,   -98,   -13,
     -98,    -4,   -11,   -98,   -98,   -98,   -98
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     4,     5,     6,    16,    17,    31,    33,    94,
      95,    26,    27,   105,    29,    86,    87,    88,    70,    71,
      72,    73,    98,    82,    40,    41,   111,    78,    79,   106,
     107,   108,   117,   118,   124,   125,   132
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,    96,    77,    11,    80,    12,    13,    80,   115,    92,
       7,     9,    24,     2,   112,    25,    18,    19,     3,    34,
      22,    23,    10,   -90,   -90,   -29,   115,    38,   116,    38,
      35,    36,    99,    20,    24,   100,    14,    25,    15,    81,
      93,    21,   123,    97,     8,   113,   116,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,   128,    67,    68,   -96,    30,   114,    63,    64,
      65,    89,    90,    91,   109,   110,   -88,   -88,    32,    24,
      37,    74,    83,    75,    76,    84,    85,   101,   102,    38,
     103,   104,   122,   119,    69,   120,   130,    28,   121,   -97,
     129,   134,   127,   135,   133,   131,   126
};

static const yytype_uint8 yycheck[] =
{
      28,    78,    38,     5,     5,     7,     8,     5,   105,     5,
       5,     4,    34,     0,     5,    37,    10,    40,     5,    41,
      14,    15,    39,    41,    42,    41,   123,    45,   105,    45,
      24,    25,    40,    40,    34,    43,    38,    37,    40,    40,
      36,    40,    40,    79,    39,    36,   123,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    40,    35,    36,    43,     5,   105,    30,    31,
      32,    71,    72,    73,     5,     6,    41,    42,     3,    34,
      41,    40,     5,    41,    41,     5,     5,    36,    41,    45,
      42,    41,    36,    41,    29,    42,    41,    19,    44,    41,
      43,    41,   123,    44,   129,   128,   120
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    47,     0,     5,    48,    49,    50,     5,    39,     4,
      39,     5,     7,     8,    38,    40,    51,    52,    51,    40,
      40,    40,    51,    51,    34,    37,    57,    58,    59,    60,
       5,    53,     3,    54,    41,    51,    51,    41,    45,    68,
      70,    71,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    35,    36,    62,
      64,    65,    66,    67,    40,    41,    41,    66,    73,    74,
       5,    40,    69,     5,     5,     5,    61,    62,    63,    63,
      63,    63,     5,    36,    55,    56,    71,    66,    68,    40,
      43,    36,    41,    42,    41,    59,    75,    76,    77,     5,
       6,    72,     5,    36,    68,    70,    71,    78,    79,    41,
      42,    44,    36,    40,    80,    81,    77,    78,    40,    43,
      41,    75,    82,    72,    41,    44
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    47,    48,    49,    50,    51,    51,
      51,    51,    51,    51,    52,    52,    52,    53,    53,    54,
      55,    55,    56,    56,    56,    56,    56,    56,    57,    58,
      58,    60,    61,    59,    62,    62,    62,    62,    62,    62,
      63,    63,    64,    64,    64,    64,    64,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    66,    66,    66,    67,    68,    69,    69,
      69,    69,    70,    70,    71,    71,    72,    72,    72,    73,
      73,    74,    74,    75,    76,    76,    77,    77,    78,    78,
      79,    79,    80,    80,    80,    81,    81,    82,    82
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     4,     2,     3,     1,     1,
       2,     3,     3,     3,     4,     4,     4,     1,     4,     1,
       0,     1,     1,     3,     1,     2,     3,     4,     1,     1,
       2,     0,     0,     3,     2,     2,     2,     2,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     1,     3,
       4,     4,     0,     1,     2,     3,     0,     1,     1,     0,
       1,     1,     2,     1,     1,     3,     2,     2,     1,     2,
       0,     1,     3,     4,     4,     0,     1,     0,     1
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


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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
       'yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 241 "ldv-aspect-parser.y" /* yacc.c:1429  */
{
  /* Initialize the beginning location and aspect file. */
  yylloc.first_line = yylloc.last_line = 1;
  ldv_set_first_line (yylloc.first_line);
  ldv_set_last_line (yylloc.last_line);
  yylloc.first_column = yylloc.last_column = 1;
  ldv_set_first_column (yylloc.first_column);
  ldv_set_last_column (yylloc.last_column);
  yylloc.file_name = ldv_copy_str (ldv_aspect_fname);
  ldv_set_file_name (yylloc.file_name);
}

#line 1403 "ldv-aspect-parser.c" /* yacc.c:1429  */
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
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
     '$$ = $1'.

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
#line 258 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      /* Add advice definition from corresponding rule to the advice definitions list. */
      ldv_list_push_back (&ldv_adef_list, (yyvsp[0].a_definition));
    }
#line 1595 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 263 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_list_push_back (&ldv_n_pointcut_list, (yyvsp[0].n_pointcut));
    }
#line 1603 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 269 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_np_ptr n_pointcut_new = NULL;
      char *p_keyword = NULL;
      char *p_name = NULL;

      /* Set pointcut keyword from lexer identifier. */
      p_keyword = ldv_get_id_name ((yyvsp[-3].id));

      if (strcmp ("pointcut", p_keyword))
        {
          ldv_print_info_location ((yylsp[-3]), LDV_ERROR_BISON, "incorrect keyword \"%s\" was used", p_keyword);
          LDV_FATAL_ERROR ("use one of the following keywords: \"pointcut\", \"before\", \"after\", \"around\", \"new\"");
        }

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-3].id));

      n_pointcut_new = XCNEW (ldv_named_pointcut);
      ldv_print_info (LDV_INFO_MEM, "named pointcut memory was released");

      /* Set a pointcut name from a lexer identifier. This name can't contain
         any '$' wildcards. */
      p_name = ldv_copy_str (ldv_get_id_name ((yyvsp[-2].id)));

      if ((yyvsp[-2].id)->isany_chars)
        {
          ldv_print_info_location ((yylsp[-3]), LDV_ERROR_BISON, "'$' wildcard was used in pointcut name \"%s\"", p_name);
          LDV_FATAL_ERROR ("pointcut name should be a correct identifier");
        }

      ldv_free_id ((yyvsp[-2].id));

      n_pointcut_new->p_name = p_name;

      /* Set a composite pointcut of a named pointcut from a corresponding rule. */
      n_pointcut_new->c_pointcut = (yyvsp[0].c_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed named pointcut");

      (yyval.n_pointcut) = n_pointcut_new;
    }
#line 1649 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 313 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_adef_ptr a_definition_new = NULL;

      a_definition_new = XCNEW (ldv_advice_definition);
      ldv_print_info (LDV_INFO_MEM, "advice definition memory was released");

      /* Set an advice definition declaration from a corresponding rule. */
      a_definition_new->a_declaration = (yyvsp[-1].a_declaration);

      /* Set and advice definition body from a lexer body. */
      a_definition_new->a_body = (yyvsp[0].body);

      /* Set that an advice wasn't used at the beginning. */
      a_definition_new->use_counter = 0;

      ldv_print_info (LDV_INFO_BISON, "bison parsed advice definition");

      (yyval.a_definition) = a_definition_new;
    }
#line 1673 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 335 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      char *a_kind = NULL;
      ldv_adecl_ptr a_declaration = NULL;

      /* Set an advice declaration kind from a lexer identifier. */
      a_kind = ldv_get_id_name ((yyvsp[-2].id));

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
      else if (!strcmp ("info", a_kind))
        a_declaration->a_kind = LDV_A_INFO;
      else
        {
          ldv_print_info_location ((yylsp[-2]), LDV_ERROR_BISON, "incorrect advice declaration kind \"%s\" was used", a_kind);
          LDV_FATAL_ERROR ("use \"after\", \"around\", \"before\", \"new\", \"info\" advice declaration kind");
        }

      /* Set a composite pointcut from a corresponding rule. */
      a_declaration->c_pointcut = (yyvsp[0].c_pointcut);

      if (a_declaration->c_pointcut->cp_type == LDV_CP_TYPE_CALL)
        {
          /* Create hash table with called function names. */
          ldv_called_func_names = htab_create (1, ldv_hash_string_hash, ldv_hash_string_eq, ldv_hash_string_del);
          /* Store information about called function names in hash table. */
          ldv_hash_recursive_composite_pointcut (a_declaration->c_pointcut);

          a_declaration->called_func_names = ldv_called_func_names;
        }

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" advice declaration", a_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-2].id));

      (yyval.a_declaration) = a_declaration;
    }
#line 1725 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 386 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_np_ptr n_pointcut = NULL;
      ldv_list_ptr n_pointcut_list = NULL;
      char *p_name = NULL;
      ldv_cp_ptr c_pointcut = NULL;

      /* Set a pointcut name from a lexer identifier. This name can't contain
         any '$' wildcards. */
      p_name = ldv_get_id_name ((yyvsp[0].id));

      if ((yyvsp[0].id)->isany_chars)
        {
          ldv_print_info_location ((yylsp[0]), LDV_ERROR_BISON, "'$' wildcard was used in pointcut name \"%s\"", p_name);
          LDV_FATAL_ERROR ("pointcut name should be a correct identifier");
        }

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

      ldv_free_id ((yyvsp[0].id));
    }
#line 1772 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 429 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_PRIMITIVE;

      /* Set a primitive pointcut from a corresponding rule. */
      c_pointcut->p_pointcut = (yyvsp[0].p_pointcut);

      /* Inherit primitive pointcut type. */
      switch (c_pointcut->p_pointcut->pp_kind)
        {
        case LDV_PP_CALL:
          c_pointcut->cp_type = LDV_CP_TYPE_CALL;
          break;

        case LDV_PP_DEFINE:
          c_pointcut->cp_type = LDV_CP_TYPE_DEFINE;
          break;

        case LDV_PP_EXECUTION:
          c_pointcut->cp_type = LDV_CP_TYPE_EXECUTION;
          break;

        case LDV_PP_INFILE:
          c_pointcut->cp_type = LDV_CP_TYPE_INFILE;
          break;

        default:
          c_pointcut->cp_type = LDV_CP_TYPE_ANY;
        }

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"primitive\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    }
#line 1815 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 468 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_NOT;

      /* Set a composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = (yyvsp[0].c_pointcut);

      /* Propagating pointcut type for negations of pointcuts is disambiguous
         like negations themselves. */
      c_pointcut->cp_type = c_pointcut->c_pointcut_first->cp_type;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"not\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    }
#line 1839 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 488 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_OR;

      /* Set a first composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = (yyvsp[-2].c_pointcut);
      /* Set a second composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_second = (yyvsp[0].c_pointcut);

      /* In general case just when both composite pointcuts have the same type
         their "or" has the same type. */
      if (c_pointcut->c_pointcut_first->cp_type == c_pointcut->c_pointcut_second->cp_type)
        c_pointcut->cp_type = c_pointcut->c_pointcut_first->cp_type;
      else
        c_pointcut->cp_type = LDV_CP_TYPE_ANY;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"or\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    }
#line 1868 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 513 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_AND;

      /* Set a first composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = (yyvsp[-2].c_pointcut);
      /* Set a second composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_second = (yyvsp[0].c_pointcut);

      /* "And" of two composite pointcuts has type of non "infile" composite
         poincut when their types are the same or one of types is "infile". */
      if (c_pointcut->c_pointcut_first->cp_type == c_pointcut->c_pointcut_second->cp_type)
        c_pointcut->cp_type = c_pointcut->c_pointcut_first->cp_type;
      else if (c_pointcut->c_pointcut_first->cp_type == LDV_CP_TYPE_INFILE)
        c_pointcut->cp_type = c_pointcut->c_pointcut_second->cp_type;
      else if (c_pointcut->c_pointcut_second->cp_type == LDV_CP_TYPE_INFILE)
        c_pointcut->cp_type = c_pointcut->c_pointcut_first->cp_type;
      else
        c_pointcut->cp_type = LDV_CP_TYPE_ANY;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"and\" composite pointcut");

      (yyval.c_pointcut) = c_pointcut;
    }
#line 1901 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 542 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed composite pointcut of associativity");

      (yyval.c_pointcut) = (yyvsp[-1].c_pointcut);
    }
#line 1911 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 550 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;
      ldv_pps_ptr pp_signature = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ((yyvsp[-3].id));

      p_pointcut = XCNEW (ldv_primitive_pointcut);
      ldv_print_info (LDV_INFO_MEM, "primitive pointcut memory was released");

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("define", pp_kind))
        p_pointcut->pp_kind = LDV_PP_DEFINE;
      else if (!strcmp ("expand", pp_kind))
        p_pointcut->pp_kind = LDV_PP_EXPAND;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a macro signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_DEFINE;

      /* Specify a macro signature. */
      pp_signature->pps_macro = (yyvsp[-1].pps_macro);

      /* Set a primitive pointcut signature. */
      p_pointcut->pp_signature = pp_signature;

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-3].id));

      (yyval.p_pointcut) = p_pointcut;
    }
#line 1953 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 588 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;
      ldv_pps_ptr pp_signature = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ((yyvsp[-3].id));

      p_pointcut = XCNEW (ldv_primitive_pointcut);
      ldv_print_info (LDV_INFO_MEM, "primitive pointcut memory was released");

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("file", pp_kind))
        p_pointcut->pp_kind = LDV_PP_FILE;
      else if (!strcmp ("infile", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INFILE;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a file signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_FILE;

      /* Specify a file signature. */
      pp_signature->pps_file = (yyvsp[-1].pps_file);

      /* Set a primitive pointcut signature. */
      p_pointcut->pp_signature = pp_signature;

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-3].id));

      (yyval.p_pointcut) = p_pointcut;
    }
#line 1995 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 626 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;
      ldv_pps_ptr pp_signature = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ((yyvsp[-3].id));

      p_pointcut = XCNEW (ldv_primitive_pointcut);
      ldv_print_info (LDV_INFO_MEM, "primitive pointcut memory was released");

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("call", pp_kind))
        p_pointcut->pp_kind = LDV_PP_CALL;
      else if (!strcmp ("callp", pp_kind))
        p_pointcut->pp_kind = LDV_PP_CALLP;
      else if (!strcmp ("use_func", pp_kind))
        p_pointcut->pp_kind = LDV_PP_USE_FUNC;
      else if (!strcmp ("execution", pp_kind))
        p_pointcut->pp_kind = LDV_PP_EXECUTION;
      else if (!strcmp ("declare_func", pp_kind))
        p_pointcut->pp_kind = LDV_PP_DECLARE_FUNC;
      else if (!strcmp ("get", pp_kind))
        p_pointcut->pp_kind = LDV_PP_GET;
      else if (!strcmp ("get_global", pp_kind))
        p_pointcut->pp_kind = LDV_PP_GET_GLOBAL;
      else if (!strcmp ("get_local", pp_kind))
        p_pointcut->pp_kind = LDV_PP_GET_LOCAL;
      else if (!strcmp ("incall", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INCALL;
      else if (!strcmp ("infunc", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INFUNC;
      else if (!strcmp ("init", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INIT;
      else if (!strcmp ("init_global", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INIT_GLOBAL;
      else if (!strcmp ("init_local", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INIT_LOCAL;
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
          ldv_print_info_location ((yylsp[-3]), LDV_ERROR_BISON, "incorrect primitive pointcut kind \"%s\" was used", pp_kind);
          LDV_FATAL_ERROR ("use \"call\", \"declare_func\", \"define\", \"execution\", \"expand\", \"file\", \"get\", \"get_global\", \"get_local\", \"incall\", \"infile\", \"infunc\", \"init\", \"init_global\", \"init_local\", \"introduce\", \"set\", \"set_global\", \"set_local\" primitive pointcut kind");
        }

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a declaration is parsed. */
      pp_signature->pps_kind = LDV_PPS_DECL;

      /* Specify a file signature. */
      pp_signature->pps_declaration = (yyvsp[-1].pps_decl);

      /* Set a primitive pointcut signature. */
      p_pointcut->pp_signature = pp_signature;

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-3].id));

      (yyval.p_pointcut) = p_pointcut;
    }
#line 2072 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 701 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);
      ldv_print_info (LDV_INFO_MEM, "macro primitive pointcut signature memory was released");

      /* Specify that macro definition signature is parsed. */
      macro->pps_macro_kind = LDV_PPS_MACRO_DEF;

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = (yyvsp[0].id);

      /* Specify that there is no macro parameters at all. */
      macro->macro_param_list = NULL;

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro definition signature");

      (yyval.pps_macro) = macro;
    }
#line 2096 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 721 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);
      ldv_print_info (LDV_INFO_MEM, "macro primitive pointcut signature memory was released");

      /* Specify that macro function signature is parsed. */
      macro->pps_macro_kind = LDV_PPS_MACRO_FUNC;

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = (yyvsp[-3].id);

      /* Set a macro function parameters from a corresponding rule. */
      macro->macro_param_list = (yyvsp[-1].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro function signature");

      (yyval.pps_macro) = macro;
    }
#line 2120 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 743 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_file_ptr file = NULL;

      file = XCNEW (ldv_primitive_pointcut_signature_file);
      ldv_print_info (LDV_INFO_MEM, "file primitive pointcut signature memory was released");

      /* Set a function name from a lexer identifier. */
      file->file_name = (yyvsp[0].file);

      ldv_print_info (LDV_INFO_BISON, "bison parsed file signature");

      (yyval.pps_file) = file;
    }
#line 2138 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 759 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = NULL;
    }
#line 2146 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 763 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 2154 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 769 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_list_ptr macro_param_list = NULL;

      ldv_list_push_back (&macro_param_list, (yyvsp[0].id));

      (yyval.list) = macro_param_list;
    }
#line 2166 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 777 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_list_push_back (&(yyvsp[-2].list), (yyvsp[0].id));

      (yyval.list) = (yyvsp[-2].list);
    }
#line 2176 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 783 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_list_ptr macro_param_list = NULL;
      ldv_id_ptr id = ldv_create_id ();

      ldv_puts_id ("...", id);

      ldv_list_push_back (&macro_param_list, id);

      (yyval.list) = macro_param_list;
    }
#line 2191 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 794 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_list_ptr macro_param_list = NULL;

      ldv_puts_id ("...", (yyvsp[-1].id));

      ldv_list_push_back (&macro_param_list, (yyvsp[-1].id));

      (yyval.list) = macro_param_list;
    }
#line 2205 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 804 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_id_ptr id = ldv_create_id ();

      ldv_puts_id ("...", id);

      ldv_list_push_back (&(yyvsp[-2].list), id);

      (yyval.list) = (yyvsp[-2].list);
    }
#line 2219 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 814 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_puts_id ("...", (yyvsp[-1].id));

      ldv_list_push_back (&(yyvsp[-3].list), (yyvsp[-1].id));

      (yyval.list) = (yyvsp[-3].list);
    }
#line 2231 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 824 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.pps_decl) = (yyvsp[0].pps_decl);
    }
#line 2239 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 830 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = (yyvsp[0].pps_declspecs);

      /* Specify an empty declarator, i.e. no name is declared. */
      pps_decl->pps_declarator = NULL;

      /* Specify a kind of a declaration, a type. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_TYPE;

      ldv_print_info (LDV_INFO_BISON, "bison parsed type declaration signature pointcut");

      (yyval.pps_decl) = pps_decl;
    }
#line 2262 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 849 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_decl_ptr decl = NULL;
      ldv_pps_declarator_ptr declarator = NULL;
      ldv_list_ptr declarator_list = NULL;
      bool isdecl_kind_specified = false;

      decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers and a declarator chain. */
      decl->pps_declspecs = (yyvsp[-1].pps_declspecs);
      decl->pps_declarator = (yyvsp[0].list);

      /* Specify a kind of a declaration, a function or a variable. */
      for (declarator_list = (yyvsp[0].list)
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
    }
#line 2325 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 908 "ldv-aspect-parser.y" /* yacc.c:1646  */
    { ldv_isdecl = true; ldv_istype_spec = false; ldv_isuniversal_type_spec = false; }
#line 2331 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 908 "ldv-aspect-parser.y" /* yacc.c:1646  */
    { ldv_isdecl = false; }
#line 2337 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 909 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
    (yyval.pps_declspecs) = (yyvsp[-1].pps_declspecs);
  }
#line 2345 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 915 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      if ((yyvsp[0].pps_declspecs))
        {
          /* TODO: implement ldv_merge_declspecs_free(). */
          pps_declspecs = ldv_merge_declspecs ((yyvsp[-1].pps_declspecs), (yyvsp[0].pps_declspecs));
          ldv_free_declspecs ((yyvsp[-1].pps_declspecs));
          ldv_free_declspecs ((yyvsp[0].pps_declspecs));
        }
      else
        pps_declspecs = (yyvsp[-1].pps_declspecs);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2367 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 933 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      if ((yyvsp[0].pps_declspecs))
        {
          pps_declspecs = ldv_merge_declspecs ((yyvsp[-1].pps_declspecs), (yyvsp[0].pps_declspecs));
          ldv_free_declspecs ((yyvsp[-1].pps_declspecs));
          ldv_free_declspecs ((yyvsp[0].pps_declspecs));
        }
      else
        pps_declspecs = (yyvsp[-1].pps_declspecs);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2388 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 950 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      if ((yyvsp[0].pps_declspecs))
        {
          pps_declspecs = ldv_merge_declspecs ((yyvsp[-1].pps_declspecs), (yyvsp[0].pps_declspecs));
          ldv_free_declspecs ((yyvsp[-1].pps_declspecs));
          ldv_free_declspecs ((yyvsp[0].pps_declspecs));
        }
      else
        pps_declspecs = (yyvsp[-1].pps_declspecs);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2409 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 967 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      if ((yyvsp[0].pps_declspecs))
        {
          pps_declspecs = ldv_merge_declspecs ((yyvsp[-1].pps_declspecs), (yyvsp[0].pps_declspecs));
          ldv_free_declspecs ((yyvsp[-1].pps_declspecs));
          ldv_free_declspecs ((yyvsp[0].pps_declspecs));
        }
      else
        pps_declspecs = (yyvsp[-1].pps_declspecs);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2430 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 984 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isany_params = true;

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2442 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 992 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvar_params = true;

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2454 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1002 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.pps_declspecs) = NULL;
    }
#line 2462 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 1006 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.pps_declspecs) = (yyvsp[0].pps_declspecs);
    }
#line 2470 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 1012 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"typedef\" storage class specifier");
      LDV_FATAL_ERROR ("Typedefs aren't supported at the moment");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2487 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1025 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isextern = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"extern\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2503 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1037 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstatic = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"static\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2519 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 1049 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isauto = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"auto\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2535 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1061 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isregister = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"register\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2551 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1075 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvoid = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"void\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2567 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1087 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->ischar = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"char\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2583 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1099 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"int\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2599 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1111 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isfloat = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"float\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2615 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1123 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isdouble = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"double\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2631 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1135 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isbool = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"bool\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2647 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1147 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->iscomplex = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"complex\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2663 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1159 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isshort = true;

      /* Assume that 'int' presents if 'short' is encountered. Don't do this if
         'double' type specifier was already parsed. */
      if (!pps_declspecs->isdouble)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"short\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2684 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1176 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->islong = true;

      /* Assume that 'int' presents if 'long' is encountered. Don't do this if
       * 'double' type specifier was already parsed. */
      if (!pps_declspecs->isdouble)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"long\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2705 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1193 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->issigned = true;

      /* Assume that 'int' presents if 'signed' is encountered. Don't do this if
       * 'double' or 'char' type specifier was already parsed. */
      if (!pps_declspecs->isdouble && !pps_declspecs->ischar)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"signed\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2726 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1210 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isunsigned = true;

      /* Assume that 'int' presents if 'unsigned' is encountered. Don't do this if
       * 'double' or 'char' type specifier was already parsed. */
      if (!pps_declspecs->isdouble && !pps_declspecs->ischar)
        pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"unsigned\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2747 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1227 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstruct = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"struct\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2764 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1240 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isunion = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"union\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2781 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1253 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isenum = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"enum\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2798 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1266 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef_name = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed typedef name \"%s\" type specifier", ldv_get_id_name ((yyvsp[0].id)));

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2815 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1279 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isuniversal_type_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed universal type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2831 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1293 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isconst = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"const\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2847 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1305 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isrestrict = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"restrict\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2863 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1317 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvolatile = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"volatile\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2879 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1331 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isinline = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"inline\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2895 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1345 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed declarator");

      (yyval.list) = ldv_list_splice ((yyvsp[-1].list), (yyvsp[0].list));
    }
#line 2905 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1353 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declarator_ptr declarator_new = NULL;
      ldv_list_ptr declarator_list = NULL;

      declarator_new = ldv_create_declarator ();

      declarator_list = ldv_list_push_front (NULL, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ID;

      declarator_new->declarator_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed identifier direct declarator");

      (yyval.list) = declarator_list;
    }
#line 2926 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1370 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct declarator of associativity");

      (yyval.list) = (yyvsp[-1].list);
    }
#line 2936 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1376 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[-3].list) = ldv_list_push_front ((yyvsp[-3].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ARRAY;

      declarator_new->pps_array_size = ldv_create_pps_array_size ();

      /* If an array size was specified then save it. */
      if ((yyvsp[-1].integer))
        {
          declarator_new->pps_array_size->pps_array_size = ldv_get_int ((yyvsp[-1].integer));
          /* Any size is specified this case. */
          if (declarator_new->pps_array_size->pps_array_size == -1)
            declarator_new->pps_array_size->isany_size = true;
          declarator_new->pps_array_size->issize_specified = true;
        }
      else
        declarator_new->pps_array_size->issize_specified = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed array direct declarator");

      (yyval.list) = (yyvsp[-3].list);
    }
#line 2968 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1404 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[-3].list) = ldv_list_push_front ((yyvsp[-3].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = (yyvsp[-1].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct declarator");

      (yyval.list) = (yyvsp[-3].list);
    }
#line 2988 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1422 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = NULL;
    }
#line 2996 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1426 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3004 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1432 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declarator_ptr declarator_new = NULL;
      ldv_list_ptr declarator_list = NULL;

      declarator_new = ldv_create_declarator ();

      declarator_list = ldv_list_push_front (NULL, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      declarator_new->pps_ptr_quals = ldv_set_ptr_quals ((yyvsp[0].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison parsed pointer declarator");

      (yyval.list) = declarator_list;
    }
#line 3025 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1449 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[0].list) = ldv_list_push_front ((yyvsp[0].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      declarator_new->pps_ptr_quals = ldv_set_ptr_quals ((yyvsp[-1].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison parsed pointer declarator");

      (yyval.list) = (yyvsp[0].list);
    }
#line 3045 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1467 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.integer) = NULL;
    }
#line 3053 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1471 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.integer) = (yyvsp[0].integer);
    }
#line 3061 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1476 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_int_ptr integer = NULL;

      integer = ldv_create_int ();
      integer->numb = -1;

      (yyval.integer) = integer;
    }
#line 3074 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1487 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.pps_declspecs) = NULL;
    }
#line 3082 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1491 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.pps_declspecs) = (yyvsp[0].pps_declspecs);
    }
#line 3090 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1497 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.pps_declspecs) = (yyvsp[0].pps_declspecs);
    }
#line 3098 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1501 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      if ((yyvsp[0].pps_declspecs))
        {
          pps_declspecs = ldv_merge_declspecs ((yyvsp[-1].pps_declspecs), (yyvsp[0].pps_declspecs));
          ldv_free_declspecs ((yyvsp[-1].pps_declspecs));
          ldv_free_declspecs ((yyvsp[0].pps_declspecs));
        }
      else
        pps_declspecs = (yyvsp[-1].pps_declspecs);

      ldv_print_info (LDV_INFO_BISON, "bison merged type qualifiers");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 3119 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1520 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_func_arg_ptr pps_func_arg = NULL;
      ldv_list_ptr pps_func_arg_list = NULL;

      /* Check that '...' wasn't used anywhere except the end of parameter
         list. '*/
      for (pps_func_arg_list = (yyvsp[0].list)
        ; pps_func_arg_list
        ; pps_func_arg_list = ldv_list_get_next (pps_func_arg_list))
        {
          pps_func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (pps_func_arg_list);

          if (pps_func_arg->isva && ldv_list_get_next (pps_func_arg_list))
            {
              LDV_FATAL_ERROR ("Used '...' not at the end of parameter list");
            }
        }

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter list");

      (yyval.list) = (yyvsp[0].list);
    }
#line 3146 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1545 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_func_arg_ptr pps_func_arg_new = NULL;
      ldv_list_ptr func_arg_list = NULL;

      pps_func_arg_new = ldv_create_pps_func_arg ();

      pps_func_arg_new->pps_func_arg = (yyvsp[0].pps_decl);

      /* First parameter can't be '...'. */
      pps_func_arg_new->isva = false;

      ldv_list_push_back (&func_arg_list, pps_func_arg_new);

      (yyval.list) = func_arg_list;
    }
#line 3166 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1561 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_func_arg_ptr pps_func_arg_new = NULL;
      ldv_pps_func_arg_ptr pps_func_arg_last = NULL;
      ldv_list_ptr pps_func_arg_list = NULL;

      if ((yyvsp[0].pps_decl)->pps_decl_kind == LDV_PPS_DECL_ELLIPSIS)
        {
          pps_func_arg_list = ldv_list_get_last ((yyvsp[-2].list));

          pps_func_arg_last = (ldv_pps_func_arg_ptr) ldv_list_get_data (pps_func_arg_list);

          pps_func_arg_last->isva = true;
        }
      else
        {
          pps_func_arg_new = ldv_create_pps_func_arg ();

          pps_func_arg_new->pps_func_arg = (yyvsp[0].pps_decl);

          ldv_list_push_back (&(yyvsp[-2].list), pps_func_arg_new);

          pps_func_arg_new->isva = false;
        }

      (yyval.list) = (yyvsp[-2].list);
    }
#line 3197 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1590 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify a kind of a declaration. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_PARAM;

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = (yyvsp[-1].pps_declspecs);

      /* Specify a declarator. */
      pps_decl->pps_declarator = (yyvsp[0].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter declaration");

      (yyval.pps_decl) = pps_decl;
    }
#line 3220 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1609 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_decl_ptr pps_decl = NULL;
      ldv_pps_declarator_ptr declarator = NULL;
      ldv_list_ptr declarator_list = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = (yyvsp[-1].pps_declspecs);

      /* Specify that any parameters may correspond to this 'declaration'. */
      if (pps_decl->pps_declspecs->isany_params)
        {
          pps_decl->pps_decl_kind = LDV_PPS_DECL_ANY_PARAMS;

          ldv_print_info (LDV_INFO_BISON, "bison parsed any parameters wildcard");
        }
      /* Specify that variable parameters may correspond to this 'declaration'. */
      else if (pps_decl->pps_declspecs->isvar_params)
        {
          /* Specify a kind of a declaration. */
          pps_decl->pps_decl_kind = LDV_PPS_DECL_ELLIPSIS;

          ldv_print_info (LDV_INFO_BISON, "bison parsed parameter list of variable length");
        }
      else
        {
          /* Specify a kind of a declaration. */
          pps_decl->pps_decl_kind = LDV_PPS_DECL_PARAM;

          /* Specify an abstact declarator. */
          pps_decl->pps_declarator = (yyvsp[0].list);

          ldv_print_info (LDV_INFO_BISON, "bison parsed parameter declaration");
        }

      if (pps_decl->pps_declspecs->isany_params
        || pps_decl->pps_declspecs->isvar_params)
        {
          for (declarator_list = (yyvsp[0].list); declarator_list; declarator_list = ldv_list_get_next (declarator_list))
            {
              declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

              ldv_free_declarator (declarator);
            }

          ldv_list_delete_all ((yyvsp[0].list));
        }

      (yyval.pps_decl) = pps_decl;
    }
#line 3276 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1663 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      (yyval.list) = (yyvsp[0].list);
    }
#line 3286 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1669 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      (yyval.list) = ldv_list_splice ((yyvsp[-1].list), (yyvsp[0].list));
    }
#line 3296 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1677 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_list_ptr declarator_list = NULL;

      declarator_list = ldv_list_push_front (NULL, ldv_create_declarator ());

      ldv_print_info (LDV_INFO_BISON, "bison parsed empty abstract declarator");

      (yyval.list) = declarator_list;
    }
#line 3310 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1687 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3318 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1693 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct abstract declarator of associativity");

      (yyval.list) = (yyvsp[-1].list);
    }
#line 3328 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1699 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[-3].list) = ldv_list_push_front ((yyvsp[-3].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ARRAY;

      declarator_new->pps_array_size = ldv_create_pps_array_size ();

      /* If an array size was specified then save it. */
      if ((yyvsp[-1].integer))
        {
          declarator_new->pps_array_size->pps_array_size = ldv_get_int ((yyvsp[-1].integer));
          declarator_new->pps_array_size->issize_specified = true;
        }
      else
        declarator_new->pps_array_size->issize_specified = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed array direct abstract declarator");

      (yyval.list) = (yyvsp[-3].list);
    }
#line 3357 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1725 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[-3].list) = ldv_list_push_front ((yyvsp[-3].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = (yyvsp[-1].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct abstract declarator");

      (yyval.list) = (yyvsp[-3].list);
    }
#line 3377 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1743 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = NULL;
    }
#line 3385 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1747 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3393 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1753 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = NULL;
    }
#line 3401 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1757 "ldv-aspect-parser.y" /* yacc.c:1646  */
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3409 "ldv-aspect-parser.c" /* yacc.c:1646  */
    break;


#line 3413 "ldv-aspect-parser.c" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc);
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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 1761 "ldv-aspect-parser.y" /* yacc.c:1906  */


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
       LDV_PPS_DEFINE - LDV_PP_EXPAND
       LDV_PPS_FILE - LDV_PP_FILE
       LDV_PPS_FILE - LDV_PP_INFILE
       LDV_PPS_DECL - LDV_PP_CALL
       LDV_PPS_DECL - LDV_PP_CALLP
       LDV_PPS_DECL - LDV_PP_USE_FUNC
       LDV_PPS_DECL - LDV_PP_DECLARE_FUNC
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
      if (pp_kind == LDV_PP_DEFINE || pp_kind == LDV_PP_EXPAND)
        return ;

      break;

    case LDV_PPS_FILE:
      if (pp_kind == LDV_PP_FILE || pp_kind == LDV_PP_INFILE)
        return ;

      break;

    case LDV_PPS_DECL:
      if (pp_kind == LDV_PP_CALL || pp_kind == LDV_PP_CALLP
        || pp_kind == LDV_PP_USE_FUNC || pp_kind == LDV_PP_DECLARE_FUNC
        || pp_kind == LDV_PP_EXECUTION || pp_kind == LDV_PP_GET
        || pp_kind == LDV_PP_GET_GLOBAL || pp_kind == LDV_PP_GET_LOCAL
        || pp_kind == LDV_PP_INCALL || pp_kind == LDV_PP_INFUNC
        || pp_kind == LDV_PP_INIT || pp_kind == LDV_PP_INIT_GLOBAL
        || pp_kind == LDV_PP_INIT_LOCAL || pp_kind == LDV_PP_INTRODUCE
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

  c_pointcut->cp_type = LDV_CP_TYPE_NONE;

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

  int i;

  /* Bind an identifier with some C keyword when parse declaration specifiers. */
  if (ldv_isdecl)
    {
      /* Check whether an identifier is a C declaration specifier keyword. */
      for (i = 0; ldv_keyword_token_map[i].keyword; i++)
        {
          if (!strcmp (id, ldv_keyword_token_map[i].keyword))
            {
              ldv_print_info (LDV_INFO_LEX, "lex parsed keyword \"%s\"", id);

              /* Mark that some type specifier for a given declaration
                 specifiers list was parsed. */
              if (i > 4 && i < 19)
                ldv_istype_spec = true;

              /* Corresponding identifier won't be used any more. */
              ldv_free_id (yylval.id);

              return ldv_keyword_token_map[i].token;
            }
        }

      if (!ldv_istype_spec && !ldv_isuniversal_type_spec)
        {
          /* A typedef name is encountered when declaration specifiers are
             parsed and a nonkeyword identifier was met. We don't treat pure
             '$' as typedef name to avoid ambiguity with universal type
             specifier. */
          if (!strcmp(id, "$"))
            {
              ldv_print_info (LDV_INFO_LEX, "lex parsed universal type specifier \"%s\"", id);

              ldv_isuniversal_type_spec = true;

              /* Corresponding identifier won't be used any more. */
              ldv_free_id (yylval.id);

              return LDV_UNIVERSAL_TYPE_SPECIFIER;
            }
          else
            {
              ldv_print_info (LDV_INFO_LEX, "lex parsed typedef name \"%s\"", id);

              ldv_istype_spec = true;

              return LDV_TYPEDEF_NAME;
            }
        }
    }

  if (!ldv_ismacro && !ldv_isdecl)
    {
      if (!strcmp (id, "expand") || !strcmp (id, "define"))
        return LDV_MACRO_POINTCUT;
      else if (!strcmp (id, "file") || !strcmp (id, "infile"))
        return LDV_FILE_POINTCUT;
    }

  ldv_print_info (LDV_INFO_LEX, "lex parsed identifier \"%s\"", id);
  return LDV_ID;
}

void
ldv_hash_add_name (char *name)
{
  void **hash_element;

  hash_element = htab_find_slot_with_hash (ldv_called_func_names, name, (*htab_hash_string) (name), INSERT);

  if (hash_element == NULL)
    {
      LDV_FATAL_ERROR ("Can't allocate memory");
    }

  /* Assign a given called function name to a hash element. */
  if (*hash_element == NULL)
    *hash_element = (void *) name;
}

void
ldv_hash_recursive_composite_pointcut (ldv_cp_ptr c_pointcut)
{
  ldv_i_func_ptr i_func = NULL;

  if (c_pointcut != NULL)
    {
      ldv_hash_recursive_composite_pointcut (c_pointcut->c_pointcut_first);

      if ((c_pointcut->cp_kind == LDV_CP_PRIMITIVE) && (c_pointcut->p_pointcut->pp_kind == LDV_PP_CALL))
        {
          /* TODO: obtain function name on the basis of declaration (see ldv_convert_func_signature_to_internal()) rather then utilize this expensive transformation. */
          i_func = ldv_convert_func_signature_to_internal (c_pointcut->p_pointcut->pp_signature->pps_declaration);

          /* Store special "$" function name in hash table if "$" wildcard was
             used elsewhere through function names. Later this special function
             name will be used to denote that we needn't to try find function
             name in hash table since it can be matched by some name with "$"
             wildcard. */
          if (i_func->name->isany_chars)
            /* TODO: ldv_copy_str() === xstrdup(). */
            ldv_hash_add_name (ldv_copy_str ("$"));
          else
            ldv_hash_add_name (ldv_copy_str (ldv_get_id_name (i_func->name)));

          ldv_free_info_func (i_func);
        }

      ldv_hash_recursive_composite_pointcut (c_pointcut->c_pointcut_second);
    }
}

void ldv_hash_string_del (void *s_p)
{
    free (s_p);
}

int
ldv_hash_string_eq (const void *s1_p, const void *s2_p)
{
  const char *s1 = (const char *) s1_p;
  const char *s2 = (const char *) s2_p;
  return strcmp (s1, s2) == 0;
}

hashval_t
ldv_hash_string_hash (const void *s_p)
{
  const char *s = (const char *) s_p;
  return (*htab_hash_string) (s);
}

/* We aren't interested in advice body content, except for correct determinition
   of the advice body end (that is '}') and aspect patterns. */
unsigned int
ldv_parse_advice_body (ldv_ab_ptr *body)
{
  int c;
  int brace_count = 1;
  ldv_aspect_pattern_ptr pattern = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  ldv_text_ptr comment = NULL;

  c = ldv_getc (LDV_ASPECT_STREAM);

  /* Open brace begins an advice body. */
  if (c == '{')
    {
      *body = ldv_create_body ();
      ldv_putc_body (c, *body);
      ldv_set_last_column (yylloc.last_column + 1);

      while (1)
        {
          /* Parse aspect patterns inside an advice body. */
          if ((pattern = ldv_parse_aspect_pattern ()))
            {
              body_pattern = ldv_create_body_aspect_pattern ();
              body_pattern->pattern = pattern;
              body_pattern->place = LDV_STR_OFFSET (ldv_get_body_text (*body));
              ldv_list_push_back (&((*body)->patterns), body_pattern);
              continue;
            }

          /* Parse comments inside a body. We should keep them since there may
             be model comments through them. */
          if ((comment = ldv_parse_comments ()))
            {
              ldv_puts_body (ldv_get_text (comment), *body);
              ldv_free_text (comment);
              continue;
            }

          if ((c = ldv_getc (LDV_ASPECT_STREAM)) == EOF)
            {
              LDV_FATAL_ERROR ("End of file is reached but advice body \"%s\" isn't completed", ldv_get_body_text (*body));
            }

          /* Process the end of line inside a body. */
          if (c == '\n')
            {
              ldv_set_last_line (yylloc.last_line + 1);
              ldv_set_last_column (1);
              /* Reset initial position as well. */
              ldv_set_first_line (yylloc.last_line);
              ldv_set_first_column (yylloc.last_column);
              ldv_putc_body (c, *body);
              continue;
            }

          ldv_putc_body (c, *body);
          ldv_set_last_column (yylloc.last_column + 1);

          /* Increase/decrease a brace counter to skip '{...}' construction
             inside a body. */
          if (c == '{')
            brace_count++;
          else if (c == '}')
            brace_count--;

          if (c == '}' && !brace_count)
            break;
        }

      /* Advice body was parsed successfully. */
      return 1;
    }

  /* Push back a non advice body character. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);

  /* That is advice body wasn't read. */
  return 0;
}

ldv_aspect_pattern_ptr
ldv_parse_aspect_pattern (void)
{
  int c, c_next;
  ldv_aspect_pattern_ptr pattern = NULL;
  char *str = NULL;
  bool isany_chars = false;
  unsigned int integer;
  ldv_list_ptr params = NULL;

  c = ldv_getc (LDV_ASPECT_STREAM);

  /* '$' begins an aspect pattern. */
  if (c == '$')
    {
      ldv_set_last_column (yylloc.last_column + 1);

      /* Move first column pointer to the beginning of pattern. */
      ldv_set_first_column (yylloc.last_column);

      pattern = ldv_create_aspect_pattern ();

      /* Process special aspect pattern '$$' that means '$' wildcard for file
         names actually. */
      if ((c_next = ldv_getc (LDV_ASPECT_STREAM)) == '$')
        {
          ldv_set_last_column (yylloc.last_column + 1);
          pattern->value = "$";

          /* Skip following processing since pattern was already read. */
          return pattern;
        }
      else
        ldv_ungetc (c_next, LDV_ASPECT_STREAM);

      /* Parse aspect pattern name. */
      if (ldv_parse_id (&str, &isany_chars, true))
        {
          pattern->name = str;
          ldv_print_info (LDV_INFO_LEX, "lex parsed aspect pattern name \"%s\"", pattern->name);
        }
      else
        {
          LDV_FATAL_ERROR ("aspect pattern hasn't a name!");
        }

      /* Some aspect patterns require corresponding argument numbers. */
      if (ldv_parse_unsigned_integer (&integer))
        {
          pattern->arg_numb = integer;
          ldv_print_info (LDV_INFO_LEX, "lex parsed aspect pattern argument number \"%u\"", pattern->arg_numb);
        }
      else if (!strcmp (pattern->name, "arg")
        || !strcmp (pattern->name, "arg_type")
        || !strcmp (pattern->name, "arg_size")
        || !strcmp (pattern->name, "arg_value"))
        {
          LDV_FATAL_ERROR ("aspect pattern \"%s\" requires corresponding argument number to be specified", pattern->name);
        }

      /* Parse aspect pattern parameters if so. */
      if ((params = ldv_parse_aspect_pattern_params ()))
        pattern->params = params;

      /* Evaluate environment variables immediately after parsing. */
      if (!strcmp (pattern->name, "env"))
        pattern->value = ldv_get_aspect_pattern_env (pattern);

      /* Use a special value for aspect pattern pointing to a file instrumented. */
      if (!strcmp (pattern->name, "this"))
        pattern->value = "$this";

      return pattern;
    }
  else
    {
      /* Push back a non aspect pattern character. */
      ldv_ungetc (c, LDV_ASPECT_STREAM);
    }

  /* That is aspect pattern wasn't read. */
  return NULL;
}

ldv_aspect_pattern_param_ptr
ldv_parse_aspect_pattern_param (void)
{
  ldv_aspect_pattern_param_ptr param = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;
  char *str = NULL;
  bool isany_chars = false;
  unsigned int integer;

  param = ldv_create_aspect_pattern_param ();

  if ((pattern = ldv_parse_aspect_pattern ()))
    {
      param->kind = LDV_ASPECT_PATTERN_ASPECT_PATTERN;
      param->aspect_pattern = pattern;

      ldv_print_info (LDV_INFO_LEX, "lex parsed aspect pattern parameter that is aspect pattern");

      return param;
    }

  if (ldv_parse_unsigned_integer (&integer))
    {
      param->kind = LDV_ASPECT_PATTERN_INTEGER;
      param->integer = integer;

      ldv_print_info (LDV_INFO_LEX, "lex parsed unsigned int aspect pattern parameter \"%u\"", integer);

      return param;
    }

  if (ldv_parse_id (&str, &isany_chars, true))
    {
      param->kind = LDV_ASPECT_PATTERN_STRING;
      param->string = str;

      ldv_print_info (LDV_INFO_LEX, "lex parsed string aspect pattern parameter \"%s\"", str);

      return param;
    }

  if (ldv_parse_aspect_pattern_param_str (&str))
    {
      param->kind = LDV_ASPECT_PATTERN_STRING;
      param->string = str;

      ldv_print_info (LDV_INFO_LEX, "lex parsed string aspect pattern parameter \"%s\"", str);

      return param;
    }

  /* That is aspect pattern parameter wasn't read. */
  return NULL;
}

unsigned int
ldv_parse_aspect_pattern_param_str (char **str)
{
  int c;
  unsigned int byte_count = 0;
  ldv_str_ptr str_read = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;

  c = ldv_getc (LDV_ASPECT_STREAM);

  /* String aspect pattern parameter is a sequence of characters in quotes. */
  if (c == '"')
    {
      byte_count++;
      str_read = ldv_create_string ();

      /* Get the rest of string. */
      while (1)
        {
          if ((pattern = ldv_parse_aspect_pattern ()))
            ldv_puts_string (pattern->value, str_read);

          c = ldv_getc (LDV_ASPECT_STREAM);
          byte_count++;

          if (c == '"')
            break;
          /* Parse escape sequences and keep their equivalents. */
          else if (c == '\\')
            {
              c = ldv_getc (LDV_ASPECT_STREAM);
              byte_count++;
              switch (c)
                {
                case '\'':
                  ldv_putc_string ('\'', str_read);
                  break;
                case '"':
                  ldv_putc_string ('\"', str_read);
                  break;
                case '?':
                  ldv_putc_string ('\?', str_read);
                  break;
                case '\\':
                  ldv_putc_string ('\\', str_read);
                  break;
                case 'a':
                  ldv_putc_string ('\a', str_read);
                  break;
                case 'b':
                  ldv_putc_string ('\b', str_read);
                  break;
                case 'f':
                  ldv_putc_string ('\f', str_read);
                  break;
                case 'n':
                  ldv_putc_string ('\n', str_read);
                  break;
                case 'r':
                  ldv_putc_string ('\r', str_read);
                  break;
                case 't':
                  ldv_putc_string ('\t', str_read);
                  break;
                case 'v':
                  ldv_putc_string ('\v', str_read);
                  break;
                case '$':
                  ldv_putc_string ('$', str_read);
                  break;
                default:
                  LDV_FATAL_ERROR ("Can't process escape sequence '\\%c'", c);
                }
            }
          else
            ldv_putc_string (c, str_read);
        }
    }

  if (byte_count)
    {
      /* Assign read from stream string to string passed through parameter. */
      *str = ldv_get_str (str_read);
      /* Move current position properly and return the number of bytes read. */
      ldv_set_last_column (yylloc.last_column + byte_count);
      return byte_count;
    }

  /* Don't assign any value to string passed through parameter. 0 bytes were
     read from stream. */
  return 0;
}

ldv_list_ptr
ldv_parse_aspect_pattern_params (void)
{
  int c;
  ldv_list_ptr params = NULL;
  ldv_aspect_pattern_param_ptr param = NULL;

  c = ldv_getc (LDV_ASPECT_STREAM);

  /* '<' begins an aspect pattern parameters list. */
  if (c == '<')
    {
      ldv_set_last_column (yylloc.last_column + 1);

      while (1)
        {
          if ((param = ldv_parse_aspect_pattern_param ()))
            {
              ldv_list_push_back (&params, param);
            }
          else
            {
              LDV_FATAL_ERROR ("aspect pattern parameter wasn't parsed");
            }

          /* Next symbol may be either ',' (that means that there is some other
             aspect pattern parameters) or '>' (that finishes parameters list). */
          ldv_set_last_column (yylloc.last_column + 1);
          switch (c = ldv_getc (LDV_ASPECT_STREAM))
            {
            case ',':
              break;

            case '>':
              return params;

            default:
              LDV_FATAL_ERROR ("aspect pattern parameters list has incorrect format!");
            }
        }
    }
  else
    {
      /* Push back a non aspect pattern parameter character. */
      ldv_ungetc (c, LDV_ASPECT_STREAM);
    }

  /* That is aspect pattern parameter wasn't read. */
  return NULL;
}

int
ldv_parse_aspect_pattern_known_value (char **str)
{
  ldv_aspect_pattern_ptr pattern = NULL;

  /* First of all try to parse aspect pattern. */
  if ((pattern = ldv_parse_aspect_pattern ()))
    {
      if (pattern->value)
        {
          *str = ldv_copy_str (pattern->value);

          ldv_free_aspect_pattern (pattern);

          return 1;
        }
      else
        {
          /* Increase and decrease a current last column position artificially
             to meet standard error conditions and print information on errors
             properly. */
          ldv_set_last_column (yylloc.last_column + 1);
          ldv_print_info_location (yylloc, LDV_ERROR_LEX, "incorrect aspect pattern \"%s\" was used", pattern->name);
          ldv_set_last_column (yylloc.last_column - 1);
          LDV_FATAL_ERROR ("aspect pattern \"%s\" cannot be calculated without matching", pattern->name);
        }
    }

  /* That is aspect pattern wasn't read. */
  return 0;
}

ldv_text_ptr
ldv_parse_comments (void)
{
  int c, c_next;
  ldv_text_ptr comment = NULL;

  while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
    {
      /* A possible comment beginning. */
      if (c == '/')
        {
          /* Define a comment type (C or C++) by means of a following
             character. */
          if ((c_next = ldv_getc (LDV_ASPECT_STREAM)) == EOF)
            {
              LDV_FATAL_ERROR ("end of file is reached, but comment isn't completed");
            }

          /* Drop a C++ comment '//...\n' from '//' up to the end of a line.
             Don't track a current file position. */
          if (c_next == '/')
            {
              comment = ldv_create_text ();
              ldv_puts_text ("//", comment);

              while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
                {
                  /* Push back the end of line to calculate position properly. */
                  if (c == '\n')
                    {
                      ldv_ungetc (c, LDV_ASPECT_STREAM);
                      break;
                    }

                  ldv_putc_text (c, comment);
                  ldv_print_info (LDV_INFO_IO, "read C++ comment character \"%c\"", ldv_end_of_line (c));
                }

              /* So, we'll skip following whitespaces and comments if so. */
              return comment;
            }
          /* Drop a C comment '/_*...*_/' from '/_*' up to '*_/'. Track a
             current file position, since '*_/' can be placed at another line
             and there may be other symbols after it. */
          else if (c_next == '*')
            {
              comment = ldv_create_text ();
              ldv_puts_text ("/*", comment);
              ldv_set_last_column (yylloc.last_column + 2);

              while ((c = ldv_getc (LDV_ASPECT_STREAM)) != EOF)
                {
                  /* See whether a C comment end is reached. */
                  if (c == '*')
                    {
                      c_next = ldv_getc (LDV_ASPECT_STREAM);

                      if (c_next != EOF && c_next == '/')
                        {
                          ldv_puts_text ("*/", comment);
                          ldv_set_last_column (yylloc.last_column + 2);
                          /* So, we'll skip following whitespaces and comments if so. */
                          return comment;
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

                  ldv_putc_text (c, comment);
                  ldv_print_info (LDV_INFO_IO, "read C comment character \"%c\"", ldv_end_of_line (c));
                }

              LDV_FATAL_ERROR ("End of file is reached but C comment \"%s\" isn't completed", ldv_get_text (comment));
            }
          else
            {
              /* Push back non comment characters. */
              ldv_ungetc (c_next, LDV_ASPECT_STREAM);
              ldv_ungetc (c, LDV_ASPECT_STREAM);
              break;
            }
        }
      else
        {
          /* Push back a non comment character. */
          ldv_ungetc (c, LDV_ASPECT_STREAM);
          break;
        }
    }

  /* No comments were parsed. */
  return NULL;
}

/* Correct file name has the form "file_name" */
unsigned int
ldv_parse_file_name (char **file_name)
{
  int c;
  unsigned int byte_count = 0;
  ldv_str_ptr str = NULL;
  char *aspect_pattern_value = NULL;

  c = ldv_getc (LDV_ASPECT_STREAM);

  /* Quote begins a file name. */
  if (c == '"')
    {
      byte_count++;
      str = ldv_create_string ();

      /* Get the rest of a file name. Note, that there may be aspect patterns
         with known values in this name. */
      while (1)
        {
          if (ldv_parse_aspect_pattern_known_value (&aspect_pattern_value))
            {
              ldv_puts_string (aspect_pattern_value, str);
              free (aspect_pattern_value);
              continue;
            }

          if ((c = ldv_getc (LDV_ASPECT_STREAM)) == EOF)
            break;

          byte_count++;

          /* Second quote finishes a given file name. */
          if (c == '"')
            break;

          if (c == '\n')
            {
              LDV_FATAL_ERROR ("file path isn't terminated with quote - it's terminated with the end of line");
            }

          ldv_putc_string (c, str);
        }

      /* Assign read file name to file name passed through parameter. */
      *file_name = ldv_copy_str (ldv_get_str (str));
      ldv_free_str (str);

      /* Move current position properly. */
      ldv_set_last_column (yylloc.last_column + byte_count);

      return byte_count;
    }

  /* Push back a non file name character. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);

  /* That is file name wasn't read. */
  return 0;
}

/* Correct identifier or C keyword begins with any character (in a low or an
   upper case) or '_'.  Subsequent identifier symbols are characters, digits and
   '_'. Aspectator identifier may contain any number of '$' wildcards each of
   which is equvalent to any lenght of characters. Aspect pattern name and
   parameter cannot containt either digits or '$' wildcards. */
unsigned int
ldv_parse_id (char **id, bool *isany_chars, bool isaspect_pattern)
{
  int c;
  unsigned int byte_count = 0;
  ldv_str_ptr str = NULL;
  bool isread_any_chars = false;

  c = ldv_getc (LDV_ASPECT_STREAM);

  if (ISIDST (c) || (!isaspect_pattern && c == '$'))
    {
      str = ldv_create_string ();

      /* Get the rest of an identifier. */
      do
        {
          byte_count++;
          ldv_putc_string (c, str);
          if (c == '$')
            isread_any_chars = true;

          c = ldv_getc (LDV_ASPECT_STREAM);
        }
      while (isaspect_pattern ? ISIDST (c) : ISIDNUM (c) || c == '$');
    }

  /* Push back a first nonidentifier character. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);

  if (str)
    {
      /* Assign read identifier to identifier passed through parameter. */
      *id = ldv_copy_str (ldv_get_str (str));
      ldv_free_string (str);

      /* Specify whether '$' wildcard was read or not. */
      *isany_chars = isread_any_chars;

      /* Move current position properly. */
      ldv_set_last_column (yylloc.last_column + byte_count);

      return byte_count;
    }

  /* That is identifier wasn't read. */
  return 0;
}

/* Preprocessor special directives have the following format:
     # \d+ "[^"]+" \d+
   where the first number denotes the following line number in the file
   specified inside quotes. There are may be less or more numbers at the end
   of such lines. A current position isn't tracked while processing such
   lines. */
int
ldv_parse_preprocessor_directives (void)
{
  int c, c_next;
  unsigned int line_numb;
  ldv_str_ptr file_name;

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

                  /* Free previously used file name. */
                  free (yylloc.file_name);

                  ldv_set_file_name (ldv_copy_str (ldv_get_str (file_name)));

                  ldv_free_str (file_name);
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

          /* So, lexer should be envolved from the beginning. */
          return 1;
        }
      else
        {
          ldv_ungetc (c, LDV_ASPECT_STREAM);
          break;
        }
    }

  /* That is a special preprocessor directive wasn't read. */
  return 0;
}

unsigned int
ldv_parse_unsigned_integer (unsigned int *integer)
{
  int c;
  unsigned int byte_count = 0;
  unsigned int integer_read = 0;

  c = ldv_getc (LDV_ASPECT_STREAM);

  if (ISDIGIT (c))
    {
      do
        {
          integer_read = 10 * integer_read + (c - '0');
          byte_count++;
          c = ldv_getc (LDV_ASPECT_STREAM);
        }
      while (ISDIGIT (c));
    }

  /* Push back a first nondigit character. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);

  if (byte_count)
    {
      /* Assign read from stream integer to integer passed through parameter. */
      *integer = integer_read;
      /* Move current position properly and return the number of bytes read. */
      ldv_set_last_column (yylloc.last_column + byte_count);
      return byte_count;
    }

  /* Don't assign any value to integer passed through parameter. 0 bytes were
     read from stream. */
  return 0;
}

void
ldv_parse_whitespaces (void)
{
  int c;

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
  int c, c_next;
  ldv_ab_ptr body = NULL;
  ldv_file_ptr file = NULL;
  char *str = NULL;
  bool isany_chars = false;
  ldv_id_ptr id = NULL;
  unsigned int i;
  ldv_int_ptr integer = NULL;
  ldv_text_ptr comment = NULL;
  int id_kind;

  /* Skip nonsignificant whitespaces from the beginning of a current line. */
  ldv_parse_whitespaces ();

  /* Examine whether a C or C++ comment is encountered. Skip a corresponding
     comment if so and continue parsing. */
  if ((comment = ldv_parse_comments ()))
    {
      ldv_free_text (comment);
      return yylex ();
    }

  /* Examine whether a special preprocessor line is encountered. */
  if (ldv_parse_preprocessor_directives ())
    return yylex ();

  /* Initialize a first line and a first column. */
  ldv_set_first_line (yylloc.last_line);
  ldv_set_first_column (yylloc.last_column);

  /* Get a first character of a token. */
  c = ldv_getc (LDV_ASPECT_STREAM);

  /* Reach the end of a file. So aspect parser must finish its work. */
  if (c == EOF)
    {
      ldv_print_info (LDV_INFO_LEX, "lex reached the end of file");
      return 0;
    }

  /* Push back non EOF character. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);

  /* Parse an advice body. */
  if (ldv_parse_advice_body (&body))
    {
      /* Set a corresponding semantic value. */
      yylval.body = body;

      ldv_print_info (LDV_INFO_LEX, "lex parsed body \"%s\"", ldv_get_body_text (body));
      return LDV_BODY;
    }

  /* Parse a file name. */
  if (ldv_parse_file_name (&str))
    {
      /* Initialize corresponding internal structure. */
      file = ldv_create_file ();
      ldv_puts_file (str, file);

      /* Check a special file name pattern that refer to a file processed. */
      if (!strcmp (str, "$this"))
        file->isthis = true;
      else
        file->isthis = false;

      free (str);

      /* Set a corresponding semantic value. */
      yylval.file = file;

      ldv_print_info (LDV_INFO_LEX, "lex parsed file name \"%s\"", ldv_get_file_name (file));
      return LDV_FILE;
    }

  /* Parse some identifier or a C keyword. */
  if (ldv_parse_id (&str, &isany_chars, false))
    {
      /* Initialize corresponding internal structure. */
      id = ldv_create_id ();
      ldv_puts_id (str, id);
      id->isany_chars = isany_chars;

      /* Set a corresponding semantic value. */
      yylval.id = id;

      id_kind = ldv_get_id_kind (str);
      free (str);

      return id_kind;
    }

  /* Parse some integer number. It consists of digits. */
  if (ldv_parse_unsigned_integer (&i))
    {
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
