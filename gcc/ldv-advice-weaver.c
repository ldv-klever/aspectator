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

/* General gcc core types and functions. */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"

/* Tree conceptions. */
#include "tree.h"

/* For lookup_name function. */
#include "c-lang.h"

/* For error functions. */
#include "diagnostic-core.h"
#include "toplev.h"

#include "ldv-advice-weaver.h"
#include "ldv-aspect-types.h"
#include "ldv-converter.h"
#include "ldv-core.h"
#include "ldv-cpp-advice-weaver.h"
#include "ldv-cpp-converter.h"
#include "ldv-cpp-pointcut-matcher.h"
#include "ldv-io.h"
#include "ldv-opts.h"
#include "ldv-pointcut-matcher.h"


#define LDV_ASPECTED_NAME_PREFIX    "ldv"
#define LDV_FUNC_ARG_NAME           "ldv_func_arg"
#define LDV_FUNC_ARG_TYPE           "ldv_func_arg_type"
#define LDV_FUNC_RET_TYPE           "ldv_func_ret_type"
#define LDV_FUNC_RES                "ldv_func_res"
#define LDV_VAR_ARG_NAME            "ldv_var_arg"

#define LDV_IDENTIFIER_POINTER(NODE) (IDENTIFIER_NODE_CHECK (NODE)->identifier.id.str)


/* These constants say where indentation spaces are needed. */
typedef enum { LDV_PADDING_AFTER, LDV_PADDING_BEFORE, LDV_PADDING_NONE } ldv_padding;


expanded_location ldv_decl_beginning_location;

static const char *ldv_aspect_func_name = NULL;
static ldv_list_ptr ldv_func_arg_type_decl_list = NULL;
static ldv_list_ptr ldv_func_arg_type_name_list = NULL;
static ldv_text_ptr ldv_func_call = NULL;
static const char *ldv_func_name = NULL;
static ldv_list_ptr ldv_func_param_list = NULL;
static ldv_pps_decl_ptr ldv_func_ret_type_decl = NULL;
static ldv_text_ptr ldv_func_va_init = NULL;
static const char *ldv_var_name = NULL;
static const char *ldv_var_type_name = NULL;
static ldv_list_ptr ldv_var_init_list = NULL;
static bool ldv_isstatic_specifier_needed = true;
static bool ldv_isstorage_class_and_function_specifiers_needed = true;
static ldv_list_ptr ldv_name_weaved_list = NULL;
static ldv_padding ldv_padding_cur = LDV_PADDING_NONE;
static ldv_text_ptr ldv_text_printed = NULL;
static ldv_i_func_ptr ldv_func_signature = NULL;


static void ldv_add_id_declarator (ldv_pps_decl_ptr, const char *);
static char *ldv_create_aspected_name (const char *);
static void ldv_create_aux_func_params (ldv_i_type_ptr, ldv_i_type_ptr);
static const char *ldv_create_aux_func_param (unsigned int);
static ldv_decl_for_print_ptr ldv_create_decl_for_print (void);
static void ldv_delete_id_declarator (ldv_list_ptr);
static void ldv_diag_composite_pointcut (ldv_cp_ptr, FILE *);
static void ldv_diag_primitive_pointcut (ldv_pp_ptr, FILE *);
static void ldv_diag_recursive_composite_pointcut (ldv_cp_ptr);
static ldv_pps_decl_ptr ldv_diag_replace_return_type (ldv_pps_decl_ptr);
static int ldv_evaluate_aspect_pattern (ldv_aspect_pattern_ptr, char **, unsigned int *);
static const char *ldv_get_arg_name (unsigned int);
static const char *ldv_get_arg_sign (unsigned int);
static const char *ldv_get_arg_type_name (unsigned int);
static int ldv_get_arg_size (unsigned int);
static const char *ldv_get_arg_value (unsigned int);
static const char *ldv_get_param_name (unsigned int);
static void ldv_print_c (unsigned int);
static void ldv_print_composite_pointcut (ldv_cp_ptr);
static void ldv_print_body (ldv_ab_ptr, ldv_ak);
static void ldv_print_decl (ldv_pps_decl_ptr);
static void ldv_print_declarator (ldv_list_ptr);
static void ldv_print_declspecs (ldv_pps_declspecs_ptr);
static void ldv_print_direct_declarator (ldv_list_ptr);
static void ldv_print_int (int);
static const char *ldv_print_func_context (ldv_i_func_ptr);
static const char *ldv_print_func_context_name (ldv_i_func_ptr);
static const char *ldv_print_func_context_path (ldv_i_func_ptr);
static const char *ldv_print_func_path (ldv_i_func_ptr);
static const char *ldv_print_func_signature (ldv_pps_decl_ptr);
static void ldv_print_macro_name (ldv_id_ptr);
static void ldv_print_macro_param (ldv_list_ptr);
static const char *ldv_print_macro_signature (ldv_pps_macro_ptr);
static ldv_list_ptr ldv_print_ptr (ldv_list_ptr);
static void ldv_print_qual (bool, bool, bool);
static void ldv_print_separator (unsigned int);
static bool ldv_print_space (void);
static bool ldv_print_space_after_pointer_star (void);
static void ldv_print_str (const char *);
static void ldv_print_str_without_padding (const char *);
static void ldv_print_types_typedefs (ldv_ab_ptr, bool);
static void ldv_store_func_arg_type_decl_list (ldv_i_type_ptr);
static void ldv_weave_func_source (ldv_i_func_ptr, ldv_ppk);
static void ldv_weave_var_source (ldv_i_var_ptr, ldv_ppk);


void
ldv_add_id_declarator (ldv_pps_decl_ptr decl, const char *name)
{
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;

  /* Go to the end of a declarator chain. */
  if (decl->pps_declarator)
    {
      declarator_list = ldv_list_get_last (decl->pps_declarator);

      declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

      /* And add a declaration name. */
      if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_NONE)
        {
          declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_ID;
          declarator->declarator_name = ldv_create_id ();
          ldv_puts_id (name, declarator->declarator_name);

          return ;
        }
    }
  else
    {
      declarator = ldv_create_declarator ();

      declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_ID;
      declarator->declarator_name = ldv_create_id ();
      ldv_puts_id (name, declarator->declarator_name);

      ldv_list_push_back (&decl->pps_declarator, declarator);

      return ;
    }

  LDV_FATAL_ERROR ("can't add identifier declarator to the end of declarator chain");
}

char *
ldv_create_aspected_name (const char *name)
{
  char *aspected_name = NULL;
  char *aspected_name_numb;

  aspected_name_numb = ldv_itoa (ldv_get_unique_numb ());

  /* Aspected name = ldv prefix + _ + original name + _ + unique number. */
  aspected_name = XCNEWVEC (char, strlen (LDV_ASPECTED_NAME_PREFIX) + 1 + strlen (name) + 1 + strlen (aspected_name_numb) + 1);
  ldv_print_info (LDV_INFO_MEM, "aspected name memory was released");

  sprintf (aspected_name, "%s_%s_%s", LDV_ASPECTED_NAME_PREFIX, name, aspected_name_numb);

  free (aspected_name_numb);

  return aspected_name;
}

const char *
ldv_create_aux_func_param (unsigned int param_numb)
{
  ldv_id_ptr param_name_aux = NULL;
  const char *param_numb_str = NULL;

  param_name_aux = ldv_create_id ();
  ldv_puts_id (LDV_FUNC_ARG_NAME, param_name_aux);
  param_numb_str = ldv_itoa (param_numb);
  ldv_puts_id (param_numb_str, param_name_aux);

  return ldv_get_id_name (param_name_aux);
}

void
ldv_create_aux_func_params (ldv_i_type_ptr src_func_type, ldv_i_type_ptr aspect_func_type)
{
  ldv_list_ptr src_param_list = NULL, aspect_param_list = NULL;
  ldv_i_param_ptr src_param = NULL, aspect_param = NULL;
  unsigned int param_numb;
  const char *param_name = NULL;
  ldv_str_ptr param_name_str = NULL;

  /* Clean up previously created function parameter names list if so. */
  ldv_func_param_list = NULL;

  /* Walk through source function declaration parameters and pointcut ones and
     prepare a resulting list of parameter names for an auxiliary function. Do
     not perform any consistency check here since we suspect that everything is
     correct. */
  for (src_param_list = src_func_type->param
    , aspect_param_list = aspect_func_type->param, param_numb = 1
    ; src_param_list
    ; src_param_list = ldv_list_get_next (src_param_list)
    , aspect_param_list = ldv_list_get_next (aspect_param_list), param_numb++)
    {
      src_param = (ldv_i_param_ptr) ldv_list_get_data (src_param_list);
      aspect_param = (ldv_i_param_ptr) ldv_list_get_data (aspect_param_list);

      /* Void type doesn't require a name for a corresponding parameter. */
      if (ldv_isvoid (src_param->type))
        continue;

      /* Parameter names specified in pointcut is more preferable then source
         ones. */
      if (aspect_param->name)
        param_name = ldv_get_id_name (aspect_param->name);
      /* Use source parameter name if so otherwise. */
      else if (src_param->name)
        param_name = ldv_get_id_name (src_param->name);
      /* Otherwise generate unique artificial name for a given parameter. */
      else
        param_name = ldv_create_aux_func_param (param_numb);

      /* Store the obtained parameter name as aspect parameter name to print
         in in auxiliary prototypes and function definitions. */
      if (!aspect_param->name)
        {
          aspect_param->name = ldv_create_id ();
          ldv_puts_id (param_name, aspect_param->name);
        }

      /* Add the obtained parameter name to a list of parameter names. */
      param_name_str = ldv_create_string ();
      ldv_puts_string (param_name, param_name_str);
      ldv_list_push_back (&ldv_func_param_list, param_name_str);

      /* Generate auxiliary name and initializations for a variable argument
         list parameter. It must have form:
           va_list aux_arg_name;
           __builtin_va_start (aux_arg_name, last_arg_name); */
      if (!ldv_list_get_next (src_param_list) && src_param->type->isva)
      {
        /* Create an auxiliary name for the given parameter. */
        param_name = ldv_create_aux_func_param (param_numb + 1);

        ldv_func_va_init = ldv_create_text ();

        ldv_puts_text ("\n  char * ", ldv_func_va_init);
        ldv_puts_text (param_name, ldv_func_va_init);
        ldv_puts_text (";\n  __builtin_va_start (", ldv_func_va_init);
        ldv_puts_text (param_name, ldv_func_va_init);
        ldv_puts_text (", ", ldv_func_va_init);
        ldv_puts_text (ldv_get_str ((ldv_str_ptr) ldv_list_get_data (ldv_list_get_last (ldv_func_param_list))), ldv_func_va_init);
        ldv_puts_text (");\n", ldv_func_va_init);

        param_name_str = ldv_create_string ();
        ldv_puts_string (param_name, param_name_str);
        ldv_list_push_back (&ldv_func_param_list, param_name_str);
      }
    }
}

ldv_decl_for_print_ptr
ldv_create_decl_for_print (void)
{
  ldv_decl_for_print_ptr decl_for_print = NULL;

  decl_for_print = XCNEW (ldv_decl_for_print);
  ldv_print_info (LDV_INFO_MEM, "declaration for print memory was released");

  decl_for_print->decl = ldv_create_text ();

  return decl_for_print;
}

void
ldv_delete_id_declarator (ldv_list_ptr declarator_list)
{
  ldv_pps_declarator_ptr declarator = NULL;

  /* Go to the end of a declarator chain. */
  if ((declarator_list = ldv_list_get_last (declarator_list)))
    {
      declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

      /* And remove a declaration name if so. */
      if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_ID)
        {
          declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_NONE;
          declarator->declarator_name = NULL;

          return ;
        }
    }

  LDV_FATAL_ERROR ("can't remove identifier declarator from the end of declarator chain");
}

int
ldv_evaluate_aspect_pattern (ldv_aspect_pattern_ptr pattern, char **string, unsigned int *integer)
{
  char *text = NULL;
  unsigned int number;
  bool is_number = false;
  char *func_arg = NULL;
  int func_arg_size;

  if (!strcmp (pattern->name, "arg"))
    text = ldv_copy_str (ldv_get_param_name (pattern->arg_numb));
  else if (!strcmp (pattern->name, "arg_type"))
    text = ldv_copy_str (ldv_get_arg_type_name (pattern->arg_numb));
  else if (!strcmp (pattern->name, "arg_name"))
    text = ldv_copy_str (ldv_get_arg_name (pattern->arg_numb));
  else if (!strcmp (pattern->name, "arg_size"))
    {
      func_arg_size = ldv_get_arg_size (pattern->arg_numb);

      /* Print stub '-1' when an argument size isn't specified. */
      if (func_arg_size == -1)
        {
          text = ldv_copy_str ("-1");
          ldv_print_info (LDV_INFO_WEAVE, "generate stub \"-1\" for aspect pattern \"%s\"", pattern->name);
        }
      else
        text = ldv_itoa ((unsigned int) func_arg_size);
    }
  else if (!strcmp (pattern->name, "arg_value"))
    {
      func_arg = ldv_copy_str (ldv_get_arg_value (pattern->arg_numb));

      /* Print stub 'NULL' when an argument size isn't specified. */
      if (func_arg == NULL)
        {
          text = ldv_copy_str ("0");
          ldv_print_info (LDV_INFO_WEAVE, "generate stub \"0\" for aspect pattern \"%s\"", pattern->name);
        }
      else
        text = func_arg;
    }
  else if (!strcmp (pattern->name, "arg_numb"))
    {
      number = ldv_list_len (ldv_func_arg_info_list);
      is_number = true;
    }
  else if (!strcmp (pattern->name, "aspect_func_name"))
    {
      if (ldv_aspect_func_name)
        text = ldv_copy_str (ldv_aspect_func_name);
      else
        {
          LDV_FATAL_ERROR ("no function name was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "func_name"))
    {
      if (ldv_func_name)
        text = ldv_copy_str (ldv_func_name);
      else
        {
          LDV_FATAL_ERROR ("no aspect function name was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "func_context"))
    {
      if (ldv_func_signature)
        {
          if (ldv_func_signature->func_context)
            text = ldv_copy_str (ldv_print_func_context (ldv_func_signature));
          else
            {
              LDV_FATAL_ERROR ("no function context was found for aspect pattern \"%s\"", pattern->name);
            }
        }
      else
        {
          LDV_FATAL_ERROR ("no function signature was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "func_context_name"))
    {
      if (ldv_func_signature)
        {
          if (ldv_func_signature->func_context)
            text = ldv_copy_str (ldv_print_func_context_name (ldv_func_signature));
          else
            {
              LDV_FATAL_ERROR ("no function context was found for aspect pattern \"%s\"", pattern->name);
            }
        }
      else
        {
          LDV_FATAL_ERROR ("no function signature was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "func_context_path"))
    {
      if (ldv_func_signature)
        {
          if (ldv_func_signature->func_context)
            text = ldv_copy_str (ldv_print_func_context_path (ldv_func_signature));
          else
            {
              LDV_FATAL_ERROR ("no function context was found for aspect pattern \"%s\"", pattern->name);
            }
        }
      else
        {
          LDV_FATAL_ERROR ("no function signature was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if ((!strcmp (pattern->name, "func_signature")) || (!strcmp (pattern->name, "signature")))
    {
      if (ldv_func_signature)
        text = ldv_copy_str (ldv_print_func_decl (ldv_func_signature));
      else
        {
          LDV_FATAL_ERROR ("no function signature was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "path"))
    {
      if (ldv_func_signature)
        text = ldv_copy_str (ldv_print_func_path (ldv_func_signature));
      else
        {
          LDV_FATAL_ERROR ("no function signature was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "proceed"))
    {
      if (ldv_func_call)
        text = ldv_get_text (ldv_func_call);
    }
  else if (!strcmp (pattern->name, "res"))
    text = ldv_copy_str (LDV_FUNC_RES);
  else if (!strcmp (pattern->name, "ret_type"))
    text = ldv_copy_str (LDV_FUNC_RET_TYPE);
  else if (!strcmp (pattern->name, "var_name"))
    {
      if (ldv_var_name)
        text = ldv_copy_str (ldv_var_name);
      else
        {
          LDV_FATAL_ERROR ("no variable name was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "var_type_name"))
    {
      if (ldv_var_type_name)
        text = ldv_copy_str (ldv_var_type_name);
      else
        {
          LDV_FATAL_ERROR ("no variable type name was found for aspect pattern \"%s\"", pattern->name);
        }
    }
  else if (!strcmp (pattern->name, "env"))
    text = ldv_copy_str (pattern->value);
  else if (!strcmp (pattern->name, "arg_sign"))
    text = ldv_copy_str (ldv_get_arg_sign (pattern->arg_numb));

  if (text)
    {
      *string = text;
      return 1;
    }
  else if (is_number)
    {
      *integer = number;
      return 1;
    }

  return 0;
}

const char *
ldv_get_arg_sign (unsigned int arg_numb)
{
  unsigned int i;
  ldv_list_ptr func_arg_info_list = NULL;
  ldv_func_arg_info_ptr func_arg_info = NULL;

  /* Walk through function arguments to find appropriate. */
  for (i = 1, func_arg_info_list = ldv_func_arg_info_list
    ; func_arg_info_list
    ; i++, func_arg_info_list = ldv_list_get_next (func_arg_info_list))
    {
      func_arg_info = (ldv_func_arg_info_ptr) ldv_list_get_data (func_arg_info_list);

      if (i == arg_numb)
        return func_arg_info->sign;
    }

  LDV_FATAL_ERROR ("required argument value has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
}

const char *
ldv_get_arg_type_name (unsigned int arg_numb)
{
  unsigned int i;
  ldv_str_ptr str = NULL;
  ldv_list_ptr str_list = NULL;

  /* Walk through function argument types names to find appropriate. */
  for (i = 1, str_list = ldv_func_arg_type_name_list
    ; str_list
    ; i++, str_list = ldv_list_get_next (str_list))
    {
      str = (ldv_str_ptr) ldv_list_get_data (str_list);

      if (i == arg_numb)
        return ldv_get_str (str);
    }

  LDV_FATAL_ERROR ("required argument type name has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
}

int
ldv_get_arg_size (unsigned int arg_numb)
{
  unsigned int i;
  ldv_list_ptr func_arg_info_list = NULL;
  ldv_func_arg_info_ptr func_arg_info = NULL;

  /* Walk through function arguments to find appropriate. */
  for (i = 1, func_arg_info_list = ldv_func_arg_info_list
    ; func_arg_info_list
    ; i++, func_arg_info_list = ldv_list_get_next (func_arg_info_list))
    {
      func_arg_info = (ldv_func_arg_info_ptr) ldv_list_get_data (func_arg_info_list);

      if (i == arg_numb)
        {
          if (func_arg_info->func_arg_info_kind == LDV_FUNC_ARG_INFO_ONE_DIM_ARRAY_SIZE)
            return func_arg_info->one_dim_array_size;
          else
            {
              ldv_print_info (LDV_INFO_WEAVE, "body pattern argument size for \"%d\" function argument can't be weaved", arg_numb);
              return -1;
            }
        }
    }

  LDV_FATAL_ERROR ("required argument size has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
}

const char *
ldv_get_arg_name (unsigned int arg_numb)
{
  unsigned int i;
  ldv_list_ptr func_arg_info_list = NULL;
  ldv_func_arg_info_ptr func_arg_info = NULL;

  /* Walk through function arguments to find appropriate. */
  for (i = 1, func_arg_info_list = ldv_func_arg_info_list
    ; func_arg_info_list
    ; i++, func_arg_info_list = ldv_list_get_next (func_arg_info_list))
    {
      func_arg_info = (ldv_func_arg_info_ptr) ldv_list_get_data (func_arg_info_list);

      if (i == arg_numb)
        {
          if (func_arg_info->arg_name)
            return func_arg_info->arg_name;
          else
            {
              ldv_print_info (LDV_INFO_WEAVE, "body pattern argument name for \"%d\" function argument can't be weaved", arg_numb);
              return NULL;
            }
        }
    }

  LDV_FATAL_ERROR ("required argument value has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
}

const char *
ldv_get_arg_value (unsigned int arg_numb)
{
  unsigned int i;
  ldv_list_ptr func_arg_info_list = NULL;
  ldv_func_arg_info_ptr func_arg_info = NULL;

  /* Walk through function arguments to find appropriate. */
  for (i = 1, func_arg_info_list = ldv_func_arg_info_list
    ; func_arg_info_list
    ; i++, func_arg_info_list = ldv_list_get_next (func_arg_info_list))
    {
      func_arg_info = (ldv_func_arg_info_ptr) ldv_list_get_data (func_arg_info_list);

      if (i == arg_numb)
        {
          if (func_arg_info->func_arg_info_kind == LDV_FUNC_ARG_INFO_FUNC_NAME)
            return func_arg_info->func_name;
          else
            {
              ldv_print_info (LDV_INFO_WEAVE, "body pattern argument value for \"%d\" function argument can't be weaved", arg_numb);
              return NULL;
            }
        }
    }

  LDV_FATAL_ERROR ("required argument value has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
}

const char *
ldv_get_param_name (unsigned int param_numb)
{
  unsigned int i;
  ldv_str_ptr param_str = NULL;
  ldv_list_ptr param_str_list = NULL;

  /* Walk through function parameters to find appropriate. */
  for (i = 1, param_str_list = ldv_func_param_list
    ; param_str_list
    ; i++, param_str_list = ldv_list_get_next (param_str_list))
    {
      param_str = (ldv_str_ptr) ldv_list_get_data (param_str_list);

      if (i == param_numb)
        return ldv_get_str (param_str);
    }

  LDV_FATAL_ERROR ("required parameter has number \"%d\" that exceeds the maximum one \"%d\"", param_numb, (i - 1));
}

bool
ldv_isweaved (const char *name, bool is_check)
{
  char *name_weaved = NULL, *name_save;
  ldv_list_ptr name_weaved_list;

  for (name_weaved_list = ldv_name_weaved_list
    ; name_weaved_list
    ; name_weaved_list = ldv_list_get_next (name_weaved_list))
    {
      name_weaved = (char *) ldv_list_get_data (name_weaved_list);

      /* In this case a name was already weaved.*/
      if (!strcmp (name, name_weaved))
        return true;
    }

  /* Store a weaved name if it isn't checking. */
  if (!is_check)
    {
      /* To prevent a warning about const qualifier discarding use an auxliary
        string. */
      name_save = XCNEWVEC (char, strlen (name) + 1);

      strcpy (name_save, name);

      ldv_list_push_back (&ldv_name_weaved_list, name_save);
    }

  /* An entity with such name wasn't weaved yet. */
  return false;
}

void
ldv_print_c (unsigned int c)
{
  /* By default there is no indentation space before a single character (that is
     before different punctuations and pointer stars). In case when such space
     is needed, it must be added manually into a corresponding place. */
  ldv_putc_text (c, ldv_text_printed);

  /* After a single character printing there is no indentation space by
     default. */
  ldv_padding_cur = LDV_PADDING_NONE;
}

void
ldv_print_composite_pointcut (ldv_cp_ptr c_pointcut)
{
  if (c_pointcut != NULL)
    {
      if (c_pointcut->cp_kind != LDV_CP_PRIMITIVE)
        ldv_print_str_without_padding ("(");

      if (c_pointcut->cp_kind == LDV_CP_NOT)
        ldv_print_str_without_padding ("!");

      ldv_print_composite_pointcut (c_pointcut->c_pointcut_first);

      if ((c_pointcut->cp_kind == LDV_CP_PRIMITIVE) && (c_pointcut->p_pointcut->pp_signature->pps_kind == LDV_PPS_DECL))
        {
          if (c_pointcut->p_pointcut->pp_kind == LDV_PP_CALL)
            {
              ldv_print_str_without_padding ("call(");
              ldv_print_decl (c_pointcut->p_pointcut->pp_signature->pps_declaration);
              ldv_print_str_without_padding (")");
            }
          else if (c_pointcut->p_pointcut->pp_kind == LDV_PP_EXECUTION)
            {
              ldv_print_str_without_padding ("execution(");
              ldv_print_decl (c_pointcut->p_pointcut->pp_signature->pps_declaration);
              ldv_print_str_without_padding (")");
            }
        }
      else if ((c_pointcut->cp_kind == LDV_CP_PRIMITIVE) && (c_pointcut->p_pointcut->pp_signature->pps_kind == LDV_PPS_DEFINE))
        {
          ldv_print_str_without_padding ("define(");

          ldv_print_macro_name (c_pointcut->p_pointcut->pp_signature->pps_macro->macro_name);
          if (c_pointcut->p_pointcut->pp_signature->pps_macro->macro_param_list)
            ldv_print_macro_param (c_pointcut->p_pointcut->pp_signature->pps_macro->macro_param_list);

          ldv_print_str_without_padding (")");
        }
      else if (c_pointcut->cp_kind == LDV_CP_OR)
        ldv_print_str_without_padding (" || ");
      else if (c_pointcut->cp_kind == LDV_CP_AND)
        ldv_print_str_without_padding (" && ");

      ldv_print_composite_pointcut (c_pointcut->c_pointcut_second);

      if (c_pointcut->cp_kind != LDV_CP_PRIMITIVE)
        ldv_print_str_without_padding (")");
    }
}

void
ldv_diag_recursive_composite_pointcut (ldv_cp_ptr c_pointcut)
{
  if (c_pointcut != NULL)
    {
      if (c_pointcut->cp_kind != LDV_CP_PRIMITIVE)
        ldv_print_str_without_padding ("(");

      if (c_pointcut->cp_kind == LDV_CP_NOT)
        ldv_print_str_without_padding ("!");

      ldv_diag_recursive_composite_pointcut (c_pointcut->c_pointcut_first);

      if ((c_pointcut->cp_kind == LDV_CP_PRIMITIVE) && (c_pointcut->p_pointcut->pp_signature->pps_kind == LDV_PPS_DECL))
        {
          if (c_pointcut->p_pointcut->pp_kind == LDV_PP_CALL)
            {
              ldv_print_str_without_padding ("(declare_func(");
              ldv_print_decl (c_pointcut->p_pointcut->pp_signature->pps_declaration);
              ldv_print_str_without_padding (") || execution(");
              ldv_print_decl (c_pointcut->p_pointcut->pp_signature->pps_declaration);
              ldv_print_str_without_padding ("))");
            }
          else if (c_pointcut->p_pointcut->pp_kind == LDV_PP_EXECUTION)
            {
              ldv_print_str_without_padding ("execution(");
              ldv_print_decl (c_pointcut->p_pointcut->pp_signature->pps_declaration);
              ldv_print_str_without_padding (")");
            }
        }
      else if ((c_pointcut->cp_kind == LDV_CP_PRIMITIVE) && (c_pointcut->p_pointcut->pp_signature->pps_kind == LDV_PPS_DEFINE))
        {
          ldv_print_str_without_padding ("define(");

          ldv_print_macro_name (c_pointcut->p_pointcut->pp_signature->pps_macro->macro_name);
          if (c_pointcut->p_pointcut->pp_signature->pps_macro->macro_param_list)
            ldv_print_macro_param (c_pointcut->p_pointcut->pp_signature->pps_macro->macro_param_list);

          ldv_print_str_without_padding (")");
        }
      else if (c_pointcut->cp_kind == LDV_CP_OR)
        ldv_print_str_without_padding (" || ");
      else if (c_pointcut->cp_kind == LDV_CP_AND)
        ldv_print_str_without_padding (" && ");

      ldv_diag_recursive_composite_pointcut (c_pointcut->c_pointcut_second);

      if (c_pointcut->cp_kind != LDV_CP_PRIMITIVE)
        ldv_print_str_without_padding (")");
    }
}

void
ldv_print_body (ldv_ab_ptr body, ldv_ak a_kind)
{
  char *body_text = NULL;
  char *text = NULL;
  unsigned int number;
  const char *func_call = NULL;
  ldv_text_ptr body_with_patterns = NULL;
  const char *body_c = NULL;
  int body_p;
  bool isres_needed = true;
  ldv_list_ptr body_patterns = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;
  ldv_list_ptr pattern_params = NULL, pattern_params_cur = NULL;
  ldv_aspect_pattern_param_ptr param1 = NULL, param2 = NULL, param_cur = NULL;
  FILE *file_stream = NULL;

  /* Print '{' that starts a body for all advice kinds. */
  ldv_print_c ('{');

  ldv_print_c ('\n');

  /* Don't generate a return when a function returns void. */
  if (ldv_func_ret_type_decl && ldv_func_ret_type_decl->pps_declspecs->isvoid)
    isres_needed = false;

  /* This is done before a body patterns weaving since function argument types
     names that may be used in patterns are generated here. */
  ldv_print_types_typedefs (body, a_kind == LDV_A_AFTER && isres_needed);

  /* Print variable argument list initializations if needed. */
  if (ldv_func_va_init)
    ldv_print_str_without_padding (ldv_get_text (ldv_func_va_init));

  /* Obtain a function call text to be used. */
  if (ldv_func_call)
    func_call = ldv_get_text (ldv_func_call);

  /* Instance different body patterns if it's needed. */
  body_with_patterns = ldv_create_text ();

  /* Walk through a body text to find places body patterns to be printed. */
  for (body_c = ldv_get_body_text (body), body_p = 0; *body_c; body_c++, body_p++)
    {
      /* Copy a non pattern text of a body. */
      ldv_putc_text (*body_c, body_with_patterns);

      /* Evaluate aspect patterns if so was placed at a given point of a body
         and print corresponding text instead of it. */
      for (body_patterns = body->patterns
        ; body_patterns
        ; body_patterns = ldv_list_get_next (body_patterns))
        {
          body_pattern = (ldv_ab_aspect_pattern_ptr) ldv_list_get_data (body_patterns);

          if (body_pattern->place == body_p)
            {
              pattern = body_pattern->pattern;

              /* Aspect patterns that can be evaluated on the basis of matching
                 information and environment variable values. */
              if (ldv_evaluate_aspect_pattern (pattern, &text, &number))
                {
                  if (text)
                    {
                      ldv_puts_text (text, body_with_patterns);
                      /* Forget about evaluated text since it is used in
                         condition above. */
                      text = NULL;
                    }
                  else
                    {
                      /* Here we are interested just in text representation of
                         aspect body pattern pattern. */
                      ldv_puts_text (ldv_itoa (number), body_with_patterns);
                    }
                }
              else if (!strcmp (pattern->name, "fprintf"))
                {
                  /* First parameter specifies file where information request
                     result to be printed. */
                  pattern_params = pattern->params;
                  param1 = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params);
                  /* Second parameter specifies format string like for standard
                     printf function. */
                  pattern_params = ldv_list_get_next (pattern_params);
                  param2 = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params);

                  /* Evaluate other parameters. */
                  pattern_params = ldv_list_get_next (pattern_params);
                  /* To keep evaluated values of parameters use parameters
                     themselves since parameter evaluation may lead to either
                     string or integer, and parameters themselves may be either
                     strings or integers. */
                  for (pattern_params_cur = pattern_params
                    ; pattern_params_cur
                    ; pattern_params_cur = ldv_list_get_next (pattern_params_cur))
                    {
                      param_cur = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params_cur);

                      /* We are interested here in parameters to be evaluated. */
                      if (param_cur->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
                        {
                          if (ldv_evaluate_aspect_pattern (param_cur->aspect_pattern, &text, &number))
                            {
                              if (text)
                                {
                                  param_cur->string = text;
                                  /* Forget about evaluated text since it is
                                     used in condition above. */
                                  text = NULL;
                                }
                              else
                                param_cur->integer = number;
                            }
                          else
                            {
                              LDV_FATAL_ERROR ("body aspect pattern \"%s\" wasn't weaved", param_cur->aspect_pattern->name);
                            }
                        }
                    }

                  file_stream = ldv_open_aspect_pattern_param_file_stream (param1);
                  ldv_print_query_result (file_stream, ldv_get_aspect_pattern_value_or_string (param2), pattern_params);
                  ldv_close_file_stream (file_stream);
                }
              else if (!strcmp (pattern->name, "fprintf_var_init_list"))
                {
                  /* First parameter specifies file where information request
                     result to be printed. */
                  pattern_params = pattern->params;
                  param1 = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params);

                  file_stream = ldv_open_aspect_pattern_param_file_stream (param1);
                  ldv_print_init_list (file_stream, 0, ldv_var_init_list);
                  ldv_close_file_stream (file_stream);
                }
              else
                {
                  LDV_FATAL_ERROR ("body aspect pattern \"%s\" wasn't weaved", pattern->name);
                }

              ldv_print_info (LDV_INFO_WEAVE, "weave body aspect pattern \"%s\"", pattern->name);

              break;
            }
        }
    }

  body_text = ldv_copy_str (ldv_get_text (body_with_patterns));
  ldv_free_text (body_with_patterns);

  /* Truncate unneeded braces from a body. */
  body_text = ldv_truncate_braces (body_text);

  if (a_kind == LDV_A_AFTER)
    {
      /* Weave a special ldv variable that has the same type as a corresponding
       * function return type and initialize it with this function call. After
       * all this variable will be returned by an aspect function. */
      ldv_print_str_without_padding ("\n  ");

      if (isres_needed)
        {
          ldv_print_str (LDV_FUNC_RET_TYPE);

          ldv_print_str (LDV_FUNC_RES);

          ldv_print_info (LDV_INFO_WEAVE, "variable \"%s\" was weaved for after advice", LDV_FUNC_RES);

          ldv_print_str_without_padding ("= ");
        }

      ldv_print_str_without_padding (func_call);

      ldv_print_str_without_padding (";\n");

      ldv_print_info (LDV_INFO_WEAVE, "function call \"%s\" was weaved to initialize variable for after advice", func_call);
    }

  ldv_print_str_without_padding (body_text);
  /* In truncating braces this pointer was moved ahead exactly by 1. */
  free (body_text - 1);

  if (a_kind == LDV_A_BEFORE)
    {
      ldv_print_str ("\n ");

      /* Don't generate return statement for a function returning void. */
      if (isres_needed)
        ldv_print_str ("return");

      ldv_print_str_without_padding (func_call);

      ldv_print_str_without_padding (";\n");

      ldv_print_info (LDV_INFO_WEAVE, "return expression was weaved for before advice");
    }
  else if (a_kind == LDV_A_AFTER && isres_needed)
    {
      ldv_print_str("\n  return");

      ldv_print_str_without_padding (LDV_FUNC_RES);

      ldv_print_str_without_padding (";\n");

      ldv_print_info (LDV_INFO_WEAVE, "return expression was weaved for after advice");
    }

  /* Print '}' that finishes a body for all advice kinds. */
  ldv_print_c ('}');

  if (ldv_func_call)
    ldv_free_text (ldv_func_call);
}

void
ldv_print_decl (ldv_pps_decl_ptr decl)
{
  /* Declaration = declaration specifiers + declarator. */
  ldv_print_declspecs (decl->pps_declspecs);

  if (decl->pps_declarator)
    ldv_print_declarator (decl->pps_declarator);
}

void
ldv_print_declarator (ldv_list_ptr declarator_list)
{
  /* Declarator = pointer(opt) + direct declarator. */
  declarator_list = ldv_print_ptr (declarator_list);
  ldv_print_direct_declarator (declarator_list);
}

void
ldv_print_declspecs (ldv_pps_declspecs_ptr declspecs)
{
  /* Environment variable for printing universal type specifier '$' */
  const char* ldv_univ_print = getenv ("LDV_UNIVERSAL_PRINT");

  /* Print storage-class specifiers. Note that there may be just one
     specifier of such kind. */
  if (ldv_isstorage_class_and_function_specifiers_needed)
    {
      if (declspecs->istypedef)
        ldv_print_str ("typedef");
      else if (declspecs->isextern)
        ldv_print_str ("extern");
      else if (declspecs->isstatic && ldv_isstatic_specifier_needed)
        ldv_print_str ("static");
      else if (declspecs->isauto)
        ldv_print_str ("auto");
      else if (declspecs->isregister)
        ldv_print_str ("register");

      /* Print function specifiers. It may be or may not be. */
      if (declspecs->isinline)
        ldv_print_str ("inline");
    }

  /* Print universal type specifier '$' */
  if ((declspecs->isuniversal_type_spec) && (ldv_univ_print))
    ldv_print_str ("$");

  /* Print special declaration specifier for '..' wildcard that matches list of
     parameters of any length. */
  if (declspecs->isany_params)
    ldv_print_str ("..");

  /* Print type specifiers according to gcc names. There may be just one of the
     following type specifiers combinations. */
  if (declspecs->ischar && declspecs->isunsigned)
    ldv_print_str ("unsigned char");
  else if (declspecs->ischar && declspecs->issigned)
    ldv_print_str ("signed char");
  else if (declspecs->ischar)
    ldv_print_str ("char");
  /* Print complex types in accordance with the C standard. */
  else if (declspecs->iscomplex)
    {
      /* There is no integer complex types in the standard but gcc supports
         it. */
      if (declspecs->isint)
        ldv_print_str ("int _Complex");
      else if (declspecs->isfloat)
        ldv_print_str ("float _Complex");
      else if (declspecs->isdouble && declspecs->islong)
        ldv_print_str ("long double _Complex");
      else if (declspecs->isdouble)
        ldv_print_str ("double _Complex");
    }
  else if (declspecs->isint && declspecs->islonglong && declspecs->isunsigned)
    ldv_print_str ("long long unsigned int");
  else if (declspecs->isint && declspecs->islong && declspecs->isunsigned)
    ldv_print_str ("long unsigned int");
  else if (declspecs->isint && declspecs->isshort && declspecs->isunsigned)
    ldv_print_str ("short unsigned int");
  else if (declspecs->isint && declspecs->islonglong)
    ldv_print_str ("long long int");
  else if (declspecs->isint && declspecs->islong)
    ldv_print_str ("long int");
  else if (declspecs->isint && declspecs->isshort)
    ldv_print_str ("short int");
  else if (declspecs->isint && declspecs->isunsigned)
    ldv_print_str ("unsigned int");
  else if (declspecs->isint)
    ldv_print_str ("int");
  else if (declspecs->isfloat)
    ldv_print_str ("float");
  else if (declspecs->isdouble && declspecs->islong)
    ldv_print_str ("long double");
  else if (declspecs->isdouble)
    ldv_print_str ("double");
  else if (declspecs->isvoid)
    ldv_print_str ("void");
  else if (declspecs->isbool)
    ldv_print_str ("_Bool");
  else if (declspecs->isstruct || declspecs->isunion || declspecs->isenum || declspecs->istypedef_name)
    {
      if (declspecs->isstruct)
        ldv_print_str ("struct");
      else if (declspecs->isunion)
        ldv_print_str ("union");
      else if (declspecs->isenum)
        ldv_print_str ("enum");

      ldv_print_str (ldv_get_id_name (declspecs->type_name));
    }

  /* Print type qualifiers. There may be any type qualifiers. */
  ldv_print_qual (declspecs->isconst, declspecs->isrestrict, declspecs->isvolatile);
}

void
ldv_diag_composite_pointcut (ldv_cp_ptr c_pointcut, FILE *ldv_diag_file)
{
  if (c_pointcut->cp_kind != LDV_CP_PRIMITIVE)
    {
      fprintf (ldv_diag_file, "COMPOSITE\n");

      /* Recursive print components of the composite pointcut. */
      ldv_text_printed = ldv_create_text ();
      ldv_padding_cur = LDV_PADDING_NONE;
      ldv_print_composite_pointcut (c_pointcut);
      fprintf (ldv_diag_file, "pointcut_signature: %s\n", ldv_get_text (ldv_text_printed));

      /* Recursive print changed components of the composite pointcut. */
      ldv_text_printed = ldv_create_text ();
      ldv_padding_cur = LDV_PADDING_NONE;
      ldv_diag_recursive_composite_pointcut (c_pointcut);
      fprintf (ldv_diag_file, "%s\n", ldv_get_text (ldv_text_printed));
    }

  if (c_pointcut->cp_kind == LDV_CP_PRIMITIVE)
    ldv_diag_primitive_pointcut (c_pointcut->p_pointcut, ldv_diag_file);
}

void
ldv_print_direct_declarator (ldv_list_ptr declarator_list)
{
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_next_list = NULL;
  ldv_pps_func_arg_ptr func_arg = NULL;
  ldv_list_ptr func_arg_list = NULL;

  /* Obtain a declarator and a following element of a declarator list. */
  declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);
  declarator_next_list = ldv_list_get_next (declarator_list);

  /* Direct declarator = identifier |
                         ( + declarator + ) |
                         direct declarator + [ + array size + ]
                         direct declarator + ( + parameter list + ). */
  switch (declarator->pps_declarator_kind)
    {
    case LDV_PPS_DECLARATOR_ARRAY:
      /* Print array element types that are direct declarators. */
      ldv_print_direct_declarator (declarator_next_list);

      /* Print an array size. */
      ldv_print_space_after_pointer_star ();
      ldv_print_c ('[');

      if (declarator->pps_array_size->issize_specified)
        ldv_print_int (declarator->pps_array_size->pps_array_size);

      ldv_print_c (']');

      break;

    case LDV_PPS_DECLARATOR_FUNC:
      /* Print a direct declarator of a function if so. */
      if (declarator_next_list)
        ldv_print_direct_declarator (declarator_next_list);

      /* Function parameters enclosed in parenthesis finish a function
         declarator. */
      if (!ldv_print_space ())
        ldv_print_space_after_pointer_star ();

      ldv_print_c ('(');

      /* Generate void for an empty list of function arguments. */
      if (!declarator->func_arg_list)
        ldv_print_str_without_padding ("void");

      for (func_arg_list = declarator->func_arg_list; func_arg_list; func_arg_list = ldv_list_get_next (func_arg_list))
        {
          func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (func_arg_list);

          /* Print an any parameters wildcard if needed. */
          if (func_arg->isany_params)
            ldv_print_str_without_padding ("..");
          else
            ldv_print_decl (func_arg->pps_func_arg);

          /* Separate parameters from each other. */
          if (ldv_list_get_next (func_arg_list) || func_arg->isva)
            ldv_print_separator (',');

          /* Print the ellipsis symbol if needed. */
          if (func_arg->isva)
            ldv_print_str_without_padding ("...");
        }

      ldv_print_c (')');

      break;

    case LDV_PPS_DECLARATOR_ID:
      ldv_print_str (ldv_get_id_name (declarator->declarator_name));
      break;

    case LDV_PPS_DECLARATOR_NONE:
      /* This case takes place just for the only function parameter 'void' and
         for abstract declarators (function parameters) that have no name . */
      break;

    case LDV_PPS_DECLARATOR_PTR:
      /* In this case associativity parenthesis are needed. A declarator is
         enclosed in them. */
      if (!ldv_print_space ())
        ldv_print_space_after_pointer_star ();

      ldv_print_c ('(');

      ldv_print_declarator (declarator_list);

      ldv_print_c (')');

      break;

    default:
      LDV_FATAL_ERROR ("incorrect primitive pointcut signature declarator kind \"%d\" is used", declarator->pps_declarator_kind);
    }
}

const char *
ldv_print_func_context (ldv_i_func_ptr decl)
{
  return ldv_print_func_decl (decl->func_context);
}

const char *
ldv_print_func_context_name (ldv_i_func_ptr decl)
{
  return ldv_get_id_name (decl->func_context->name);
}

const char *
ldv_print_func_context_path (ldv_i_func_ptr decl)
{
  return ldv_print_func_path (decl->func_context);
}

const char *
ldv_print_func_decl (ldv_i_func_ptr func)
{
  ldv_pps_decl_ptr decl;

  decl = ldv_convert_internal_to_declaration (func->type, ldv_get_id_name (func->name));

  ldv_text_printed = ldv_create_text ();

  ldv_padding_cur = LDV_PADDING_NONE;

  ldv_print_decl (decl);

  return ldv_get_text (ldv_text_printed);
}

const char *
ldv_print_func_path (ldv_i_func_ptr decl)
{
  const char* path = NULL;
  char* occurrence = NULL;

  ldv_text_printed = ldv_create_text ();

  ldv_padding_cur = LDV_PADDING_NONE;

  path = decl->file_path;
  occurrence = strstr(path, ".prepared");
  if (occurrence)
    *occurrence = '\0';
  ldv_print_str (path);

  return ldv_get_text (ldv_text_printed);
}

const char *
ldv_print_func_signature (ldv_pps_decl_ptr decl)
{
  ldv_text_printed = ldv_create_text ();

  ldv_padding_cur = LDV_PADDING_NONE;

  ldv_print_decl (decl);

  return ldv_get_text (ldv_text_printed);
}

void
ldv_print_macro_name (ldv_id_ptr m_name)
{
  ldv_print_str (ldv_get_id_name (m_name));
}

void
ldv_print_macro_param (ldv_list_ptr mp_list)
{
  ldv_list_ptr macro_param_list = NULL;
  char *macro_param;

  for (macro_param_list = mp_list; macro_param_list; macro_param_list = ldv_list_get_next (macro_param_list))
    {
      macro_param = ldv_get_id_name((ldv_id_ptr)ldv_list_get_data (macro_param_list));

      if (macro_param_list == mp_list)
        {
          ldv_print_c (' ');
          ldv_print_c ('(');
        }

      ldv_print_str (macro_param);

      if (ldv_list_get_next (macro_param_list))
        {
          ldv_print_c (',');
          ldv_print_c (' ');
        }
      else
        ldv_print_c (')');
    }
}

const char *
ldv_print_macro_signature (ldv_pps_macro_ptr m_decl)
{
  ldv_text_printed = ldv_create_text ();

  ldv_padding_cur = LDV_PADDING_NONE;

  ldv_print_macro_name (m_decl->macro_name);

  if (m_decl->macro_param_list)
    ldv_print_macro_param (m_decl->macro_param_list);
  else if (m_decl->pps_macro_kind == LDV_PPS_MACRO_FUNC)
    ldv_print_str_without_padding ("()");

  return ldv_get_text (ldv_text_printed);
}

const char *
ldv_print_type_decl (ldv_i_typedecl_ptr typedecl)
{
  ldv_text_printed = ldv_create_text ();

  ldv_padding_cur = LDV_PADDING_NONE;

  switch (typedecl->itd_kind)
    {
      case LDV_ITD_ENUM:
        ldv_print_str ("enum");
        break;

      case LDV_ITD_STRUCT:
        ldv_print_str ("struct");
        break;

      case LDV_ITD_UNION:
        ldv_print_str ("union");
        break;

      default:
        LDV_FATAL_ERROR ("incorrect type declaration information kind \"%d\" is used", typedecl->itd_kind);
    }

  ldv_print_str (ldv_get_id_name (typedecl->name));

  return ldv_get_text (ldv_text_printed);
}

char *
ldv_print_var_decl (ldv_i_var_ptr var)
{
  ldv_pps_decl_ptr decl;
  char *str;

  decl = ldv_convert_internal_to_declaration (var->type, ldv_get_id_name (var->name));

  ldv_text_printed = ldv_create_text ();

  ldv_padding_cur = LDV_PADDING_NONE;

  ldv_print_decl (decl);

  ldv_free_pps_decl (decl);

  str = ldv_copy_str (ldv_get_text (ldv_text_printed));

  ldv_free_text (ldv_text_printed);

  return str;
}

void
ldv_print_int (int n)
{
  char *str = NULL;

  str = ldv_itoa (n);

  ldv_print_str (str);

  free (str);
}

ldv_list_ptr
ldv_print_ptr (ldv_list_ptr declarator_list)
{
  ldv_pps_declarator_ptr declarator;

  /* If a declarator kind is a pointer then print it with corresponding
     qualifiers. All declarators from a current pointer declarator chain are
     printed here and a function return first nonpointer declarator following
     this chain. . */
  for (; declarator_list; declarator_list = ldv_list_get_next (declarator_list))
    {
      declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

      /* Finish when a first non pointer declarator is found. */
      if (declarator->pps_declarator_kind != LDV_PPS_DECLARATOR_PTR)
        break;

      /* Print the pointer star. */
      ldv_print_space ();
      ldv_print_c ('*');

      /* Print pointer qualifiers. */
      ldv_print_qual (declarator->pps_ptr_quals->isconst
        , declarator->pps_ptr_quals->isrestrict
        , declarator->pps_ptr_quals->isvolatile);
    }

  /* This means that there is left no pointers before a direct declarator. */
  return declarator_list;
}

void
ldv_print_qual (bool isconst, bool isrestrict, bool isvolatile)
{
  /* Do nothing if there is no qualifiers. */
  if (!isconst && !isrestrict && !isvolatile)
    return ;

  ldv_print_space_after_pointer_star ();

  if (isconst)
    ldv_print_str ("const");

  if (isrestrict)
    ldv_print_str ("restrict");

  if (isvolatile)
    ldv_print_str ("volatile");
}

void
ldv_print_separator (unsigned int c)
{
  /* Print a separator symbol itself (most likely it's ','). */
  ldv_print_c (c);
  /* And the nice indentation space after it. */
  ldv_print_c (' ');
}

void
ldv_diag_primitive_pointcut (ldv_pp_ptr p_pointcut, FILE *ldv_diag_file)
{
  ldv_primitive_pointcut_signature_decl original_declaration;
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;
  ldv_pps_func_arg_ptr func_arg = NULL;
  ldv_list_ptr func_arg_list = NULL;
  ldv_pps_decl_ptr pps_declaration = NULL;
  ldv_pps_ptr pp_signature = NULL;
  const char* format = NULL;
  const char* ldv_diag_text = NULL;

  pp_signature = p_pointcut->pp_signature;
  pps_declaration = pp_signature->pps_declaration;

  if (p_pointcut->pp_signature->pps_kind == LDV_PPS_DECL)
    {
      fprintf (ldv_diag_file, "PRIMITIVE_FUNCTION\n");

      ldv_diag_text = ldv_print_func_signature (pps_declaration);
      fprintf (ldv_diag_file, "pointcut_signature: %s\n", ldv_diag_text);

      format = p_pointcut->pp_kind == LDV_PP_CALL ? "declare_func(%s) || execution(%s)\n" : "execution(%s)\n";
      fprintf (ldv_diag_file, format, ldv_diag_text, ldv_diag_text);

      original_declaration = *pps_declaration;

      pps_declaration = ldv_diag_replace_return_type (pps_declaration);
      ldv_diag_text = ldv_print_func_signature (pps_declaration);
      fprintf (ldv_diag_file, format, ldv_diag_text, ldv_diag_text);
      *pps_declaration = original_declaration;


      for (declarator_list = pps_declaration->pps_declarator; declarator_list; declarator_list = ldv_list_get_next (declarator_list))
        {
          declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

          for (func_arg_list = declarator->func_arg_list; func_arg_list; func_arg_list = ldv_list_get_next (func_arg_list))
            {
              func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (func_arg_list);
              func_arg->isany_params = 1;
            }
        }

      ldv_diag_text = ldv_print_func_signature (pps_declaration);
      fprintf (ldv_diag_file, format, ldv_diag_text, ldv_diag_text);

      pps_declaration = ldv_diag_replace_return_type (pps_declaration);

      for (declarator_list = pps_declaration->pps_declarator; declarator_list; declarator_list = ldv_list_get_next (declarator_list))
        {
          declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

          for (func_arg_list = declarator->func_arg_list; func_arg_list; func_arg_list = ldv_list_get_next (func_arg_list))
            {
              func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (func_arg_list);
              func_arg->isany_params = 1;
            }
        }

      ldv_diag_text = ldv_print_func_signature (pps_declaration);
      fprintf (ldv_diag_file, format, ldv_diag_text, ldv_diag_text);

      *pps_declaration = original_declaration;
    }
  else if (pp_signature->pps_kind == LDV_PPS_DEFINE)
    {
      fprintf (ldv_diag_file, "PRIMITIVE_MACROS\n");

      ldv_diag_text = ldv_print_macro_signature (pp_signature->pps_macro);
      fprintf (ldv_diag_file, "pointcut_signature: %s\n", ldv_diag_text);

      fprintf (ldv_diag_file, "define(%s)\n", ldv_diag_text);
    }
}

ldv_pps_decl_ptr
ldv_diag_replace_return_type (ldv_pps_decl_ptr pps_declaration)
{
  ldv_i_func_ptr i_func_sign = ldv_convert_func_signature_to_internal (pps_declaration);
  i_func_sign->type->ret_type->it_kind = LDV_IT_PRIMITIVE;
  i_func_sign->type->ret_type->primitive_type = ldv_create_declspecs ();
  i_func_sign->type->ret_type->primitive_type->isuniversal_type_spec = 1;
  pps_declaration = ldv_convert_internal_to_declaration (i_func_sign->type, ldv_get_id_name(i_func_sign->name));
  return pps_declaration;
}

void
ldv_diagnostics (void)
{
  ldv_list_ptr n_pointcut_list = NULL;
  ldv_np_ptr n_pointcut = NULL;

  const char* ldv_diag_env = getenv ("LDV_DIAGNOSTICS");
  FILE *ldv_diag_file;

  if (ldv_diag_env)
  {
    if((ldv_diag_file = fopen (ldv_diag_env, "a")) == NULL)
      {
        LDV_FATAL_ERROR ("Failed to open file %s", ldv_diag_env);
      }

    for (n_pointcut_list = ldv_n_pointcut_list; n_pointcut_list ; n_pointcut_list = ldv_list_get_next (n_pointcut_list))
    {
      n_pointcut = (ldv_np_ptr) ldv_list_get_data (n_pointcut_list);

      fprintf (ldv_diag_file, "pointcut %s\n", n_pointcut->p_name);
      ldv_diag_composite_pointcut (n_pointcut->c_pointcut, ldv_diag_file);
    }

    fclose (ldv_diag_file);
  }
}

bool
ldv_print_space (void)
{
  if (ldv_padding_cur == LDV_PADDING_BEFORE)
    {
      ldv_print_c (' ');
      return true;
    }

  return false;
}

bool
ldv_print_space_after_pointer_star (void)
{
  /* If a previous symbol was the pointer star then print an indentation space
     between it and following symbol or identifier. */
  if (ldv_text_printed && *(LDV_STR_END (ldv_get_text (ldv_text_printed))) == '*')
    {
      ldv_print_c (' ');
      return true;
    }

  return false;
}

void
ldv_print_str (const char *str)
{
  /* Put an indentation space if it's needed. */
  ldv_print_space ();

  ldv_puts_text (str, ldv_text_printed);

  /* An indentation space is used by default after a string (i.e. one or more
     identifiers). */
  ldv_padding_cur = LDV_PADDING_BEFORE;
}

void
ldv_print_str_without_padding (const char *str)
{
  ldv_print_str (str);

  /* Padding is unneeded in this case. */
  ldv_padding_cur = LDV_PADDING_NONE;
}

void
ldv_print_types_typedefs (ldv_ab_ptr body, bool isret_type_needed)
{
  ldv_pps_decl_ptr func_arg_type_decl = NULL;
  ldv_list_ptr func_arg_type_decl_list = NULL;
  const char *arg_type_name = NULL;
  ldv_id_ptr arg_type_name_aux;
  char *arg_type_name_numb_aux = NULL;
  unsigned int arg_type_numb;
  ldv_str_ptr str = NULL;
  ldv_list_ptr body_patterns = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;

  ldv_func_arg_type_name_list = NULL;

  /* Don't print storage class and function specifiers for generated typedefs
     on a function return type and arguments types. */
  ldv_isstorage_class_and_function_specifiers_needed = false;

  /* Print a function return type typedef if there is $ret_type pattern
     in the advice body. Otherwise it will be printed just for nonvoid
     function return types for after advices. */
  for (body_patterns = body->patterns
      ; body_patterns
      ; body_patterns = ldv_list_get_next (body_patterns))
    {
      body_pattern = (ldv_ab_aspect_pattern_ptr) ldv_list_get_data (body_patterns);
      pattern = body_pattern->pattern;

      if (!strcmp (pattern->name, "ret_type"))
        {
          isret_type_needed = true;
          break;
        }
    }

  /* Print a function return type typedef just if it is required. */
  if (ldv_func_ret_type_decl && isret_type_needed)
    {
      ldv_print_str ("  typedef");
      ldv_add_id_declarator (ldv_func_ret_type_decl, LDV_FUNC_RET_TYPE);
      ldv_print_decl (ldv_func_ret_type_decl);
      ldv_delete_id_declarator (ldv_func_ret_type_decl->pps_declarator);
      ldv_print_c (';');
      ldv_print_c ('\n');
    }

  /* Print function argument typedefs (if there are appropriate
     $arg_typeN patterns) and store them to a list. */
  /* Walk through all function argument types declarations. */
  for (func_arg_type_decl_list = ldv_func_arg_type_decl_list, arg_type_numb = 1
    ; func_arg_type_decl_list
    ; func_arg_type_decl_list = ldv_list_get_next (func_arg_type_decl_list), arg_type_numb++)
    {
      func_arg_type_decl = (ldv_pps_decl_ptr) ldv_list_get_data (func_arg_type_decl_list);

      /* Create a function argument type typedef name. */
      arg_type_name_aux = ldv_create_id ();

      ldv_puts_id (LDV_FUNC_ARG_TYPE, arg_type_name_aux);

      arg_type_name_numb_aux = ldv_itoa (arg_type_numb);

      ldv_puts_id (arg_type_name_numb_aux, arg_type_name_aux);

      free (arg_type_name_numb_aux);

      arg_type_name = ldv_get_id_name (arg_type_name_aux);

      for (body_patterns = body->patterns
      ; body_patterns
      ; body_patterns = ldv_list_get_next (body_patterns))
        {
          body_pattern = (ldv_ab_aspect_pattern_ptr) ldv_list_get_data (body_patterns);
          pattern = body_pattern->pattern;

          if ((!strcmp (pattern->name, "arg_type")) && (pattern->arg_numb == arg_type_numb))
            {
              ldv_print_str ("  typedef");
              ldv_add_id_declarator (func_arg_type_decl, arg_type_name);
              ldv_print_decl (func_arg_type_decl);
              ldv_delete_id_declarator (func_arg_type_decl->pps_declarator);
              ldv_print_c (';');
              ldv_print_c ('\n');

              break;
            }
        }

      ldv_free_pps_decl (func_arg_type_decl);

      str = ldv_create_string ();

      ldv_puts_string (arg_type_name, str);

      ldv_free_id (arg_type_name_aux);

      ldv_list_push_back (&ldv_func_arg_type_name_list, str);
    }

  ldv_isstorage_class_and_function_specifiers_needed = true;
}

void
ldv_store_func_arg_type_decl_list (ldv_i_type_ptr func_type)
{
  ldv_i_type_ptr arg_type = NULL;
  ldv_list_ptr param_list = NULL;
  ldv_pps_decl_ptr func_arg_type_decl_new = NULL;

  ldv_func_arg_type_decl_list = NULL;

  /* Walk through function arguments. */
  for (param_list = func_type->param
    ; param_list
    ; param_list = ldv_list_get_next (param_list))
    {
      arg_type = ((ldv_i_param_ptr) ldv_list_get_data (param_list))->type;

      func_arg_type_decl_new = ldv_convert_internal_to_declaration (arg_type, NULL);

      ldv_list_push_back (&ldv_func_arg_type_decl_list, func_arg_type_decl_new);
    }
}

void
ldv_weave_advice (expanded_location *open_brace, expanded_location *close_brace)
{
  ldv_ak a_kind;
  char *aspected_name = NULL;
  ldv_decl_for_print_ptr aspect_func_decl_for_print_new = NULL
    , func_decl_for_print_new = NULL
    , typedecl_for_print_new = NULL;
  char *body_text = NULL;
  ldv_i_func_ptr func_aspect = NULL, func_source = NULL;
  const char *func_name = NULL;
  ldv_pps_decl_ptr decl = NULL;
  ldv_ppk pp_kind;
  ldv_str_ptr str = NULL;
  ldv_list_ptr str_list = NULL;
  ldv_i_var_ptr var = NULL;
  ldv_i_param_ptr param_new = NULL;
  ldv_str_ptr var_param_name = NULL;

  /* Check whether something was matched. */
  if (!ldv_i_match)
    return ;

  a_kind = ldv_i_match->a_definition->a_declaration->a_kind;
  pp_kind = ldv_i_match->p_pointcut->pp_kind;

  /* In case of information request do not perform weaving just "print" advice
     body that implicitly invokes evaluation of all $fprintf. */
  if (a_kind == LDV_A_INFO && (pp_kind == LDV_PP_INIT_GLOBAL || pp_kind == LDV_PP_INIT_LOCAL))
    {
      ldv_text_printed = ldv_create_text ();
      ldv_var_name = ldv_get_id_name (ldv_i_match->i_var_aspect->name);
      if (ldv_i_match->i_var_aspect->type->it_kind == LDV_IT_PRIMITIVE && ldv_i_match->i_var_aspect->type->primitive_type->type_name)
        ldv_var_type_name = ldv_get_id_name (ldv_i_match->i_var_aspect->type->primitive_type->type_name);
      ldv_var_init_list = ldv_i_match->i_var->initializer_list;
      ldv_print_body (ldv_i_match->a_definition->a_body, a_kind);
      ldv_var_name = NULL;
      ldv_var_type_name = NULL;
      ldv_var_init_list = NULL;
      return;
    }
  else if (a_kind == LDV_A_INFO && (pp_kind == LDV_PP_DECLARE_FUNC || pp_kind == LDV_PP_EXECUTION || pp_kind == LDV_PP_CALL))
    {
       ldv_text_printed = ldv_create_text ();
       ldv_func_signature = ldv_i_match->i_func;
       ldv_func_name = ldv_get_id_name (ldv_func_signature->name);
       ldv_print_body (ldv_i_match->a_definition->a_body, a_kind);
       ldv_func_signature = NULL;
       ldv_func_name = NULL;
       return;
    }

  ldv_padding_cur = LDV_PADDING_NONE;

  /* Weave an advice in depend on a matched entity kind and a ldv stage. */
  switch (ldv_i_match->i_kind)
    {
    case LDV_I_FUNC:
      func_aspect = ldv_i_match->i_func_aspect;
      func_source = ldv_i_match->i_func;

      if (ldv_instrumentation ())
        {
          ldv_func_call = ldv_create_text ();

          if (!ldv_func_defs_for_print)
            ldv_func_defs_for_print = ldv_create_text ();

          aspect_func_decl_for_print_new = ldv_create_decl_for_print ();
          func_decl_for_print_new = ldv_create_decl_for_print ();

          /* Place (aspect) function declarations immediately after the end of
             a function definition, i.e. after a close brace. */
          if (!close_brace)
            {
              LDV_FATAL_ERROR ("for function \"%s\" location of close brace isn't specified", ldv_get_id_name (func_source->name));
            }

          func_decl_for_print_new->file = (*close_brace).file;
          aspect_func_decl_for_print_new->file = (*close_brace).file;
          func_decl_for_print_new->line = (*close_brace).line;
          aspect_func_decl_for_print_new->line = (*close_brace).line;
          func_decl_for_print_new->column = (*close_brace).column;
          aspect_func_decl_for_print_new->column = (*close_brace).column;

          /* Create list of function parameter names on the basis of a source
             function declaration and a pointcut function signature. */
          ldv_create_aux_func_params (func_source->type, func_aspect->type);

          aspected_name = ldv_create_aspected_name (ldv_get_id_name (func_aspect->name));

          /* Create a function declaration. */
          ldv_text_printed = ldv_create_text ();

          ldv_putc_text ('\n', ldv_text_printed);

          if (pp_kind == LDV_PP_EXECUTION)
            {
              func_name = aspected_name;
              ldv_aspect_func_name = func_name;
            }
          else if (pp_kind == LDV_PP_CALL)
            {
              func_name = ldv_get_id_name (func_aspect->name);
              ldv_func_name = func_name;
            }

          decl = ldv_convert_internal_to_declaration (func_aspect->type, func_name);

          ldv_print_decl (decl);

          ldv_free_pps_decl (decl);

          ldv_puts_text (";\n", ldv_text_printed);

          ldv_puts_text (ldv_get_text (ldv_text_printed), func_decl_for_print_new->decl);

          ldv_free_text (ldv_text_printed);

          ldv_list_push_back (&ldv_decl_for_print_list, func_decl_for_print_new);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" function declaration for \"%s\" function weaving", func_name, ldv_get_id_name (func_aspect->name));

          /* Create an aspect function declaration. */
          ldv_text_printed = ldv_create_text ();

          ldv_putc_text ('\n', ldv_text_printed);

          if (pp_kind == LDV_PP_EXECUTION)
            {
              func_name = ldv_get_id_name (func_aspect->name);
              ldv_func_name = func_name;
            }
          else if (pp_kind == LDV_PP_CALL)
            {
              func_name = aspected_name;
              ldv_aspect_func_name = func_name;
            }

          decl = ldv_convert_internal_to_declaration (func_aspect->type, func_name);

          ldv_print_decl (decl);

          ldv_free_pps_decl (decl);

          ldv_puts_text (";\n", ldv_text_printed);

          ldv_puts_text (ldv_get_text (ldv_text_printed), aspect_func_decl_for_print_new->decl);

          ldv_free_text (ldv_text_printed);

          ldv_list_push_back (&ldv_decl_for_print_list, aspect_func_decl_for_print_new);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" aspect function declaration for \"%s\" function weaving", func_name, ldv_get_id_name (func_aspect->name));

          /* Create a function call. */
          ldv_text_printed = ldv_create_text ();

          if (pp_kind == LDV_PP_EXECUTION)
            func_name = aspected_name;
          else if (pp_kind == LDV_PP_CALL)
            func_name = ldv_get_id_name (func_aspect->name);

          ldv_puts_text (func_name, ldv_text_printed);

          ldv_putc_text ('(', ldv_text_printed);

          ldv_putc_text (')', ldv_text_printed);

          ldv_puts_text (ldv_get_text (ldv_text_printed), ldv_func_call);

          for (str_list = ldv_func_param_list
            ; str_list
            ; str_list = ldv_list_get_next (str_list))
            {
              str = (ldv_str_ptr) ldv_list_get_data (str_list);

              ldv_puts_text (ldv_get_str (str), ldv_text_printed);

              if (ldv_list_get_next (str_list))
                ldv_puts_text (", ", ldv_text_printed);
            }
          ldv_free_text (ldv_text_printed);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" function call for weaving", func_name);

          /* Create an aspect function definition. */
          ldv_text_printed = ldv_create_text ();

          ldv_putc_text ('\n', ldv_text_printed);

          if (pp_kind == LDV_PP_EXECUTION)
            func_name = ldv_get_id_name (func_aspect->name);
          else if (pp_kind == LDV_PP_CALL)
            func_name = aspected_name;

          decl = ldv_convert_internal_to_declaration (func_aspect->type, func_name);

          ldv_print_decl (decl);

          ldv_free_pps_decl (decl);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" aspect function declaration for \"%s\" function weaving", func_name, ldv_get_id_name (func_aspect->name));

          /* Store an information on a function return type and function
             arguments types that will be used in a body patterns weaving. */
          ldv_func_ret_type_decl = ldv_convert_internal_to_declaration (func_aspect->type->ret_type, NULL);

          ldv_store_func_arg_type_decl_list (func_aspect->type);

          ldv_putc_text ('\n', ldv_text_printed);

          ldv_print_body (ldv_i_match->a_definition->a_body, a_kind);

          ldv_putc_text ('\n', ldv_text_printed);

          ldv_puts_text (ldv_get_text (ldv_text_printed), ldv_func_defs_for_print);

          free (aspected_name);
          ldv_free_pps_decl (ldv_func_ret_type_decl);
          ldv_list_delete_all (ldv_func_arg_type_decl_list);

          for (str_list = ldv_func_arg_type_name_list
            ; str_list
            ; str_list = ldv_list_get_next (str_list))
            {
              str = (ldv_str_ptr) ldv_list_get_data (str_list);
              ldv_free_str (str);
            }

          ldv_list_delete_all (ldv_func_arg_type_name_list);

          for (str_list = ldv_func_param_list
            ; str_list
            ; str_list = ldv_list_get_next (str_list))
            {
              str = (ldv_str_ptr) ldv_list_get_data (str_list);
              ldv_free_str (str);
            }

          ldv_list_delete_all (ldv_func_param_list);
          ldv_free_text (ldv_text_printed);

          /* Remove auxiliary entities. */
          ldv_aspect_func_name = NULL;
          ldv_func_name = NULL;
          ldv_func_ret_type_decl = NULL;
          ldv_func_arg_type_decl_list = NULL;
          ldv_func_arg_type_name_list = NULL;
          ldv_func_va_init = NULL;
        }
      else if (ldv_compilation ())
        {
          /* Change a source function name for an execution join point just one
             time. */
          if (pp_kind == LDV_PP_EXECUTION && ldv_isweaved (ldv_get_id_name (func_aspect->name), false))
            return ;

          ldv_weave_func_source (func_aspect, pp_kind);
        }

      ldv_free_info_func (func_source);
      ldv_free_info_func (func_aspect);

      break;

    case LDV_I_TYPE:
      /* A type declaration weaving is needed just at the third ldv stage. */
      if (ldv_instrumentation ())
      {
        typedecl_for_print_new = ldv_create_decl_for_print ();

        /* Print type declaration extensions in depend on an advice kind. */
        switch (a_kind)
          {
          case LDV_A_AFTER:
            typedecl_for_print_new->file = (*close_brace).file;
            typedecl_for_print_new->line = (*close_brace).line;
            /* Place an extension directly before '}' that finishes a type
               declaration. */
            typedecl_for_print_new->column = (*close_brace).column - 1;

            break;

          case LDV_A_BEFORE:
            typedecl_for_print_new->file = (*open_brace).file;
            typedecl_for_print_new->line = (*open_brace).line;
            /* Place an extension directly after '{' that starts a typ
               declaration. */
            typedecl_for_print_new->column = (*open_brace).column;

            break;

          default:
            LDV_FATAL_ERROR ("incorrect advice kind \"%d\" is used", a_kind);
          }

        body_text = ldv_copy_str (ldv_get_body_text (ldv_i_match->a_definition->a_body));

        body_text = ldv_truncate_braces (body_text);

        ldv_puts_text (body_text, typedecl_for_print_new->decl);

        ldv_list_push_back (&ldv_decl_for_print_list, typedecl_for_print_new);

        ldv_print_info (LDV_INFO_WEAVE, "extension for type declaration weaving was created");
      }

      break;

    case LDV_I_VAR:
      var = ldv_i_match->i_var;

      if (ldv_instrumentation ())
        {
          ldv_func_call = ldv_create_text ();

          if (!ldv_func_defs_for_print)
            ldv_func_defs_for_print = ldv_create_text ();

          aspect_func_decl_for_print_new = ldv_create_decl_for_print ();

          /* Add information on a location where an aspect function declaration
             will be placed. */
          /* Place an aspect function declaration directly before a function
             definition for local variables and parameters declarations. */
          if (var->func_context)
            {
              aspect_func_decl_for_print_new->file = ldv_decl_beginning_location.file;
              aspect_func_decl_for_print_new->line = ldv_decl_beginning_location.line;
              /* Place an aspect function declaration directly before a first
                 declaration specifier of a function return type. */
              aspect_func_decl_for_print_new->column = ldv_decl_beginning_location.column - 1;
            }
          /* Place an aspect function declaration after a variable declaration
             for global variables. */
          else
            {
              aspect_func_decl_for_print_new->file = (*open_brace).file;
              aspect_func_decl_for_print_new->line = (*open_brace).line;
              aspect_func_decl_for_print_new->column = (*open_brace).column;
            }

          /* A variable signature has the form: 'variable type + variable name'.
             An aspect function declaration and a declaration of definition will
             have the form: 'variable type + aspected variable name + ( +
             variable type + variable argument name)'.*/

          /* Create aspect function declaration. */
          func_aspect = ldv_create_info_func ();

          func_aspect->name = ldv_create_id ();
          ldv_puts_id (ldv_create_aspected_name (ldv_get_id_name (var->name)), func_aspect->name);

          ldv_aspect_func_name = ldv_get_id_name (func_aspect->name);

          func_aspect->type = ldv_create_info_type ();

          func_aspect->type->it_kind = LDV_IT_FUNC;

          func_aspect->type->ret_type = var->type;

          param_new = ldv_create_info_param ();

          param_new->name = ldv_create_id ();
          ldv_puts_id (LDV_VAR_ARG_NAME, param_new->name);

          param_new->type = var->type;

          ldv_list_push_back (&func_aspect->type->param, param_new);

          ldv_text_printed = ldv_create_text ();

          ldv_putc_text ('\n', ldv_text_printed);

          decl = ldv_convert_internal_to_declaration (func_aspect->type, ldv_get_id_name (func_aspect->name));

          ldv_print_decl (decl);

          ldv_free_pps_decl (decl);

          ldv_puts_text (";\n", ldv_text_printed);

          ldv_puts_text (ldv_get_text (ldv_text_printed), aspect_func_decl_for_print_new->decl);

          ldv_list_push_back (&ldv_decl_for_print_list, aspect_func_decl_for_print_new);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" aspect function declaration for \"%s\" variable weaving", ldv_get_id_name (func_aspect->name), ldv_get_id_name (var->name));

          /* Create a function call. */
          ldv_text_printed = ldv_create_text ();

          ldv_puts_text (LDV_VAR_ARG_NAME, ldv_text_printed);

          ldv_puts_text (ldv_get_text (ldv_text_printed), ldv_func_call);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" variable name reference for weaving", LDV_VAR_ARG_NAME);

          /* Create an aspect function definition. */
          ldv_text_printed = ldv_create_text ();

          decl = ldv_convert_internal_to_declaration (func_aspect->type, ldv_get_id_name (func_aspect->name));

          ldv_print_decl (decl);

          ldv_free_pps_decl (decl);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" aspect function declaration for \"%s\" variable weaving", ldv_get_id_name (func_aspect->name), ldv_get_id_name (var->name));

          /* Store a function argument name. */
          var_param_name = ldv_create_string ();
          ldv_puts_string (LDV_VAR_ARG_NAME, var_param_name);
          ldv_list_push_back (&ldv_func_param_list, var_param_name);

          /* Store information on a function return type and a function argument
             type that will be used in a body patterns weaving. */
          ldv_func_ret_type_decl = ldv_convert_internal_to_declaration (func_aspect->type->ret_type, NULL);

          ldv_store_func_arg_type_decl_list (func_aspect->type);

          ldv_putc_text ('\n', ldv_text_printed);

          ldv_print_body (ldv_i_match->a_definition->a_body, a_kind);

          ldv_putc_text ('\n', ldv_text_printed);

          ldv_puts_text (ldv_get_text (ldv_text_printed), ldv_func_defs_for_print);

          /* Remove auxiliary entities. */
          ldv_aspect_func_name = NULL;
          ldv_func_ret_type_decl = NULL;
          ldv_func_arg_type_decl_list = NULL;
          ldv_func_arg_type_name_list = NULL;
        }
      else if (ldv_compilation ())
        ldv_weave_var_source (var, pp_kind);

      break;

    default:
      LDV_FATAL_ERROR ("incorrect information kind \"%d\" is used", ldv_i_match->i_kind);
    }

  ldv_free_info_match (ldv_i_match);
}

void
ldv_weave_func_source (ldv_i_func_ptr func, ldv_ppk pp_kind)
{
  char *aspected_name = NULL;
  tree id = NULL_TREE;

  /* Obtain a corresponding aspected name for a function. */
  aspected_name = ldv_create_aspected_name (ldv_get_id_name (func->name));

  /* Obtain a function declaratation with a corresponding name. */
  if (pp_kind == LDV_PP_EXECUTION)
    {
      /* Change a source function name for an execution join point. */
      /* Override an existing declaration name since its identifier has constant
         qualifier. */
    /*  id = DECL_NAME (ldv_func_decl_matched); */
      DECL_NAME (ldv_func_decl_matched) = get_identifier (aspected_name);
      /* Release a binding between a name and an entity. */
    /*  ldv_release_binding (id); */

      ldv_print_info (LDV_INFO_WEAVE, "change source function name in function definition from \"%s\" to \"%s\" for execution join point", ldv_get_id_name (func->name), aspected_name);
    }
  else if (pp_kind == LDV_PP_CALL)
    {
      ldv_print_info (LDV_INFO_WEAVE, "change source function name in function call from \"%s\" to \"%s\" for call join point", ldv_get_id_name (func->name), aspected_name);

      /* Get an identifier corresponding to an aspected name - it's an
         identifier of an aspect function declaration. */
      id = get_identifier (aspected_name);

      /* Obtain an aspect function declaration through an identifier. */
      ldv_func_called_matched = lookup_name (id);
    }

  free (aspected_name);

  return ;
}

void
ldv_weave_var_source (ldv_i_var_ptr var, ldv_ppk pp_kind)
{
  const char *aspected_name = NULL;
  tree id = NULL_TREE;

  /* Obtain a corresponding aspected name for a function. */
  aspected_name = ldv_create_aspected_name (ldv_get_id_name (var->name));

  /* Obtain a function declaratation with a corresponding name. */
  if (pp_kind == LDV_PP_SET
    || pp_kind == LDV_PP_SET_LOCAL || pp_kind == LDV_PP_SET_GLOBAL
    || pp_kind == LDV_PP_GET
    || pp_kind == LDV_PP_GET_LOCAL || pp_kind == LDV_PP_GET_GLOBAL)
    {
      ldv_print_info (LDV_INFO_WEAVE, "insert function call \"%s\" for variable \"%s\" for set or get join point", aspected_name, ldv_get_id_name (var->name));

      /* Get an identifier corresponding to an aspect name - it's an identifier
         of an aspect function declaration. */
      id = get_identifier (aspected_name);

      /* Obtain an aspect function declaration through an identifier. */
      ldv_func_called_matched = lookup_name (id);
    }

  return ;
}
