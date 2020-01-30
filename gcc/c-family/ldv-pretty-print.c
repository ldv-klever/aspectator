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
#include "hash-set.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "alias.h"
#include "symtab.h"
#include "options.h"
#include "wide-int.h"
#include "inchash.h"
#include "tree.h"
#include "stor-layout.h"
#include "attribs.h"
#include "intl.h"
#include "c-pretty-print.h"
#include "tree-pretty-print.h"
#include "tree-iterator.h"
#include "diagnostic.h"
#include "wide-int-print.h"

#include "ldv-convert.h"
#include "ldv-cpp-core.h"
#include "ldv-cbe-core.h"
#include "ldv-grammar.h"
#include "ldv-pretty-print.h"


/* The macro is portable like in the kernel 'do { ... } while (0)'.
   Note that this requires semicolon! */
#define LDV_PRETTY_PRINT_ERROR(indent_level, msg) do { LDV_ERROR (msg); ldv_c_backend_print (indent_level, true, "/* LDV: %s: %d: %s */", __FILE__, __LINE__, msg); } while (0)
/* Free memory of second argument is first argument is true. */
#define LDV_XDELETE_ON_PRINTING(ptr) if (ldv_free_on_printing) XDELETE (ptr)

/* This ugly global variable is to implement a workaround for #3692.
   Please, replace it with a parameter of a structure type to be passed for all
   converting/printing functions if you will need something similar in more
   cases. */
static bool ldv_disable_cast_printing = false;
/* This global variable is for #372. If it is true, then memory for
   printed entities will be immediately freed after printing is
   completed. */
static bool ldv_free_on_printing = false;


static ldv_location_ptr ldv_declarator_location (ldv_declarator_ptr);
static ldv_location_ptr ldv_direct_declarator_location (ldv_direct_declarator_ptr);

static bool ldv_isglobal_var_was_printed (tree t);

static void ldv_print_abstract_declarator (unsigned int, ldv_abstract_declarator_ptr);
static void ldv_print_additive_expr (unsigned int, ldv_additive_expr_ptr);
static void ldv_print_and_expr (unsigned int, ldv_and_expr_ptr);
static void ldv_print_arg_expr_list (unsigned int, ldv_arg_expr_list_ptr);
static void ldv_print_asm_arg (unsigned int, ldv_asm_arg_ptr);
static void ldv_print_asm_clobbers (unsigned int, ldv_asm_clobbers_ptr);
static void ldv_print_asm_goto_arg (unsigned int, ldv_asm_goto_arg_ptr);
static void ldv_print_asm_goto_operands (unsigned int, ldv_asm_goto_operands_ptr);
static void ldv_print_asm_operand (unsigned int, ldv_asm_operand_ptr);
static void ldv_print_asm_operands (unsigned int, ldv_asm_operands_ptr);
static void ldv_print_asm_statement (unsigned int, ldv_asm_statement_ptr);
static void ldv_print_asm_str_literal (unsigned int, ldv_asm_str_literal_ptr);
static void ldv_print_assignment_expr (unsigned int, ldv_assignment_expr_ptr);
static void ldv_print_assignment_operator (unsigned int, ldv_assignment_operator_ptr);
static void ldv_print_block_item (unsigned int, ldv_block_item_ptr);
static void ldv_print_block_item_list (unsigned int, ldv_block_item_list_ptr);
static void ldv_print_cast_expr (unsigned int, ldv_cast_expr_ptr);
static void ldv_print_compound_statement (unsigned int, ldv_compound_statement_ptr);
static void ldv_print_cond_expr (unsigned int, ldv_cond_expr_ptr);
static void ldv_print_const_expr (unsigned int, ldv_const_expr_ptr);
static void ldv_print_constant (unsigned int, ldv_constant_ptr);
static void ldv_print_decl (unsigned int, ldv_decl_ptr);
static void ldv_print_decl_spec (unsigned int, ldv_decl_spec_ptr);
static void ldv_print_declarator (unsigned int, ldv_declarator_ptr);
static void ldv_print_designation (unsigned int, ldv_designation_ptr);
static void ldv_print_designator (unsigned int, ldv_designator_ptr);
static void ldv_print_designator_list (unsigned int, ldv_designator_list_ptr);
static void ldv_print_direct_abstract_declarator (unsigned int, ldv_direct_abstract_declarator_ptr);
static void ldv_print_direct_declarator (unsigned int, ldv_direct_declarator_ptr);
static void ldv_print_enum (unsigned int, ldv_enum_ptr);
static void ldv_print_enum_const (unsigned int, ldv_enum_const_ptr);
static void ldv_print_enum_list (unsigned int, ldv_enum_list_ptr);
static void ldv_print_enum_spec (unsigned int, ldv_enum_spec_ptr);
static void ldv_print_enumeration_constant (unsigned int, ldv_enumeration_constant_ptr);
static void ldv_print_equality_expr (unsigned int, ldv_equality_expr_ptr);
static void ldv_print_exclusive_or_expr (unsigned int, ldv_exclusive_or_expr_ptr);
static void ldv_print_expr (unsigned int, ldv_expr_ptr);
static void ldv_print_expr_statement (unsigned int, ldv_expr_statement_ptr);
static void ldv_print_ext_decl (unsigned int, ldv_ext_decl_ptr);
static void ldv_print_floating_constant (unsigned int, ldv_floating_constant_ptr);
static void ldv_print_func_def (unsigned int, ldv_func_def_ptr);
static void ldv_print_func_spec (unsigned int, ldv_func_spec_ptr);
static void ldv_print_identifier (unsigned int, ldv_identifier_ptr);
static void ldv_print_inclusive_or_expr (unsigned int, ldv_inclusive_or_expr_ptr);
static void ldv_print_init_declarator (unsigned int, ldv_init_declarator_ptr);
static void ldv_print_init_declarator_list (unsigned int, ldv_init_declarator_list_ptr);
static void ldv_print_initializer (unsigned int, ldv_initializer_ptr);
static void ldv_print_initializer_list (unsigned int, ldv_initializer_list_ptr);
static void ldv_print_integer_constant (unsigned int, ldv_integer_constant_ptr);
static void ldv_print_integer_suffix (unsigned int, ldv_integer_suffix_ptr);
static void ldv_print_jump_statement (unsigned int, ldv_jump_statement_ptr);
static void ldv_print_label_decls (unsigned int, ldv_label_decls_ptr);
static void ldv_print_labeled_statement (unsigned int, ldv_labeled_statement_ptr);
static void ldv_print_line_directive (bool, int, ldv_location_ptr);
static void ldv_print_logical_and_expr (unsigned int, ldv_logical_and_expr_ptr);
static void ldv_print_logical_or_expr (unsigned int, ldv_logical_or_expr_ptr);
static void ldv_print_long_long_suffix (unsigned int, ldv_long_long_suffix_ptr);
static void ldv_print_long_suffix (unsigned int, ldv_long_suffix_ptr);
static void ldv_print_multiplicative_expr (unsigned int, ldv_multiplicative_expr_ptr);
static void ldv_print_nested_decl (unsigned int, ldv_nested_decl_ptr);
static void ldv_print_nested_func_def (unsigned int, ldv_nested_func_def_ptr);
static void ldv_print_param_decl (unsigned int, ldv_param_decl_ptr);
static void ldv_print_param_list (unsigned int, ldv_param_list_ptr);
static void ldv_print_param_type_list (unsigned int, ldv_param_type_list_ptr);
static void ldv_print_pointer (unsigned int, ldv_pointer_ptr);
static void ldv_print_postfix_expr (unsigned int, ldv_postfix_expr_ptr);
static void ldv_print_primary_expr (unsigned int, ldv_primary_expr_ptr);
static void ldv_print_relational_expr (unsigned int, ldv_relational_expr_ptr);
static void ldv_print_selection_statement (unsigned int, ldv_selection_statement_ptr);
static void ldv_print_shift_expr (unsigned int, ldv_shift_expr_ptr);
static void ldv_print_spec_qual_list (unsigned int, ldv_spec_qual_list_ptr);
static void ldv_print_statement (unsigned int, ldv_statement_ptr);
static void ldv_print_statement_internal (unsigned int, ldv_statement_ptr);
static void ldv_print_storage_class_spec (unsigned int, ldv_storage_class_spec_ptr);
static void ldv_print_str_literal (unsigned int, ldv_str_literal_ptr);
static void ldv_print_struct_decl (unsigned int, ldv_struct_decl_ptr);
static void ldv_print_struct_decl_list (unsigned int, ldv_struct_decl_list_ptr);
static void ldv_print_struct_declarator (unsigned int, ldv_struct_declarator_ptr);
static void ldv_print_struct_declarator_list (unsigned int, ldv_struct_declarator_list_ptr);
static void ldv_print_struct_or_union (unsigned int, ldv_struct_or_union_ptr);
static void ldv_print_struct_or_union_spec (unsigned int, ldv_struct_or_union_spec_ptr);
static void ldv_print_type_name (unsigned int, ldv_type_name_ptr);
static void ldv_print_type_qual (unsigned int, ldv_type_qual_ptr);
static void ldv_print_type_qual_list (unsigned int, ldv_type_qual_list_ptr);
static void ldv_print_type_spec (unsigned int, ldv_type_spec_ptr);
static void ldv_print_typedef_name (unsigned int, ldv_typedef_name_ptr);
static void ldv_print_unary_expr (unsigned int, ldv_unary_expr_ptr);
static void ldv_print_unary_operator (unsigned int, ldv_unary_operator_ptr);
static void ldv_print_unsigned_suffix (unsigned int, ldv_unsigned_suffix_ptr);


static ldv_location_ptr
ldv_declarator_location (ldv_declarator_ptr declarator)
{
  ldv_direct_declarator_ptr direct_declarator;

  if ((direct_declarator = LDV_DECLARATOR_DIRECT_DECLARATOR (declarator)))
    return ldv_direct_declarator_location (direct_declarator);
  else
    LDV_ERROR ("can't find direct declarator of declarator");

  return NULL;
}

static ldv_location_ptr
ldv_direct_declarator_location (ldv_direct_declarator_ptr direct_declarator)
{
  ldv_declarator_ptr declarator;
  ldv_direct_declarator_ptr direct_declarator_next;
  ldv_location_ptr location;

  if ((declarator = LDV_DIRECT_DECLARATOR_DECLARATOR (direct_declarator)))
    return ldv_declarator_location (declarator);
  else if ((direct_declarator_next = LDV_DIRECT_DECLARATOR_DIRECT_DECLARATOR (direct_declarator)))
    return ldv_direct_declarator_location (direct_declarator_next);
  else
    {
      if ((location = LDV_DIRECT_DECLARATOR_LOCATION (direct_declarator)))
        return location;
      else
        LDV_ERROR ("can't find location of direct declarator");
    }

  return NULL;
}

static bool ldv_isglobal_var_was_printed (tree t)
{
  struct ldv_global_var_name
  {
    const char *name;
    struct ldv_global_var_name *next;
  };
  static struct ldv_global_var_name *global_var_names = NULL;
  struct ldv_global_var_name *global_var_name_cur;
  ldv_identifier_ptr identifier;

  /* Do not consider anything else except variables. Moreover take into
     account just that variables that have initializers. */
  if (TREE_CODE (t) != VAR_DECL || !DECL_INITIAL (t))
    return false;

  if ((identifier = ldv_convert_identifier (t)) && LDV_IDENTIFIER_STR (identifier))
    {
      /* If there isn't any global variable name then just push it to the
         list. */
      if (!global_var_names)
        {
          global_var_names = XCNEW (struct ldv_global_var_name);
          global_var_names->name = LDV_IDENTIFIER_STR (identifier);
        }
      /* Otherwise check whether a variable having the same name was processed.
         Keep a current one to the list. */
      else
        {
          for (global_var_name_cur = global_var_names; ; global_var_name_cur = global_var_name_cur->next)
            {
              if (!strcmp (LDV_IDENTIFIER_STR (identifier), global_var_name_cur->name))
                return true;

              if (global_var_name_cur->next)
                continue;

              break;
            }

          global_var_name_cur->next = XCNEW (struct ldv_global_var_name);
          global_var_name_cur->next->name = LDV_IDENTIFIER_STR (identifier);
        }
    }
  else
    LDV_ERROR ("can't find variable name");

  return false;
}

/*
abstract-declarator:
    pointer
    pointeropt direct-abstract-declarator
*/
static void
ldv_print_abstract_declarator (unsigned int indent_level, ldv_abstract_declarator_ptr abstract_declarator)
{
  ldv_pointer_ptr pointer;
  ldv_direct_abstract_declarator_ptr direct_abstract_declarator;

  if ((pointer = LDV_ABSTRACT_DECLARATOR_POINTER (abstract_declarator)))
    ldv_print_pointer (indent_level, pointer);

  if ((direct_abstract_declarator = LDV_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (abstract_declarator)))
    ldv_print_direct_abstract_declarator (indent_level, direct_abstract_declarator);

  if (!pointer && !direct_abstract_declarator)
    LDV_ERROR ("neither pointer nor abstract declarator direct abstract declarator was printed");

  LDV_XDELETE_ON_PRINTING (abstract_declarator);
}

/*
additive-expression:
    multiplicative-expression
    additive-expression + multiplicative-expression
    additive-expression - multiplicative-expression
*/
static void
ldv_print_additive_expr (unsigned int indent_level, ldv_additive_expr_ptr additive_expr)
{
  ldv_multiplicative_expr_ptr multiplicative_expr;
  ldv_additive_expr_ptr additive_expr_next;
  ldv_location_ptr location;

  switch (LDV_ADDITIVE_EXPR_KIND (additive_expr))
    {
    case LDV_ADDITIVE_EXPR_FIRST:
      if ((multiplicative_expr = LDV_ADDITIVE_EXPR_MULTIPLICATIVE_EXPR (additive_expr)))
        ldv_print_multiplicative_expr (indent_level, multiplicative_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "multiplicative expression of additive expression was not printed");

      break;

    case LDV_ADDITIVE_EXPR_SECOND:
    case LDV_ADDITIVE_EXPR_THIRD:

      if ((additive_expr_next = LDV_ADDITIVE_EXPR_ADDITIVE_EXPR (additive_expr)))
        ldv_print_additive_expr (indent_level, additive_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "additive expression of additive expression was not printed");

      if ((location = LDV_ADDITIVE_EXPR_LOCATION (additive_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if (LDV_ADDITIVE_EXPR_KIND (additive_expr) == LDV_ADDITIVE_EXPR_SECOND)
        ldv_c_backend_print (indent_level, true, "+");
      else
        ldv_c_backend_print (indent_level, true, "-");

      if ((multiplicative_expr = LDV_ADDITIVE_EXPR_MULTIPLICATIVE_EXPR (additive_expr)))
        ldv_print_multiplicative_expr (indent_level, multiplicative_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "multiplicative expression of additive expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "additive expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (additive_expr);
}

/*
AND-expression:
    equality-expression
    AND-expression & equality-expression
*/
static void
ldv_print_and_expr (unsigned int indent_level, ldv_and_expr_ptr and_expr)
{
  ldv_equality_expr_ptr equality_expr;
  ldv_and_expr_ptr and_expr_next;
  ldv_location_ptr location;

  switch (LDV_AND_EXPR_KIND (and_expr))
    {
    case LDV_AND_EXPR_FIRST:
      if ((equality_expr = LDV_AND_EXPR_EQUALITY_EXPR (and_expr)))
        ldv_print_equality_expr (indent_level, equality_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "equality expression of and expression was not printed");

      break;

    case LDV_AND_EXPR_SECOND:

      if ((and_expr_next = LDV_AND_EXPR_AND_EXPR (and_expr)))
        ldv_print_and_expr (indent_level, and_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "and expression of and expression was not printed");

      if ((location = LDV_AND_EXPR_LOCATION (and_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, "&");

      if ((equality_expr = LDV_AND_EXPR_EQUALITY_EXPR (and_expr)))
        ldv_print_equality_expr (indent_level, equality_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "equality expression of and expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "and expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (and_expr);
}

/*
argument-expression-list:
    assignment-expression
    argument-expression-list , assignment-expression
*/
static void
ldv_print_arg_expr_list (unsigned int indent_level, ldv_arg_expr_list_ptr arg_expr_list)
{
  ldv_assignment_expr_ptr assignment_expr;
  ldv_arg_expr_list_ptr arg_expr_list_next;

  switch (LDV_ARG_EXPR_LIST_KIND (arg_expr_list))
    {
    case LDV_ARG_EXPR_LIST_FIRST:
      if ((assignment_expr = LDV_ARG_EXPR_LIST_ASSIGNEMT_EXPR (arg_expr_list)))
        ldv_print_assignment_expr (indent_level, assignment_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression of argument expression list was not printed");

      break;

    case LDV_ARG_EXPR_LIST_SECOND:
      if ((arg_expr_list_next = LDV_ARG_EXPR_LIST_ARG_EXPR_LIST(arg_expr_list)))
        ldv_print_arg_expr_list (indent_level, arg_expr_list_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "argument expression list of argument expression list was not printed");

      ldv_c_backend_print (indent_level, true, ",");

      if ((assignment_expr = LDV_ARG_EXPR_LIST_ASSIGNEMT_EXPR (arg_expr_list)))
        ldv_print_assignment_expr (indent_level, assignment_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression of argument expression list was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "argument expression list was not printed");
    }

  LDV_XDELETE_ON_PRINTING (arg_expr_list);
}

/*
GNU extensions:

asm-argument:
    asm-string-literal
    asm-string-literal : asm-operandsopt
    asm-string-literal : asm-operandsopt : asm-operandsopt
    asm-string-literal : asm-operandsopt : asm-operandsopt : asm-clobbersopt
*/
static void
ldv_print_asm_arg (unsigned int indent_level, ldv_asm_arg_ptr asm_arg)
{
  ldv_asm_str_literal_ptr asm_str_literal;
  ldv_asm_operands_ptr asm_operands1, asm_operands2;
  ldv_asm_clobbers_ptr asm_clobbers;

  switch (LDV_ASM_ARG_KIND (asm_arg))
    {
    case LDV_ASM_ARG_FIRST:
    case LDV_ASM_ARG_SECOND:
    case LDV_ASM_ARG_THIRD:
    case LDV_ASM_ARG_FOURTH:
      if ((asm_str_literal = LDV_ASM_ARG_ASM_STR_LITERAL (asm_arg)))
        ldv_print_asm_str_literal (indent_level, asm_str_literal);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "asm string literal of asm arg was not printed");

      if (LDV_ASM_ARG_KIND (asm_arg) > LDV_ASM_ARG_FIRST)
        {
          ldv_c_backend_print (indent_level, true, ":");

          if ((asm_operands1 = LDV_ASM_ARG_ASM_OPERANDS1 (asm_arg)))
            ldv_print_asm_operands (indent_level, asm_operands1);
        }

      if (LDV_ASM_ARG_KIND (asm_arg) > LDV_ASM_ARG_SECOND)
        {
          ldv_c_backend_print (indent_level, true, ":");

          if ((asm_operands2 = LDV_ASM_ARG_ASM_OPERANDS2 (asm_arg)))
            ldv_print_asm_operands (indent_level, asm_operands2);
        }

      if (LDV_ASM_ARG_KIND (asm_arg) > LDV_ASM_ARG_THIRD)
        {
          ldv_c_backend_print (indent_level, true, ":");

          if ((asm_clobbers = LDV_ASM_ARG_ASM_CLOBBERS (asm_arg)))
            ldv_print_asm_clobbers (indent_level, asm_clobbers);
        }

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "asm arg was not printed");
    }

  LDV_XDELETE_ON_PRINTING (asm_arg);
}

/*
GNU extensions:

asm-clobbers:
    asm-string-literal
    asm-clobbers , asm-string-literal
*/
static void
ldv_print_asm_clobbers (unsigned int indent_level, ldv_asm_clobbers_ptr asm_clobbers)
{
  ldv_asm_str_literal_ptr asm_str_literal;
  ldv_asm_clobbers_ptr asm_clobbers_next;

  switch (LDV_ASM_CLOBBERS_KIND (asm_clobbers))
    {
    case LDV_ASM_CLOBBERS_FIRST:
    case LDV_ASM_CLOBBERS_SECOND:
      if (LDV_ASM_CLOBBERS_KIND (asm_clobbers) == LDV_ASM_CLOBBERS_SECOND)
        {
          if ((asm_clobbers_next = LDV_ASM_CLOBBERS_ASM_CLOBBERS (asm_clobbers)))
            ldv_print_asm_clobbers (indent_level, asm_clobbers_next);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "asm clobbers of asm clobbers was not printed");

          ldv_c_backend_print (indent_level, true, ",");
        }

      if ((asm_str_literal = LDV_ASM_CLOBBERS_ASM_STR_LITERAL (asm_clobbers)))
        ldv_print_asm_str_literal (indent_level, asm_str_literal);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "asm string literal of asm clobbers was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "asm clobbers was not printed");
    }

  LDV_XDELETE_ON_PRINTING (asm_clobbers);
}

/*
GNU extensions:

asm-goto-argument:
    asm-string-literal : : asm-operandsopt : asm-clobbersopt : asm-goto-operands
*/
static void
ldv_print_asm_goto_arg (unsigned int indent_level, ldv_asm_goto_arg_ptr asm_goto_arg)
{
  ldv_asm_str_literal_ptr asm_str_literal;
  ldv_asm_operands_ptr asm_operands;
  ldv_asm_clobbers_ptr asm_clobbers;
  ldv_asm_goto_operands_ptr asm_goto_operands;

  if ((asm_str_literal = LDV_ASM_GOTO_ARG_ASM_STR_LITERAL (asm_goto_arg)))
    ldv_print_asm_str_literal (indent_level, asm_str_literal);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "asm string literal of asm goto argument was not printed");

  ldv_c_backend_print (indent_level, true, ":");

  ldv_c_backend_print (indent_level, true, ":");

  if ((asm_operands = LDV_ASM_GOTO_ARG_ASM_OPERANDS (asm_goto_arg)))
    ldv_print_asm_operands (indent_level, asm_operands);

  ldv_c_backend_print (indent_level, true, ":");

  if ((asm_clobbers = LDV_ASM_GOTO_ARG_ASM_CLOBBERS (asm_goto_arg)))
    ldv_print_asm_clobbers (indent_level, asm_clobbers);

  ldv_c_backend_print (indent_level, true, ":");

  if ((asm_goto_operands = LDV_ASM_GOTO_ARG_ASM_GOTO_OPERANDS (asm_goto_arg)))
    ldv_print_asm_goto_operands (indent_level, asm_goto_operands);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "asm goto operands of asm goto argument was not printed");

  LDV_XDELETE_ON_PRINTING (asm_goto_arg);
}

/*
GNU extensions:

asm-goto-operands:
    identifier
    asm-goto-operands, identifier
*/
static void
ldv_print_asm_goto_operands (unsigned int indent_level, ldv_asm_goto_operands_ptr asm_goto_operands)
{
  ldv_identifier_ptr identifier;
  ldv_asm_goto_operands_ptr asm_goto_operands_next;

  switch (LDV_ASM_GOTO_OPERANDS_KIND (asm_goto_operands))
    {
    case LDV_ASM_GOTO_OPERANDS_FIRST:
    case LDV_ASM_GOTO_OPERANDS_SECOND:
      if (LDV_ASM_GOTO_OPERANDS_KIND (asm_goto_operands) == LDV_ASM_GOTO_OPERANDS_SECOND)
        {
          if ((asm_goto_operands_next = LDV_ASM_GOTO_OPERANDS_ASM_GOTO_OPERANDS (asm_goto_operands)))
            ldv_print_asm_goto_operands (indent_level, asm_goto_operands_next);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "asm goto operands of asm goto operands was not printed");

          ldv_c_backend_print (indent_level, true, ",");
        }

      if ((identifier = LDV_ASM_GOTO_OPERANDS_IDENTIFIER (asm_goto_operands)))
        ldv_print_identifier (indent_level, identifier);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of asm goto operands was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "asm goto operands was not printed");
    }

  LDV_XDELETE_ON_PRINTING (asm_goto_operands);
}

/*
GNU extensions:

asm-operand:
    asm-string-literal ( expression )
    [ identifier ] asm-string-literal ( expression )
*/
static void
ldv_print_asm_operand (unsigned int indent_level, ldv_asm_operand_ptr asm_operand)
{
  ldv_asm_str_literal_ptr asm_str_literal;
  ldv_expr_ptr expr;
  ldv_identifier_ptr identifier;

  switch (LDV_ASM_OPERAND_KIND (asm_operand))
    {
    case LDV_ASM_OPERAND_FIRST:
    case LDV_ASM_OPERAND_SECOND:
      if (LDV_ASM_OPERAND_KIND (asm_operand) == LDV_ASM_OPERAND_SECOND)
        {
          ldv_c_backend_print (indent_level, true, "[");

          if ((identifier = LDV_ASM_OPERAND_IDENTIFIER (asm_operand)))
            ldv_print_identifier (indent_level, identifier);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of asm operand was not printed");

          ldv_c_backend_print (indent_level, true, "]");
        }

      if ((asm_str_literal = LDV_ASM_OPERAND_ASM_STR_LITERAL (asm_operand)))
        ldv_print_asm_str_literal (indent_level, asm_str_literal);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "asm string literal of asm operand was not printed");

      ldv_c_backend_print (indent_level, true, "(");

      if ((expr = LDV_ASM_OPERAND_EXPR (asm_operand)))
        ldv_print_expr (indent_level, expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of asm operand was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "asm operand was not printed");
    }

  LDV_XDELETE_ON_PRINTING (asm_operand);
}

/*
GNU extensions:

asm-operands:
    asm-operand
    asm-operands , asm-operand
*/
static void
ldv_print_asm_operands (unsigned int indent_level, ldv_asm_operands_ptr asm_operands)
{
  ldv_asm_operand_ptr asm_operand;
  ldv_asm_operands_ptr asm_operands_next;

  switch (LDV_ASM_OPERANDS_KIND (asm_operands))
    {
    case LDV_ASM_OPERANDS_FIRST:
    case LDV_ASM_OPERANDS_SECOND:
      if (LDV_ASM_OPERANDS_KIND (asm_operands) == LDV_ASM_OPERANDS_SECOND)
        {
          if ((asm_operands_next = LDV_ASM_OPERANDS_ASM_OPERANDS (asm_operands)))
            ldv_print_asm_operands (indent_level, asm_operands_next);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "asm operands of asm operands was not printed");

          ldv_c_backend_print (indent_level, true, ",");
        }

      if ((asm_operand = LDV_ASM_OPERANDS_ASM_OPERAND (asm_operands)))
        ldv_print_asm_operand (indent_level, asm_operand);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "asm operand of asm operands was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "asm operands was not printed");
    }

  LDV_XDELETE_ON_PRINTING (asm_operands);
}

/*
GNU extensions:

asm-statement:
    asm type-qualifieropt ( asm-argument ) ;
    asm type-qualifieropt goto ( asm-goto-argument ) ;
*/
static void
ldv_print_asm_statement (unsigned int indent_level, ldv_asm_statement_ptr asm_statement)
{
  ldv_type_qual_ptr type_qual;
  ldv_asm_arg_ptr asm_arg;
  ldv_asm_goto_arg_ptr asm_goto_arg;
  ldv_location_ptr location;

  switch (LDV_ASM_STATEMENT_KIND (asm_statement))
    {
    case LDV_ASM_STATEMENT_FIRST:
    case LDV_ASM_STATEMENT_SECOND:
      if ((location = LDV_ASM_STATEMENT_LOCATION (asm_statement)))
        ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);

      ldv_c_backend_print (indent_level, true, "asm");

      if ((type_qual = LDV_ASM_STATEMENT_TYPE_QUAL (asm_statement)))
        ldv_print_type_qual (indent_level, type_qual);

      if (LDV_ASM_STATEMENT_KIND (asm_statement) == LDV_ASM_STATEMENT_SECOND)
        ldv_c_backend_print (indent_level, true, "goto");

      ldv_c_backend_print (indent_level, true, "(");

      if (LDV_ASM_STATEMENT_KIND (asm_statement) == LDV_ASM_STATEMENT_FIRST)
        {
          if ((asm_arg = LDV_ASM_STATEMENT_ASM_ARG (asm_statement)))
            ldv_print_asm_arg (indent_level, asm_arg);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "asm argument of asm statement was not printed");
        }
      else
        {
          if ((asm_goto_arg = LDV_ASM_STATEMENT_ASM_GOTO_ARG (asm_statement)))
            ldv_print_asm_goto_arg (indent_level, asm_goto_arg);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "asm goto argument of asm statement was not printed");
        }

      ldv_c_backend_print (indent_level, true, ")");

      ldv_c_backend_print (indent_level, true, ";");

      break;

    case LDV_ASM_STATEMENT_STUB:
      ldv_c_backend_print (indent_level, true, "ldv_inline_asm();");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "asm statement was not printed");
    }

  LDV_XDELETE_ON_PRINTING (asm_statement);
}

/*
GNU extensions:

asm-string-literal:
    string-literal
*/
static void
ldv_print_asm_str_literal (unsigned int indent_level, ldv_asm_str_literal_ptr asm_str_literal)
{
  ldv_str_literal_ptr str_literal;

  if ((str_literal = LDV_ASM_STR_LITERAL_STR_LITERAL (asm_str_literal)))
    ldv_print_str_literal (indent_level, str_literal);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "string literal of asm string literal was not printed");

  LDV_XDELETE_ON_PRINTING (asm_str_literal);
}

/*
assignment-expression:
    conditional-expression
    unary-expression assignment-operator assignment-expression
*/
static void
ldv_print_assignment_expr (unsigned int indent_level, ldv_assignment_expr_ptr assignment_expr)
{
  ldv_cond_expr_ptr cond_expr;
  ldv_unary_expr_ptr unary_expr;
  ldv_assignment_operator_ptr assignment_operator;
  ldv_assignment_expr_ptr assignment_expr_next;
  ldv_location_ptr location;

  switch (LDV_ASSIGNMENT_EXPR_KIND (assignment_expr))
    {
    case LDV_ASSIGNMENT_EXPR_FIRST:
      if ((cond_expr = LDV_ASSIGNMENT_EXPR_COND_EXPR (assignment_expr)))
        ldv_print_cond_expr (indent_level, cond_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression conditional expression was not printed");

      break;

    case LDV_ASSIGNMENT_EXPR_SECOND:
      if ((unary_expr = LDV_ASSIGNMENT_EXPR_UNARY_EXPR (assignment_expr)))
        ldv_print_unary_expr (indent_level, unary_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "unary expression of assignment expression was not printed");

      if ((location = LDV_ASSIGNMENT_EXPR_LOCATION (assignment_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if ((assignment_operator = LDV_ASSIGNMENT_EXPR_ASSIGNMENT_OPERATOR (assignment_expr)))
        ldv_print_assignment_operator (indent_level, assignment_operator);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment operator of assignment expression was not printed");

      if ((assignment_expr_next = LDV_ASSIGNMENT_EXPR_ASSIGNMENT_EXPR (assignment_expr)))
        ldv_print_assignment_expr (indent_level, assignment_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression of assignment expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (assignment_expr);
}

/*
assignment-operator: one of
    = *= /= %= += -= <<= >>= &= ^= |=
*/
static void
ldv_print_assignment_operator (unsigned int indent_level, ldv_assignment_operator_ptr assignment_operator)
{
  switch (LDV_ASSIGNMENT_OPERATOR_KIND (assignment_operator))
    {
    case LDV_ASSIGNMENT_OPERATOR_FIRST:
      ldv_c_backend_print (indent_level, true, "=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_SECOND:
      ldv_c_backend_print (indent_level, true, "*=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_THIRD:
      ldv_c_backend_print (indent_level, true, "/=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_FOURTH:
      ldv_c_backend_print (indent_level, true, "%%=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_FIVTH:
      ldv_c_backend_print (indent_level, true, "+=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_SIXTH:
      ldv_c_backend_print (indent_level, true, "-=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_SEVENTH:
      ldv_c_backend_print (indent_level, true, "<<=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_EIGHTH:
      ldv_c_backend_print (indent_level, true, ">>=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_NINTH:
      ldv_c_backend_print (indent_level, true, "&=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_TENTH:
      ldv_c_backend_print (indent_level, true, "^=");
      break;

    case LDV_ASSIGNMENT_OPERATOR_ELEVEN:
      ldv_c_backend_print (indent_level, true, "|=");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "assignment operator was not printed");
    }

  LDV_XDELETE_ON_PRINTING (assignment_operator);
}

/*
block-item:
    declaration
    statement

GNU extensions:

block-item:
    declaration -> nested-declaration
*/
static void
ldv_print_block_item (unsigned int indent_level, ldv_block_item_ptr block_item)
{
  ldv_nested_decl_ptr nested_decl;
  ldv_statement_ptr statement;

  switch (LDV_BLOCK_ITEM_KIND (block_item))
    {
    case LDV_BLOCK_ITEM_FIRST:
      if ((nested_decl = LDV_BLOCK_ITEM_NESTED_DECL (block_item)))
        ldv_print_nested_decl (indent_level, nested_decl);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "nested declaration of block item was not printed");

      break;

    case LDV_BLOCK_ITEM_SECOND:
      if ((statement = LDV_BLOCK_ITEM_STATEMENT (block_item)))
        ldv_print_statement (indent_level, statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "statement of block item was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "block item was not printed");
    }

  LDV_XDELETE_ON_PRINTING (block_item);
}

/*
block-item-list:
    block-item
    block-item-list block-item
*/
static void
ldv_print_block_item_list (unsigned int indent_level, ldv_block_item_list_ptr block_item_list)
{
  ldv_block_item_list_ptr block_item_list_next;
  ldv_block_item_ptr block_item;

  if ((block_item_list_next = LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST (block_item_list)))
    {
      ldv_print_block_item_list (indent_level, block_item_list_next);
      ldv_c_backend_print (indent_level, false, "\n");
    }

  if ((block_item = LDV_BLOCK_ITEM_LIST_BLOCK_ITEM (block_item_list)))
    ldv_print_block_item (indent_level, block_item);
  else
    LDV_ERROR ("block item wasn't printed");

  LDV_XDELETE_ON_PRINTING (block_item_list);
}

/*
cast-expression:
    unary-expression
    ( type-name ) cast-expression
*/
static void
ldv_print_cast_expr (unsigned int indent_level, ldv_cast_expr_ptr cast_expr)
{
  ldv_unary_expr_ptr unary_expr;
  ldv_type_name_ptr type_name;
  ldv_cast_expr_ptr cast_expr_next;
  ldv_location_ptr location;
  bool is_print_disabled = false;

  switch (LDV_CAST_EXPR_KIND (cast_expr))
    {
    case LDV_CAST_EXPR_FIRST:
      if ((unary_expr = LDV_CAST_EXPR_UNARY_EXPR (cast_expr)))
        ldv_print_unary_expr (indent_level, unary_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "unary expression of cast expression was not printed");

      break;

    case LDV_CAST_EXPR_SECOND:
      if ((location = LDV_CAST_EXPR_LOCATION (cast_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      /* See comment for this variable. */
      if (ldv_disable_cast_printing && !ldv_c_backend_printing_disabled)
        {
          ldv_c_backend_printing_disabled = true;
          is_print_disabled = true;
        }

      ldv_c_backend_print (indent_level, true, "(");

      if ((type_name = LDV_CAST_EXPR_TYPE_NAME (cast_expr)))
        ldv_print_type_name (indent_level, type_name);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "type name of cast expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      /* To avoid enabling of printing in nested casts enable it just
         there where it was disabled for the first time. */
      if (is_print_disabled)
        ldv_c_backend_printing_disabled = false;

      if ((cast_expr_next = LDV_CAST_EXPR_CAST_EXPR (cast_expr)))
        ldv_print_cast_expr (indent_level, cast_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "cast expression of cast expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "cast expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (cast_expr);
}

/*
compound-statement:
    { block-item-listopt }

GNU extensions:

compound-statement:
    { label-declarations block-item-list }
*/
static void
ldv_print_compound_statement (unsigned int indent_level, ldv_compound_statement_ptr compound_statement)
{
  ldv_block_item_list_ptr block_item_list;
  ldv_label_decls_ptr label_decls;

  ldv_c_backend_print (indent_level, false, "{\n");

  if ((label_decls = LDV_COMPOUND_STATEMENT_LABEL_DECLS (compound_statement)))
    {
      ldv_print_label_decls (indent_level + 1, label_decls);
      ldv_c_backend_print (indent_level, false, "\n");
    }

  if ((block_item_list = LDV_COMPOUND_STATEMENT_BLOCK_ITEM_LIST (compound_statement)))
    {
      ldv_print_block_item_list (indent_level + 1, block_item_list);
      ldv_c_backend_print (indent_level, false, "\n");
    }

  ldv_c_backend_print (indent_level, true, "}");

  LDV_XDELETE_ON_PRINTING (compound_statement);
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
static void
ldv_print_cond_expr (unsigned int indent_level, ldv_cond_expr_ptr cond_expr)
{
  ldv_logical_or_expr_ptr logical_or_expr;
  ldv_expr_ptr expr;
  ldv_cond_expr_ptr cond_expr_next;
  ldv_location_ptr location;

  switch (LDV_COND_EXPR_KIND (cond_expr))
    {
    case LDV_COND_EXPR_FIRST:
      if ((logical_or_expr = LDV_COND_EXPR_LOGICAL_OR_EXPR (cond_expr)))
        ldv_print_logical_or_expr (indent_level, logical_or_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "logical or expression of conditional expression was not printed");

      break;

    case LDV_COND_EXPR_SECOND:
    case LDV_COND_EXPR_THIRD:
      if ((logical_or_expr = LDV_COND_EXPR_LOGICAL_OR_EXPR (cond_expr)))
        ldv_print_logical_or_expr (indent_level, logical_or_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "logical or expression of conditional expression was not printed");

      ldv_c_backend_print (indent_level, true, "?");

      if ((expr = LDV_COND_EXPR_EXPR (cond_expr)))
        ldv_print_expr (indent_level, expr);
      else if (LDV_COND_EXPR_KIND (cond_expr) != LDV_COND_EXPR_THIRD)
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of conditional expression was not printed");

      if ((location = LDV_COND_EXPR_LOCATION (cond_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, ":");

      if ((cond_expr_next = LDV_COND_EXPR_COND_EXPR (cond_expr)))
        ldv_print_cond_expr (indent_level, cond_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "conditional expression of conditional expression was not printed");

      break;

    case LDV_COND_EXPR_FOURTH:
    case LDV_COND_EXPR_FIFTH:
      ldv_c_backend_print (indent_level, true, "(");

      /* Do not free memory for expressions since they are reused below. */
      ldv_free_on_printing = false;

      if ((expr = LDV_COND_EXPR_EXPR (cond_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_expr (indent_level, expr);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of conditional expression was not printed");

      if (LDV_COND_EXPR_KIND (cond_expr) == LDV_COND_EXPR_FOURTH)
        ldv_c_backend_print (indent_level, true, "<");
      else
        ldv_c_backend_print (indent_level, true, ">");

      if ((cond_expr_next = LDV_COND_EXPR_COND_EXPR (cond_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_cond_expr (indent_level, cond_expr_next);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "conditional expression of conditional expression was not printed");

      ldv_c_backend_print (indent_level, true, "?");

      ldv_free_on_printing = true;

      if ((expr = LDV_COND_EXPR_EXPR (cond_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_expr (indent_level, expr);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of conditional expression was not printed");

      ldv_c_backend_print (indent_level, true, ":");

      if ((cond_expr_next = LDV_COND_EXPR_COND_EXPR (cond_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_cond_expr (indent_level, cond_expr_next);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "conditional expression of conditional expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      break;

    case LDV_COND_EXPR_SIXTH:
      if ((location = LDV_COND_EXPR_LOCATION (cond_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, "(");

      /* Do not free memory for expressions since they are reused below. */
      ldv_free_on_printing = false;

      if ((expr = LDV_COND_EXPR_EXPR (cond_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_expr (indent_level, expr);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of conditional expression was not printed");

      ldv_c_backend_print (indent_level, true, "< 0 ?");

      if ((expr = LDV_COND_EXPR_EXPR (cond_expr)))
        {
          ldv_c_backend_print (indent_level, true, "-(");
          ldv_print_expr (indent_level, expr);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of conditional expression was not printed");

      ldv_c_backend_print (indent_level, true, ":");

      ldv_free_on_printing = true;

      if ((expr = LDV_COND_EXPR_EXPR (cond_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_expr (indent_level, expr);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of conditional expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "conditional expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (cond_expr);
}

/*
constant-expression:
    conditional-expression
*/
static void
ldv_print_const_expr (unsigned int indent_level, ldv_const_expr_ptr const_expr)
{
  ldv_cond_expr_ptr cond_expr;

  if ((cond_expr = LDV_CONST_EXPR_COND_EXPR (const_expr)))
    ldv_print_cond_expr (indent_level, cond_expr);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "conditional expression of constant expression was not printed");

  LDV_XDELETE_ON_PRINTING (const_expr);
}

/*
constant:
    integer-constant
    floating-constant
    enumeration-constant
    character-constant
*/
static void
ldv_print_constant (unsigned int indent_level, ldv_constant_ptr constant)
{
  ldv_integer_constant_ptr integer_constant;
  ldv_floating_constant_ptr floating_constant;
  ldv_enumeration_constant_ptr enumeration_constant;

  switch (LDV_CONSTANT_KIND (constant))
    {
    case LDV_CONSTANT_FIRST:
      if ((integer_constant = LDV_CONSTANT_INTEGER_CONSTANT (constant)))
        ldv_print_integer_constant (indent_level, integer_constant);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "integer constant of constant was not printed");

      break;

    case LDV_CONSTANT_SECOND:
      if ((floating_constant = LDV_CONSTANT_FLOATING_CONSTANT (constant)))
        ldv_print_floating_constant (indent_level, floating_constant);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "floating constant of constant was not printed");

      break;

    case LDV_CONSTANT_THIRD:
      if ((enumeration_constant = LDV_CONSTANT_ENUMERATION_CONSTANT (constant)))
        ldv_print_enumeration_constant (indent_level, enumeration_constant);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "enumeration constant of constant was not printed");

      break;

    case LDV_CONSTANT_FOURTH:

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "constant was not printed");
    }

  LDV_XDELETE_ON_PRINTING (constant);
}

/*
declaration:
    declaration-specifiers init-declarator-listopt ;
*/
static void
ldv_print_decl (unsigned int indent_level, ldv_decl_ptr decl)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_init_declarator_list_ptr init_declarator_list;
  ldv_init_declarator_ptr init_declarator;
  ldv_declarator_ptr declarator;
  ldv_location_ptr location;

  /* Print #line directive before variable declaration. */
  if ((init_declarator_list = LDV_DECL_INIT_DECLARATOR_LIST (decl)))
    {
      if ((init_declarator = LDV_INIT_DECLARATOR_LIST_INIT_DECLARATOR (init_declarator_list))
        && (declarator = LDV_INIT_DECLARATOR_DECLARATOR (init_declarator))
        && (location = ldv_declarator_location (declarator)))
        ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_DECL, location);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "line directive for variable declaration was not printed");
    }

  if ((decl_spec = LDV_DECL_DECL_SPEC (decl)))
    ldv_print_decl_spec (indent_level, decl_spec);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "declaration specifiers of declaration were not printed");

  if ((init_declarator_list = LDV_DECL_INIT_DECLARATOR_LIST (decl)))
    ldv_print_init_declarator_list (indent_level, init_declarator_list);

  ldv_c_backend_print (indent_level, false, ";");

  LDV_XDELETE_ON_PRINTING (decl);
}

/*
declaration-specifiers:
    storage-class-specifier declaration-specifiersopt
    type-specifier declaration-specifiersopt
    type-qualifier declaration-specifiersopt
    function-specifier declaration-specifiersopt
*/
static void
ldv_print_decl_spec (unsigned int indent_level, ldv_decl_spec_ptr decl_spec)
{
  ldv_storage_class_spec_ptr storage_class_spec;
  ldv_type_spec_ptr type_spec;
  ldv_type_qual_ptr type_qual;
  ldv_func_spec_ptr func_spec;
  ldv_decl_spec_ptr decl_spec_next;

  if ((storage_class_spec = LDV_DECL_SPEC_STORAGE_CLASS_SPEC (decl_spec)))
    ldv_print_storage_class_spec (indent_level, storage_class_spec);
  else if ((type_spec = LDV_DECL_SPEC_TYPE_SPEC (decl_spec)))
    ldv_print_type_spec (indent_level, type_spec);
  else if ((type_qual = LDV_DECL_SPEC_TYPE_QUAL (decl_spec)))
    ldv_print_type_qual (indent_level, type_qual);
  else if ((func_spec = LDV_DECL_SPEC_FUNC_SPEC (decl_spec)))
    ldv_print_func_spec (indent_level, func_spec);
  else
    LDV_ERROR ("neither storage class specifier nor type specifier nor type qualifier nor function specifier was printed");

  if ((decl_spec_next = LDV_DECL_SPEC_DECL_SPEC (decl_spec)))
    ldv_print_decl_spec (indent_level, decl_spec_next);

  LDV_XDELETE_ON_PRINTING (decl_spec);
}

/*
declarator:
   pointeropt direct-declarator
*/
static void
ldv_print_declarator (unsigned int indent_level, ldv_declarator_ptr declarator)
{
  ldv_pointer_ptr pointer;
  ldv_direct_declarator_ptr direct_declarator;

  if ((pointer = LDV_DECLARATOR_POINTER (declarator)))
    ldv_print_pointer (indent_level, pointer);

  if ((direct_declarator = LDV_DECLARATOR_DIRECT_DECLARATOR (declarator)))
    ldv_print_direct_declarator (indent_level, direct_declarator);
  else
    LDV_ERROR ("declarator direct declarator wasn't printed");

  LDV_XDELETE_ON_PRINTING (declarator);
}

/*
designation:
    designator-list =
*/
static void
ldv_print_designation (unsigned int indent_level, ldv_designation_ptr designation)
{
  ldv_designator_list_ptr designator_list;

  if ((designator_list = LDV_DESIGNATION_DESIGNATOR_LIST (designation)))
    ldv_print_designator_list (indent_level, designator_list);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "designator list of designation was not printed");

  ldv_c_backend_print (indent_level, true, "=");

  LDV_XDELETE_ON_PRINTING (designation);
}

/*
designator:
    [ constant-expression ]
    . identifier
*/
static void
ldv_print_designator (unsigned int indent_level, ldv_designator_ptr designator)
{
  ldv_const_expr_ptr const_expr;
  ldv_identifier_ptr identifier;

  switch (LDV_DESIGNATOR_KIND (designator))
    {
    case LDV_DESIGNATOR_FIRST:
      ldv_c_backend_print (indent_level, true, "[");

      if ((const_expr = LDV_DESIGNATOR_CONST_EXPR (designator)))
        ldv_print_const_expr (indent_level, const_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "constant expression of designator was not printed");

      ldv_c_backend_print (indent_level, true, "]");

      break;

    case LDV_DESIGNATOR_SECOND:
      ldv_c_backend_print (indent_level, true, ".");

      if ((identifier = LDV_DESIGNATOR_IDENTIFIER (designator)))
        ldv_print_identifier (indent_level, identifier);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of designator was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "designator was not printed");
    }

  LDV_XDELETE_ON_PRINTING (designator);
}

/*
designator-list:
    designator
    designator-list designator
*/
static void
ldv_print_designator_list (unsigned int indent_level, ldv_designator_list_ptr designator_list)
{
  ldv_designator_ptr designator;
  ldv_designator_list_ptr designator_list_next;

  switch (LDV_DESIGNATOR_LIST_KIND (designator_list))
    {
    case LDV_DESIGNATOR_LIST_FIRST:
    case LDV_DESIGNATOR_LIST_SECOND:
      if (LDV_DESIGNATOR_LIST_KIND (designator_list) == LDV_DESIGNATOR_LIST_SECOND)
        {
          if ((designator_list_next = LDV_DESIGNATOR_LIST_DESIGNATOR_LIST(designator_list)))
            ldv_print_designator_list (indent_level, designator_list_next);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "designator list of designator list was not printed");

          ldv_c_backend_print (indent_level, true, ",");
        }

      if ((designator = LDV_DESIGNATOR_LIST_DESIGNATOR (designator_list)))
        ldv_print_designator (indent_level, designator);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "designator of designator list was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "designator list was not printed");
    }

  LDV_XDELETE_ON_PRINTING (designator_list);
}

/*
direct-abstract-declarator:
    ( abstract-declarator )
    direct-abstract-declaratoropt [ assignment-expressionopt ]
    direct-abstract-declaratoropt [ * ]
    direct-abstract-declaratoropt ( parameter-type-listopt )
*/
static void
ldv_print_direct_abstract_declarator (unsigned int indent_level, ldv_direct_abstract_declarator_ptr direct_abstract_declarator)
{
  ldv_abstract_declarator_ptr abstract_declarator;
  ldv_direct_abstract_declarator_ptr direct_abstract_declarator_next;
  ldv_param_type_list_ptr param_type_list;
  ldv_assignment_expr_ptr assignment_expr;

  switch (LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator))
    {
    case LDV_DIRECT_ABSTRACT_DECLARATOR_FIRST:
      ldv_c_backend_print (indent_level, true, "(");
      ldv_c_backend_padding_cancel ();

      if ((abstract_declarator = LDV_DIRECT_ABSTRACT_DECLARATOR_ABSTRACT_DECLARATOR (direct_abstract_declarator)))
        ldv_print_abstract_declarator (indent_level, abstract_declarator);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "abstract declarator of direct abstract declarator was not printed");

      ldv_c_backend_print (indent_level, false, ")");

      break;

    case LDV_DIRECT_ABSTRACT_DECLARATOR_SECOND:
    case LDV_DIRECT_ABSTRACT_DECLARATOR_THIRD:
    case LDV_DIRECT_ABSTRACT_DECLARATOR_FOURTH:
      if ((direct_abstract_declarator_next = LDV_DIRECT_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR (direct_abstract_declarator)))
        ldv_print_direct_abstract_declarator (indent_level, direct_abstract_declarator_next);

      if (LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator) == LDV_DIRECT_ABSTRACT_DECLARATOR_SECOND
        || LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator) == LDV_DIRECT_ABSTRACT_DECLARATOR_THIRD)
        {
          ldv_c_backend_print (indent_level, false, "[");

          if (LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator) == LDV_DIRECT_ABSTRACT_DECLARATOR_SECOND)
            {
              if ((assignment_expr = LDV_DIRECT_ABSTRACT_DECLARATOR_ASSIGN_EXPR (direct_abstract_declarator)))
                ldv_print_assignment_expr (indent_level, assignment_expr);
            }
          else
            LDV_ERROR ("something strange");

          ldv_c_backend_print (indent_level, false, "]");
        }
      else if (LDV_DIRECT_ABSTRACT_DECLARATOR_KIND (direct_abstract_declarator) == LDV_DIRECT_ABSTRACT_DECLARATOR_FOURTH)
        {
          ldv_c_backend_print (indent_level, false, "(");

          if ((param_type_list = LDV_DIRECT_ABSTRACT_DECLARATOR_PARAM_TYPE_LIST (direct_abstract_declarator)))
            ldv_print_param_type_list (indent_level, param_type_list);

          ldv_c_backend_print (indent_level, false, ")");
        }
      else
        LDV_ERROR ("something strange");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "direct abstract declarator was not printed");
    }

  LDV_XDELETE_ON_PRINTING (direct_abstract_declarator);
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
ldv_print_direct_declarator (unsigned int indent_level, ldv_direct_declarator_ptr direct_declarator)
{
  ldv_identifier_ptr identifier;
  ldv_declarator_ptr declarator;
  ldv_direct_declarator_ptr direct_declarator_next;
  ldv_assignment_expr_ptr assignment_expr;
  ldv_param_type_list_ptr param_type_list;

  switch (LDV_DIRECT_DECLARATOR_KIND (direct_declarator))
    {
    case LDV_DIRECT_DECLARATOR_FIRST:
      if ((identifier = LDV_DIRECT_DECLARATOR_IDENTIFIER (direct_declarator)))
        ldv_print_identifier (indent_level, identifier);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of direct declarator was not printed");

      break;

    case LDV_DIRECT_DECLARATOR_SECOND:
      ldv_c_backend_print (indent_level, true, "(");
      ldv_c_backend_padding_cancel ();

      if ((declarator = LDV_DIRECT_DECLARATOR_DECLARATOR (direct_declarator)))
        ldv_print_declarator (indent_level, declarator);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "declarator of direct declarator was not printed");

      ldv_c_backend_print (indent_level, false, ")");

      break;

    case LDV_DIRECT_DECLARATOR_THIRD:
    case LDV_DIRECT_DECLARATOR_FOURTH:
    case LDV_DIRECT_DECLARATOR_FIFTH:
    case LDV_DIRECT_DECLARATOR_SIXTH:
    case LDV_DIRECT_DECLARATOR_SEVENTH:
    case LDV_DIRECT_DECLARATOR_EIGHT:
      if ((direct_declarator_next = LDV_DIRECT_DECLARATOR_DIRECT_DECLARATOR (direct_declarator)))
        ldv_print_direct_declarator (indent_level, direct_declarator_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "direct declarator of direct declarator was not printed");

      if (LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_THIRD
        || LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_FOURTH
        || LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_FIFTH
        || LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_SIXTH)
        {
          ldv_c_backend_print (indent_level, false, "[");

          if (LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_THIRD)
            {
              if ((assignment_expr = LDV_DIRECT_DECLARATOR_ASSIGNMENT_EXPR (direct_declarator)))
                ldv_print_assignment_expr (indent_level, assignment_expr);
            }
          else
            LDV_ERROR ("something strange");

          ldv_c_backend_print (indent_level, false, "]");
        }
      else if (LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_SEVENTH
        || LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_EIGHT)
        {
          ldv_c_backend_print (indent_level, false, "(");

          /* See comments in ldv-convert.c. */
          if (LDV_DIRECT_DECLARATOR_KIND (direct_declarator) == LDV_DIRECT_DECLARATOR_SEVENTH)
            {
              if ((param_type_list = LDV_DIRECT_DECLARATOR_PARAM_TYPE_LIST (direct_declarator)))
                ldv_print_param_type_list (indent_level, param_type_list);
              else
                LDV_PRETTY_PRINT_ERROR (indent_level, "parameter type list of direct declarator was not printed");
            }

          ldv_c_backend_print (indent_level, false, ")");
        }
      else
        LDV_ERROR ("something strange");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "direct declarator was not printed");
    }

  LDV_XDELETE_ON_PRINTING (direct_declarator);
}

/*
enumerator:
    enumeration-constant
    enumeration-constant = constant-expression
*/
static void
ldv_print_enum (unsigned int indent_level, ldv_enum_ptr ldv_enum)
{
  ldv_enum_const_ptr enum_const;
  ldv_const_expr_ptr const_expr;

  switch (LDV_ENUM_KIND (ldv_enum))
    {
    case LDV_ENUM_FIRST:
    case LDV_ENUM_SECOND:
      if ((enum_const = LDV_ENUM_ENUM_CONST (ldv_enum)))
        ldv_print_enum_const (indent_level, enum_const);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "enumeration constant of enumerator was not printed");

      if (LDV_ENUM_KIND (ldv_enum) == LDV_ENUM_SECOND)
        {
          ldv_c_backend_print (indent_level, true, "=");

          if ((const_expr = LDV_ENUM_CONST_EXPR (ldv_enum)))
            ldv_print_const_expr (indent_level, const_expr);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "constant expression of enumerator was not printed");
        }

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "enumerator was not printed");
    }

  LDV_XDELETE_ON_PRINTING (ldv_enum);
}

/*
enumeration-constant:
    identifier
*/
static void
ldv_print_enum_const (unsigned int indent_level, ldv_enum_const_ptr enum_const)
{
  const char *id;

  if ((id = LDV_ENUM_CONST_ID (enum_const)))
    ldv_c_backend_print (indent_level, true, "%s", id);
  else
    LDV_ERROR ("enumeration constant name wasn't printed");

  LDV_XDELETE_ON_PRINTING (enum_const);
}

/*
enumerator-list:
    enumerator
    enumerator-list , enumerator
*/
static void
ldv_print_enum_list (unsigned int indent_level, ldv_enum_list_ptr enum_list)
{
  ldv_enum_list_ptr enum_list_next;
  ldv_enum_ptr ldv_enum;

  if ((enum_list_next = LDV_ENUM_LIST_ENUM_LIST (enum_list)))
    {
      ldv_print_enum_list (indent_level, enum_list_next);
      ldv_c_backend_print (indent_level, false, ",\n");
    }

  if ((ldv_enum = LDV_ENUM_LIST_ENUM (enum_list)))
    ldv_print_enum (indent_level, ldv_enum);
  else
    LDV_ERROR ("enumerator wasn't printed");

  LDV_XDELETE_ON_PRINTING (enum_list);
}

/*
enum-specifier:
    enum identifieropt { enumerator-list }
    enum identifieropt { enumerator-list , }
    enum identifier
*/
static void
ldv_print_enum_spec (unsigned int indent_level, ldv_enum_spec_ptr enum_spec)
{
  ldv_identifier_ptr identifier;
  ldv_enum_list_ptr enum_list;

  ldv_c_backend_print (indent_level, true, "enum");

  if ((identifier = LDV_ENUM_SPEC_IDENTIFIER (enum_spec)))
    ldv_print_identifier (indent_level, identifier);

  if ((enum_list = LDV_ENUM_SPEC_ENUM_LIST (enum_spec)))
    {
      ldv_c_backend_print (indent_level, false, "\n");
      ldv_c_backend_print (indent_level, false, "{\n");
      ldv_print_enum_list (indent_level + 1, enum_list);
      ldv_c_backend_print (indent_level, false, "\n");
      ldv_c_backend_print (indent_level, true, "}");
    }

  if (!identifier && !enum_list)
    LDV_ERROR ("neither enum name nor enumerator list was printed");

  LDV_XDELETE_ON_PRINTING (enum_spec);
}

/*
enumeration-constant:
    identiﬁer
*/
static void
ldv_print_enumeration_constant (unsigned int indent_level, ldv_enumeration_constant_ptr enumeration_constant)
{
  ldv_identifier_ptr identifier;

  if ((identifier = LDV_ENUMERATION_CONSTANT_IDENTIFIER (enumeration_constant)))
    ldv_print_identifier (indent_level, identifier);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of enumeration constant was not printed");

  LDV_XDELETE_ON_PRINTING (enumeration_constant);
}

/*
equality-expression:
    relational-expression
    equality-expression == relational-expression
    equality-expression != relational-expression
*/
static void
ldv_print_equality_expr (unsigned int indent_level, ldv_equality_expr_ptr equality_expr)
{
  ldv_relational_expr_ptr relational_expr;
  ldv_equality_expr_ptr equality_expr_next;
  ldv_location_ptr location;

  switch (LDV_EQUALITY_EXPR_KIND (equality_expr))
    {
    case LDV_EQUALITY_EXPR_FIRST:
      if ((relational_expr = LDV_EQUALITY_EXPR_RELATIONAL_EXPR (equality_expr)))
        ldv_print_relational_expr (indent_level, relational_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "relational expression of equality expression was not printed");

      break;

    case LDV_EQUALITY_EXPR_SECOND:
    case LDV_EQUALITY_EXPR_THIRD:

      if ((equality_expr_next = LDV_EQUALITY_EXPR_EQUALITY_EXPR (equality_expr)))
        ldv_print_equality_expr (indent_level, equality_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "equality expression of equality expression was not printed");

      if ((location = LDV_EQUALITY_EXPR_LOCATION (equality_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if (LDV_EQUALITY_EXPR_KIND (equality_expr) == LDV_EQUALITY_EXPR_SECOND)
        ldv_c_backend_print (indent_level, true, "==");
      else
        ldv_c_backend_print (indent_level, true, "!=");

      if ((relational_expr = LDV_EQUALITY_EXPR_RELATIONAL_EXPR (equality_expr)))
        ldv_print_relational_expr (indent_level, relational_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "relational expression of equality expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "equality expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (equality_expr);
}

/*
exclusive-OR-expression:
    AND-expression
    exclusive-OR-expression ^ AND-expression
*/
static void
ldv_print_exclusive_or_expr (unsigned int indent_level, ldv_exclusive_or_expr_ptr exclusive_or_expr)
{
  ldv_and_expr_ptr and_expr;
  ldv_exclusive_or_expr_ptr exclusive_or_expr_next;
  ldv_location_ptr location;

  switch (LDV_EXCLUSIVE_OR_EXPR_KIND (exclusive_or_expr))
    {
    case LDV_EXCLUSIVE_OR_EXPR_FIRST:
      if ((and_expr = LDV_EXCLUSIVE_OR_EXPR_AND_EXPR (exclusive_or_expr)))
        ldv_print_and_expr (indent_level, and_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "and expression of exclusive or expression was not printed");

      break;

    case LDV_EXCLUSIVE_OR_EXPR_SECOND:

      if ((exclusive_or_expr_next = LDV_EXCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR (exclusive_or_expr)))
        ldv_print_exclusive_or_expr (indent_level, exclusive_or_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "exclusive or expression of exclusive or expression was not printed");

      if ((location = LDV_EXCLUSIVE_OR_EXPR_LOCATION (exclusive_or_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, "^");

      if ((and_expr = LDV_EXCLUSIVE_OR_EXPR_AND_EXPR (exclusive_or_expr)))
        ldv_print_and_expr (indent_level, and_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "and expression of exclusive or expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "exclusive or expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (exclusive_or_expr);
}

/*
expression:
    assignment-expression
    expression , assignment-expression
*/
static void
ldv_print_expr (unsigned int indent_level, ldv_expr_ptr expr)
{
  ldv_assignment_expr_ptr assignment_expr;
  ldv_expr_ptr expr_next;

  switch (LDV_EXPR_KIND (expr))
    {
    case LDV_EXPR_FIRST:
    case LDV_EXPR_SECOND:
      if (LDV_EXPR_KIND (expr) == LDV_EXPR_SECOND)
        {
          if ((expr_next = LDV_EXPR_EXPR (expr)))
            {
              ldv_print_expr (indent_level, expr_next);
              ldv_c_backend_print (indent_level, false, ",");
              ldv_c_backend_padding_force ();
            }
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "expression of expression was not printed");
        }

      if ((assignment_expr = LDV_EXPR_ASSIGNMENT_EXPR (expr)))
        ldv_print_assignment_expr (indent_level, assignment_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression of expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (expr);
}

/*
expression-statement:
    expressionopt ;
*/
static void
ldv_print_expr_statement (unsigned int indent_level, ldv_expr_statement_ptr expr_statement)
{
  ldv_expr_ptr expr;
  ldv_location_ptr location;

  if ((location = LDV_EXPR_STATEMENT_LOCATION (expr_statement)))
    ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);

  if ((expr = LDV_EXPR_STATEMENT_EXPR (expr_statement)))
    ldv_print_expr (indent_level, expr);

  ldv_c_backend_print (indent_level, false, ";");

  LDV_XDELETE_ON_PRINTING (expr_statement);
}

/*
external-declaration:
    function-definition
    declaration
*/
void
ldv_print_ext_decl (unsigned int indent_level, ldv_ext_decl_ptr ext_decl)
{
  ldv_func_def_ptr func_def;
  ldv_decl_ptr decl;

  switch (LDV_EXT_DECL_KIND (ext_decl))
    {
    case LDV_EXT_DECL_FIRST:
      if ((func_def = LDV_EXT_DECL_FUNC_DEF (ext_decl)))
        ldv_print_func_def (indent_level, func_def);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "function definition of external declaration was not printed");

      break;

    case LDV_EXT_DECL_SECOND:
      if ((decl = LDV_EXT_DECL_DECL (ext_decl)))
        ldv_print_decl (indent_level, decl);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "declaration of external declaration was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "external declaration was not printed");
    }

  LDV_XDELETE_ON_PRINTING (ext_decl);
}

/*
floating-constant:
    decimal-floating-constant
    hexadecimal-floating-constant
*/
static void
ldv_print_floating_constant (unsigned int indent_level, ldv_floating_constant_ptr floating_constant)
{
  const char *cst;

  switch (LDV_FLOATING_CONSTANT_KIND (floating_constant))
    {
    case LDV_FLOATING_CONSTANT_FIRST:
      cst = LDV_FLOATING_CONSTANT_CST (floating_constant);
      ldv_c_backend_print (indent_level, true, "%s", cst);

      break;

    case LDV_FLOATING_CONSTANT_SECOND:

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "floating constant was not printed");
    }

  LDV_XDELETE_ON_PRINTING (floating_constant);
}

/*
function-definition:
    declaration-specifiers declarator declaration-listopt compound-statement
*/
static void
ldv_print_func_def (unsigned int indent_level, ldv_func_def_ptr func_def)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_declarator_ptr declarator;
  ldv_compound_statement_ptr compound_statement;
  ldv_location_ptr location;

  /* Print #line directive before function definition. */
  if ((declarator = LDV_FUNC_DEF_DECLARATOR (func_def))
    && (location = ldv_declarator_location (declarator)))
    ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_DECL, location);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "line directive for function definition was not printed");


  if ((decl_spec = LDV_FUNC_DEF_DECL_SPEC (func_def)))
    ldv_print_decl_spec (indent_level, decl_spec);
  else
    LDV_ERROR ("function definition declaration specifiers weren't printed");


  if ((declarator = LDV_FUNC_DEF_DECLARATOR (func_def)))
    ldv_print_declarator (indent_level, declarator);
  else
    LDV_ERROR ("function definition declarator wasn't printed");

  if ((compound_statement = LDV_FUNC_DEF_COMPOUND_STATEMENT (func_def)))
    {
      ldv_c_backend_print (indent_level, false, "\n");
      ldv_print_compound_statement (indent_level, compound_statement);
    }
  else
    LDV_ERROR ("function definition compound statement wasn't printed");

  LDV_XDELETE_ON_PRINTING (func_def);
}

/*
function-specifier:
    inline
*/
static void
ldv_print_func_spec (unsigned int indent_level, ldv_func_spec_ptr func_spec)
{
  switch (LDV_FUNC_SPEC_KIND (func_spec))
    {
    case LDV_FUNC_SPEC_INLINE:
      ldv_c_backend_print (indent_level, true, "inline");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "function specifier was not printed");
    }

  LDV_XDELETE_ON_PRINTING (func_spec);
}

/*
identifier:
    identifier-nondigit
    identifier identifier-nondigit
    identifier digit
*/
static void
ldv_print_identifier (unsigned int indent_level, ldv_identifier_ptr identifier)
{
  char *str;

  if ((str = LDV_IDENTIFIER_STR (identifier)))
    {
      ldv_c_backend_print (indent_level, true, "%s", str);
      LDV_XDELETE_ON_PRINTING (str);
    }
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "identifier was not printed");

  LDV_XDELETE_ON_PRINTING (identifier);
}

/*
inclusive-OR-expression:
    exclusive-OR-expression
    inclusive-OR-expression | exclusive-OR-expression
*/
static void
ldv_print_inclusive_or_expr (unsigned int indent_level, ldv_inclusive_or_expr_ptr inclusive_or_expr)
{
  ldv_exclusive_or_expr_ptr exclusive_or_expr;
  ldv_inclusive_or_expr_ptr inclusive_or_expr_next;
  ldv_location_ptr location;

  switch (LDV_INCLUSIVE_OR_EXPR_KIND (inclusive_or_expr))
    {
    case LDV_INCLUSIVE_OR_EXPR_FIRST:
      if ((exclusive_or_expr = LDV_INCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR (inclusive_or_expr)))
        ldv_print_exclusive_or_expr (indent_level, exclusive_or_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "exclusive or expression of inclusive or expression was not printed");

      break;

    case LDV_INCLUSIVE_OR_EXPR_SECOND:

      if ((inclusive_or_expr_next = LDV_INCLUSIVE_OR_EXPR_INCLUSIVE_OR_EXPR (inclusive_or_expr)))
        ldv_print_inclusive_or_expr (indent_level, inclusive_or_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "inclusive or expression of inclusive or expression was not printed");

      if ((location = LDV_INCLUSIVE_OR_EXPR_LOCATION (inclusive_or_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, "|");

      if ((exclusive_or_expr = LDV_INCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR (inclusive_or_expr)))
        ldv_print_exclusive_or_expr (indent_level, exclusive_or_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "exclusive or expression of inclusive or expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "inclusive or expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (inclusive_or_expr);
}

/*
init-declarator:
    declarator
    declarator = initializer
*/
static void
ldv_print_init_declarator (unsigned int indent_level, ldv_init_declarator_ptr init_declarator)
{
  ldv_declarator_ptr declarator;
  ldv_initializer_ptr initializer;

  switch (LDV_INIT_DECLARATOR_KIND (init_declarator))
    {
    case LDV_INIT_DECLARATOR_FIRST:
    case LDV_INIT_DECLARATOR_SECOND:
      if ((declarator = LDV_INIT_DECLARATOR_DECLARATOR (init_declarator)))
        ldv_print_declarator (indent_level, declarator);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "declarator of init declarator was not printed");

      if (LDV_INIT_DECLARATOR_KIND (init_declarator) == LDV_INIT_DECLARATOR_SECOND)
        {
          ldv_c_backend_padding_force ();
          ldv_c_backend_print (indent_level, true, "=");

          if ((initializer = LDV_INIT_DECLARATOR_INITIALIZER (init_declarator)))
            ldv_print_initializer (indent_level, initializer);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "initializer of init declarator was not printed");
        }

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "init declarator was not printed");
    }

  LDV_XDELETE_ON_PRINTING (init_declarator);
}

/*
init-declarator-list:
    init-declarator
    init-declarator-list , init-declarator
*/
static void
ldv_print_init_declarator_list (unsigned int indent_level, ldv_init_declarator_list_ptr init_declarator_list)
{
  ldv_init_declarator_list_ptr init_declarator_list_next;
  ldv_init_declarator_ptr init_declarator;

  if ((init_declarator_list_next = LDV_INIT_DECLARATOR_LIST_INIT_DECLARATOR_LIST (init_declarator_list)))
    {
      ldv_print_init_declarator_list (indent_level, init_declarator_list_next);
      ldv_c_backend_print (indent_level, false, ",");
    }

  if ((init_declarator = LDV_INIT_DECLARATOR_LIST_INIT_DECLARATOR (init_declarator_list)))
    ldv_print_init_declarator (indent_level, init_declarator);
  else
    LDV_ERROR ("init declarator list init declarator wasn't printed");

  LDV_XDELETE_ON_PRINTING (init_declarator_list);
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
static void
ldv_print_initializer (unsigned int indent_level, ldv_initializer_ptr initializer)
{
  ldv_assignment_expr_ptr assignment_expr;
  ldv_initializer_list_ptr initializer_list;

  switch (LDV_INITIALIZER_KIND (initializer))
    {
    case LDV_INITIALIZER_FIRST:
      if ((assignment_expr = LDV_INITIALIZER_ASSIGNMENT_EXPR (initializer)))
        ldv_print_assignment_expr (indent_level, assignment_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression of initializer was not printed");

      break;

    case LDV_INITIALIZER_SECOND:
    case LDV_INITIALIZER_THIRD:
    case LDV_INITIALIZER_FOURTH:
      ldv_c_backend_print (indent_level, true, "{");

      if (LDV_INITIALIZER_KIND (initializer) != LDV_INITIALIZER_FOURTH)
        {
          if ((initializer_list = LDV_INITIALIZER_INITIALIZER_LIST (initializer)))
            ldv_print_initializer_list (indent_level, initializer_list);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "initializer list of inititializer was not printed");

          if (LDV_INITIALIZER_KIND (initializer) == LDV_INITIALIZER_THIRD)
            ldv_c_backend_print (indent_level, true, ",");
        }

      ldv_c_backend_print (indent_level, true, "}");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "initializer was not printed");
    }

  LDV_XDELETE_ON_PRINTING (initializer);
}

/*
initializer-list:
    designationopt initializer
    initializer-list , designationopt initializer
*/
static void
ldv_print_initializer_list (unsigned int indent_level, ldv_initializer_list_ptr initializer_list)
{
  ldv_designation_ptr designation;
  ldv_initializer_ptr initializer;
  ldv_initializer_list_ptr initializer_list_next;

  switch (LDV_INITIALIZER_LIST_KIND (initializer_list))
    {
    case LDV_INITIALIZER_LIST_FIRST:
    case LDV_INITIALIZER_LIST_SECOND:
      if (LDV_INITIALIZER_LIST_KIND (initializer_list) == LDV_INITIALIZER_LIST_SECOND)
        {
          if ((initializer_list_next = LDV_INITIALIZER_LIST_INITIALIZER_LIST(initializer_list)))
            ldv_print_initializer_list (indent_level, initializer_list_next);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "initializer list of argument expression list was not printed");

          ldv_c_backend_print (indent_level, true, ",");
        }

      if ((designation = LDV_INITIALIZER_LIST_DESIGNATION (initializer_list)))
        ldv_print_designation (indent_level, designation);

      if ((initializer = LDV_INITIALIZER_LIST_INITIALIZER (initializer_list)))
        ldv_print_initializer (indent_level, initializer);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "initializer of argument expression list was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "initializer list was not printed");
    }

  LDV_XDELETE_ON_PRINTING (initializer_list);
}

/*
integer-constant:
    decimal-constant integer-suffixopt
    octal-constant integer-suffixopt
    hexadecimal-constant integer-suffixopt
*/
static void
ldv_print_integer_constant (unsigned int indent_level, ldv_integer_constant_ptr integer_constant)
{
  HOST_WIDE_INT decimal_constant;
  ldv_integer_suffix_ptr integer_suffix;

  switch (LDV_INTEGER_CONSTANT_KIND (integer_constant))
    {
    case LDV_INTEGER_CONSTANT_FIRST:
      decimal_constant = LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT (integer_constant);
      if (LDV_INTEGER_CONSTANT_ISSIGNED (integer_constant))
        ldv_c_backend_print (indent_level, true, HOST_WIDE_INT_PRINT_DEC, decimal_constant);
      else
        ldv_c_backend_print (indent_level, true, HOST_WIDE_INT_PRINT_UNSIGNED, /*(UINTMAX_TYPE)*/ decimal_constant);

      if ((integer_suffix = LDV_INTEGER_CONSTANT_INTEGER_SUFFIX (integer_constant)))
        ldv_print_integer_suffix (indent_level, integer_suffix);

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "integer constant was not printed");
    }

  LDV_XDELETE_ON_PRINTING (integer_constant);
}

/*
integer-suffix:
    unsigned-suffix long-suffixopt
    unsigned-suffix long-long-suffix
    long-suffix unsigned-suffixopt
    long-long-suffix unsigned-suffixopt
*/
static void
ldv_print_integer_suffix (unsigned int indent_level, ldv_integer_suffix_ptr integer_suffix)
{
  ldv_unsigned_suffix_ptr unsigned_suffix;
  ldv_long_suffix_ptr long_suffix;
  ldv_long_long_suffix_ptr long_long_suffix;

  switch (LDV_INTEGER_SUFFIX_KIND (integer_suffix))
    {
    case LDV_INTEGER_SUFFIX_FIRST:
    case LDV_INTEGER_SUFFIX_SECOND:
      if ((unsigned_suffix = LDV_INTEGER_SUFFIX_UNSIGNED_SUFFIX (integer_suffix)))
        ldv_print_unsigned_suffix (indent_level, unsigned_suffix);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "unsigned suffix of integer suffix was not printed");

      if (LDV_INTEGER_SUFFIX_KIND (integer_suffix) == LDV_INTEGER_SUFFIX_FIRST)
        {
          if ((long_suffix = LDV_INTEGER_SUFFIX_LONG_SUFFIX (integer_suffix)))
            ldv_print_long_suffix (indent_level, long_suffix);
        }
      else
        {
          if ((long_long_suffix = LDV_INTEGER_SUFFIX_LONG_LONG_SUFFIX (integer_suffix)))
            ldv_print_long_long_suffix (indent_level, long_long_suffix);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "long long suffix of integer suffix was not printed");
        }

      break;

    case LDV_INTEGER_SUFFIX_THIRD:
    case LDV_INTEGER_SUFFIX_FOURTH:
      if (LDV_INTEGER_SUFFIX_KIND (integer_suffix) == LDV_INTEGER_SUFFIX_THIRD)
        {
          if ((long_suffix = LDV_INTEGER_SUFFIX_LONG_SUFFIX (integer_suffix)))
            ldv_print_long_suffix (indent_level, long_suffix);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "long suffix of integer suffix was not printed");
        }
      else
        {
          if ((long_long_suffix = LDV_INTEGER_SUFFIX_LONG_LONG_SUFFIX (integer_suffix)))
            ldv_print_long_long_suffix (indent_level, long_long_suffix);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "long long suffix of integer suffix was not printed");
        }

      if ((unsigned_suffix = LDV_INTEGER_SUFFIX_UNSIGNED_SUFFIX (integer_suffix)))
        ldv_print_unsigned_suffix (indent_level, unsigned_suffix);

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "integer suffix was not printed");
    }

  LDV_XDELETE_ON_PRINTING (integer_suffix);
}

/*
jump-statement:
    goto identifier ;
    continue ;
    break ;
    return expressionopt ;
*/
static void
ldv_print_jump_statement (unsigned int indent_level, ldv_jump_statement_ptr jump_statement)
{
  const char *id;
  ldv_expr_ptr expr;
  ldv_location_ptr location;

  switch (LDV_JUMP_STATEMENT_KIND (jump_statement))
    {
      case LDV_JUMP_STATEMENT_GOTO:
      case LDV_JUMP_STATEMENT_RETURN:
        if ((location = LDV_JUMP_STATEMENT_LOCATION (jump_statement)))
          ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);

        if (LDV_JUMP_STATEMENT_KIND (jump_statement) == LDV_JUMP_STATEMENT_GOTO)
          {
            ldv_c_backend_print (indent_level, true, "goto");

            if ((id = LDV_JUMP_STATEMENT_ID (jump_statement)))
              ldv_c_backend_print (indent_level, true, id);
            else
              LDV_ERROR ("can't find goto name");
          }
        else
          {
            ldv_c_backend_print (indent_level, true, "return");

            if ((expr = LDV_JUMP_STATEMENT_EXPR (jump_statement)))
              ldv_print_expr (indent_level, expr);
          }

        break;

      default:
        LDV_PRETTY_PRINT_ERROR (indent_level, "jump statement was not printed");
    }

  ldv_c_backend_print (indent_level, false, ";");

  LDV_XDELETE_ON_PRINTING (jump_statement);
}

/*
GNU extensions:

label-declaration:
    __label__ identifier-list ;
*/
static void
ldv_print_label_decl (unsigned int indent_level, ldv_label_decl_ptr label_decl)
{
  ldv_identifier_ptr identifier;

  ldv_c_backend_print (indent_level, true, "__label__");

  if ((identifier = LDV_LABEL_DECL_IDENTIFIER (label_decl)))
    ldv_print_identifier (indent_level, identifier);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "label declaration was not printed");

  ldv_c_backend_print (indent_level, false, ";");

  LDV_XDELETE_ON_PRINTING (label_decl);
}

/*
GNU extensions:

label-declarations:
    label-declaration
    label-declarations label-declaration
*/
static void
ldv_print_label_decls (unsigned int indent_level, ldv_label_decls_ptr label_decls)
{
  ldv_label_decls_ptr label_decls_next;
  ldv_label_decl_ptr label_decl;

  if ((label_decls_next = LDV_LABEL_DECLS_LABEL_DECLS (label_decls)))
    {
      ldv_print_label_decls (indent_level, label_decls_next);
      ldv_c_backend_print (indent_level, false, "\n");
    }

  if ((label_decl = LDV_LABEL_DECLS_LABEL_DECL (label_decls)))
    ldv_print_label_decl (indent_level, label_decl);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "label declarations were not printed");

  LDV_XDELETE_ON_PRINTING (label_decls);
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
static void
ldv_print_labeled_statement (unsigned int indent_level, ldv_labeled_statement_ptr labeled_statement)
{
  const char *id;
  ldv_statement_ptr statement;
  int const_expr;
  ldv_location_ptr location;

  switch (LDV_LABELED_STATEMENT_KIND (labeled_statement))
    {
    case LDV_LABELED_STATEMENT_LABEL:
      if ((location = LDV_LABELED_STATEMENT_LOCATION (labeled_statement)))
        ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);

      if ((id = LDV_LABELED_STATEMENT_ID (labeled_statement)))
        ldv_c_backend_print (indent_level, true, id);
      else
        LDV_ERROR ("labeled statement name wasn't printed");

      ldv_c_backend_print (indent_level, false, ":");
      ldv_c_backend_padding_force ();

      if ((statement = LDV_LABELED_STATEMENT_STATEMENT (labeled_statement)))
        ldv_print_statement_internal (indent_level, statement);
      else
        LDV_ERROR ("labeled statement statement wasn't printed");

      break;

    case LDV_LABELED_STATEMENT_CASE:
    case LDV_LABELED_STATEMENT_CASE_RANGE:
      if ((location = LDV_LABELED_STATEMENT_LOCATION (labeled_statement)))
        ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "line directive for labeled statement was not printed");

      ldv_c_backend_print (indent_level, true, "case");

      const_expr = LDV_LABELED_STATEMENT_CONST_EXPR1 (labeled_statement);
      ldv_c_backend_print (indent_level, true, "%d", const_expr);

      if (LDV_LABELED_STATEMENT_KIND (labeled_statement) == LDV_LABELED_STATEMENT_CASE_RANGE)
        {
          ldv_c_backend_print (indent_level, true, "...");
          const_expr = LDV_LABELED_STATEMENT_CONST_EXPR2 (labeled_statement);
          ldv_c_backend_print (indent_level, true, "%d", const_expr);
        }

      ldv_c_backend_print (indent_level, false, ":");

      if ((statement = LDV_LABELED_STATEMENT_STATEMENT (labeled_statement)))
        ldv_print_statement_internal (indent_level, statement);
      else
        LDV_ERROR ("labeled statement statement wasn't printed");

      break;

    case LDV_LABELED_STATEMENT_DEFAULT:
      if ((location = LDV_LABELED_STATEMENT_LOCATION (labeled_statement)))
        ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "line directive for labeled statement was not printed");

      ldv_c_backend_print (indent_level, true, "default");
      ldv_c_backend_print (indent_level, false, ":");

      if ((statement = LDV_LABELED_STATEMENT_STATEMENT (labeled_statement)))
        ldv_print_statement_internal (indent_level, statement);
      else
        LDV_ERROR ("labeled statement statement wasn't printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "labeled statement was not printed");
    }

  LDV_XDELETE_ON_PRINTING (labeled_statement);
}

static void
ldv_print_line_directive (bool new_line, int lines_level, ldv_location_ptr location)
{
  const char *file;
  unsigned int line = LDV_LOCATION_LINE (location);

  /* Don't print line directives if they are unneeded. */
  if (ldv_c_backend_is_lines_level (LDV_C_BACKEND_LINES_LEVEL_NO))
    goto free_location;

  /* Print just required entities if this is specified. */
  if (ldv_c_backend_is_lines_level (LDV_C_BACKEND_LINES_LEVEL_USEFUL))
    {
      if (ldv_c_backend_is_current_line (line))
        goto free_location;
    }
  else if (!ldv_c_backend_is_lines_level (LDV_C_BACKEND_LINES_LEVEL_ALL))
    {
      switch (lines_level)
        {
          case LDV_C_BACKEND_LINES_LEVEL_DECL:
          case LDV_C_BACKEND_LINES_LEVEL_STATEMENT:
          case LDV_C_BACKEND_LINES_LEVEL_EXPR:
            if (!ldv_c_backend_is_lines_level (lines_level))
              goto free_location;

            break;

          default:
            LDV_ERROR ("something strange");
        }
    }

  if (new_line)
    ldv_c_backend_print (0, false, "\n");

  ldv_c_backend_print (0, true, "#line");

  /* Line is either normal line from some source file or 0 for built-in stuff,
   * e.g. for artificial return expressions added by GCC for function main. */
  ldv_c_backend_print (0, true, "%d", line);

  if ((file = LDV_LOCATION_FILE (location)))
    ldv_c_backend_print (0, true, "\"%s\"", file);
  else
    LDV_PRETTY_PRINT_ERROR (0, "file for line directive was not printed");

  ldv_c_backend_print (0, false, "\n");

  /* Update current line if a line directive was printed. This also allows to
     ignore lines printed for line directive itself. */
  ldv_c_backend_current_line_set (line);

  /* The same locations can be printed just once. So freeing them here
     is safe. */
  free_location: LDV_XDELETE_ON_PRINTING (location);
}

/*
logical-AND-expression:
    inclusive-OR-expression
    logical-AND-expression && inclusive-OR-expression
*/
static void
ldv_print_logical_and_expr (unsigned int indent_level, ldv_logical_and_expr_ptr logical_and_expr)
{
  ldv_inclusive_or_expr_ptr inclusive_or_expr;
  ldv_logical_and_expr_ptr logical_and_expr_next;
  ldv_location_ptr location;

  switch (LDV_LOGICAL_AND_EXPR_KIND (logical_and_expr))
    {
    case LDV_LOGICAL_AND_EXPR_FIRST:
      if ((inclusive_or_expr = LDV_LOGICAL_AND_EXPR_INCLUSIVE_OR_EXPR (logical_and_expr)))
        ldv_print_inclusive_or_expr (indent_level, inclusive_or_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "inclusive or expression of logical and expression was not printed");

      break;

    case LDV_LOGICAL_AND_EXPR_SECOND:
      if ((logical_and_expr_next = LDV_LOGICAL_AND_EXPR_LOGICAL_AND_EXPR (logical_and_expr)))
        ldv_print_logical_and_expr (indent_level, logical_and_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "logical and expression of logical and expression was not printed");

      if ((location = LDV_LOGICAL_AND_EXPR_LOCATION (logical_and_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, "&&");

      if ((inclusive_or_expr = LDV_LOGICAL_AND_EXPR_INCLUSIVE_OR_EXPR (logical_and_expr)))
        ldv_print_inclusive_or_expr (indent_level, inclusive_or_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "inclusive or expression of logical and expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "logical and expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (logical_and_expr);
}

/*
logical-OR-expression:
    logical-AND-expression
    logical-OR-expression || logical-AND-expression
*/
static void
ldv_print_logical_or_expr (unsigned int indent_level, ldv_logical_or_expr_ptr logical_or_expr)
{
  ldv_logical_and_expr_ptr logical_and_expr;
  ldv_logical_or_expr_ptr logical_or_expr_next;
  ldv_location_ptr location;

  switch (LDV_LOGICAL_OR_EXPR_KIND (logical_or_expr))
    {
    case LDV_LOGICAL_OR_EXPR_FIRST:
      if ((logical_and_expr = LDV_LOGICAL_OR_EXPR_LOGICAL_AND_EXPR (logical_or_expr)))
        ldv_print_logical_and_expr (indent_level, logical_and_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "logical and expression of logical or expression was not printed");

      break;

    case LDV_LOGICAL_OR_EXPR_SECOND:

      if ((logical_or_expr_next = LDV_LOGICAL_OR_EXPR_LOGICAL_OR_EXPR (logical_or_expr)))
        ldv_print_logical_or_expr (indent_level, logical_or_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "logical or expression of logical or expression was not printed");

      if ((location = LDV_LOGICAL_OR_EXPR_LOCATION (logical_or_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, "||");

      if ((logical_and_expr = LDV_LOGICAL_OR_EXPR_LOGICAL_AND_EXPR (logical_or_expr)))
        ldv_print_logical_and_expr (indent_level, logical_and_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "logical and expression of logical or expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "logical or expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (logical_or_expr);
}

/*
long-long-suffix: one of
    ll LL
*/
static void
ldv_print_long_long_suffix (unsigned int indent_level, ldv_long_long_suffix_ptr long_long_suffix)
{
  /* Join suffix to the previously printed integer constant. */
  ldv_c_backend_padding_cancel ();

  switch (LDV_LONG_LONG_SUFFIX_KIND (long_long_suffix))
    {
    case LDV_LONG_LONG_SUFFIX_FIRST:
      ldv_c_backend_print (indent_level, true, "ll");
      break;

    case LDV_LONG_LONG_SUFFIX_SECOND:
      ldv_c_backend_print (indent_level, true, "LL");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "long long suffix was not printed");
    }

  LDV_XDELETE_ON_PRINTING (long_long_suffix);
}

/*
long-suffix: one of
    l L
*/
static void
ldv_print_long_suffix (unsigned int indent_level, ldv_long_suffix_ptr long_suffix)
{
  /* Join suffix to the previously printed integer constant. */
  ldv_c_backend_padding_cancel ();

  switch (LDV_LONG_SUFFIX_KIND (long_suffix))
    {
    case LDV_LONG_SUFFIX_FIRST:
      ldv_c_backend_print (indent_level, true, "l");
      break;

    case LDV_LONG_SUFFIX_SECOND:
      ldv_c_backend_print (indent_level, true, "L");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "long suffix was not printed");
    }

  LDV_XDELETE_ON_PRINTING (long_suffix);
}

/*
multiplicative-expression:
    cast-expression
    multiplicative-expression * cast-expression
    multiplicative-expression / cast-expression
    multiplicative-expression % cast-expression
*/
static void
ldv_print_multiplicative_expr (unsigned int indent_level, ldv_multiplicative_expr_ptr multiplicative_expr)
{
  ldv_cast_expr_ptr cast_expr;
  ldv_multiplicative_expr_ptr multiplicative_expr_next;
  ldv_location_ptr location;

  switch (LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr))
    {
    case LDV_MULTIPLICATIVE_EXPR_FIRST:
      if ((cast_expr = LDV_MULTIPLICATIVE_EXPR_CAST_EXPR (multiplicative_expr)))
        ldv_print_cast_expr (indent_level, cast_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "cast expression of multiplicative expression was not printed");

      break;

    case LDV_MULTIPLICATIVE_EXPR_SECOND:
    case LDV_MULTIPLICATIVE_EXPR_THIRD:
    case LDV_MULTIPLICATIVE_EXPR_FOURTH:

      if ((multiplicative_expr_next = LDV_MULTIPLICATIVE_EXPR_MULTIPLICATIVE_EXPR (multiplicative_expr)))
        ldv_print_multiplicative_expr (indent_level, multiplicative_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "multiplicative expression of multiplicative expression was not printed");

      if ((location = LDV_MULTIPLICATIVE_EXPR_LOCATION (multiplicative_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if (LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr) == LDV_MULTIPLICATIVE_EXPR_SECOND)
        ldv_c_backend_print (indent_level, true, "*");
      else if (LDV_MULTIPLICATIVE_EXPR_KIND (multiplicative_expr) == LDV_MULTIPLICATIVE_EXPR_THIRD)
        ldv_c_backend_print (indent_level, true, "/");
      else
        ldv_c_backend_print (indent_level, true, "%%");

      if ((cast_expr = LDV_MULTIPLICATIVE_EXPR_CAST_EXPR (multiplicative_expr)))
        ldv_print_cast_expr (indent_level, cast_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "cast expression of multiplicative expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "multiplicative expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (multiplicative_expr);
}

/*
GNU extensions:

nested-declaration:
    declaration
    nested-function-definition
*/
static void
ldv_print_nested_decl (unsigned int indent_level, ldv_nested_decl_ptr nested_decl)
{
  ldv_decl_ptr decl;
  ldv_nested_func_def_ptr nested_func_def;

  switch (LDV_NESTED_DECL_KIND (nested_decl))
    {
    case LDV_NESTED_DECL_FIRST:
      if ((decl = LDV_NESTED_DECL_DECL (nested_decl)))
        ldv_print_decl (indent_level, decl);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "declaration of nested declaration was not printed");

      break;

    case LDV_NESTED_DECL_SECOND:
      if ((nested_func_def = LDV_NESTED_DECL_NESTED_FUNC_DEF (nested_decl)))
        ldv_print_nested_func_def (indent_level, nested_func_def);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "nested function definition of nested declaration was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "nested declaration was not printed");
    }

  LDV_XDELETE_ON_PRINTING (nested_decl);
}

/*
GNU extensions:

nested-function-definition:
    declaration-specifiers declarator declaration-listopt compound-statement
*/
static void
ldv_print_nested_func_def (unsigned int indent_level, ldv_nested_func_def_ptr nested_func_def)
{
  ldv_func_def_ptr func_def;

  if ((func_def = LDV_NESTED_FUNC_DEF_FUNC_DEF (nested_func_def)))
    ldv_print_func_def (indent_level, func_def);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "function definition of nested function definition was not printed");

  LDV_XDELETE_ON_PRINTING (nested_func_def);
}

/*
parameter-declaration:
    declaration-specifiers declarator
    declaration-specifiers abstract-declaratoropt
*/
static void
ldv_print_param_decl (unsigned int indent_level, ldv_param_decl_ptr param_decl)
{
  ldv_decl_spec_ptr decl_spec;
  ldv_declarator_ptr declarator;
  ldv_abstract_declarator_ptr abstract_declarator;

  if ((decl_spec = LDV_PARAM_DECL_DECL_SPEC (param_decl)))
    ldv_print_decl_spec (indent_level, decl_spec);
  else
    LDV_ERROR ("parameter declaration declaration specifiers weren't printed");

  if ((declarator = LDV_PARAM_DECL_DECLARATOR (param_decl)))
    ldv_print_declarator (indent_level, declarator);
  else if ((abstract_declarator = LDV_PARAM_DECL_ABSTRACT_DECLARATOR (param_decl)))
    ldv_print_abstract_declarator (indent_level, abstract_declarator);

  LDV_XDELETE_ON_PRINTING (param_decl);
}

/*
parameter-list:
    parameter-declaration
    parameter-list , parameter-declaration
*/
static void
ldv_print_param_list (unsigned int indent_level, ldv_param_list_ptr param_list)
{
  ldv_param_list_ptr param_list_next;
  ldv_param_decl_ptr param_decl;

  if ((param_list_next = LDV_PARAM_LIST_PARAM_LIST (param_list)))
    {
      ldv_print_param_list (indent_level, param_list_next);
      ldv_c_backend_print (indent_level, false, ",");
      ldv_c_backend_padding_force ();
    }

  if ((param_decl = LDV_PARAM_LIST_PARAM_DECL (param_list)))
    ldv_print_param_decl (indent_level, param_decl);
  else
    LDV_ERROR ("parameter declaration list parameter declaration wasn't printed");

  LDV_XDELETE_ON_PRINTING (param_list);
}

/*
parameter-type-list:
    parameter-list
    parameter-list , ...
*/
static void
ldv_print_param_type_list (unsigned int indent_level, ldv_param_type_list_ptr param_type_list)
{
  ldv_param_list_ptr param_list;
  bool is_va;

  if ((param_list = LDV_PARAM_TYPE_LIST_PARAM_LIST (param_type_list)))
    ldv_print_param_list (indent_level, param_list);
  else
    LDV_ERROR ("parameter type list parametr list wasn't printed");

  if ((is_va = LDV_PARAM_TYPE_LIST_IS_VA (param_type_list)))
    {
      ldv_c_backend_print (indent_level, false, ",");
      ldv_c_backend_padding_force ();
      ldv_c_backend_print (indent_level, true, "...");
      ldv_c_backend_padding_cancel ();
    }

  LDV_XDELETE_ON_PRINTING (param_type_list);
}

/*
pointer:
    * type-qualifier-listopt
    * type-qualifier-listopt pointer
*/
static void
ldv_print_pointer (unsigned int indent_level, ldv_pointer_ptr pointer)
{
  ldv_type_qual_list_ptr type_qual_list;
  ldv_pointer_ptr pointer_next;

  ldv_c_backend_print (indent_level, true, "*");
  ldv_c_backend_padding_cancel ();

  if ((type_qual_list = LDV_POINTER_TYPE_QUAL_LIST (pointer)))
    ldv_print_type_qual_list (indent_level, type_qual_list);

  if ((pointer_next = LDV_POINTER_POINTER (pointer)))
    ldv_print_pointer (indent_level, pointer_next);

  LDV_XDELETE_ON_PRINTING (pointer);
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
static void
ldv_print_postfix_expr (unsigned int indent_level, ldv_postfix_expr_ptr postfix_expr)
{
  ldv_primary_expr_ptr primary_expr;
  ldv_postfix_expr_ptr postfix_expr_next;
  ldv_expr_ptr expr;
  ldv_arg_expr_list_ptr arg_expr_list;
  ldv_identifier_ptr identifier;
  ldv_type_name_ptr type_name;
  ldv_initializer_list_ptr initializer_list;
  ldv_location_ptr location;

  switch (LDV_POSTFIX_EXPR_KIND (postfix_expr))
    {
    case LDV_POSTFIX_EXPR_FIRST:
      if ((primary_expr = LDV_POSTFIX_EXPR_PRIMARY_EXPR (postfix_expr)))
        ldv_print_primary_expr (indent_level, primary_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "primary expression of postfix expression was not printed");

      break;

    case LDV_POSTFIX_EXPR_SECOND:
    case LDV_POSTFIX_EXPR_THIRD:
    case LDV_POSTFIX_EXPR_FOURTH:
    case LDV_POSTFIX_EXPR_FIFTH:
    case LDV_POSTFIX_EXPR_SIXTH:
    case LDV_POSTFIX_EXPR_SEVENTH:
    case LDV_POSTFIX_EXPR_EIGHTH:
    case LDV_POSTFIX_EXPR_NINETH:
      if (LDV_POSTFIX_EXPR_KIND (postfix_expr) != LDV_POSTFIX_EXPR_EIGHTH
          && LDV_POSTFIX_EXPR_KIND (postfix_expr) != LDV_POSTFIX_EXPR_NINETH)
        {
          if ((postfix_expr_next = LDV_POSTFIX_EXPR_POSTFIX_EXPR (postfix_expr)))
            ldv_print_postfix_expr (indent_level, postfix_expr_next);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "postfix expression of postfix expression was not printed");
        }

      if ((location = LDV_POSTFIX_EXPR_LOCATION (postfix_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if (LDV_POSTFIX_EXPR_KIND (postfix_expr) == LDV_POSTFIX_EXPR_SECOND)
        {
          ldv_c_backend_print (indent_level, true, "[");

          if ((expr = LDV_POSTFIX_EXPR_EXPR (postfix_expr)))
            ldv_print_expr (indent_level, expr);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "expression of postfix expression was not printed");

          ldv_c_backend_print (indent_level, true, "]");
        }
      else if (LDV_POSTFIX_EXPR_KIND (postfix_expr) == LDV_POSTFIX_EXPR_THIRD)
        {
          ldv_c_backend_print (indent_level, true, "(");

          if ((arg_expr_list = LDV_POSTFIX_EXPR_ARG_EXPR_LIST (postfix_expr)))
            ldv_print_arg_expr_list (indent_level, arg_expr_list);

          ldv_c_backend_print (indent_level, true, ")");
        }
      else if (LDV_POSTFIX_EXPR_KIND (postfix_expr) == LDV_POSTFIX_EXPR_FOURTH)
        {
          ldv_c_backend_print (indent_level, true, ".");

          if ((identifier = LDV_POSTFIX_EXPR_IDENTIFIER (postfix_expr)))
            ldv_print_identifier (indent_level, identifier);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of postfix expression was not printed");
        }
      else if (LDV_POSTFIX_EXPR_KIND (postfix_expr) == LDV_POSTFIX_EXPR_SIXTH)
        ldv_c_backend_print (indent_level, true, "++");
      else if (LDV_POSTFIX_EXPR_KIND (postfix_expr) == LDV_POSTFIX_EXPR_SEVENTH)
        ldv_c_backend_print (indent_level, true, "--");
      else
        {
          ldv_c_backend_print (indent_level, true, "(");

          if ((type_name = LDV_POSTFIX_EXPR_TYPE_NAME (postfix_expr)))
            ldv_print_type_name (indent_level, type_name);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "type name of postfix expression was not printed");

          ldv_c_backend_print (indent_level, true, ")");

          ldv_c_backend_print (indent_level, true, "{");

          if (LDV_POSTFIX_EXPR_KIND (postfix_expr) == LDV_POSTFIX_EXPR_EIGHTH)
            {
              if ((initializer_list = LDV_POSTFIX_EXPR_INITIALIZER_LIST (postfix_expr)))
                ldv_print_initializer_list (indent_level, initializer_list);
              else
                LDV_PRETTY_PRINT_ERROR (indent_level, "initializer list of postfix expression was not printed");
            }

          ldv_c_backend_print (indent_level, true, "}");
        }

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "postfix expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (postfix_expr);
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
static void
ldv_print_primary_expr (unsigned int indent_level, ldv_primary_expr_ptr primary_expr)
{
  ldv_identifier_ptr identifier;
  ldv_constant_ptr constant;
  ldv_str_literal_ptr str_literal;
  ldv_expr_ptr expr;
  ldv_compound_statement_ptr compound_statement;
  ldv_assignment_expr_ptr assignment_expr;
  ldv_type_name_ptr type_name;

  switch (LDV_PRIMARY_EXPR_KIND (primary_expr))
    {
    case LDV_PRIMARY_EXPR_FIRST:
      if ((identifier = LDV_PRIMARY_EXPR_IDENTIFIER (primary_expr)))
        ldv_print_identifier (indent_level, identifier);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of primary expression was not printed");

      break;

    case LDV_PRIMARY_EXPR_SECOND:
      if ((constant = LDV_PRIMARY_EXPR_CONSTANT (primary_expr)))
        ldv_print_constant (indent_level, constant);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "constant of primary expression was not printed");

      break;

    case LDV_PRIMARY_EXPR_THIRD:
      if ((str_literal = LDV_PRIMARY_EXPR_STR_LITERAL (primary_expr)))
        ldv_print_str_literal (indent_level, str_literal);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "string literal of primary expression was not printed");

      break;

    case LDV_PRIMARY_EXPR_FOURTH:
      ldv_c_backend_print (indent_level, true, "(");

      if ((expr = LDV_PRIMARY_EXPR_EXPR (primary_expr)))
        ldv_print_expr (indent_level, expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression of primary expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      break;

    case LDV_PRIMARY_EXPR_FIFTH:
      ldv_c_backend_print (indent_level, true, "(");

      if ((compound_statement = LDV_PRIMARY_EXPR_COMPOUND_STATEMENT (primary_expr)))
        ldv_print_compound_statement (indent_level, compound_statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "compound statement of primary expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      break;

    case LDV_PRIMARY_EXPR_SIXTH:
      ldv_c_backend_print (indent_level, true, "__builtin_va_arg (");

      if ((assignment_expr = LDV_PRIMARY_EXPR_ASSIGNMENT_EXPR (primary_expr)))
        ldv_print_assignment_expr (indent_level, assignment_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "assignment expression of primary expression was not printed");

      ldv_c_backend_print (indent_level, true, ",");

      if ((type_name = LDV_PRIMARY_EXPR_TYPE_NAME (primary_expr)))
        ldv_print_type_name (indent_level, type_name);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "type name of primary expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "primary expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (primary_expr);
}

/*
relational-expression:
    shift-expression
    relational-expression < shift-expression
    relational-expression > shift-expression
    relational-expression <= shift-expression
    relational-expression >= shift-expression
*/
static void
ldv_print_relational_expr (unsigned int indent_level, ldv_relational_expr_ptr relational_expr)
{
  ldv_shift_expr_ptr shift_expr;
  ldv_relational_expr_ptr relational_expr_next;
  ldv_location_ptr location;

  switch (LDV_RELATIONAL_EXPR_KIND (relational_expr))
    {
    case LDV_RELATIONAL_EXPR_FIRST:
      if ((shift_expr = LDV_RELATIONAL_EXPR_SHIFT_EXPR (relational_expr)))
        ldv_print_shift_expr (indent_level, shift_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "shift expression of relational expression was not printed");

      break;

    case LDV_RELATIONAL_EXPR_SECOND:
    case LDV_RELATIONAL_EXPR_THIRD:
    case LDV_RELATIONAL_EXPR_FOURTH:
    case LDV_RELATIONAL_EXPR_FIFTH:

      if ((relational_expr_next = LDV_RELATIONAL_EXPR_RELATIONAL_EXPR (relational_expr)))
        ldv_print_relational_expr (indent_level, relational_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "relational expression of relational expression was not printed");

      if ((location = LDV_RELATIONAL_EXPR_LOCATION (relational_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if (LDV_RELATIONAL_EXPR_KIND (relational_expr) == LDV_RELATIONAL_EXPR_SECOND)
        ldv_c_backend_print (indent_level, true, "<");
      else if (LDV_RELATIONAL_EXPR_KIND (relational_expr) == LDV_RELATIONAL_EXPR_THIRD)
        ldv_c_backend_print (indent_level, true, ">");
      else if (LDV_RELATIONAL_EXPR_KIND (relational_expr) == LDV_RELATIONAL_EXPR_FOURTH)
        ldv_c_backend_print (indent_level, true, "<=");
      else
        ldv_c_backend_print (indent_level, true, ">=");

      if ((shift_expr = LDV_RELATIONAL_EXPR_SHIFT_EXPR (relational_expr)))
        ldv_print_shift_expr (indent_level, shift_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "shift expression of relational expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "relational expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (relational_expr);
}

/*
selection-statement:
    if ( expression ) statement
    if ( expression ) statement else statement
    switch ( expression ) statement
*/
static void
ldv_print_selection_statement (unsigned int indent_level, ldv_selection_statement_ptr selection_statement)
{
  enum ldv_selection_statement_kind kind;
  ldv_expr_ptr cond_expr;
  ldv_statement_ptr then_statement, else_statement;
  ldv_statement_ptr switch_statement;
  ldv_location_ptr location;

  switch (kind = LDV_SELECTION_STATEMENT_KIND (selection_statement))
    {
    case LDV_SELECTION_STATEMENT_SWITCH:
      if ((location = LDV_SELECTION_STATEMENT_LOCATION (selection_statement)))
        ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "line directive for selection statement was not printed");

      ldv_c_backend_print (indent_level, true, "switch");
      ldv_c_backend_print (indent_level, true, "(");
      ldv_c_backend_padding_cancel ();

      if ((cond_expr = LDV_SELECTION_STATEMENT_EXPR (selection_statement)))
        ldv_print_expr (indent_level, cond_expr);
      else
        LDV_ERROR ("selection statement conditional exrpession wasn't printed");

      ldv_c_backend_print (indent_level, false, ")");
      ldv_c_backend_print (indent_level, false, "\n");

      if ((switch_statement = LDV_SELECTION_STATEMENT_STATEMENT1 (selection_statement)))
        ldv_print_statement_internal (indent_level, switch_statement);
      else
        LDV_ERROR ("selection statement switch statement wasn't printed");

      break;

    case LDV_SELECTION_STATEMENT_IF_THEN:
    case LDV_SELECTION_STATEMENT_IF_THEN_ELSE:
      if ((location = LDV_SELECTION_STATEMENT_LOCATION (selection_statement)))
        ldv_print_line_directive (false, LDV_C_BACKEND_LINES_LEVEL_STATEMENT, location);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "line directive for selection statement was not printed");

      ldv_c_backend_print (indent_level, true, "if");
      ldv_c_backend_print (indent_level, true, "(");
      ldv_c_backend_padding_cancel ();

      if ((cond_expr = LDV_SELECTION_STATEMENT_EXPR (selection_statement)))
        ldv_print_expr (indent_level, cond_expr);
      else
        LDV_ERROR ("selection statement conditional exrpession wasn't printed");

      ldv_c_backend_print (indent_level, false, ")");
      ldv_c_backend_print (indent_level, false, "\n");

      if ((then_statement = LDV_SELECTION_STATEMENT_STATEMENT1 (selection_statement)))
        ldv_print_statement_internal (indent_level, then_statement);
      else
        LDV_ERROR ("selection statement then statement wasn't printed");

      if (kind == LDV_SELECTION_STATEMENT_IF_THEN_ELSE)
        {
          ldv_c_backend_print (indent_level, false, "\n");
          ldv_c_backend_print (indent_level, true, "else");
          ldv_c_backend_print (indent_level, false, "\n");
        }

      if ((else_statement = LDV_SELECTION_STATEMENT_STATEMENT2 (selection_statement)))
        ldv_print_statement_internal (indent_level, else_statement);

      break;

    default:
        LDV_PRETTY_PRINT_ERROR (indent_level, "selection statement was not printed");
    }

  LDV_XDELETE_ON_PRINTING (selection_statement);
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
static void
ldv_print_shift_expr (unsigned int indent_level, ldv_shift_expr_ptr shift_expr)
{
  ldv_additive_expr_ptr additive_expr;
  ldv_shift_expr_ptr shift_expr_next;
  ldv_location_ptr location;

  switch (LDV_SHIFT_EXPR_KIND (shift_expr))
    {
    case LDV_SHIFT_EXPR_FIRST:
      if ((additive_expr = LDV_SHIFT_EXPR_ADDITIVE_EXPR (shift_expr)))
        ldv_print_additive_expr (indent_level, additive_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "additive expression of shift expression was not printed");

      break;

    case LDV_SHIFT_EXPR_SECOND:
    case LDV_SHIFT_EXPR_THIRD:
      if ((shift_expr_next = LDV_SHIFT_EXPR_SHIFT_EXPR (shift_expr)))
        ldv_print_shift_expr (indent_level, shift_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "shift expression of shift expression was not printed");

      if ((location = LDV_SHIFT_EXPR_LOCATION (shift_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if (LDV_SHIFT_EXPR_KIND (shift_expr) == LDV_SHIFT_EXPR_SECOND)
        ldv_c_backend_print (indent_level, true, "<<");
      else if (LDV_SHIFT_EXPR_KIND (shift_expr) == LDV_SHIFT_EXPR_THIRD)
        ldv_c_backend_print (indent_level, true, ">>");

      if ((additive_expr = LDV_SHIFT_EXPR_ADDITIVE_EXPR (shift_expr)))
        ldv_print_additive_expr (indent_level, additive_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "additive expression of shift expression was not printed");

      break;

    case LDV_SHIFT_EXPR_FOURTH:
    case LDV_SHIFT_EXPR_FIFTH:
      ldv_c_backend_print (indent_level, true, "(");

      ldv_c_backend_print (indent_level, true, "(");

      /* Do not free memory for expressions since they are reused below. */
      ldv_free_on_printing = false;

      if ((shift_expr_next = LDV_SHIFT_EXPR_SHIFT_EXPR (shift_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_shift_expr (indent_level, shift_expr_next);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "shift expression of shift expression was not printed");

      if (LDV_SHIFT_EXPR_KIND (shift_expr) == LDV_SHIFT_EXPR_FOURTH)
        ldv_c_backend_print (indent_level, true, "<<");
      else if (LDV_SHIFT_EXPR_KIND (shift_expr) == LDV_SHIFT_EXPR_FIFTH)
        ldv_c_backend_print (indent_level, true, ">>");

      if ((additive_expr = LDV_SHIFT_EXPR_ADDITIVE_EXPR (shift_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_additive_expr (indent_level, additive_expr);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "additive expression of shift expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      ldv_c_backend_print (indent_level, true, "|");

      ldv_c_backend_print (indent_level, true, "(");

      if ((shift_expr_next = LDV_SHIFT_EXPR_SHIFT_EXPR (shift_expr)))
        {
          ldv_c_backend_print (indent_level, true, "(");
          ldv_print_shift_expr (indent_level, shift_expr_next);
          ldv_c_backend_print (indent_level, true, ")");
        }
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "shift expression of shift expression was not printed");

      ldv_free_on_printing = true;

      if (LDV_SHIFT_EXPR_KIND (shift_expr) == LDV_SHIFT_EXPR_FOURTH)
        ldv_c_backend_print (indent_level, true, "<<");
      else if (LDV_SHIFT_EXPR_KIND (shift_expr) == LDV_SHIFT_EXPR_FIFTH)
        ldv_c_backend_print (indent_level, true, ">>");

      ldv_c_backend_print (indent_level, true, "(8 * sizeof(");

      if ((shift_expr_next = LDV_SHIFT_EXPR_SHIFT_EXPR (shift_expr)))
        ldv_print_shift_expr (indent_level, shift_expr_next);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "shift expression of shift expression was not printed");

      ldv_c_backend_print (indent_level, true, ") - (");

      if ((additive_expr = LDV_SHIFT_EXPR_ADDITIVE_EXPR (shift_expr)))
        ldv_print_additive_expr (indent_level, additive_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "additive expression of shift expression was not printed");

      ldv_c_backend_print (indent_level, true, ")");

      ldv_c_backend_print (indent_level, true, ")");

      ldv_c_backend_print (indent_level, true, ")");

      ldv_c_backend_print (indent_level, true, ")");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "shift expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (shift_expr);
}

/*
specifier-qualifier-list:
    type-specifier specifier-qualifier-listopt
    type-qualifier specifier-qualifier-listopt
*/
static void
ldv_print_spec_qual_list (unsigned int indent_level, ldv_spec_qual_list_ptr spec_qual_list)
{
  ldv_type_spec_ptr type_spec;
  ldv_type_qual_ptr type_qual;
  ldv_spec_qual_list_ptr spec_qual_list_next;

  if ((type_spec = LDV_SPEC_QUAL_LIST_TYPE_SPEC (spec_qual_list)))
    ldv_print_type_spec (indent_level, type_spec);
  else if ((type_qual = LDV_SPEC_QUAL_LIST_TYPE_QUAL (spec_qual_list)))
    ldv_print_type_qual (indent_level, type_qual);
  else
    LDV_ERROR ("neither type specifier nor type qualifier was printed");

  if ((spec_qual_list_next = LDV_SPEC_QUAL_LIST_SPEC_QUAL_LIST (spec_qual_list)))
    ldv_print_spec_qual_list (indent_level, spec_qual_list_next);

  LDV_XDELETE_ON_PRINTING (spec_qual_list);
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
static void
ldv_print_statement (unsigned int indent_level, ldv_statement_ptr statement)
{
  ldv_labeled_statement_ptr labeled_statement;
  ldv_compound_statement_ptr compound_statement;
  ldv_expr_statement_ptr expr_statement;
  ldv_selection_statement_ptr selection_statement;
  ldv_jump_statement_ptr jump_statement;
  ldv_asm_statement_ptr asm_statement;

  switch (LDV_STATEMENT_KIND (statement))
    {
    case LDV_LABELED_STATEMENT:
      if ((labeled_statement = LDV_STATEMENT_LABELED_STATEMENT (statement)))
        ldv_print_labeled_statement (indent_level, labeled_statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "labeled statement of statement was not printed");

      break;

    case LDV_COMPOUND_STATEMENT:
      if ((compound_statement = LDV_STATEMENT_COMPOUND_STATEMENT (statement)))
        ldv_print_compound_statement (indent_level, compound_statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "compound statement of statement was not printed");

      break;

    case LDV_EXPR_STATEMENT:
      if ((expr_statement = LDV_STATEMENT_EXPR_STATEMENT (statement)))
        ldv_print_expr_statement (indent_level, expr_statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "expression statement of statement was not printed");

      break;

    case LDV_SELECTION_STATEMENT:
      if ((selection_statement = LDV_STATEMENT_SELECTION_STATEMENT (statement)))
        ldv_print_selection_statement (indent_level, selection_statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "selection statement of statement was not printed");

      break;

    case LDV_ITERATION_STATEMENT:
      LDV_PRETTY_PRINT_ERROR (indent_level, "iteration statement of statement was not printed");

      break;

    case LDV_JUMP_STATEMENT:
      if ((jump_statement = LDV_STATEMENT_JUMP_STATEMENT (statement)))
        ldv_print_jump_statement (indent_level, jump_statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "jump statement of statement was not printed");

      break;

    case LDV_ASM_STATEMENT:
      if ((asm_statement = LDV_STATEMENT_ASM_STATEMENT (statement)))
        ldv_print_asm_statement (indent_level, asm_statement);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "asm statement of statement was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "statement was not printed");
    }

  LDV_XDELETE_ON_PRINTING (statement);
}

static void
ldv_print_statement_internal (unsigned int indent_level, ldv_statement_ptr statement)
{
  if (LDV_STATEMENT_COMPOUND_STATEMENT (statement))
    ldv_print_statement (indent_level, statement);
  else
    ldv_print_statement (indent_level + 1, statement);
}

/*
storage-class-specifier:
    typedef
    extern
    static
    auto
    register
*/
static void
ldv_print_storage_class_spec (unsigned int indent_level, ldv_storage_class_spec_ptr storage_class_spec)
{
  switch (LDV_STORAGE_CLASS_SPEC_KIND (storage_class_spec))
    {
    case LDV_STORAGE_CLASS_SPEC_TYPEDEF:
      ldv_c_backend_print (indent_level, true, "typedef");
      break;

    case LDV_STORAGE_CLASS_SPEC_EXTERN:
      ldv_c_backend_print (indent_level, true, "extern");
      break;

    case LDV_STORAGE_CLASS_SPEC_STATIC:
      ldv_c_backend_print (indent_level, true, "static");
      break;

    case LDV_STORAGE_CLASS_SPEC_AUTO:
      ldv_c_backend_print (indent_level, true, "auto");
      break;

    case LDV_STORAGE_CLASS_SPEC_REGISTER:
      ldv_c_backend_print (indent_level, true, "register");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "storage class specifier was not printed");
    }

  LDV_XDELETE_ON_PRINTING (storage_class_spec);
}

/*
struct-or-union:
    struct
    union
*/
static void
ldv_print_struct_or_union (unsigned int indent_level, ldv_struct_or_union_ptr struct_or_union)
{
  switch (LDV_STRUCT_OR_UNION_SPEC_STRUCT_OR_UNION (struct_or_union))
    {
    case LDV_STRUCT_OR_UNION_STRUCT:
      ldv_c_backend_print (indent_level, true, "struct");
      break;

    case LDV_STRUCT_OR_UNION_UNION:
      ldv_c_backend_print (indent_level, true, "union");
      break;
    }

  LDV_XDELETE_ON_PRINTING (struct_or_union);
}

/* String literal is considered as the sequence of UTF-8 encoded symbols.
string-literal:
    " s-char-sequenceopt "
    L" s-char-sequenceopt "
*/
static void
ldv_print_str_literal (unsigned int indent_level, ldv_str_literal_ptr str_literal)
{
  const char *str;
  const char *c;
  ldv_str_ptr escaped_str;

  /* A given string can contain some escape sequences. Escape them before
     printing. */
  if ((str = LDV_STR_LITERAL_STR (str_literal)))
    {
      c = str;
      escaped_str = ldv_create_string ();

      if (*c == 'L')
        {
          c++;
          ldv_putc_string ('L', escaped_str);
        }

      if (*c == '"')
        c++;

      ldv_putc_string ('"', escaped_str);

      for (; *c; c++)
        {
          if (!*(c + 1) && *c == '"')
            break;

          switch (*c)
            {
            case '\a':
              ldv_puts_string ("\a", escaped_str);
              break;

            case '\b':
              ldv_puts_string ("\b", escaped_str);
              break;

            case '\f':
              ldv_puts_string ("\f", escaped_str);
              break;

            case '\n':
              ldv_puts_string ("\n", escaped_str);
              break;

            case '\r':
              ldv_puts_string ("\r", escaped_str);
              break;

            case '\t':
              ldv_puts_string ("\t", escaped_str);
              break;

            case '\v':
              ldv_puts_string ("\v", escaped_str);
              break;

            case '\\':
              ldv_puts_string ("\\", escaped_str);
              break;

            case '"':
              ldv_puts_string ("\"", escaped_str);
              break;

            case '\'':
              ldv_puts_string ("\'", escaped_str);
              break;

            default:
              ldv_putc_string (*c, escaped_str);
            }
        }

      ldv_putc_string ('"', escaped_str);
      ldv_c_backend_print (indent_level, true, "%s", ldv_get_str (escaped_str));
      ldv_free_string (escaped_str);
    }
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "string literal was not printed");

  LDV_XDELETE_ON_PRINTING (str_literal);
}

/*
struct-declaration:
    specifier-qualifier-list struct-declarator-list ;

GNU extensions:

struct-declaration:
    specifier-qualifier-list
*/
static void
ldv_print_struct_decl (unsigned int indent_level, ldv_struct_decl_ptr struct_decl)
{
  ldv_spec_qual_list_ptr spec_qual_list;
  ldv_struct_declarator_list_ptr struct_declarator_list;

  if ((spec_qual_list = LDV_STRUCT_DECL_SPEC_QUAL_LIST (struct_decl)))
    ldv_print_spec_qual_list (indent_level, spec_qual_list);
  else
    LDV_ERROR ("struct declaration specifier list wasn't printed");

  if ((struct_declarator_list = LDV_STRUCT_DECL_STRUCT_DECLARATOR_LIST (struct_decl)))
    ldv_print_struct_declarator_list (indent_level, struct_declarator_list);

  ldv_c_backend_print (indent_level, false, ";");

  LDV_XDELETE_ON_PRINTING (struct_decl);
}

/*
struct-declaration-list:
    struct-declaration
    struct-declaration-list struct-declaration
*/
static void
ldv_print_struct_decl_list (unsigned int indent_level, ldv_struct_decl_list_ptr struct_decl_list)
{
  ldv_struct_decl_list_ptr struct_decl_list_next;
  ldv_struct_decl_ptr struct_decl;

  if ((struct_decl_list_next = LDV_STRUCT_DECL_LIST_STRUCT_DECL_LIST (struct_decl_list)))
    {
      ldv_print_struct_decl_list (indent_level, struct_decl_list_next);
      ldv_c_backend_print (indent_level, false, "\n");
    }

  if ((struct_decl = LDV_STRUCT_DECL_LIST_STRUCT_DECL (struct_decl_list)))
    ldv_print_struct_decl (indent_level, struct_decl);
  else
    LDV_ERROR ("struct declaration list struct declaration wasn't printed");

  LDV_XDELETE_ON_PRINTING (struct_decl_list);
}

/*
struct-declarator:
    declarator
    declaratoropt : constant-expression
*/
static void
ldv_print_struct_declarator (unsigned int indent_level, ldv_struct_declarator_ptr struct_declarator)
{
  ldv_declarator_ptr declarator;
  ldv_location_ptr location;
  ldv_integer_constant_ptr const_expr;

  if ((declarator = LDV_STRUCT_DECLARATOR_DECLARATOR (struct_declarator)))
    {
      /* Print #line directive before a given field declaration. */
      if ((location = ldv_declarator_location (declarator)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_DECL, location);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "line directive for field declaration was not printed");

      ldv_print_declarator (indent_level, declarator);
    }

  if ((const_expr = LDV_STRUCT_DECLARATOR_CONST_EXPR (struct_declarator)))
    {
      ldv_c_backend_print (indent_level, true, ":");
      ldv_print_integer_constant (indent_level, const_expr);
    }

  if (!declarator && !const_expr)
    LDV_ERROR ("neither struct declarator declarator nor constant expression was printed");

  LDV_XDELETE_ON_PRINTING (struct_declarator);
}

/*
struct-declarator-list:
    struct-declarator
    struct-declarator-list , struct-declarator
*/
static void
ldv_print_struct_declarator_list (unsigned int indent_level, ldv_struct_declarator_list_ptr struct_declarator_list)
{
  ldv_struct_declarator_list_ptr struct_declarator_list_next;
  ldv_struct_declarator_ptr struct_declarator;

  if ((struct_declarator_list_next = LDV_STRUCT_DECLARATOR_LIST_STRUCT_DECLARATOR_LIST (struct_declarator_list)))
    {
      ldv_print_struct_declarator_list (indent_level, struct_declarator_list_next);
      ldv_c_backend_print (indent_level, false, ",");
    }

  if ((struct_declarator = LDV_STRUCT_DECLARATOR_LIST_STRUCT_DECLARATOR (struct_declarator_list)))
    ldv_print_struct_declarator (indent_level, struct_declarator);
  else
    LDV_ERROR ("struct declarator list struct declarator wasn't printed");

  LDV_XDELETE_ON_PRINTING (struct_declarator_list);
}

/*
struct-or-union-specifier:
    struct-or-union identifieropt { struct-declaration-list }
    struct-or-union identifier

GNU extensions:

struct-or-union-specifier:
    struct-or-union identifieropt { }
*/
static void
ldv_print_struct_or_union_spec (unsigned int indent_level, ldv_struct_or_union_spec_ptr struct_or_union_spec)
{
  ldv_struct_or_union_ptr struct_or_union;
  ldv_struct_decl_list_ptr struct_decl_list;
  const char *struct_or_union_name;

  switch (LDV_STRUCT_OR_UNION_SPEC_KIND (struct_or_union_spec))
    {
    case LDV_STRUCT_OR_UNION_SPEC_FIRST:
    case LDV_STRUCT_OR_UNION_SPEC_SECOND:
    case LDV_STRUCT_OR_UNION_SPEC_THIRD:
      if ((struct_or_union = LDV_STRUCT_OR_UNION_SPEC_STRUCT_OR_UNION (struct_or_union_spec)))
        ldv_print_struct_or_union (indent_level, struct_or_union);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "struct or union of struct or union specifier was not printed");

      if ((struct_or_union_name = LDV_STRUCT_OR_UNION_SPEC_ID (struct_or_union_spec)))
        ldv_c_backend_print (indent_level, true, "%s", struct_or_union_name);
      else if (LDV_STRUCT_OR_UNION_SPEC_KIND (struct_or_union_spec) == LDV_STRUCT_OR_UNION_SPEC_SECOND)
        LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of struct or union specifier was not printed");

      if (LDV_STRUCT_OR_UNION_SPEC_KIND (struct_or_union_spec) != LDV_STRUCT_OR_UNION_SPEC_SECOND)
        {
          ldv_c_backend_print (indent_level, false, "\n");
          ldv_c_backend_print (indent_level, false, "{\n");

          if (LDV_STRUCT_OR_UNION_SPEC_KIND (struct_or_union_spec) == LDV_STRUCT_OR_UNION_SPEC_FIRST)
            {
              if ((struct_decl_list = LDV_STRUCT_OR_UNION_SPEC_STRUCT_DECL_LIST (struct_or_union_spec)))
                ldv_print_struct_decl_list (indent_level + 1, struct_decl_list);
              else
                LDV_PRETTY_PRINT_ERROR (indent_level, "struct declaration list of struct or union specifier was not printed");

              ldv_c_backend_print (indent_level, false, "\n");
            }

          ldv_c_backend_print (indent_level, true, "}");
        }

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "struct or union specifier was not printed");
    }

  LDV_XDELETE_ON_PRINTING (struct_or_union_spec);
}

char *
ldv_convert_and_print_assignment_expr (tree t)
{
  ldv_assignment_expr_ptr assignment_expr = NULL;
  int current_lines_level;

  assignment_expr = ldv_convert_assignment_expr (t, LDV_CONVERT_EXPR_RECURSION_LIMIT);

  /* Store current lines level and set it to LDV_C_BACKEND_LINES_LEVEL_NO to
     avoid printing of lines directives. */
  current_lines_level = ldv_c_backend_get_lines_level ();
  ldv_c_backend_set_lines_level (LDV_C_BACKEND_LINES_LEVEL_NO);
  ldv_c_backend_print_to_buffer ();
  /* See comment for this variable.
     Since the only caller is Aspectator, we can implement the workaround in
     such the way. */
  ldv_disable_cast_printing = true;
  /* We do not need corresponding entities after printing is completed. */
  ldv_free_on_printing = true;

  ldv_print_assignment_expr (0, assignment_expr);

  ldv_free_on_printing = false;
  ldv_disable_cast_printing = false;
  ldv_c_backend_padding_cancel ();
  ldv_c_backend_print_to_file ();
  ldv_c_backend_set_lines_level (current_lines_level);

  return ldv_c_backend_get_buffer ();
}

char *
ldv_convert_and_print_decl (tree t)
{
  ldv_decl_ptr decl = NULL;
  int current_lines_level;

  decl = ldv_convert_decl (t);

  /* Store current lines level and set it to LDV_C_BACKEND_LINES_LEVEL_NO to
     avoid printing of lines directives. */
  current_lines_level = ldv_c_backend_get_lines_level ();
  ldv_c_backend_set_lines_level (LDV_C_BACKEND_LINES_LEVEL_NO);
  ldv_c_backend_print_to_buffer ();
  /* We do not need corresponding entities after printing is completed. */
  ldv_free_on_printing = true;

  ldv_print_decl (0, decl);

  ldv_free_on_printing = false;
  ldv_c_backend_padding_cancel ();
  ldv_c_backend_print_to_file ();
  ldv_c_backend_set_lines_level (current_lines_level);

  return ldv_c_backend_get_buffer ();
}

char *
ldv_convert_and_print_struct_decl (tree t)
{
  ldv_struct_decl_ptr struct_decl = NULL;
  int current_lines_level;

  struct_decl = ldv_convert_struct_decl (t);

  /* Store current lines level and set it to LDV_C_BACKEND_LINES_LEVEL_NO to
     avoid printing of lines directives. */
  current_lines_level = ldv_c_backend_get_lines_level ();
  ldv_c_backend_set_lines_level (LDV_C_BACKEND_LINES_LEVEL_NO);
  ldv_c_backend_print_to_buffer ();
  /* We do not need corresponding entities after printing is completed. */
  ldv_free_on_printing = true;

  ldv_print_struct_decl (0, struct_decl);

  ldv_free_on_printing = false;
  ldv_c_backend_padding_cancel ();
  ldv_c_backend_print_to_file ();
  ldv_c_backend_set_lines_level (current_lines_level);

  return ldv_c_backend_get_buffer ();
}

/*
translation-unit:
    external-declaration
    translation-unit external-declaration
*/
void
ldv_print_translation_unit (tree t, bool isdecl)
{
  ldv_translation_unit_ptr translation_unit;
  ldv_ext_decl_ptr ext_decl;

  /* To prevent redefinition of global variables print them just one time. In
     fact this causes problem when we have something like this in source code:
       int dupl = 0;
       extern int dupl;
     that after all becomes:
       int dupl = 0;
       int dupl = 0;
     Filter entities here to prevent any memory allocation and warning messages.
   */
  if (ldv_isglobal_var_was_printed (t))
    return;

  /* Original source code can define the same type names several times.
     However, GCC represents them differently and C back-end can't process
     corresponding typedefs similarly. Since typedef duplicates are redundant,
     simply skip them. */
  if (TREE_CODE (t) == TYPE_DECL && !DECL_ORIGINAL_TYPE (t))
    return;

  /* This is the artificial function added just to correspond to the C standard.
     We don't collect all external declaration together now and print them
     directly after converting to the internal representation. Generally
     speaking this means that there is no corresponding converting function for
     this one. May be in future this will be useful. */
  translation_unit = XCNEW (struct ldv_translation_unit);

  if ((ext_decl = ldv_convert_ext_decl (t, isdecl)))
    {
      LDV_TRANSLATION_UNIT_EXT_DECL (translation_unit) = ext_decl;

      /* We do not need corresponding entities after printing is completed. */
      ldv_free_on_printing = true;

      ldv_print_ext_decl (0, LDV_TRANSLATION_UNIT_EXT_DECL (translation_unit));

      ldv_free_on_printing = false;
    }
  else
    LDV_PRETTY_PRINT_ERROR (0, "external declaration was not printed");

  /* Separate external declarations from each other. */
  ldv_c_backend_padding_cancel ();
  ldv_c_backend_print (0, false, "\n");

  XDELETE (translation_unit);
}

/*
type-name:
    specifier-qualifier-list abstract-declaratoropt
*/
static void
ldv_print_type_name (unsigned int indent_level, ldv_type_name_ptr type_name)
{
  ldv_spec_qual_list_ptr spec_qual_list;
  ldv_abstract_declarator_ptr abstract_declarator;

  if ((spec_qual_list = LDV_TYPE_NAME_SPEC_QUAL_LIST (type_name)))
    ldv_print_spec_qual_list (indent_level, spec_qual_list);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "specifier qualifier list of type name wasn't printed");

  if ((abstract_declarator = LDV_TYPE_NAME_ABSTRACT_DECLARATOR (type_name)))
    ldv_print_abstract_declarator (indent_level, abstract_declarator);

  LDV_XDELETE_ON_PRINTING (type_name);
}

/*
type-qualifier:
    const
    restrict
    volatile

*/
static void
ldv_print_type_qual (unsigned int indent_level, ldv_type_qual_ptr type_qual)
{
  switch (LDV_TYPE_QUAL_KIND (type_qual))
    {
    case LDV_TYPE_QUAL_CONST:
      ldv_c_backend_print (indent_level, true, "const");
      break;

    case LDV_TYPE_QUAL_RESTRICT:
      ldv_c_backend_print (indent_level, true, "restrict");
      break;

    case LDV_TYPE_QUAL_VOLATILE:
      ldv_c_backend_print (indent_level, true, "volatile");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "type qualifier was not printed");
    }

  LDV_XDELETE_ON_PRINTING (type_qual);
}

/*
type-qualifier-list:
    type-qualifier
    type-qualifier-list type-qualifier
*/
static void
ldv_print_type_qual_list (unsigned int indent_level, ldv_type_qual_list_ptr type_qual_list)
{
  ldv_type_qual_ptr type_qual;
  ldv_type_qual_list_ptr type_qual_list_next;

  if ((type_qual = LDV_TYPE_QUAL_LIST_TYPE_QUAL (type_qual_list)))
    ldv_print_type_qual (indent_level, type_qual);
  else
    LDV_ERROR ("type qualifier wasn't printed");

  if ((type_qual_list_next = LDV_TYPE_QUAL_LIST_TYPE_QUAL_LIST (type_qual_list)))
    ldv_print_type_qual_list (indent_level, type_qual_list_next);

  LDV_XDELETE_ON_PRINTING (type_qual_list);
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

ISO/IEC TS 18661-3:2015 extensions:

type-specifier:
    _Float32
    _Float32x
    _Float64
    _Float64x
    _Float128
*/
static void
ldv_print_type_spec (unsigned int indent_level, ldv_type_spec_ptr type_spec)
{
  ldv_struct_or_union_spec_ptr struct_or_union_spec;
  ldv_enum_spec_ptr enum_spec;
  ldv_typedef_name_ptr typedef_name;

  switch (LDV_TYPE_SPEC_KIND (type_spec))
    {
    case LDV_TYPE_SPEC_VOID:
      ldv_c_backend_print (indent_level, true, "void");
      break;

    case LDV_TYPE_SPEC_CHAR:
      ldv_c_backend_print (indent_level, true, "char");
      break;

    case LDV_TYPE_SPEC_SHORT:
      ldv_c_backend_print (indent_level, true, "short");
      break;

    case LDV_TYPE_SPEC_INT:
      ldv_c_backend_print (indent_level, true, "int");
      break;

    case LDV_TYPE_SPEC_LONG:
      ldv_c_backend_print (indent_level, true, "long");
      break;

    case LDV_TYPE_SPEC_FLOAT:
      ldv_c_backend_print (indent_level, true, "float");
      break;

    case LDV_TYPE_SPEC_DOUBLE:
      ldv_c_backend_print (indent_level, true, "double");
      break;

    case LDV_TYPE_SPEC_SIGNED:
      ldv_c_backend_print (indent_level, true, "signed");
      break;

    case LDV_TYPE_SPEC_UNSIGNED:
      ldv_c_backend_print (indent_level, true, "unsigned");
      break;

    case LDV_TYPE_SPEC_BOOL:
      ldv_c_backend_print (indent_level, true, "_Bool");
      break;

    case LDV_TYPE_SPEC_COMPLEX:
      ldv_c_backend_print (indent_level, true, "_Complex");
      break;

    case LDV_TYPE_SPEC_FIRST:
      if ((struct_or_union_spec = LDV_TYPE_SPEC_STRUCT_OR_UNION_SPEC (type_spec)))
        ldv_print_struct_or_union_spec (indent_level, struct_or_union_spec);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "struct or union specifier of type specifier was not printed");

      break;

    case LDV_TYPE_SPEC_SECOND:
      if ((enum_spec = LDV_TYPE_SPEC_ENUM_SPEC (type_spec)))
        ldv_print_enum_spec (indent_level, enum_spec);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "enum specifier of type specifier was not printed");

      break;

    case LDV_TYPE_SPEC_THIRD:
      if ((typedef_name = LDV_TYPE_SPEC_TYPEDEF_NAME (type_spec)))
        ldv_print_typedef_name (indent_level, typedef_name);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "typedef name of type specifier was not printed");

      break;

    case LDV_TYPE_SPEC_FLOAT32:
      ldv_c_backend_print (indent_level, true, "_Float32");
      break;

    case LDV_TYPE_SPEC_FLOAT32X:
      ldv_c_backend_print (indent_level, true, "_Float32x");
      break;

    case LDV_TYPE_SPEC_FLOAT64:
      ldv_c_backend_print (indent_level, true, "_Float64");
      break;

    case LDV_TYPE_SPEC_FLOAT64X:
      ldv_c_backend_print (indent_level, true, "_Float64x");
      break;

    case LDV_TYPE_SPEC_FLOAT128:
      ldv_c_backend_print (indent_level, true, "_Float128");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "type specifier was not printed");
    }

  LDV_XDELETE_ON_PRINTING (type_spec);
}

/*
typedef-name:
    identifier
*/
static void
ldv_print_typedef_name (unsigned int indent_level, ldv_typedef_name_ptr typedef_name)
{
  ldv_identifier_ptr identifier;

  if ((identifier = LDV_TYPEDEF_NAME_IDENTIFIER (typedef_name)))
    ldv_print_identifier (indent_level, identifier);
  else
    LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of typedef name was not printed");

  LDV_XDELETE_ON_PRINTING (typedef_name);
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
static void
ldv_print_unary_expr (unsigned int indent_level, ldv_unary_expr_ptr unary_expr)
{
  ldv_postfix_expr_ptr postfix_expr;
  ldv_unary_expr_ptr unary_expr_next;
  ldv_unary_operator_ptr unary_operator;
  ldv_cast_expr_ptr cast_expr;
  ldv_location_ptr location;
  ldv_identifier_ptr identifier;

  switch (LDV_UNARY_EXPR_KIND (unary_expr))
    {
    case LDV_UNARY_EXPR_FIRST:
      if ((postfix_expr = LDV_UNARY_EXPR_POSTFIX_EXPR (unary_expr)))
        ldv_print_postfix_expr (indent_level, postfix_expr);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "postfix expression of unary expression was not printed");

      break;

    case LDV_UNARY_EXPR_SECOND:
    case LDV_UNARY_EXPR_THIRD:
    case LDV_UNARY_EXPR_FOURTH:
      if ((location = LDV_UNARY_EXPR_LOCATION (unary_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      if (LDV_UNARY_EXPR_KIND (unary_expr) == LDV_UNARY_EXPR_SECOND)
        ldv_c_backend_print (indent_level, true, "++");
      else if (LDV_UNARY_EXPR_KIND (unary_expr) == LDV_UNARY_EXPR_THIRD)
        ldv_c_backend_print (indent_level, true, "--");

      if (LDV_UNARY_EXPR_KIND (unary_expr) == LDV_UNARY_EXPR_FOURTH)
        {
          if ((unary_operator = LDV_UNARY_EXPR_UNARY_OPERATOR (unary_expr)))
            ldv_print_unary_operator (indent_level, unary_operator);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "unary operator of unary expression was not printed");

          if ((cast_expr = LDV_UNARY_EXPR_CAST_EXPR (unary_expr)))
            ldv_print_cast_expr (indent_level, cast_expr);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "cast expression of unary expression was not printed");
        }
      else
        {
          if ((unary_expr_next = LDV_UNARY_EXPR_UNARY_EXPR (unary_expr)))
            ldv_print_unary_expr (indent_level, unary_expr_next);
          else
            LDV_PRETTY_PRINT_ERROR (indent_level, "unary expression of unary expression was not printed");
        }

      break;

    /* Taking address of a label */
    case LDV_UNARY_EXPR_FIFTH:
      if ((location = LDV_UNARY_EXPR_LOCATION (unary_expr)))
        ldv_print_line_directive (true, LDV_C_BACKEND_LINES_LEVEL_EXPR, location);

      ldv_c_backend_print (indent_level, true, "&&");

      if ((identifier = LDV_UNARY_EXPR_IDENTIFIER (unary_expr)))
        ldv_print_identifier (indent_level, identifier);
      else
        LDV_PRETTY_PRINT_ERROR (indent_level, "identifier of unary expression was not printed");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "unary expression was not printed");
    }

  LDV_XDELETE_ON_PRINTING (unary_expr);
}

/*
unary-operator: one of
    & * + - ~ !
*/
static void
ldv_print_unary_operator (unsigned int indent_level, ldv_unary_operator_ptr unary_operator)
{
  switch (LDV_UNARY_OPERATOR_KIND (unary_operator))
    {
    case LDV_UNARY_OPERATOR_FIRST:
      ldv_c_backend_print (indent_level, true, "&");

      break;

    case LDV_UNARY_OPERATOR_SECOND:
      ldv_c_backend_print (indent_level, true, "*");

      break;

    case LDV_UNARY_OPERATOR_THIRD:
      ldv_c_backend_print (indent_level, true, "+");

      break;

    case LDV_UNARY_OPERATOR_FOURTH:
      ldv_c_backend_print (indent_level, true, "-");

      break;

    case LDV_UNARY_OPERATOR_FIVTH:
      ldv_c_backend_print (indent_level, true, "~");

      break;

    case LDV_UNARY_OPERATOR_SIXTH:
      ldv_c_backend_print (indent_level, true, "!");

      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "unary operator was not printed");
    }

  LDV_XDELETE_ON_PRINTING (unary_operator);
}

/*
unsigned-suffix: one of
    u U
*/
static void
ldv_print_unsigned_suffix (unsigned int indent_level, ldv_unsigned_suffix_ptr unsigned_suffix)
{
  /* Join suffix to the previously printed integer constant. */
  ldv_c_backend_padding_cancel ();

  switch (LDV_UNSIGNED_SUFFIX_KIND (unsigned_suffix))
    {
    case LDV_UNSIGNED_SUFFIX_FIRST:
      ldv_c_backend_print (indent_level, true, "u");
      break;

    case LDV_UNSIGNED_SUFFIX_SECOND:
      ldv_c_backend_print (indent_level, true, "U");
      break;

    default:
      LDV_PRETTY_PRINT_ERROR (indent_level, "unsigned suffix was not printed");
    }

  LDV_XDELETE_ON_PRINTING (unsigned_suffix);
}
