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

/* General cpp core types and functions. */
#include "config.h"
#include "system.h"
#include "cpplib.h"
#include "internal.h"

#include "ldv-aspect-types.h"
#include "ldv-cpp-converter.h"
#include "ldv-cpp-core.h"


static ldv_i_type_ptr ldv_convert_declspecs_declarator_to_internal (ldv_pps_declspecs_ptr, ldv_list_ptr, ldv_list_ptr);
static ldv_i_param_ptr ldv_convert_param_signature_to_internal (ldv_pps_decl_ptr);


char *
ldv_convert_body_to_macro (ldv_ab_ptr body)
{
  char *value = NULL, *value_new = NULL, *value_new_begin = NULL;

  value = ldv_copy_str (ldv_cpp_get_body_text (body));
  value = ldv_trunkate_braces (value);
  value_new_begin = value_new = ldv_copy_str (value);

  /* Go to a first non whitespace symbol. */
  while (*value != '\0' && (*value == ' ' || *value == '\n' || *value == '\t'))
    value++;

  while (1)
    {
      while (*value != '\0' && *value == '\n')
        {
          /* Go to a next line. */
          value++;

          /* Go to a first non space and non tab symbol. */
          while (*value != '\0' && (*value == ' ' || *value == '\t'))
            value++;
        }

      /* Just copy a symbol. */
      *value_new = *value;

      if (*value == '\0')
        break;

      value_new++;
      value++;
    }

  return value_new_begin;
}

ldv_i_type_ptr
ldv_convert_declspecs_declarator_to_internal (ldv_pps_declspecs_ptr declspecs, ldv_list_ptr declarator_first_list, ldv_list_ptr declarator_cur_list)
{
  ldv_i_type_ptr type = NULL, type_first = NULL;
  ldv_pps_declarator_ptr declarator_cur = NULL;
  ldv_list_ptr declarator_prev_list = NULL;
  ldv_pps_func_arg_ptr func_arg = NULL;
  ldv_list_ptr func_arg_list = NULL;
  ldv_i_param_ptr param_new = NULL;

  declarator_cur = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_cur_list);

  type = ldv_create_info_type ();
  type_first = ldv_create_info_type ();

  /* In case when we reach the lowest base type determined by declaration
     specifiers. */
  if (declarator_cur_list == declarator_first_list)
    {
      type_first->it_kind = LDV_IT_PRIMITIVE;
      type_first->primitive_type = declspecs;

      /* That is an id or an abstract declarator that has just declarations
         specifiers without intermediate declarators. A function declarator base
         type is its return type. It also may consist of declaration specifiers
         without other intermediate declarators. */
      if (declarator_cur->pps_declarator_kind == LDV_PPS_DECLARATOR_ID
          || declarator_cur->pps_declarator_kind == LDV_PPS_DECLARATOR_NONE)
        return type_first;
    }

  /* Otherwise relate a current declarator with its base type. */
  declarator_prev_list = ldv_list_get_prev (declarator_first_list, declarator_cur_list);

  switch (declarator_cur->pps_declarator_kind)
    {
    case LDV_PPS_DECLARATOR_ARRAY:
      type->it_kind = LDV_IT_ARRAY;

      type->issize_specified = declarator_cur->pps_array_size->issize_specified;
      type->isany_size =  declarator_cur->pps_array_size->isany_size;

      if (declarator_cur->pps_array_size->issize_specified)
        type->array_size = declarator_cur->pps_array_size->pps_array_size;

      type->array_type = (declarator_cur_list == declarator_first_list) ? type_first : ldv_convert_declspecs_declarator_to_internal (declspecs, declarator_first_list, declarator_prev_list);

      break;

    case LDV_PPS_DECLARATOR_FUNC:
      type->it_kind = LDV_IT_FUNC;

      /* Convert a function return type. */
      type->ret_type = (declarator_cur_list == declarator_first_list) ? type_first : ldv_convert_declspecs_declarator_to_internal (declspecs, declarator_first_list, declarator_prev_list);

      /* Convert function arguments. */
      for (func_arg_list = declarator_cur->func_arg_list
        ; func_arg_list
        ; func_arg_list = ldv_list_get_next (func_arg_list))
        {
          func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (func_arg_list);

          param_new = ldv_convert_param_signature_to_internal (func_arg->pps_func_arg);

          param_new->type->isva = func_arg->isva;

          ldv_list_push_back (&type->param, param_new);
        }

      break;

    case LDV_PPS_DECLARATOR_NONE:
      break;

    case LDV_PPS_DECLARATOR_PTR:
      type->it_kind = LDV_IT_PTR;
      type->isconst = declarator_cur->pps_ptr_quals->isconst;
      type->isvolatile = declarator_cur->pps_ptr_quals->isvolatile;
      type->isrestrict = declarator_cur->pps_ptr_quals->isrestrict;
      type->ptr_type = (declarator_cur_list == declarator_first_list) ? type_first : ldv_convert_declspecs_declarator_to_internal (declspecs, declarator_first_list, declarator_prev_list);

      break;

    default:
      LDV_CPP_FATAL_ERROR ("incorrect primitive pointcut signature declarator kind \"%d\" is used", declarator_cur->pps_declarator_kind);
    }

  return type;
}

ldv_i_func_ptr
ldv_convert_func_signature_to_internal (ldv_pps_decl_ptr pps_func)
{
  ldv_i_func_ptr i_func = NULL;
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;

  i_func = ldv_create_info_func ();

  /* A last declarator in a chain must contain a function name. */
  declarator_list = ldv_list_get_last (pps_func->pps_declarator);

  declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

  /* Convert a declarator name to a function name. */
  if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_ID)
    {
      /* Copy function name rather then keep reference to it since it
         will be freed. */
      i_func->name = ldv_copy_id (declarator->declarator_name);

      /* A previous to last declarators contains a function type. */
      declarator_list = ldv_list_get_prev (pps_func->pps_declarator, declarator_list);

      /* Convert a declarator type to a function type. */
      i_func->type = ldv_convert_declspecs_declarator_to_internal (pps_func->pps_declspecs, pps_func->pps_declarator, declarator_list);

      return i_func;
    }

  LDV_CPP_FATAL_ERROR ("can't convert function signature from declaration form to the internal type representation");
  return NULL;
}

ldv_i_macro_ptr
ldv_convert_macro_signature_to_internal (ldv_pps_macro_ptr pps_macro)
{
  ldv_i_macro_ptr i_macro = NULL;
  ldv_id_ptr pps_macro_param = NULL;
  ldv_list_ptr macro_param_list = NULL;

  i_macro = ldv_create_info_macro ();

  /* Copy a macro kind. */
  i_macro->macro_kind = pps_macro->pps_macro_kind;

  /* Convert a declarator name to a macro name. */
  i_macro->macro_name = pps_macro->macro_name;

  /* Convert declarator parameters to macro parameters. */
  for (macro_param_list = pps_macro->macro_param_list
    ; macro_param_list
    ; macro_param_list = ldv_list_get_next (macro_param_list))
    {
      pps_macro_param = (ldv_id_ptr) ldv_list_get_data (macro_param_list);

      ldv_list_push_back (&i_macro->macro_param, pps_macro_param);
    }

  return i_macro;
}

ldv_i_param_ptr
ldv_convert_param_signature_to_internal (ldv_pps_decl_ptr pps_param)
{
  ldv_i_param_ptr i_param = NULL;
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;

  i_param = ldv_create_info_param ();

  if (pps_param->pps_decl_kind == LDV_PPS_DECL_ANY_PARAMS)
    {
      /* So any parameters may correspond to this case. */
      i_param->isany_params = true;

      /* A type is needed to specify variable arguments list if so. */
      i_param->type = ldv_create_info_type ();

      return i_param;
    }

  /* A last declarator in a chain must contain a parameter name that may be
     empty. */
  declarator_list = ldv_list_get_last (pps_param->pps_declarator);

  declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

  /* Convert a declarator name to a parameter name. */
  if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_ID)
    {
      i_param->name = declarator->declarator_name;

      /* A previous to last declarators contain a paramter type. */
      declarator_list = ldv_list_get_prev (pps_param->pps_declarator, declarator_list);
    }
  else
    i_param->name = NULL;

  /* Convert a declarator type to a paramter type. */
  i_param->type = ldv_convert_declspecs_declarator_to_internal (pps_param->pps_declspecs, pps_param->pps_declarator, declarator_list);

  return i_param;
}

ldv_i_typedecl_ptr
ldv_convert_typedecl_signature_to_internal (ldv_pps_decl_ptr pps_typedecl)
{
  ldv_i_typedecl_ptr i_typedecl = NULL;
  ldv_pps_declspecs_ptr pps_declspecs = NULL;

  i_typedecl = ldv_create_info_typedecl ();

  /* Specify that a type type is absent at the beginnning. */
  i_typedecl->itd_kind = LDV_ITD_NONE;

  /* A type declaration signature has just declaration specifiers and no declarators. */
  pps_declspecs = pps_typedecl->pps_declspecs;

  /* Obtain a type declaration name. */
  i_typedecl->name = pps_declspecs->type_name;

  /* Obtain a type declaration type type. */
  if (pps_declspecs->isstruct)
    i_typedecl->itd_kind = LDV_ITD_STRUCT;
  else if (pps_declspecs->isunion)
    i_typedecl->itd_kind = LDV_ITD_UNION;
  else if (pps_declspecs->isenum)
    i_typedecl->itd_kind = LDV_ITD_ENUM;
  else
    {
      LDV_CPP_FATAL_ERROR ("incorrect type declaration information kind \"%d\" is used", i_typedecl->itd_kind);
    }

  return i_typedecl;
}

ldv_i_var_ptr
ldv_convert_var_signature_to_internal (ldv_pps_decl_ptr pps_var)
{
  ldv_i_var_ptr i_var = NULL;
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;

  i_var = ldv_create_info_var ();

  /* A last declarator in a chain must contain a variable name. */
  declarator_list = ldv_list_get_last (pps_var->pps_declarator);

  declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);

  /* Convert a declarator name to a variable name. */
  if (declarator->pps_declarator_kind == LDV_PPS_DECLARATOR_ID)
    {
      i_var->name = declarator->declarator_name;

      /* A previous to last declarators contain a variable type. */
      declarator_list = ldv_list_get_prev (pps_var->pps_declarator, declarator_list);

      /* Convert a declarator type to a variable type. */
      i_var->type = ldv_convert_declspecs_declarator_to_internal (pps_var->pps_declspecs, pps_var->pps_declarator, declarator_list);

      return i_var;
    }

  LDV_CPP_FATAL_ERROR ("can't convert variable signature from declaration form to the internal type representation");

  return NULL;
}
