/* Copyright (C) 2009-2012
   Institute for System Programming, Russian Academy of Sciences (ISPRAS).

This file is part of C Instrumentation Framework.

C Instrumentation Framework is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

C Instrumentation Framework is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
C Instrumentation Framework.  If not, see <http://www.gnu.org/licenses/>.  */

/* To produce corresponding C file on the basis of this bison grammar file you
need to go to the sources root directory and execute there:
$ cd gcc
$ bison ldv-aspect-parser.y
*/

%{

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

/* The named pointcuts list. */
static ldv_list_ptr ldv_n_pointcut_list = NULL;
/* Flag says whether it's parsed macro signature or not. */
static bool ldv_ismacro;
/* Flag true if some type specifier was parsed and false otherwise.
 * It becomes false when declaration specifiers are parsed. */
static bool ldv_istype_spec = true;


static void ldv_check_pp_semantics (ldv_pp_ptr);
static ldv_cp_ptr ldv_create_c_pointcut (void);
static ldv_pps_ptr ldv_create_pp_signature (void);
static int ldv_get_id_kind (char *id);
static unsigned int ldv_parse_advice_body (ldv_ab_ptr *body);
static ldv_aspect_pattern_ptr ldv_parse_aspect_pattern (void);
static ldv_aspect_pattern_param_ptr ldv_parse_aspect_pattern_param (void);
static unsigned int ldv_parse_aspect_pattern_param_str (char **str);
static ldv_list_ptr ldv_parse_aspect_pattern_params (void);
static int ldv_parse_aspect_pattern_known_value (char const **str);
static char *ldv_parse_comments (void);
static unsigned int ldv_parse_file_name (char **file_name);
static unsigned int ldv_parse_id (char **id, bool *isany_chars, bool isaspect_pattern);
static int ldv_parse_preprocessor_directives (void);
static unsigned int ldv_parse_unsigned_integer (unsigned int *integer);
static void ldv_parse_whitespaces (void);
static void ldv_print_info_location (yyltype, const char *, const char *, ...) ATTRIBUTE_PRINTF_3;
static ldv_pps_ptr_quals_ptr ldv_set_ptr_quals (ldv_pps_declspecs_ptr);
static void yyerror (char const *, ...);
static int yylex (void);

%}

/* File name of generated parser. */
%output="ldv-aspect-parser.c"

/* Prefix for all external symbols needed to avoid overlap with gcc symbols. */
%name-prefix="ldv_yy"

/* Enable debugging. */
%debug

/* To enable generation of file containing extra debugging information. */
%verbose

/* All possible semantic values. */
%union
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

/* Terminal symbols semantic values. */
%token <file>       LDV_FILE
%token <body>       LDV_BODY
%token <id>         LDV_ID
%token <integer>    LDV_INT_NUMB

%token <id>         LDV_TYPEDEF
%token <id>         LDV_EXTERN
%token <id>         LDV_STATIC
%token <id>         LDV_AUTO
%token <id>         LDV_REGISTER

%token <id>         LDV_VOID
%token <id>         LDV_CHAR
%token <id>         LDV_INT
%token <id>         LDV_FLOAT
%token <id>         LDV_DOUBLE
%token <id>         LDV_BOOL
%token <id>         LDV_COMPLEX
%token <id>         LDV_SHORT
%token <id>         LDV_LONG
%token <id>         LDV_SIGNED
%token <id>         LDV_UNSIGNED
%token <id>         LDV_STRUCT
%token <id>         LDV_UNION
%token <id>         LDV_ENUM

%token <id>         LDV_TYPEDEF_NAME

%token <id>         LDV_CONST
%token <id>         LDV_RESTRICT
%token <id>         LDV_VOLATILE

%token <id>         LDV_INLINE

/* Multicharacter terminal symbols. */
%token LDV_AND          "&&"
%token LDV_ANY_PARAMS   ".."
%token LDV_ELLIPSIS     "..."
%token LDV_OR           "||"

/* Nonterminal symbols semantic values. */
%type <a_definition>    advice_definition
%type <n_pointcut>      named_pointcut
%type <a_declaration>   advice_declaration
%type <c_pointcut>      composite_pointcut
%type <p_pointcut>      primitive_pointcut
%type <pp_signature>    primitive_pointcut_signature
%type <pps_macro>       primitive_pointcut_signature_macro
%type <list>            macro_param
%type <list>            macro_param_opt
%type <pps_file>        primitive_pointcut_signature_file
%type <pps_decl>        primitive_pointcut_signature_declaration
%type <pps_decl>        c_declaration
%type <pps_declspecs>   c_declaration_specifiers
%type <pps_declspecs>   c_declaration_specifiers_opt
%type <pps_declspecs>   c_declaration_specifiers_aux
%type <pps_declspecs>   c_storage_class_specifier
%type <pps_declspecs>   c_type_specifier
%type <pps_declspecs>   c_type_qualifier
%type <pps_declspecs>   c_function_specifier
%type <list>            c_declarator
%type <list>            c_direct_declarator
%type <list>            c_pointer
%type <list>            c_pointer_opt
%type <integer>         int_opt
%type <pps_declspecs>   c_type_qualifier_list
%type <pps_declspecs>   c_type_qualifier_list_opt
%type <list>            c_parameter_type_list
%type <list>            c_parameter_type_list_opt
%type <list>            c_parameter_list
%type <pps_decl>        c_parameter_declaration
%type <list>            c_abstract_declarator
%type <list>            c_abstract_declarator_opt
%type <list>            c_direct_abstract_declarator
%type <list>            c_direct_abstract_declarator_opt

/* Boolean operator precedence. */
%left LDV_OR
%left LDV_AND
%left '!'

/* Make necessary initializations before parsing starts. */
%initial-action
{
  /* Initialize the beginning location and aspect file. */
  @$.first_line = @$.last_line = 1;
  ldv_set_first_line (@$.first_line);
  ldv_set_last_line (@$.last_line);
  @$.first_column = @$.last_column = 1;
  ldv_set_first_column (@$.first_column);
  ldv_set_last_column (@$.last_column);
  @$.file_name = ldv_aspect_fname;
  ldv_set_file_name (@$.file_name);
}

%%

input: /* It's the advice definitions and named pointcuts list. */
    /* There may be no advices at all. */
  | input advice_definition /* More one advice definition after readed input. */
    {
      /* Add advice definition from corresponding rule to the advice definitions list. */
      ldv_list_push_back (&ldv_adef_list, $2);
    }
  | input named_pointcut /* More one named pointcut after readed input. */
    {
      ldv_list_push_back (&ldv_n_pointcut_list, $2);
    };

named_pointcut: /* It's a named pointcut, the first of two input conceptions. */
  LDV_ID LDV_ID ':' composite_pointcut /* Named pointcut is in the form: "pointcut pointcut_name : composite_pointcut". */
    {
      ldv_np_ptr n_pointcut_new = NULL;
      char *p_keyword = NULL;
      char *p_name = NULL;

      /* Set pointcut keyword from lexer identifier. */
      p_keyword = ldv_get_id_name ($1);

      if (strcmp ("pointcut", p_keyword))
        {
          ldv_print_info_location (@1, LDV_ERROR_BISON, "incorrect keyword \"%s\" was used", p_keyword);
          LDV_FATAL_ERROR ("use one of the following keywords: \"pointcut\", \"before\", \"after\", \"around\", \"new\"");
        }

      /* Delete an unneeded identifier. */
      ldv_delete_id ($1);

      n_pointcut_new = XCNEW (ldv_named_pointcut);
      ldv_print_info (LDV_INFO_MEM, "named pointcut memory was released");

      /* Set a pointcut name from a lexer identifier. This name can't contain
         any '$' wildcards. */
      p_name = ldv_get_id_name ($2);

      if ($2->isany_chars)
        {
          ldv_print_info_location (@1, LDV_ERROR_BISON, "'$' wildcard was used in pointcut name \"%s\"", p_name);
          LDV_FATAL_ERROR ("pointcut name should be a correct identifier");
        }

      n_pointcut_new->p_name = p_name;

      /* Set a composite pointcut of a named pointcut from a corresponding rule. */
      n_pointcut_new->c_pointcut = $4;

      ldv_print_info (LDV_INFO_BISON, "bison parsed named pointcut");

      $$ = n_pointcut_new;
    };

advice_definition: /* It's an advice definition, the second of two input conceptions. */
  advice_declaration LDV_BODY  /* An advice definition is in the form: "advice_declaration advice_body". */
    {
      ldv_adef_ptr a_definition_new = NULL;

      a_definition_new = XCNEW (ldv_advice_definition);
      ldv_print_info (LDV_INFO_MEM, "advice definition memory was released");

      /* Set an advice definition declaration from a corresponding rule. */
      a_definition_new->a_declaration = $1;

      /* Set and advice definition body from a lexer body. */
      a_definition_new->a_body = $2;

      /* Set that an advice wasn't used at the beginning. */
      a_definition_new->use_counter = 0;

      ldv_print_info (LDV_INFO_BISON, "bison parsed advice definition");

      $$ = a_definition_new;
    };

advice_declaration: /* It's an advice declaration, the part of an advice definition. */
  LDV_ID ':' composite_pointcut /* An advice declaration is in the form: "advice_declaration_kind : composite_pointcut". */
    {
      char *a_kind = NULL;
      ldv_adecl_ptr a_declaration = NULL;

      /* Set an advice declaration kind from a lexer identifier. */
      a_kind = ldv_get_id_name ($1);

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
          ldv_print_info_location (@1, LDV_ERROR_BISON, "incorrect advice declaration kind \"%s\" was used", a_kind);
          LDV_FATAL_ERROR ("use \"after\", \"around\", \"before\", \"new\", \"info\" advice declaration kind");
        }

      /* Set a composite pointcut from a corresponding rule. */
      a_declaration->c_pointcut = $3;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" advice declaration", a_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ($1);

      $$ = a_declaration;
    };


composite_pointcut: /* It's a composite pointcut, the part of named pointcut, advice declaration or other composite pointcut. */
  LDV_ID /* A named pointcut is in the form: "pointcut_name". */
    {
      ldv_np_ptr n_pointcut = NULL;
      ldv_list_ptr n_pointcut_list = NULL;
      char *p_name = NULL;
      ldv_cp_ptr c_pointcut = NULL;

      /* Set a pointcut name from a lexer identifier. This name can't contain
         any '$' wildcards. */
      p_name = ldv_get_id_name ($1);

      if ($1->isany_chars)
        {
          ldv_print_info_location (@1, LDV_ERROR_BISON, "'$' wildcard was used in pointcut name \"%s\"", p_name);
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
        $$ = c_pointcut;
      else
        {
          LDV_FATAL_ERROR ("undefined pointcut with name \"%s\" was used", p_name);
        }
    }
  | primitive_pointcut /* The primitive poincut form is described below. */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_PRIMITIVE;

      /* Set a primitive pointcut from a corresponding rule. */
      c_pointcut->p_pointcut = $1;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"primitive\" composite pointcut");

      $$ = c_pointcut;
    }
  | '!' composite_pointcut /* Boolean not for some composite pointcut. */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_NOT;

      /* Set a composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = $2;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"not\" composite pointcut");

      $$ = c_pointcut;
    }
  | composite_pointcut LDV_OR composite_pointcut /* Boolean or for two composite pointcuts. */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_OR;

      /* Set a first composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = $1;
      /* Set a second composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_second = $3;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"or\" composite pointcut");

      $$ = c_pointcut;
    }
  | composite_pointcut LDV_AND composite_pointcut /* Boolean and for two composite pointcuts. */
    {
      ldv_cp_ptr c_pointcut = NULL;

      c_pointcut = ldv_create_c_pointcut ();

      /* Set a corresponding composite pointcut kind. */
      c_pointcut->cp_kind = LDV_CP_AND;

      /* Set a first composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_first = $1;
      /* Set a second composite pointcut from a corresponding rule. */
      c_pointcut->c_pointcut_second = $3;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"and\" composite pointcut");

      $$ = c_pointcut;
    }
  | '(' composite_pointcut ')' /* Priority brackets. */
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed composite pointcut of associativity");

      $$ = $2;
    };

primitive_pointcut: /* It's a primitive pointcut, the part of composite pointcut. */
  LDV_ID '(' primitive_pointcut_signature ')' /* A primitive pointcut is in the form: "primitive_pointcut_kind (primitive_pointcut_signature)". */
    {
      char *pp_kind = NULL;
      ldv_pp_ptr p_pointcut = NULL;

      /* Set a primitive pointcut kind from a lexer identifier. */
      pp_kind = ldv_get_id_name ($1);

      p_pointcut = XCNEW (ldv_primitive_pointcut);
      ldv_print_info (LDV_INFO_MEM, "primitive pointcut memory was released");

      /* Set a corresponding primitive pointcut kind. */
      if (!strcmp ("call", pp_kind))
        p_pointcut->pp_kind = LDV_PP_CALL;
      else if (!strcmp ("define", pp_kind))
        p_pointcut->pp_kind = LDV_PP_DEFINE;
      else if (!strcmp ("execution", pp_kind))
        p_pointcut->pp_kind = LDV_PP_EXECUTION;
      else if (!strcmp ("expand", pp_kind))
        p_pointcut->pp_kind = LDV_PP_EXPAND;
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
          ldv_print_info_location (@1, LDV_ERROR_BISON, "incorrect primitive pointcut kind \"%s\" was used", pp_kind);
          LDV_FATAL_ERROR ("use \"call\", \"define\", \"execution\", \"expand\", \"file\", \"get\", \"get_global\", \"get_local\", \"incall\", \"infile\", \"infunc\", \"introduce\", \"set\", \"set_global\", \"set_local\" primitive pointcut kind");
        }

      /* Set a primitive pointcut signature from a corresponding rule. */
      p_pointcut->pp_signature = $3;

      ldv_check_pp_semantics (p_pointcut);

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"%s\" primitive pointcut", pp_kind);

      /* Delete an unneeded identifier. */
      ldv_delete_id ($1);

      $$ = p_pointcut;
    };

primitive_pointcut_signature: /* It's a primitive pointcut signature, the part of primitive pointcut. */
  { ldv_ismacro = true; } primitive_pointcut_signature_macro { ldv_ismacro = false; } /* The macro primitive poincut signature form is described below. */
    {
      ldv_pps_ptr pp_signature = NULL;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a macro signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_DEFINE;

      /* Specify a macro signature. */
      pp_signature->pps_macro = $2;

      $$ = pp_signature;
    }
  | primitive_pointcut_signature_file /* The file primitive poincut signature form is described below. */
    {
      ldv_pps_ptr pp_signature = NULL;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a file signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_FILE;

      /* Specify a file signature. */
      pp_signature->pps_file = $1;

      $$ = pp_signature;
    }
  | typedef_marker_opt primitive_pointcut_signature_declaration
    {
      ldv_pps_ptr pp_signature = NULL;

      pp_signature = ldv_create_pp_signature ();

      /* Specify that a declaration signature is parsed. */
      pp_signature->pps_kind = LDV_PPS_DECL;

      /* Specify a declaration signature. */
      pp_signature->pps_declaration = $2;

      $$ = pp_signature;
    };

/* This rule is implemented just to fix a parsing bug because of typedef
   name placed at the beginning of a declaration signature. In that case
   caret simbol must prevent that typedef name. Otherwise nothing is
   needed.*/
typedef_marker_opt:

  | '^'

primitive_pointcut_signature_macro: /* It's a macro primitive pointcut signature, the one of primitive pointcut signatures. */
  LDV_ID /* A macro primitive pointcut signature is in the form: "macro_name". It's macro definition. */
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);
      ldv_print_info (LDV_INFO_MEM, "macro primitive pointcut signature memory was released");

      /* Specify that macro definition signature is parsed. */
      macro->pps_macro_kind = LDV_PPS_MACRO_DEF;

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = $1;

      /* Specify that there is no macro parameters at all. */
      macro->macro_param_list = NULL;

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro definition signature");

      $$ = macro;
    }
  | LDV_ID '(' macro_param_opt ')' /* A macro primitive pointcut signature is in the form: "macro_name (macro_parameters)". It's macro function. */
    {
      ldv_pps_macro_ptr macro = NULL;

      macro = XCNEW (ldv_primitive_pointcut_signature_macro);
      ldv_print_info (LDV_INFO_MEM, "macro primitive pointcut signature memory was released");

      /* Specify that macro function signature is parsed. */
      macro->pps_macro_kind = LDV_PPS_MACRO_FUNC;

      /* Set a macro primitive pointcut signature macro name from a lexer identifier. */
      macro->macro_name = $1;

      /* Set a macro function parameters from a corresponding rule. */
      macro->macro_param_list = $3;

      ldv_print_info (LDV_INFO_BISON, "bison parsed macro function signature");

      $$ = macro;
    };

primitive_pointcut_signature_file: /* It's a file primitive pointcut signature, the one of primitive pointcut signatures. */
  LDV_FILE /* A File primitive pointcut signature is in the form: "path_to_file". */
    {
      ldv_pps_file_ptr file = NULL;

      file = XCNEW (ldv_primitive_pointcut_signature_file);
      ldv_print_info (LDV_INFO_MEM, "file primitive pointcut signature memory was released");

      /* Set a function name from a lexer identifier. */
      file->file_name = $1;

      ldv_print_info (LDV_INFO_BISON, "bison parsed file signature");

      $$ = file;
    };

macro_param_opt:
  /* I.e. an empty list of macro parameters. */
    {
      $$ = NULL;
    }
  | macro_param
    {
      $$ = $1;
    };

macro_param: /* It's a macro function parameters, the part of macro primitive pointcut signature. */
  LDV_ID  /* A macro function parameter is in the form: "macro_parameter_name". It's the first macro function parameter. */
    {
      ldv_list_ptr macro_param_list = NULL;

      ldv_list_push_back (&macro_param_list, $1);

      $$ = macro_param_list;
    }
  | macro_param ',' LDV_ID /* A macro function parameter is in the form: "macro_parameter_name". It's the last macro function parameter after read ones. */
    {
      ldv_list_push_back (&$1, $3);

      $$ = $1;
    }
  | LDV_ELLIPSIS /* Single ellipsis is used as a macro parameter. */
    {
      ldv_list_ptr macro_param_list = NULL;
      ldv_id_ptr id = ldv_create_id ();

      ldv_puts_id ("...", id);

      ldv_list_push_back (&macro_param_list, id);

      $$ = macro_param_list;
    }
  | LDV_ID LDV_ELLIPSIS /* Identifier and following ellipsis are used for named variadic parameters. */
    {
      ldv_list_ptr macro_param_list = NULL;

      ldv_puts_id ("...", $1);

      ldv_list_push_back (&macro_param_list, $1);

      $$ = macro_param_list;
    }
  | macro_param ',' LDV_ELLIPSIS /* Ellipsis finishes a macro parameter list. */
    {
      ldv_id_ptr id = ldv_create_id ();

      ldv_puts_id ("...", id);

      ldv_list_push_back (&$1, id);

      $$ = $1;
    }
  | macro_param ',' LDV_ID LDV_ELLIPSIS /* Identifier and following ellipsis (named variadic parameters) finishes a macro parameter list. */
    {
      ldv_puts_id ("...", $3);

      ldv_list_push_back (&$1, $3);

      $$ = $1;
    };

primitive_pointcut_signature_declaration:
  c_declaration
    {
      $$ = $1;
    };

c_declaration:
  c_declaration_specifiers_aux
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = $1;

      /* Specify an empty declarator, i.e. no name is declared. */
      pps_decl->pps_declarator = NULL;

      /* Specify a kind of a declaration, a type. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_TYPE;

      ldv_print_info (LDV_INFO_BISON, "bison parsed type declaration signature pointcut");

      $$ = pps_decl;
    }
  | c_declaration_specifiers_aux c_declarator
    {
      ldv_pps_decl_ptr decl = NULL;
      ldv_pps_declarator_ptr declarator = NULL;
      ldv_list_ptr declarator_list = NULL;
      bool isdecl_kind_specified = false;

      decl = ldv_create_pps_decl ();

      /* Specify declaration specifiers and a declarator chain. */
      decl->pps_declspecs = $1;
      decl->pps_declarator = $2;

      /* Specify a kind of a declaration, a function or a variable. */
      for (declarator_list = $2
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

              $$ = decl;

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

                  $$ = decl;

                  break;
                }
            }
        }

      if (!isdecl_kind_specified)
        {
          LDV_FATAL_ERROR ("declaration kind can't be determined");
        }
    };

c_declaration_specifiers:
  c_storage_class_specifier c_declaration_specifiers_opt
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ($1, $2);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      $$ = pps_declspecs;
    }
  | c_type_specifier c_declaration_specifiers_opt
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ($1, $2);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      $$ = pps_declspecs;
    }
  | c_type_qualifier c_declaration_specifiers_opt
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ($1, $2);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      $$ = pps_declspecs;
    }
  | c_function_specifier c_declaration_specifiers_opt
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ($1, $2);

      ldv_print_info (LDV_INFO_BISON, "bison merged declaration specifiers");

      $$ = pps_declspecs;
    };

c_declaration_specifiers_opt:
  /* The end of declaration specifiers. */
    {
      $$ = NULL;
    }
  | c_declaration_specifiers
    {
      $$ = $1;
    };

c_declaration_specifiers_aux: /* This auxiliary rule isn't a part of standard and it's just intended for a context depended analysis of typedef names. */
 { ldv_istype_spec = false; } c_declaration_specifiers { ldv_istype_spec = true; }
    {
      $$ = $2;
    }

c_storage_class_specifier:
  LDV_TYPEDEF
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"typedef\" storage class specifier");

      $$ = pps_declspecs;
    }
  | LDV_EXTERN
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isextern = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"extern\" storage class specifier");

      $$ = pps_declspecs;
    }
  | LDV_STATIC
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstatic = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"static\" storage class specifier");

      $$ = pps_declspecs;
    }
  | LDV_AUTO
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isauto = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"auto\" storage class specifier");

      $$ = pps_declspecs;
    }
  | LDV_REGISTER
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isregister = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"register\" storage class specifier");

      $$ = pps_declspecs;
    };

c_type_specifier:
  LDV_VOID
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvoid = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"void\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_CHAR
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->ischar = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"char\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_INT
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isint = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"int\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_FLOAT
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isfloat = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"float\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_DOUBLE
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isdouble = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"double\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_BOOL
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isbool = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"bool\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_COMPLEX
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->iscomplex = true;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"complex\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_SHORT
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

      $$ = pps_declspecs;
    }
  | LDV_LONG
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

      $$ = pps_declspecs;
    }
  | LDV_SIGNED
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

      $$ = pps_declspecs;
    }
  | LDV_UNSIGNED
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

      $$ = pps_declspecs;
    }
  | LDV_STRUCT { ldv_istype_spec = true; } LDV_ID
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isstruct = true;
      pps_declspecs->type_name = $3;
      pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"struct\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_UNION { ldv_istype_spec = true; } LDV_ID
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isunion = true;
      pps_declspecs->type_name = $3;
      pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"union\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_ENUM { ldv_istype_spec = true; } LDV_ID
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isenum = true;
      pps_declspecs->type_name = $3;
      pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"enum\" type specifier");

      $$ = pps_declspecs;
    }
  | LDV_TYPEDEF_NAME
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->istypedef_name = true;
      pps_declspecs->type_name = $1;
      ldv_istype_spec = pps_declspecs->istype_spec = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed typedef name \"%s\" type specifier", ldv_get_id_name ($1));

      $$ = pps_declspecs;
    };

c_type_qualifier:
  LDV_CONST
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isconst = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"const\" type qualifier");

      $$ = pps_declspecs;
    }
  | LDV_RESTRICT
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isrestrict = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"restrict\" type qualifier");

      $$ = pps_declspecs;
    }
  | LDV_VOLATILE
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isvolatile = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"volatile\" type qualifier");

      $$ = pps_declspecs;
    };

c_function_specifier:
  LDV_INLINE
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_create_declspecs ();

      pps_declspecs->isinline = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed \"inline\" type qualifier");

      $$ = pps_declspecs;
    };

c_declarator:
  c_pointer_opt c_direct_declarator
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed declarator");

      $$ = ldv_list_splice ($1, $2);
    };

c_direct_declarator:
  LDV_ID
    {
      ldv_pps_declarator_ptr declarator_new = NULL;
      ldv_list_ptr declarator_list = NULL;

      declarator_new = ldv_create_declarator ();

      declarator_list = ldv_list_push_front (NULL, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ID;

      declarator_new->declarator_name = $1;

      ldv_print_info (LDV_INFO_BISON, "bison parsed identifier direct declarator");

      $$ = declarator_list;
    }
  | '(' c_declarator ')'
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct declarator of associativity");

      $$ = $2;
    }
  | c_direct_declarator '[' int_opt ']'
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      $1 = ldv_list_push_front ($1, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ARRAY;

      declarator_new->pps_array_size = ldv_create_pps_array_size ();

      /* If an array size was specified then save it. */
      if ($3)
        {
          declarator_new->pps_array_size->pps_array_size = ldv_get_int ($3);
          declarator_new->pps_array_size->issize_specified = true;
        }
      else
        declarator_new->pps_array_size->issize_specified = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed array direct declarator");

      $$ = $1;
    }
  | c_direct_declarator '(' c_parameter_type_list ')'
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      $1 = ldv_list_push_front ($1, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = $3;

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct declarator");

      $$ = $1;
    };

c_pointer_opt:
  /* A declarator without pointers at the beginning. */
    {
      $$ = NULL;
    }
  | c_pointer
    {
      $$ = $1;
    };

c_pointer:
  '*' c_type_qualifier_list_opt
    {
      ldv_pps_declarator_ptr declarator_new = NULL;
      ldv_list_ptr declarator_list = NULL;

      declarator_new = ldv_create_declarator ();

      declarator_list = ldv_list_push_front (NULL, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      declarator_new->pps_ptr_quals = ldv_set_ptr_quals ($2);

      ldv_print_info (LDV_INFO_BISON, "bison parsed pointer declarator");

      $$ = declarator_list;
    }
  | '*' c_type_qualifier_list_opt c_pointer
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      $3 = ldv_list_push_front ($3, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      declarator_new->pps_ptr_quals = ldv_set_ptr_quals ($2);

      ldv_print_info (LDV_INFO_BISON, "bison parsed pointer declarator");

      $$ = $3;
    };

int_opt:
  /* An array without specified size. */
    {
      $$ = NULL;
    };
  | LDV_INT_NUMB
    {
      $$ = $1;
    }

c_type_qualifier_list_opt:
  /* A pointer without qualifiers. */
    {
      $$ = NULL;
    };
  | c_type_qualifier_list
    {
      $$ = $1;
    };

c_type_qualifier_list:
  c_type_qualifier
    {
      $$ = $1;
    }
  | c_type_qualifier_list c_type_qualifier
    {
      ldv_pps_declspecs_ptr pps_declspecs = NULL;

      pps_declspecs = ldv_merge_declspecs ($1, $2);

      ldv_print_info (LDV_INFO_BISON, "bison merged type qualifiers");

      $$ = pps_declspecs;
    };

c_parameter_type_list:
  c_parameter_list { /* It's a hack!!! It's needed to alow a correct processing of typedefs names inside a parameter list. */ ldv_istype_spec = true; }
    {
      ldv_pps_func_arg_ptr func_arg = NULL;

      func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data ($1);

      func_arg->isva = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter list");

      $$ = $1;
    }
  | c_parameter_list ',' LDV_ELLIPSIS
    {
      ldv_pps_func_arg_ptr func_arg = NULL;
      ldv_list_ptr func_arg_list = NULL;

      func_arg_list = ldv_list_get_last ($1);

      func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (func_arg_list);

      func_arg->isva = true;

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter list of variable length");

      $$ = $1;
    };

c_parameter_list:
  c_parameter_declaration
    {
      ldv_pps_func_arg_ptr pps_func_arg_new = NULL;
      ldv_list_ptr func_arg_list = NULL;

      pps_func_arg_new = ldv_create_pps_func_arg ();

      pps_func_arg_new->pps_func_arg = $1;

      pps_func_arg_new->isva = false;

      /* It's a hack!!! It's needed to alow a correct processing of typedefs names inside a parameter list. */
      ldv_istype_spec = false;

      ldv_list_push_back (&func_arg_list, pps_func_arg_new);

      $$ = func_arg_list;
    }
  | c_parameter_list ',' c_parameter_declaration
    {
      ldv_pps_func_arg_ptr pps_func_arg_new = NULL;

      pps_func_arg_new = ldv_create_pps_func_arg ();

      pps_func_arg_new->pps_func_arg = $3;

      ldv_list_push_back (&$1, pps_func_arg_new);

      pps_func_arg_new->isva = false;

      /* It's a hack!!! It's needed to alow a correct processing of typedefs names inside a parameter list. */
      ldv_istype_spec = false;

      $$ = $1;
    };

c_parameter_declaration:
   LDV_ANY_PARAMS
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify that any parameters may correspond to this 'declaration'. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_ANY_PARAMS;

      ldv_print_info (LDV_INFO_BISON, "bison parsed any parameters wildcard");

      $$ = pps_decl;
    }
   | c_declaration_specifiers_aux c_declarator
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify a kind of a declaration. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_PARAM;

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = $1;

      /* Specify a declarator. */
      pps_decl->pps_declarator = $2;

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter declaration");

      $$ = pps_decl;
    }
  | c_declaration_specifiers_aux c_abstract_declarator_opt
    {
      ldv_pps_decl_ptr pps_decl = NULL;

      pps_decl = ldv_create_pps_decl ();

      /* Specify a kind of a declaration. */
      pps_decl->pps_decl_kind = LDV_PPS_DECL_PARAM;

      /* Specify declaration specifiers. */
      pps_decl->pps_declspecs = $1;

      /* Specify an abstact declarator. */
      pps_decl->pps_declarator = $2;

      ldv_print_info (LDV_INFO_BISON, "bison parsed parameter declaration");

      $$ = pps_decl;
    };

c_abstract_declarator:
  c_pointer
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      $$ = $1;
    }
  | c_pointer_opt c_direct_abstract_declarator
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed abstract declarator");

      $$ = ldv_list_splice ($1, $2);
    };

c_abstract_declarator_opt:
  /* A function argument as pure declaration specifiers. */
    {
      ldv_list_ptr declarator_list = NULL;

      declarator_list = ldv_list_push_front (NULL, ldv_create_declarator ());

      ldv_print_info (LDV_INFO_BISON, "bison parsed empty abstract declarator");

      $$ = declarator_list;
    }
  | c_abstract_declarator
    {
      $$ = $1;
    };

c_direct_abstract_declarator:
  '(' c_abstract_declarator ')'
    {
      ldv_print_info (LDV_INFO_BISON, "bison parsed direct abstract declarator of associativity");

      $$ = $2;
    }
  | c_direct_abstract_declarator_opt '[' int_opt ']'
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      $1 = ldv_list_push_front ($1, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_ARRAY;

      declarator_new->pps_array_size = ldv_create_pps_array_size ();

      /* If an array size was specified then save it. */
      if ($3)
        {
          declarator_new->pps_array_size->pps_array_size = ldv_get_int ($3);
          declarator_new->pps_array_size->issize_specified = true;
        }
      else
        declarator_new->pps_array_size->issize_specified = false;

      ldv_print_info (LDV_INFO_BISON, "bison parsed array direct abstract declarator");

      $$ = $1;
    }
  /* This isn't equal to the C specification. This is done to avoid a bison shift/reduce conflict. */
  | c_direct_abstract_declarator '(' c_parameter_type_list_opt ')'
    {
      ldv_pps_declarator_ptr declarator_new = NULL;

      declarator_new = ldv_create_declarator ();

      $1 = ldv_list_push_front ($1, declarator_new);

      declarator_new->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      declarator_new->func_arg_list = $3;

      ldv_print_info (LDV_INFO_BISON, "bison parsed function direct abstract declarator");

      $$ = $1;
    };

c_direct_abstract_declarator_opt:
  /* An empty direct abstract declarator. */
    {
      $$ = NULL;
    }
  | c_direct_abstract_declarator
    {
      $$ = $1;
    };

c_parameter_type_list_opt:
  /* I.e. an empty list of direct abstact declarator parameters. */
    {
      $$ = NULL;
    }
  | c_parameter_type_list
    {
      $$ = $1;
    };

%%

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

  /* Do not bind an identifier with some C keyword when parse a macro
     signature. */
  if (!ldv_ismacro)
    {
      /* Check whether an identifier is a C declaration specifier keyword. */
      for (i = 0; ldv_keyword_token_map[i].keyword; i++)
        {
          if (!strcmp(id, ldv_keyword_token_map[i].keyword))
            {
              ldv_print_info (LDV_INFO_LEX, "lex parsed keyword \"%s\"", id);
              return ldv_keyword_token_map[i].token;
            }
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

/* We aren't interested in advice body content, except for correct determinition
   of the advice body end (that is '}') and aspect patterns. */
unsigned int
ldv_parse_advice_body (ldv_ab_ptr *body)
{
  int c;
  int brace_count = 1;
  ldv_aspect_pattern_ptr pattern = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  char *comment = NULL;

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
              ldv_puts_body (comment, *body);
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

  c = ldv_getc (LDV_ASPECT_STREAM);

  /* String aspect pattern parameter is a sequence of characters in quotes. */
  if (c == '"')
    {
      byte_count++;
      str_read = ldv_create_string ();

      /* Get the rest of string. */
      while (1)
        {
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
ldv_parse_aspect_pattern_known_value (char const **str)
{
  ldv_aspect_pattern_ptr pattern = NULL;

  /* First of all try to parse aspect pattern. */
  if ((pattern = ldv_parse_aspect_pattern ()))
    {
      if (pattern->value)
        {
          *str = pattern->value;

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

char *
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
              return ldv_get_text (comment);
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
                          return ldv_get_text (comment);
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
  char const *aspect_pattern_value = NULL;

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
      *file_name = ldv_get_str (str);

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
      *id = ldv_get_str (str);

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

  /* Skip nonsignificant whitespaces from the beginning of a current line. */
  ldv_parse_whitespaces ();

  /* Examine whether a C or C++ comment is encountered. Skip a corresponding
     comment if so and continue parsing. */
  if (ldv_parse_comments ())
    return yylex ();

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

      return ldv_get_id_kind (str);
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
