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
#include "ldv-cpp-advice-weaver.h"
#include "ldv-cpp-converter.h"
#include "ldv-cpp-core.h"
#include "ldv-cpp-pointcut-matcher.h"


/* Macro from ldv-io.h that's needed for printing of matched just by name
   macros. */
#define LDV_MATCHED_BY_NAME             (stderr)


/* An advice definitions list. */
ldv_list_ptr ldv_adef_list = NULL;
ldv_i_match_ptr ldv_i_match;

static bool ldv_cpp_isprint_signature_of_matched_by_name = false;


static bool ldv_match_bool (bool, bool);
static bool ldv_match_declspecs (ldv_pps_declspecs_ptr declspecs_first, ldv_pps_declspecs_ptr declspecs_second);
static bool ldv_match_param_type (ldv_i_type_ptr, ldv_i_type_ptr);
static bool ldv_match_type (ldv_i_type_ptr, ldv_i_type_ptr);
static bool ldv_match_universal_bool (bool, bool, bool);


bool
ldv_match_bool (bool first, bool second)
{
  return ldv_match_universal_bool (first, second, false);
}

bool
ldv_match_cp (ldv_cp_ptr c_pointcut, ldv_i_match_ptr i_match)
{
  ldv_ppk pp_kind;
  ldv_ik i_kind;

  i_kind = i_match->i_kind;

  switch (c_pointcut->cp_kind)
    {
      /* A result is true if both operands are true. Logic is short. A second
         operand isn't evaluated if a first operand is false. */
    case LDV_CP_AND:
      if (ldv_match_cp (c_pointcut->c_pointcut_first, i_match) && ldv_match_cp (c_pointcut->c_pointcut_second, i_match))
        return true;

      break;

    /* A result is true if an operand is false. */
    case LDV_CP_NOT:
      if (!ldv_match_cp (c_pointcut->c_pointcut_first, i_match))
        return true;

      break;

    /* A result is true if one of operands is true. Logic is short. A second
       operand isn't evaluated if a first operand is true. */
    case LDV_CP_OR:
      if (ldv_match_cp (c_pointcut->c_pointcut_first, i_match))
        return true;

      if (ldv_match_cp (c_pointcut->c_pointcut_second, i_match))
        return true;

      break;

    /* Match a primitive pointcut in depending on its kind. */
    case LDV_CP_PRIMITIVE:
      pp_kind = c_pointcut->p_pointcut->pp_kind;

      switch (pp_kind)
        {
        case LDV_PP_DEFINE:
        case LDV_PP_EXPAND:
          if (i_kind == LDV_I_MACRO && i_match->pp_kind == pp_kind && ldv_match_macro_signature (i_match, c_pointcut->p_pointcut->pp_signature->pps_macro))
            {
              i_match->p_pointcut = c_pointcut->p_pointcut;

              return true;
            }

          break;

        case LDV_PP_CALL:
        case LDV_PP_EXECUTION:
          if (i_kind == LDV_I_FUNC && i_match->pp_kind == pp_kind && ldv_match_func_signature (i_match, c_pointcut->p_pointcut->pp_signature->pps_declaration))
            {
              i_match->p_pointcut = c_pointcut->p_pointcut;

              return true;
            }

           break;

        case LDV_PP_FILE:
          if (i_kind == LDV_I_FILE && i_match->pp_kind == pp_kind && c_pointcut->p_pointcut->pp_signature->pps_file->file_name->isthis)
            {
              i_match->i_file->name = ldv_cpp_get_file_name (c_pointcut->p_pointcut->pp_signature->pps_file->file_name);
              i_match->i_file->isthis = c_pointcut->p_pointcut->pp_signature->pps_file->file_name->isthis;

              return true;
            }

          break;

        case LDV_PP_INFILE:
          if (i_kind == LDV_I_FUNC && ldv_match_file_signature (i_match->i_func->file_path, c_pointcut->p_pointcut->pp_signature->pps_file))
            return true;
          else if (i_kind == LDV_I_MACRO && ldv_match_file_signature (i_match->i_macro->file_path, c_pointcut->p_pointcut->pp_signature->pps_file))
            return true;
          else if (i_kind == LDV_I_TYPE && ldv_match_file_signature (i_match->i_typedecl->file_path, c_pointcut->p_pointcut->pp_signature->pps_file))
            return true;
          else if (i_kind == LDV_I_VAR && ldv_match_file_signature (i_match->i_var->file_path, c_pointcut->p_pointcut->pp_signature->pps_file))
            return true;

          break;

        case LDV_PP_INFUNC:
          if (i_kind == LDV_I_VAR && i_match->i_var->func_context && ldv_match_func_signature (i_match->i_var->func_context, c_pointcut->p_pointcut->pp_signature->pps_declaration))
            return true;

          break;

        case LDV_PP_INTRODUCE:
          if (i_kind == LDV_I_TYPE && ldv_match_typedecl_signature (i_match, c_pointcut->p_pointcut->pp_signature->pps_declaration))
            {
              i_match->p_pointcut = c_pointcut->p_pointcut;

              return true;
            }

          break;

        case LDV_PP_GET_GLOBAL:
        case LDV_PP_GET_LOCAL:
        case LDV_PP_INIT_GLOBAL:
        case LDV_PP_INIT_LOCAL:
        case LDV_PP_SET_GLOBAL:
        case LDV_PP_SET_LOCAL:
          if (i_kind == LDV_I_VAR
            /* * = *_local || *_local, where "*" is "get", "init" or "set". */
            && (i_match->pp_kind == pp_kind
            || ((i_match->pp_kind == LDV_PP_GET_GLOBAL || i_match->pp_kind == LDV_PP_GET_LOCAL) && pp_kind == LDV_PP_GET)
            || ((i_match->pp_kind == LDV_PP_INIT_GLOBAL || i_match->pp_kind == LDV_PP_INIT_LOCAL) && pp_kind == LDV_PP_INIT)
            || ((i_match->pp_kind == LDV_PP_SET_GLOBAL || i_match->pp_kind == LDV_PP_SET_LOCAL) && pp_kind == LDV_PP_SET))
            && ldv_match_var_signature (i_match, c_pointcut->p_pointcut->pp_signature->pps_declaration))
            {
              i_match->p_pointcut = c_pointcut->p_pointcut;

              return true;
            }

          break;

        default:
          LDV_CPP_FATAL_ERROR ("incorrect primitive pointcut kind \"%d\" is used", pp_kind);
        }

      break;

    default:
      LDV_CPP_FATAL_ERROR ("incorrect composite pointcut kind \"%d\" is used", c_pointcut->cp_kind);
    }

  return false;
}

static bool
ldv_match_declspecs (ldv_pps_declspecs_ptr declspecs_first, ldv_pps_declspecs_ptr declspecs_second)
{
  return ldv_match_universal_bool (declspecs_first->isinline, declspecs_second->isinline, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->istypedef, declspecs_second->istypedef, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isstatic, declspecs_second->isstatic, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isextern, declspecs_second->isextern, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isauto, declspecs_second->isauto, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isregister, declspecs_second->isregister, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isvoid, declspecs_second->isvoid, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->ischar, declspecs_second->ischar, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isint, declspecs_second->isint, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isfloat, declspecs_second->isfloat, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isdouble, declspecs_second->isdouble, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isbool, declspecs_second->isbool, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->iscomplex, declspecs_second->iscomplex, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isshort, declspecs_second->isshort, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->islong, declspecs_second->islong, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->islonglong, declspecs_second->islonglong, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->issigned, declspecs_second->issigned, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isunsigned, declspecs_second->isunsigned, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isconst, declspecs_second->isconst, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isvolatile, declspecs_second->isvolatile, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isrestrict, declspecs_second->isrestrict, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isstruct, declspecs_second->isstruct, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isunion, declspecs_second->isunion, declspecs_second->isuniversal_type_spec)
    && ldv_match_universal_bool (declspecs_first->isenum, declspecs_second->isenum, declspecs_second->isuniversal_type_spec)
    && ((!declspecs_first->type_name && !declspecs_second->type_name)
      || (declspecs_second->type_name && !ldv_cmp_str (declspecs_second->type_name, ldv_cpp_get_id_name (declspecs_first->type_name)))
      || (!declspecs_second->type_name && declspecs_second->isuniversal_type_spec));
}

void
ldv_match_macro (cpp_reader *pfile, cpp_hashnode *node, const cpp_token ***arg_values, ldv_ppk pp_kind)
{
  ldv_adef_ptr adef = NULL;
  ldv_list_ptr adef_list = NULL;
  ldv_i_match_ptr match = NULL;
  ldv_i_macro_ptr macro = NULL;
  const struct line_map *map = NULL;
  int i, j;
  const char *macro_param_name_original = NULL;
  char *macro_param_name = NULL;
  ldv_id_ptr macro_param = NULL;
  const char *macro_param_ellipsis = "...";
  const cpp_token *arg_value = NULL;
  ldv_str_ptr macro_param_val = NULL;
  char *macro_param_val_str = NULL;

  /* There is no aspect definitions at all. */
  if (ldv_adef_list == NULL)
    {
      ldv_i_match = NULL;
      return;
    }

  match = ldv_create_info_match ();

  macro = ldv_create_info_macro ();

  match->i_kind = LDV_I_MACRO;
  match->pp_kind = pp_kind;
  match->i_macro = macro;

  map = linemap_lookup (pfile->line_table, pfile->directive_line);

  /* Understand whether a macro function or a macro definition is given. */
  if (node->value.macro->fun_like)
    macro->macro_kind = LDV_PPS_MACRO_FUNC;
  else
    macro->macro_kind = LDV_PPS_MACRO_DEF;

  /* Remember a macro name and a name of file containing a given macro. */
  macro->macro_name = ldv_create_id ();
  ldv_puts_id ((const char *) (NODE_NAME (node)), macro->macro_name);
  macro->file_path = map->to_file;
  macro->macro_param = NULL;

  /* Remember macro parameters. */
  for (i = 0; i < node->value.macro->paramc; ++i)
    {
      macro_param_name_original = (char *) NODE_NAME (node->value.macro->params[i]);

      /* Check the last parameter since it may be variadic and should be
       * processed respectively. */
      if (node->value.macro->variadic && i == node->value.macro->paramc - 1)
        {
          /* Use the standard designition (ellipsis) for variadic macro
           * parameters instead of __VA_ARGS__. */
          if (!strcmp ("__VA_ARGS__", macro_param_name_original))
            macro_param_name = (char *) macro_param_ellipsis;
          /* Named variadic parameters. Use a name with ellipsis how it was
           * in original source code. */
          else
            {
              macro_param_name = (char *) xmalloc (sizeof (char) * (strlen (macro_param_name_original) + 3 + 1));
              sprintf (macro_param_name, "%s...", macro_param_name_original);
            }
        }
      else
        {
          macro_param_name = (char *) macro_param_name_original;
        }

      macro_param = ldv_create_id ();
      ldv_puts_id (macro_param_name, macro_param);
      ldv_list_push_back (&macro->macro_param, macro_param);
    }

  /* Store macro argument actual values if so. */
  if (arg_values)
    {
      for (i = 0; i < node->value.macro->paramc; ++i)
        {
          macro_param_val = ldv_create_string ();

          /* Keep empty string as values for variadic macro parameters. */
          if (!node->value.macro->variadic || i != node->value.macro->paramc - 1)
            {
              for (j = 0; 1; j++)
                {
                  if (!arg_values[i] || !arg_values[i][j])
                    {
                      LDV_CPP_FATAL_ERROR ("Can't get the following token");
                      break;
                    }

                  arg_value = arg_values[i][j];

                  /* CPP_EOF finishes current argument. */
                  if (arg_value->type == CPP_EOF)
                    break;

                  /* Skip auxiliary unspellable tokens. */
                  if (arg_value->type == CPP_PADDING)
                    continue;

                  /* Print spaces between tokens if this is required. */
                  if (arg_value->flags & PREV_WHITE)
                    ldv_puts_string (" ", macro_param_val);

                  if ((macro_param_val_str = (char *) cpp_token_as_text (pfile, arg_value)))
                    ldv_puts_string (macro_param_val_str, macro_param_val);
                  else
                    {
                      LDV_CPP_FATAL_ERROR ("Can't convert token to text");
                    }
                }
            }

          ldv_list_push_back (&macro->macro_param_value, ldv_get_str (macro_param_val));
        }
    }

  /* Walk through an advice definitions list to find matches. */
  for (adef_list = ldv_adef_list; adef_list; adef_list = ldv_list_get_next (adef_list))
    {
      adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

      if (ldv_match_cp (adef->a_declaration->c_pointcut, match))
        {
          /* Count advice weavings. */
          ++(adef->use_counter);

          ldv_i_match = match;
          match->a_definition = adef;

          return;
        }
      /* Print signatures of matched by name but not by other signature
         macros if it's needed. */
      else if (match->ismatched_by_name)
        {
          if (ldv_cpp_isprint_signature_of_matched_by_name)
            {
              fprintf (LDV_MATCHED_BY_NAME, "\nThese macros were matched by name but have different signatures:\n  source macro: ");
              ldv_print_macro (match->i_macro);
              fprintf (LDV_MATCHED_BY_NAME, "\n  aspect macro: ");
              ldv_print_macro (match->i_macro_aspect);
              fprintf (LDV_MATCHED_BY_NAME, "\n");
            }

          match->ismatched_by_name = false;
        }
    }

  /* Nothing was matched. */
  ldv_i_match = NULL;

  return;
}

bool
ldv_match_file_signature (const char *file_path, ldv_pps_file_ptr pps_file)
{
  const char *file_path_aspect_str = NULL;
  ldv_id_ptr file_path_aspect = NULL;
  char *file_path_source = NULL;

  file_path_aspect_str = ldv_cpp_get_file_name (pps_file->file_name);
  file_path_source = ldv_copy_str (file_path);

  /* Add artificial "$" wildcards to the beginning and to the end of aspect file
     name to make it possible to match absolute source file name with
     additionally generated by Aspectator suffix. */
  file_path_aspect = ldv_create_id ();
  file_path_aspect->isany_chars = true;
  ldv_putc_id ('$', file_path_aspect);
  ldv_puts_id (file_path_aspect_str, file_path_aspect);
  ldv_putc_id ('$', file_path_aspect);

  /* Files are matched by pathes to them. If a file path obtained from an aspect
     file is a part of an entity file path, then it is considered as matched. */
  if (!ldv_cmp_str (file_path_aspect, file_path_source))
    {
      /* TODO print these messages just in case of debug
      fprintf (stderr, "File with path \"%s\" was matched.\n", file_path_source);
      */
      return true;
    }

  return false;
}

bool
ldv_match_func_signature (ldv_i_match_ptr i_match, ldv_pps_decl_ptr pps_func)
{
  ldv_i_func_ptr func_source = NULL, func_aspect = NULL;

  func_source = i_match->i_func;
  func_aspect = ldv_convert_func_signature_to_internal (pps_func);

  /* Set an aspect function declaration. */
  i_match->i_func_aspect = func_aspect;

  /* Compare functions names. */
  if (ldv_cmp_str (func_aspect->name, ldv_cpp_get_id_name (func_source->name)))
    return false;

  /* Replace aspect function name used just for a current matching with the source
     one since they match each other but the aspect one can contain '$'
     wildcards.*/
  func_aspect->name = func_source->name;

  /* Specify that a function was matched by name. */
  i_match->ismatched_by_name = true;

  /* Compare functions types. */
  if (!ldv_match_type (func_source->type, func_aspect->type))
    return false;

  /* Specify that a function was matched by a whole signature not just by a
     name. */
  i_match->ismatched_by_name = false;

  /* A function signature is matched. */
  return true;
}

bool
ldv_match_macro_signature (ldv_i_match_ptr i_match, ldv_pps_macro_ptr pps_macro)
{
  ldv_i_macro_ptr macro_source = NULL, macro_aspect = NULL;
  ldv_list_ptr i_macro_param_first_list = NULL, i_macro_param_second_list = NULL;
  ldv_id_ptr i_macro_param_first = NULL, i_macro_param_second = NULL;

  macro_source = i_match->i_macro;
  macro_aspect = ldv_convert_macro_signature_to_internal (pps_macro);

  /* Set an aspect macro. */
  i_match->i_macro_aspect = macro_aspect;

  /* Compare macro kinds. */
  if (i_match->i_macro->macro_kind != i_match->i_macro_aspect->macro_kind)
    return false;

  /* Compare macro names. */
  if (ldv_cmp_str (macro_aspect->macro_name, ldv_cpp_get_id_name (macro_source->macro_name)))
    return false;

  /* Replace aspect macro name used just for a current matching with the source
     one since they match each other but the aspect one can contain '$'
     wildcards.*/
  macro_aspect->macro_name = macro_source->macro_name;

  /* Specify that a macro was matched by a name. */
  i_match->ismatched_by_name = true;

  /* Macro parameters (if so) are matched just in case when '$' wildcard is used
     in them because of in this case we need to replace aspect parameter names
     with the source ones. Otherwise aspect parameter name is taken. */
  for (i_macro_param_first_list = macro_source->macro_param, i_macro_param_second_list = macro_aspect->macro_param
    ; i_macro_param_first_list && i_macro_param_second_list
    ; i_macro_param_first_list = ldv_list_get_next (i_macro_param_first_list), i_macro_param_second_list = ldv_list_get_next (i_macro_param_second_list))
    {
      i_macro_param_first = (ldv_id_ptr) ldv_list_get_data (i_macro_param_first_list);
      i_macro_param_second = (ldv_id_ptr) ldv_list_get_data (i_macro_param_second_list);

      if (i_macro_param_second->isany_chars)
        {
          if (ldv_cmp_str (i_macro_param_second, ldv_cpp_get_id_name (i_macro_param_first)))
            return false;

          ldv_list_set_data (i_macro_param_second_list, i_macro_param_first);
        }
    }

  /* I.e. the numbers of macro parameters aren't equal. */
  if (i_macro_param_first_list || i_macro_param_second_list)
    return false;

  /* Specify that a macro was matched by a whole signature not just by a
     name. */
  i_match->ismatched_by_name = false;

  return true;
}

bool
ldv_match_param_type (ldv_i_type_ptr arg_type_first, ldv_i_type_ptr arg_type_second)
{
  /* Don't compare first base types of function argument types beginning
   * with an array type (since it's converted to a pointer type by gcc). */
  if (arg_type_first->it_kind == LDV_IT_PTR && arg_type_second->it_kind == LDV_IT_ARRAY)
    {
      /* Compare shifted arguments types. */
      if (!ldv_match_type (arg_type_first->ptr_type, arg_type_second->array_type))
        return false;
    }
  /* Gcc add a pointer to a function type if an argument has a function type.
     So, skip this artificial gcc pointer. */
  else if (arg_type_first->it_kind == LDV_IT_PTR && arg_type_second->it_kind == LDV_IT_FUNC)
    {
      /* Compare shifted arguments types. */
      if (!ldv_match_type (arg_type_first->ptr_type, arg_type_second))
        return false;
    }
  else
    {
      /* Compare variable lengths of function arguments. */
      if (!ldv_match_bool (arg_type_first->isva, arg_type_second->isva))
        return false;

      /* Compare arguments types. */
      if (!ldv_match_type (arg_type_first, arg_type_second))
        return false;
    }

  return true;
}

bool
ldv_match_type (ldv_i_type_ptr first, ldv_i_type_ptr second)
{
  ldv_i_param_ptr param_first = NULL, param_second = NULL, param_second_next = NULL;
  ldv_list_ptr param_first_list = NULL, param_second_list = NULL, param_second_list_next = NULL;
  bool isany_params = false;
  bool **param_matching_table = NULL;
  unsigned int param_source_numb, param_aspect_numb, any_params_numb;
  unsigned int i, j, j_first_matched;
  ldv_i_param_ptr *params_first = NULL;
  bool ismatched;
  typedef struct ldv_coord_internal
  {
    unsigned int x;
    unsigned int y;
  } ldv_coord;
  typedef ldv_coord *ldv_coord_ptr;
  ldv_coord_ptr matching_table_coord = NULL, matching_table_coord_next = NULL, matching_table_coord_prev = NULL;
  ldv_list_ptr matching_table_coord_list = NULL;
  ldv_list_ptr matching_table_coord_list_cur = NULL;

  /* Separately matches universal type specifier with nonprimitive source type.
     At this point they are matched unconditionally. Note that universal type
     specifier doesn't match pointer or array if standard type specifier is
     specified togther with it. */
  if ((first->it_kind == LDV_IT_ARRAY || first->it_kind == LDV_IT_PTR)
    && second->it_kind == LDV_IT_PRIMITIVE && second->primitive_type->isuniversal_type_spec
    && !second->primitive_type->isvoid && !second->primitive_type->ischar
    && !second->primitive_type->isint && !second->primitive_type->isfloat
    && !second->primitive_type->isdouble && !second->primitive_type->isbool
    && !second->primitive_type->iscomplex && !second->primitive_type->isshort
    && !second->primitive_type->islong && !second->primitive_type->islonglong
    && !second->primitive_type->issigned && !second->primitive_type->isunsigned
    && !second->primitive_type->isstruct && !second->primitive_type->isunion
    && !second->primitive_type->isenum && !second->primitive_type->istypedef_name)
    {
      /* Replace aspect type used just for a current matching with the source
         one since they match each other but the aspect one can contain '$'
         universal type specifier.*/
      memcpy (second, first, sizeof (*second));

      return true;
    }

  /* Type kinds must be equal. */
  if (first->it_kind != second->it_kind)
    return false;

  switch (first->it_kind)
    {
    case LDV_IT_ARRAY:
      /* Compare array sizes. */
      if (second->isany_size)
        second->array_size = first->array_size;
      else if ((first->issize_specified && !second->issize_specified)
        || (!first->issize_specified && second->issize_specified)
        || first->array_size != second->array_size)
        return false;

      /* Compare arrays elements types. */
      if (!ldv_match_type (first->array_type, second->array_type))
        return false;

      break;

    case LDV_IT_FUNC:
      /* Compare functions return types. */
      if (!ldv_match_type (first->ret_type, second->ret_type))
        return false;

      /* See whether aspect function argument types contain any parameters
         wildcard and count the number of them. Note that continuous '..'
         wildcards sequences are removed here (each of them is joined together
         into one '..' wildcard. */
      for (param_second_list = second->param, any_params_numb = 0
        ; param_second_list
        ; param_second_list = ldv_list_get_next (param_second_list))
        {
          param_second = (ldv_i_param_ptr) ldv_list_get_data (param_second_list);

          if (param_second->isany_params)
            {
              isany_params = true;
              any_params_numb++;

              /* Remove following '..' wildcards. */
              while ((param_second_list_next = ldv_list_get_next (param_second_list)))
                {
                  param_second_next = (ldv_i_param_ptr) ldv_list_get_data (param_second_list_next);

                  if (param_second_next->isany_params)
                    ldv_list_delete (&param_second_list, param_second_list_next);
                  else
                      break;
                }
            }
        }

      /* Perform a simple element-by-element matching when there is no any
         parameters wildcard. */
      if (!isany_params)
        {
          /* Compare functions arguments types. */
          for (param_first_list = first->param, param_second_list = second->param
            ; param_first_list || param_second_list
            ; param_first_list = ldv_list_get_next (param_first_list), param_second_list = ldv_list_get_next (param_second_list))
            {
              /* Obtain a list data if it's present. */
              if (param_first_list)
                param_first = (ldv_i_param_ptr) ldv_list_get_data (param_first_list);

              if (param_second_list)
                param_second = (ldv_i_param_ptr) ldv_list_get_data (param_second_list);

              /* In the case when the numbers of function arguments are different. */
              if ((!param_first_list && param_second_list)
                || (param_first_list && !param_second_list))
                return false;

              if (!ldv_match_param_type (param_first->type, param_second->type))
                return false;
            }
        }
      /* Create a special table and try to find a greedy path in it to provide
         function arguments types matching. */
      else
        {
          /* Obtain sizes of the matching table. Note that source parameters are
             along x direction while aspect parameters are along y direction. */
          param_source_numb = ldv_list_len (first->param);
          param_aspect_numb = ldv_list_len (second->param);

          /* Finish if the number of specified parameters (that are not '..'
             wildcards) is greater then the number of source parameters. */
          if (param_aspect_numb > param_source_numb + any_params_numb)
            return false;

          /* If there is no source function parameters, then aspect function
             parameters may be just a '..' wildcard. Just remove it in this
             case. */
          if (param_source_numb == 0)
            {
              if (param_aspect_numb != any_params_numb)
                return false;

              second->param = NULL;

              /* Specify that types are equal. */
             return true;
            }

          /* Otherwise create the special matching table, try to find a 'true'
             path in it and replace '..' wildcards with real arguments when a
             path was found.
             First of all allocate and initialize the matching table. At the
             beginning allocate rows memory (corresponding to aspect parameters)
             along y direction. */
          param_matching_table = XCNEWVEC (bool *, param_aspect_numb);

          for (i = 0; i < param_aspect_numb; i++)
            {
              /* Fox each aspect parameter allocate all source parameters along
                 x direction. */
              param_matching_table[i] = XCNEWVEC (bool, param_source_numb);

              /* Initialize the whole matching table with false values at the
                 beginning. */
              for (j = 0; j < param_source_numb; j++)
                param_matching_table[i][j] = false;
            }

          /* Create a dynamic array for source parameters to optimize a bit. */
          params_first = XCNEWVEC (ldv_i_param_ptr, param_source_numb);

          /* Initialize the created array. */
          for (j = 0, param_first_list = first->param
            ; j < param_source_numb
            ; j++, param_first_list = ldv_list_get_next (param_first_list))
            {
              param_first = (ldv_i_param_ptr) ldv_list_get_data (param_first_list);
              params_first[j] = param_first;
            }

          /* Fill the table by going through its rows (directly from the first
             to the last one) and updating information for them in accordence
             with a previously obtained information. Note that the first row is
             considered especially since there isn't a previous row for it. */
          for (i = 0, j_first_matched = 0, param_second_list = second->param
            ; i < param_aspect_numb
            ; i++, param_second_list = ldv_list_get_next (param_second_list))
            {
              param_second = (ldv_i_param_ptr) ldv_list_get_data (param_second_list);

              /* The first row is considered in the special way since there
                 isn't a previous information for it. */
              if (i == 0)
                {
                  /* Mark all first row elements as true if there is '..'
                     wildcard at the first place. */
                  if (param_second->isany_params)
                    {
                      for (j = 0; j < param_source_numb; j++)
                        param_matching_table[i][j] = true;
                    }
                  /* Otherwise the first row element must coinside with the
                     first column element. */
                  else if (ldv_match_param_type (params_first[0]->type, param_second->type))
                    {
                      param_matching_table[i][0] = true;
                      j_first_matched++;
                    }
                  /* Parameters don't coinside. */
                  else
                    return false;
                }
              /* All next rows are processed in accordance with the previous
                 ones. Note that we see just on a previous row for each row. */
              else
                {
                  /* Mark row elements as true if there is '..' wildcard. Begin
                     with an element that has a marked previous (left-up)
                     element. */
                  if (param_second->isany_params)
                    {
                      for (j = j_first_matched; j < param_source_numb; j++)
                        param_matching_table[i][j] = true;
                    }
                  /* Otherwise try to find all correspondences to column
                     elements for a given row element. Also begin with an
                     element that has a marked previous (left-up) element. Mark
                     elements that correspond to each other and that have a
                     previosly marked left-up or up element. Finish if can't
                    find correspondence. */
                  else
                    {
                      ismatched = false;

                      for (j = j_first_matched; j < param_source_numb; j++)
                        {
                          /* Compare an element from a given row with an element
                             from a given column just in case when it may become
                             a next element in a chain of trues. */
                          if (j == 0 || param_matching_table[i - 1][j - 1] == true
                            || param_matching_table[i - 1][j] == true)
                            {
                              if (ldv_match_param_type (params_first[j]->type, param_second->type))
                                {
                                  param_matching_table[i][j] = true;

                                  /* We may see following rows beginning with
                                     a place of a first match to a real source
                                     function parameter. */
                                  if (!ismatched)
                                    j_first_matched = j + 1;

                                  ismatched = true;
                                }
                            }
                        }

                      /* Can't find a correspondence in a given row. */
                      if (!ismatched)
                        return false;
                    }
                }
            }

          /* Dump the matching table if needed. */
          if (ldv_cpp_isinfo_matching_table)
            {
              fprintf (stderr, "MATCHING TABLE (OX - SOURCE ARGUMENT, OY - ASPECT ARGUMENTS):\n");

              fprintf (stderr, "    ");

              /* Print Ox axis signatures. */
              for (j = 0; j < param_source_numb; j++)
                fprintf (stderr, "%2d  ", (j + 1));

              fprintf (stderr, "\n");

              /* Iterate over all matching table rows. */
              for (i = 0; i < param_aspect_numb; i++)
                {
                  /* Print Oy axis signatures. */
                  fprintf (stderr, "%2d: ", (i + 1));

                  /* Iterate over all matching table columns. */
                  for (j = 0; j < param_source_numb; j++)
                    {
                      fprintf (stderr, "%2d", (int)param_matching_table[i][j]);

                      /* Print the sepator between elements. */
                      if (j != param_source_numb - 1)
                        fprintf (stderr, ", ");
                    }

                  fprintf (stderr, "\n");
                }

              fprintf (stderr, "\n");
            }

          /* The matching table contains a correspondence between aspect
             function arguments types and the source ones. So find a 'true' way
             in it from the end to the beginning and remember this way. */
          matching_table_coord = XCNEW (ldv_coord);
          j = param_source_numb - 1;
          i = param_aspect_numb - 1;

          /* Skip a '..' wildcard that catches nothing and that is placed at the
             last row. */
          param_second_list = ldv_list_get_last (second->param);
          param_second = (ldv_i_param_ptr) ldv_list_get_data (param_second_list);

          if (param_second->isany_params && !param_matching_table[i][j])
            i--;

          matching_table_coord->x = j;
          matching_table_coord->y = i;

          /* Push a last 'true' element implicitly since then we will add
             a previous element for a current element each time. */
          matching_table_coord_list = ldv_list_push_front (matching_table_coord_list, matching_table_coord);

          /* Build the rest of the 'true' path. */
          while (1)
            {
              /* Finish building of the 'true' path when there is just one
                 parameter among source and aspect function declarations. */
              if (i == 0 && j == 0)
                break;

              matching_table_coord = XCNEW (ldv_coord);

              /* For other rows make a decision that corresponds to a previous
                 row. In depend on it following combinations may take place
                 (note, that their priority decreases; x means either 0 or 1; we
                 must follow in a pointed direction):
                  1. "Next parameter"
                      1 0 x
                       \
                      x 1 x
                  2. "'..' wildcard finishes"
                      x 1 x
                        |
                      x 1 x
                  3. "'..' wildcard catches"
                      0    0  x
                      1 <- 1  x
                      or
                      1 <- 1  x (for a walking through the first row)
               */
              /* If we achieve the first column then it must contain true from
                 the beginning to a last obtained row number. Note that it may
                 be just 1 since there isn't continuos sequences of '..'
                 wildcards in the matching table at the moment. */
              if (j == 0 && i > 0 && !param_matching_table[i - 1][j])
                {
                  LDV_CPP_FATAL_ERROR ("Can't find 'true' path in the first column of matching table");
                }

              /* "Next parameter". */
              if (i > 0 && j > 0 && param_matching_table[i - 1][j - 1])
                {
                  i--;
                  j--;
                }
              /* "'..' wildcard finishes". */
              else if (i > 0 && param_matching_table[i - 1][j])
                i--;
              /* "'..' wildcard catches". */
              else if (j > 0 && param_matching_table[i][j - 1])
                j--;
              else
                {
                  LDV_CPP_FATAL_ERROR ("Can't find 'true' path in the matching table");
                }

              /* Push a current element of the matching table to the beginning
                 of the 'true' way. */
              matching_table_coord->x = j;
              matching_table_coord->y = i;

              matching_table_coord_list = ldv_list_push_front (matching_table_coord_list, matching_table_coord);

              /* Finish when we achieve the first matching table element. */
              if (i == 0 && j == 0)
                break;
            }

          /* Dump the 'true' path if needed. */
          if (ldv_cpp_isinfo_matching_table)
            {
              fprintf (stderr, "'TRUE' PATH IN MATCHING TABLE:\n");

              /* Iterate over all elements of the 'true' path. */
              for (matching_table_coord_list_cur = matching_table_coord_list
                ; matching_table_coord_list_cur
                ; matching_table_coord_list_cur = ldv_list_get_next (matching_table_coord_list_cur))
                {
                  matching_table_coord = (ldv_coord_ptr) ldv_list_get_data (matching_table_coord_list_cur);

                  fprintf (stderr, "(%d, %d)", (matching_table_coord->x + 1), (matching_table_coord->y + 1));

                  /* Print separators between path elements. */
                  if (ldv_list_get_next (matching_table_coord_list_cur))
                    fprintf (stderr, " -> ");
                }

              fprintf (stderr, "\n\n");
            }

          if (ldv_cpp_isinfo_matching_table)
            fprintf (stderr, "MATCHES BETWEEN ARGUMENTS TYPES OF SOURCE AND ASPECT FUNCTION DECLARATIONS:\n");

          /* Replace '..' wildcards with real source function arguments types to
             which they correspond. So walk through aspect and source arguments
             and the 'true' way lists. */
          for (matching_table_coord_list_cur = matching_table_coord_list, param_second_list = second->param, i = 0, j = 0
            ; matching_table_coord_list_cur && j < param_source_numb
            ; matching_table_coord_list_cur = ldv_list_get_next (matching_table_coord_list_cur), matching_table_coord_prev = matching_table_coord)
            {
              matching_table_coord = (ldv_coord_ptr) ldv_list_get_data (matching_table_coord_list_cur);

              if (ldv_cpp_isinfo_matching_table)
                fprintf(stderr, "CURRENT 'TRUE' PATH ELEMENT: (%d, %d)\n", (matching_table_coord->x + 1), (matching_table_coord->y + 1));

              /* See on a next 'true' path element to understand when '..'
                 wildcard finishes. */
              if (ldv_list_get_next (matching_table_coord_list_cur))
                matching_table_coord_next = (ldv_coord_ptr) ldv_list_get_data (ldv_list_get_next (matching_table_coord_list_cur));
              else
                matching_table_coord_next = NULL;

              param_second = (ldv_i_param_ptr) ldv_list_get_data (param_second_list);

              if (ldv_cpp_isinfo_matching_table)
                {
                  fprintf(stderr, "SOURCE ARGUMENT NUMBER: %d\n", (j + 1));
                  fprintf(stderr, "ASPECT ARGUMENT NUMBER: %d (IS '..' WILDCARD: %d)\n", (i + 1), (int)param_second->isany_params);
                }

              /* See on whether we at the first element of the 'true' path or
                 not. */
              if (matching_table_coord_prev)
                {
                  /* Replace a '..' wildcard with an appropriate source function
                     argument if so. */
                  if (param_second->isany_params)
                    {
                      /* Delete a '..' wildcard list element. Note that it can't
                         be on the first place of aspect function agruments
                         (this case is considered below), so the list_delete
                         function always must return a non 0 element. */
                      param_second_list = ldv_list_delete (&second->param, param_second_list);

                      /* Replace a '..' wildcard with some data just in case
                         when some data corresponds to it. */
                      if ((matching_table_coord_next
                        && matching_table_coord->x != matching_table_coord_next->x)
                        || (!matching_table_coord_next && matching_table_coord_prev))
                        {
                          param_second_list = ldv_list_insert_data (param_second_list, params_first[j]);
                          j++;
                        }

                      /* Shift an aspect function argument when a next element
                         of the 'true' path has a larger y coordinate (i.e. a
                         '..' wildcard finishes). */
                      if ((matching_table_coord_next
                        && matching_table_coord->y != matching_table_coord_next->y)
                        || (!matching_table_coord_next && matching_table_coord_prev
                        && matching_table_coord->y != matching_table_coord_prev->y))
                        {
                          param_second_list = ldv_list_get_next (param_second_list);
                          i++;
                        }
                    }
                  /* So we are under a '..' wildcard action. Push a next element
                     from a source function arguments list to an aspect function
                     arguments list. */
                  else if (matching_table_coord->y == matching_table_coord_prev->y)
                    {
                      param_second_list = ldv_list_insert_data (param_second_list, params_first[j]);
                      j++;

                      /* Shift an aspect function argument when a next true
                         element of the 'true' path has a larger y coordinate
                         (i.e. a '..' wildcard finishes). Also do this when a
                         '..' wildcard catches a last source function
                         argument. */
                      if ((matching_table_coord_next
                        && matching_table_coord->y != matching_table_coord_next->y)
                        || !matching_table_coord_next)
                        {
                          param_second_list = ldv_list_get_next (param_second_list);
                          i++;
                        }
                    }
                  /* Just go to next arguments since they're equal. */
                  else
                    {
                      param_second_list = ldv_list_get_next (param_second_list);
                      i++;
                      j++;
                    }
                }
              else
                {
                  /* So a first aspect function argument is '..' wildcard.
                     Replace it with an appropriate source function argument if
                     so. */
                  if (param_second->isany_params)
                    {
                      /* Delete a '..' wildcard list element. */
                      ldv_list_delete (&second->param, param_second_list);

                      /* Replace a '..' wildcard with some data just in case
                         when some data corresponds to a '..' wildcard. */
                      if (matching_table_coord_next)
                        {
                          /* Push a first source function argument to an aspect
                             function arguments list instead of a '..'
                             wildcard. */
                          if (matching_table_coord->x != matching_table_coord_next->x)
                            {
                              param_second_list = second->param = ldv_list_push_front (second->param, params_first[j]);
                              j++;

                              /* Finalize a '..' wildcard. */
                              if (matching_table_coord->y != matching_table_coord_next->y)
                                {
                                  param_second_list = ldv_list_get_next (param_second_list);
                                  i++;
                                }
                            }
                          /* Go to a next aspect function argument since a '..'
                             wildcard catches nothing. */
                          else
                            {
                              param_second_list = second->param;
                              i++;
                            }
                        }
                      /* There is the only '..' wildcard that catches the only
                         source function argument. */
                      else
                        {
                          ldv_list_push_back (&second->param, params_first[j]);
                          param_second_list = second->param;
                          param_second_list = ldv_list_get_next (param_second_list);
                          i++;
                          j++;
                        }
                    }
                  /* Just go to next arguments since they're equal. */
                  else
                    {
                      param_second_list = ldv_list_get_next (param_second_list);
                      i++;
                      j++;
                    }
                }
            }

          /* Remove a trailing '..' wildcard that catches nothing. */
          if (param_second_list)
            {
              param_second = (ldv_i_param_ptr) ldv_list_get_data (param_second_list);

              if (param_second->isany_params)
                /* Delete a '..' wildcard list element. */
                ldv_list_delete (&second->param, param_second_list);
              else
                {
                  LDV_CPP_FATAL_ERROR ("Meet something that isn't '..' wildcard that catches nothing at the end of arguments lists");
                }
            }

          if (ldv_cpp_isinfo_matching_table)
            fprintf (stderr, "\n");
        }

      /* Function parameter names (if so) are matched just in case when '$'
         wildcard is used in them because of in this case we need to replace
         aspect parameter names with the source ones. Otherwise aspect parameter
         name is taken. */
      for (param_first_list = first->param, param_second_list = second->param
        ; param_first_list && param_second_list
        ; param_first_list = ldv_list_get_next (param_first_list), param_second_list = ldv_list_get_next (param_second_list))
        {
          param_first = (ldv_i_param_ptr) ldv_list_get_data (param_first_list);
          param_second = (ldv_i_param_ptr) ldv_list_get_data (param_second_list);

          if (param_second->name && param_second->name->isany_chars)
            {
              /* If there isn't source parameter name (e.g. for function
                 prototype) then simply ignore the aspect one with '$'
                 wildcards since we can't match it. After all artificial
                 name will be generated. */
              if (param_first->name && ldv_cmp_str (param_second->name, ldv_cpp_get_id_name (param_first->name)))
                return false;

              ldv_list_set_data (param_second_list, param_first);
            }
        }

      break;

    case LDV_IT_PRIMITIVE:
      /* Compare declarations specificators. */
      if (!ldv_match_declspecs (first->primitive_type, second->primitive_type))
        return false;

      /* Merge aspect declaration specifiers used just for a current matching
         with the source one since they match each other but the aspect one can
         contain '$' universal type specifier.*/
      second->primitive_type = ldv_merge_declspecs (first->primitive_type, second->primitive_type, true);

      break;

    case LDV_IT_PTR:
      /* Compare pointers qualifiers. */
      if (first->isconst != second->isconst || first->isvolatile != second->isvolatile || first->isrestrict != second->isrestrict)
        return false;

      /* Compare pointered types. */
      if (!ldv_match_type (first->ptr_type, second->ptr_type))
        return false;

      break;

    default:
      LDV_CPP_FATAL_ERROR ("incorrect type information kind \"%d\" is used", first->it_kind);
    }

  /* Types are equal. */
  return true;
}

bool
ldv_match_typedecl_signature (ldv_i_match_ptr i_match, ldv_pps_decl_ptr pps_typedecl)
{
  ldv_i_typedecl_ptr typedecl_source = NULL, typedecl_aspect = NULL;

  typedecl_source = i_match->i_typedecl;
  typedecl_aspect = ldv_convert_typedecl_signature_to_internal (pps_typedecl);

  /* Set an aspect type declaration. */
  i_match->i_typedecl_aspect = typedecl_aspect;

  /* Compare type declaration names. */
  if (ldv_cmp_str (typedecl_aspect->name, ldv_cpp_get_id_name (typedecl_source->name)))
    return false;

  /* Replace aspect type declaration name used just for a current matching with
     the source one since they match each other but the aspect one can contain
     '$' wildcards.*/
  typedecl_aspect->name = typedecl_source->name;

  /* Specify that a type declaration was matched by a name. */
  i_match->ismatched_by_name = true;

  /* Compare a type declaration type type. */
  if (typedecl_source->itd_kind != typedecl_aspect->itd_kind)
    return false;

  /* Specify that a type declaration was matched by a whole signature not just
     by a name. */
  i_match->ismatched_by_name = false;

  return true;
}

bool
ldv_match_universal_bool (bool first, bool second, bool universal)
{
  /* Matches first and second booleans (type specifiers). Also matches universal
     boolean with the first one if the second one isn't specified. The latter
     means that universal type specifier matches any non specified declaration
     specifier. */
  return ((first && second) || (!first && !second) || (first && !second && universal));
}

bool
ldv_match_var_signature (ldv_i_match_ptr i_match, ldv_pps_decl_ptr pps_var)
{
  ldv_i_var_ptr var_source = NULL, var_aspect = NULL;

  var_source = i_match->i_var;
  var_aspect = ldv_convert_var_signature_to_internal (pps_var);

  /* Set an aspect variable declaration. */
  i_match->i_var_aspect = var_aspect;

  /* Compare variable names. */
  if (ldv_cmp_str (var_aspect->name, ldv_cpp_get_id_name (var_source->name)))
    return false;

  /* Replace aspect variable name used just for a current matching with the
     source one since they match each other but the aspect one can contain '$'
     wildcards.*/
  var_aspect->name = var_source->name;

  /* Specify that a variable declaration was matched by a name. */
  i_match->ismatched_by_name = true;

  /* Compare variables types. */
  if (!ldv_match_type (var_source->type, var_aspect->type))
    return false;

  /* Specify that a variable declaration was matched by a whole signature not
     just by a name. */
  i_match->ismatched_by_name = false;

  return true;
}

void
ldv_set_isprint_signature_of_matched_by_name (bool gcc_isprint_signature_of_matched_by_name)
{
  /* Set a flag from gcc to cpp. */
  ldv_cpp_isprint_signature_of_matched_by_name = gcc_isprint_signature_of_matched_by_name;
}
