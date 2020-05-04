/* Copyright (C) 2010-2012
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "c-common.h"
#include "diagnostic-core.h"
#include "tree-iterator.h"
#include "tree.h"
#include "c/c-tree.h"

#include "ldv-convert.h"
#include "ldv-core.h"
#include "ldv-cbe-core.h"
#include "ldv-cpp-core.h"
#include "ldv-grammar.h"


#define LDV_TREE_NODE_NAME(t) (get_tree_code_name(TREE_CODE (t)))
#define LDV_CONVERT_ERROR(t) error ("LDV: %s: %d: tree node '%s' isn't supported", __FILE__, __LINE__, LDV_TREE_NODE_NAME (t))
#define LDV_OP_FIRST(t) TREE_OPERAND (t, 0)
#define LDV_OP_SECOND(t) TREE_OPERAND (t, 1)
#define LDV_OP_THIRD(t) TREE_OPERAND (t, 2)


bool ldv_disable_anon_enum_spec;


static ldv_constant_ptr ldv_constant_current;
static bool ldv_is_convert_enum_const_const_expr;


static bool ldv_check_declarator_xor_direct_declarator (ldv_declarator_ptr, ldv_direct_declarator_ptr);
static void ldv_convert_abstract_declarator (tree, ldv_abstract_declarator_ptr *, ldv_direct_abstract_declarator_ptr, ldv_abstract_declarator_ptr);
static ldv_abstract_declarator_ptr ldv_convert_abstract_declarator_internal (tree);
static ldv_additive_expr_ptr ldv_convert_additive_expr (tree, unsigned int);
static ldv_and_expr_ptr ldv_convert_and_expr (tree, unsigned int);
static ldv_arg_expr_list_ptr ldv_convert_arg_expr_list (tree, unsigned int);
static ldv_asm_arg_ptr ldv_convert_asm_arg (tree);
static ldv_asm_clobbers_ptr ldv_convert_asm_clobbers (tree);
static ldv_asm_goto_arg_ptr ldv_convert_asm_goto_arg (tree);
static ldv_asm_goto_operands_ptr ldv_convert_asm_goto_operands (tree);
static ldv_asm_operand_ptr ldv_convert_asm_operand (tree);
static ldv_asm_operands_ptr ldv_convert_asm_operands (tree);
static ldv_asm_statement_ptr ldv_convert_asm_statement (tree);
static ldv_asm_str_literal_ptr ldv_convert_asm_str_literal (tree);
static ldv_assignment_operator_ptr ldv_convert_assignment_operator (tree);
static ldv_block_item_ptr ldv_convert_block_item (tree);
static ldv_block_item_list_ptr ldv_convert_block_item_list (tree);
static ldv_cast_expr_ptr ldv_convert_cast_expr (tree, unsigned int);
static ldv_cond_expr_ptr ldv_convert_cond_expr (tree, unsigned int);
static ldv_const_expr_ptr ldv_convert_const_expr (tree);
static ldv_constant_ptr ldv_convert_constant (tree);
static ldv_compound_statement_ptr ldv_convert_compound_statement (tree);
static ldv_decl_spec_ptr ldv_convert_decl_spec (tree, bool);
static void ldv_convert_declarator (tree, bool, ldv_declarator_ptr *, ldv_direct_declarator_ptr, ldv_declarator_ptr);
static ldv_declarator_ptr ldv_convert_declarator_internal (tree, bool);
static ldv_assignment_expr_ptr ldv_convert_array_size (tree);
static void ldv_convert_direct_abstract_declarator (tree, ldv_abstract_declarator_ptr *, ldv_direct_abstract_declarator_ptr, ldv_abstract_declarator_ptr);
static void ldv_convert_direct_declarator (tree, bool, tree, ldv_declarator_ptr *, ldv_direct_declarator_ptr, ldv_declarator_ptr);
static ldv_designation_ptr ldv_convert_designation (tree);
static ldv_designator_ptr ldv_convert_designator (tree);
static ldv_designator_list_ptr ldv_convert_designator_list (tree);
static ldv_enum_ptr ldv_convert_enum (tree);
static ldv_enum_const_ptr ldv_convert_enum_const (tree);
static ldv_enum_list_ptr ldv_convert_enum_list (tree);
static ldv_decl_spec_ptr ldv_convert_enum_spec (tree, bool);
static ldv_enumeration_constant_ptr ldv_convert_enumeration_constant (tree);
static ldv_equality_expr_ptr ldv_convert_equality_expr (tree, unsigned int);
static ldv_exclusive_or_expr_ptr ldv_convert_exclusive_or_expr (tree, unsigned int);
static ldv_expr_ptr ldv_convert_expr (tree, unsigned int);
static ldv_expr_statement_ptr ldv_convert_expr_statement (tree);
static ldv_floating_constant_ptr ldv_convert_floating_constant (tree);
static ldv_func_def_ptr ldv_convert_func_def (tree);
static ldv_decl_spec_ptr ldv_convert_func_spec (tree);
static ldv_inclusive_or_expr_ptr ldv_convert_inclusive_or_expr (tree, unsigned int);
static ldv_init_declarator_ptr ldv_convert_init_declarator (tree);
static ldv_init_declarator_list_ptr ldv_convert_init_declarator_list (tree);
static ldv_initializer_ptr ldv_convert_initializer (tree);
static ldv_initializer_list_ptr ldv_convert_initializer_list (tree);
static ldv_integer_constant_ptr ldv_convert_integer_constant (tree);
static ldv_integer_suffix_ptr ldv_convert_integer_suffix (tree);
static ldv_jump_statement_ptr ldv_convert_jump_statement (tree);
static ldv_label_decl_ptr ldv_convert_label_decl (tree);
static ldv_labeled_statement_ptr ldv_convert_labeled_statement (tree);
static ldv_location_ptr ldv_convert_location (tree);
static ldv_logical_and_expr_ptr ldv_convert_logical_and_expr (tree, unsigned int);
static ldv_logical_or_expr_ptr ldv_convert_logical_or_expr (tree, unsigned int);
static ldv_long_long_suffix_ptr ldv_convert_long_long_suffix (tree);
static ldv_long_suffix_ptr ldv_convert_long_suffix (tree);
static ldv_multiplicative_expr_ptr ldv_convert_multiplicative_expr (tree, unsigned int);
static ldv_nested_decl_ptr ldv_convert_nested_decl (tree);
static ldv_nested_func_def_ptr ldv_convert_nested_func_def (tree);
static ldv_param_decl_ptr ldv_convert_param_decl (tree);
static ldv_param_list_ptr ldv_convert_param_list (tree);
static ldv_param_type_list_ptr ldv_convert_param_type_list (tree);
static ldv_pointer_ptr ldv_convert_pointer (tree, tree *);
static ldv_postfix_expr_ptr ldv_convert_postfix_expr (tree, unsigned int);
static ldv_primary_expr_ptr ldv_convert_primary_expr (tree, unsigned int);
static ldv_relational_expr_ptr ldv_convert_relational_expr (tree, unsigned int);
static ldv_selection_statement_ptr ldv_convert_selection_statement (tree);
static ldv_shift_expr_ptr ldv_convert_shift_expr (tree, unsigned int);
static ldv_spec_qual_list_ptr ldv_convert_spec_qual_list (tree);
static ldv_statement_ptr ldv_convert_statement (tree);
static ldv_decl_spec_ptr ldv_convert_storage_class_spec (tree);
static ldv_str_literal_ptr ldv_convert_str_literal (tree);
static ldv_struct_decl_list_ptr ldv_convert_struct_decl_list (tree);
static ldv_struct_declarator_ptr ldv_convert_struct_declarator (tree);
static ldv_struct_declarator_list_ptr ldv_convert_struct_declarator_list (tree);
static ldv_struct_or_union_ptr ldv_convert_struct_or_union (tree);
static ldv_decl_spec_ptr ldv_convert_struct_or_union_spec (tree, bool);
static ldv_type_name_ptr ldv_convert_type_name (tree);
static ldv_decl_spec_ptr ldv_convert_type_qual (tree);
static ldv_decl_spec_ptr ldv_convert_type_qual_internal (tree);
static ldv_type_qual_list_ptr ldv_convert_type_qual_list (tree);
static ldv_decl_spec_ptr ldv_convert_type_spec (tree, bool);
static ldv_decl_spec_ptr ldv_convert_type_spec_internal (tree);
static ldv_decl_spec_ptr ldv_convert_typedef_name (tree);
static ldv_unary_expr_ptr ldv_convert_unary_expr (tree, unsigned int);
static ldv_unary_operator_ptr ldv_convert_unary_operator (tree);
static ldv_unsigned_suffix_ptr ldv_convert_unsigned_suffix (tree);
static ldv_expr_statement_ptr ldv_create_null_expr_statement (void);
static ldv_statement_ptr ldv_create_null_statement (void);
static ldv_param_decl_ptr ldv_create_void_param_decl (void);
static ldv_decl_spec_ptr ldv_create_void_decl_spec (void);
static ldv_type_spec_ptr ldv_create_void_type_spec (void);
static void ldv_dump_expr_ops (tree, unsigned int, unsigned int);
static bool ldv_is_not_empty_statement_list (tree);
static const char *ldv_label_decl_name (tree);
static void ldv_new_decl_spec (bool *, ldv_decl_spec_ptr *,
                                ldv_decl_spec_ptr, ldv_decl_spec_ptr,
                                ldv_decl_spec_ptr, ldv_decl_spec_ptr);
static void ldv_new_type_qual (bool *, ldv_decl_spec_ptr *,
                                enum ldv_type_qual_kind);
static void ldv_new_type_spec (bool *, ldv_decl_spec_ptr *,
                                enum ldv_type_spec_kind);


static bool
ldv_check_declarator_xor_direct_declarator (ldv_declarator_ptr declarator, ldv_direct_declarator_ptr direct_declarator)
{
  if (!direct_declarator && !declarator)
    {
      LDV_ERROR ("neither previous direct declarator nor declarator is specified");
      return false;
    }

  if (direct_declarator && declarator)
    {
      LDV_ERROR ("both previous direct declarator and declarator is specified");
      return false;
    }

  return true;
}

/*
abstract-declarator:
    pointer
    pointeropt direct-abstract-declarator
*/
static void
ldv_convert_abstract_declarator (tree t, ldv_abstract_declarator_ptr *abstract_declarator_first_ptr, ldv_direct_abstract_declarator_ptr direct_abstract_declarator_prev, ldv_abstract_declarator_ptr abstract_declarator_prev)
{
  ldv_abstract_declarator_ptr abstract_declarator;
  ldv_pointer_ptr pointer;
  tree first_non_pointer;
  tree type_name;

  first_non_pointer = NULL;

  switch (TREE_CODE (t))
    {
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case BOOLEAN_TYPE:
    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
      /* Create required declarator consisting just of direct declarator. */
      if (direct_abstract_declarator_prev)
        {
          abstract_declarator = XCNEW (struct ldv_abstract_declarator);
          LDV_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (abstract_declarator) = direct_abstract_declarator_prev;
          *abstract_declarator_first_ptr = abstract_declarator;
        }
      else if (abstract_declarator_prev)
        *abstract_declarator_first_ptr = abstract_declarator_prev;

      break;

    case POINTER_TYPE:
      if ((type_name = TYPE_NAME (t)) && TREE_CODE(type_name) == TYPE_DECL)
        {
          if (direct_abstract_declarator_prev)
            {
              abstract_declarator = XCNEW (struct ldv_abstract_declarator);
              LDV_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (abstract_declarator) = direct_abstract_declarator_prev;
              *abstract_declarator_first_ptr = abstract_declarator;
            }
          else if (abstract_declarator_prev)
            *abstract_declarator_first_ptr = abstract_declarator_prev;
        }
      else if ((pointer = ldv_convert_pointer (t, &first_non_pointer)))
        {
          abstract_declarator = XCNEW (struct ldv_abstract_declarator);
          LDV_ABSTRACT_DECLARATOR_POINTER (abstract_declarator) = pointer;

          if (direct_abstract_declarator_prev)
            LDV_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (abstract_declarator) = direct_abstract_declarator_prev;

          if (first_non_pointer)
            switch (TREE_CODE (first_non_pointer))
              {
              case VOID_TYPE:
              case INTEGER_TYPE:
              case REAL_TYPE:
              case BOOLEAN_TYPE:
              case ENUMERAL_TYPE:
              case RECORD_TYPE:
              case UNION_TYPE:
                ldv_convert_abstract_declarator (first_non_pointer, abstract_declarator_first_ptr, NULL, abstract_declarator);
                break;

              case ARRAY_TYPE:
              case FUNCTION_TYPE:
              case POINTER_TYPE:
                if ((type_name = TYPE_NAME (first_non_pointer)) && TREE_CODE(type_name) == TYPE_DECL)
                  *abstract_declarator_first_ptr = abstract_declarator;
                else
                  ldv_convert_direct_abstract_declarator (first_non_pointer, abstract_declarator_first_ptr, NULL, abstract_declarator);

                break;

              default:
                LDV_CONVERT_ERROR (first_non_pointer);
              }
          else
            LDV_ERROR ("first non pointer type isn't specified");
        }

      break;

    case ARRAY_TYPE:
      ldv_convert_direct_abstract_declarator (t, abstract_declarator_first_ptr, NULL, *abstract_declarator_first_ptr);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }
}

static ldv_abstract_declarator_ptr
ldv_convert_abstract_declarator_internal (tree t)
{
  ldv_abstract_declarator_ptr abstract_declarator;

  abstract_declarator = NULL;

  /* This function is used since internal gcc representation has reverse order
     of abstract declarator and direct abstract declarator sequences. */
  ldv_convert_abstract_declarator (t, &abstract_declarator, NULL, NULL);

  return abstract_declarator;
}

/*
additive-expression:
    multiplicative-expression
    additive-expression + multiplicative-expression
    additive-expression - multiplicative-expression
*/
static ldv_additive_expr_ptr
ldv_convert_additive_expr (tree t, unsigned int recursion_limit)
{
  ldv_additive_expr_ptr additive_expr;
  tree op1, op2;

  additive_expr = XCNEW (struct ldv_additive_expr);

  switch (TREE_CODE (t))
    {
    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
    case MINUS_EXPR:
      if (TREE_CODE (t) == PLUS_EXPR || TREE_CODE (t) == POINTER_PLUS_EXPR)
        LDV_ADDITIVE_EXPR_KIND (additive_expr) = LDV_ADDITIVE_EXPR_SECOND;
      else
        LDV_ADDITIVE_EXPR_KIND (additive_expr) = LDV_ADDITIVE_EXPR_THIRD;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_ADDITIVE_EXPR_ADDITIVE_EXPR (additive_expr) = ldv_convert_additive_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of additive expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_ADDITIVE_EXPR_MULTIPLICATIVE_EXPR (additive_expr) = ldv_convert_multiplicative_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of additive expression");

      LDV_ADDITIVE_EXPR_LOCATION (additive_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_ADDITIVE_EXPR_KIND (additive_expr) = LDV_ADDITIVE_EXPR_FIRST;
      LDV_ADDITIVE_EXPR_MULTIPLICATIVE_EXPR (additive_expr) = ldv_convert_multiplicative_expr (t, recursion_limit);
    }

  if (LDV_ADDITIVE_EXPR_KIND (additive_expr))
    return additive_expr;

  LDV_ERROR ("additive expression wasn't converted");

  XDELETE (additive_expr);

  return NULL;
}

/*
AND-expression:
    equality-expression
    AND-expression & equality-expression
*/
static ldv_and_expr_ptr
ldv_convert_and_expr (tree t, unsigned int recursion_limit)
{
  ldv_and_expr_ptr and_expr;
  tree op1, op2;

  and_expr = XCNEW (struct ldv_and_expr);

  switch (TREE_CODE (t))
    {
    case BIT_AND_EXPR:
      if ((op1 = LDV_OP_FIRST (t)))
        {
          /* Skip artificial and expression added by gcc for bit field references. */
          if (TREE_CODE (op1) == BIT_FIELD_REF)
            {
              LDV_AND_EXPR_KIND (and_expr) = LDV_AND_EXPR_FIRST;
              LDV_AND_EXPR_EQUALITY_EXPR (and_expr) = ldv_convert_equality_expr (op1, recursion_limit);
              break;
            }
          else
            LDV_AND_EXPR_AND_EXPR (and_expr) = ldv_convert_and_expr (op1, recursion_limit);
        }
      else
        LDV_ERROR ("can't find the first operand of and expression");

      LDV_AND_EXPR_KIND (and_expr) = LDV_AND_EXPR_SECOND;

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_AND_EXPR_EQUALITY_EXPR (and_expr) = ldv_convert_equality_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of and expression");

      LDV_AND_EXPR_LOCATION (and_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_AND_EXPR_KIND (and_expr) = LDV_AND_EXPR_FIRST;
      LDV_AND_EXPR_EQUALITY_EXPR (and_expr) = ldv_convert_equality_expr (t, recursion_limit);
    }

  if (LDV_AND_EXPR_KIND (and_expr))
    return and_expr;

  LDV_ERROR ("and expression wasn't converted");

  XDELETE (and_expr);

  return NULL;
}

/*
argument-expression-list:
    assignment-expression
    argument-expression-list , assignment-expression
*/
static ldv_arg_expr_list_ptr
ldv_convert_arg_expr_list (tree t, unsigned int recursion_limit)
{
  ldv_arg_expr_list_ptr arg_expr_list, arg_expr_list_next, arg_expr_list_next_next;
  call_expr_arg_iterator func_arg_iterator;
  tree func_arg;

  arg_expr_list = XCNEW (struct ldv_arg_expr_list);
  arg_expr_list_next = NULL;

  FOR_EACH_CALL_EXPR_ARG (func_arg, func_arg_iterator, t)
    {
      if (!arg_expr_list_next)
        {
          arg_expr_list_next = arg_expr_list;
          LDV_ARG_EXPR_LIST_KIND (arg_expr_list_next) = LDV_ARG_EXPR_LIST_FIRST;
        }
      else
        {
          arg_expr_list_next_next = XCNEW (struct ldv_arg_expr_list);
          LDV_ARG_EXPR_LIST_ARG_EXPR_LIST (arg_expr_list_next_next) = arg_expr_list_next;
          arg_expr_list_next = arg_expr_list_next_next;
          LDV_ARG_EXPR_LIST_KIND (arg_expr_list_next) = LDV_ARG_EXPR_LIST_SECOND;
        }

      LDV_ARG_EXPR_LIST_ASSIGNEMT_EXPR (arg_expr_list_next) = ldv_convert_assignment_expr (func_arg, recursion_limit);
    }

  if (arg_expr_list_next && LDV_ARG_EXPR_LIST_KIND (arg_expr_list_next))
    return arg_expr_list_next;

  LDV_ERROR ("argument expression list wasn't converted");

  XDELETE (arg_expr_list_next);

  return NULL;
}

/*
GNU extensions:

asm-argument:
    asm-string-literal
    asm-string-literal : asm-operandsopt
    asm-string-literal : asm-operandsopt : asm-operandsopt
    asm-string-literal : asm-operandsopt : asm-operandsopt : asm-clobbersopt
*/
static ldv_asm_arg_ptr
ldv_convert_asm_arg (tree t)
{
  ldv_asm_arg_ptr asm_arg;
  tree asm_str_literal, asm_operands1, asm_operands2, asm_clobbers;

  asm_arg = XCNEW (struct ldv_asm_arg);

  switch (TREE_CODE (t))
    {
    case ASM_EXPR:
      LDV_ASM_ARG_KIND (asm_arg) = LDV_ASM_ARG_FIRST;

      if ((asm_str_literal = ASM_STRING (t)))
        LDV_ASM_ARG_ASM_STR_LITERAL (asm_arg) = ldv_convert_asm_str_literal (asm_str_literal);
      else
        LDV_ERROR ("can't find asm string literal");

      if ((asm_operands1 = ASM_OUTPUTS (t)))
        {
          LDV_ASM_ARG_ASM_OPERANDS1 (asm_arg) = ldv_convert_asm_operands (asm_operands1);
          LDV_ASM_ARG_KIND (asm_arg) = LDV_ASM_ARG_SECOND;
        }

      if ((asm_operands2 = ASM_INPUTS (t)))
        {
          LDV_ASM_ARG_ASM_OPERANDS2 (asm_arg) = ldv_convert_asm_operands (asm_operands2);
          LDV_ASM_ARG_KIND (asm_arg) = LDV_ASM_ARG_THIRD;
        }

      if ((asm_clobbers = ASM_CLOBBERS (t)))
        {
          LDV_ASM_ARG_ASM_CLOBBERS (asm_arg) = ldv_convert_asm_clobbers (asm_clobbers);
          LDV_ASM_ARG_KIND (asm_arg) = LDV_ASM_ARG_FOURTH;
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  return asm_arg;
}

/*
GNU extensions:

asm-clobbers:
    asm-string-literal
    asm-clobbers , asm-string-literal
*/
static ldv_asm_clobbers_ptr
ldv_convert_asm_clobbers (tree t)
{
  ldv_asm_clobbers_ptr asm_clobbers, asm_clobbers_next;
  tree asm_clobbers_value_next, asm_str_literal;

  asm_clobbers = XCNEW (struct ldv_asm_clobbers);
  asm_clobbers_next = NULL;

  switch (TREE_CODE (t))
    {
    case TREE_LIST:
      LDV_ASM_CLOBBERS_KIND (asm_clobbers) = LDV_ASM_CLOBBERS_FIRST;

      if ((asm_clobbers_value_next = TREE_CHAIN (t)))
        {
          asm_clobbers_next = ldv_convert_asm_clobbers (asm_clobbers_value_next);
          LDV_ASM_CLOBBERS_KIND (asm_clobbers) = LDV_ASM_CLOBBERS_SECOND;
          LDV_ASM_CLOBBERS_ASM_CLOBBERS (asm_clobbers) = asm_clobbers_next;
        }

      if ((asm_str_literal = TREE_VALUE (t)))
        LDV_ASM_CLOBBERS_ASM_STR_LITERAL (asm_clobbers) = ldv_convert_asm_str_literal (asm_str_literal);
      else
        LDV_ERROR ("can't find asm string literal");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_ASM_CLOBBERS_KIND (asm_clobbers))
    return asm_clobbers;

  LDV_ERROR ("asm clobbers wasn't converted");

  XDELETE (asm_clobbers);

  return NULL;
}

/*
GNU extensions:

asm-goto-argument:
    asm-string-literal : : asm-operandsopt : asm-clobbersopt : asm-goto-operands
*/
static ldv_asm_goto_arg_ptr
ldv_convert_asm_goto_arg (tree t)
{
  ldv_asm_goto_arg_ptr asm_goto_arg;
  tree asm_str_literal, asm_operands, asm_clobbers, asm_goto_operands;

  asm_goto_arg = XCNEW (struct ldv_asm_goto_arg);

  switch (TREE_CODE (t))
    {
    case ASM_EXPR:
      if ((asm_str_literal = ASM_STRING (t)))
        LDV_ASM_GOTO_ARG_ASM_STR_LITERAL (asm_goto_arg) = ldv_convert_asm_str_literal (asm_str_literal);
      else
        LDV_ERROR ("can't find asm string literal");

      if ((asm_operands = ASM_INPUTS (t)))
        LDV_ASM_GOTO_ARG_ASM_OPERANDS (asm_goto_arg) = ldv_convert_asm_operands (asm_operands);

      if ((asm_clobbers = ASM_CLOBBERS (t)))
        LDV_ASM_GOTO_ARG_ASM_CLOBBERS (asm_goto_arg) = ldv_convert_asm_clobbers (asm_clobbers);

      if ((asm_goto_operands = ASM_LABELS (t)))
        LDV_ASM_GOTO_ARG_ASM_GOTO_OPERANDS (asm_goto_arg) = ldv_convert_asm_goto_operands (asm_goto_operands);
      else
        LDV_ERROR ("can't find asm string literal");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  return asm_goto_arg;
}

/*
GNU extensions:

asm-goto-operands:
    identifier
    asm-goto-operands, identifier
*/
static ldv_asm_goto_operands_ptr
ldv_convert_asm_goto_operands (tree t)
{
  ldv_asm_goto_operands_ptr asm_goto_operands, asm_goto_operands_next, asm_goto_operands_next_next;
  tree asm_goto_operands_value_next, label_decl;

  asm_goto_operands = XCNEW (struct ldv_asm_goto_operands);
  asm_goto_operands_next = NULL;

  switch (TREE_CODE (t))
    {
    case TREE_LIST:
      LDV_ASM_GOTO_OPERANDS_KIND (asm_goto_operands) = LDV_ASM_GOTO_OPERANDS_FIRST;

      if ((asm_goto_operands_value_next = TREE_CHAIN (t)))
        {
          asm_goto_operands_next = ldv_convert_asm_goto_operands (asm_goto_operands_value_next);
          for (asm_goto_operands_next_next = asm_goto_operands_next; LDV_ASM_GOTO_OPERANDS_ASM_GOTO_OPERANDS (asm_goto_operands_next_next); asm_goto_operands_next_next = LDV_ASM_GOTO_OPERANDS_ASM_GOTO_OPERANDS (asm_goto_operands_next_next)) ;
          LDV_ASM_GOTO_OPERANDS_KIND (asm_goto_operands_next_next) = LDV_ASM_GOTO_OPERANDS_SECOND;
          LDV_ASM_GOTO_OPERANDS_ASM_GOTO_OPERANDS (asm_goto_operands_next_next) = asm_goto_operands;
        }

      if ((label_decl = TREE_VALUE (t)))
        LDV_ASM_GOTO_OPERANDS_IDENTIFIER (asm_goto_operands) = ldv_convert_identifier (label_decl);
      else
        LDV_ERROR ("can't find label declaration");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (asm_goto_operands_next)
    return asm_goto_operands_next;

  if (LDV_ASM_GOTO_OPERANDS_KIND (asm_goto_operands))
    return asm_goto_operands;

  LDV_ERROR ("asm goto operands wasn't converted");

  XDELETE (asm_goto_operands);

  return NULL;
}

/*
GNU extensions:

asm-operand:
    asm-string-literal ( expression )
    [ identifier ] asm-string-literal ( expression )
*/
static ldv_asm_operand_ptr
ldv_convert_asm_operand (tree t)
{
  ldv_asm_operand_ptr asm_operand;
  tree tree_purpose, asm_str_literal, expr, identifier;

  asm_operand = XCNEW (struct ldv_asm_operand);

  switch (TREE_CODE (t))
    {
    case TREE_LIST:
      if ((tree_purpose = TREE_PURPOSE (t)))
        {
          if ((identifier = TREE_PURPOSE (tree_purpose)))
            {
              LDV_ASM_OPERAND_KIND (asm_operand) = LDV_ASM_OPERAND_SECOND;
              LDV_ASM_OPERAND_IDENTIFIER (asm_operand) = ldv_convert_identifier (identifier);
            }
          else
            LDV_ASM_OPERAND_KIND (asm_operand) = LDV_ASM_OPERAND_FIRST;

          if ((asm_str_literal = TREE_VALUE (tree_purpose)))
            LDV_ASM_OPERAND_ASM_STR_LITERAL (asm_operand) = ldv_convert_asm_str_literal (asm_str_literal);
          else
            LDV_ERROR ("can't find asm string literal");
        }
      else
        LDV_ERROR ("can't find asm string literal and identifier");

      if ((expr = TREE_VALUE (t)))
        LDV_ASM_OPERAND_EXPR (asm_operand) = ldv_convert_expr (expr, LDV_CONVERT_EXPR_RECURSION_LIMIT);
      else
        LDV_ERROR ("can't find expression");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_ASM_OPERAND_KIND (asm_operand))
    return asm_operand;

  LDV_ERROR ("asm operand wasn't converted");

  XDELETE (asm_operand);

  return NULL;
}

/*
GNU extensions:

asm-operands:
    asm-operand
    asm-operands , asm-operand
*/
static ldv_asm_operands_ptr
ldv_convert_asm_operands (tree t)
{
  ldv_asm_operands_ptr asm_operands, asm_operands_next, asm_operands_next_next;
  tree asm_operands_value_next;

  asm_operands = XCNEW (struct ldv_asm_operands);
  asm_operands_next = NULL;

  switch (TREE_CODE (t))
    {
    case TREE_LIST:
      LDV_ASM_OPERANDS_KIND (asm_operands) = LDV_ASM_OPERANDS_FIRST;

      if ((asm_operands_value_next = TREE_CHAIN (t)))
        {
          asm_operands_next = ldv_convert_asm_operands (asm_operands_value_next);
          for (asm_operands_next_next = asm_operands_next; LDV_ASM_OPERANDS_ASM_OPERANDS (asm_operands_next_next); asm_operands_next_next = LDV_ASM_OPERANDS_ASM_OPERANDS (asm_operands_next_next)) ;
          LDV_ASM_OPERANDS_KIND (asm_operands_next_next) = LDV_ASM_OPERANDS_SECOND;
          LDV_ASM_OPERANDS_ASM_OPERANDS (asm_operands_next_next) = asm_operands;
        }

      LDV_ASM_OPERANDS_ASM_OPERAND (asm_operands) = ldv_convert_asm_operand (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (asm_operands_next)
    return asm_operands_next;

  if (LDV_ASM_OPERANDS_KIND (asm_operands))
    return asm_operands;

  LDV_ERROR ("asm operands wasn't converted");

  XDELETE (asm_operands);

  return NULL;
}

/*
GNU extensions:

asm-statement:
    asm type-qualifieropt ( asm-argument ) ;
    asm type-qualifieropt goto ( asm-goto-argument ) ;
*/
static ldv_asm_statement_ptr
ldv_convert_asm_statement (tree t)
{
  ldv_asm_statement_ptr asm_statement;
  ldv_type_qual_ptr type_qual;

  asm_statement = XCNEW (struct ldv_asm_statement);

  /* Inline Assembler is not interpreted by source code analysis & verification
     tools. Moreover, it can confuse them, e.g. CPAchecker doesn't support
     asm goto. So, it is better to print some stub instead of inline
     Assembler. */
  if (ldv_inline_asm_stub)
    {
      LDV_ASM_STATEMENT_KIND (asm_statement) = LDV_ASM_STATEMENT_STUB;
      return asm_statement;
    }

  switch (TREE_CODE (t))
    {
    case ASM_EXPR:
      /* Gcc supports just volatile type qualifier for asm statement. Obtain it
         here to simplify processing. */
      if (ASM_VOLATILE_P (t))
        {
          type_qual = XCNEW (struct ldv_type_qual);
          LDV_TYPE_QUAL_KIND (type_qual) = LDV_TYPE_QUAL_VOLATILE;
          LDV_ASM_STATEMENT_TYPE_QUAL (asm_statement) = type_qual;
        }

      /* Gcc labels corresponds to asm-goto-operands. */
      if (ASM_LABELS (t))
        {
          LDV_ASM_STATEMENT_KIND (asm_statement) = LDV_ASM_STATEMENT_SECOND;
          LDV_ASM_STATEMENT_ASM_GOTO_ARG (asm_statement) = ldv_convert_asm_goto_arg (t);
        }
      else
        {
          LDV_ASM_STATEMENT_KIND (asm_statement) = LDV_ASM_STATEMENT_FIRST;
          LDV_ASM_STATEMENT_ASM_ARG (asm_statement) = ldv_convert_asm_arg (t);
        }

      LDV_ASM_STATEMENT_LOCATION (asm_statement) = ldv_convert_location (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  return asm_statement;
}

/*
GNU extensions:

asm-string-literal:
    string-literal
*/
static ldv_asm_str_literal_ptr
ldv_convert_asm_str_literal (tree t)
{
  ldv_asm_str_literal_ptr asm_str_literal;

  asm_str_literal = XCNEW (struct ldv_asm_str_literal);

  switch (TREE_CODE (t))
    {
    case STRING_CST:
      LDV_ASM_STR_LITERAL_STR_LITERAL (asm_str_literal) = ldv_convert_str_literal (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  return asm_str_literal;
}

/*
assignment-expression:
    conditional-expression
    unary-expression assignment-operator assignment-expression
*/
ldv_assignment_expr_ptr
ldv_convert_assignment_expr (tree t, unsigned int recursion_limit)
{
  ldv_assignment_expr_ptr assignment_expr;
  ldv_assignment_operator_ptr assignment_operator;
  tree op1, op2;

  /* Dump expression operands being almost on the top of expression stack. */
  ldv_dump_expr_ops(t, 0, 0);

  assignment_expr = XCNEW (struct ldv_assignment_expr);

  switch (TREE_CODE (t))
    {
    /* Simply process the first operand of NON_LVALUE_EXPR as well as for
       SAVE_EXPR in ldv_convert_expr. */
    case NON_LVALUE_EXPR:
      if ((op1 = LDV_OP_FIRST (t)))
        {
          LDV_EXPR_KIND (assignment_expr) = LDV_ASSIGNMENT_EXPR_FIRST;
          LDV_ASSIGNMENT_EXPR_COND_EXPR (assignment_expr) = ldv_convert_cond_expr (op1, recursion_limit);
        }
      else
        LDV_ERROR ("can't find the first operand of non lvalue expression");

      break;

    case MODIFY_EXPR:
      LDV_ASSIGNMENT_EXPR_KIND (assignment_expr) = LDV_ASSIGNMENT_EXPR_SECOND;

      if (!(op1 = LDV_OP_FIRST (t)))
        LDV_ERROR ("can't find the first operand of assignment expression");

      if (!(op2 = LDV_OP_SECOND (t)))
        LDV_ERROR ("can't find the second operand of assignment expression");

      /* This is the case for GNU extension of compound expression in primary
         expression, i.e. '({...})'. For it auxiliary artificial variable is
         generated where compound expression result is assigned. In fact it
         becomes a problem just when the given result has constant type. */
      if (TREE_CODE (op1) == VAR_DECL && DECL_ARTIFICIAL (op1))
        {
          LDV_ASSIGNMENT_EXPR_KIND (assignment_expr) = LDV_ASSIGNMENT_EXPR_FIRST;
          if (op2)
            LDV_ASSIGNMENT_EXPR_COND_EXPR (assignment_expr) = ldv_convert_cond_expr (op2, recursion_limit);
          break;
        }

      LDV_ASSIGNMENT_EXPR_UNARY_EXPR (assignment_expr) = ldv_convert_unary_expr (op1, recursion_limit);

      LDV_ASSIGNMENT_EXPR_ASSIGNMENT_OPERATOR (assignment_expr) = ldv_convert_assignment_operator (t);

      /* See on whether an assignment operator is a compound one (i.e. like,
         '+=', '*=' and so on. If so then get the second operand of the second
         operand of the given expression to make next assignment expression. */
      if ((assignment_operator = LDV_ASSIGNMENT_EXPR_ASSIGNMENT_OPERATOR (assignment_expr)))
        {
          if (LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) > 1)
            {
              if (!(op2 = LDV_OP_SECOND (op2)))
                LDV_ERROR ("can't find the second operand of compound assignment expression");
            }
        }
      else
        LDV_ERROR ("can't get assignment operator of assignment expression");

      if (op2)
        LDV_ASSIGNMENT_EXPR_ASSIGNMENT_EXPR (assignment_expr) = ldv_convert_assignment_expr (op2, recursion_limit);

      LDV_ASSIGNMENT_EXPR_LOCATION (assignment_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_ASSIGNMENT_EXPR_KIND (assignment_expr) = LDV_ASSIGNMENT_EXPR_FIRST;
      LDV_ASSIGNMENT_EXPR_COND_EXPR (assignment_expr) = ldv_convert_cond_expr (t, recursion_limit);
    }

  if (LDV_ASSIGNMENT_EXPR_KIND (assignment_expr))
    return assignment_expr;

  LDV_ERROR ("assignment expression wasn't converted");

  XDELETE (assignment_expr);

  return NULL;
}

/*
assignment-operator: one of
    = *= /= %= += -= <<= >>= &= ^= |=
*/
static ldv_assignment_operator_ptr
ldv_convert_assignment_operator (tree t)
{
  tree op1, op2, op21;
  ldv_assignment_operator_ptr assignment_operator;

  assignment_operator = XCNEW (struct ldv_assignment_operator);

  /* There aren't distinct tree nodes for different assignment operators. Insted
     they all are considered as MODIFY_EXPR. Nevertheless we can obtain compound
     assignment operator (that is something unlike '=') by investigating second
     operand kind. So if we have something like this:
       MODIFY_EXPR
         op1
         op2 (PLUS_EXPR)
           op1 (the same as above!)
           op3
     we can "build" compound assignment operator:
       op1 += op3
   */
  switch (TREE_CODE (t))
    {
    case MODIFY_EXPR:
      LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_FIRST;

      if (!(op1 = LDV_OP_FIRST (t)))
        {
          LDV_ERROR ("can't find the first operand of assignment expression");
          break;
        }

      if (!(op2 = LDV_OP_SECOND (t)))
        {
          LDV_ERROR ("can't find the second operand of assignment expression");
          break;
        }

      switch (TREE_CODE (op2))
        {
        case MULT_EXPR:
        case TRUNC_DIV_EXPR:
        case EXACT_DIV_EXPR:
        case TRUNC_MOD_EXPR:
        case PLUS_EXPR:
        case POINTER_PLUS_EXPR:
        case MINUS_EXPR:
        case LSHIFT_EXPR:
        case RSHIFT_EXPR:
        case BIT_AND_EXPR:
        case BIT_XOR_EXPR:
        case BIT_IOR_EXPR:
          if (!(op21 = LDV_OP_FIRST (op2)))
            {
              LDV_ERROR ("can't find the first operand of compound assignment expression");
              break;
            }

          if (op1 == op21)
            {
              switch (TREE_CODE (op2))
                {
                case MULT_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_SECOND;
                  break;

                case TRUNC_DIV_EXPR:
                case EXACT_DIV_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_THIRD;
                  break;

                case TRUNC_MOD_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_FOURTH;
                  break;

                case PLUS_EXPR:
                case POINTER_PLUS_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_FIVTH;
                  break;

                case MINUS_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_SIXTH;
                  break;

                case LSHIFT_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_SEVENTH;
                  break;

                case RSHIFT_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_EIGHTH;
                  break;

                case BIT_AND_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_NINTH;
                  break;

                case BIT_XOR_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_TENTH;
                  break;

                case BIT_IOR_EXPR:
                  LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator) = LDV_ASSIGNMENT_OPERATOR_ELEVEN;
                  break;

                default:
                  LDV_ERROR ("something strange");
                }
            }

          break;

        /* In this case we have simple assignment '='. */
        default: ;
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator))
    return assignment_operator;

  LDV_ERROR ("assignment operator wasn't converted");

  XDELETE (assignment_operator);

  return NULL;
}

/*
cast-expression:
    unary-expression
    ( type-name ) cast-expression
*/
static ldv_cast_expr_ptr
ldv_convert_cast_expr (tree t, unsigned int recursion_limit)
{
  ldv_cast_expr_ptr cast_expr;
  tree type, pointer_type, op1;

  cast_expr = XCNEW (struct ldv_cast_expr);

  switch (TREE_CODE (t))
    {
    case CONVERT_EXPR:
    case NOP_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
    case VIEW_CONVERT_EXPR:
      if ((type = TREE_TYPE (t)) &&
          /* Do not consider casts to anonymous structures. */
          ((!TYPE_NAME (type) && TREE_CODE (type) != RECORD_TYPE) ||
          /* Do not consider casts to integer types. */
          (TYPE_NAME (type) && (DECL_P (TYPE_NAME (type)) || TREE_CODE (type) != INTEGER_TYPE))))
        {
          LDV_CAST_EXPR_KIND (cast_expr) = LDV_CAST_EXPR_SECOND;

          /* It was noticed that on x86 (32 bit) architecture there is a "problem"
             with __builtin_va_start and similar functions because of their
             parameters have reference type. Thus in their calls casting to
             reference type is performed. But we wouldn't like to introduce
             references in C so just ignore this casting at all.
             In addition we wouldn't like to introduce casts for bitfields since
             artificial types that haven't names are used for them. */
          if (TREE_CODE (type) == REFERENCE_TYPE || (TREE_CODE (type) == INTEGER_TYPE && !(TYPE_NAME (type))))
            {
              LDV_CAST_EXPR_KIND (cast_expr) = LDV_CAST_EXPR_FIRST;

              if ((op1 = LDV_OP_FIRST (t)))
                LDV_CAST_EXPR_UNARY_EXPR (cast_expr) = ldv_convert_unary_expr (op1, recursion_limit);
              else
                LDV_ERROR ("can't find the first operand of cast expression");

              break;
            }
          else
            LDV_CAST_EXPR_TYPE_NAME (cast_expr) = ldv_convert_type_name (type);

          /* Avoid casts to internal GCC structure __va_list_tag. */
          if (TREE_CODE (type) == POINTER_TYPE)
            {
              pointer_type = TREE_TYPE (type);
              if (TREE_CODE (pointer_type) == RECORD_TYPE && TYPE_NAME (pointer_type) && DECL_P (TYPE_NAME (pointer_type)))
                {
                  if (!strcmp (LDV_IDENTIFIER_STR (ldv_convert_identifier (TYPE_NAME (pointer_type))), "__va_list_tag"))
                    {
                      LDV_CAST_EXPR_KIND (cast_expr) = LDV_CAST_EXPR_FIRST;
                      if ((op1 = LDV_OP_FIRST (t)))
                        {
                          if ((op1 = LDV_OP_FIRST (op1)))
                            LDV_CAST_EXPR_UNARY_EXPR (cast_expr) = ldv_convert_unary_expr (op1, recursion_limit);
                          else
                            LDV_ERROR ("can't find the first operand of expression");

                          break;
                        }
                      else
                        LDV_ERROR ("can't find the first operand of cast expression");
                    }
                }
            }

          if ((op1 = LDV_OP_FIRST (t)))
            LDV_CAST_EXPR_CAST_EXPR (cast_expr) = ldv_convert_cast_expr (op1, recursion_limit);
          else
            LDV_ERROR ("can't find the first operand of cast expression");

          LDV_CAST_EXPR_LOCATION (cast_expr) = ldv_convert_location (t);

          break;
        }

      LDV_CAST_EXPR_KIND (cast_expr) = LDV_CAST_EXPR_FIRST;
      t = LDV_OP_FIRST (t);
      LDV_CAST_EXPR_UNARY_EXPR (cast_expr) = ldv_convert_unary_expr (t, recursion_limit);

      break;

    default:
      LDV_CAST_EXPR_KIND (cast_expr) = LDV_CAST_EXPR_FIRST;
      LDV_CAST_EXPR_UNARY_EXPR (cast_expr) = ldv_convert_unary_expr (t, recursion_limit);
    }

  if (LDV_CAST_EXPR_KIND (cast_expr))
    return cast_expr;

  LDV_ERROR ("cast expression wasn't converted");

  XDELETE (cast_expr);

  return NULL;
}

/*
conditional-expression:
    logical-OR-expression
    logical-OR-expression ? expression : conditional-expression

GNU extensions:

conditional-expression:
    logical-OR-expression ? : conditional-expression

LDV extensions:

conditional-expression:
    LDV_MIN (expression, conditional-expression)
    LDV_MAX (expression, conditional-expression)
    LDV_ABS (expression)
*/
static ldv_cond_expr_ptr
ldv_convert_cond_expr (tree t, unsigned int recursion_limit)
{
  ldv_cond_expr_ptr cond_expr;
  tree op1, op2, op3;

  cond_expr = XCNEW (struct ldv_cond_expr);

  switch (TREE_CODE (t))
    {
    case COND_EXPR:
      LDV_COND_EXPR_KIND (cond_expr) = LDV_COND_EXPR_SECOND;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_COND_EXPR_LOGICAL_OR_EXPR (cond_expr) = ldv_convert_logical_or_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of conditional expression");

      if ((op2 = LDV_OP_SECOND (t)))
        {
          /* GCC allows empty second statement in ternary conditional
             expression (#3711). SAVE_EXPR means exactly it since it
             doesn't require evaluation. */
          if (TREE_CODE (op2) == SAVE_EXPR)
            LDV_COND_EXPR_KIND (cond_expr) = LDV_COND_EXPR_THIRD;
          else
            LDV_COND_EXPR_EXPR (cond_expr) = ldv_convert_expr (op2, LDV_CONVERT_EXPR_RECURSION_LIMIT);
        }
      else
        LDV_ERROR ("can't find the second operand of conditional expression");

      if ((op3 = LDV_OP_THIRD (t)))
        LDV_COND_EXPR_COND_EXPR (cond_expr) = ldv_convert_cond_expr (op3, recursion_limit);
      else
        LDV_ERROR ("can't find the third operand of conditional expression");

      LDV_COND_EXPR_LOCATION (cond_expr) = ldv_convert_location (t);

      break;

    case MIN_EXPR:
    case MAX_EXPR:
      if (TREE_CODE (t) == MIN_EXPR)
        LDV_COND_EXPR_KIND (cond_expr) = LDV_COND_EXPR_FOURTH;
      else
        LDV_COND_EXPR_KIND (cond_expr) = LDV_COND_EXPR_FIFTH;

      if (!(op1 = LDV_OP_FIRST (t)))
        LDV_ERROR ("can't find the first operand of conditional expression");

      if (!(op2 = LDV_OP_SECOND (t)))
        LDV_ERROR ("can't find the second operand of conditional expression");

      if (op1 && op2)
        {
          LDV_COND_EXPR_EXPR (cond_expr) = ldv_convert_expr (op2, recursion_limit);
          LDV_COND_EXPR_COND_EXPR (cond_expr) = ldv_convert_cond_expr (op1, recursion_limit);
        }

      LDV_COND_EXPR_LOCATION (cond_expr) = ldv_convert_location (t);

      break;

    case ABS_EXPR:
      LDV_COND_EXPR_KIND (cond_expr) = LDV_COND_EXPR_SIXTH;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_COND_EXPR_EXPR (cond_expr) = ldv_convert_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of conditional expression");

      LDV_COND_EXPR_LOCATION (cond_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_COND_EXPR_KIND (cond_expr) = LDV_COND_EXPR_FIRST;
      LDV_COND_EXPR_LOGICAL_OR_EXPR (cond_expr) = ldv_convert_logical_or_expr (t, recursion_limit);
    }

  if (LDV_COND_EXPR_KIND (cond_expr))
    return cond_expr;

  LDV_ERROR ("conditional expression wasn't converted");

  XDELETE (cond_expr);

  return NULL;
}

/*
block-item:
    declaration
    statement

GNU extensions:

block-item:
    declaration -> nested-declaration
*/
static ldv_block_item_ptr
ldv_convert_block_item (tree t)
{
  ldv_block_item_ptr block_item;

  block_item = XCNEW (struct ldv_block_item);

  switch (TREE_CODE (t))
    {
    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case TYPE_DECL:
    case VAR_DECL:
    case FUNCTION_DECL:
      LDV_BLOCK_ITEM_KIND (block_item) = LDV_BLOCK_ITEM_FIRST;
      LDV_BLOCK_ITEM_NESTED_DECL (block_item) = ldv_convert_nested_decl (t);

      break;

    default:
      LDV_BLOCK_ITEM_KIND (block_item) = LDV_BLOCK_ITEM_SECOND;
      LDV_BLOCK_ITEM_STATEMENT (block_item) = ldv_convert_statement (t);
    }

  if (LDV_BLOCK_ITEM_KIND (block_item))
    return block_item;

  LDV_ERROR ("block item wasn't converted");

  XDELETE (block_item);

  return NULL;
}

/*
block-item-list:
    block-item
    block-item-list block-item
*/
static ldv_block_item_list_ptr
ldv_convert_block_item_list (tree t)
{
  ldv_block_item_list_ptr block_item_list, block_item_list_next;
  ldv_block_item_ptr block_item;
  tree_stmt_iterator si;
  tree statement;

  block_item_list = XCNEW (struct ldv_block_item_list);
  block_item_list_next = NULL;

  switch (TREE_CODE (t))
    {
    case STATEMENT_LIST:
      /* Reverse the order of block items to correspond to the grammar. */
      for (si = tsi_last (t); !tsi_end_p (si); tsi_prev (&si))
        {
          if ((statement = tsi_stmt (si)))
            switch (TREE_CODE (statement))
              {
              case DECL_EXPR:
              case SAVE_EXPR:
              case PREDICT_EXPR:
                break;

              default:
                if ((block_item = ldv_convert_block_item (statement)))
                  {
                    if (!block_item_list_next)
                      block_item_list_next = block_item_list;
                    else
                      block_item_list_next = LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST (block_item_list_next) = XCNEW (struct ldv_block_item_list);

                    LDV_BLOCK_ITEM_LIST_BLOCK_ITEM (block_item_list_next) = block_item;
                  }
                else
                  LDV_ERROR ("block item wasn't converted");
              }
          else
            LDV_ERROR ("can't find statement");
        }
      break;

    default:
      if ((block_item = ldv_convert_block_item (t)))
        LDV_BLOCK_ITEM_LIST_BLOCK_ITEM (block_item_list) = block_item;
      else
        LDV_ERROR ("block item wasn't converted");
    }

  if (LDV_BLOCK_ITEM_LIST_BLOCK_ITEM (block_item_list) || LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST (block_item_list))
    return block_item_list;

  LDV_ERROR ("block item list wasn't converted");

  XDELETE (block_item_list);

  return NULL;
}

/*
constant-expression:
    conditional-expression
*/
static ldv_const_expr_ptr
ldv_convert_const_expr (tree t)
{
  ldv_const_expr_ptr const_expr;

  const_expr = XCNEW (struct ldv_const_expr);

  LDV_CONST_EXPR_COND_EXPR (const_expr) = ldv_convert_cond_expr (t, LDV_CONVERT_EXPR_RECURSION_LIMIT);

  if (LDV_CONST_EXPR_COND_EXPR (const_expr))
    return const_expr;

  LDV_ERROR ("constant expression wasn't converted");

  XDELETE (const_expr);

  return NULL;
}

/*
constant:
    integer-constant
    floating-constant
    enumeration-constant
    character-constant
*/
static ldv_constant_ptr
ldv_convert_constant (tree t)
{
  ldv_constant_ptr constant;
  tree type;

  constant = XCNEW (struct ldv_constant);

  switch (TREE_CODE (t))
    {
    case INTEGER_CST:
      if ((type = TREE_TYPE (t)))
        {
          if (TREE_CODE (type) == ENUMERAL_TYPE && !ldv_is_convert_enum_const_const_expr)
            LDV_CONSTANT_ENUMERATION_CONSTANT (constant) = ldv_convert_enumeration_constant (t);

          if (LDV_CONSTANT_ENUMERATION_CONSTANT (constant))
            LDV_CONSTANT_KIND (constant) = LDV_CONSTANT_THIRD;
          else /* TODO: erase comment!!!  if (TREE_CODE (type) == INTEGER_TYPE) */
            {
              LDV_CONSTANT_KIND (constant) = LDV_CONSTANT_FIRST;
              LDV_CONSTANT_INTEGER_CONSTANT (constant) = ldv_convert_integer_constant (t);
            }
          /*
          else
            LDV_CONVERT_ERROR (type); */
        }
      else
        LDV_ERROR ("can't find type of integer constant");

      break;

    case REAL_CST:
      LDV_CONSTANT_KIND (constant) = LDV_CONSTANT_SECOND;
      LDV_CONSTANT_FLOATING_CONSTANT (constant) = ldv_convert_floating_constant (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_CONSTANT_KIND (constant))
    return constant;

  LDV_ERROR ("constant wasn't converted");

  XDELETE (constant);

  return NULL;
}

/*
compound-statement:
    { block-item-listopt }

GNU extensions:

compound-statement:
    { label-declarations block-item-list }
*/
static ldv_compound_statement_ptr
ldv_convert_compound_statement (tree t)
{
  ldv_compound_statement_ptr compound_statement;
  ldv_block_item_list_ptr block_item_list, block_item_list_next, decl_block_item_list, decl_block_item_list_next, statement_block_item_list;
  tree block_decl, block_decl_type, block_statement;
  ldv_label_decls_ptr label_decls_next, label_decls_next_old;
  ldv_identifier_ptr identifier;

  compound_statement = XCNEW (struct ldv_compound_statement);
  decl_block_item_list = statement_block_item_list = NULL;

  label_decls_next_old = label_decls_next = NULL;

  switch (TREE_CODE (t))
    {
    case ASM_EXPR:
      LDV_COMPOUND_STATEMENT_BLOCK_ITEM_LIST (compound_statement) = ldv_convert_block_item_list (t);
      break;

    case BIND_EXPR:
      for (block_decl = BIND_EXPR_VARS (t); block_decl; block_decl = TREE_CHAIN (block_decl))
        {
          /* Skip constant declarations and use type of type declarations without name. */
          block_decl_type = block_decl;

          switch (TREE_CODE (block_decl))
            {
            case TYPE_DECL:
              if (!DECL_NAME (block_decl))
                if (!(block_decl_type = TREE_TYPE (block_decl)))
                  LDV_ERROR ("can't find type of type declaration");

              break;

            case CONST_DECL:
              block_decl_type = NULL;

              break;

            /* Convert label declarations */
            case LABEL_DECL:
              /* See on whether a label was defined explicitly by means of __label__. */
              if (TREE_LANG_FLAG_1 (block_decl))
                {
                  label_decls_next_old = label_decls_next;
                  label_decls_next = XCNEW (struct ldv_label_decls);
                  LDV_LABEL_DECLS_LABEL_DECLS (label_decls_next) = label_decls_next_old;

                  LDV_LABEL_DECLS_LABEL_DECL (label_decls_next) = ldv_convert_label_decl (block_decl);
                }

              /* Do not convert them like usual variables. */
              block_decl_type = NULL;

              break;

            /* TODO: make a special function for that (search __func__). */
            /* Skip artificial __func__ variable. */
            case VAR_DECL:
              if ((identifier = ldv_convert_identifier (block_decl)))
                {
                  if (LDV_IDENTIFIER_STR (identifier) && !strcmp (LDV_IDENTIFIER_STR (identifier), "__func__"))
                    block_decl_type = NULL;

                  XDELETE (LDV_IDENTIFIER_STR (identifier));
                  XDELETE (identifier);
                }
              else
                LDV_ERROR ("can't find variable name");

            default:
              ;
            }

          if (!block_decl_type)
            continue;

          if ((decl_block_item_list_next = ldv_convert_block_item_list (block_decl_type)))
            {
              if (decl_block_item_list)
                {
                  LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST (decl_block_item_list_next) = decl_block_item_list;
                  decl_block_item_list = decl_block_item_list_next;
                }
              else
                decl_block_item_list = decl_block_item_list_next;
            }
          else
            LDV_ERROR ("block item list wasn't converted");
        }

      if ((block_statement = BIND_EXPR_BODY (t)))
        {
          switch (TREE_CODE (block_statement))
            {
            case STATEMENT_LIST:
              if (ldv_is_not_empty_statement_list (block_statement))
                statement_block_item_list = ldv_convert_block_item_list (block_statement);

              break;

            case DECL_EXPR:
            case SAVE_EXPR:
              break;

            default:
              statement_block_item_list = ldv_convert_block_item_list (block_statement);
            }
        }
      else
        LDV_ERROR ("can't find statement list of compound statement");

      /* Merge label declarations if any */
      if (label_decls_next)
        LDV_COMPOUND_STATEMENT_LABEL_DECLS (compound_statement) = label_decls_next;

      /* Merge declaration and statement block item lists together. */
      if (statement_block_item_list)
        {
          block_item_list = statement_block_item_list;
          for (block_item_list_next = block_item_list; LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST (block_item_list_next); block_item_list_next = LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST (block_item_list_next)) ;
          LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST (block_item_list_next) = decl_block_item_list;
        }
      else
        block_item_list = decl_block_item_list;

      LDV_COMPOUND_STATEMENT_BLOCK_ITEM_LIST (compound_statement) = block_item_list;

      break;

    case STATEMENT_LIST:
      if (ldv_is_not_empty_statement_list (t))
        LDV_COMPOUND_STATEMENT_BLOCK_ITEM_LIST (compound_statement) = ldv_convert_block_item_list (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  return compound_statement;
}

/*
declaration:
    declaration-specifiers init-declarator-listopt ;
*/
ldv_decl_ptr
ldv_convert_decl (tree t)
{
  ldv_decl_ptr decl;

  decl = XCNEW (struct ldv_decl);

  /* Declaration specifiers of a declaration must be processed in a special way.
     For example, printing of struct/union fields and enumeration constant must
     be done. Pass the corresponding flag. */
  LDV_DECL_DECL_SPEC (decl) = ldv_convert_decl_spec (t, true);

  switch (TREE_CODE (t))
    {
    case RECORD_TYPE:
    case UNION_TYPE:
    case ENUMERAL_TYPE:

      break;

    case FUNCTION_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      LDV_DECL_INIT_DECLARATOR_LIST (decl) = ldv_convert_init_declarator_list (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  return decl;
}

/*
declaration-specifiers:
    storage-class-specifier declaration-specifiersopt
    type-specifier declaration-specifiersopt
    type-qualifier declaration-specifiersopt
    function-specifier declaration-specifiersopt
*/
static ldv_decl_spec_ptr
ldv_convert_decl_spec (tree t, bool is_decl_decl_spec)
{
  ldv_decl_spec_ptr decl_spec, decl_spec_cur;
  ldv_decl_spec_ptr decl_storage_class_spec;
  ldv_decl_spec_ptr decl_type_spec;
  ldv_decl_spec_ptr decl_type_qual;
  ldv_decl_spec_ptr decl_func_spec;
  bool is_decl_spec;

  decl_spec_cur = decl_spec = XCNEW (struct ldv_decl_spec);
  is_decl_spec = false;

  if ((decl_storage_class_spec = ldv_convert_storage_class_spec (t)))
    ldv_new_decl_spec (&is_decl_spec, &decl_spec_cur, decl_storage_class_spec, NULL, NULL, NULL);

  if ((decl_func_spec = ldv_convert_func_spec (t)))
    ldv_new_decl_spec (&is_decl_spec, &decl_spec_cur, NULL, NULL, NULL, decl_func_spec);

  if ((decl_type_spec = ldv_convert_type_spec (t, is_decl_decl_spec)))
    ldv_new_decl_spec (&is_decl_spec, &decl_spec_cur, NULL, decl_type_spec, NULL, NULL);

  if ((decl_type_qual = ldv_convert_type_qual (t)))
    ldv_new_decl_spec (&is_decl_spec, &decl_spec_cur, NULL, NULL, decl_type_qual, NULL);

  if (is_decl_spec)
    return decl_spec;

  LDV_ERROR ("declaration specifiers weren't converted");

  XDELETE (decl_spec);

  return NULL;
}

/*
declarator:
   pointeropt direct-declarator
*/
void
ldv_convert_declarator (tree t, bool is_decl_decl_spec, ldv_declarator_ptr *declarator_first_ptr, ldv_direct_declarator_ptr direct_declarator_prev, ldv_declarator_ptr declarator_prev)
{
  ldv_declarator_ptr declarator;
  ldv_pointer_ptr pointer;
  tree first_non_pointer;
  tree type_name;

  first_non_pointer = NULL;

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
    case PARM_DECL:
    case FIELD_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      ldv_convert_direct_declarator (t, is_decl_decl_spec, NULL, declarator_first_ptr, NULL, NULL);
      break;

    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case BOOLEAN_TYPE:
    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case ARRAY_TYPE:
    case FUNCTION_TYPE:
      if (!ldv_check_declarator_xor_direct_declarator (declarator_prev, direct_declarator_prev))
        break;

      /* Create required declarator consisting just of direct declarator. */
      if (direct_declarator_prev)
        {
          declarator = XCNEW (struct ldv_declarator);
          LDV_DECLARATOR_DIRECT_DECLARATOR (declarator) = direct_declarator_prev;
          *declarator_first_ptr = declarator;
        }
      else if (declarator_prev)
        *declarator_first_ptr = declarator_prev;

      break;

    case POINTER_TYPE:
      if ((type_name = TYPE_NAME (t)) && TREE_CODE(type_name) == TYPE_DECL)
        {
          if (direct_declarator_prev)
            {
              declarator = XCNEW (struct ldv_declarator);
              LDV_DECLARATOR_DIRECT_DECLARATOR (declarator) = direct_declarator_prev;
              *declarator_first_ptr = declarator;
            }
          else if (declarator_prev)
            *declarator_first_ptr = declarator_prev;
        }
      else if ((pointer = ldv_convert_pointer (t, &first_non_pointer)))
        {
          if (!direct_declarator_prev)
          {
            LDV_ERROR ("a previous direct declarator isn't specified");
            break;
          }

          declarator = XCNEW (struct ldv_declarator);
          LDV_DECLARATOR_POINTER (declarator) = pointer;

          LDV_DECLARATOR_DIRECT_DECLARATOR (declarator) = direct_declarator_prev;

          if (first_non_pointer)
            switch (TREE_CODE (first_non_pointer))
              {
              case VOID_TYPE:
              case INTEGER_TYPE:
              case REAL_TYPE:
              case BOOLEAN_TYPE:
              case ENUMERAL_TYPE:
              case RECORD_TYPE:
              case UNION_TYPE:
                ldv_convert_declarator (first_non_pointer, is_decl_decl_spec, declarator_first_ptr, NULL, declarator);
                break;

              case ARRAY_TYPE:
              case FUNCTION_TYPE:
              case POINTER_TYPE:
                if ((type_name = TYPE_NAME (first_non_pointer)) && TREE_CODE(type_name) == TYPE_DECL)
                  *declarator_first_ptr = declarator;
                else
                  ldv_convert_direct_declarator (first_non_pointer, is_decl_decl_spec, NULL, declarator_first_ptr, NULL, declarator);
                break;

              default:
                LDV_CONVERT_ERROR (first_non_pointer);
              }
          else
            LDV_ERROR ("first non pointer type isn't specified");
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }
}

static ldv_declarator_ptr
ldv_convert_declarator_internal (tree t, bool is_decl_decl_spec)
{
  ldv_declarator_ptr declarator;

  declarator = NULL;

  /* This function is used since internal gcc representation has reverse order
     of declarator and direct declarator sequences. */
  ldv_convert_declarator (t, is_decl_decl_spec, &declarator, NULL, NULL);

  return declarator;
}

static ldv_assignment_expr_ptr
ldv_convert_array_size (tree t)
{
  tree type_domain = NULL_TREE, array_size = NULL_TREE;
  ldv_assignment_expr_ptr assignment_expr = NULL;
  ldv_integer_constant_ptr integer_constant = NULL;

  /* If field declation has array type with flexible number of elements [],
     then TYPE_SIZE is NULL (#3378). */
  if (!TYPE_SIZE (t))
    return NULL;

  /* Absence of type domain was already treated as absence of array size for
     direct declarators. */
  if ((type_domain = TYPE_DOMAIN (t)))
    {
      if ((array_size = TYPE_MAX_VALUE (type_domain)) || (array_size = TYPE_MIN_VALUE (type_domain)))
        {
          ldv_constant_current = NULL;

          assignment_expr = ldv_convert_assignment_expr (array_size, LDV_CONVERT_EXPR_RECURSION_LIMIT);

          /* GCC keeps array index boundaries. Array size is a maximum value of
             index plus 1. The only exception is actual zero array size, but in
             this case there is just a left boundary. */
          if (TYPE_MAX_VALUE (type_domain))
            {
              if (ldv_constant_current)
                {
                  switch (LDV_CONSTANT_KIND (ldv_constant_current))
                    {
                      case LDV_CONSTANT_FIRST:
                        if ((integer_constant = LDV_CONSTANT_INTEGER_CONSTANT (ldv_constant_current)))
                          LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) ++;
                        else
                          LDV_ERROR ("can't find array size");

                        break;

                      default:
                        LDV_ERROR ("something strange");
                    }
                }
              else
                LDV_ERROR ("array size wasn't found or isn't a constant");
            }
        }
      else
        LDV_ERROR ("can't find array size");
    }

  return assignment_expr;
}

/*
direct-abstract-declarator:
    ( abstract-declarator )
    direct-abstract-declaratoropt [ assignment-expressionopt ]
    direct-abstract-declaratoropt [ * ]
    direct-abstract-declaratoropt ( parameter-type-listopt )
*/
static void
ldv_convert_direct_abstract_declarator (tree t, ldv_abstract_declarator_ptr *abstract_declarator_first_ptr, ldv_direct_abstract_declarator_ptr direct_abstract_declarator_prev, ldv_abstract_declarator_ptr abstract_declarator_prev)
{
  ldv_direct_abstract_declarator_ptr direct_abstract_declarator, direct_abstract_declarator_next;
  tree array_type;
  tree func_ret_type;

  direct_abstract_declarator = XCNEW (struct ldv_direct_abstract_declarator);

  switch (TREE_CODE (t))
    {
    case ARRAY_TYPE:
      if (abstract_declarator_prev)
        {
          LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator) = LDV_DIRECT_ABSTRACT_DECLARATOR_FIRST;
          LDV_DIRECT_ABSTRACT_DECLARATOR_ABSTRACT_DECLARATOR (direct_abstract_declarator) = abstract_declarator_prev;
          direct_abstract_declarator_prev = direct_abstract_declarator;
          direct_abstract_declarator_next = XCNEW (struct ldv_direct_abstract_declarator);
        }
      else
        direct_abstract_declarator_next = direct_abstract_declarator;

      LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator_next) = LDV_DIRECT_ABSTRACT_DECLARATOR_SECOND;
      LDV_DIRECT_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (direct_abstract_declarator_next) = direct_abstract_declarator_prev;
      LDV_DIRECT_ABSTRACT_DECLARATOR_ASSIGN_EXPR (direct_abstract_declarator_next) = ldv_convert_array_size (t);

      if ((array_type = TREE_TYPE (t)))
        switch (TREE_CODE (array_type))
          {
          case ARRAY_TYPE:
            ldv_convert_direct_abstract_declarator (array_type, abstract_declarator_first_ptr, direct_abstract_declarator_next, NULL);
            break;

          case VOID_TYPE:
          case INTEGER_TYPE:
          case REAL_TYPE:
          case BOOLEAN_TYPE:
          case POINTER_TYPE:
          case ENUMERAL_TYPE:
          case RECORD_TYPE:
          case UNION_TYPE:
            ldv_convert_abstract_declarator (array_type, abstract_declarator_first_ptr, direct_abstract_declarator_next, NULL);
            break;

          default:
            LDV_CONVERT_ERROR (t);
          }
      else
        LDV_ERROR ("can't find array type");

      break;

    case FUNCTION_TYPE:
      if (abstract_declarator_prev)
        {
          LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator) = LDV_DIRECT_ABSTRACT_DECLARATOR_FIRST;
          LDV_DIRECT_ABSTRACT_DECLARATOR_ABSTRACT_DECLARATOR (direct_abstract_declarator) = abstract_declarator_prev;
          direct_abstract_declarator_prev = direct_abstract_declarator;
          direct_abstract_declarator_next = XCNEW (struct ldv_direct_abstract_declarator);
        }
      else
        {
          if (LDV_DIRECT_ABSTRACT_DECLARATOR_PARAM_TYPE_LIST (direct_abstract_declarator_prev))
            {
              LDV_DIRECT_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (direct_abstract_declarator) = direct_abstract_declarator_prev;
              direct_abstract_declarator_prev = direct_abstract_declarator;
              direct_abstract_declarator_next = XCNEW (struct ldv_direct_abstract_declarator);
            }
          else
            direct_abstract_declarator_next = direct_abstract_declarator;
        }

      LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator_next) = LDV_DIRECT_ABSTRACT_DECLARATOR_FOURTH;
      LDV_DIRECT_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (direct_abstract_declarator_next) = direct_abstract_declarator_prev;
      LDV_DIRECT_ABSTRACT_DECLARATOR_PARAM_TYPE_LIST (direct_abstract_declarator_next) = ldv_convert_param_type_list (t);

      if ((func_ret_type = TREE_TYPE (t)))
        switch (TREE_CODE (func_ret_type))
          {
          case VOID_TYPE:
          case INTEGER_TYPE:
          case REAL_TYPE:
          case BOOLEAN_TYPE:
          case POINTER_TYPE:
          case ENUMERAL_TYPE:
          case RECORD_TYPE:
          case UNION_TYPE:
            ldv_convert_abstract_declarator (func_ret_type, abstract_declarator_first_ptr, direct_abstract_declarator_next, NULL);
            break;

          default:
            LDV_CONVERT_ERROR (t);
          }
      else
        LDV_ERROR ("can't find function return type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }
}
/*
direct-declarator:
    identifier
    ( declarator )
    direct-declarator [ type-qualifier-listopt assignment-expressionopt ]
    direct-declarator [ static type-qualifier-listopt assignment-expression ]
    direct-declarator [ type-qualifier-list static assignment-expression ]
    direct-declarator [ type-qualifier-listopt * ]
    direct-declarator ( parameter-type-list )
    direct-declarator ( identifier-listopt )
*/
static void
ldv_convert_direct_declarator (tree t, bool is_decl_decl_spec, tree decl, ldv_declarator_ptr *declarator_first_ptr, ldv_direct_declarator_ptr direct_declarator_prev, ldv_declarator_ptr declarator_prev)
{
  ldv_direct_declarator_ptr direct_declarator, direct_declarator_next;
  tree decl_type;
  tree array_type;
  tree func_ret_type;
  tree type_name;

  direct_declarator = XCNEW (struct ldv_direct_declarator);

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
    case PARM_DECL:
    case FIELD_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      /* At the beginning process a declaration name that is a last direct
         declarator in a given declarator and direct declarator sequence. */
      LDV_DIRECT_DECLARATOR_KIND (direct_declarator) = LDV_DIRECT_DECLARATOR_FIRST;
      LDV_DIRECT_DECLARATOR_IDENTIFIER (direct_declarator) = ldv_convert_identifier (t);

      LDV_DIRECT_DECLARATOR_LOCATION (direct_declarator) = ldv_convert_location (t);

      /* Then recursively analyze declaration type to build the rest part of a
         declarator and direct declarator sequence. */
      if (TREE_CODE (t) == TYPE_DECL)
        {
          if (!(decl_type = DECL_ORIGINAL_TYPE (t)))
            LDV_ERROR ("can't find original declaration type");
        }
      else if (!(decl_type = TREE_TYPE (t)))
        LDV_ERROR ("can't find declaration type");

      /* TODO: it seems that this if-branch is redundant. */
      if ((type_name = TYPE_NAME (decl_type)) && TREE_CODE(type_name) == TYPE_DECL)
        ldv_convert_declarator (decl_type, is_decl_decl_spec, declarator_first_ptr, direct_declarator, NULL);
      else
        {
          switch (TREE_CODE (decl_type))
            {
            case ARRAY_TYPE:
            case FUNCTION_TYPE:
              // TODO: remove tree code checking from here! Type declarations shouldn't passed here!
              if (TREE_CODE (t) == FUNCTION_DECL)
                ldv_convert_direct_declarator (decl_type, is_decl_decl_spec, t, declarator_first_ptr, direct_declarator, NULL);
              else
                ldv_convert_direct_declarator (decl_type, is_decl_decl_spec, NULL_TREE, declarator_first_ptr, direct_declarator, NULL);

              break;

            case VOID_TYPE:
            case INTEGER_TYPE:
            case REAL_TYPE:
            case BOOLEAN_TYPE:
            case POINTER_TYPE:
            case ENUMERAL_TYPE:
            case RECORD_TYPE:
            case UNION_TYPE:
              ldv_convert_declarator (decl_type, is_decl_decl_spec, declarator_first_ptr, direct_declarator, NULL);
              break;

            default:
              LDV_CONVERT_ERROR (decl_type);
            }
        }

      break;

    case ARRAY_TYPE:
      if (!ldv_check_declarator_xor_direct_declarator (declarator_prev, direct_declarator_prev))
        break;

      if (declarator_prev)
        {
          LDV_DIRECT_DECLARATOR_KIND (direct_declarator) = LDV_DIRECT_DECLARATOR_SECOND;
          LDV_DIRECT_DECLARATOR_DECLARATOR (direct_declarator) = declarator_prev;
          direct_declarator_prev = direct_declarator;
          direct_declarator_next = XCNEW (struct ldv_direct_declarator);
        }
      else
        direct_declarator_next = direct_declarator;

      LDV_DIRECT_DECLARATOR_KIND (direct_declarator_next) = LDV_DIRECT_DECLARATOR_THIRD;
      LDV_DIRECT_DECLARATOR_DIRECT_DECLARATOR (direct_declarator_next) = direct_declarator_prev;
      LDV_DIRECT_DECLARATOR_ASSIGNMENT_EXPR (direct_declarator_next) = ldv_convert_array_size (t);

      if ((array_type = TREE_TYPE (t)))
        switch (TREE_CODE (array_type))
          {
          case ARRAY_TYPE:
            ldv_convert_direct_declarator (array_type, is_decl_decl_spec, NULL, declarator_first_ptr, direct_declarator_next, NULL);
            break;

          case VOID_TYPE:
          case INTEGER_TYPE:
          case REAL_TYPE:
          case BOOLEAN_TYPE:
          case POINTER_TYPE:
          case ENUMERAL_TYPE:
          case RECORD_TYPE:
          case UNION_TYPE:
            ldv_convert_declarator (array_type, is_decl_decl_spec, declarator_first_ptr, direct_declarator_next, NULL);
            break;

          default:
            LDV_CONVERT_ERROR (t);
          }
      else
        LDV_ERROR ("can't find array type");

      break;

    case FUNCTION_TYPE:
      if (!ldv_check_declarator_xor_direct_declarator (declarator_prev, direct_declarator_prev))
        break;

      if (declarator_prev)
        {
          LDV_DIRECT_DECLARATOR_KIND (direct_declarator) = LDV_DIRECT_DECLARATOR_SECOND;
          LDV_DIRECT_DECLARATOR_DECLARATOR (direct_declarator) = declarator_prev;
          direct_declarator_prev = direct_declarator;
          direct_declarator_next = XCNEW (struct ldv_direct_declarator);
        }
      else
        {
          if (LDV_DIRECT_DECLARATOR_PARAM_TYPE_LIST (direct_declarator_prev))
            {
              LDV_DIRECT_DECLARATOR_DIRECT_DECLARATOR (direct_declarator) = direct_declarator_prev;
              direct_declarator_prev = direct_declarator;
              direct_declarator_next = XCNEW (struct ldv_direct_declarator);
            }
          else
            direct_declarator_next = direct_declarator;
        }

      LDV_DIRECT_DECLARATOR_KIND (direct_declarator_next) = LDV_DIRECT_DECLARATOR_SEVENTH;
      LDV_DIRECT_DECLARATOR_DIRECT_DECLARATOR (direct_declarator_next) = direct_declarator_prev;

      if (decl && DECL_SAVED_TREE (decl))
        LDV_DIRECT_DECLARATOR_PARAM_TYPE_LIST (direct_declarator_next) = ldv_convert_param_type_list (decl);
      else
        LDV_DIRECT_DECLARATOR_PARAM_TYPE_LIST (direct_declarator_next) = ldv_convert_param_type_list (t);

      /* Function declarations (prototypes) can omit argument declarations. Use the last form of direct declarators to
       * represent this. In other cases that form is not used. */
      if (!LDV_DIRECT_DECLARATOR_PARAM_TYPE_LIST (direct_declarator_next))
        LDV_DIRECT_DECLARATOR_KIND (direct_declarator_next) = LDV_DIRECT_DECLARATOR_EIGHT;

      if ((func_ret_type = TREE_TYPE (t)))
        switch (TREE_CODE (func_ret_type))
          {
          case VOID_TYPE:
          case INTEGER_TYPE:
          case REAL_TYPE:
          case BOOLEAN_TYPE:
          case POINTER_TYPE:
          case ENUMERAL_TYPE:
          case RECORD_TYPE:
          case UNION_TYPE:
            ldv_convert_declarator (func_ret_type, is_decl_decl_spec, declarator_first_ptr, direct_declarator_next, NULL);
            break;

          default:
            LDV_CONVERT_ERROR (t);
          }
      else
        LDV_ERROR ("can't find function return type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }
}

/*
designation:
    designator-list =
*/
static ldv_designation_ptr
ldv_convert_designation (tree t)
{
  ldv_designation_ptr designation;

  designation = XCNEW (struct ldv_designation);

  LDV_DESIGNATION_DESIGNATOR_LIST (designation) = ldv_convert_designator_list (t);

  if (LDV_DESIGNATION_DESIGNATOR_LIST (designation))
    return designation;

  LDV_ERROR ("designation wasn't converted");

  XDELETE (designation);

  return NULL;
}

/*
designator:
    [ constant-expression ]
    . identifier
*/
static ldv_designator_ptr
ldv_convert_designator (tree t)
{
  ldv_designator_ptr designator;

  designator = XCNEW (struct ldv_designator);

  switch (TREE_CODE (t))
    {
    case INTEGER_CST:
      LDV_DESIGNATOR_KIND (designator) = LDV_DESIGNATOR_FIRST;
      LDV_DESIGNATOR_CONST_EXPR (designator) = ldv_convert_const_expr (t);

      break;

    case FIELD_DECL:
      LDV_DESIGNATOR_KIND (designator) = LDV_DESIGNATOR_SECOND;
      LDV_DESIGNATOR_IDENTIFIER (designator) = ldv_convert_identifier (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_DESIGNATOR_KIND (designator))
    return designator;

  LDV_ERROR ("designator wasn't converted");

  XDELETE (designator);

  return NULL;
}

/*
designator-list:
    designator
    designator-list designator
*/
static ldv_designator_list_ptr
ldv_convert_designator_list (tree t)
{
  ldv_designator_list_ptr designator_list;

  designator_list = XCNEW (struct ldv_designator_list);

  /* Gcc always has just one designator per a given designator list. */
  LDV_DESIGNATOR_LIST_KIND (designator_list) = LDV_DESIGNATOR_LIST_FIRST;
  LDV_DESIGNATOR_LIST_DESIGNATOR (designator_list) = ldv_convert_designator (t);

  if (LDV_DESIGNATOR_LIST_KIND (designator_list))
    return designator_list;

  LDV_ERROR ("designator list wasn't converted");

  XDELETE (designator_list);

  return NULL;
}

/*
enumerator:
    enumeration-constant
    enumeration-constant = constant-expression
*/
static ldv_enum_ptr
ldv_convert_enum (tree t)
{
  ldv_enum_ptr ldv_enum;
  tree enum_const, const_expr;

  ldv_enum = XCNEW (struct ldv_enum);

  switch (TREE_CODE (t))
    {
    case TREE_LIST:
      LDV_ENUM_KIND (ldv_enum) = LDV_ENUM_FIRST;

      if ((enum_const = TREE_PURPOSE (t)))
        LDV_ENUM_ENUM_CONST (ldv_enum) = ldv_convert_enum_const (enum_const);
      else
        LDV_ERROR ("can't get an enumeration constant");

      if ((const_expr = TREE_VALUE (t)))
        {
          LDV_ENUM_KIND (ldv_enum) = LDV_ENUM_SECOND;
          ldv_is_convert_enum_const_const_expr = true;
          LDV_ENUM_CONST_EXPR (ldv_enum) = ldv_convert_const_expr (const_expr);
          ldv_is_convert_enum_const_const_expr = false;
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_ENUM_ENUM_CONST (ldv_enum))
    return ldv_enum;

  LDV_ERROR ("enumerator wasn't converted");

  XDELETE (ldv_enum);

  return NULL;
}

/*
enumeration-constant:
    identifier
*/
static ldv_enum_const_ptr
ldv_convert_enum_const (tree t)
{
  ldv_enum_const_ptr enum_const;
  const char *enum_value_name_str;

  enum_const = XCNEW (struct ldv_enum_const);

  switch (TREE_CODE (t))
    {
    case IDENTIFIER_NODE:
      if ((enum_value_name_str = IDENTIFIER_POINTER (t)))
        LDV_ENUM_CONST_ID (enum_const) = enum_value_name_str;
      else
        LDV_ERROR ("enumeration constant hasn't a name");
      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_ENUM_CONST_ID (enum_const))
    return enum_const;

  LDV_ERROR ("enumeration constant wasn't converted");

  XDELETE (enum_const);

  return NULL;
}

/*
enumerator-list:
    enumerator
    enumerator-list , enumerator
*/
static ldv_enum_list_ptr
ldv_convert_enum_list (tree t)
{
  ldv_enum_list_ptr enum_list, enum_list_next, enum_list_next_next;
  tree enum_value_next;

  enum_list = XCNEW (struct ldv_enum_list);
  enum_list_next = NULL;

  switch (TREE_CODE (t))
    {
    case TREE_LIST:
      if ((enum_value_next = TREE_CHAIN (t)))
        {
          enum_list_next = ldv_convert_enum_list (enum_value_next);
          for (enum_list_next_next = enum_list_next; LDV_ENUM_LIST_ENUM_LIST (enum_list_next_next); enum_list_next_next = LDV_ENUM_LIST_ENUM_LIST (enum_list_next_next)) ;
          LDV_ENUM_LIST_ENUM_LIST (enum_list_next_next) = enum_list;
        }
      LDV_ENUM_LIST_ENUM (enum_list) = ldv_convert_enum (t);
      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (enum_list_next)
    return enum_list_next;

  if (LDV_ENUM_LIST_ENUM (enum_list) || LDV_ENUM_LIST_ENUM_LIST (enum_list))
    return enum_list;

  LDV_ERROR ("enumeration list wasn't converted");

  XDELETE (enum_list);

  return NULL;
}

/*
enum-specifier:
    enum identifieropt { enumerator-list }
    enum identifieropt { enumerator-list , }
    enum identifier
*/
static ldv_decl_spec_ptr
ldv_convert_enum_spec (tree t, bool is_decl_decl_spec)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_type_spec_ptr type_spec;
  ldv_enum_spec_ptr enum_spec;
  ldv_enum_list_ptr enum_list;
  tree enum_values;

  decl_spec = XCNEW (struct ldv_decl_spec);
  type_spec = XCNEW (struct ldv_type_spec);
  enum_spec = XCNEW (struct ldv_enum_spec);

  LDV_DECL_SPEC_TYPE_SPEC (decl_spec) = type_spec;
  LDV_TYPE_SPEC_ENUM_SPEC (type_spec) = enum_spec;

  switch (TREE_CODE (t))
    {
    case ENUMERAL_TYPE:
      LDV_TYPE_SPEC_KIND (type_spec) = LDV_TYPE_SPEC_SECOND;

      if (!TYPE_NAME (t) && ldv_disable_anon_enum_spec)
        {
            if ((enum_values = TYPE_VALUES (t)))
              if ((enum_list = ldv_convert_enum_list (enum_values)))
                LDV_ENUM_SPEC_ENUM_LIST (enum_spec) = enum_list;
	    }
      else
        {
          LDV_ENUM_SPEC_IDENTIFIER (enum_spec) = ldv_convert_identifier (t);

          if (is_decl_decl_spec)
            if ((enum_values = TYPE_VALUES (t)))
              if ((enum_list = ldv_convert_enum_list (enum_values)))
                LDV_ENUM_SPEC_ENUM_LIST (enum_spec) = enum_list;
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_TYPE_SPEC_KIND (type_spec))
    return decl_spec;

  LDV_ERROR ("enum specifier wasn't converted");

  XDELETE (enum_spec);
  XDELETE (type_spec);
  XDELETE (decl_spec);

  return NULL;
}

/*
enumeration-constant:
    identifier
*/
static ldv_enumeration_constant_ptr
ldv_convert_enumeration_constant (tree t)
{
  ldv_enumeration_constant_ptr enumeration_constant;
  tree type, value, purpose;

  enumeration_constant = XCNEW (struct ldv_enumeration_constant);

  /* Don't warn if no corresponding constant was find since here the check of
     whether there is such enumeration constant in general is also performed. */

  switch (TREE_CODE (t))
    {
    case INTEGER_CST:
      if ((type = TREE_TYPE (t)))
        {
          if (TREE_CODE (type) == ENUMERAL_TYPE)
            {
              for (value = TYPE_VALUES (type); value != NULL_TREE && !tree_int_cst_equal (TREE_VALUE (value), t); value = TREE_CHAIN (value));

              if (value != NULL_TREE)
                {
                  if ((purpose = TREE_PURPOSE (value)))
                    LDV_ENUMERATION_CONSTANT_IDENTIFIER (enumeration_constant) = ldv_convert_identifier (purpose);
                  else
                    LDV_ERROR ("can't find enumeration constant name");
                }
            }
          else
            LDV_ERROR ("type of constant isn't enumeration");
        }
      else
        LDV_ERROR ("can't find type of integer constant");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_ENUMERATION_CONSTANT_IDENTIFIER (enumeration_constant))
    return enumeration_constant;

  XDELETE (enumeration_constant);

  return NULL;
}

/*
equality-expression:
    relational-expression
    equality-expression == relational-expression
    equality-expression != relational-expression
*/
static ldv_equality_expr_ptr
ldv_convert_equality_expr (tree t, unsigned int recursion_limit)
{
  ldv_equality_expr_ptr equality_expr;
  tree op1, op2;

  equality_expr = XCNEW (struct ldv_equality_expr);

  switch (TREE_CODE (t))
    {
    case EQ_EXPR:
    case NE_EXPR:
      if (TREE_CODE (t) == EQ_EXPR)
        LDV_EQUALITY_EXPR_KIND (equality_expr) = LDV_EQUALITY_EXPR_SECOND;
      else
        LDV_EQUALITY_EXPR_KIND (equality_expr) = LDV_EQUALITY_EXPR_THIRD;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_EQUALITY_EXPR_EQUALITY_EXPR (equality_expr) = ldv_convert_equality_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of equality expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_EQUALITY_EXPR_RELATIONAL_EXPR (equality_expr) = ldv_convert_relational_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of equality expression");

      LDV_EQUALITY_EXPR_LOCATION (equality_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_EQUALITY_EXPR_KIND (equality_expr) = LDV_EQUALITY_EXPR_FIRST;
      LDV_EQUALITY_EXPR_RELATIONAL_EXPR (equality_expr) = ldv_convert_relational_expr (t, recursion_limit);
    }

  if (LDV_EQUALITY_EXPR_KIND (equality_expr))
    return equality_expr;

  LDV_ERROR ("equality expression wasn't converted");

  XDELETE (equality_expr);

  return NULL;
}

/*
exclusive-OR-expression:
    AND-expression
    exclusive-OR-expression ^ AND-expression
*/
static ldv_exclusive_or_expr_ptr
ldv_convert_exclusive_or_expr (tree t, unsigned int recursion_limit)
{
  ldv_exclusive_or_expr_ptr exclusive_or_expr;
  tree op1, op2;

  exclusive_or_expr = XCNEW (struct ldv_exclusive_or_expr);

  switch (TREE_CODE (t))
    {
    case BIT_XOR_EXPR:
    case TRUTH_XOR_EXPR:
      LDV_EXCLUSIVE_OR_EXPR_KIND (exclusive_or_expr) = LDV_EXCLUSIVE_OR_EXPR_SECOND;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_EXCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR (exclusive_or_expr) = ldv_convert_exclusive_or_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of exclusive or expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_EXCLUSIVE_OR_EXPR_AND_EXPR (exclusive_or_expr) = ldv_convert_and_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of exclusive or expression");

      LDV_EXCLUSIVE_OR_EXPR_LOCATION (exclusive_or_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_EXCLUSIVE_OR_EXPR_KIND (exclusive_or_expr) = LDV_EXCLUSIVE_OR_EXPR_FIRST;
      LDV_EXCLUSIVE_OR_EXPR_AND_EXPR (exclusive_or_expr) = ldv_convert_and_expr (t, recursion_limit);
    }

  if (LDV_EXCLUSIVE_OR_EXPR_KIND (exclusive_or_expr))
    return exclusive_or_expr;

  LDV_ERROR ("exclusive or expression wasn't converted");

  XDELETE (exclusive_or_expr);

  return NULL;
}

/*
expression:
    assignment-expression
    expression , assignment-expression
*/
static ldv_expr_ptr
ldv_convert_expr (tree t, unsigned int recursion_limit)
{
  ldv_expr_ptr expr, expr_next;
  ldv_assignment_expr_ptr assignment_expr;
  tree type, op1, op2, op3;
  tree addr, cast, cast_type, offset, ptr_plus, indirect_ref;
  HOST_WIDE_INT bitpos, size;

  expr = XCNEW (struct ldv_expr);
  expr_next = NULL;

  switch (TREE_CODE (t))
    {
    case COMPOUND_EXPR:
      if (!(op1 = LDV_OP_FIRST (t)))
        {
          LDV_ERROR ("can't find the first operand of compound expression");
          break;
        }

      if (!(op2 = LDV_OP_SECOND (t)))
        {
          LDV_ERROR ("can't find the second operand of compound expression");
          break;
        }

      /* Skip save expressions since they introduce artificial things. */
      if (TREE_CODE (op1) == SAVE_EXPR)
        {
          LDV_EXPR_KIND (expr) = LDV_EXPR_FIRST;
          LDV_EXPR_ASSIGNMENT_EXPR (expr) = ldv_convert_assignment_expr (op2, recursion_limit);
        }
      else
        {
          LDV_EXPR_KIND (expr) = LDV_EXPR_SECOND;

          expr_next = ldv_convert_expr (op1, recursion_limit);
          LDV_EXPR_EXPR (expr) = expr_next;

          if ((assignment_expr = ldv_convert_assignment_expr (op2, recursion_limit)))
            LDV_EXPR_ASSIGNMENT_EXPR (expr) = assignment_expr;
          else
            LDV_ERROR ("assigment expression wasn't converted");
        }

      break;

    /* Bit field reference is stored as bit field type, variable, bit field size
       and bit field offset. To print it correctly translate this data to
       ((type *)&variable)[bit_field_offset / sizeof (bit field type)] */
    case BIT_FIELD_REF:
      if (!(type = TREE_TYPE (t)))
        {
          LDV_ERROR ("can't find the type of bit field reference");
          break;
        }

      if (!(op1 = LDV_OP_FIRST (t)))
        {
          LDV_ERROR ("can't find the first operand of bit field reference");
          break;
        }

      if (!(op3 = LDV_OP_THIRD (t)))
        {
          LDV_ERROR ("can't find the third operand of bit field reference");
          break;
        }

      if (!(addr = build1 (ADDR_EXPR, type, op1)))
        {
          LDV_ERROR ("can't build address for variable");
          break;
        }

      if (!(cast_type = build_pointer_type (type)))
        {
          LDV_ERROR ("can't build pointer for type");
          break;
        }

      if (!(cast = build1 (CONVERT_EXPR, cast_type, addr)))
        {
          LDV_ERROR ("can't build cast for address");
          break;
        }

      bitpos = tree_to_shwi (op3);
      size = tree_to_shwi (TYPE_SIZE (type));

      if (bitpos % size)
        {
          LDV_ERROR ("bit field position isn't multiple of type size");
          break;
        }

      offset = build_int_cst (long_unsigned_type_node, (bitpos / size));

      if (!(ptr_plus = build2 (POINTER_PLUS_EXPR, cast_type, cast, offset)))
        {
          LDV_ERROR ("can't build cast for address");
          break;
        }

      if (!(indirect_ref = build1 (INDIRECT_REF, TREE_TYPE (ptr_plus), ptr_plus)))
        {
          LDV_ERROR ("can't build indirect reference for pointer plus expression");
          break;
        }

      LDV_EXPR_KIND (expr) = LDV_EXPR_FIRST;
      LDV_EXPR_ASSIGNMENT_EXPR (expr) = ldv_convert_assignment_expr (indirect_ref, recursion_limit);

      break;

    default:
      LDV_EXPR_KIND (expr) = LDV_EXPR_FIRST;
      LDV_EXPR_ASSIGNMENT_EXPR (expr) = ldv_convert_assignment_expr (t, recursion_limit);
    }

  if ((LDV_EXPR_KIND (expr)))
    return expr;

  LDV_ERROR ("expression wasn't converted");

  XDELETE (expr);

  return NULL;
}

/*
external-declaration:
    function-definition
    declaration
*/
ldv_ext_decl_ptr
ldv_convert_ext_decl (tree t, bool isdecl)
{
  ldv_ext_decl_ptr ext_decl;

  ext_decl = XCNEW (struct ldv_ext_decl);

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
      if (isdecl)
        {
          LDV_EXT_DECL_KIND (ext_decl) = LDV_EXT_DECL_SECOND;
          LDV_EXT_DECL_DECL (ext_decl) = ldv_convert_decl (t);
        }
      else
        {
          LDV_EXT_DECL_KIND (ext_decl) = LDV_EXT_DECL_FIRST;
          LDV_EXT_DECL_FUNC_DEF (ext_decl) = ldv_convert_func_def (t);
        }

      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case ENUMERAL_TYPE:
    case TYPE_DECL:
    case VAR_DECL:
      LDV_EXT_DECL_KIND (ext_decl) = LDV_EXT_DECL_SECOND;
      LDV_EXT_DECL_DECL (ext_decl) = ldv_convert_decl (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_EXT_DECL_KIND (ext_decl))
    return ext_decl;

  LDV_ERROR ("external declaration wasn't converted");

  XDELETE (ext_decl);

  return NULL;
}

/*
expression-statement:
    expressionopt ;
*/
static ldv_expr_statement_ptr
ldv_convert_expr_statement (tree t)
{
  ldv_expr_statement_ptr expr_statement;

  expr_statement = XCNEW (struct ldv_expr_statement);

  LDV_EXPR_STATEMENT_EXPR (expr_statement) = ldv_convert_expr (t, LDV_CONVERT_EXPR_RECURSION_LIMIT);
  LDV_EXPR_STATEMENT_LOCATION (expr_statement) = ldv_convert_location (t);

  /* We believe that gcc doesn't contain null expression statement here. */
  if (LDV_EXPR_STATEMENT_EXPR (expr_statement))
    return expr_statement;

  LDV_ERROR ("expression statement wasn't converted");

  XDELETE (expr_statement);

  return NULL;
}

/*
floating-constant:
    decimal-floating-constant
    hexadecimal-floating-constant
*/
static ldv_floating_constant_ptr
ldv_convert_floating_constant (tree t)
{
  ldv_floating_constant_ptr floating_constant;
  struct real_value *cst;

  floating_constant = XCNEW (struct ldv_floating_constant);

  switch (TREE_CODE (t))
    {
    case REAL_CST:
      LDV_FLOATING_CONSTANT_KIND (floating_constant) = LDV_FLOATING_CONSTANT_FIRST;
// TODO see on this!!! #if !defined(REAL_IS_NOT_DOUBLE) || defined(REAL_ARITHMETIC)
      if ((cst = TREE_REAL_CST_PTR (t)))
        real_to_decimal (LDV_FLOATING_CONSTANT_CST (floating_constant), cst, sizeof (LDV_FLOATING_CONSTANT_CST (floating_constant)), 0, true);
      else
        LDV_ERROR ("can't find floating constant");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_FLOATING_CONSTANT_KIND (floating_constant))
    return floating_constant;

  LDV_ERROR ("floating constant wasn't converted");

  XDELETE (floating_constant);

  return NULL;
}

/*
function-definition:
    declaration-specifiers declarator declaration-listopt compound-statement
*/
static ldv_func_def_ptr
ldv_convert_func_def (tree t)
{
  ldv_func_def_ptr func_def;
  tree func_body;

  func_def = XCNEW (struct ldv_func_def);

  LDV_FUNC_DEF_DECL_SPEC (func_def) = ldv_convert_decl_spec (t, false);
  LDV_FUNC_DEF_DECLARATOR (func_def) = ldv_convert_declarator_internal (t, false);

  if ((func_body = DECL_SAVED_TREE (t)))
    LDV_FUNC_DEF_COMPOUND_STATEMENT (func_def) = ldv_convert_compound_statement (func_body);
  else
    LDV_ERROR ("can't find function body");

  if (LDV_FUNC_DEF_DECL_SPEC (func_def) && LDV_FUNC_DEF_DECLARATOR (func_def) && LDV_FUNC_DEF_COMPOUND_STATEMENT (func_def))
    return func_def;

  LDV_ERROR ("function definition wasn't converted");

  XDELETE (func_def);

  return NULL;
}

/*
function-specifier:
    inline
*/
static ldv_decl_spec_ptr
ldv_convert_func_spec (tree t)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_func_spec_ptr func_spec;

  decl_spec = XCNEW (struct ldv_decl_spec);
  func_spec = LDV_DECL_SPEC_FUNC_SPEC (decl_spec) = XCNEW (struct ldv_func_spec);

  /* Note that in accordance with the C standard may be just one function
     specifier for functions. */
  switch (TREE_CODE (t))
    {
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case BOOLEAN_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case ENUMERAL_TYPE:
      break;

    case POINTER_TYPE:
      break;

    case PARM_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      break;

    case FUNCTION_DECL:
      if (DECL_DECLARED_INLINE_P (t))
        LDV_FUNC_SPEC_KIND (func_spec) = LDV_FUNC_SPEC_INLINE;
      break;

    default:
      LDV_CONVERT_ERROR (t);
      break;
    }

  if (LDV_FUNC_SPEC_KIND (func_spec))
    return decl_spec;

  XDELETE (func_spec);
  XDELETE (decl_spec);

  return NULL;
}

/*
identifier:
    identifier-nondigit
    identifier identifier-nondigit
    identifier digit
*/
ldv_identifier_ptr
ldv_convert_identifier (tree t)
{
  ldv_identifier_ptr identifier;
  tree decl_name;
  const char *decl_name_str;
  char *decl_uid_name_str;
  unsigned int decl_uid;
  const char *decl_uid_str;
  const char *id, *str;
  tree enum_name;
  const char *enum_name_str;
  char *enum_uid_name_str;
  unsigned int enum_uid;
  const char *enum_uid_str;

  identifier = XCNEW (struct ldv_identifier);

  switch (TREE_CODE (t))
    {
    case FIELD_DECL:
    case FUNCTION_DECL:
    case LABEL_DECL:
    case PARM_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      if ((decl_name = DECL_NAME (t)))
        {
          if ((decl_name_str = IDENTIFIER_POINTER (decl_name)))
            LDV_IDENTIFIER_STR (identifier) = xstrdup (decl_name_str);
          else
            LDV_CONVERT_ERROR (t);
        }
      else if ((decl_uid = DECL_UID (t)))
        {
          /* TODO: implement a special function for this (below there are two similar sequences of actions). */
          decl_uid_str = ldv_itoa (decl_uid);
          decl_uid_name_str = XCNEWVEC (char, 3 + 1 + strlen (decl_uid_str) + 1);
          sprintf (decl_uid_name_str, "ldv_%d", decl_uid);
          XDELETE (CONST_CAST (char *, decl_uid_str));
          LDV_IDENTIFIER_STR (identifier) = decl_uid_name_str;
        }
      else
        LDV_CONVERT_ERROR (t);

      break;

    case IDENTIFIER_NODE:
      if ((id = IDENTIFIER_POINTER (t)))
        LDV_IDENTIFIER_STR (identifier) = xstrdup (id);
      else
        LDV_CONVERT_ERROR (t);

      break;

    case STRING_CST:
      if ((str = TREE_STRING_POINTER (t)))
        LDV_IDENTIFIER_STR (identifier) = xstrdup (str);
      else
        LDV_CONVERT_ERROR (t);

      break;

    case ENUMERAL_TYPE:
      if ((enum_name = TYPE_NAME (t)))
        {
          if ((enum_name_str = IDENTIFIER_POINTER (enum_name)))
            LDV_IDENTIFIER_STR (identifier) = xstrdup (enum_name_str);
          else
            LDV_CONVERT_ERROR (t);
        }
      /* Always give names to enumerations to process typedefs on anonymous
         enumerations correctly. */
      else if ((enum_uid = TYPE_UID (t)))
        {
          enum_uid_str = ldv_itoa (enum_uid);
          enum_uid_name_str = XCNEWVEC (char, 3 + 1 + strlen (enum_uid_str) + 1);
          sprintf (enum_uid_name_str, "ldv_%d", enum_uid);
          XDELETE (CONST_CAST (char *, enum_uid_str));
          LDV_IDENTIFIER_STR (identifier) = enum_uid_name_str;
        }
      else
        LDV_CONVERT_ERROR (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_IDENTIFIER_STR (identifier))
    return identifier;

  LDV_ERROR ("identifier wasn't converted");

  XDELETE (identifier);

  return NULL;
}

/*
inclusive-OR-expression:
    exclusive-OR-expression
    inclusive-OR-expression | exclusive-OR-expression
*/
static ldv_inclusive_or_expr_ptr
ldv_convert_inclusive_or_expr (tree t, unsigned int recursion_limit)
{
  ldv_inclusive_or_expr_ptr inclusive_or_expr;
  tree op1, op2;

  inclusive_or_expr = XCNEW (struct ldv_inclusive_or_expr);

  switch (TREE_CODE (t))
    {
    case BIT_IOR_EXPR:
      LDV_INCLUSIVE_OR_EXPR_KIND (inclusive_or_expr) = LDV_INCLUSIVE_OR_EXPR_SECOND;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_INCLUSIVE_OR_EXPR_INCLUSIVE_OR_EXPR (inclusive_or_expr) = ldv_convert_inclusive_or_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of inclusive or expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_INCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR (inclusive_or_expr) = ldv_convert_exclusive_or_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of inclusive or expression");

      LDV_INCLUSIVE_OR_EXPR_LOCATION (inclusive_or_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_INCLUSIVE_OR_EXPR_KIND (inclusive_or_expr) = LDV_INCLUSIVE_OR_EXPR_FIRST;
      LDV_INCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR (inclusive_or_expr) = ldv_convert_exclusive_or_expr (t, recursion_limit);
    }

  if (LDV_INCLUSIVE_OR_EXPR_KIND (inclusive_or_expr))
    return inclusive_or_expr;

  LDV_ERROR ("inclusive or expression wasn't converted");

  XDELETE (inclusive_or_expr);

  return NULL;
}

/*
init-declarator:
    declarator
    declarator = initializer
*/
static ldv_init_declarator_ptr
ldv_convert_init_declarator (tree t)
{
  ldv_init_declarator_ptr init_declarator;
  tree initializer;
  bool is_typedef;

  init_declarator = XCNEW (struct ldv_init_declarator);

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      LDV_INIT_DECLARATOR_KIND (init_declarator) = LDV_INIT_DECLARATOR_FIRST;

      is_typedef = (TREE_CODE (t) == TYPE_DECL);
      LDV_INIT_DECLARATOR_DECLARATOR (init_declarator) = ldv_convert_declarator_internal (t, is_typedef);

      if (TREE_CODE (t) == VAR_DECL && (initializer = DECL_INITIAL (t)))
        {
          LDV_INIT_DECLARATOR_KIND (init_declarator) = LDV_INIT_DECLARATOR_SECOND;
          LDV_INIT_DECLARATOR_INITIALIZER (init_declarator) = ldv_convert_initializer (initializer);
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_INIT_DECLARATOR_KIND (init_declarator))
    return init_declarator;

  LDV_ERROR ("init declarator wasn't converted");

  XDELETE (init_declarator);

  return NULL;
}

/*
init-declarator-list:
    init-declarator
    init-declarator-list , init-declarator
*/
static ldv_init_declarator_list_ptr
ldv_convert_init_declarator_list (tree t)
{
  ldv_init_declarator_list_ptr init_declarator_list;

  init_declarator_list = XCNEW (struct ldv_init_declarator_list);

  /* Note that gcc doesn't keep init declarator list. It separates all init
     declarators from each other. But this function is for the C standard
     correspondence. */
  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      LDV_INIT_DECLARATOR_LIST_INIT_DECLARATOR (init_declarator_list) = ldv_convert_init_declarator (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_INIT_DECLARATOR_LIST_INIT_DECLARATOR (init_declarator_list))
    return init_declarator_list;

  LDV_ERROR ("init declarator list wasn't converted");

  XDELETE (init_declarator_list);

  return NULL;
}

/*
initializer:
    assignment-expression
    { initializer-list }
    { initializer-list , }

GNU extensions:

initializer:
    { }
*/
static ldv_initializer_ptr
ldv_convert_initializer (tree t)
{
  ldv_initializer_ptr initializer;

  initializer = XCNEW (struct ldv_initializer);

  switch (TREE_CODE (t))
    {
    case CONSTRUCTOR:
      if (CONSTRUCTOR_ELTS (t))
        {
          LDV_INITIALIZER_KIND (initializer) = LDV_INITIALIZER_SECOND;
          LDV_INITIALIZER_INITIALIZER_LIST (initializer) = ldv_convert_initializer_list (t);
        }
      else
        LDV_INITIALIZER_KIND (initializer) = LDV_INITIALIZER_FOURTH;

      break;

    default:
      LDV_INITIALIZER_KIND (initializer) = LDV_INITIALIZER_FIRST;
      LDV_INITIALIZER_ASSIGNMENT_EXPR (initializer) = ldv_convert_assignment_expr (t, LDV_CONVERT_EXPR_RECURSION_LIMIT);
    }

  if (LDV_INITIALIZER_KIND (initializer))
    return initializer;

  LDV_ERROR ("initializer wasn't converted");

  XDELETE (initializer);

  return NULL;
}

/*
initializer-list:
    designationopt initializer
    initializer-list , designationopt initializer
*/
static ldv_initializer_list_ptr
ldv_convert_initializer_list (tree t)
{
  ldv_initializer_list_ptr initializer_list, initializer_list_next, initializer_list_next_next;
  unsigned HOST_WIDE_INT ix;
  tree index, value;

  initializer_list = XCNEW (struct ldv_initializer_list);
  initializer_list_next = NULL;

  switch (TREE_CODE (t))
    {
    case CONSTRUCTOR:
      FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (t), ix, index, value)
        {
          if (!initializer_list_next)
            {
              initializer_list_next = initializer_list;
              LDV_INITIALIZER_LIST_KIND (initializer_list_next) = LDV_INITIALIZER_LIST_FIRST;
            }
          else
            {
              initializer_list_next_next = XCNEW (struct ldv_initializer_list);
              LDV_INITIALIZER_LIST_INITIALIZER_LIST (initializer_list_next_next) = initializer_list_next;
              initializer_list_next = initializer_list_next_next;
              LDV_INITIALIZER_LIST_KIND (initializer_list_next) = LDV_INITIALIZER_LIST_SECOND;
            }

          /* Handle initialization of fields without names
             (http://forge.ispras.ru/issues/5280. */
          if (TREE_CODE (index) != FIELD_DECL || DECL_NAME (index))
            LDV_INITIALIZER_LIST_DESIGNATION (initializer_list_next) = ldv_convert_designation (index);

          LDV_INITIALIZER_LIST_INITIALIZER (initializer_list_next) = ldv_convert_initializer (value);
        }


      break;

    default:
      initializer_list_next = XCNEW (struct ldv_initializer_list);
      LDV_INITIALIZER_LIST_KIND (initializer_list_next) = LDV_INITIALIZER_LIST_FIRST;
      LDV_INITIALIZER_LIST_INITIALIZER (initializer_list_next) = ldv_convert_initializer (t);
    }

  if (initializer_list_next && LDV_INITIALIZER_LIST_KIND (initializer_list_next))
    return initializer_list_next;

  LDV_ERROR ("initializer list wasn't converted");

  XDELETE (initializer_list_next);

  return NULL;
}

/*
integer-constant:
    decimal-constant integer-suffixopt
    octal-constant integer-suffixopt
    hexadecimal-constant integer-suffixopt
*/
static ldv_integer_constant_ptr
ldv_convert_integer_constant (tree t)
{
  ldv_integer_constant_ptr integer_constant;
  tree type;

  integer_constant = XCNEW (struct ldv_integer_constant);

  switch (TREE_CODE (t))
    {
    case INTEGER_CST:
      LDV_INTEGER_CONSTANT_KIND (integer_constant) = LDV_INTEGER_CONSTANT_FIRST;
      /* TODO: this is needed to debug integer constants. Delete it after all!!!
      fprintf(stderr, "!!!" HOST_WIDE_INT_PRINT_DEC "!!!\n", TREE_INT_CST_LOW (t));
      fprintf(stderr, "!!!!!!" HOST_WIDE_INT_PRINT_DEC "!!!!!!\n", TREE_INT_CST_HIGH (t));
      fprintf(stderr, "%d '%s'!!!!!!!!!\n", host_integerp (t, 0), IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (TREE_TYPE (t)))));
      */
      LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) = TREE_INT_CST_LOW (t);

      /* To see underlying builtin types that may be masked by typedef get a
         main variant of type. */
      if ((type = TREE_TYPE (t)) && (type = TYPE_MAIN_VARIANT (type)))
        {
          LDV_INTEGER_CONSTANT_ISSIGNED (integer_constant) = !TYPE_UNSIGNED (type);
          /* We decide to make unsigned char/short/int in the same wave as
             unsigned long/long long. That is convert numbers exceeding a
             maximum value of corresponding signed type to negative ones. Do it
             by default. */
          /* Stupid decision :)
          if (!getenv ("LDV_DONT_CONVERT_SMALL_INT"))
            {
              if (type == unsigned_char_type_node && LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) > CHAR_MAX)
                LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) -= UCHAR_MAX + 1L;
              else if (type == short_unsigned_type_node && LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) > SHRT_MAX)
                LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) -= USHRT_MAX + 1L;
              else if (type == unsigned_type_node && LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) > INT_MAX)
                LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant) -= UINT_MAX + 1L;
            }
          */
          /* TODO in fact checking of TREE_CODE (type) == INTEGER_TYPE is incorrect
             since here also pointers and so on are passed (see caller). See issue #1137. */
          if ((TYPE_UNSIGNED (type) && TREE_CODE (type) == INTEGER_TYPE)
            || type == long_integer_type_node || type == long_unsigned_type_node
            || type == long_long_integer_type_node || type == long_long_unsigned_type_node)
            LDV_INTEGER_CONSTANT_INTEGER_SUFFIX (integer_constant) = ldv_convert_integer_suffix (type);
        }
      else
        LDV_ERROR ("can't find integer constant type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_INTEGER_CONSTANT_KIND (integer_constant))
    return integer_constant;

  LDV_ERROR ("integer constant wasn't converted");

  XDELETE (integer_constant);

  return NULL;
}

/*
integer-suffix:
    unsigned-suffix long-suffixopt
    unsigned-suffix long-long-suffix
    long-suffix unsigned-suffixopt
    long-long-suffix unsigned-suffixopt
*/
static ldv_integer_suffix_ptr
ldv_convert_integer_suffix (tree t)
{
  ldv_integer_suffix_ptr integer_suffix;

  integer_suffix = XCNEW (struct ldv_integer_suffix);

  switch (TREE_CODE (t))
    {
    case INTEGER_TYPE:
      if (TYPE_UNSIGNED (t))
        {
          LDV_INTEGER_SUFFIX_KIND (integer_suffix) = LDV_INTEGER_SUFFIX_FIRST;
          LDV_INTEGER_SUFFIX_UNSIGNED_SUFFIX (integer_suffix) = ldv_convert_unsigned_suffix (t);

          if (t == long_unsigned_type_node)
            LDV_INTEGER_SUFFIX_LONG_SUFFIX (integer_suffix) = ldv_convert_long_suffix (t);
          else if (t == long_long_unsigned_type_node)
            {
              LDV_INTEGER_SUFFIX_KIND (integer_suffix) = LDV_INTEGER_SUFFIX_SECOND;
              LDV_INTEGER_SUFFIX_LONG_LONG_SUFFIX (integer_suffix) = ldv_convert_long_long_suffix (t);
            }
        }
      else
        {
          if (t == long_integer_type_node)
            {
              LDV_INTEGER_SUFFIX_KIND (integer_suffix) = LDV_INTEGER_SUFFIX_THIRD;
              LDV_INTEGER_SUFFIX_LONG_SUFFIX (integer_suffix) = ldv_convert_long_suffix (t);
            }
          else if (t == long_long_integer_type_node)
            {
              LDV_INTEGER_SUFFIX_KIND (integer_suffix) = LDV_INTEGER_SUFFIX_FOURTH;
              LDV_INTEGER_SUFFIX_LONG_LONG_SUFFIX (integer_suffix) = ldv_convert_long_long_suffix (t);
            }
          else
            LDV_ERROR ("something strange");
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_INTEGER_SUFFIX_KIND (integer_suffix))
    return integer_suffix;

  LDV_ERROR ("integer suffix wasn't converted");

  XDELETE (integer_suffix);

  return NULL;
}

/*
jump-statement:
    goto identifier ;
    continue ;
    break ;
    return expressionopt ;
*/
static ldv_jump_statement_ptr
ldv_convert_jump_statement (tree t)
{
  ldv_jump_statement_ptr jump_statement;
  tree goto_dest, ret_val_expr, ret_val;

  jump_statement = XCNEW (struct ldv_jump_statement);

  switch (TREE_CODE (t))
    {
    case GOTO_EXPR:
      LDV_JUMP_STATEMENT_KIND (jump_statement) = LDV_JUMP_STATEMENT_GOTO;

      if ((goto_dest = GOTO_DESTINATION (t)))
        {
          switch (TREE_CODE (goto_dest))
            {
            case LABEL_DECL:
              LDV_JUMP_STATEMENT_ID (jump_statement) = ldv_label_decl_name (goto_dest);
              break;

            default:
              LDV_CONVERT_ERROR (t);
            }
        }
      else
        LDV_ERROR ("can't find goto destination");

      LDV_JUMP_STATEMENT_LOCATION (jump_statement) = ldv_convert_location (t);

      break;

    case RETURN_EXPR:
      LDV_JUMP_STATEMENT_KIND (jump_statement) = LDV_JUMP_STATEMENT_RETURN;

      if ((ret_val_expr = LDV_OP_FIRST (t)))
        {
          /* Skip artificial RESULT_DECL introduce for non-void functions. */
          if (TREE_CODE (ret_val_expr) == MODIFY_EXPR)
            {
              if ((ret_val = LDV_OP_SECOND (ret_val_expr)))
                LDV_JUMP_STATEMENT_EXPR (jump_statement) = ldv_convert_expr (ret_val, LDV_CONVERT_EXPR_RECURSION_LIMIT);
              else
                LDV_ERROR ("can't find return value");
            }
          else
              LDV_JUMP_STATEMENT_EXPR (jump_statement) = ldv_convert_expr (ret_val_expr, LDV_CONVERT_EXPR_RECURSION_LIMIT);
        }

      LDV_JUMP_STATEMENT_LOCATION (jump_statement) = ldv_convert_location (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_JUMP_STATEMENT_KIND (jump_statement))
    return jump_statement;

  LDV_ERROR ("jump statement wasn't converted");

  XDELETE (jump_statement);

  return NULL;
}

/*
GNU extensions:

label-declaration:
    __label__ identifier-list ;
*/
static ldv_label_decl_ptr
ldv_convert_label_decl (tree t)
{
  ldv_label_decl_ptr label_decl;

  label_decl = XCNEW (struct ldv_label_decl);

  switch (TREE_CODE (t))
    {
    case LABEL_DECL:
      LDV_LABEL_DECL_IDENTIFIER (label_decl) = ldv_convert_identifier (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_LABEL_DECL_IDENTIFIER (label_decl))
    return label_decl;

  LDV_ERROR ("label declaration wasn't converted");

  XDELETE (label_decl);

  return NULL;
}

/*
labeled-statement:
    identifier : statement
    case constant-expression : statement
    default : statement

GNU extensions:

labeled-statement:
    case constant-expression ... constant-expression : statement
*/
static ldv_labeled_statement_ptr
ldv_convert_labeled_statement (tree t)
{
  ldv_labeled_statement_ptr labeled_statement;
  tree label_decl;
  tree const_expr;

  labeled_statement = XCNEW (struct ldv_labeled_statement);

  switch (TREE_CODE (t))
    {
    case LABEL_EXPR:
      LDV_LABELED_STATEMENT_KIND (labeled_statement) = LDV_LABELED_STATEMENT_LABEL;

      if ((label_decl = LABEL_EXPR_LABEL (t)))
        LDV_LABELED_STATEMENT_ID (labeled_statement) = ldv_label_decl_name (label_decl);
      else
        LDV_ERROR ("can't find label declaration");

      LDV_LABELED_STATEMENT_LOCATION (labeled_statement) = ldv_convert_location (t);

      break;

    case CASE_LABEL_EXPR:
      /* Handle various cases like suggested in comments to function
         c_add_case_label() in c-common.c. */
      if (!CASE_LOW (t) && !CASE_HIGH (t))
        LDV_LABELED_STATEMENT_KIND (labeled_statement) = LDV_LABELED_STATEMENT_DEFAULT;
      else
        {
          LDV_LABELED_STATEMENT_KIND (labeled_statement) = LDV_LABELED_STATEMENT_CASE;

          if ((const_expr = CASE_LOW (t)))
            LDV_LABELED_STATEMENT_CONST_EXPR1 (labeled_statement) = (int) TREE_INT_CST_LOW (const_expr);
          else
            LDV_ERROR ("can't find case constant");

          if ((const_expr = CASE_HIGH (t)))
            {
              LDV_LABELED_STATEMENT_KIND (labeled_statement) = LDV_LABELED_STATEMENT_CASE_RANGE;
              LDV_LABELED_STATEMENT_CONST_EXPR2 (labeled_statement) = (int) TREE_INT_CST_LOW (const_expr);
            }
        }

      LDV_LABELED_STATEMENT_LOCATION (labeled_statement) = ldv_convert_location (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  /* Gcc doesn't relate any statement with a given label. So generate empty
     expression statement related with a given label that does nothing to
     prevent printer warnings on statement absence. */
  LDV_LABELED_STATEMENT_STATEMENT (labeled_statement) = ldv_create_null_statement ();

  if (LDV_LABELED_STATEMENT_KIND (labeled_statement))
    return labeled_statement;

  LDV_ERROR ("labeled statement wasn't converted");

  XDELETE (labeled_statement);

  return NULL;
}

static ldv_location_ptr
ldv_convert_location (tree t)
{
  ldv_location_ptr location;
  const char *file;
  unsigned int line;

  location = XCNEW (struct ldv_location);

  if (DECL_P (t))
    {
      if ((file = DECL_SOURCE_FILE (t)))
        LDV_LOCATION_FILE (location) = ldv_get_realpath (file);
      else
        LDV_ERROR ("can't find location file for declaration");

      if ((line = DECL_SOURCE_LINE (t)))
        LDV_LOCATION_LINE (location) = line;
      else
        LDV_ERROR ("can't find location line for declaration");
    }
  else if (EXPR_P (t))
    {
      /* A lot of expressions haven't location. */
      if (!EXPR_HAS_LOCATION (t))
        {
          XDELETE (location);
          return NULL;
        }

      if ((file = EXPR_FILENAME (t)))
        LDV_LOCATION_FILE (location) = ldv_get_realpath (file);
      else
        LDV_ERROR ("can't find location file for expression");

      LDV_LOCATION_LINE (location) = EXPR_LINENO (t);
    }
  else
    LDV_CONVERT_ERROR (t);

  if (LDV_LOCATION_FILE (location) || LDV_LOCATION_LINE (location))
    return location;

  LDV_ERROR ("location wasn't converted");

  XDELETE (location);

  return NULL;
}

/*
logical-AND-expression:
    inclusive-OR-expression
    logical-AND-expression && inclusive-OR-expression
*/
static ldv_logical_and_expr_ptr
ldv_convert_logical_and_expr (tree t, unsigned int recursion_limit)
{
  ldv_logical_and_expr_ptr logical_and_expr;
  tree op1, op2;

  logical_and_expr = XCNEW (struct ldv_logical_and_expr);

  switch (TREE_CODE (t))
    {
    case TRUTH_ANDIF_EXPR:
    case TRUTH_AND_EXPR:
      LDV_LOGICAL_AND_EXPR_KIND (logical_and_expr) = LDV_LOGICAL_AND_EXPR_SECOND;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_LOGICAL_AND_EXPR_LOGICAL_AND_EXPR (logical_and_expr) = ldv_convert_logical_and_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of logical and expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_LOGICAL_AND_EXPR_INCLUSIVE_OR_EXPR (logical_and_expr) = ldv_convert_inclusive_or_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of logical and expression");

      LDV_LOGICAL_AND_EXPR_LOCATION (logical_and_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_LOGICAL_AND_EXPR_KIND (logical_and_expr) = LDV_LOGICAL_AND_EXPR_FIRST;
      LDV_LOGICAL_AND_EXPR_INCLUSIVE_OR_EXPR (logical_and_expr) = ldv_convert_inclusive_or_expr (t, recursion_limit);
    }

  if (LDV_LOGICAL_AND_EXPR_KIND (logical_and_expr))
    return logical_and_expr;

  LDV_ERROR ("logical and expression wasn't converted");

  XDELETE (logical_and_expr);

  return NULL;
}

/*
logical-OR-expression:
    logical-AND-expression
    logical-OR-expression || logical-AND-expression
*/
static ldv_logical_or_expr_ptr
ldv_convert_logical_or_expr (tree t, unsigned int recursion_limit)
{
  ldv_logical_or_expr_ptr logical_or_expr;
  tree op1, op2;

  logical_or_expr = XCNEW (struct ldv_logical_or_expr);

  switch (TREE_CODE (t))
    {
    case TRUTH_ORIF_EXPR:
    case TRUTH_OR_EXPR:
      LDV_LOGICAL_OR_EXPR_KIND (logical_or_expr) = LDV_LOGICAL_OR_EXPR_SECOND;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_LOGICAL_OR_EXPR_LOGICAL_OR_EXPR (logical_or_expr) = ldv_convert_logical_or_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of logical or expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_LOGICAL_OR_EXPR_LOGICAL_AND_EXPR (logical_or_expr) = ldv_convert_logical_and_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of logical or expression");

      LDV_LOGICAL_OR_EXPR_LOCATION (logical_or_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_LOGICAL_OR_EXPR_KIND (logical_or_expr) = LDV_LOGICAL_OR_EXPR_FIRST;
      LDV_LOGICAL_OR_EXPR_LOGICAL_AND_EXPR (logical_or_expr) = ldv_convert_logical_and_expr (t, recursion_limit);
    }

  if (LDV_LOGICAL_OR_EXPR_KIND (logical_or_expr))
    return logical_or_expr;

  LDV_ERROR ("logical or expression wasn't converted");

  XDELETE (logical_or_expr);

  return NULL;
}

/*
long-long-suffix: one of
    ll LL
*/
static ldv_long_long_suffix_ptr
ldv_convert_long_long_suffix (tree t)
{
  ldv_long_long_suffix_ptr long_long_suffix;

  long_long_suffix = XCNEW (struct ldv_long_long_suffix);

  switch (TREE_CODE (t))
    {
    case INTEGER_TYPE:
      if (t == long_long_integer_type_node || t == long_long_unsigned_type_node)
        LDV_LONG_LONG_SUFFIX_KIND (long_long_suffix) = LDV_LONG_LONG_SUFFIX_SECOND;
      else
        LDV_ERROR ("try to get long long suffix for not long long type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_LONG_LONG_SUFFIX_KIND (long_long_suffix))
    return long_long_suffix;

  LDV_ERROR ("long long suffix wasn't converted");

  XDELETE (long_long_suffix);

  return NULL;
}

/*
long-suffix: one of
    l L
*/
static ldv_long_suffix_ptr
ldv_convert_long_suffix (tree t)
{
  ldv_long_suffix_ptr long_suffix;

  long_suffix = XCNEW (struct ldv_long_suffix);

  switch (TREE_CODE (t))
    {
    case INTEGER_TYPE:
      if (t == long_integer_type_node || t == long_unsigned_type_node)
        LDV_LONG_SUFFIX_KIND (long_suffix) = LDV_LONG_SUFFIX_SECOND;
      else
        LDV_ERROR ("try to get long suffix for not long type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_LONG_SUFFIX_KIND (long_suffix))
    return long_suffix;

  LDV_ERROR ("long suffix wasn't converted");

  XDELETE (long_suffix);

  return NULL;
}

/*
multiplicative-expression:
    cast-expression
    multiplicative-expression * cast-expression
    multiplicative-expression / cast-expression
    multiplicative-expression % cast-expression
*/
static ldv_multiplicative_expr_ptr
ldv_convert_multiplicative_expr (tree t, unsigned int recursion_limit)
{
  ldv_multiplicative_expr_ptr multiplicative_expr;
  tree op1, op2;

  multiplicative_expr = XCNEW (struct ldv_multiplicative_expr);

  switch (TREE_CODE (t))
    {
    case MULT_EXPR:
    case TRUNC_DIV_EXPR:
    case EXACT_DIV_EXPR:
    case RDIV_EXPR:
    case TRUNC_MOD_EXPR:
      if (TREE_CODE (t) == MULT_EXPR)
        LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr) = LDV_MULTIPLICATIVE_EXPR_SECOND;
      else if (TREE_CODE (t) == TRUNC_DIV_EXPR || TREE_CODE (t) == EXACT_DIV_EXPR || TREE_CODE (t) == RDIV_EXPR)
        LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr) = LDV_MULTIPLICATIVE_EXPR_THIRD;
      else
        LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr) = LDV_MULTIPLICATIVE_EXPR_FOURTH;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_MULTIPLICATIVE_EXPR_MULTIPLICATIVE_EXPR (multiplicative_expr) = ldv_convert_multiplicative_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of multiplicative expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_MULTIPLICATIVE_EXPR_CAST_EXPR (multiplicative_expr) = ldv_convert_cast_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of multiplicative expression");

      LDV_MULTIPLICATIVE_EXPR_LOCATION (multiplicative_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr) = LDV_MULTIPLICATIVE_EXPR_FIRST;
      LDV_MULTIPLICATIVE_EXPR_CAST_EXPR (multiplicative_expr) = ldv_convert_cast_expr (t, recursion_limit);
    }

  if (LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr))
    return multiplicative_expr;

  LDV_ERROR ("multiplicative expression wasn't converted");

  XDELETE (multiplicative_expr);

  return NULL;
}

/*
GNU extensions:

nested-declaration:
    declaration
    nested-function-definition
*/
static ldv_nested_decl_ptr
ldv_convert_nested_decl (tree t)
{
  ldv_nested_decl_ptr nested_decl;

  nested_decl = XCNEW (struct ldv_nested_decl);

  switch (TREE_CODE (t))
    {
    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case TYPE_DECL:
    case VAR_DECL:
      LDV_NESTED_DECL_KIND (nested_decl) = LDV_NESTED_DECL_FIRST;
      LDV_NESTED_DECL_DECL (nested_decl) = ldv_convert_decl (t);

      break;

    case FUNCTION_DECL:
      if (DECL_SAVED_TREE (t))
        {
          LDV_NESTED_DECL_KIND (nested_decl) = LDV_NESTED_DECL_SECOND;
          LDV_NESTED_DECL_NESTED_FUNC_DEF (nested_decl) = ldv_convert_nested_func_def (t);
        }
      else
        {
          LDV_NESTED_DECL_KIND (nested_decl) = LDV_NESTED_DECL_FIRST;
          LDV_NESTED_DECL_DECL (nested_decl) = ldv_convert_decl (t);
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_NESTED_DECL_KIND (nested_decl))
    return nested_decl;

  LDV_ERROR ("nested declaration wasn't converted");

  XDELETE (nested_decl);

  return NULL;
}

/* Treated in the same way as function-definition.
GNU extensions:

nested-function-definition:
    declaration-specifiers declarator declaration-listopt compound-statement
*/
static ldv_nested_func_def_ptr
ldv_convert_nested_func_def (tree t)
{
  ldv_nested_func_def_ptr nested_func_def;

  nested_func_def = XCNEW (struct ldv_nested_func_def);

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
      LDV_NESTED_FUNC_DEF_FUNC_DEF (nested_func_def) = ldv_convert_func_def (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_NESTED_FUNC_DEF_FUNC_DEF (nested_func_def))
    return nested_func_def;

  LDV_ERROR ("nested function definition wasn't converted");

  XDELETE (nested_func_def);

  return NULL;
}

static bool
ldv_replace_va_list_tag (ldv_type_spec_ptr type_spec)
{
  ldv_typedef_name_ptr typedef_name;

  if (type_spec && LDV_TYPE_SPEC_KIND (type_spec) == LDV_TYPE_SPEC_THIRD)
    {
      typedef_name = LDV_TYPE_SPEC_TYPEDEF_NAME (type_spec);
      if (typedef_name && !strcmp (LDV_IDENTIFIER_STR (LDV_TYPEDEF_NAME_IDENTIFIER (typedef_name)), "__va_list_tag"))
        {
          LDV_IDENTIFIER_STR (LDV_TYPEDEF_NAME_IDENTIFIER (typedef_name)) = xstrdup ("va_list");
          return true;
        }
    }

  return false;
}

/*
parameter-declaration:
    declaration-specifiers declarator
    declaration-specifiers abstract-declaratoropt
*/
static ldv_param_decl_ptr
ldv_convert_param_decl (tree t)
{
  ldv_param_decl_ptr param_decl;
  tree param_type;

  param_decl = XCNEW (struct ldv_param_decl);

  switch (TREE_CODE (t))
    {
    case PARM_DECL:
      LDV_PARAM_DECL_DECLARATOR (param_decl) = ldv_convert_declarator_internal (t, false);
      LDV_PARAM_DECL_DECL_SPEC (param_decl) = ldv_convert_decl_spec (t, false);

      /* Replace "__va_list_tag *param_name" with "va_list param_name". */
      if (ldv_replace_va_list_tag (LDV_DECL_SPEC_TYPE_SPEC (LDV_PARAM_DECL_DECL_SPEC (param_decl))))
        (LDV_PARAM_DECL_DECLARATOR (param_decl))->pointer = NULL;

      break;

    case TREE_LIST:
      if (!(param_type = TREE_VALUE (t)))
        {
          LDV_ERROR ("can't find function parameter type");
          break;
        }

      LDV_PARAM_DECL_DECL_SPEC (param_decl) = ldv_convert_decl_spec (param_type, false);

      /* Replace "__va_list_tag *" with "va_list". */
      if (!ldv_replace_va_list_tag (LDV_DECL_SPEC_TYPE_SPEC (LDV_PARAM_DECL_DECL_SPEC (param_decl))))
        LDV_PARAM_DECL_ABSTRACT_DECLARATOR (param_decl) = ldv_convert_abstract_declarator_internal (param_type);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_PARAM_DECL_DECL_SPEC (param_decl))
    return param_decl;

  LDV_ERROR ("parameter declaration wasn't converted");

  XDELETE (param_decl);

  return NULL;
}

/*
parameter-list:
    parameter-declaration
    parameter-list , parameter-declaration
*/
static ldv_param_list_ptr
ldv_convert_param_list (tree t)
{
  ldv_param_list_ptr param_list, param_list_next, param_list_next_next;
  tree param_next;

  param_list = XCNEW (struct ldv_param_list);
  param_list_next = NULL;

  if (t)
    {
      switch (TREE_CODE (t))
        {
        case PARM_DECL:
        case TREE_LIST:
          if ((param_next = TREE_CHAIN (t)))
            {
              /* Skip artificial void type terminated parameter type list. */
              if (!(TREE_CODE (t) == TREE_LIST && !TREE_CHAIN (param_next) && TREE_VALUE (param_next) == void_type_node))
                {
                  if ((param_list_next = ldv_convert_param_list (param_next)))
                    {
                      for (param_list_next_next = param_list_next; LDV_PARAM_LIST_PARAM_LIST (param_list_next_next); param_list_next_next = LDV_PARAM_LIST_PARAM_LIST (param_list_next_next)) ;
                      LDV_PARAM_LIST_PARAM_LIST (param_list_next_next) = param_list;
                    }
                  else
                    LDV_ERROR ("can't process following parameter list");
                }
            }
          LDV_PARAM_LIST_PARAM_DECL (param_list) = ldv_convert_param_decl (t);
          break;

        default:
          LDV_CONVERT_ERROR (t);
        }
    }
  /* When there is no parameters at all generate artificial void type to
     correspond to the C grammar. */
  else
    LDV_PARAM_LIST_PARAM_DECL (param_list) = ldv_create_void_param_decl ();

  if (param_list_next)
    return param_list_next;

  if (LDV_PARAM_LIST_PARAM_DECL (param_list) || LDV_PARAM_LIST_PARAM_LIST (param_list))
    return param_list;

  LDV_ERROR ("parameter list wasn't converted");

  XDELETE (param_list);

  return NULL;
}

/*
parameter-type-list:
    parameter-list
    parameter-list , ...
*/
static ldv_param_type_list_ptr
ldv_convert_param_type_list (tree t)
{
  ldv_param_type_list_ptr param_type_list;
  tree func_type;
  tree func_param_decl, func_param_type, func_param_type_list;

  param_type_list = XCNEW (struct ldv_param_type_list);

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
      if (!(func_type = TREE_TYPE (t)))
        LDV_ERROR ("can't find function type");

      break;

    case FUNCTION_TYPE:
      func_type = t;

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (func_type)
    /* A given function has variable number of arguments if the end of a function
       parameter type list doesn't terminated by artificial void type in case when
       there is at least one argument at all. */
    for (func_param_type_list = TYPE_ARG_TYPES (func_type); func_param_type_list; func_param_type_list = TREE_CHAIN (func_param_type_list))
      if ((func_param_type = TREE_VALUE (func_param_type_list)))
        if (!TREE_CHAIN (func_param_type_list) && func_param_type != void_type_node)
          LDV_PARAM_TYPE_LIST_IS_VA (param_type_list) = true;

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
      func_param_decl = DECL_ARGUMENTS (t);

      LDV_PARAM_TYPE_LIST_PARAM_LIST (param_type_list) = ldv_convert_param_list (func_param_decl);

      break;

    case FUNCTION_TYPE:
      if ((func_param_type_list = TYPE_ARG_TYPES (t)))
        LDV_PARAM_TYPE_LIST_PARAM_LIST (param_type_list) = ldv_convert_param_list (func_param_type_list);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_PARAM_TYPE_LIST_PARAM_LIST (param_type_list))
    return param_type_list;

  XDELETE (param_type_list);

  return NULL;
}

/*
pointer:
    * type-qualifier-listopt
    * type-qualifier-listopt pointer
*/
static ldv_pointer_ptr
ldv_convert_pointer (tree t, tree *first_non_pointer_ptr)
{
  ldv_pointer_ptr pointer, pointer_next, pointer_last;
  tree type, type_name;

  /* Note that internal representation keeps pointers in backward order. */
  pointer = pointer_last = XCNEW (struct ldv_pointer);

  LDV_POINTER_TYPE_QUAL_LIST (pointer_last) = ldv_convert_type_qual_list (t);

  type = TREE_TYPE (t);

  /* Here is typedef name is also processed. It depends on whether type has
     a name or not. */
  if (((type_name = TYPE_NAME (type)) && TREE_CODE(type_name) == TYPE_DECL) || TREE_CODE (type) != POINTER_TYPE)
    *first_non_pointer_ptr = type;
  else
    {
      pointer = ldv_convert_pointer (type, first_non_pointer_ptr);
      for (pointer_next = pointer; LDV_POINTER_POINTER (pointer_next); pointer_next = LDV_POINTER_POINTER (pointer_next)) ;
      LDV_POINTER_POINTER (pointer_next) = pointer_last;
    }

  return pointer;
}

/*
postfix-expression:
    primary-expression
    postfix-expression [ expression ]
    postfix-expression ( argument-expression-listopt )
    postfix-expression . identifier
    postfix-expression -> identifier
    postfix-expression ++
    postfix-expression --
    ( type-name ) { initializer-list }
    ( type-name ) { initializer-list , }

GNU extensions (not mentioned in c-parser.c):

postfix-expression:
    ( type-name ) { }
*/
static ldv_postfix_expr_ptr
ldv_convert_postfix_expr (tree t, unsigned int recursion_limit)
{
  ldv_postfix_expr_ptr postfix_expr;
  tree op1, op2;
  tree func_addr, func;
  call_expr_arg_iterator func_arg_iterator;
  tree expr_decl, expr_decl_type, expr_decl_initial;
  unsigned HOST_WIDE_INT ix;
  tree index, value;

  postfix_expr = XCNEW (struct ldv_postfix_expr);

  switch (TREE_CODE (t))
    {
    case ARRAY_REF:
    case COMPONENT_REF:
    case POSTINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
    case COMPOUND_LITERAL_EXPR:
      if (TREE_CODE (t) == ARRAY_REF)
        LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_SECOND;
      else if (TREE_CODE (t) == COMPONENT_REF)
        {
          /* Handle accesses to fields without names
             (http://forge.ispras.ru/issues/5280. */
          if ((op1 = LDV_OP_FIRST (t)) && (op2 = LDV_OP_SECOND (t)))
            {
              if (!DECL_NAME (op2))
                {
                  LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_FIRST;
                  LDV_POSTFIX_EXPR_PRIMARY_EXPR (postfix_expr) = ldv_convert_primary_expr (op1, recursion_limit);
                  break;
                }
            }
          else
            LDV_ERROR ("can't find the first or the second operand of postfix expression");

          LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_FOURTH;
        }
      else if (TREE_CODE (t) == POSTINCREMENT_EXPR)
        LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_SIXTH;
      else if (TREE_CODE (t) == POSTDECREMENT_EXPR)
        LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_SEVENTH;
      else
        LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_EIGHTH;

      if (TREE_CODE (t) != COMPOUND_LITERAL_EXPR)
        {
          if ((op1 = LDV_OP_FIRST (t)))
            LDV_POSTFIX_EXPR_POSTFIX_EXPR (postfix_expr) = ldv_convert_postfix_expr (op1, recursion_limit);
          else
            LDV_ERROR ("can't find the first operand of postfix expression");
        }

      if (TREE_CODE (t) == ARRAY_REF)
        {
          if ((op2 = LDV_OP_SECOND (t)))
            LDV_POSTFIX_EXPR_EXPR (postfix_expr) = ldv_convert_expr (op2, recursion_limit);
          else
            LDV_ERROR ("can't find the second operand of postfix expression");
        }
      else if (TREE_CODE (t) == COMPONENT_REF)
        {
          if ((op2 = LDV_OP_SECOND (t)))
            LDV_POSTFIX_EXPR_IDENTIFIER (postfix_expr) = ldv_convert_identifier (op2);
          else
            LDV_ERROR ("can't find the second operand of postfix expression");
        }
      else if (TREE_CODE (t) == COMPOUND_LITERAL_EXPR)
        {
          if ((expr_decl = COMPOUND_LITERAL_EXPR_DECL (t)))
            {
              if ((expr_decl_type = TREE_TYPE (expr_decl)))
                LDV_POSTFIX_EXPR_TYPE_NAME (postfix_expr) = ldv_convert_type_name (expr_decl_type);
              else
                LDV_ERROR ("can't find initialization of expression declaration");

              if ((expr_decl_initial = DECL_INITIAL (expr_decl)))
                {
                  if (CONSTRUCTOR_ELTS (expr_decl_initial))
                    LDV_POSTFIX_EXPR_INITIALIZER_LIST (postfix_expr) = ldv_convert_initializer_list (expr_decl_initial);
                  else
                    LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_NINETH;
                }
              else
                LDV_ERROR ("can't find initialization of expression declaration");
            }
          else
            LDV_ERROR ("can't find expression declaration of compound literal");
        }

      LDV_POSTFIX_EXPR_LOCATION (postfix_expr) = ldv_convert_location (t);

      break;

    case CALL_EXPR:
      LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_THIRD;

      if ((func_addr = CALL_EXPR_FN (t)))
        {
          /* Here is calls peformed through the function pointers are also
             processed. Functions themselves are called through their addresses,
             so "dereference" them. */
          if (TREE_CODE(func_addr) == ADDR_EXPR)
            {
              if ((func = LDV_OP_FIRST (func_addr)))
                LDV_POSTFIX_EXPR_POSTFIX_EXPR (postfix_expr) = ldv_convert_postfix_expr (func, recursion_limit);
              else
                LDV_ERROR ("can't find the called function of postfix expression");
            }
          else
            LDV_POSTFIX_EXPR_POSTFIX_EXPR (postfix_expr) = ldv_convert_postfix_expr (func_addr, recursion_limit);
        }
      else
        LDV_ERROR ("can't find the called function adrress of postfix expression");

      if (first_call_expr_arg(t, &func_arg_iterator))
        LDV_POSTFIX_EXPR_ARG_EXPR_LIST (postfix_expr) = ldv_convert_arg_expr_list (t, recursion_limit);

      LDV_POSTFIX_EXPR_LOCATION (postfix_expr) = ldv_convert_location (t);

      break;

    case CONSTRUCTOR:
      /* For transparent unions there is constructor of one element referencing particular fields of those unions
         and actual function arguments casted to these unions from normal types. */
      if (CONSTRUCTOR_ELTS (t) && CONSTRUCTOR_NELTS (t) == 1)
        FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (t), ix, index, value)
          if (TREE_CODE (index) == FIELD_DECL && TYPE_TRANSPARENT_AGGR (DECL_FIELD_CONTEXT (index)))
            return ldv_convert_postfix_expr (value, recursion_limit);

      break;

    default:
      LDV_POSTFIX_EXPR_KIND (postfix_expr) = LDV_POSTFIX_EXPR_FIRST;
      LDV_POSTFIX_EXPR_PRIMARY_EXPR (postfix_expr) = ldv_convert_primary_expr (t, recursion_limit);
    }

  if (LDV_POSTFIX_EXPR_KIND (postfix_expr))
    return postfix_expr;

  LDV_ERROR ("postfix expression wasn't converted");

  XDELETE (postfix_expr);

  return NULL;
}

/*
primary-expression:
    identifier
    constant
    string-literal
    ( expression )

GNU extensions:

primary-expression:
    ( compound-statement )
    __builtin_va_arg ( assignment-expression , type-name )
*/
static ldv_primary_expr_ptr
ldv_convert_primary_expr (tree t, unsigned int recursion_limit  )
{
  ldv_primary_expr_ptr primary_expr;
  tree initial;
  tree op1, type;
  tree copy, cast;

  primary_expr = XCNEW (struct ldv_primary_expr);

  switch (TREE_CODE (t))
    {
    case FUNCTION_DECL:
    case PARM_DECL:
    case VAR_DECL:
    case LABEL_DECL:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_FIRST;
      LDV_PRIMARY_EXPR_IDENTIFIER (primary_expr) = ldv_convert_identifier (t);

      break;

    case INTEGER_CST:
    case REAL_CST:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_SECOND;

      if ((type = TREE_TYPE (t)))
        {
          switch (TREE_CODE (type))
            {
            /* Build artificial conversion for "pointer" constants to avoid
               int-to-pointer conversion warning. As well it is required to
               resolve #4911 issue. */
            case POINTER_TYPE:
              /* Copy constant to replace its pointer type with pointed integer
                 type. */
              if (!(copy = copy_node (t)))
                {
                  LDV_ERROR ("can't copy constant");
                  break;
                }

              /* Replace pointer type by pointed non pointer type to avoid
                 infinite recursion in further conversions that will pass here
                 again. */
              do
                {
                  TREE_TYPE (copy) = TREE_TYPE (TREE_TYPE (copy));
                }
              while (POINTER_TYPE_P (TREE_TYPE (copy)));

              if (!(cast = build1 (CONVERT_EXPR, type, copy)))
                {
                  LDV_ERROR ("can't build cast for constant");
                  break;
                }

              LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_FOURTH;
              LDV_PRIMARY_EXPR_EXPR (primary_expr) = ldv_convert_expr (cast, recursion_limit - 1);

              break;

            default:
              ; /* TODO: anything else? No additional warnings observed... */
            }
        }
      else
        LDV_ERROR ("can't find constant type");

      if ((LDV_PRIMARY_EXPR_KIND (primary_expr) == LDV_PRIMARY_EXPR_SECOND))
        ldv_constant_current = LDV_PRIMARY_EXPR_CONSTANT (primary_expr) = ldv_convert_constant (t);

      break;

    case STRING_CST:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_THIRD;
      LDV_PRIMARY_EXPR_STR_LITERAL (primary_expr) = ldv_convert_str_literal (t);

      break;

    case TARGET_EXPR:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_FIFTH;

      if ((initial = TARGET_EXPR_INITIAL (t)))
        LDV_PRIMARY_EXPR_COMPOUND_STATEMENT (primary_expr) = ldv_convert_compound_statement (initial);
      else
        LDV_ERROR ("can't find compound statement of primary expression");

      break;

    case ASM_EXPR:
    case BIND_EXPR:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_FIFTH;
      LDV_PRIMARY_EXPR_COMPOUND_STATEMENT (primary_expr) = ldv_convert_compound_statement (t);

      break;

    case VA_ARG_EXPR:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_SIXTH;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_PRIMARY_EXPR_ASSIGNMENT_EXPR (primary_expr) = ldv_convert_assignment_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of variable argument expression");

      if ((type = TREE_TYPE (t)))
        LDV_PRIMARY_EXPR_TYPE_NAME (primary_expr) = ldv_convert_type_name (type);
      else
        LDV_ERROR ("can't find type of variable argument expression");

      break;

    /* Skip this artificial expression. */
    case SAVE_EXPR:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_FOURTH;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_PRIMARY_EXPR_EXPR (primary_expr) = ldv_convert_expr (op1, recursion_limit - 1);
      else
        LDV_ERROR ("can't find the first operand of save expression");

      break;

    default:
      LDV_PRIMARY_EXPR_KIND (primary_expr) = LDV_PRIMARY_EXPR_FOURTH;

      /* Check whether recursion limit is reached. This is neede to avoid
         infinite recursion. */
      if (recursion_limit > 0)
        LDV_PRIMARY_EXPR_EXPR (primary_expr) = ldv_convert_expr (t, recursion_limit - 1);
      else
        {
          LDV_ERROR ("expression recursion limit is reached");
          LDV_CONVERT_ERROR (t);
        }
    }

  if (LDV_PRIMARY_EXPR_KIND (primary_expr))
    return primary_expr;

  LDV_ERROR ("primary expression wasn't converted");

  XDELETE (primary_expr);

  return NULL;
}

/*
relational-expression:
    shift-expression
    relational-expression < shift-expression
    relational-expression > shift-expression
    relational-expression <= shift-expression
    relational-expression >= shift-expression
*/
static ldv_relational_expr_ptr
ldv_convert_relational_expr (tree t, unsigned int recursion_limit)
{
  ldv_relational_expr_ptr relational_expr;
  tree op1, op2;

  relational_expr = XCNEW (struct ldv_relational_expr);

  switch (TREE_CODE (t))
    {
    case LT_EXPR:
    case GT_EXPR:
    case LE_EXPR:
    case GE_EXPR:
      if (TREE_CODE (t) == LT_EXPR)
        LDV_RELATIONAL_EXPR_KIND (relational_expr) = LDV_RELATIONAL_EXPR_SECOND;
      else if (TREE_CODE (t) == GT_EXPR)
        LDV_RELATIONAL_EXPR_KIND (relational_expr) = LDV_RELATIONAL_EXPR_THIRD;
      else if (TREE_CODE (t) == LE_EXPR)
        LDV_RELATIONAL_EXPR_KIND (relational_expr) = LDV_RELATIONAL_EXPR_FOURTH;
      else
        LDV_RELATIONAL_EXPR_KIND (relational_expr) = LDV_RELATIONAL_EXPR_FIFTH;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_RELATIONAL_EXPR_RELATIONAL_EXPR (relational_expr) = ldv_convert_relational_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of relational expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_RELATIONAL_EXPR_SHIFT_EXPR (relational_expr) = ldv_convert_shift_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of relational expression");

      LDV_RELATIONAL_EXPR_LOCATION (relational_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_RELATIONAL_EXPR_KIND (relational_expr) = LDV_RELATIONAL_EXPR_FIRST;
      LDV_RELATIONAL_EXPR_SHIFT_EXPR (relational_expr) = ldv_convert_shift_expr (t, recursion_limit);
    }

  if (LDV_RELATIONAL_EXPR_KIND (relational_expr))
    return relational_expr;

  LDV_ERROR ("relational expression wasn't converted");

  XDELETE (relational_expr);

  return NULL;
}

/*
selection-statement:
    if ( expression ) statement
    if ( expression ) statement else statement
    switch ( expression ) statement
*/
static ldv_selection_statement_ptr
ldv_convert_selection_statement (tree t)
{
  ldv_selection_statement_ptr selection_statement;
  ldv_statement_ptr then_statement = NULL, then_statement_aux;
  ldv_compound_statement_ptr compound_statement;
  ldv_block_item_list_ptr block_item_list;
  ldv_block_item_ptr block_item;
  tree cond_expr, then_statement_tree, else_statement;
  tree switch_statement;

  selection_statement = XCNEW (struct ldv_selection_statement);

  switch (TREE_CODE (t))
    {
    case COND_EXPR:
      if ((cond_expr = COND_EXPR_COND (t)))
        LDV_SELECTION_STATEMENT_EXPR (selection_statement) = ldv_convert_expr (cond_expr, LDV_CONVERT_EXPR_RECURSION_LIMIT);
      else
        LDV_ERROR ("can't find conditional expression");

      if ((then_statement_tree = COND_EXPR_THEN (t)))
        LDV_SELECTION_STATEMENT_STATEMENT1 (selection_statement) = then_statement = ldv_convert_statement (then_statement_tree);
      else
        LDV_ERROR ("can't find then statement");

      if ((else_statement = COND_EXPR_ELSE (t)))
        {
          LDV_SELECTION_STATEMENT_KIND (selection_statement) = LDV_SELECTION_STATEMENT_IF_THEN_ELSE;
          LDV_SELECTION_STATEMENT_STATEMENT2 (selection_statement) = ldv_convert_statement (else_statement);

          /* We need to pay attention to so called dangling else (#3707).
             This becomes a problem when a first statement of a processed
             selection statement isn't a compound statement (that is enclosed in
             braces) and there is else and a second statement. This looks like
             gcc optimisation that works well with internal representation but
             leads to mistakes during source code printing. So use "artificial"
             compound statement instead of the first statement for this case. */
          if (then_statement && LDV_STATEMENT_KIND (then_statement) != LDV_COMPOUND_STATEMENT)
            {
              then_statement_aux = XCNEW (struct ldv_statement);
              LDV_STATEMENT_KIND (then_statement_aux) = LDV_COMPOUND_STATEMENT;

              compound_statement = XCNEW (struct ldv_compound_statement);
              LDV_STATEMENT_COMPOUND_STATEMENT (then_statement_aux) = compound_statement;

              block_item_list = XCNEW (struct ldv_block_item_list);
              LDV_COMPOUND_STATEMENT_BLOCK_ITEM_LIST (compound_statement) = block_item_list;

              block_item = XCNEW (struct ldv_block_item);
              LDV_BLOCK_ITEM_LIST_BLOCK_ITEM (block_item_list) = block_item;
              LDV_BLOCK_ITEM_KIND (block_item) = LDV_BLOCK_ITEM_SECOND;
              LDV_BLOCK_ITEM_STATEMENT (block_item) = then_statement;

              LDV_SELECTION_STATEMENT_STATEMENT1 (selection_statement) = then_statement_aux;
            }
        }
      else
        LDV_SELECTION_STATEMENT_KIND (selection_statement) = LDV_SELECTION_STATEMENT_IF_THEN;

      LDV_SELECTION_STATEMENT_LOCATION (selection_statement) = ldv_convert_location (t);

      break;

    case SWITCH_EXPR:
      LDV_SELECTION_STATEMENT_KIND (selection_statement) = LDV_SELECTION_STATEMENT_SWITCH;

      if ((cond_expr = SWITCH_COND (t)))
        LDV_SELECTION_STATEMENT_EXPR (selection_statement) = ldv_convert_expr (cond_expr, LDV_CONVERT_EXPR_RECURSION_LIMIT);
      else
        LDV_ERROR ("can't find conditional expression");

      if ((switch_statement = SWITCH_BODY (t)))
        LDV_SELECTION_STATEMENT_STATEMENT1 (selection_statement) = ldv_convert_statement (switch_statement);
      else
        LDV_ERROR ("can't find switch statement");

      LDV_SELECTION_STATEMENT_LOCATION (selection_statement) = ldv_convert_location (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_SELECTION_STATEMENT_KIND (selection_statement))
    return selection_statement;

  LDV_ERROR ("selection statement wasn't converted");

  XDELETE (selection_statement);

  return NULL;
}

/*
shift-expression:
    additive-expression
    shift-expression << additive-expression
    shift-expression >> additive-expression

LDV extensions:

shift-expression:
    LDV_LROTATE (shift-expression, additive-expression)
    LDV_RROTATE (shift-expression, additive-expression)
*/
static ldv_shift_expr_ptr
ldv_convert_shift_expr (tree t, unsigned int recursion_limit)
{
  ldv_shift_expr_ptr shift_expr;
  tree op1, op2;

  shift_expr = XCNEW (struct ldv_shift_expr);

  switch (TREE_CODE (t))
    {
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case LROTATE_EXPR:
    case RROTATE_EXPR:
      if (TREE_CODE (t) == LSHIFT_EXPR)
        LDV_SHIFT_EXPR_KIND (shift_expr) = LDV_SHIFT_EXPR_SECOND;
      else if (TREE_CODE (t) == RSHIFT_EXPR)
        LDV_SHIFT_EXPR_KIND (shift_expr) = LDV_SHIFT_EXPR_THIRD;
      else if (TREE_CODE (t) == LROTATE_EXPR)
        LDV_SHIFT_EXPR_KIND (shift_expr) = LDV_SHIFT_EXPR_FOURTH;
      else
        LDV_SHIFT_EXPR_KIND (shift_expr) = LDV_SHIFT_EXPR_FIFTH;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_SHIFT_EXPR_SHIFT_EXPR (shift_expr) = ldv_convert_shift_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of shift expression");

      if ((op2 = LDV_OP_SECOND (t)))
        LDV_SHIFT_EXPR_ADDITIVE_EXPR (shift_expr) = ldv_convert_additive_expr (op2, recursion_limit);
      else
        LDV_ERROR ("can't find the second operand of shift expression");

      LDV_SHIFT_EXPR_LOCATION (shift_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_SHIFT_EXPR_KIND (shift_expr) = LDV_SHIFT_EXPR_FIRST;
      LDV_SHIFT_EXPR_ADDITIVE_EXPR (shift_expr) = ldv_convert_additive_expr (t, recursion_limit);
    }

  if (LDV_SHIFT_EXPR_KIND (shift_expr))
    return shift_expr;

  LDV_ERROR ("shift expression wasn't converted");

  XDELETE (shift_expr);

  return NULL;
}

/*
specifier-qualifier-list:
    type-specifier specifier-qualifier-listopt
    type-qualifier specifier-qualifier-listopt
*/
static ldv_spec_qual_list_ptr
ldv_convert_spec_qual_list (tree t)
{
  ldv_spec_qual_list_ptr spec_qual_list, spec_qual_list_cur;
  ldv_decl_spec_ptr decl_type_spec, decl_type_qual, decl_type_spec_qual, decl_cur, decl_prev;
  ldv_type_spec_ptr type_spec;
  ldv_type_qual_ptr type_qual;

  spec_qual_list = XCNEW (struct ldv_spec_qual_list);
  spec_qual_list_cur = NULL;

  decl_type_spec = ldv_convert_type_spec (t, false);
  decl_type_qual = ldv_convert_type_qual (t);
  decl_type_spec_qual = NULL;

  /* Merge type specifiers and qualifiers to the same declaration specifiers list. */
  if (decl_type_spec || decl_type_qual)
    {
      if (decl_type_spec)
        {
          decl_type_spec_qual = decl_type_spec;
          for (decl_cur = decl_type_spec_qual; LDV_DECL_DECL_SPEC (decl_cur); decl_cur = LDV_DECL_DECL_SPEC (decl_cur)) ;
          LDV_DECL_DECL_SPEC (decl_cur) = decl_type_qual;
        }
      else
        decl_type_spec_qual = decl_type_qual;
    }

  /* Assign type specifiers and qualifiers to the specifier qualifier list itself. */
  for (decl_cur = decl_type_spec_qual, decl_prev = NULL; decl_cur; decl_cur = LDV_DECL_DECL_SPEC (decl_cur))
    {
      if (decl_prev)
        XDELETE (decl_prev);

      if (spec_qual_list_cur)
        {
          LDV_SPEC_QUAL_LIST_SPEC_QUAL_LIST (spec_qual_list_cur) = XCNEW (struct ldv_spec_qual_list);
          spec_qual_list_cur = LDV_SPEC_QUAL_LIST_SPEC_QUAL_LIST (spec_qual_list_cur);
        }
      else
        spec_qual_list_cur = spec_qual_list;

      if ((type_spec = LDV_DECL_SPEC_TYPE_SPEC (decl_cur)))
        LDV_SPEC_QUAL_LIST_TYPE_SPEC (spec_qual_list_cur) = type_spec;
      else if ((type_qual = LDV_DECL_SPEC_TYPE_QUAL (decl_cur)))
        LDV_SPEC_QUAL_LIST_TYPE_QUAL (spec_qual_list_cur) = type_qual;
      else
        LDV_ERROR ("incorrect declaration specifier");

      /* Remember current declaration specifier to free it on the next iteration. */
      decl_prev = decl_cur;
    }

  /* Free last declaration specifier in list. */
  if (decl_prev)
    XDELETE (decl_prev);

  if (LDV_SPEC_QUAL_LIST_TYPE_SPEC (spec_qual_list) || LDV_SPEC_QUAL_LIST_TYPE_QUAL (spec_qual_list))
    return spec_qual_list;

  LDV_ERROR ("specifier qualifier list wasn't converted");

  XDELETE (spec_qual_list);

  return NULL;
}

/*
statement:
    labeled-statement
    compound-statement
    expression-statement
    selection-statement
    iteration-statement
    jump-statement

GNU extensions:

statement:
    asm-statement
*/
static ldv_statement_ptr
ldv_convert_statement (tree t)
{
  ldv_statement_ptr statement;

  statement = XCNEW (struct ldv_statement);

  switch (TREE_CODE (t))
    {
    case CASE_LABEL_EXPR:
    case LABEL_EXPR:
      LDV_STATEMENT_KIND (statement) = LDV_LABELED_STATEMENT;
      LDV_STATEMENT_LABELED_STATEMENT (statement) = ldv_convert_labeled_statement (t);

      break;

    case BIND_EXPR:
    case STATEMENT_LIST:
      LDV_STATEMENT_KIND (statement) = LDV_COMPOUND_STATEMENT;
      LDV_STATEMENT_COMPOUND_STATEMENT (statement) = ldv_convert_compound_statement (t);

      break;

    case COND_EXPR:
    case SWITCH_EXPR:
      /* In this case we have conditional expression (... ? ... : ...). */
      if (TREE_CODE (t) == COND_EXPR
        && TREE_TYPE (t) && TREE_TYPE (t) != void_type_node)
        {
          LDV_STATEMENT_KIND (statement) = LDV_EXPR_STATEMENT;
          LDV_STATEMENT_EXPR_STATEMENT (statement) = ldv_convert_expr_statement (t);
        }
      else
        {
          LDV_STATEMENT_KIND (statement) = LDV_SELECTION_STATEMENT;
          LDV_STATEMENT_SELECTION_STATEMENT (statement) = ldv_convert_selection_statement (t);
        }

      break;

    case RETURN_EXPR:
    case GOTO_EXPR:
      LDV_STATEMENT_KIND (statement) = LDV_JUMP_STATEMENT;
      LDV_STATEMENT_JUMP_STATEMENT (statement) = ldv_convert_jump_statement (t);

      break;

    case ASM_EXPR:
      LDV_STATEMENT_KIND (statement) = LDV_ASM_STATEMENT;
      LDV_STATEMENT_ASM_STATEMENT (statement) = ldv_convert_asm_statement (t);

      break;

    default:
      LDV_STATEMENT_KIND (statement) = LDV_EXPR_STATEMENT;
      LDV_STATEMENT_EXPR_STATEMENT (statement) = ldv_convert_expr_statement (t);
    }

  if (LDV_STATEMENT_KIND (statement))
    return statement;

  LDV_ERROR ("statement wasn't converted");

  XDELETE (statement);

  return NULL;
}

/*
storage-class-specifier:
    typedef
    extern
    static
    auto
    register
*/
static ldv_decl_spec_ptr
ldv_convert_storage_class_spec (tree t)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_storage_class_spec_ptr storage_class_spec;

  decl_spec = XCNEW (struct ldv_decl_spec);
  storage_class_spec = LDV_DECL_SPEC_STORAGE_CLASS_SPEC (decl_spec) = XCNEW (struct ldv_storage_class_spec);

  /* Note that in accordance with the C standard may be just one storage class
     specifier for a given entity. */
  switch (TREE_CODE (t))
    {
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case BOOLEAN_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case ENUMERAL_TYPE:
      break;

    case POINTER_TYPE:
      break;

    case PARM_DECL:
      break;

    case FUNCTION_DECL:
      if (DECL_REGISTER (t))
        LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec) = LDV_STORAGE_CLASS_SPEC_REGISTER;
      else if (!TREE_PUBLIC (t) && DECL_FILE_SCOPE_P (t))
        LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec) = LDV_STORAGE_CLASS_SPEC_STATIC;

      break;

    case TYPE_DECL:
      LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec) = LDV_STORAGE_CLASS_SPEC_TYPEDEF;

      break;

    case VAR_DECL:
      if (DECL_REGISTER (t))
        LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec) = LDV_STORAGE_CLASS_SPEC_REGISTER;
      else if (!TREE_PUBLIC (t) && DECL_FILE_SCOPE_P (t))
        LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec) = LDV_STORAGE_CLASS_SPEC_STATIC;
      else if (!TREE_STATIC (t) && DECL_FILE_SCOPE_P (t))
        LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec) = LDV_STORAGE_CLASS_SPEC_EXTERN;

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec))
    return decl_spec;

  XDELETE (storage_class_spec);
  XDELETE (decl_spec);

  return NULL;
}

/*
string-literal:
    " s-char-sequenceopt "
    L" s-char-sequenceopt "
*/
static ldv_str_literal_ptr
ldv_convert_str_literal (tree t)
{
  ldv_str_literal_ptr str_literal;
  const char *str;
  void **slot;

  str_literal = XCNEW (struct ldv_str_literal);

  switch (TREE_CODE (t))
    {
    case STRING_CST:
      if ((str = TREE_STRING_POINTER (t)))
        {
          if (ldv_tree_string_htab)
            {
              slot = htab_find_slot_with_hash (ldv_tree_string_htab, t, htab_hash_pointer (t), NO_INSERT);

              if (slot == NULL)
                LDV_STR_LITERAL_STR (str_literal) = str;
              else if (*slot)
                LDV_STR_LITERAL_STR (str_literal) = ((struct ldv_hash_tree_string *)*slot)->str;
            }
          else
            LDV_STR_LITERAL_STR (str_literal) = str;
        }
      else
        LDV_CONVERT_ERROR (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_STR_LITERAL_STR (str_literal))
    return str_literal;

  LDV_ERROR ("string literal wasn't converted");

  XDELETE (str_literal);

  return NULL;
}

/*
struct-declaration:
    specifier-qualifier-list struct-declarator-list ;

GNU extensions:

struct-declaration:
    specifier-qualifier-list
*/
ldv_struct_decl_ptr
ldv_convert_struct_decl (tree t)
{
  ldv_struct_decl_ptr struct_decl;
  tree field_type;

  struct_decl = XCNEW (struct ldv_struct_decl);

  switch (TREE_CODE (t))
    {
    case FIELD_DECL:
      if (DECL_C_BIT_FIELD (t))
        {
          if (!(field_type = DECL_BIT_FIELD_TYPE (t)))
            LDV_ERROR ("can't find original bitfield type");
        }
      else
        {
          if (!(field_type = TREE_TYPE (t)))
            LDV_ERROR ("can't find field declaration type");
        }

      if (field_type)
        LDV_STRUCT_DECL_SPEC_QUAL_LIST (struct_decl) = ldv_convert_spec_qual_list (field_type);

      /* Do not create artificial structure declarators (the GNU extension
         allows empty structure declarator lists). This fixes
         http://forge.ispras.ru/issues/5280. */
      if (DECL_NAME (t))
        LDV_STRUCT_DECL_STRUCT_DECLARATOR_LIST (struct_decl) = ldv_convert_struct_declarator_list (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_STRUCT_DECL_SPEC_QUAL_LIST (struct_decl) || LDV_STRUCT_DECL_STRUCT_DECLARATOR_LIST (struct_decl))
    return struct_decl;

  LDV_ERROR ("struct declaration wasn't converted");

  XDELETE (struct_decl);

  return NULL;
}

/*
struct-declaration-list:
    struct-declaration
    struct-declaration-list struct-declaration
*/
static ldv_struct_decl_list_ptr
ldv_convert_struct_decl_list (tree t)
{
  ldv_struct_decl_list_ptr struct_decl_list, struct_decl_list_next, struct_decl_list_next_next;
  tree struct_decl_next;

  struct_decl_list = XCNEW (struct ldv_struct_decl_list);
  struct_decl_list_next = NULL;

  switch (TREE_CODE (t))
    {
    case FIELD_DECL:
      if ((struct_decl_next = TREE_CHAIN (t)))
        {
          struct_decl_list_next = ldv_convert_struct_decl_list (struct_decl_next);
          for (struct_decl_list_next_next = struct_decl_list_next; LDV_STRUCT_DECL_LIST_STRUCT_DECL_LIST (struct_decl_list_next_next); struct_decl_list_next_next = LDV_STRUCT_DECL_LIST_STRUCT_DECL_LIST (struct_decl_list_next_next)) ;
          LDV_STRUCT_DECL_LIST_STRUCT_DECL_LIST (struct_decl_list_next_next) = struct_decl_list;
        }
      LDV_STRUCT_DECL_LIST_STRUCT_DECL (struct_decl_list) = ldv_convert_struct_decl (t);
      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (struct_decl_list_next)
    return struct_decl_list_next;

  if (LDV_STRUCT_DECL_LIST_STRUCT_DECL (struct_decl_list) || LDV_STRUCT_DECL_LIST_STRUCT_DECL_LIST (struct_decl_list))
    return struct_decl_list;

  LDV_ERROR ("struct declaration list wasn't converted");

  XDELETE (struct_decl_list);

  return NULL;
}

/*
struct-declarator:
    declarator
    declaratoropt : constant-expression
*/
static ldv_struct_declarator_ptr
ldv_convert_struct_declarator (tree t)
{
  ldv_struct_declarator_ptr struct_declarator;

  struct_declarator = XCNEW (struct ldv_struct_declarator);

  LDV_STRUCT_DECLARATOR_DECLARATOR (struct_declarator) = ldv_convert_declarator_internal (t, false);

  switch (TREE_CODE (t))
    {
    case FIELD_DECL:
      if (DECL_C_BIT_FIELD (t))
        {
          if (!DECL_SIZE (t) || TREE_CODE (DECL_SIZE (t)) != INTEGER_CST)
            LDV_ERROR ("can't find bitfield size");
          else
            LDV_STRUCT_DECLARATOR_CONST_EXPR (struct_declarator) = ldv_convert_integer_constant(DECL_SIZE (t));
        }

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  return struct_declarator;
}

/*
struct-declarator-list:
    struct-declarator
    struct-declarator-list , struct-declarator
*/
static ldv_struct_declarator_list_ptr
ldv_convert_struct_declarator_list (tree t)
{
  ldv_struct_declarator_list_ptr struct_declarator_list;

  struct_declarator_list = XCNEW (struct ldv_struct_declarator_list);

  /* Note that gcc doesn't keep declarator list. It separates all declarators
     from each other. But this function is for the C standard correspondence. */
  if ((LDV_STRUCT_DECLARATOR_LIST_STRUCT_DECLARATOR (struct_declarator_list) = ldv_convert_struct_declarator (t)))
    return struct_declarator_list;

  LDV_ERROR ("struct declarator list wasn't converted");

  XDELETE (struct_declarator_list);

  return NULL;
}

/*
struct-or-union:
    struct
    union
*/
static ldv_struct_or_union_ptr
ldv_convert_struct_or_union (tree t)
{
  ldv_struct_or_union_ptr struct_or_union;

  struct_or_union = XCNEW (struct ldv_struct_or_union);

  switch (TREE_CODE (t))
    {
    case RECORD_TYPE:
      LDV_STRUCT_OR_UNION_STRUCT_OR_UNION (struct_or_union) = LDV_STRUCT_OR_UNION_STRUCT;
      break;

    case UNION_TYPE:
      LDV_STRUCT_OR_UNION_STRUCT_OR_UNION (struct_or_union) = LDV_STRUCT_OR_UNION_UNION;
      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_STRUCT_OR_UNION_STRUCT_OR_UNION (struct_or_union))
    return struct_or_union;

  LDV_ERROR ("struct or union wasn't converted");

  XDELETE (struct_or_union);

  return NULL;
}

/*
struct-or-union-specifier:
    struct-or-union identifieropt { struct-declaration-list }
    struct-or-union identifier

GNU extensions:

struct-or-union-specifier:
    struct-or-union identifieropt { }
*/
static ldv_decl_spec_ptr
ldv_convert_struct_or_union_spec (tree t, bool is_decl_decl_spec)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_type_spec_ptr type_spec;
  ldv_struct_or_union_spec_ptr struct_or_union_spec;
  ldv_struct_decl_list_ptr fields_list;
  tree struct_or_union_name, struct_or_union_fields, attrs;
  const char *struct_or_union_name_str;

  decl_spec = XCNEW (struct ldv_decl_spec);
  type_spec = XCNEW (struct ldv_type_spec);
  struct_or_union_spec = XCNEW (struct ldv_struct_or_union_spec);

  LDV_DECL_SPEC_TYPE_SPEC (decl_spec) = type_spec;
  LDV_TYPE_SPEC_STRUCT_OR_UNION_SPEC (type_spec) = struct_or_union_spec;

  switch (TREE_CODE (t))
    {
    case RECORD_TYPE:
    case UNION_TYPE:
      LDV_TYPE_SPEC_KIND (type_spec) = LDV_TYPE_SPEC_FIRST;

      LDV_STRUCT_OR_UNION_SPEC_STRUCT_OR_UNION (struct_or_union_spec) = ldv_convert_struct_or_union (t);

      if ((struct_or_union_name = TYPE_NAME (t)))
        if ((struct_or_union_name_str = IDENTIFIER_POINTER (struct_or_union_name)))
            LDV_STRUCT_OR_UNION_SPEC_ID (struct_or_union_spec) = struct_or_union_name_str;

      /* Fields should be printed just for full type declarations. */
      if ((!struct_or_union_name || is_decl_decl_spec) && C_TYPE_BEING_DEFINED (t))
        {
          if ((struct_or_union_fields = TYPE_FIELDS (t)))
            {
              LDV_STRUCT_OR_UNION_SPEC_KIND (struct_or_union_spec) = LDV_STRUCT_OR_UNION_SPEC_FIRST;

              if ((fields_list = ldv_convert_struct_decl_list (struct_or_union_fields)))
                LDV_STRUCT_OR_UNION_SPEC_STRUCT_DECL_LIST (struct_or_union_spec) = fields_list;

              if ((attrs = TYPE_ATTRIBUTES (t)))
                for (; attrs != NULL_TREE; attrs = TREE_CHAIN (attrs))
                  {
                    if (strstr (IDENTIFIER_POINTER (TREE_PURPOSE (attrs)), "packed"))
                      LDV_STRUCT_OR_UNION_SPEC_ISPACKED (struct_or_union_spec) = true;
                    else if (strstr (IDENTIFIER_POINTER (TREE_PURPOSE (attrs)), "aligned"))
                      {
                        LDV_STRUCT_OR_UNION_SPEC_ISALIGNED (struct_or_union_spec) = true;
                        if (TREE_VALUE (attrs))
                          LDV_STRUCT_OR_UNION_SPEC_ALIGNMENT (struct_or_union_spec) = ldv_convert_integer_constant (TREE_VALUE (TREE_VALUE (attrs)));
                      }
                  }

              if (TYPE_TRANSPARENT_AGGR (t))
                LDV_STRUCT_OR_UNION_SPEC_ISTRANSPARENT_UNION (struct_or_union_spec) = true;
            }
          else
            LDV_STRUCT_OR_UNION_SPEC_KIND (struct_or_union_spec) = LDV_STRUCT_OR_UNION_SPEC_THIRD;
        }
      else
        LDV_STRUCT_OR_UNION_SPEC_KIND (struct_or_union_spec) = LDV_STRUCT_OR_UNION_SPEC_SECOND;

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_TYPE_SPEC_KIND (type_spec))
    return decl_spec;

  LDV_ERROR ("struct or union specifier wasn't converted");

  XDELETE (struct_or_union_spec);
  XDELETE (type_spec);
  XDELETE (decl_spec);

  return NULL;
}

/*
type-name:
    specifier-qualifier-list abstract-declaratoropt
*/
static ldv_type_name_ptr
ldv_convert_type_name (tree t)
{
  ldv_type_name_ptr type_name;

  type_name = XCNEW (struct ldv_type_name);

  LDV_TYPE_NAME_SPEC_QUAL_LIST (type_name) = ldv_convert_spec_qual_list (t);
  LDV_TYPE_NAME_ABSTRACT_DECLARATOR (type_name) = ldv_convert_abstract_declarator_internal (t);

  return type_name;
}

/*
type-qualifier:
    const
    restrict
    volatile
*/
static ldv_decl_spec_ptr
ldv_convert_type_qual (tree t)
{
  tree decl_type;
  tree array_type, func_ret_type, pointer_type;

  switch (TREE_CODE (t))
    {
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case BOOLEAN_TYPE:
    //case FIXED_POINT_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case ENUMERAL_TYPE:
      return ldv_convert_type_qual_internal (t);

    case FUNCTION_DECL:
    case PARM_DECL:
    case TYPE_DECL:
    case VAR_DECL:
      if ((decl_type = TREE_TYPE (t)))
        return ldv_convert_type_qual (decl_type);

      LDV_ERROR ("can't find declaration type");

      break;

    case POINTER_TYPE:
      if ((pointer_type = TREE_TYPE (t)))
        return ldv_convert_type_qual (pointer_type);

      LDV_ERROR ("can't find pointer type");

      break;

    case ARRAY_TYPE:
      if ((array_type = TREE_TYPE (t)))
        return ldv_convert_type_qual (array_type);

      LDV_ERROR ("can't find array type");

      break;

    case FUNCTION_TYPE:
      if ((func_ret_type = TREE_TYPE (t)))
        return ldv_convert_type_qual (func_ret_type);

      LDV_ERROR ("can't find function return type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  LDV_ERROR ("type qualifier wasn't converted");

  return NULL;
}

static ldv_decl_spec_ptr
ldv_convert_type_qual_internal (tree t)
{
  ldv_decl_spec_ptr decl_spec, decl_spec_cur;
  bool is_type_qual;
  int type_quals;

  decl_spec_cur = decl_spec = XCNEW (struct ldv_decl_spec);
  is_type_qual = false;

  type_quals = TYPE_QUALS (t);

  if (type_quals != TYPE_UNQUALIFIED)
    {
      if (type_quals & TYPE_QUAL_CONST)
        ldv_new_type_qual (&is_type_qual, &decl_spec_cur, LDV_TYPE_QUAL_CONST);

      if (type_quals & TYPE_QUAL_RESTRICT)
        ldv_new_type_qual (&is_type_qual, &decl_spec_cur, LDV_TYPE_QUAL_RESTRICT);

      if (type_quals & TYPE_QUAL_VOLATILE)
        ldv_new_type_qual (&is_type_qual, &decl_spec_cur, LDV_TYPE_QUAL_VOLATILE);

      /* Sometimes a new type qualifier may arise. In this case say about it and
         generate artificial type qualifier to print warning message later. */
      if (!is_type_qual)
        {
          LDV_ERROR ("type qualifier wasn't converted");
          ldv_new_type_qual (&is_type_qual, &decl_spec_cur, LDV_TYPE_QUAL_UNKNOWN);
        }
    }

  if (is_type_qual)
    return decl_spec;

  XDELETE (decl_spec);

  return NULL;
}

/*
type-qualifier-list:
    type-qualifier
    type-qualifier-list type-qualifier
*/
static ldv_type_qual_list_ptr
ldv_convert_type_qual_list (tree t)
{
  ldv_decl_spec_ptr decl_spec_cur, decl_spec_prev;
  ldv_type_qual_list_ptr type_qual_list, type_qual_list_cur;
  ldv_type_qual_ptr type_qual;
  bool is_type_qual;

  type_qual_list_cur = type_qual_list = XCNEW (struct ldv_type_qual_list);
  is_type_qual = false;

  for (decl_spec_cur = ldv_convert_type_qual_internal (t)
    ; decl_spec_cur
    ; decl_spec_cur = LDV_DECL_SPEC_DECL_SPEC (decl_spec_cur), XDELETE (decl_spec_prev))
    {
      if ((type_qual = LDV_DECL_SPEC_TYPE_QUAL (decl_spec_cur)))
        {
          if (is_type_qual)
            {
              LDV_TYPE_QUAL_LIST_TYPE_QUAL_LIST (type_qual_list_cur) = XCNEW (struct ldv_type_qual_list);
              type_qual_list_cur = LDV_TYPE_QUAL_LIST_TYPE_QUAL_LIST (type_qual_list_cur);
            }

          LDV_TYPE_QUAL_LIST_TYPE_QUAL (type_qual_list_cur) = type_qual;
          is_type_qual = true;
        }
      else
        LDV_ERROR ("incorrect declaration specifier");

      decl_spec_prev = decl_spec_cur;
    }

  if (is_type_qual)
    return type_qual_list;

  /* There is may be no type qualifiers at all so don't consider such the case
     as an error. */

  XDELETE (type_qual_list);

  return NULL;
}

/*
type-specifier:
    void
    char
    short
    int
    long
    float
    double
    signed
    unsigned
    _Bool
    _Complex
    struct-or-union-specifier
    enum-specifier
    typedef-name
*/
static ldv_decl_spec_ptr
ldv_convert_type_spec (tree t, bool is_decl_decl_spec)
{
  tree decl_type, decl_type_original;
  tree array_type, func_ret_type, pointer_type;
  tree type_name;

  switch (TREE_CODE (t))
    {
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case BOOLEAN_TYPE:
    //case FIXED_POINT_TYPE:
      /* Here is typedef name is also processed. It depends on whether type has
         an another main variant or not. */
      if (TYPE_MAIN_VARIANT (t) == t)
        return ldv_convert_type_spec_internal (t);
      else
        {
          if ((type_name = TYPE_NAME (t)))
            return ldv_convert_typedef_name (type_name);
          else
            LDV_ERROR ("can't find type declaration");
        }

      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case ENUMERAL_TYPE:
      /* Here is typedef name is also processed. It depends on whether type has
         a name or not. */
      if ((type_name = TYPE_NAME (t)) && TREE_CODE(type_name) == TYPE_DECL)
        return ldv_convert_typedef_name (type_name);

      if (TREE_CODE (t) == ENUMERAL_TYPE)
        return ldv_convert_enum_spec (t, is_decl_decl_spec);
      else
        return ldv_convert_struct_or_union_spec (t, is_decl_decl_spec);

      break;

    case FUNCTION_DECL:
    case PARM_DECL:
    case VAR_DECL:
      if ((decl_type = TREE_TYPE (t)))
        return ldv_convert_type_spec (decl_type, false);

      LDV_ERROR ("can't find declaration type");

      break;

    case TYPE_DECL:
      if ((decl_type_original = DECL_ORIGINAL_TYPE (t)))
        return ldv_convert_type_spec (decl_type_original, false);

      LDV_ERROR ("can't find original declaration type");

      break;

    case POINTER_TYPE:
      /* Here is typedef name is also processed. It depends on whether type has
         a name or not. */
      if ((type_name = TYPE_NAME (t)) && TREE_CODE(type_name) == TYPE_DECL)
        return ldv_convert_typedef_name (type_name);

      if ((pointer_type = TREE_TYPE (t)))
        return ldv_convert_type_spec (pointer_type, false);

      LDV_ERROR ("can't find pointer type");

      break;

    case ARRAY_TYPE:
      /* Here is typedef name is also processed. It depends on whether type has
         a name or not. */
      if ((type_name = TYPE_NAME (t)) && TREE_CODE(type_name) == TYPE_DECL)
        return ldv_convert_typedef_name (type_name);

      if ((array_type = TREE_TYPE (t)))
        return ldv_convert_type_spec (array_type, false);

      LDV_ERROR ("can't find array type");

      break;

    case FUNCTION_TYPE:
      /* Here is typedef name is also processed. It depends on whether type has
         a name or not. */
      if ((type_name = TYPE_NAME (t)) && TREE_CODE(type_name) == TYPE_DECL)
        return ldv_convert_typedef_name (type_name);

      if ((func_ret_type = TREE_TYPE (t)))
        return ldv_convert_type_spec (func_ret_type, false);

      LDV_ERROR ("can't find function return type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  LDV_ERROR ("type specifier wasn't converted");

  return NULL;
}

static ldv_decl_spec_ptr
ldv_convert_type_spec_internal (tree t)
{
  ldv_decl_spec_ptr decl_spec, decl_spec_cur;
  bool is_type_spec;
  tree type_decl, type_name;
  tree type = NULL_TREE;
  const char *type_name_str;

  decl_spec_cur = decl_spec = XCNEW (struct ldv_decl_spec);
  is_type_spec = false;

  if (!(type_decl = TYPE_NAME (t)))
    {
      if (!(type = c_common_type_for_mode (TYPE_MODE (t), TYPE_UNSIGNED (t))))
        LDV_ERROR ("can't find appropriate type");
    }
  else
    type = t;

  if (type && (type_decl = TYPE_NAME (type)))
    {
      if ((type_name = DECL_NAME (type_decl)))
        {
          if ((type_name_str = IDENTIFIER_POINTER ((type_name))))
            {
              /* Here we match type names with the corresponding ones
                 from gcc/builtin-types.def. */
              if (strstr (type_name_str, "short"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_SHORT);

              if (strstr (type_name_str, "long long"))
                {
                  ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_LONG);
                  ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_LONG);
                }
              else if (strstr (type_name_str, "long"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_LONG);

              if (strstr (type_name_str, "unsigned"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_UNSIGNED);
              else if (strstr (type_name_str, "signed"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_SIGNED);

              if (strstr (type_name_str, "void"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_VOID);

              if (strstr (type_name_str, "char"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_CHAR);

              if (strstr (type_name_str, "int"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_INT);

              if (strstr (type_name_str, "float"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_FLOAT);

              if (strstr (type_name_str, "double"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_DOUBLE);

              if (strstr (type_name_str, "_Bool"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_BOOL);

              if (strstr (type_name_str, "_Float32x"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_FLOAT32X);
              else if (strstr (type_name_str, "_Float32"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_FLOAT32);

              if (strstr (type_name_str, "_Float64x"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_FLOAT64X);
              else if (strstr (type_name_str, "_Float64"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_FLOAT64);

              if (strstr (type_name_str, "_Float128"))
                ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_FLOAT128);

              /* Sometimes a new type specifier may arise. In this case say
                 about it and generate artificial type specifier to print
                 warning message later. */
              if (!is_type_spec)
                {
                  ldv_str_ptr str = ldv_create_string ();
                  ldv_puts_string ("type specifier \"", str);
                  ldv_puts_string (type_name_str, str);
                  ldv_puts_string ("\" wasn't converted", str);
                  LDV_ERROR (ldv_get_str (str));
                  ldv_free_string (str);
                  ldv_new_type_spec (&is_type_spec, &decl_spec_cur, LDV_TYPE_SPEC_UNKNOWN);
                }
            }
          else
            LDV_ERROR ("can't find type name");
        }
      else
        LDV_ERROR ("can't find type name identifier");
    }
  else
    LDV_ERROR ("can't find type declaration");

  if (is_type_spec)
    return decl_spec;

  LDV_ERROR ("type specifier wasn't converted");

  XDELETE (decl_spec);

  return NULL;
}

/*
typedef-name:
    identifier
*/
static ldv_decl_spec_ptr
ldv_convert_typedef_name (tree t)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_type_spec_ptr type_spec;
  ldv_typedef_name_ptr typedef_name;

  decl_spec = XCNEW (struct ldv_decl_spec);
  type_spec = XCNEW (struct ldv_type_spec);
  typedef_name = XCNEW (struct ldv_typedef_name);

  LDV_DECL_SPEC_TYPE_SPEC (decl_spec) = type_spec;
  LDV_TYPE_SPEC_TYPEDEF_NAME (type_spec) = typedef_name;

  switch (TREE_CODE (t))
    {
    case TYPE_DECL:
      LDV_TYPE_SPEC_KIND (type_spec) = LDV_TYPE_SPEC_THIRD;
      LDV_TYPEDEF_NAME_IDENTIFIER (typedef_name) = ldv_convert_identifier (t);

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_TYPE_SPEC_KIND (type_spec))
    return decl_spec;

  LDV_ERROR ("typedef name wasn't converted");

  XDELETE (typedef_name);
  XDELETE (type_spec);
  XDELETE (decl_spec);

  return NULL;
}

/*
unary-expression:
    postfix-expression
    ++ unary-expression
    -- unary-expression
    unary-operator cast-expression
    sizeof unary-expression
    sizeof ( type-name )

GNU extensions:

    && identifier
*/
static ldv_unary_expr_ptr
ldv_convert_unary_expr (tree t, unsigned int recursion_limit)
{
  ldv_unary_expr_ptr unary_expr;
  tree op1;
  ldv_identifier_ptr identifier;
  char *str;

  /* There is no sizeof expressions in gcc, they are converted to the other
     ones. */

  unary_expr = XCNEW (struct ldv_unary_expr);

  switch (TREE_CODE (t))
    {
    case PREINCREMENT_EXPR:
    case PREDECREMENT_EXPR:
      if (TREE_CODE (t) == PREINCREMENT_EXPR)
        LDV_UNARY_EXPR_KIND (unary_expr) = LDV_UNARY_EXPR_SECOND;
      else if (TREE_CODE (t) == PREDECREMENT_EXPR)
        LDV_UNARY_EXPR_KIND (unary_expr) = LDV_UNARY_EXPR_THIRD;

      if ((op1 = LDV_OP_FIRST (t)))
        LDV_UNARY_EXPR_UNARY_EXPR (unary_expr) = ldv_convert_unary_expr (op1, recursion_limit);
      else
        LDV_ERROR ("can't find the first operand of unary expression");

      LDV_UNARY_EXPR_LOCATION (unary_expr) = ldv_convert_location (t);

      break;

    case ADDR_EXPR:
    case INDIRECT_REF:
    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
    case TRUTH_NOT_EXPR:
      if (!(op1 = LDV_OP_FIRST (t)))
        LDV_ERROR ("can't find the first operand of unary expression");

      /* String constants are always taken by their addresses. To prevent this
         skip address expression and go directly to string constant itself.
         This also arises for __func__ GNU extension represented as address to
         variable having corresponding name. */
      if (TREE_CODE (t) == ADDR_EXPR)
        {
          switch (TREE_CODE (op1))
            {
            case STRING_CST:
              LDV_UNARY_EXPR_KIND (unary_expr) = LDV_UNARY_EXPR_FIRST;
              LDV_UNARY_EXPR_POSTFIX_EXPR (unary_expr) = ldv_convert_postfix_expr (op1, recursion_limit);

              break;

            case VAR_DECL:
              if ((identifier = ldv_convert_identifier (op1)))
                {
                  if ((str = LDV_IDENTIFIER_STR (identifier)) && !strcmp (str, "__func__"))
                    {
                      LDV_UNARY_EXPR_KIND (unary_expr) = LDV_UNARY_EXPR_FIRST;
                      LDV_UNARY_EXPR_POSTFIX_EXPR (unary_expr) = ldv_convert_postfix_expr (op1, recursion_limit);
                    }

                  XDELETE (str);
                  XDELETE (identifier);
                }
              else
                LDV_ERROR ("can't find variable name");

              break;

            default:
              ;
            }

          if (LDV_UNARY_EXPR_KIND (unary_expr))
            break;
        }

      LDV_UNARY_EXPR_KIND (unary_expr) = LDV_UNARY_EXPR_FOURTH;

      LDV_UNARY_EXPR_UNARY_OPERATOR (unary_expr) = ldv_convert_unary_operator (t);

      if (op1)
      {
        if (TREE_CODE (op1) == LABEL_DECL)
        {
          LDV_UNARY_EXPR_KIND (unary_expr) = LDV_UNARY_EXPR_FIFTH;
          LDV_UNARY_EXPR_IDENTIFIER (unary_expr) = ldv_convert_identifier (op1);
        }
        else
          LDV_UNARY_EXPR_CAST_EXPR (unary_expr) = ldv_convert_cast_expr (op1, recursion_limit);
      }

      LDV_UNARY_EXPR_LOCATION (unary_expr) = ldv_convert_location (t);

      break;

    default:
      LDV_UNARY_EXPR_KIND (unary_expr) = LDV_UNARY_EXPR_FIRST;
      LDV_UNARY_EXPR_POSTFIX_EXPR (unary_expr) = ldv_convert_postfix_expr (t, recursion_limit);
    }

  if (LDV_UNARY_EXPR_KIND (unary_expr))
    return unary_expr;

  LDV_ERROR ("unary expression wasn't converted");

  XDELETE (unary_expr);

  return NULL;
}

/*
unary-operator: one of
    & * + - ~ !
*/
static ldv_unary_operator_ptr
ldv_convert_unary_operator (tree t)
{
  ldv_unary_operator_ptr unary_operator;

  unary_operator = XCNEW (struct ldv_unary_operator);

  switch (TREE_CODE (t))
    {
    case ADDR_EXPR:
      LDV_UNARY_OPERATOR_KIND (unary_operator) = LDV_UNARY_OPERATOR_FIRST;

      break;

    case INDIRECT_REF:
      LDV_UNARY_OPERATOR_KIND (unary_operator) = LDV_UNARY_OPERATOR_SECOND;

      break;

    case NEGATE_EXPR:
      LDV_UNARY_OPERATOR_KIND (unary_operator) = LDV_UNARY_OPERATOR_FOURTH;

      break;

    case BIT_NOT_EXPR:
      LDV_UNARY_OPERATOR_KIND (unary_operator) = LDV_UNARY_OPERATOR_FIVTH;

      break;

    case TRUTH_NOT_EXPR:
      LDV_UNARY_OPERATOR_KIND (unary_operator) = LDV_UNARY_OPERATOR_SIXTH;

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_UNARY_OPERATOR_KIND (unary_operator))
    return unary_operator;

  LDV_ERROR ("unary operator wasn't converted");

  XDELETE (unary_operator);

  return NULL;
}

/*
unsigned-suffix: one of
    u U
*/
static ldv_unsigned_suffix_ptr
ldv_convert_unsigned_suffix (tree t)
{
  ldv_unsigned_suffix_ptr unsigned_suffix;

  unsigned_suffix = XCNEW (struct ldv_unsigned_suffix);

  switch (TREE_CODE (t))
    {
    case INTEGER_TYPE:
      if (TYPE_UNSIGNED (t))
        LDV_UNSIGNED_SUFFIX_KIND (unsigned_suffix) = LDV_UNSIGNED_SUFFIX_SECOND;
      else
        LDV_ERROR ("try to get unsigned suffix for signed type");

      break;

    default:
      LDV_CONVERT_ERROR (t);
    }

  if (LDV_UNSIGNED_SUFFIX_KIND (unsigned_suffix))
    return unsigned_suffix;

  LDV_ERROR ("unsigned suffix wasn't converted");

  XDELETE (unsigned_suffix);

  return NULL;
}

static ldv_expr_statement_ptr
ldv_create_null_expr_statement (void)
{
  ldv_expr_statement_ptr expr_statement;

  expr_statement = XCNEW (struct ldv_expr_statement);

  /* Null expression statement has nothing. */

  return expr_statement;
}

static ldv_statement_ptr
ldv_create_null_statement (void)
{
  ldv_statement_ptr statement;

  statement = XCNEW (struct ldv_statement);

  LDV_STATEMENT_KIND (statement) = LDV_EXPR_STATEMENT;
  LDV_STATEMENT_EXPR_STATEMENT (statement) = ldv_create_null_expr_statement ();

  return statement;
}

static ldv_param_decl_ptr
ldv_create_void_param_decl (void)
{
  ldv_param_decl_ptr param_decl;

  param_decl = XCNEW (struct ldv_param_decl);

  LDV_PARAM_DECL_DECL_SPEC (param_decl) = ldv_create_void_decl_spec ();

  return param_decl;
}

static ldv_decl_spec_ptr
ldv_create_void_decl_spec (void)
{
  ldv_decl_spec_ptr decl_spec;

  decl_spec = XCNEW (struct ldv_decl_spec);

  /* Void declaration specifiers consists just of one type specifier 'void'. */
  LDV_DECL_SPEC_TYPE_SPEC (decl_spec) = ldv_create_void_type_spec ();

  return decl_spec;
}

static ldv_type_spec_ptr
ldv_create_void_type_spec (void)
{
  ldv_type_spec_ptr type_spec;

  type_spec = XCNEW (struct ldv_type_spec);

  LDV_TYPE_SPEC_KIND (type_spec) = LDV_TYPE_SPEC_VOID;

  return type_spec;
}

static void
ldv_dump_expr_ops (tree t, unsigned int op_numb, unsigned int indent_level)
{
  unsigned int i;
  tree op1, op2, op3;

  if (!ldv_is_dump_ops ())
    return;

  if (!indent_level)
    printf ("\nLDV expression operands dump:\n");

  for (i = 0; i < indent_level * 2; i++)
    printf (" ");

  switch (op_numb)
    {
    case 0:
      break;

    case 1:
      printf ("op1: ");
      break;

    case 2:
      printf ("op2: ");
      break;

    case 3:
      printf ("op3: ");
      break;

    default:
      LDV_ERROR ("operand number isn't supported");
    }

  printf ("%s\n", LDV_TREE_NODE_NAME (t));

  if (EXPR_P (t))
    {
      if (TREE_OPERAND_LENGTH (t) > 0 && (op1 = LDV_OP_FIRST (t)))
        ldv_dump_expr_ops (op1, 1, indent_level + 1);

      if (TREE_OPERAND_LENGTH (t) > 1 && (op2 = LDV_OP_SECOND (t)))
        ldv_dump_expr_ops (op2, 2, indent_level + 1);

      if (TREE_OPERAND_LENGTH (t) > 2 && (op3 = LDV_OP_THIRD (t)))
        ldv_dump_expr_ops (op3, 3, indent_level + 1);
    }

  if (!indent_level)
    printf ("\n");
}

static bool
ldv_is_not_empty_statement_list (tree t)
{
  tree_stmt_iterator si;
  tree statement;
  bool is_not_empty;

  is_not_empty = false;

  switch (TREE_CODE (t))
    {
    case STATEMENT_LIST:
      /* Check that block item list isn't empty before its processing.
         Declarations also related to the given list aren't took into
         consideration. */
      for (si = tsi_last (t); !tsi_end_p (si); tsi_prev (&si))
        {
          if ((statement = tsi_stmt (si)))
            switch (TREE_CODE (statement))
              {
              case DECL_EXPR:
              case SAVE_EXPR:
                break;

              default:
                is_not_empty = true;
              }
          else
            LDV_ERROR ("can't find statement");
        }

      break;

    default:
      LDV_ERROR ("try to check not a statement list");
    }

  return is_not_empty;
}

static const char *
ldv_label_decl_name (tree t)
{
  tree label_decl_name;
  char *label_decl_name_str;
  unsigned int label_decl_uid;
  const char *label_decl_uid_str;

  switch (TREE_CODE (t))
    {
      case LABEL_DECL:
        if ((label_decl_name = DECL_NAME (t)))
          return IDENTIFIER_POINTER (label_decl_name);
        else if ((label_decl_uid = DECL_UID (t)))
          {
            label_decl_uid_str = ldv_itoa (label_decl_uid);
            label_decl_name_str = XCNEWVEC (char, 3 + 1 + strlen (label_decl_uid_str) + 1);
            sprintf (label_decl_name_str, "ldv_%d", label_decl_uid);
            XDELETE (CONST_CAST (char *, label_decl_uid_str));
            return label_decl_name_str;
          }
        else
          LDV_ERROR ("can't find label declaration name");

        break;

      default:
        LDV_CONVERT_ERROR (t);
    }

  LDV_ERROR ("label declaration name wasn't converted");

  return NULL;
}

static void
ldv_new_decl_spec (bool *is_decl_spec_ptr, ldv_decl_spec_ptr *decl_spec_ptr,
                   ldv_decl_spec_ptr decl_storage_class_spec,
                   ldv_decl_spec_ptr decl_type_spec,
                   ldv_decl_spec_ptr decl_type_qual,
                   ldv_decl_spec_ptr decl_func_spec)
{
  ldv_decl_spec_ptr decl_spec;

  decl_spec = *decl_spec_ptr;

  if (*is_decl_spec_ptr)
    {
      LDV_DECL_SPEC_DECL_SPEC (decl_spec) = XCNEW (struct ldv_decl_spec);
      decl_spec = LDV_DECL_SPEC_DECL_SPEC (decl_spec);
    }

  if (decl_storage_class_spec)
    {
      LDV_DECL_SPEC_STORAGE_CLASS_SPEC (decl_spec) = LDV_DECL_SPEC_STORAGE_CLASS_SPEC (decl_storage_class_spec);
      LDV_DECL_SPEC_DECL_SPEC (decl_spec) = LDV_DECL_SPEC_DECL_SPEC (decl_storage_class_spec);
      XDELETE (decl_storage_class_spec);
    }
  else if (decl_type_spec)
    {
      LDV_DECL_SPEC_TYPE_SPEC (decl_spec) = LDV_DECL_SPEC_TYPE_SPEC (decl_type_spec);
      LDV_DECL_SPEC_DECL_SPEC (decl_spec) = LDV_DECL_SPEC_DECL_SPEC (decl_type_spec);
      XDELETE (decl_type_spec);
    }
  else if (decl_type_qual)
    {
      LDV_DECL_SPEC_TYPE_QUAL (decl_spec) = LDV_DECL_SPEC_TYPE_QUAL (decl_type_qual);
      LDV_DECL_SPEC_DECL_SPEC (decl_spec) = LDV_DECL_SPEC_DECL_SPEC (decl_type_qual);
      XDELETE (decl_type_qual);
    }
  else
    {
      LDV_DECL_SPEC_FUNC_SPEC (decl_spec) = LDV_DECL_SPEC_FUNC_SPEC (decl_func_spec);
      LDV_DECL_SPEC_DECL_SPEC (decl_spec) = LDV_DECL_SPEC_DECL_SPEC (decl_func_spec);
      XDELETE (decl_func_spec);
    }


  for (; LDV_DECL_SPEC_DECL_SPEC (decl_spec); decl_spec = LDV_DECL_SPEC_DECL_SPEC (decl_spec)) ;
  *decl_spec_ptr = decl_spec;

  *is_decl_spec_ptr = true;
}

static void
ldv_new_type_qual (bool *is_type_qual_ptr, ldv_decl_spec_ptr *decl_spec_ptr,
                   enum ldv_type_qual_kind type_qual_kind)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_type_qual_ptr type_qual;

  decl_spec = *decl_spec_ptr;

  if (*is_type_qual_ptr)
    {
      LDV_DECL_SPEC_DECL_SPEC (decl_spec) = XCNEW (struct ldv_decl_spec);
      *decl_spec_ptr = decl_spec = LDV_DECL_SPEC_DECL_SPEC (decl_spec);
    }

  type_qual = LDV_DECL_SPEC_TYPE_QUAL (decl_spec) = XCNEW (struct ldv_type_qual);
  LDV_TYPE_QUAL_KIND (type_qual) = type_qual_kind;
  *is_type_qual_ptr = true;
}

static void
ldv_new_type_spec (bool *is_type_spec_ptr, ldv_decl_spec_ptr *decl_spec_ptr,
                   enum ldv_type_spec_kind type_spec_kind)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_type_spec_ptr type_spec;

  decl_spec = *decl_spec_ptr;

  if (*is_type_spec_ptr)
    {
      LDV_DECL_SPEC_DECL_SPEC (decl_spec) = XCNEW (struct ldv_decl_spec);
      *decl_spec_ptr = decl_spec = LDV_DECL_SPEC_DECL_SPEC (decl_spec);
    }

  type_spec = LDV_DECL_SPEC_TYPE_SPEC (decl_spec) = XCNEW (struct ldv_type_spec);
  LDV_TYPE_SPEC_KIND (type_spec) = type_spec_kind;
  *is_type_spec_ptr = true;
}
