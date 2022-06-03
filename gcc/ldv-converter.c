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
#include "c/c-tree.h"
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
#include "c/c-lang.h"
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
      ldv_free_declarator (declarator);
      ldv_list_delete_all (declarator_list);
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
      internal_error ("incorrect type information kind \"%d\" is used", type->it_kind);
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
      internal_error ("incorrect type information kind \"%d\" is used", type->it_kind);
    }
}

ldv_i_initializer_ptr
ldv_convert_initializer_to_internal (tree initializer_tree)
{
  ldv_i_initializer_ptr initializer = NULL;
  unsigned HOST_WIDE_INT ix;
  tree index, value;
  ldv_i_struct_field_initializer_ptr struct_field_initializer = NULL;
  ldv_i_array_elem_initializer_ptr array_elem_initializer = NULL;
  char *c;

  /* Do nothing if there is no tree node implementing initializer. */
  if (!initializer_tree)
    return NULL;

  initializer = ldv_create_info_initializer ();

  if (TREE_CODE (initializer_tree) == CONSTRUCTOR)
    {
      /* Do not deal with very large initializers like:
       * https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/lib/stackdepot.c?id=cd11016e5f5212c13c0cec7384a525edc93b4921#n150
       * Such the initializers most likely do not hold anything useful for
       * users. For instance, in mentioned case all initializer values are
       * just NULL filled by specific GCC construction "...". We likely can
       * support large initializers as well but some optimizations are
       * necessary since otherwise it can take too much time uselessly.
       */
      if (CONSTRUCTOR_NELTS (initializer_tree) > (2 << 10))
          return NULL;

      FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (initializer_tree), ix, index, value)
        {
          if (TREE_CODE (index) == FIELD_DECL)
            {
              struct_field_initializer = XCNEW (ldv_info_struct_field_initializer);
              struct_field_initializer->decl = ldv_convert_and_print_struct_decl (index);
              /* Parsers dislike identation for declarations since it intermixes with identation for initializer. */
              for (c = struct_field_initializer->decl; *c; c++)
                if (*c == '\n')
                  *c = ' ';
              struct_field_initializer->initializer = ldv_convert_initializer_to_internal (value);
              ldv_list_push_back (&initializer->struct_initializer, struct_field_initializer);
            }
          else if (TREE_CODE (index) == INTEGER_CST)
            {
              array_elem_initializer = XCNEW (ldv_info_array_elem_initializer);
              array_elem_initializer->index = TREE_INT_CST_LOW (index);
              array_elem_initializer->initializer = ldv_convert_initializer_to_internal (value);
              ldv_list_push_back (&initializer->array_initializer, array_elem_initializer);
            }
          else
            internal_error ("can't find neither initialized structure field nor array element");
        }
    }
  else
    {
      initializer->non_struct_or_array_initializer = ldv_convert_and_print_assignment_expr (initializer_tree);

      if (TREE_CODE (initializer_tree) == ADDR_EXPR && TREE_CODE (TREE_OPERAND (initializer_tree, 0)) == FUNCTION_DECL)
        initializer->is_func_ptr = 1;
      else
        initializer->is_func_ptr = 0;
    }

  return initializer;
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
    case COMPLEX_TYPE:
      type->it_kind = LDV_IT_PRIMITIVE;

      type->primitive_type = ldv_create_declspecs ();

      /* Add additional declaration specifiers like inline and static that were
         obtained earlier. */
      if (ldv_entity_declspecs)
        {
          declspecs_aux = ldv_merge_declspecs (type->primitive_type, ldv_entity_declspecs);

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
    case REFERENCE_TYPE:
      type->it_kind = LDV_IT_PTR;
      type->ptr_type = ldv_convert_type_tree_to_internal (TREE_TYPE (type_tree), NULL);

      type->isconst = TYPE_READONLY (type_tree) ? true : false;
      type->isvolatile = TYPE_VOLATILE (type_tree) ? true : false;
      type->isrestrict = TYPE_RESTRICT (type_tree) ? true : false;

      break;

    default:
      internal_error ("tree node type '%s' isn't supported", get_tree_code_name(TREE_CODE (type_tree)));
      break;
    }

    return type;
}
