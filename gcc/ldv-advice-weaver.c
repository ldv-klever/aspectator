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
#include "builtins.h"
#include "c/c-lang.h"

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


#define LDV_FUNC_ARG_NAME           "ldv_func_arg"
#define LDV_FUNC_ARG_TYPE           "ldv_func_arg_type"
#define LDV_FUNC_RET_TYPE           "ldv_func_ret_type"
#define LDV_FUNC_RES                "ldv_func_res"
#define LDV_VAR_ARG_NAME            "ldv_var_arg"

#define LDV_IDENTIFIER_POINTER(NODE) (IDENTIFIER_NODE_CHECK (NODE)->identifier.id.str)


/* These constants say where indentation spaces are needed. */
typedef enum { LDV_PADDING_AFTER, LDV_PADDING_BEFORE, LDV_PADDING_NONE } ldv_padding;


expanded_location ldv_decl_beginning_location;
/* Hash table to store auxiliary function names. */
static htab_t ldv_aux_func_names;


static const char *ldv_aspect_func_name = NULL;
static ldv_list_ptr ldv_func_arg_type_decl_list = NULL;
static ldv_list_ptr ldv_func_arg_type_name_list = NULL;
static ldv_text_ptr ldv_func_call = NULL;
static const char *ldv_func_name = NULL;
static const char *ldv_func_ptr_name = NULL;
static ldv_list_ptr ldv_func_param_list = NULL;
static ldv_pps_decl_ptr ldv_func_ret_type_decl = NULL;
static ldv_text_ptr ldv_func_va_init = NULL;
static const char *ldv_var_name = NULL;
static const char *ldv_var_type_name = NULL;
static tree ldv_var_initializer_tree = NULL;
static bool ldv_isstatic_specifier_needed = true;
static bool ldv_isstorage_class_and_function_specifiers_needed = true;
static ldv_list_ptr ldv_name_weaved_list = NULL;
static ldv_padding ldv_padding_cur = LDV_PADDING_NONE;
static ldv_text_ptr ldv_text_printed = NULL;
static ldv_i_func_ptr ldv_func_signature = NULL;
static ldv_i_var_ptr ldv_var_signature = NULL;
static ldv_i_typedecl_ptr ldv_type_signature = NULL;
static const char *ldv_func_decl = NULL;
static const char *ldv_var_decl = NULL;
static const char *ldv_type_decl = NULL;


static void ldv_add_id_declarator (ldv_pps_decl_ptr, const char *);
static void ldv_create_aux_func_params (ldv_i_type_ptr, ldv_i_type_ptr);
static const char *ldv_create_aux_func_param (unsigned int);
static ldv_decl_for_print_ptr ldv_create_decl_for_print (void);
static void ldv_delete_id_declarator (ldv_list_ptr);
static void ldv_diag_composite_pointcut (ldv_cp_ptr, FILE *);
static void ldv_diag_primitive_pointcut (ldv_pp_ptr, FILE *);
static void ldv_diag_recursive_composite_pointcut (ldv_cp_ptr);
static ldv_pps_decl_ptr ldv_diag_replace_return_type (ldv_pps_decl_ptr);
static LDV_EVALUATE_ASPECT_PATTERN_FUNC ldv_evaluate_aspect_pattern;
static char *ldv_get_actual_arg_func_names (void);
static const char *ldv_get_arg_name (unsigned int);
static const char *ldv_get_arg_sign (unsigned int);
static const char *ldv_get_arg_type_name (unsigned int);
static int ldv_get_arg_size (unsigned int);
static const char *ldv_get_arg_value (unsigned int);
static const char *ldv_get_body_sign (ldv_ab_ptr);
static const char *ldv_get_param_name (unsigned int);
static char *ldv_print_arg_type_str (unsigned int);
static void ldv_print_c (unsigned int);
static void ldv_print_composite_pointcut (ldv_cp_ptr);
static void ldv_print_body (ldv_ab_ptr, ldv_ak);
static void ldv_print_decl (ldv_pps_decl_ptr);
static void ldv_print_declarator (ldv_list_ptr);
static void ldv_print_declspecs (ldv_pps_declspecs_ptr);
static void ldv_print_direct_declarator (ldv_list_ptr);
static void ldv_print_int (int);
static const char *ldv_print_func_context (ldv_i_func_ptr);
static const char *ldv_print_func_name (ldv_i_func_ptr);
static const char *ldv_print_func_signature (ldv_pps_decl_ptr);
static void ldv_print_initializer (ldv_i_initializer_ptr, ldv_text_ptr, unsigned int);
static const char *ldv_print_line_number (unsigned int);
static void ldv_print_macro_name (ldv_id_ptr);
static void ldv_print_macro_param (ldv_list_ptr);
static const char *ldv_print_macro_signature (ldv_pps_macro_ptr);
static ldv_list_ptr ldv_print_ptr (ldv_list_ptr);
static char *ldv_print_ret_type_str (void);
static void ldv_print_qual (bool, bool, bool);
static void ldv_print_separator (unsigned int);
static bool ldv_print_space (void);
static bool ldv_print_space_after_pointer_star (void);
static void ldv_print_str (const char *);
static void ldv_print_str_without_padding (const char *);
static void ldv_print_types_typedefs (ldv_ab_ptr, bool);
static void ldv_print_var_init_values (ldv_i_initializer_ptr, ldv_text_ptr);
static void ldv_store_func_arg_type_decl_list (ldv_i_type_ptr);
static void ldv_free_func_arg_type_decl_list (void);
static void ldv_weave_func_source (const char *, const char *, ldv_ppk);
static void ldv_weave_var_source (const char *, const char *, ldv_ppk);


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

  internal_error ("can't add identifier declarator to the end of declarator chain");
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

        ldv_puts_text ("\n  __builtin_va_list ", ldv_func_va_init);
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

  internal_error ("can't remove identifier declarator from the end of declarator chain");
}


/* Aspect patterns that can be evaluated basing on matching information. */
void
ldv_evaluate_aspect_pattern (ldv_aspect_pattern_ptr pattern, const char **text, unsigned int *number, const char **sign)
{
  const char *func_arg = NULL;
  int func_arg_size;
  ldv_text_ptr ldv_text = NULL;
  ldv_i_initializer_ptr ldv_var_initializer = NULL;

  if (!strcmp (pattern->name, "arg"))
    *text = ldv_copy_str (ldv_get_param_name (pattern->arg_numb));
  else if (!strcmp (pattern->name, "arg_type"))
    *text = ldv_copy_str (ldv_get_arg_type_name (pattern->arg_numb));
  else if (!strcmp (pattern->name, "arg_type_str"))
    *text = ldv_print_arg_type_str (pattern->arg_numb);
  else if (!strcmp (pattern->name, "arg_name"))
    *text = ldv_copy_str (ldv_get_arg_name (pattern->arg_numb));
  else if (!strcmp (pattern->name, "arg_size"))
    {
      func_arg_size = ldv_get_arg_size (pattern->arg_numb);

      /* Print stub '-1' when an argument size isn't specified. */
      if (func_arg_size == -1)
        {
          *text = ldv_copy_str ("-1");
          ldv_print_info (LDV_INFO_WEAVE, "generate stub \"-1\" for aspect pattern \"%s\"", pattern->name);
        }
      else
        *text = ldv_itoa ((unsigned int) func_arg_size);
    }
  else if (!strcmp (pattern->name, "arg_value"))
    {
      func_arg = ldv_get_arg_value (pattern->arg_numb);

      /* Print stub 'NULL' when an argument size isn't specified. */
      if (func_arg == NULL)
        {
          *text = ldv_copy_str ("0");
          ldv_print_info (LDV_INFO_WEAVE, "generate stub \"0\" for aspect pattern \"%s\"", pattern->name);
        }
      else
        *sign = *text = ldv_copy_str (func_arg);
    }
  else if (!strcmp (pattern->name, "actual_arg_func_names"))
    {
      *text = ldv_get_actual_arg_func_names ();
      if (!*text)
          *text = ldv_copy_str ("NULL");
    }
  else if (!strcmp (pattern->name, "arg_numb"))
    *number = ldv_list_len (ldv_func_arg_info_list);
  else if (!strcmp (pattern->name, "aspect_func_name"))
    {
      if (ldv_aspect_func_name)
        *text = ldv_copy_str (ldv_aspect_func_name);
      else
        internal_error ("no function name was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "func_name"))
    {
      if (ldv_func_name)
        *text = ldv_copy_str (ldv_func_name);
      else
        internal_error ("no aspect function name was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "func_ptr_name"))
    {
      if (ldv_func_ptr_name)
        *text = ldv_copy_str (ldv_func_ptr_name);
      else
        internal_error ("no function pointer name was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "func_context"))
    {
      if (ldv_func_signature)
        {
          if (ldv_func_signature->func_context)
            *text = ldv_copy_str (ldv_print_func_context (ldv_func_signature));
          else
            internal_error ("no function context was found for aspect pattern \"%s\"", pattern->name);
        }
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "func_context_decl_line"))
    {
      if (ldv_func_signature)
        {
          if (ldv_func_signature->func_context)
            *text = ldv_copy_str (ldv_print_line_number (ldv_func_signature->func_context->decl_line));
          else
            internal_error ("no function context was found for aspect pattern \"%s\"", pattern->name);
        }
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "func_context_name"))
    {
      if (ldv_func_signature)
        {
          if (ldv_func_signature->func_context)
            *text = ldv_copy_str (ldv_print_func_name (ldv_func_signature->func_context));
          else
            *text = ldv_copy_str ("NULL");
        }
      else if (ldv_var_signature)
        {
          if (ldv_var_signature->func_context)
            *text = ldv_copy_str (ldv_print_func_name (ldv_var_signature->func_context));
          else
            *text = ldv_copy_str ("NULL");
        }
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "func_context_path"))
    {
      if (ldv_func_signature)
        {
          if (ldv_func_signature->func_context)
            *text = ldv_copy_str (ldv_func_signature->func_context->file_path);
          else
            internal_error ("no function context was found for aspect pattern \"%s\"", pattern->name);
        }
      else if (ldv_var_signature)
        {
          if (ldv_var_signature->func_context)
            *text = ldv_copy_str (ldv_var_signature->func_context->file_path);
          else
            internal_error ("no function context was found for aspect pattern \"%s\"", pattern->name);
        }
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if ((!strcmp (pattern->name, "func_signature")) || (!strcmp (pattern->name, "signature")))
    {
      if (ldv_func_decl)
        *text = ldv_copy_str (ldv_func_decl);
      else if (ldv_var_decl)
        *text = ldv_copy_str (ldv_var_decl);
      else if (ldv_type_decl)
        *text = ldv_copy_str (ldv_type_decl);
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "call_line"))
    {
      if (ldv_func_signature)
        *number = ldv_func_signature->call_line;
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "decl_line"))
    {
      if (ldv_func_signature)
        *number = ldv_func_signature->decl_line;
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "use_line"))
    {
      if (ldv_func_signature)
        *number = ldv_func_signature->use_line;
      else if (ldv_var_signature)
        *number = ldv_var_signature->use_line;
      else
        internal_error ("no function signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "path"))
    {
      if (ldv_func_signature)
        *text = ldv_copy_str (ldv_func_signature->file_path);
      else if (ldv_var_signature)
        *text = ldv_copy_str (ldv_var_signature->file_path);
      else if (ldv_type_signature)
        *text = ldv_copy_str (ldv_type_signature->file_path);
      else
        internal_error ("no signature was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "proceed"))
    {
      if (ldv_func_call)
        *text = ldv_get_text (ldv_func_call);
    }
  else if (!strcmp (pattern->name, "res"))
    *text = ldv_copy_str (LDV_FUNC_RES);
  else if (!strcmp (pattern->name, "ret_type"))
    *text = ldv_copy_str (LDV_FUNC_RET_TYPE);
  else if (!strcmp (pattern->name, "ret_type_str"))
    *text = ldv_print_ret_type_str ();
  else if (!strcmp (pattern->name, "var_name"))
    {
      if (ldv_var_name)
        *text = ldv_copy_str (ldv_var_name);
      else
        internal_error ("no variable name was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "var_type_name"))
    {
      if (ldv_var_type_name)
        *text = ldv_copy_str (ldv_var_type_name);
      else
        internal_error ("no variable type name was found for aspect pattern \"%s\"", pattern->name);
    }
  else if (!strcmp (pattern->name, "arg_sign"))
    *sign = *text = ldv_copy_str (ldv_get_arg_sign (pattern->arg_numb));
  else if (!strcmp (pattern->name, "var_init_list"))
    {
      ldv_text = ldv_create_text ();
      ldv_var_initializer = ldv_convert_initializer_to_internal (ldv_var_initializer_tree);

      ldv_print_initializer (ldv_var_initializer, ldv_text, 0);
      *text = ldv_copy_str (ldv_get_text (ldv_text));

      ldv_free_text (ldv_text);
      if (ldv_var_initializer)
        ldv_free_info_initializer (ldv_var_initializer);
    }
  else if (!strcmp (pattern->name, "var_init_values"))
    {
      ldv_text = ldv_create_text ();
      ldv_var_initializer = ldv_convert_initializer_to_internal (ldv_var_initializer_tree);

      ldv_print_var_init_values (ldv_var_initializer, ldv_text);
      *text = ldv_copy_str (ldv_get_text (ldv_text));

      ldv_free_text (ldv_text);
      if (ldv_var_initializer)
        ldv_free_info_initializer (ldv_var_initializer);
    }
  else
    internal_error ("aspect pattern \"%s\" wasn't weaved", pattern->name);
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

  internal_error ("required argument value has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
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

  internal_error ("required argument type name has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
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

  internal_error ("required argument size has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
}

char *
ldv_get_actual_arg_func_names (void)
{
  unsigned int i;
  const char *i_str;
  ldv_list_ptr func_arg_info_list = NULL;
  ldv_func_arg_info_ptr func_arg_info = NULL;
  ldv_str_ptr actual_arg_func_names = NULL;
  char *res = NULL;

  /* Walk through function arguments to find appropriate.
   * Result will look like:
   *   actual_arg_func_name1=func_name1, actual_arg_func_name3=actual_arg_func_name3
   */
  for (i = 1, func_arg_info_list = ldv_func_arg_info_list
    ; func_arg_info_list
    ; i++, func_arg_info_list = ldv_list_get_next (func_arg_info_list))
    {
      func_arg_info = (ldv_func_arg_info_ptr) ldv_list_get_data (func_arg_info_list);

      if (func_arg_info->func_arg_info_kind == LDV_FUNC_ARG_INFO_FUNC_NAME)
        {
          if (actual_arg_func_names)
            ldv_puts_string (", ", actual_arg_func_names);
          else
            actual_arg_func_names = ldv_create_string ();

          ldv_puts_string ("actual_arg_func_name", actual_arg_func_names);

          i_str = ldv_itoa (i);
          ldv_puts_string (i_str, actual_arg_func_names);
          free (CONST_CAST (char *, i_str));

          ldv_putc_string ('=', actual_arg_func_names);
          ldv_puts_string (func_arg_info->func_name, actual_arg_func_names);
        }
    }

  if (actual_arg_func_names)
    {
      res = ldv_copy_str (ldv_get_str (actual_arg_func_names));
      ldv_free_str (actual_arg_func_names);
    }

  return res;
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

  internal_error ("required argument value has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
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

  internal_error ("required argument value has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));
}

const char *
ldv_get_body_sign (ldv_ab_ptr body)
{
  const char *body_c = NULL;
  int body_p;
  ldv_list_ptr body_patterns = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;
  const char *text = NULL;
  unsigned int number;
  const char *sign = NULL;
  ldv_str_ptr body_sign = NULL;

  /* Walk through a body text to find out body patterns that can affect body signature. */
  for (body_c = ldv_get_body_text (body), body_p = 0; *body_c; body_c++, body_p++)
    {
      /* Evaluate aspect patterns if so was placed at a given point of a body
         and remember corresponding signatures if so. */
      for (body_patterns = body->patterns
        ; body_patterns
        ; body_patterns = ldv_list_get_next (body_patterns))
        {
          body_pattern = (ldv_ab_aspect_pattern_ptr) ldv_list_get_data (body_patterns);

          if (body_pattern->place == body_p)
            {
              pattern = body_pattern->pattern;

              if (strcmp (pattern->name, "fprintf"))
                {
                  ldv_evaluate_aspect_pattern (pattern, &text, &number, &sign);
                  if (sign)
                    {
                      if (body_sign)
                        /* Separate several pattern signatures by "_". */
                        ldv_putc_string ('_', body_sign);
                      else
                        body_sign = ldv_create_string ();

                      ldv_puts_string (sign, body_sign);
                      /* Forget about evaluated signature since it is used in
                         condition above. */
                      sign = NULL;
                    }
                }

              break;
            }
        }
    }

  return body_sign ? ldv_get_str(body_sign) : NULL;
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

  internal_error ("required parameter has number \"%d\" that exceeds the maximum one \"%d\"", param_numb, (i - 1));
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

/* Adapted from ldv_print_types_typedefs(). */
char *
ldv_print_arg_type_str (unsigned int arg_numb)
{
  ldv_pps_decl_ptr func_arg_type_decl = NULL;
  ldv_list_ptr func_arg_type_decl_list = NULL;
  unsigned int i;
  char *arg_type_str;

  if (ldv_text_printed)
    {
      ldv_free_text (ldv_text_printed);
      ldv_text_printed = ldv_create_text ();
      ldv_padding_cur = LDV_PADDING_NONE;
    }

  ldv_isstorage_class_and_function_specifiers_needed = false;

  if (!ldv_func_arg_type_decl_list)
    internal_error ("argument type declarations list wasn't found");

  for (func_arg_type_decl_list = ldv_func_arg_type_decl_list, i = 1
    ; func_arg_type_decl_list
    ; func_arg_type_decl_list = ldv_list_get_next (func_arg_type_decl_list), i++)
    {
      if (i == arg_numb)
        {
          func_arg_type_decl = (ldv_pps_decl_ptr) ldv_list_get_data (func_arg_type_decl_list);
          ldv_add_id_declarator (func_arg_type_decl, "%s");
          ldv_print_decl (func_arg_type_decl);
          ldv_delete_id_declarator (func_arg_type_decl->pps_declarator);

          break;
        }
    }

  if (arg_numb > i)
    internal_error ("required argument type string has number \"%d\" that exceeds the maximum one \"%d\"", arg_numb, (i - 1));

  ldv_isstorage_class_and_function_specifiers_needed = true;

  arg_type_str = ldv_copy_str (ldv_get_text (ldv_text_printed));

  ldv_free_text (ldv_text_printed);
  ldv_text_printed = ldv_create_text ();

  return arg_type_str;
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
  const char *text = NULL;
  unsigned int number;
  const char *sign = NULL;
  const char *func_call = NULL;
  ldv_text_ptr body_with_patterns = NULL;
  const char *body_c = NULL;
  int body_p;
  bool isres_needed = true;
  ldv_list_ptr body_patterns = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;

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

              if (!strcmp (pattern->name, "fprintf"))
                ldv_process_aspect_pattern_fprintf (pattern->params, ldv_evaluate_aspect_pattern);
              else
                {
                  /* Parameter signatures are of interest here. */
                  ldv_evaluate_aspect_pattern (pattern, &text, &number, &sign);
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
                         aspect pattern integers. */
                      ldv_puts_text (ldv_itoa (number), body_with_patterns);
                    }
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
    {
      ldv_free_text (ldv_func_call);
      ldv_func_call = NULL;
    }
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
      internal_error ("incorrect primitive pointcut signature declarator kind \"%d\" is used", declarator->pps_declarator_kind);
    }
}

const char *
ldv_print_func_context (ldv_i_func_ptr decl)
{
  return ldv_print_func_decl (decl->func_context);
}

const char *
ldv_print_func_name (ldv_i_func_ptr decl)
{
  return ldv_get_id_name (decl->name);
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

void
ldv_print_initializer (ldv_i_initializer_ptr initializer, ldv_text_ptr initializer_text, unsigned int indent_level)
{
  ldv_list_ptr struct_field_initializer_list = NULL;
  ldv_i_struct_field_initializer_ptr struct_field_initializer = NULL;
  ldv_list_ptr array_elem_initializer_list = NULL;
  ldv_i_array_elem_initializer_ptr array_elem_initializer = NULL;
  const char *indent_spaces = "";
  const char *next_level_indent_spaces = "";

  /* Indentation spaces to be printed before particular initializers. It looks
     like spaces function use the same pointer for different strings. So copy
     obtained string for safety. */
  if (indent_level)
    indent_spaces = ldv_copy_str (spaces (2 * indent_level));

  next_level_indent_spaces = ldv_copy_str (spaces (2 * (indent_level + 1)));

  ldv_puts_text (indent_spaces, initializer_text);
  ldv_puts_text ("value:", initializer_text);

  /* We miss large intializers of global variables. */
  if (!initializer)
    ldv_puts_text (" too large\n", initializer_text);
  else if (initializer->non_struct_or_array_initializer)
    {
      ldv_putc_text (' ', initializer_text);
      ldv_puts_text (initializer->non_struct_or_array_initializer, initializer_text);
      ldv_putc_text ('\n', initializer_text);
    }
  else
    {
      if (initializer->struct_initializer)
        {
          ldv_putc_text ('\n', initializer_text);

          for (struct_field_initializer_list = initializer->struct_initializer
            ; struct_field_initializer_list
            ; struct_field_initializer_list = ldv_list_get_next (struct_field_initializer_list))
            {
              struct_field_initializer = (ldv_i_struct_field_initializer_ptr) ldv_list_get_data (struct_field_initializer_list);
              ldv_puts_text (next_level_indent_spaces, initializer_text);
              ldv_puts_text ("field declaration: ", initializer_text);
              ldv_puts_text (struct_field_initializer->decl, initializer_text);
              ldv_putc_text ('\n', initializer_text);
              ldv_print_initializer (struct_field_initializer->initializer, initializer_text, indent_level + 1);
            }
        }
      else if (initializer->array_initializer)
        {
          ldv_putc_text ('\n', initializer_text);

          for (array_elem_initializer_list = initializer->array_initializer
            ; array_elem_initializer_list
            ; array_elem_initializer_list = ldv_list_get_next (array_elem_initializer_list))
            {
              array_elem_initializer = (ldv_i_array_elem_initializer_ptr) ldv_list_get_data (array_elem_initializer_list);
              ldv_puts_text (next_level_indent_spaces, initializer_text);
              ldv_puts_text ("array element index: ", initializer_text);
              ldv_puts_text (ldv_itoa (array_elem_initializer->index), initializer_text);
              ldv_putc_text ('\n', initializer_text);
              ldv_print_initializer (array_elem_initializer->initializer, initializer_text, indent_level + 1);
            }
        }
      else
        ldv_puts_text (" {}\n", initializer_text);
    }
}

const char *
ldv_print_line_number (unsigned int line_number)
{
  ldv_text_printed = ldv_create_text ();
  ldv_padding_cur = LDV_PADDING_NONE;
  ldv_print_int (line_number);

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

  /* Does not work any more.
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
        internal_error ("incorrect type declaration information kind \"%d\" is used", typedecl->itd_kind);
    }

  ldv_print_str (ldv_get_id_name (typedecl->name));
  */

  return ldv_get_text (ldv_text_printed);
}

char *
ldv_print_var_decl (ldv_i_var_ptr var)
{
  ldv_pps_decl_ptr decl;
  /* char *str; */

  decl = ldv_convert_internal_to_declaration (var->type, ldv_get_id_name (var->name));

  ldv_text_printed = ldv_create_text ();

  ldv_padding_cur = LDV_PADDING_NONE;

  ldv_print_decl (decl);

  ldv_free_pps_decl (decl);

  /* Like in ldv_print_func_decl(). */
  return ldv_get_text (ldv_text_printed);
  /* More proper way to print variable declaration but it causes segmentation faults in ldv_print_body().
  str = ldv_copy_str (ldv_get_text (ldv_text_printed));

  ldv_free_text (ldv_text_printed);

  return str;
  */
}

void
ldv_print_int (int n)
{
  const char *str = NULL;

  str = ldv_itoa (n);

  ldv_print_str (str);

  free (CONST_CAST (char *, str));
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

/* Adapted from ldv_print_types_typedefs(). */
char *
ldv_print_ret_type_str (void)
{
  char *ret_type_str;

  if (ldv_text_printed)
    {
      ldv_free_text (ldv_text_printed);
      ldv_text_printed = ldv_create_text ();
      ldv_padding_cur = LDV_PADDING_NONE;
    }

  ldv_isstorage_class_and_function_specifiers_needed = false;

  if (ldv_func_ret_type_decl)
    {
      ldv_add_id_declarator (ldv_func_ret_type_decl, "%s");
      ldv_print_decl (ldv_func_ret_type_decl);
      ldv_delete_id_declarator (ldv_func_ret_type_decl->pps_declarator);
    }
  else
    internal_error ("return type declaration wasn't found");

  ldv_isstorage_class_and_function_specifiers_needed = true;

  ret_type_str = ldv_copy_str (ldv_get_text (ldv_text_printed));

  ldv_free_text (ldv_text_printed);
  ldv_text_printed = ldv_create_text ();

  return ret_type_str;
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
      internal_error ("Failed to open file %s", ldv_diag_env);

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
  const char *arg_type_name_numb_aux = NULL;
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

      free (CONST_CAST (char *, arg_type_name_numb_aux));

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

      /* TODO: do not free this declaration since it could be used for $arg_type_strN aspect patterns.
       * Free should be done at the end of advice weaving.
      ldv_free_pps_decl (func_arg_type_decl); */

      str = ldv_create_string ();
      ldv_puts_string (arg_type_name, str);
      ldv_free_id (arg_type_name_aux);
      ldv_list_push_back (&ldv_func_arg_type_name_list, str);
    }

  ldv_isstorage_class_and_function_specifiers_needed = true;
}

void
ldv_print_var_init_values (ldv_i_initializer_ptr initializer, ldv_text_ptr initializer_text)
{
  ldv_list_ptr struct_field_initializer_list = NULL;
  ldv_i_struct_field_initializer_ptr struct_field_initializer = NULL;
  ldv_list_ptr array_elem_initializer_list = NULL;
  ldv_i_array_elem_initializer_ptr array_elem_initializer = NULL;

  if (initializer->non_struct_or_array_initializer)
    {
      ldv_puts_text ("||", initializer_text);
      ldv_puts_text (initializer->non_struct_or_array_initializer, initializer_text);
      ldv_putc_text (':', initializer_text);
      ldv_puts_text (ldv_itoa (initializer->is_func_ptr), initializer_text);
    }
  else
    {
      if (initializer->struct_initializer)
        {
          for (struct_field_initializer_list = initializer->struct_initializer
            ; struct_field_initializer_list
            ; struct_field_initializer_list = ldv_list_get_next (struct_field_initializer_list))
            {
              struct_field_initializer = (ldv_i_struct_field_initializer_ptr) ldv_list_get_data (struct_field_initializer_list);
              ldv_print_var_init_values (struct_field_initializer->initializer, initializer_text);
            }
        }
      else
        {
          for (array_elem_initializer_list = initializer->array_initializer
            ; array_elem_initializer_list
            ; array_elem_initializer_list = ldv_list_get_next (array_elem_initializer_list))
            {
              array_elem_initializer = (ldv_i_array_elem_initializer_ptr) ldv_list_get_data (array_elem_initializer_list);
              ldv_print_var_init_values (array_elem_initializer->initializer, initializer_text);
            }
        }
    }
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
ldv_free_func_arg_type_decl_list (void)
{
  ldv_list_ptr param_list = NULL;

  for (param_list = ldv_func_arg_type_decl_list
    ; param_list
    ; param_list = ldv_list_get_next (param_list))
    ldv_free_pps_decl ((ldv_pps_decl_ptr) ldv_list_get_data (param_list));

  ldv_list_delete_all (ldv_func_arg_type_decl_list);
  ldv_func_arg_type_decl_list = NULL;
}

static int
htab_eq_string (const void *s1, const void *s2)
{
  return strcmp ((const char*)s1, (const char*)s2) == 0;
}

void
ldv_weave_advice (expanded_location *open_brace, expanded_location *close_brace)
{
  ldv_ak a_kind;
  char *aux_func_name = NULL;
  ldv_str_ptr aux_func_name_str = NULL;
  void **slot = NULL;
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
  const char *body_sign = NULL;

  /* Check whether something was matched. */
  if (!ldv_i_match)
    return ;

  a_kind = ldv_i_match->a_definition->a_declaration->a_kind;
  pp_kind = ldv_i_match->p_pointcut->pp_kind;

  /* In case of source code queries do not perform weaving just "print" advice
     body that implicitly invokes evaluation of all $fprintf. */
  if (a_kind == LDV_A_QUERY && (pp_kind == LDV_PP_INIT_GLOBAL || pp_kind == LDV_PP_INIT_LOCAL || pp_kind == LDV_PP_USE_VAR))
    {
      ldv_text_printed = ldv_create_text ();

      ldv_var_signature = ldv_i_match->i_var;
      ldv_var_decl = ldv_i_match->i_var->decl;
      ldv_var_name = ldv_get_id_name (ldv_i_match->i_var_aspect->name);

      if (ldv_i_match->i_var_aspect->type->it_kind == LDV_IT_PRIMITIVE && ldv_i_match->i_var_aspect->type->primitive_type->type_name)
        ldv_var_type_name = ldv_get_id_name (ldv_i_match->i_var_aspect->type->primitive_type->type_name);

      /* Convert void * to tree. */
      ldv_var_initializer_tree = (tree) ldv_i_match->i_var->initializer;
      ldv_print_body (ldv_i_match->a_definition->a_body, a_kind);

      ldv_free_text (ldv_text_printed);
      ldv_free_info_match (ldv_i_match);

      ldv_var_signature = NULL;
      ldv_var_decl = NULL;
      ldv_var_name = NULL;
      ldv_var_type_name = NULL;
      ldv_var_initializer_tree = NULL;

      return;
    }
  else if (a_kind == LDV_A_QUERY && (pp_kind == LDV_PP_DECLARE_FUNC || pp_kind == LDV_PP_EXECUTION|| pp_kind == LDV_PP_CALL || pp_kind == LDV_PP_CALLP || pp_kind == LDV_PP_USE_FUNC))
    {
      ldv_text_printed = ldv_create_text ();

      ldv_func_signature = ldv_i_match->i_func;
      ldv_func_decl = ldv_i_match->i_func->decl;
      if (ldv_func_signature->name)
        ldv_func_name = ldv_get_id_name (ldv_func_signature->name);
      if (ldv_func_signature->ptr_name)
        ldv_func_ptr_name = ldv_get_id_name (ldv_func_signature->ptr_name);

      /* TODO: merge this code with the same code below. */
      /* Store an information on a function return type and function
         arguments types that will be used in a body patterns weaving. */
      ldv_func_ret_type_decl = ldv_convert_internal_to_declaration (ldv_i_match->i_func_aspect->type->ret_type, NULL);
      ldv_store_func_arg_type_decl_list (ldv_i_match->i_func_aspect->type);
      ldv_print_body (ldv_i_match->a_definition->a_body, a_kind);
      ldv_free_text (ldv_text_printed);

      for (str_list = ldv_func_arg_type_name_list
        ; str_list
        ; str_list = ldv_list_get_next (str_list))
        {
          str = (ldv_str_ptr) ldv_list_get_data (str_list);
          ldv_free_str (str);
        }

      ldv_list_delete_all (ldv_func_arg_type_name_list);
      ldv_free_func_arg_type_decl_list ();
      ldv_free_pps_decl (ldv_func_ret_type_decl);
      ldv_free_info_match (ldv_i_match);

      ldv_func_ret_type_decl = NULL;
      ldv_func_signature = NULL;
      ldv_func_decl = NULL;
      ldv_func_name = NULL;
      ldv_func_ptr_name = NULL;
      ldv_func_arg_type_name_list = NULL;

      return;
    }
  if (a_kind == LDV_A_QUERY && pp_kind == LDV_PP_INTRODUCE)
    {
      ldv_text_printed = ldv_create_text ();

      ldv_type_signature = ldv_i_match->i_typedecl;
      ldv_type_decl = ldv_i_match->i_typedecl->decl;
      ldv_print_body (ldv_i_match->a_definition->a_body, a_kind);

      ldv_free_text (ldv_text_printed);
      ldv_type_signature = NULL;
      ldv_type_decl = NULL;

      return;
    }

  ldv_padding_cur = LDV_PADDING_NONE;

  /* Weave an advice in depend on a matched entity kind and a ldv stage. */
  switch (ldv_i_match->i_kind)
    {
    case LDV_I_FUNC:
      func_aspect = ldv_i_match->i_func_aspect;
      func_source = ldv_i_match->i_func;

      /* Create list of function parameter names on the basis of a source
         function declaration and a pointcut function signature. */
      ldv_create_aux_func_params (func_source->type, func_aspect->type);

      body_sign = ldv_get_body_sign (ldv_i_match->a_definition->a_body);
      aux_func_name_str = ldv_create_string ();
      ldv_puts_string (CIF_AUX_FUNC_NAME_PREFIX, aux_func_name_str);
      ldv_puts_string (ldv_get_id_name (func_aspect->name), aux_func_name_str);

      if (body_sign)
        {
          ldv_putc_string ('_', aux_func_name_str);
          ldv_puts_string (body_sign, aux_func_name_str);
        }

      aux_func_name = ldv_copy_str (ldv_get_str (aux_func_name_str));
      ldv_free_str (aux_func_name_str);

      if (ldv_instrumentation ())
        {
          if (!ldv_aux_func_names)
            ldv_aux_func_names = htab_create (10, htab_hash_string, htab_eq_string, NULL);

          /* Auxiliary function was already created. */
          if (htab_find_with_hash (ldv_aux_func_names, aux_func_name, htab_hash_string (aux_func_name)))
            {
              free (aux_func_name);
              ldv_free_info_match (ldv_i_match);
              ldv_func_va_init = NULL;
              break;
            }

          /* Store a new auxiliary function name in hash table. */
          slot = htab_find_slot_with_hash (ldv_aux_func_names, aux_func_name, htab_hash_string (aux_func_name), INSERT);
          *slot = ldv_copy_str (aux_func_name);

          if (!ldv_func_defs_for_print)
            ldv_func_defs_for_print = ldv_create_text ();

          aspect_func_decl_for_print_new = ldv_create_decl_for_print ();
          func_decl_for_print_new = ldv_create_decl_for_print ();

          /* Place (aspect) function declarations immediately after the end of
             a function definition, i.e. after a close brace. */
          if (!close_brace)
            internal_error ("for function \"%s\" location of close brace isn't specified", ldv_get_id_name (func_source->name));

          func_decl_for_print_new->file = (*close_brace).file;
          aspect_func_decl_for_print_new->file = (*close_brace).file;
          func_decl_for_print_new->line = (*close_brace).line;
          aspect_func_decl_for_print_new->line = (*close_brace).line;
          /* Print declaration just after the close brace. */
          func_decl_for_print_new->column = (*close_brace).column;
          aspect_func_decl_for_print_new->column = (*close_brace).column;

          /* Create a function declaration. */
          ldv_text_printed = ldv_create_text ();

          ldv_putc_text ('\n', ldv_text_printed);

          if (pp_kind == LDV_PP_EXECUTION)
            {
              func_name = aux_func_name;
              ldv_aspect_func_name = func_name;
            }
          else if (pp_kind == LDV_PP_CALL)
            {
              func_name = ldv_get_id_name (func_aspect->name);
              ldv_func_name = func_name;
            }

          if (func_source->isgnu_inline)
            ldv_puts_text ("__attribute__((gnu_inline)) ", ldv_text_printed);
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
              func_name = aux_func_name;
              ldv_aspect_func_name = func_name;
            }

          if (func_source->isgnu_inline)
            ldv_puts_text ("__attribute__((gnu_inline)) ", ldv_text_printed);
          decl = ldv_convert_internal_to_declaration (func_aspect->type, func_name);
          /* Make auxiliary functions static to avoid duplicates
           * (http://forge.ispras.ru/issues/7100). */
          if (pp_kind == LDV_PP_CALL)
            decl->pps_declspecs->isstatic = true;
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
            func_name = aux_func_name;
          else if (pp_kind == LDV_PP_CALL)
            func_name = ldv_get_id_name (func_aspect->name);

          ldv_puts_text (func_name, ldv_text_printed);
          ldv_putc_text ('(', ldv_text_printed);

          for (str_list = ldv_func_param_list
            ; str_list
            ; str_list = ldv_list_get_next (str_list))
            {
              str = (ldv_str_ptr) ldv_list_get_data (str_list);

              ldv_puts_text (ldv_get_str (str), ldv_text_printed);

              if (ldv_list_get_next (str_list))
                ldv_puts_text (", ", ldv_text_printed);
            }

          ldv_putc_text (')', ldv_text_printed);
          ldv_func_call = ldv_create_text ();
          ldv_puts_text (ldv_get_text (ldv_text_printed), ldv_func_call);
          ldv_free_text (ldv_text_printed);

          ldv_print_info (LDV_INFO_WEAVE, "create \"%s\" function call for weaving", func_name);

          /* Create an aspect function definition. */
          ldv_text_printed = ldv_create_text ();

          if (pp_kind == LDV_PP_EXECUTION)
            func_name = ldv_get_id_name (func_aspect->name);
          else if (pp_kind == LDV_PP_CALL)
            func_name = aux_func_name;

          ldv_puts_text ("\n/* CIF Original function \"", ldv_text_printed);
          ldv_puts_text (ldv_get_id_name (func_aspect->name), ldv_text_printed);
          ldv_puts_text ("\". Instrumenting function \"", ldv_text_printed);
          ldv_puts_text (func_name, ldv_text_printed);
          ldv_puts_text ("\". */\n", ldv_text_printed);
          if (func_source->isgnu_inline)
            ldv_puts_text ("__attribute__((gnu_inline)) ", ldv_text_printed);
          decl = ldv_convert_internal_to_declaration (func_aspect->type, func_name);
          if (pp_kind == LDV_PP_CALL)
            decl->pps_declspecs->isstatic = true;
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

          free (aux_func_name);
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

          ldv_weave_func_source (ldv_get_id_name (func_source->name), aux_func_name, pp_kind);

          free (aux_func_name);
        }

      ldv_free_info_match (ldv_i_match);

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
            internal_error ("incorrect advice kind \"%d\" is used", a_kind);
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

      /* Store a function argument name. */
      var_param_name = ldv_create_string ();
      ldv_puts_string (LDV_VAR_ARG_NAME, var_param_name);
      ldv_list_push_back (&ldv_func_param_list, var_param_name);

      body_sign = ldv_get_body_sign (ldv_i_match->a_definition->a_body);
      aux_func_name_str = ldv_create_string ();
      ldv_puts_string (CIF_AUX_FUNC_NAME_PREFIX, aux_func_name_str);
      ldv_puts_string (pp_kind == LDV_PP_SET ||
                       pp_kind == LDV_PP_SET_LOCAL ||
                       pp_kind == LDV_PP_SET_GLOBAL ? "set_" : "get_", aux_func_name_str);
      ldv_puts_string (ldv_get_id_name (var->name), aux_func_name_str);

      if (body_sign)
        {
          ldv_putc_string ('_', aux_func_name_str);
          ldv_puts_string (body_sign, aux_func_name_str);
        }

      aux_func_name = ldv_copy_str (ldv_get_str (aux_func_name_str));
      ldv_free_str (aux_func_name_str);

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
          if (var->decl_func_context)
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
          ldv_puts_id (aux_func_name, func_aspect->name);
          ldv_aspect_func_name = aux_func_name;
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
        ldv_weave_var_source (ldv_get_id_name (var->name), aux_func_name, pp_kind);

      ldv_free_info_match (ldv_i_match);

      break;

    default:
      internal_error ("incorrect information kind \"%d\" is used", ldv_i_match->i_kind);
    }
}

void
ldv_weave_func_source (const char *func_name, const char *aux_func_name, ldv_ppk pp_kind)
{
  tree id = NULL_TREE;

  if (pp_kind == LDV_PP_EXECUTION)
    {
      ldv_print_info (LDV_INFO_WEAVE, "change source function name in function definition from \"%s\" to \"%s\" for execution join point", func_name, aux_func_name);

      /* Override an existing declaration name since its identifier has constant
         qualifier. */
      DECL_NAME (ldv_func_decl_matched) = get_identifier (aux_func_name);
    }
  else if (pp_kind == LDV_PP_CALL)
    {
      ldv_print_info (LDV_INFO_WEAVE, "change source function name in function call from \"%s\" to \"%s\" for call join point", func_name, aux_func_name);

      /* Get identifier corresponding to auxiliary function name. */
      id = get_identifier (aux_func_name);
      /* Obtain auxiliary function declaration through identifier. */
      ldv_func_called_matched = lookup_name (id);
    }
}

void
ldv_weave_var_source (const char *var_name, const char *aux_func_name, ldv_ppk pp_kind)
{
  tree id = NULL_TREE;

  if (pp_kind == LDV_PP_SET
    || pp_kind == LDV_PP_SET_LOCAL || pp_kind == LDV_PP_SET_GLOBAL
    || pp_kind == LDV_PP_GET
    || pp_kind == LDV_PP_GET_LOCAL || pp_kind == LDV_PP_GET_GLOBAL)
    {
      ldv_print_info (LDV_INFO_WEAVE, "insert function call \"%s\" for variable \"%s\" for set or get join point", aux_func_name, var_name);

      /* Get identifier corresponding to auxiliary function name. */
      id = get_identifier (aux_func_name);
      /* Obtain auxiliary function declaration through identifier. */
      ldv_func_called_matched = lookup_name (id);
    }
}
