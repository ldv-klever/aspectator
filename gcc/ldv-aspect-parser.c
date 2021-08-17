/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30704

/* Bison version string.  */
#define YYBISON_VERSION "3.7.4"

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

/* First part of user prologue.  */
#line 25 "ldv-aspect-parser.y"


/* For ISALPHA, ISDIGIT, etc. functions. */
#include <safe-ctype.h>

/* General gcc core types and functions. */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "input.h"
#include "tm.h"
#include "intl.h"
#include "hash-set.h"
#include "vec.h"
#include "symtab.h"
#include "input.h"
#include "alias.h"
#include "double-int.h"
#include "machmode.h"
#include "inchash.h"
#include "tree.h"
#include "fold-const.h"
#include "print-tree.h"
#include "stor-layout.h"
#include "varasm.h"
#include "attribs.h"
#include "stringpool.h"
#include "tree-inline.h"
#include "flags.h"
#include "hashtab.h"
#include "hash-set.h"
#include "vec.h"
#include "machmode.h"
#include "hard-reg-set.h"
#include "function.h"
#include "toplev.h"
#include "tm_p.h"
#include "cpplib.h"
#include "target.h"
#include "debug.h"
#include "opts.h"
#include "timevar.h"
#include "c-family/c-common.h"
#include "c-family/c-objc.h"
#include "c-family/c-pragma.h"
#include "c-family/c-ubsan.h"
#include "langhooks.h"
#include "tree-iterator.h"
#include "diagnostic-core.h"
#include "dumpfile.h"
#include "hash-map.h"
#include "is-a.h"
#include "plugin-api.h"
#include "ipa-ref.h"
#include "cgraph.h"
#include "hash-table.h"
#include "langhooks-def.h"
#include "plugin.h"
#include "c-family/c-ada-spec.h"
#include "cilk.h"
#include "builtins.h"
#include "c/c-lang.h"

#include "ldv-aspect-parser.h"
#include "ldv-aspect-types.h"
#include "ldv-core.h"
#include "ldv-cpp-converter.h"
#include "ldv-cpp-pointcut-matcher.h"
#include "ldv-io.h"


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

/* Flag says whether it's parsed primitive pointcut declaration signature or not. */
static bool ldv_isdecl_pps = false;
/* Flag says whether it's parsed primitive pointcut file signature or not. */
static bool ldv_isfile_pps = false;
/* Flag says whether it's parsed primitive pointcut macro signature or not. */
static bool ldv_ismacro_pps = false;
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


#line 213 "ldv-aspect-parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int ldv_yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LDV_FILE = 258,                /* LDV_FILE  */
    LDV_BODY = 259,                /* LDV_BODY  */
    LDV_ID = 260,                  /* LDV_ID  */
    LDV_INT_NUMB = 261,            /* LDV_INT_NUMB  */
    LDV_MACRO_POINTCUT = 262,      /* LDV_MACRO_POINTCUT  */
    LDV_FILE_POINTCUT = 263,       /* LDV_FILE_POINTCUT  */
    LDV_TYPEDEF = 264,             /* LDV_TYPEDEF  */
    LDV_EXTERN = 265,              /* LDV_EXTERN  */
    LDV_STATIC = 266,              /* LDV_STATIC  */
    LDV_AUTO = 267,                /* LDV_AUTO  */
    LDV_REGISTER = 268,            /* LDV_REGISTER  */
    LDV_VOID = 269,                /* LDV_VOID  */
    LDV_CHAR = 270,                /* LDV_CHAR  */
    LDV_INT = 271,                 /* LDV_INT  */
    LDV_FLOAT = 272,               /* LDV_FLOAT  */
    LDV_DOUBLE = 273,              /* LDV_DOUBLE  */
    LDV_BOOL = 274,                /* LDV_BOOL  */
    LDV_COMPLEX = 275,             /* LDV_COMPLEX  */
    LDV_SHORT = 276,               /* LDV_SHORT  */
    LDV_LONG = 277,                /* LDV_LONG  */
    LDV_SIGNED = 278,              /* LDV_SIGNED  */
    LDV_UNSIGNED = 279,            /* LDV_UNSIGNED  */
    LDV_STRUCT = 280,              /* LDV_STRUCT  */
    LDV_UNION = 281,               /* LDV_UNION  */
    LDV_ENUM = 282,                /* LDV_ENUM  */
    LDV_TYPEDEF_NAME = 283,        /* LDV_TYPEDEF_NAME  */
    LDV_UNIVERSAL_TYPE_SPECIFIER = 284, /* LDV_UNIVERSAL_TYPE_SPECIFIER  */
    LDV_CONST = 285,               /* LDV_CONST  */
    LDV_RESTRICT = 286,            /* LDV_RESTRICT  */
    LDV_VOLATILE = 287,            /* LDV_VOLATILE  */
    LDV_INLINE = 288,              /* LDV_INLINE  */
    LDV_AND = 289,                 /* "&&"  */
    LDV_ANY_PARAMS = 290,          /* ".."  */
    LDV_ELLIPSIS = 291,            /* "..."  */
    LDV_OR = 292                   /* "||"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 173 "ldv-aspect-parser.y"

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

#line 316 "ldv-aspect-parser.c"

};
typedef union YYSTYPE YYSTYPE;
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


/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LDV_FILE = 3,                   /* LDV_FILE  */
  YYSYMBOL_LDV_BODY = 4,                   /* LDV_BODY  */
  YYSYMBOL_LDV_ID = 5,                     /* LDV_ID  */
  YYSYMBOL_LDV_INT_NUMB = 6,               /* LDV_INT_NUMB  */
  YYSYMBOL_LDV_MACRO_POINTCUT = 7,         /* LDV_MACRO_POINTCUT  */
  YYSYMBOL_LDV_FILE_POINTCUT = 8,          /* LDV_FILE_POINTCUT  */
  YYSYMBOL_LDV_TYPEDEF = 9,                /* LDV_TYPEDEF  */
  YYSYMBOL_LDV_EXTERN = 10,                /* LDV_EXTERN  */
  YYSYMBOL_LDV_STATIC = 11,                /* LDV_STATIC  */
  YYSYMBOL_LDV_AUTO = 12,                  /* LDV_AUTO  */
  YYSYMBOL_LDV_REGISTER = 13,              /* LDV_REGISTER  */
  YYSYMBOL_LDV_VOID = 14,                  /* LDV_VOID  */
  YYSYMBOL_LDV_CHAR = 15,                  /* LDV_CHAR  */
  YYSYMBOL_LDV_INT = 16,                   /* LDV_INT  */
  YYSYMBOL_LDV_FLOAT = 17,                 /* LDV_FLOAT  */
  YYSYMBOL_LDV_DOUBLE = 18,                /* LDV_DOUBLE  */
  YYSYMBOL_LDV_BOOL = 19,                  /* LDV_BOOL  */
  YYSYMBOL_LDV_COMPLEX = 20,               /* LDV_COMPLEX  */
  YYSYMBOL_LDV_SHORT = 21,                 /* LDV_SHORT  */
  YYSYMBOL_LDV_LONG = 22,                  /* LDV_LONG  */
  YYSYMBOL_LDV_SIGNED = 23,                /* LDV_SIGNED  */
  YYSYMBOL_LDV_UNSIGNED = 24,              /* LDV_UNSIGNED  */
  YYSYMBOL_LDV_STRUCT = 25,                /* LDV_STRUCT  */
  YYSYMBOL_LDV_UNION = 26,                 /* LDV_UNION  */
  YYSYMBOL_LDV_ENUM = 27,                  /* LDV_ENUM  */
  YYSYMBOL_LDV_TYPEDEF_NAME = 28,          /* LDV_TYPEDEF_NAME  */
  YYSYMBOL_LDV_UNIVERSAL_TYPE_SPECIFIER = 29, /* LDV_UNIVERSAL_TYPE_SPECIFIER  */
  YYSYMBOL_LDV_CONST = 30,                 /* LDV_CONST  */
  YYSYMBOL_LDV_RESTRICT = 31,              /* LDV_RESTRICT  */
  YYSYMBOL_LDV_VOLATILE = 32,              /* LDV_VOLATILE  */
  YYSYMBOL_LDV_INLINE = 33,                /* LDV_INLINE  */
  YYSYMBOL_LDV_AND = 34,                   /* "&&"  */
  YYSYMBOL_LDV_ANY_PARAMS = 35,            /* ".."  */
  YYSYMBOL_LDV_ELLIPSIS = 36,              /* "..."  */
  YYSYMBOL_LDV_OR = 37,                    /* "||"  */
  YYSYMBOL_38_ = 38,                       /* '!'  */
  YYSYMBOL_39_ = 39,                       /* ':'  */
  YYSYMBOL_40_ = 40,                       /* '('  */
  YYSYMBOL_41_ = 41,                       /* ')'  */
  YYSYMBOL_42_ = 42,                       /* ','  */
  YYSYMBOL_43_ = 43,                       /* '['  */
  YYSYMBOL_44_ = 44,                       /* ']'  */
  YYSYMBOL_45_ = 45,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 46,                  /* $accept  */
  YYSYMBOL_input = 47,                     /* input  */
  YYSYMBOL_named_pointcut = 48,            /* named_pointcut  */
  YYSYMBOL_advice_definition = 49,         /* advice_definition  */
  YYSYMBOL_advice_declaration = 50,        /* advice_declaration  */
  YYSYMBOL_composite_pointcut = 51,        /* composite_pointcut  */
  YYSYMBOL_primitive_pointcut = 52,        /* primitive_pointcut  */
  YYSYMBOL_53_1 = 53,                      /* $@1  */
  YYSYMBOL_54_2 = 54,                      /* $@2  */
  YYSYMBOL_55_3 = 55,                      /* $@3  */
  YYSYMBOL_56_4 = 56,                      /* $@4  */
  YYSYMBOL_57_5 = 57,                      /* $@5  */
  YYSYMBOL_58_6 = 58,                      /* $@6  */
  YYSYMBOL_primitive_pointcut_signature_macro = 59, /* primitive_pointcut_signature_macro  */
  YYSYMBOL_primitive_pointcut_signature_file = 60, /* primitive_pointcut_signature_file  */
  YYSYMBOL_macro_param_opt = 61,           /* macro_param_opt  */
  YYSYMBOL_macro_param = 62,               /* macro_param  */
  YYSYMBOL_primitive_pointcut_signature_declaration = 63, /* primitive_pointcut_signature_declaration  */
  YYSYMBOL_c_declaration = 64,             /* c_declaration  */
  YYSYMBOL_c_declaration_specifiers = 65,  /* c_declaration_specifiers  */
  YYSYMBOL_66_7 = 66,                      /* $@7  */
  YYSYMBOL_c_declaration_specifiers_aux = 67, /* c_declaration_specifiers_aux  */
  YYSYMBOL_c_declaration_specifiers_opt = 68, /* c_declaration_specifiers_opt  */
  YYSYMBOL_c_storage_class_specifier = 69, /* c_storage_class_specifier  */
  YYSYMBOL_c_type_specifier = 70,          /* c_type_specifier  */
  YYSYMBOL_c_type_qualifier = 71,          /* c_type_qualifier  */
  YYSYMBOL_c_function_specifier = 72,      /* c_function_specifier  */
  YYSYMBOL_c_declarator = 73,              /* c_declarator  */
  YYSYMBOL_c_direct_declarator = 74,       /* c_direct_declarator  */
  YYSYMBOL_c_pointer_opt = 75,             /* c_pointer_opt  */
  YYSYMBOL_c_pointer = 76,                 /* c_pointer  */
  YYSYMBOL_int_opt = 77,                   /* int_opt  */
  YYSYMBOL_c_type_qualifier_list_opt = 78, /* c_type_qualifier_list_opt  */
  YYSYMBOL_c_type_qualifier_list = 79,     /* c_type_qualifier_list  */
  YYSYMBOL_c_parameter_type_list = 80,     /* c_parameter_type_list  */
  YYSYMBOL_c_parameter_list = 81,          /* c_parameter_list  */
  YYSYMBOL_c_parameter_declaration = 82,   /* c_parameter_declaration  */
  YYSYMBOL_c_abstract_declarator = 83,     /* c_abstract_declarator  */
  YYSYMBOL_c_abstract_declarator_opt = 84, /* c_abstract_declarator_opt  */
  YYSYMBOL_c_direct_abstract_declarator = 85, /* c_direct_abstract_declarator  */
  YYSYMBOL_c_direct_abstract_declarator_opt = 86, /* c_direct_abstract_declarator_opt  */
  YYSYMBOL_c_parameter_type_list_opt = 87  /* c_parameter_type_list_opt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
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
#define YYLAST   118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  104
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  142

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
static const yytype_int16 yyrline[] =
{
       0,   294,   294,   296,   301,   321,   364,   385,   435,   476,
     515,   535,   560,   589,   597,   597,   597,   634,   634,   634,
     671,   671,   671,   747,   766,   787,   803,   806,   812,   823,
     833,   844,   855,   867,   877,   891,   897,   916,   983,   983,
     989,  1007,  1024,  1041,  1058,  1066,  1077,  1080,  1086,  1098,
    1110,  1122,  1134,  1148,  1160,  1172,  1184,  1196,  1208,  1220,
    1232,  1249,  1266,  1283,  1300,  1313,  1326,  1339,  1352,  1366,
    1378,  1390,  1404,  1418,  1426,  1443,  1449,  1477,  1496,  1499,
    1505,  1522,  1541,  1544,  1549,  1561,  1564,  1570,  1574,  1593,
    1616,  1632,  1661,  1680,  1734,  1740,  1749,  1758,  1764,  1770,
    1796,  1815,  1818,  1825,  1828
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "LDV_FILE", "LDV_BODY",
  "LDV_ID", "LDV_INT_NUMB", "LDV_MACRO_POINTCUT", "LDV_FILE_POINTCUT",
  "LDV_TYPEDEF", "LDV_EXTERN", "LDV_STATIC", "LDV_AUTO", "LDV_REGISTER",
  "LDV_VOID", "LDV_CHAR", "LDV_INT", "LDV_FLOAT", "LDV_DOUBLE", "LDV_BOOL",
  "LDV_COMPLEX", "LDV_SHORT", "LDV_LONG", "LDV_SIGNED", "LDV_UNSIGNED",
  "LDV_STRUCT", "LDV_UNION", "LDV_ENUM", "LDV_TYPEDEF_NAME",
  "LDV_UNIVERSAL_TYPE_SPECIFIER", "LDV_CONST", "LDV_RESTRICT",
  "LDV_VOLATILE", "LDV_INLINE", "\"&&\"", "\"..\"", "\"...\"", "\"||\"",
  "'!'", "':'", "'('", "')'", "','", "'['", "']'", "'*'", "$accept",
  "input", "named_pointcut", "advice_definition", "advice_declaration",
  "composite_pointcut", "primitive_pointcut", "$@1", "$@2", "$@3", "$@4",
  "$@5", "$@6", "primitive_pointcut_signature_macro",
  "primitive_pointcut_signature_file", "macro_param_opt", "macro_param",
  "primitive_pointcut_signature_declaration", "c_declaration",
  "c_declaration_specifiers", "$@7", "c_declaration_specifiers_aux",
  "c_declaration_specifiers_opt", "c_storage_class_specifier",
  "c_type_specifier", "c_type_qualifier", "c_function_specifier",
  "c_declarator", "c_direct_declarator", "c_pointer_opt", "c_pointer",
  "int_opt", "c_type_qualifier_list_opt", "c_type_qualifier_list",
  "c_parameter_type_list", "c_parameter_list", "c_parameter_declaration",
  "c_abstract_declarator", "c_abstract_declarator_opt",
  "c_direct_abstract_declarator", "c_direct_abstract_declarator_opt",
  "c_parameter_type_list_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    33,    58,
      40,    41,    44,    91,    93,    42
};
#endif

#define YYPACT_NINF (-104)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-104)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
    -104,    11,  -104,     4,  -104,  -104,    29,     7,    -2,  -104,
      -2,    34,    39,    45,    -2,    -2,    -3,  -104,    -3,  -104,
    -104,  -104,  -104,   -22,    -2,    -2,  -104,    32,    44,  -104,
    -104,    46,  -104,  -104,   -16,    40,    51,  -104,  -104,  -104,
      53,    52,  -104,    -1,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,    87,    88,    90,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,    40,    40,    40,    40,     5,    55,    56,
    -104,  -104,    54,    52,  -104,    54,    -8,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,    62,  -104,  -104,    59,    60,
    -104,  -104,  -104,  -104,    63,  -104,    17,  -104,  -104,     9,
    -104,   -21,    64,    61,  -104,  -104,  -104,    57,    70,  -104,
    -104,     2,    48,  -104,  -104,  -104,  -104,  -104,  -104,    54,
      38,    65,  -104,    66,    68,    17,  -104,  -104,    69,    67,
    -104,  -104
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     4,     3,     0,     0,     0,     6,
       0,     8,     0,     0,     0,     0,     7,     9,     5,    20,
      14,    17,    10,     0,     0,     0,    38,     0,     0,    13,
      12,    11,    21,    35,    78,     0,    23,    15,    25,    18,
       0,    85,    37,     0,    79,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,     0,     0,     0,    67,    68,    69,    70,    71,    72,
      44,    45,    39,    46,    46,    46,    46,    26,     0,     0,
      22,    87,    80,    86,    74,    78,    73,    64,    65,    66,
      47,    40,    41,    42,    43,    28,    30,    31,     0,    27,
      16,    19,    81,    88,     0,    38,    82,    32,    24,     0,
      75,    78,     0,    89,    90,    84,    83,     0,    29,    33,
      92,   101,    79,    97,    93,    77,    38,    76,    34,    78,
      95,     0,    91,     0,    38,    82,    98,   104,     0,     0,
     100,    99
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -104,  -104,  -104,  -104,  -104,     3,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,    86,
    -104,    78,    12,  -104,  -104,   -39,  -104,   -34,  -104,  -103,
     -81,   -20,  -104,  -104,   -18,  -104,   -12,   -11,  -104,  -104,
    -104,  -104
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     4,     5,     6,    16,    17,    27,    78,    28,
      79,    26,    40,    37,    39,    98,    99,    32,    33,   111,
      35,    90,    91,    73,    74,    75,    76,   104,    86,    43,
      44,   117,    82,    83,   112,   113,   114,   123,   124,   130,
     131,   138
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   102,    81,    11,    84,    12,    13,    84,   121,     7,
      95,     2,    24,    18,   118,    25,     3,    22,    23,    29,
     -96,   -96,   115,   116,    41,   -36,   121,    30,    31,    41,
     122,    24,   105,     9,    25,   106,    14,    36,    15,    85,
      96,    97,   129,     8,   103,   119,    10,    38,   122,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    19,    70,    71,   120,   134,    20,
      24,  -102,    66,    67,    68,    21,    92,    93,    94,   -94,
     -94,    77,    87,    88,    80,    89,   100,   101,   107,    41,
     108,   127,   109,   126,   110,   125,   128,   136,   135,  -103,
     140,   141,    34,    72,   132,   139,   137,     0,   133
};

static const yytype_int16 yycheck[] =
{
      34,    82,    41,     5,     5,     7,     8,     5,   111,     5,
       5,     0,    34,    10,     5,    37,     5,    14,    15,    41,
      41,    42,     5,     6,    45,    41,   129,    24,    25,    45,
     111,    34,    40,     4,    37,    43,    38,     5,    40,    40,
      35,    36,    40,    39,    83,    36,    39,     3,   129,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    40,    35,    36,   111,    40,    40,
      34,    43,    30,    31,    32,    40,    74,    75,    76,    41,
      42,    40,     5,     5,    41,     5,    41,    41,    36,    45,
      41,    44,    42,    42,    41,    41,    36,    41,    43,    41,
      41,    44,    26,    35,   126,   135,   134,    -1,   129
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    47,     0,     5,    48,    49,    50,     5,    39,     4,
      39,     5,     7,     8,    38,    40,    51,    52,    51,    40,
      40,    40,    51,    51,    34,    37,    57,    53,    55,    41,
      51,    51,    63,    64,    65,    66,     5,    59,     3,    60,
      58,    45,    73,    75,    76,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      35,    36,    67,    69,    70,    71,    72,    40,    54,    56,
      41,    71,    78,    79,     5,    40,    74,     5,     5,     5,
      67,    68,    68,    68,    68,     5,    35,    36,    61,    62,
      41,    41,    76,    71,    73,    40,    43,    36,    41,    42,
      41,    65,    80,    81,    82,     5,     6,    77,     5,    36,
      73,    75,    76,    83,    84,    41,    42,    44,    36,    40,
      85,    86,    82,    83,    40,    43,    41,    80,    87,    77,
      41,    44
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    47,    47,    48,    49,    50,    51,    51,
      51,    51,    51,    51,    53,    54,    52,    55,    56,    52,
      57,    58,    52,    59,    59,    60,    61,    61,    62,    62,
      62,    62,    62,    62,    62,    63,    64,    64,    66,    65,
      67,    67,    67,    67,    67,    67,    68,    68,    69,    69,
      69,    69,    69,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    71,
      71,    71,    72,    73,    74,    74,    74,    74,    75,    75,
      76,    76,    77,    77,    77,    78,    78,    79,    79,    80,
      81,    81,    82,    82,    83,    83,    84,    84,    85,    85,
      85,    86,    86,    87,    87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     4,     2,     3,     1,     1,
       2,     3,     3,     3,     0,     0,     6,     0,     0,     6,
       0,     0,     6,     1,     4,     1,     0,     1,     1,     3,
       1,     1,     2,     3,     4,     1,     1,     2,     0,     2,
       2,     2,     2,     2,     1,     1,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     4,     4,     0,     1,
       2,     3,     0,     1,     1,     0,     1,     1,     2,     1,
       1,     3,     2,     2,     1,     2,     0,     1,     3,     4,
       4,     0,     1,     0,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

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

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
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

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 280 "ldv-aspect-parser.y"
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

#line 1409 "ldv-aspect-parser.c"

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
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

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* input: input advice_definition  */
#line 297 "ldv-aspect-parser.y"
    {
      /* Add advice definition from corresponding rule to the advice definitions list. */
      ldv_list_push_back (&ldv_adef_list, (yyvsp[0].a_definition));
    }
#line 1624 "ldv-aspect-parser.c"
    break;

  case 4: /* input: input named_pointcut  */
#line 302 "ldv-aspect-parser.y"
    {
      ldv_np_ptr n_pointcut = NULL;
      ldv_list_ptr n_pointcut_list = NULL;

      /* Walk through a named pointcuts list to find name collisions. */
      for (n_pointcut_list = ldv_n_pointcut_list; n_pointcut_list ; n_pointcut_list = ldv_list_get_next (n_pointcut_list))
        {
          n_pointcut = (ldv_np_ptr) ldv_list_get_data (n_pointcut_list);

          if (!strcmp(n_pointcut->p_name, (yyvsp[0].n_pointcut)->p_name))
            {
              internal_error ("duplicate pointcut name \"%s\" was used", (yyvsp[0].n_pointcut)->p_name);
            }
        }

      ldv_list_push_back (&ldv_n_pointcut_list, (yyvsp[0].n_pointcut));
    }
#line 1646 "ldv-aspect-parser.c"
    break;

  case 5: /* named_pointcut: LDV_ID LDV_ID ':' composite_pointcut  */
#line 322 "ldv-aspect-parser.y"
    {
      ldv_np_ptr n_pointcut_new = NULL;
      char *p_keyword = NULL;
      char *p_name = NULL;

      /* Set pointcut keyword from lexer identifier. */
      p_keyword = ldv_get_id_name ((yyvsp[-3].id));

      if (strcmp ("pointcut", p_keyword))
        {
          ldv_print_info_location ((yylsp[-3]), LDV_ERROR_BISON, "incorrect keyword \"%s\" was used", p_keyword);
          internal_error ("use one of the following keywords: \"pointcut\", \"before\", \"after\", \"around\", \"new\"");
        }

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-3].id));

      n_pointcut_new = XCNEW (ldv_named_pointcut);

      /* Set a pointcut name from a lexer identifier. This name can't contain
         any '$' wildcards. */
      p_name = ldv_copy_str (ldv_get_id_name ((yyvsp[-2].id)));

      if ((yyvsp[-2].id)->isany_chars)
        {
          ldv_print_info_location ((yylsp[-3]), LDV_ERROR_BISON, "'$' wildcard was used in pointcut name \"%s\"", p_name);
          internal_error ("pointcut name should be a correct identifier");
        }

      ldv_free_id ((yyvsp[-2].id));

      n_pointcut_new->p_name = p_name;

      /* Set a composite pointcut of a named pointcut from a corresponding rule. */
      n_pointcut_new->c_pointcut = (yyvsp[0].c_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed named pointcut");

      (yyval.n_pointcut) = n_pointcut_new;
    }
#line 1691 "ldv-aspect-parser.c"
    break;

  case 6: /* advice_definition: advice_declaration LDV_BODY  */
#line 365 "ldv-aspect-parser.y"
    {
      ldv_adef_ptr a_definition_new = NULL;

      a_definition_new = XCNEW (ldv_advice_definition);

      /* Set an advice definition declaration from a corresponding rule. */
      a_definition_new->a_declaration = (yyvsp[-1].a_declaration);

      /* Set and advice definition body from a lexer body. */
      a_definition_new->a_body = (yyvsp[0].body);

      /* Set that an advice wasn't used at the beginning. */
      a_definition_new->use_counter = 0;

      ldv_print_info (LDV_INFO_BISON, "bison parsed advice definition");

      (yyval.a_definition) = a_definition_new;
    }
#line 1714 "ldv-aspect-parser.c"
    break;

  case 7: /* advice_declaration: LDV_ID ':' composite_pointcut  */
#line 386 "ldv-aspect-parser.y"
    {
      char *a_kind = NULL;
      ldv_adecl_ptr a_declaration = NULL;

      /* Set an advice declaration kind from a lexer identifier. */
      a_kind = ldv_get_id_name ((yyvsp[-2].id));

      a_declaration = XCNEW (ldv_advice_declaration);

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
          internal_error ("use \"after\", \"around\", \"before\", \"new\", \"info\" advice declaration kind");
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
#line 1765 "ldv-aspect-parser.c"
    break;

  case 8: /* composite_pointcut: LDV_ID  */
#line 436 "ldv-aspect-parser.y"
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
          internal_error ("pointcut name should be a correct identifier");
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
        internal_error ("undefined pointcut with name \"%s\" was used", p_name);

      ldv_free_id ((yyvsp[0].id));
    }
#line 1810 "ldv-aspect-parser.c"
    break;

  case 9: /* composite_pointcut: primitive_pointcut  */
#line 477 "ldv-aspect-parser.y"
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
#line 1853 "ldv-aspect-parser.c"
    break;

  case 10: /* composite_pointcut: '!' composite_pointcut  */
#line 516 "ldv-aspect-parser.y"
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
#line 1877 "ldv-aspect-parser.c"
    break;

  case 11: /* composite_pointcut: composite_pointcut "||" composite_pointcut  */
#line 536 "ldv-aspect-parser.y"
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
#line 1906 "ldv-aspect-parser.c"
    break;

  case 12: /* composite_pointcut: composite_pointcut "&&" composite_pointcut  */
#line 561 "ldv-aspect-parser.y"
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
#line 1939 "ldv-aspect-parser.c"
    break;

  case 13: /* composite_pointcut: '(' composite_pointcut ')'  */
#line 590 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed composite pointcut of associativity");

      (yyval.c_pointcut) = (yyvsp[-1].c_pointcut);
    }
#line 1949 "ldv-aspect-parser.c"
    break;

  case 14: /* $@1: %empty  */
#line 597 "ldv-aspect-parser.y"
                         { ldv_ismacro_pps = true; }
#line 1955 "ldv-aspect-parser.c"
    break;

  case 15: /* $@2: %empty  */
#line 597 "ldv-aspect-parser.y"
                                                                                        { ldv_ismacro_pps = false; }
#line 1961 "ldv-aspect-parser.c"
    break;

  case 16: /* primitive_pointcut: LDV_MACRO_POINTCUT '(' $@1 primitive_pointcut_signature_macro $@2 ')'  */
#line 598 "ldv-aspect-parser.y"
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;
      ldv_pps_ptr pp_signature = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ((yyvsp[-5].id));

      p_pointcut = XCNEW (ldv_primitive_pointcut);

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("define", pp_kind))
        p_pointcut->pp_kind = LDV_PP_DEFINE;
      else if (!strcmp ("expand", pp_kind))
        p_pointcut->pp_kind = LDV_PP_EXPAND;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a macro signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_DEFINE;

      /* Specify a macro signature. */
      pp_signature->pps_macro = (yyvsp[-2].pps_macro);

      /* Set a primitive pointcut signature. */
      p_pointcut->pp_signature = pp_signature;

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-5].id));

      (yyval.p_pointcut) = p_pointcut;
    }
#line 2002 "ldv-aspect-parser.c"
    break;

  case 17: /* $@3: %empty  */
#line 634 "ldv-aspect-parser.y"
                          { ldv_isfile_pps = true; }
#line 2008 "ldv-aspect-parser.c"
    break;

  case 18: /* $@4: %empty  */
#line 634 "ldv-aspect-parser.y"
                                                                                       { ldv_isfile_pps = false; }
#line 2014 "ldv-aspect-parser.c"
    break;

  case 19: /* primitive_pointcut: LDV_FILE_POINTCUT '(' $@3 primitive_pointcut_signature_file $@4 ')'  */
#line 635 "ldv-aspect-parser.y"
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;
      ldv_pps_ptr pp_signature = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ((yyvsp[-5].id));

      p_pointcut = XCNEW (ldv_primitive_pointcut);

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("file", pp_kind))
        p_pointcut->pp_kind = LDV_PP_FILE;
      else if (!strcmp ("infile", pp_kind))
        p_pointcut->pp_kind = LDV_PP_INFILE;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a file signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_FILE;

      /* Specify a file signature. */
      pp_signature->pps_file = (yyvsp[-2].pps_file);

      /* Set a primitive pointcut signature. */
      p_pointcut->pp_signature = pp_signature;

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-5].id));

      (yyval.p_pointcut) = p_pointcut;
    }
#line 2055 "ldv-aspect-parser.c"
    break;

  case 20: /* $@5: %empty  */
#line 671 "ldv-aspect-parser.y"
               { ldv_isdecl_pps = true; }
#line 2061 "ldv-aspect-parser.c"
    break;

  case 21: /* $@6: %empty  */
#line 671 "ldv-aspect-parser.y"
                                                                                   { ldv_isdecl_pps = false; }
#line 2067 "ldv-aspect-parser.c"
    break;

  case 22: /* primitive_pointcut: LDV_ID '(' $@5 primitive_pointcut_signature_declaration $@6 ')'  */
#line 672 "ldv-aspect-parser.y"
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;
      ldv_pps_ptr pp_signature = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ((yyvsp[-5].id));

      p_pointcut = XCNEW (ldv_primitive_pointcut);

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("call", pp_kind))
        p_pointcut->pp_kind = LDV_PP_CALL;
      else if (!strcmp ("callp", pp_kind))
        p_pointcut->pp_kind = LDV_PP_CALLP;
      else if (!strcmp ("use_func", pp_kind))
        p_pointcut->pp_kind = LDV_PP_USE_FUNC;
      else if (!strcmp ("use_var", pp_kind))
        p_pointcut->pp_kind = LDV_PP_USE_VAR;
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
          ldv_print_info_location ((yylsp[-5]), LDV_ERROR_BISON, "incorrect primitive pointcut kind \"%s\" was used", pp_kind);
          internal_error ("use \"call\", \"declare_func\", \"define\", \"execution\", \"expand\", \"file\", \"get\", \"get_global\", \"get_local\", \"incall\", \"infile\", \"infunc\", \"init\", \"init_global\", \"init_local\", \"introduce\", \"set\", \"set_global\", \"set_local\" primitive pointcut kind");
        }

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a declaration is parsed. */
      pp_signature->pps_kind = LDV_PPS_DECL;

      /* Specify a file signature. */
      pp_signature->pps_declaration = (yyvsp[-2].pps_decl);

      /* Set a primitive pointcut signature. */
      p_pointcut->pp_signature = pp_signature;

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ((yyvsp[-5].id));

      (yyval.p_pointcut) = p_pointcut;
    }
#line 2145 "ldv-aspect-parser.c"
    break;

  case 23: /* primitive_pointcut_signature_macro: LDV_ID  */
#line 748 "ldv-aspect-parser.y"
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);\

      /* Specify that macro definition signature is parsed. */
      macro->pps_macro_kind = LDV_PPS_MACRO_DEF;

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = (yyvsp[0].id);

      /* Specify that there is no macro parameters at all. */
      macro->macro_param_list = NULL;

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro definition signature");

      (yyval.pps_macro) = macro;
    }
#line 2168 "ldv-aspect-parser.c"
    break;

  case 24: /* primitive_pointcut_signature_macro: LDV_ID '(' macro_param_opt ')'  */
#line 767 "ldv-aspect-parser.y"
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);\

      /* Specify that macro function signature is parsed. */
      macro->pps_macro_kind = LDV_PPS_MACRO_FUNC;

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = (yyvsp[-3].id);

      /* Set a macro function parameters from a corresponding rule. */
      macro->macro_param_list = (yyvsp[-1].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro function signature");

      (yyval.pps_macro) = macro;
    }
#line 2191 "ldv-aspect-parser.c"
    break;

  case 25: /* primitive_pointcut_signature_file: LDV_FILE  */
#line 788 "ldv-aspect-parser.y"
    {
      ldv_pps_file_ptr file = NULL;

      file = XCNEW (ldv_primitive_pointcut_signature_file);

      /* Set a function name from a lexer identifier. */
      file->file_name = (yyvsp[0].file);

      ldv_print_info (LDV_INFO_BISON, "bison parsed file signature");

      (yyval.pps_file) = file;
    }
#line 2208 "ldv-aspect-parser.c"
    break;

  case 26: /* macro_param_opt: %empty  */
#line 803 "ldv-aspect-parser.y"
    {
      (yyval.list) = NULL;
    }
#line 2216 "ldv-aspect-parser.c"
    break;

  case 27: /* macro_param_opt: macro_param  */
#line 807 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 2224 "ldv-aspect-parser.c"
    break;

  case 28: /* macro_param: LDV_ID  */
#line 813 "ldv-aspect-parser.y"
    {
      ldv_list_ptr macro_param_list = NULL;
      ldv_pps_macro_func_param_ptr pps_macro_func_param = NULL;

      pps_macro_func_param = ldv_create_macro_func_param ();
      pps_macro_func_param->name = (yyvsp[0].id);
      ldv_list_push_back (&macro_param_list, pps_macro_func_param);

      (yyval.list) = macro_param_list;
    }
#line 2239 "ldv-aspect-parser.c"
    break;

  case 29: /* macro_param: macro_param ',' LDV_ID  */
#line 824 "ldv-aspect-parser.y"
    {
      ldv_pps_macro_func_param_ptr pps_macro_func_param = NULL;

      pps_macro_func_param = ldv_create_macro_func_param ();
      pps_macro_func_param->name = (yyvsp[0].id);
      ldv_list_push_back (&(yyvsp[-2].list), pps_macro_func_param);

      (yyval.list) = (yyvsp[-2].list);
    }
#line 2253 "ldv-aspect-parser.c"
    break;

  case 30: /* macro_param: ".."  */
#line 834 "ldv-aspect-parser.y"
    {
      ldv_list_ptr macro_param_list = NULL;
      ldv_pps_macro_func_param_ptr pps_macro_func_param = NULL;

      pps_macro_func_param = ldv_create_macro_func_param ();
      pps_macro_func_param->isany_params = true;
      ldv_list_push_back (&macro_param_list, pps_macro_func_param);

      (yyval.list) = macro_param_list;
    }
#line 2268 "ldv-aspect-parser.c"
    break;

  case 31: /* macro_param: "..."  */
#line 845 "ldv-aspect-parser.y"
    {
      ldv_list_ptr macro_param_list = NULL;
      ldv_pps_macro_func_param_ptr pps_macro_func_param = NULL;

      pps_macro_func_param = ldv_create_macro_func_param ();
      pps_macro_func_param->isvar_params = true;
      ldv_list_push_back (&macro_param_list, pps_macro_func_param);

      (yyval.list) = macro_param_list;
    }
#line 2283 "ldv-aspect-parser.c"
    break;

  case 32: /* macro_param: LDV_ID "..."  */
#line 856 "ldv-aspect-parser.y"
    {
      ldv_list_ptr macro_param_list = NULL;
      ldv_pps_macro_func_param_ptr pps_macro_func_param = NULL;

      pps_macro_func_param = ldv_create_macro_func_param ();
      pps_macro_func_param->name = (yyvsp[-1].id);
      pps_macro_func_param->isvar_params = true;
      ldv_list_push_back (&macro_param_list, pps_macro_func_param);

      (yyval.list) = macro_param_list;
    }
#line 2299 "ldv-aspect-parser.c"
    break;

  case 33: /* macro_param: macro_param ',' "..."  */
#line 868 "ldv-aspect-parser.y"
    {
      ldv_pps_macro_func_param_ptr pps_macro_func_param = NULL;

      pps_macro_func_param = ldv_create_macro_func_param ();
      pps_macro_func_param->isvar_params = true;
      ldv_list_push_back (&(yyvsp[-2].list), pps_macro_func_param);

      (yyval.list) = (yyvsp[-2].list);
    }
#line 2313 "ldv-aspect-parser.c"
    break;

  case 34: /* macro_param: macro_param ',' LDV_ID "..."  */
#line 878 "ldv-aspect-parser.y"
    {
      ldv_pps_macro_func_param_ptr pps_macro_func_param = NULL;

      pps_macro_func_param = ldv_create_macro_func_param ();
      ldv_puts_id ("...", (yyvsp[-1].id));
      pps_macro_func_param->name = (yyvsp[-1].id);
      pps_macro_func_param->isvar_params = true;
      ldv_list_push_back (&(yyvsp[-3].list), pps_macro_func_param);

      (yyval.list) = (yyvsp[-3].list);
    }
#line 2329 "ldv-aspect-parser.c"
    break;

  case 35: /* primitive_pointcut_signature_declaration: c_declaration  */
#line 892 "ldv-aspect-parser.y"
    {
      (yyval.pps_decl) = (yyvsp[0].pps_decl);
    }
#line 2337 "ldv-aspect-parser.c"
    break;

  case 36: /* c_declaration: c_declaration_specifiers  */
#line 898 "ldv-aspect-parser.y"
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
#line 2360 "ldv-aspect-parser.c"
    break;

  case 37: /* c_declaration: c_declaration_specifiers c_declarator  */
#line 917 "ldv-aspect-parser.y"
    {
      ldv_pps_decl_ptr decl = NULL;
      ldv_pps_declarator_ptr declarator = NULL;
      ldv_list_ptr declarator_list = NULL;
      bool isdecl_kind_specified = false;

      decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers and a declarator chain. */
      decl->pps_declspecs = (yyvsp[-1].pps_declspecs);
      decl->pps_declarator = (yyvsp[0].list);

      /* Specify a kind of a declaration, a function, a variable or a type definition. */
      for (declarator_list = (yyvsp[0].list)
        ;
        ; declarator_list = ldv_list_get_next (declarator_list))
        {
          declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

          /* We deal with a variable declaration or a type definition in this case. */
          if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_ID)
            {
              if (decl->pps_declspecs->istypedef)
                {
                  decl->pps_declspecs->istypedef = false;
                  decl->pps_decl_kind = LDV_PPS_DECL_TYPE;
                  ldv_print_info (LDV_INFO_BISON, "bison parsed type definition signature pointcut");
                }
              else
                {
                  decl->pps_decl_kind = LDV_PPS_DECL_VAR;
                  ldv_print_info (LDV_INFO_BISON, "bison parsed variable declaration signature pointcut");
                }

              isdecl_kind_specified = true;

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
        internal_error ("declaration kind can't be determined");
    }
#line 2430 "ldv-aspect-parser.c"
    break;

  case 38: /* $@7: %empty  */
#line 983 "ldv-aspect-parser.y"
                          { ldv_istype_spec = false; ldv_isuniversal_type_spec = false; }
#line 2436 "ldv-aspect-parser.c"
    break;

  case 39: /* c_declaration_specifiers: $@7 c_declaration_specifiers_aux  */
#line 984 "ldv-aspect-parser.y"
  {
    (yyval.pps_declspecs) = (yyvsp[0].pps_declspecs);
  }
#line 2444 "ldv-aspect-parser.c"
    break;

  case 40: /* c_declaration_specifiers_aux: c_storage_class_specifier c_declaration_specifiers_opt  */
#line 990 "ldv-aspect-parser.y"
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
#line 2466 "ldv-aspect-parser.c"
    break;

  case 41: /* c_declaration_specifiers_aux: c_type_specifier c_declaration_specifiers_opt  */
#line 1008 "ldv-aspect-parser.y"
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
#line 2487 "ldv-aspect-parser.c"
    break;

  case 42: /* c_declaration_specifiers_aux: c_type_qualifier c_declaration_specifiers_opt  */
#line 1025 "ldv-aspect-parser.y"
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
#line 2508 "ldv-aspect-parser.c"
    break;

  case 43: /* c_declaration_specifiers_aux: c_function_specifier c_declaration_specifiers_opt  */
#line 1042 "ldv-aspect-parser.y"
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
#line 2529 "ldv-aspect-parser.c"
    break;

  case 44: /* c_declaration_specifiers_aux: ".."  */
#line 1059 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isany_params = true;

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2541 "ldv-aspect-parser.c"
    break;

  case 45: /* c_declaration_specifiers_aux: "..."  */
#line 1067 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvar_params = true;

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2553 "ldv-aspect-parser.c"
    break;

  case 46: /* c_declaration_specifiers_opt: %empty  */
#line 1077 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = NULL;
    }
#line 2561 "ldv-aspect-parser.c"
    break;

  case 47: /* c_declaration_specifiers_opt: c_declaration_specifiers_aux  */
#line 1081 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = (yyvsp[0].pps_declspecs);
    }
#line 2569 "ldv-aspect-parser.c"
    break;

  case 48: /* c_storage_class_specifier: LDV_TYPEDEF  */
#line 1087 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"typedef\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2585 "ldv-aspect-parser.c"
    break;

  case 49: /* c_storage_class_specifier: LDV_EXTERN  */
#line 1099 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isextern = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"extern\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2601 "ldv-aspect-parser.c"
    break;

  case 50: /* c_storage_class_specifier: LDV_STATIC  */
#line 1111 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstatic = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"static\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2617 "ldv-aspect-parser.c"
    break;

  case 51: /* c_storage_class_specifier: LDV_AUTO  */
#line 1123 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isauto = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"auto\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2633 "ldv-aspect-parser.c"
    break;

  case 52: /* c_storage_class_specifier: LDV_REGISTER  */
#line 1135 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isregister = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"register\" storage class specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2649 "ldv-aspect-parser.c"
    break;

  case 53: /* c_type_specifier: LDV_VOID  */
#line 1149 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvoid = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"void\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2665 "ldv-aspect-parser.c"
    break;

  case 54: /* c_type_specifier: LDV_CHAR  */
#line 1161 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->ischar = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"char\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2681 "ldv-aspect-parser.c"
    break;

  case 55: /* c_type_specifier: LDV_INT  */
#line 1173 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isint = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"int\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2697 "ldv-aspect-parser.c"
    break;

  case 56: /* c_type_specifier: LDV_FLOAT  */
#line 1185 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isfloat = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"float\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2713 "ldv-aspect-parser.c"
    break;

  case 57: /* c_type_specifier: LDV_DOUBLE  */
#line 1197 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isdouble = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"double\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2729 "ldv-aspect-parser.c"
    break;

  case 58: /* c_type_specifier: LDV_BOOL  */
#line 1209 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isbool = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"bool\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2745 "ldv-aspect-parser.c"
    break;

  case 59: /* c_type_specifier: LDV_COMPLEX  */
#line 1221 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->iscomplex = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"complex\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2761 "ldv-aspect-parser.c"
    break;

  case 60: /* c_type_specifier: LDV_SHORT  */
#line 1233 "ldv-aspect-parser.y"
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
#line 2782 "ldv-aspect-parser.c"
    break;

  case 61: /* c_type_specifier: LDV_LONG  */
#line 1250 "ldv-aspect-parser.y"
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
#line 2803 "ldv-aspect-parser.c"
    break;

  case 62: /* c_type_specifier: LDV_SIGNED  */
#line 1267 "ldv-aspect-parser.y"
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
#line 2824 "ldv-aspect-parser.c"
    break;

  case 63: /* c_type_specifier: LDV_UNSIGNED  */
#line 1284 "ldv-aspect-parser.y"
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
#line 2845 "ldv-aspect-parser.c"
    break;

  case 64: /* c_type_specifier: LDV_STRUCT LDV_ID  */
#line 1301 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstruct = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"struct\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2862 "ldv-aspect-parser.c"
    break;

  case 65: /* c_type_specifier: LDV_UNION LDV_ID  */
#line 1314 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isunion = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"union\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2879 "ldv-aspect-parser.c"
    break;

  case 66: /* c_type_specifier: LDV_ENUM LDV_ID  */
#line 1327 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isenum = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"enum\" type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2896 "ldv-aspect-parser.c"
    break;

  case 67: /* c_type_specifier: LDV_TYPEDEF_NAME  */
#line 1340 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef_name = true;
      pps_declspecs->type_name = (yyvsp[0].id);

      ldv_print_info (LDV_INFO_BISON, "bison parsed typedef name \"%s\" type specifier", ldv_get_id_name ((yyvsp[0].id)));

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2913 "ldv-aspect-parser.c"
    break;

  case 68: /* c_type_specifier: LDV_UNIVERSAL_TYPE_SPECIFIER  */
#line 1353 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isuniversal_type_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed universal type specifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2929 "ldv-aspect-parser.c"
    break;

  case 69: /* c_type_qualifier: LDV_CONST  */
#line 1367 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isconst = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"const\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2945 "ldv-aspect-parser.c"
    break;

  case 70: /* c_type_qualifier: LDV_RESTRICT  */
#line 1379 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isrestrict = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"restrict\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2961 "ldv-aspect-parser.c"
    break;

  case 71: /* c_type_qualifier: LDV_VOLATILE  */
#line 1391 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvolatile = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"volatile\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2977 "ldv-aspect-parser.c"
    break;

  case 72: /* c_function_specifier: LDV_INLINE  */
#line 1405 "ldv-aspect-parser.y"
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isinline = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"inline\" type qualifier");

      (yyval.pps_declspecs) = pps_declspecs;
    }
#line 2993 "ldv-aspect-parser.c"
    break;

  case 73: /* c_declarator: c_pointer_opt c_direct_declarator  */
#line 1419 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed declarator");

      (yyval.list) = ldv_list_splice ((yyvsp[-1].list), (yyvsp[0].list));
    }
#line 3003 "ldv-aspect-parser.c"
    break;

  case 74: /* c_direct_declarator: LDV_ID  */
#line 1427 "ldv-aspect-parser.y"
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
#line 3024 "ldv-aspect-parser.c"
    break;

  case 75: /* c_direct_declarator: '(' c_declarator ')'  */
#line 1444 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct declarator of associativity");

      (yyval.list) = (yyvsp[-1].list);
    }
#line 3034 "ldv-aspect-parser.c"
    break;

  case 76: /* c_direct_declarator: c_direct_declarator '[' int_opt ']'  */
#line 1450 "ldv-aspect-parser.y"
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
#line 3066 "ldv-aspect-parser.c"
    break;

  case 77: /* c_direct_declarator: c_direct_declarator '(' c_parameter_type_list ')'  */
#line 1478 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[-3].list) = ldv_list_push_front ((yyvsp[-3].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = (yyvsp[-1].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct declarator");

      (yyval.list) = (yyvsp[-3].list);
    }
#line 3086 "ldv-aspect-parser.c"
    break;

  case 78: /* c_pointer_opt: %empty  */
#line 1496 "ldv-aspect-parser.y"
    {
      (yyval.list) = NULL;
    }
#line 3094 "ldv-aspect-parser.c"
    break;

  case 79: /* c_pointer_opt: c_pointer  */
#line 1500 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3102 "ldv-aspect-parser.c"
    break;

  case 80: /* c_pointer: '*' c_type_qualifier_list_opt  */
#line 1506 "ldv-aspect-parser.y"
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
#line 3123 "ldv-aspect-parser.c"
    break;

  case 81: /* c_pointer: '*' c_type_qualifier_list_opt c_pointer  */
#line 1523 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[0].list) = ldv_list_push_front ((yyvsp[0].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      declarator_new->pps_ptr_quals = ldv_set_ptr_quals ((yyvsp[-1].pps_declspecs));

      ldv_print_info (LDV_INFO_BISON, "bison parsed pointer declarator");

      (yyval.list) = (yyvsp[0].list);
    }
#line 3143 "ldv-aspect-parser.c"
    break;

  case 82: /* int_opt: %empty  */
#line 1541 "ldv-aspect-parser.y"
    {
      (yyval.integer) = NULL;
    }
#line 3151 "ldv-aspect-parser.c"
    break;

  case 83: /* int_opt: LDV_INT_NUMB  */
#line 1545 "ldv-aspect-parser.y"
    {
      (yyval.integer) = (yyvsp[0].integer);
    }
#line 3159 "ldv-aspect-parser.c"
    break;

  case 84: /* int_opt: LDV_ID  */
#line 1550 "ldv-aspect-parser.y"
    {
      ldv_int_ptr integer = NULL;

      integer = ldv_create_int ();
      integer->numb = -1;

      (yyval.integer) = integer;
    }
#line 3172 "ldv-aspect-parser.c"
    break;

  case 85: /* c_type_qualifier_list_opt: %empty  */
#line 1561 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = NULL;
    }
#line 3180 "ldv-aspect-parser.c"
    break;

  case 86: /* c_type_qualifier_list_opt: c_type_qualifier_list  */
#line 1565 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = (yyvsp[0].pps_declspecs);
    }
#line 3188 "ldv-aspect-parser.c"
    break;

  case 87: /* c_type_qualifier_list: c_type_qualifier  */
#line 1571 "ldv-aspect-parser.y"
    {
      (yyval.pps_declspecs) = (yyvsp[0].pps_declspecs);
    }
#line 3196 "ldv-aspect-parser.c"
    break;

  case 88: /* c_type_qualifier_list: c_type_qualifier_list c_type_qualifier  */
#line 1575 "ldv-aspect-parser.y"
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
#line 3217 "ldv-aspect-parser.c"
    break;

  case 89: /* c_parameter_type_list: c_parameter_list  */
#line 1594 "ldv-aspect-parser.y"
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
            internal_error ("Used '...' not at the end of parameter list");
        }

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter list");

      (yyval.list) = (yyvsp[0].list);
    }
#line 3242 "ldv-aspect-parser.c"
    break;

  case 90: /* c_parameter_list: c_parameter_declaration  */
#line 1617 "ldv-aspect-parser.y"
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
#line 3262 "ldv-aspect-parser.c"
    break;

  case 91: /* c_parameter_list: c_parameter_list ',' c_parameter_declaration  */
#line 1633 "ldv-aspect-parser.y"
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
#line 3293 "ldv-aspect-parser.c"
    break;

  case 92: /* c_parameter_declaration: c_declaration_specifiers c_declarator  */
#line 1662 "ldv-aspect-parser.y"
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
#line 3316 "ldv-aspect-parser.c"
    break;

  case 93: /* c_parameter_declaration: c_declaration_specifiers c_abstract_declarator_opt  */
#line 1681 "ldv-aspect-parser.y"
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
#line 3372 "ldv-aspect-parser.c"
    break;

  case 94: /* c_abstract_declarator: c_pointer  */
#line 1735 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      (yyval.list) = (yyvsp[0].list);
    }
#line 3382 "ldv-aspect-parser.c"
    break;

  case 95: /* c_abstract_declarator: c_pointer_opt c_direct_abstract_declarator  */
#line 1741 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      (yyval.list) = ldv_list_splice ((yyvsp[-1].list), (yyvsp[0].list));
    }
#line 3392 "ldv-aspect-parser.c"
    break;

  case 96: /* c_abstract_declarator_opt: %empty  */
#line 1749 "ldv-aspect-parser.y"
    {
      ldv_list_ptr declarator_list = NULL;

      declarator_list = ldv_list_push_front (NULL, ldv_create_declarator ());

      ldv_print_info (LDV_INFO_BISON, "bison parsed empty abstract declarator");

      (yyval.list) = declarator_list;
    }
#line 3406 "ldv-aspect-parser.c"
    break;

  case 97: /* c_abstract_declarator_opt: c_abstract_declarator  */
#line 1759 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3414 "ldv-aspect-parser.c"
    break;

  case 98: /* c_direct_abstract_declarator: '(' c_abstract_declarator ')'  */
#line 1765 "ldv-aspect-parser.y"
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct abstract declarator of associativity");

      (yyval.list) = (yyvsp[-1].list);
    }
#line 3424 "ldv-aspect-parser.c"
    break;

  case 99: /* c_direct_abstract_declarator: c_direct_abstract_declarator_opt '[' int_opt ']'  */
#line 1771 "ldv-aspect-parser.y"
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
#line 3453 "ldv-aspect-parser.c"
    break;

  case 100: /* c_direct_abstract_declarator: c_direct_abstract_declarator '(' c_parameter_type_list_opt ')'  */
#line 1797 "ldv-aspect-parser.y"
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      (yyvsp[-3].list) = ldv_list_push_front ((yyvsp[-3].list), declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = (yyvsp[-1].list);

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct abstract declarator");

      (yyval.list) = (yyvsp[-3].list);
    }
#line 3473 "ldv-aspect-parser.c"
    break;

  case 101: /* c_direct_abstract_declarator_opt: %empty  */
#line 1815 "ldv-aspect-parser.y"
    {
      (yyval.list) = NULL;
    }
#line 3481 "ldv-aspect-parser.c"
    break;

  case 102: /* c_direct_abstract_declarator_opt: c_direct_abstract_declarator  */
#line 1819 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3489 "ldv-aspect-parser.c"
    break;

  case 103: /* c_parameter_type_list_opt: %empty  */
#line 1825 "ldv-aspect-parser.y"
    {
      (yyval.list) = NULL;
    }
#line 3497 "ldv-aspect-parser.c"
    break;

  case 104: /* c_parameter_type_list_opt: c_parameter_type_list  */
#line 1829 "ldv-aspect-parser.y"
    {
      (yyval.list) = (yyvsp[0].list);
    }
#line 3505 "ldv-aspect-parser.c"
    break;


#line 3509 "ldv-aspect-parser.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1833 "ldv-aspect-parser.y"


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
       LDV_PPS_DECL - LDV_PP_USE_VAR
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
        || pp_kind == LDV_PP_USE_FUNC || pp_kind == LDV_PP_USE_VAR
        || pp_kind == LDV_PP_DECLARE_FUNC || pp_kind == LDV_PP_EXECUTION
        || pp_kind == LDV_PP_GET || pp_kind == LDV_PP_GET_GLOBAL || pp_kind == LDV_PP_GET_LOCAL
        || pp_kind == LDV_PP_INCALL || pp_kind == LDV_PP_INFUNC
        || pp_kind == LDV_PP_INIT || pp_kind == LDV_PP_INIT_GLOBAL
        || pp_kind == LDV_PP_INIT_LOCAL || pp_kind == LDV_PP_INTRODUCE
        || pp_kind == LDV_PP_SET || pp_kind == LDV_PP_SET_GLOBAL
        || pp_kind == LDV_PP_SET_LOCAL)
        return ;

      break;

    default:
      internal_error ("incorrect primitive pointcut signature kind \"%d\" is used", pps_kind);
    }

  internal_error ("incorrect primitive pointcut kind \"%d\" is used with primitive pointcut signature kind \"%d\"", pp_kind, pps_kind);
}

ldv_cp_ptr
ldv_create_c_pointcut (void)
{
  ldv_cp_ptr c_pointcut = NULL;

  c_pointcut = XCNEW (ldv_composite_pointcut);
  c_pointcut->cp_kind = LDV_CP_NONE;
  c_pointcut->cp_type = LDV_CP_TYPE_NONE;

  return c_pointcut;
}

ldv_pps_ptr
ldv_create_pp_signature (void)
{
  ldv_pps_ptr pp_signature = NULL;

  pp_signature = XCNEW (ldv_primitive_pointcut_signature);
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

  /* Map between declaration specifier keywords and tokens. */
  struct ldv_keyword_token ldv_decl_spec_keyword_token_map [] = {
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

  /* Special identifier processing when parsing non-primitive pointcut
     signatures. */
  if (!ldv_isdecl_pps && !ldv_isfile_pps && !ldv_ismacro_pps)
    {
      if (!strcmp (id, "expand") || !strcmp (id, "define"))
        return LDV_MACRO_POINTCUT;
      else if (!strcmp (id, "file") || !strcmp (id, "infile"))
        return LDV_FILE_POINTCUT;
    }

  if (ldv_isdecl_pps)
    {
      /* Always bind an identifier with C declaration specifier keyword if so
         when parsing declaration primitive pointcut signature. */
      for (i = 0; ldv_decl_spec_keyword_token_map[i].keyword; i++)
        {
          if (!strcmp (id, ldv_decl_spec_keyword_token_map[i].keyword))
            {
              ldv_print_info (LDV_INFO_LEX, "lex parsed keyword \"%s\"", id);

              /* Mark that some type specifier for given declaration
                 specifiers list was parsed. */
              if (i > 4 && i < 19)
                ldv_istype_spec = true;

              /* Corresponding identifier won't be used any more. */
              ldv_free_id (yylval.id);

              return ldv_decl_spec_keyword_token_map[i].token;
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

  ldv_print_info (LDV_INFO_LEX, "lex parsed identifier \"%s\"", id);
  return LDV_ID;
}

void
ldv_hash_add_name (char *name)
{
  void **hash_element;

  hash_element = htab_find_slot_with_hash (ldv_called_func_names, name, (*htab_hash_string) (name), INSERT);

  if (hash_element == NULL)
    internal_error ("Can't allocate memory");

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
            internal_error ("End of file is reached but advice body \"%s\" isn't completed", ldv_get_body_text (*body));

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
        internal_error ("aspect pattern hasn't a name!");

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
        internal_error ("aspect pattern \"%s\" requires corresponding argument number to be specified", pattern->name);

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
      ldv_print_info (LDV_INFO_LEX, "lex parsed aspect pattern parameter that is aspect pattern");

      /* Replace evaluated environment variables with corresponding strings. */
      if (pattern->value)
        {
          param->kind = LDV_ASPECT_PATTERN_STRING;
          param->string = pattern->value;
        }
      else
        {
          param->kind = LDV_ASPECT_PATTERN_ASPECT_PATTERN;
          param->aspect_pattern = pattern;
        }

      return param;
    }

  if (ldv_parse_unsigned_integer (&integer))
    {
      ldv_print_info (LDV_INFO_LEX, "lex parsed unsigned int aspect pattern parameter \"%u\"", integer);

      param->kind = LDV_ASPECT_PATTERN_INTEGER;
      param->integer = integer;

      return param;
    }

  if (ldv_parse_id (&str, &isany_chars, true))
    {
      ldv_print_info (LDV_INFO_LEX, "lex parsed string aspect pattern parameter \"%s\"", str);

      param->kind = LDV_ASPECT_PATTERN_STRING;
      param->string = str;

      return param;
    }

  if (ldv_parse_aspect_pattern_param_str (&str))
    {
      ldv_print_info (LDV_INFO_LEX, "lex parsed string aspect pattern parameter \"%s\"", str);

      param->kind = LDV_ASPECT_PATTERN_STRING;
      param->string = str;

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
                  internal_error ("Can't process escape sequence '\\%c'", c);
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

  /* Push back a first non-string character. */
  ldv_ungetc (c, LDV_ASPECT_STREAM);

  /* Don't assign any value to string passed through parameter. 0 bytes were
     read from stream. */
  return 0;
}

ldv_list_ptr
ldv_parse_aspect_pattern_params (void)
{
  int c;
  ldv_list_ptr params = NULL;
  ldv_aspect_pattern_param_ptr prev_param = NULL, param = NULL;
  ldv_str_ptr str = NULL;

  c = ldv_getc (LDV_ASPECT_STREAM);

  /* '<' begins an aspect pattern parameters list. */
  if (c == '<')
    {
      ldv_set_last_column (yylloc.last_column + 1);

      /* Aspect parameters can be separated by commas and spaces.
         We need to preserve commas to pass them as is to, say,
         fprintf. This will allow to concatenate several aspect
         parameters represented as strings.*/
      while (1)
        {
          if ((param = ldv_parse_aspect_pattern_param ()))
            {
              /* Merge strings located together. */
              if (prev_param
                && !prev_param->next_param
                && param->kind == LDV_ASPECT_PATTERN_STRING
                && prev_param->kind == LDV_ASPECT_PATTERN_STRING)
                {
                  str = ldv_create_string ();
                  ldv_puts_string (prev_param->string, str);
                  ldv_puts_string (param->string, str);
                  prev_param->string = ldv_get_str (str);
                  continue;
                }

              prev_param = param;
              ldv_list_push_back (&params, param);
              continue;
            }

          /* Next symbol may be:
               - ',' that means that there are some other aspect pattern
                 parameters,
               - meaningless space,
               - '>' that finishes parameters list.
          */
          ldv_set_last_column (yylloc.last_column + 1);
          switch (c = ldv_getc (LDV_ASPECT_STREAM))
            {
            case ',':
              prev_param->next_param = true;
              break;

            case ' ':
              break;

            case '>':
              return params;

            default:
              internal_error ("aspect pattern parameters list has incorrect format!");
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
          internal_error ("aspect pattern \"%s\" cannot be calculated without matching", pattern->name);
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
            internal_error ("end of file is reached, but comment isn't completed");

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
                }

              internal_error ("End of file is reached but C comment \"%s\" isn't completed", ldv_get_text (comment));
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
            internal_error ("file path isn't terminated with quote - it's terminated with the end of line");

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
                     if (c_next == '"')
                       break;

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

              /* Don't ungetc the end of line to avoid line enlarging. */
              if (c == '\n')
                break;
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
    internal_error ("don't use location tracking beyond lex or bison information");

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

  internal_error ("terminate work after syntax error happened");
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
