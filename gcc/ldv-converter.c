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

/* For error functions. */
#include "toplev.h"

/* Tree conceptions. */
#include "diagnostic-core.h"
#include "tree.h"

#include "ldv-advice-weaver.h"
#include "ldv-aspect-types.h"
#include "ldv-converter.h"
#include "ldv-core.h"
#include "ldv-pointcut-matcher.h"

#include "c-family/ldv-pretty-print.h"


ldv_pps_decl_ptr
ldv_convert_internal_to_declaration (ldv_i_type_ptr type, const char *decl_name)
{
  ldv_pps_decl_ptr pps_decl = NULL;
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;

  pps_decl = ldv_create_pps_decl ();

  /* Convert an internal type representation to a declarator chain. Declaration
     specifiers and a declaration name will be processed below. */
  declarator_list = ldv_convert_internal_to_declarator (type);

  /* Create a declarator for a declaration name. */
  declarator = ldv_create_declarator ();

  if (declarator_list)
    {
      /* Add a first declarator of a declarator chain to a declaration
         declarator. */
      pps_decl->pps_declarator = declarator_list;

      /* Go to the end of a declarator chain to add a declaration name. */
      declarator_list = ldv_list_get_last (declarator_list);

      ldv_list_push_back (&declarator_list, declarator);
    }
  /* Use a name declarator for a declaration declarator if there is no
     declarators in a declarator chain. */
  else
    {
      ldv_list_push_back (&declarator_list, declarator);
      pps_decl->pps_declarator = declarator_list;
    }

  /* Create a name for a declaration. */
  if (decl_name)
    {
      declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_ID;
      declarator->declarator_name = ldv_create_id ();
      ldv_puts_id (decl_name, declarator->declarator_name);
    }
  /* Create an abstract declaration. */
  else
    declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_NONE;

  /* Add declaration specifiers. Declaration specifiers is a primitive type in
     a chain of base types of an initial type. */
  pps_decl->pps_declspecs = ldv_convert_internal_to_declspecs (type);

  return pps_decl;
}

ldv_list_ptr
ldv_convert_internal_to_declarator (ldv_i_type_ptr type)
{
  ldv_list_ptr list = NULL, list_reverse = NULL;

  /* Obtain a reverse declarator chain. */
  list_reverse = ldv_convert_internal_to_declarator_reverse (type);

  /* Obtain a convinient declarator chain. */
  list = ldv_list_reverse (list_reverse);

  return list;
}

ldv_list_ptr
ldv_convert_internal_to_declarator_reverse (ldv_i_type_ptr type)
{
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;
  ldv_i_param_ptr param = NULL;
  ldv_list_ptr param_list = NULL;
  const char *param_name;
  unsigned int param_numb;
  ldv_pps_func_arg_ptr func_arg_new = NULL;

  declarator = ldv_create_declarator ();
  ldv_list_push_back (&declarator_list, declarator);

  switch (type->it_kind)
    {
    case LDV_IT_ARRAY:
      declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_ARRAY;

      declarator->pps_array_size = ldv_create_pps_array_size ();

      /* Convert an array size. */
      declarator->pps_array_size->issize_specified = type->issize_specified;
      declarator->pps_array_size->isany_size = type->isany_size;

      if (type->issize_specified)
        declarator->pps_array_size->pps_array_size = type->array_size;

      /* Convert an array elements type. */
      ldv_list_splice (declarator_list, ldv_convert_internal_to_declarator_reverse (type->array_type));

      break;

    case LDV_IT_FUNC:
      declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_FUNC;

      /* Convert a function return type. */
      ldv_list_splice (declarator_list, ldv_convert_internal_to_declarator_reverse (type->ret_type));

      /* Convert function parameters. */
      for (param_list = type->param, param_numb = 1
        ; param_list
        ; param_list = ldv_list_get_next (param_list), param_numb++)
        {
          param = (ldv_i_param_ptr) ldv_list_get_data (param_list);

          func_arg_new = ldv_create_pps_func_arg ();

          ldv_list_push_back (&declarator->func_arg_list, func_arg_new);

          /* Check whether an any parameters wildcard is encountered. */
          if (param->isany_params)
            {
              func_arg_new->isany_params = true;
              continue;
            }

          /* Parameter may have not a name (abstract declaration). */
          param_name = param->name ? ldv_get_id_name (param->name) : NULL;

          func_arg_new->pps_func_arg = ldv_convert_internal_to_declaration (param->type, param_name);

          /* Convert a variable parameter length. */
          func_arg_new->isva = param->type->isva;
        }

      break;

    case LDV_IT_PRIMITIVE:
      /* A primitive type corresponds to declaration specifiers. And it's
         processed separately. */
      return NULL;

    case LDV_IT_PTR:
      declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_PTR;

      /* Convert pointer qualifiers. */
      declarator->pps_ptr_quals = ldv_create_ptr_quals ();

      declarator->pps_ptr_quals->isconst = type->isconst;
      declarator->pps_ptr_quals->isvolatile = type->isvolatile;
      declarator->pps_ptr_quals->isrestrict = type->isrestrict;

      /* Convert a pointed type. */
      ldv_list_splice (declarator_list, ldv_convert_internal_to_declarator_reverse (type->ptr_type));

      break;

    default:
      LDV_FATAL_ERROR ("incorrect type information kind \"%d\" is used", type->it_kind);
    }

  return declarator_list;
}

ldv_pps_declspecs_ptr
ldv_convert_internal_to_declspecs (ldv_i_type_ptr type)
{
  switch (type->it_kind)
    {
    case LDV_IT_ARRAY:
      /* Get declaration specifiers from an array element type. */
      return ldv_convert_internal_to_declspecs (type->array_type);

    case LDV_IT_FUNC:
      /* Get declaration specifiers from a function return type. */
      return ldv_convert_internal_to_declspecs (type->ret_type);

    case LDV_IT_PRIMITIVE:
      /* A primitive type corresponds to declaration specifiers. */
      return ldv_copy_declspecs (type->primitive_type);

    case LDV_IT_PTR:
      /* Get declaration specifiers from a pointed type. */
      return ldv_convert_internal_to_declspecs (type->ptr_type);

    default:
      LDV_FATAL_ERROR ("incorrect type information kind \"%d\" is used", type->it_kind);
    }
}

ldv_list_ptr
ldv_convert_initializer_to_internal (tree initializer_tree)
{
  ldv_list_ptr initializer_list = NULL;
  ldv_i_initializer_ptr initializer = NULL;
  ldv_i_var_ptr artificial_decl = NULL, artificial_ret_type_decl = NULL, artificial_param_decl = NULL;
  unsigned HOST_WIDE_INT ix;
  tree index, value;
  ldv_list_ptr params = NULL;
  ldv_i_param_ptr param = NULL;

  /* Do nothing if there is no tree node implementing initializer. */
  if (!initializer_tree)
    return NULL;

  if (TREE_CODE (initializer_tree) == CONSTRUCTOR)
    {
      FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (initializer_tree), ix, index, value)
        {
          initializer = ldv_create_info_initializer ();

          if (TREE_CODE (index) == FIELD_DECL)
            {
              initializer->field_name = DECL_NAME (index)
                ? (const char *) (IDENTIFIER_POINTER (DECL_NAME (index)))
                : "";
            }
          else if (TREE_CODE (index) == INTEGER_CST)
            {
              initializer->isarray_index = true;
              initializer->array_index = TREE_INT_CST_LOW (index);
            }
          else
            {
              LDV_FATAL_ERROR ("can't find neiter initialized structure field nor array element");
            }

          /* Create artificial declaration corresponding to structure field or
             array element. */
          artificial_decl = ldv_create_info_var ();
          artificial_decl->name = ldv_create_id ();
          ldv_puts_id ("%s", artificial_decl->name);
          artificial_decl->type = TREE_CODE (index) == FIELD_DECL
            ? ldv_convert_type_tree_to_internal (TREE_TYPE (index), NULL)
            : ldv_convert_type_tree_to_internal (TREE_TYPE (TREE_TYPE (initializer_tree)), NULL);
          initializer->decl = ldv_print_var_decl (artificial_decl);

          if (artificial_decl->type->it_kind == LDV_IT_PRIMITIVE)
            {
              if (artificial_decl->type->primitive_type->isstruct)
                initializer->type = "structure";
              else
                initializer->type = "primitive";
            }
          else if (artificial_decl->type->it_kind == LDV_IT_PTR)
            {
              if (artificial_decl->type->ptr_type->it_kind == LDV_IT_PRIMITIVE)
                {
                  if (artificial_decl->type->ptr_type->primitive_type->isstruct)
                    initializer->type = "pointer to structure variable";
                  else
                    initializer->type = "primitive pointer";
                }
              else if (artificial_decl->type->ptr_type->it_kind == LDV_IT_FUNC)
                {
                  initializer->type = "function pointer";

                  /* Store information on pointed function return type and argument types. */
                  artificial_ret_type_decl = ldv_create_info_var ();
                  artificial_ret_type_decl->name = ldv_create_id ();
                  ldv_puts_id ("%s", artificial_ret_type_decl->name);
                  artificial_ret_type_decl->type = artificial_decl->type->ptr_type->ret_type;
                  initializer->pointed_func_ret_type_decl = ldv_print_var_decl (artificial_ret_type_decl);

                  for (params = artificial_decl->type->ptr_type->param
                    ; params
                    ; params = ldv_list_get_next (params))
                    {
                      param = (ldv_i_param_ptr) ldv_list_get_data (params);

                      artificial_param_decl = ldv_create_info_var ();
                      artificial_param_decl->name = ldv_create_id ();
                      ldv_puts_id ("%s", artificial_param_decl->name);
                      artificial_param_decl->type = param->type;
                      ldv_list_push_back (&initializer->pointed_func_arg_type_decls, xstrdup (ldv_print_var_decl (artificial_param_decl)));
                    }
                }
              else
                initializer->type = "pointer to pointer";
            }
          else if (artificial_decl->type->it_kind == LDV_IT_ARRAY)
            initializer->type = "array";
          else
            {
              LDV_FATAL_ERROR ("can't process type");
            }

          ldv_free_info_var (artificial_decl);

          if (TREE_CODE (value) == CONSTRUCTOR)
            initializer->initializer = ldv_convert_initializer_to_internal (value);
          else
            initializer->value = ldv_convert_and_print_assignment_expr (value);

          ldv_list_push_back (&initializer_list, initializer);
        }
    }
  else
    {
      initializer = ldv_create_info_initializer ();
      initializer->value = ldv_convert_and_print_assignment_expr (initializer_tree);
      ldv_list_push_back (&initializer_list, initializer);
    }

  return initializer_list;
}

ldv_i_type_ptr
ldv_convert_type_tree_to_internal (tree type_tree, tree decl_tree)
{
  ldv_i_type_ptr type = NULL;
  enum tree_code code;
  const char *type_name = NULL;
  tree func_arg_tree = NULL_TREE, func_param_tree = NULL_TREE;
  ldv_i_type_ptr arg_type_new = NULL;
  ldv_i_param_ptr param_new = NULL;
  bool isvoid_arg_type_new = false;
  ldv_pps_declspecs_ptr declspecs_aux = NULL;

  /* Do nothing if there is no tree node implementing type. */
  if (!type_tree)
    return NULL;

  type = ldv_create_info_type ();

  code = TREE_CODE (type_tree);

  switch (code)
    {
    case ARRAY_TYPE:
      type->it_kind = LDV_IT_ARRAY;
      type->array_type = ldv_convert_type_tree_to_internal (TREE_TYPE (type_tree), NULL);

      if (TYPE_DOMAIN (type_tree) && TYPE_MAX_VALUE (TYPE_DOMAIN (type_tree)))
        {
          type->issize_specified = true;
          type->array_size = TREE_INT_CST_LOW (TYPE_MAX_VALUE (TYPE_DOMAIN (type_tree))) + 1;
        }
      else
        type->issize_specified = false;

      break;

    case FUNCTION_TYPE:
      type->it_kind = LDV_IT_FUNC;

      type->ret_type = ldv_convert_type_tree_to_internal (TREE_TYPE (type_tree), NULL);

      for (func_arg_tree = TYPE_ARG_TYPES (type_tree), func_param_tree = decl_tree ? DECL_ARGUMENTS (decl_tree) : NULL
        ; func_arg_tree
        ; func_arg_tree = TREE_CHAIN (func_arg_tree), func_param_tree = func_param_tree ? TREE_CHAIN (func_param_tree) : NULL)
        {
          arg_type_new = ldv_convert_type_tree_to_internal (TREE_VALUE (func_arg_tree), NULL);
          isvoid_arg_type_new = ldv_isvoid (arg_type_new);

          /* Finish the cycle when a gcc artificial void type from the end of a
             function parameters list is found. Note that it's needed in case
             when it isn't the only i.e. if a function has other parameters. */
          if (type->param && isvoid_arg_type_new)
            {
              ldv_free_info_type (arg_type_new);
              break;
            }

          param_new = ldv_create_info_param ();

          param_new->type = arg_type_new;

          if (func_param_tree && DECL_NAME (func_param_tree))
            {
              param_new->name = ldv_create_id ();
              ldv_puts_id (IDENTIFIER_POINTER (DECL_NAME (func_param_tree)), param_new->name);
            }

          ldv_list_push_back (&type->param, param_new);
        }

      /* Check whether function arguments have a variable length. In that case
         gcc doesn't put a void type at the end of a  list of argument types. */
      if (arg_type_new && !isvoid_arg_type_new)
        arg_type_new->isva = true;

      break;

    case BOOLEAN_TYPE:
    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case UNION_TYPE:
    case REAL_TYPE:
    case RECORD_TYPE:
    case VOID_TYPE:
      type->it_kind = LDV_IT_PRIMITIVE;

      type->primitive_type = ldv_create_declspecs ();

      /* Add additional declaration specifiers like inline and static that were
         obtained earlier. */
      if (ldv_entity_declspecs)
        {
          declspecs_aux = ldv_merge_declspecs (type->primitive_type, ldv_entity_declspecs, false);

          /* In merging declaration specifiers new declaration
             specifiers are created. So old ones should be freed. */
          ldv_free_declspecs (type->primitive_type);
          type->primitive_type = declspecs_aux;

          /* Additional declaration specifiers may be applied just one time for
             some entity return type. */
          ldv_free_declspecs (ldv_entity_declspecs);
          ldv_entity_declspecs = NULL;
        }

      if (code == ENUMERAL_TYPE || code == UNION_TYPE || code == RECORD_TYPE)
        {
          /* Check wheter a type is anonymous. */
          if (TYPE_NAME (type_tree))
            {
              /* In this case we deal with a real structure (union, enum) that
                 has a name. */
              if (TREE_CODE (TYPE_NAME (type_tree)) == IDENTIFIER_NODE)
                type_name = IDENTIFIER_POINTER (TYPE_NAME (type_tree));
              /* In this case we deal with a typedef on some type. */
              else if (TREE_CODE (TYPE_NAME (type_tree)) == TYPE_DECL
                && DECL_NAME (TYPE_NAME (type_tree)))
                {
                  /* To specify that a typedef name is used. */
                  code = ERROR_MARK;

                  /* Get a typedef name. */
                  type_name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type_tree)));
                }
              /* In this case a type has not a name. */
              else
                type_name = "";
            }
          else
            type_name = "";
        }
      else
        {
          /* Try to obtain a type name from a related declaration if so. Note,
             that a void type hasn't such a name, so it will be parsed specially
             below. */
          if (TYPE_NAME (type_tree) && TREE_CODE (DECL_NAME (TYPE_NAME (type_tree))) == IDENTIFIER_NODE)
            type_name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type_tree)));
          else
            type_name = "";
        }

      if (code == ENUMERAL_TYPE)
        type->primitive_type->isenum = true;
      else if (code == UNION_TYPE)
        type->primitive_type->isunion = true;
      else if (code == RECORD_TYPE)
        type->primitive_type->isstruct = true;
      /* On the basis of gcc/c-common.c. */
      else if (!strcmp (type_name, "int"))
        type->primitive_type->isint = true;
      else if (!strcmp (type_name, "char"))
        type->primitive_type->ischar = true;
      else if (!strcmp (type_name, "long int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->islong = true;
        }
      else if (!strcmp (type_name, "unsigned int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->isunsigned = true;
        }
      else if (!strcmp (type_name, "long unsigned int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->islong = true;
          type->primitive_type->isunsigned = true;
        }
      else if (!strcmp (type_name, "long long int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->islonglong = true;
        }
      else if (!strcmp (type_name, "long long unsigned int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->islonglong = true;
          type->primitive_type->isunsigned = true;
        }
      else if (!strcmp (type_name, "short int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->isshort = true;
        }
      else if (!strcmp (type_name, "short unsigned int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->isshort = true;
          type->primitive_type->isunsigned = true;
        }
      else if (!strcmp (type_name, "signed char"))
        {
          type->primitive_type->ischar = true;
          type->primitive_type->issigned = true;
        }
      else if (!strcmp (type_name, "unsigned char"))
        {
          type->primitive_type->ischar = true;
          type->primitive_type->isunsigned = true;
        }
      else if (!strcmp (type_name, "float"))
        type->primitive_type->isfloat = true;
      else if (!strcmp (type_name, "double"))
        type->primitive_type->isdouble = true;
      else if (!strcmp (type_name, "long double"))
        {
          type->primitive_type->isdouble = true;
          type->primitive_type->islong = true;
        }
      else if (!strcmp (type_name, "complex int"))
        {
          type->primitive_type->isint = true;
          type->primitive_type->iscomplex = true;
        }
      else if (!strcmp (type_name, "complex float"))
        {
          type->primitive_type->isfloat = true;
          type->primitive_type->iscomplex = true;
        }
      else if (!strcmp (type_name, "complex double"))
        {
          type->primitive_type->isdouble = true;
          type->primitive_type->iscomplex = true;
        }
      else if (!strcmp (type_name, "complex long double"))
        {
          type->primitive_type->isdouble = true;
          type->primitive_type->islong = true;
          type->primitive_type->iscomplex = true;
        }
      else if (code == VOID_TYPE || !strcmp (type_name, "void"))
        type->primitive_type->isvoid = true;
      else if (!strcmp (type_name, "_Bool"))
        type->primitive_type->isbool = true;
      /* In this case a typedef name is processed. */
      else
        type->primitive_type->istypedef_name = true;

      /* Remember corresponddint type name for enums, unions, structs and typedefs. */
      if (code == ENUMERAL_TYPE || code == UNION_TYPE || code == RECORD_TYPE || type->primitive_type->istypedef_name)
        {
          type->primitive_type->type_name = ldv_create_id ();
          ldv_puts_id (type_name, type->primitive_type->type_name);
        }

      type->primitive_type->isconst = TYPE_READONLY (type_tree) ? true : false;
      type->primitive_type->isvolatile = TYPE_VOLATILE (type_tree) ? true : false;
      type->primitive_type->isrestrict = TYPE_RESTRICT (type_tree) ? true : false;

      break;

    case POINTER_TYPE:
      type->it_kind = LDV_IT_PTR;
      type->ptr_type = ldv_convert_type_tree_to_internal (TREE_TYPE (type_tree), NULL);

      type->isconst = TYPE_READONLY (type_tree) ? true : false;
      type->isvolatile = TYPE_VOLATILE (type_tree) ? true : false;
      type->isrestrict = TYPE_RESTRICT (type_tree) ? true : false;

      break;

    default:
      LDV_FATAL_ERROR ("tree node type '%s' isn't supported", tree_code_name[(int) TREE_CODE (type_tree)]);
      break;
    }

    return type;
}
