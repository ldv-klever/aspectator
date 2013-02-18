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

/* For build_function_call function. */
#include "c-lang.h"

/* For error functions. */
#include "diagnostic-core.h"
#include "toplev.h"

/* Statement list iterator. */
#include "tree-iterator.h"

/* For function structure. */
#include "function.h"

#include "ldv-advice-weaver.h"
#include "ldv-aspect-parser.h"
#include "ldv-converter.h"
#include "ldv-core.h"
#include "ldv-cpp-pointcut-matcher.h"
#include "ldv-io.h"
#include "ldv-opts.h"
#include "ldv-pointcut-matcher.h"


#define LDV_ARRAY_SIZE(t) (TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE ? TYPE_DOMAIN (TREE_TYPE (t)) ? TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (t))) : NULL : NULL)

#define LDV_OFFSET_NEXT(offset) (offset + 2)

#define LDV_OP1 TREE_OPERAND (t, 0)
#define LDV_OP2 TREE_OPERAND (t, 1)
#define LDV_OP3 TREE_OPERAND (t, 2)
#define LDV_OP4 TREE_OPERAND (t, 3)

/* This is copypasted from C-backend. */
#define LDV_CONVERT_WARN(t) error ("LDV: %s: %d: tree node '%s' isn't supported", __FILE__, __LINE__, LDV_TREE_NODE_NAME (t))
#define LDV_TREE_NODE_NAME(t) (tree_code_name[(int) TREE_CODE (t)])


typedef struct ldv_var_array_internal
{
  unsigned int uid;
  unsigned int array_size;
  bool issize_specified;
} ldv_var_array;
typedef ldv_var_array *ldv_var_array_ptr;


ldv_pps_declspecs_ptr ldv_entity_declspecs = NULL;
ldv_list_ptr ldv_func_arg_info_list = NULL;
tree ldv_func_called_matched = NULL_TREE;
tree ldv_func_decl_matched = NULL_TREE;
ldv_i_match_ptr ldv_i_match = NULL;

static ldv_list_ptr ldv_var_array_list;

static tree cur_var;
static bool isfunc_vars;


static unsigned int ldv_array_field_size (tree);
static ldv_func_arg_info_ptr ldv_create_func_arg_info (void);
static ldv_var_array_ptr ldv_create_var_array (void);
static const char *ldv_get_arg_sign (tree, enum ldv_arg_signs);
static void ldv_match_expr (tree);
static void ldv_visualize_expr (tree, int);
static void ldv_visualize_body (tree);


unsigned int
ldv_array_field_size (tree t)
{
  tree array_size = NULL_TREE, op11 = NULL_TREE, op112 = NULL_TREE;

  /* Check that a record field has an array type that has a size.
     ADDR_EXPR
       ARRAY_REF
         COMPONENT_REF
           ...
           FIELD_DECL -> ARRAY_TYPE -> TYPE_DOMAIN -> TYPE_MAX_VALUE
   */
  if (TREE_CODE (t) == ADDR_EXPR
    && TREE_CODE (LDV_OP1) == ARRAY_REF
    && (op11 = TREE_OPERAND (LDV_OP1, 0))
    && TREE_CODE (op11) == COMPONENT_REF
    && (op112 = TREE_OPERAND (op11, 1))
    && TREE_CODE (op112) == FIELD_DECL
    && (array_size = LDV_ARRAY_SIZE (op112)))
    /* Return a corresponding array size. */
    return (TREE_INT_CST_LOW (array_size) + 1);

  /* This means that no size was found. */
  return 0;
}

ldv_func_arg_info_ptr
ldv_create_func_arg_info (void)
{
  ldv_func_arg_info_ptr func_arg_info;

  func_arg_info = XCNEW (ldv_func_arg_info);
  ldv_print_info (LDV_INFO_MEM, "function argument information memory was released");

  func_arg_info->func_arg_info_kind = LDV_FUNC_ARG_INFO_NONE;

  return func_arg_info;
}

ldv_var_array_ptr
ldv_create_var_array (void)
{
  ldv_var_array_ptr var_array;

  var_array = XCNEW (ldv_var_array);
  ldv_print_info (LDV_INFO_MEM, "variable array memory was released");

  return var_array;
}

const char *
ldv_get_arg_sign (tree t, enum ldv_arg_signs ldv_arg_sign)
{
  tree record_type = NULL_TREE, op1 = NULL_TREE, op2 = NULL_TREE;
  const char *arg_sign = NULL, *field_sign = NULL, *struct_sign = NULL;
  char *arg_sign_p = NULL;

  /* Skip any '*' and '&' used before identifiers. */
  if (t && (TREE_CODE (t) == ADDR_EXPR || TREE_CODE (t) == INDIRECT_REF))
    {
      op1 = TREE_OPERAND (t, 0);
      return ldv_get_arg_sign (op1, ldv_arg_sign);
    }

  /* Argument signature equals to declaration name passed. */
  if (t && DECL_P (t) && DECL_NAME (t)
    && (TREE_CODE (DECL_NAME (t)) == IDENTIFIER_NODE))
    arg_sign = IDENTIFIER_POINTER (DECL_NAME (t));
  /* Argument signature equals to type name passed. */
  else if (t && (TREE_CODE (t) == RECORD_TYPE || TREE_CODE (t) == UNION_TYPE)
    && TYPE_NAME (t))
    arg_sign = IDENTIFIER_POINTER (TYPE_NAME (t));
  /* Argument signature equals to field name possibly complemented with a number
   * of names of structures containing it. */
  else if (t && TREE_CODE (t) == COMPONENT_REF)
    {
      /* Calculate field signature. */
      op2 = TREE_OPERAND (t, 1);

      if (DECL_P (op2) && DECL_NAME (op2)
        && (TREE_CODE (DECL_NAME (op2)) == IDENTIFIER_NODE))
        field_sign = IDENTIFIER_POINTER (DECL_NAME (op2));

      /* "Complex identifier" means "simple identifier" together with a name of
       * structure containing this field. */
      if (ldv_arg_sign == LDV_ARG_SIGN_COMPLEX_ID)
        {
          /* Calculate signature of a structure containing a given field. */
          record_type = DECL_CONTEXT (op2);

          struct_sign = ldv_get_arg_sign (record_type, LDV_ARG_SIGN_SIMPLE_ID);

          arg_sign_p = XCNEWVEC (char, (strlen (field_sign) + 4 + strlen (struct_sign) + 1));

          sprintf(arg_sign_p, "%s_of_%s", field_sign, struct_sign);
          arg_sign = arg_sign_p;
        }
      /* Otherwise calculate argument signature as "simple identifier", i.e. as
       * a variable, argument or field name. */
      else
        arg_sign = field_sign;
    }

  /* Argument signature can't be exctracted, so use stub instead. */
  if (!arg_sign)
    return "NOT_ARG_SIGN";

  return arg_sign;
}

void
ldv_match_expr (tree t)
{
  enum tree_code code;
  enum tree_code_class code_class;
  tree block_decl, statement, func_called, func_called_addr;
  tree_stmt_iterator si;
  struct function *cfunc = NULL;
  expanded_location func_close_brace_location;
  unsigned HOST_WIDE_INT ix;
  tree index = NULL, value = NULL;
  bool isvar_global;
  tree list = NULL_TREE;
  expanded_location var_func_location;
  unsigned int arg_numb;
  ldv_func_arg_info_ptr func_arg_info_new = NULL;
  tree op1 = NULL_TREE;
  unsigned int array_size;
  ldv_var_array_ptr var_array = NULL, var_array_new = NULL;
  ldv_list_ptr var_array_list = NULL;
  tree array_size_tree = NULL_TREE;
  call_expr_arg_iterator iter;
  tree arg = NULL_TREE;

  /* Stop processing if there is not a node given. */
  if (!t)
    return;

  code = TREE_CODE (t);
  code_class = TREE_CODE_CLASS (code);

  switch (code_class)
    {
    /* These entities aren't matched at the moment. */
    case tcc_constant:
    case tcc_type:
      break;

    /* For declarations just investigate variable initializers. */
    case tcc_declaration:
      if (code == VAR_DECL)
        /* To avoid infinite recursion check that initializer isn't equal to
           an initialized variable itself. */
        /* Don't consider an initialized variable in scope of initialization. */
        if (t != cur_var)
          {
            /* Set a current variable just ones. */
            if (!cur_var)
              cur_var = t;

            /* Consider initialization just in global scope and through function
               variables. */
            if (DECL_FILE_SCOPE_P (cur_var) || isfunc_vars)
              {
                if (!DECL_FILE_SCOPE_P (cur_var))
                  isfunc_vars = false;
                ldv_match_expr (DECL_INITIAL (t));
                if (!DECL_FILE_SCOPE_P (cur_var))
                  isfunc_vars = true;
              }

            /* Finish current variable processing. */
            if (t == cur_var)
              cur_var = NULL_TREE;
          }

      break;

    case tcc_expression:
      switch (code)
        {
        /* They have one operand. */
        case ADDR_EXPR:
        case TRUTH_NOT_EXPR:
        case VA_ARG_EXPR:
          ldv_match_expr (LDV_OP1);

          break;

        /* It has three operands. */
        case BIND_EXPR:
          /* Match information on bound variables. Mark that we process function
             variables to avoid infinite recursion in initialization. */
          isfunc_vars = true;
          for (block_decl = BIND_EXPR_VARS (t); block_decl; block_decl = TREE_CHAIN (block_decl))
            ldv_match_expr (block_decl);
          isfunc_vars = false;

          /* Match a statement list. */
          ldv_match_expr (BIND_EXPR_BODY (t));

          /* Third operand is required for debugging purposes. */

          break;

        /* They have two operands. */
        case COMPOUND_EXPR:
        case TRUTH_ANDIF_EXPR:
        case TRUTH_ORIF_EXPR:
        case TRUTH_AND_EXPR:
        case TRUTH_OR_EXPR:
        case TRUTH_XOR_EXPR:
          ldv_match_expr (LDV_OP1);
          ldv_match_expr (LDV_OP2);

          break;

        /* It also has two operands but may be involved in set/get pointcuts. */
        case MODIFY_EXPR:
          /* See just on a modified parameter or a variable declaration. */
          if (TREE_CODE (LDV_OP1) == PARM_DECL || TREE_CODE (LDV_OP1) == VAR_DECL)
            {
              /* For generated by gcc variables try to remember information on
                 array sizes that may have types on which variables point. */
              if (!DECL_NAME (LDV_OP1) && (array_size = ldv_array_field_size (LDV_OP2)))
                {
                  var_array_new = ldv_create_var_array ();

                  ldv_list_push_back (&ldv_var_array_list, var_array_new);

                  var_array_new->uid = DECL_UID (LDV_OP1);
                  var_array_new->array_size = array_size;
                  var_array_new->issize_specified = true;
                }

              /* Find out a variable context. */
              if (DECL_CONTEXT (LDV_OP1) && (TREE_CODE (DECL_CONTEXT (LDV_OP1)) == FUNCTION_DECL))
                isvar_global = false;
              else
                isvar_global = true;

              /* Match just named variables. */
              if (DECL_NAME (LDV_OP1) && (TREE_CODE (DECL_NAME (LDV_OP1)) == IDENTIFIER_NODE))
                {
                  if (isvar_global)
                    ldv_match_var (LDV_OP1, LDV_PP_SET_GLOBAL);
                  else
                    ldv_match_var (LDV_OP1, LDV_PP_SET_LOCAL);

                  if (ldv_i_match)
                    {
                      /* Instance a matched advice. Note that for global variables ldv
                         function declarations are printed after a variable declaration, for
                         local variables directly before a function declaration as well as
                         for parameter declarations. */
                      if (isvar_global)
                        var_func_location = expand_location (DECL_SOURCE_LOCATION (LDV_OP1));
                      else
                        var_func_location = expand_location (DECL_SOURCE_LOCATION (DECL_CONTEXT (LDV_OP1)));

                      ldv_weave_advice (&var_func_location, NULL);

                      /* Finish matching. */
                      ldv_i_match = NULL;

                      /* Change a modifier to an aspect function call if a set was matched. */
                      if (ldv_func_called_matched)
                        {
                          /* Create a list of arguments of a called in future function. */
                          list = build_tree_list (NULL_TREE, LDV_OP2);

                          /* Create an aspect function call with corresponding arguments. */
                          LDV_OP2 = build_function_call (DECL_SOURCE_LOCATION (ldv_func_called_matched), ldv_func_called_matched, list);

                          ldv_func_called_matched = NULL;
                        }

                      /* Don't try to match both set and get pointcuts in scope
                         of one expression. */
                      break;
                    }
                }
            }

          ldv_match_expr (LDV_OP1);

          /* See just on a modified parameter or a variable declaration. */
          if (TREE_CODE (LDV_OP2) == PARM_DECL || TREE_CODE (LDV_OP2) == VAR_DECL)
            {
              /* Find out a variable context. */
              if (DECL_CONTEXT (LDV_OP2) && (TREE_CODE (DECL_CONTEXT (LDV_OP2)) == FUNCTION_DECL))
                isvar_global = false;
              else
                isvar_global = true;

              /* Match just named variables. */
              if (DECL_NAME (LDV_OP2) && (TREE_CODE (DECL_NAME (LDV_OP2)) == IDENTIFIER_NODE))
                {
                  if (isvar_global)
                    ldv_match_var (LDV_OP2, LDV_PP_GET_GLOBAL);
                  else
                    ldv_match_var (LDV_OP2, LDV_PP_GET_LOCAL);

                  if (ldv_i_match)
                    {
                      /* Instance a matched advice. Note that for global variables ldv
                         function declarations are printed after a variable declaration, for
                         local variables directly before a function declaration as well as
                         for parameter declarations. */
                      if (isvar_global)
                        var_func_location = expand_location (DECL_SOURCE_LOCATION (LDV_OP2));
                      else
                        var_func_location = expand_location (DECL_SOURCE_LOCATION (DECL_CONTEXT (LDV_OP2)));

                      ldv_weave_advice (&var_func_location, NULL);

                      /* Finish matching. */
                      ldv_i_match = NULL;

                      /* Change a modifier to an aspect function call if a get was matched. */
                      if (ldv_func_called_matched)
                        {
                          /* Create a list of arguments of a called in future function. */
                          list = build_tree_list (NULL_TREE, LDV_OP2);

                          /* Create an aspect function call with corresponding arguments. */
                          LDV_OP2 = build_function_call (DECL_SOURCE_LOCATION (ldv_func_called_matched), ldv_func_called_matched, list);

                          ldv_func_called_matched = NULL;
                        }

                      /* Don't try to match the given variable or parameter any
                         more. */
                      break;
                    }
                }
            }

          ldv_match_expr (LDV_OP2);

          break;

        /* It has one operand. */
        case COMPOUND_LITERAL_EXPR:
          /* But in fact just it initializer is interested. */
          ldv_match_expr (DECL_INITIAL (COMPOUND_LITERAL_EXPR_DECL (t)));

          break;

        /* It has three operands. */
        case COND_EXPR:
          ldv_match_expr (COND_EXPR_COND (t));
          ldv_match_expr (COND_EXPR_THEN (t));
          ldv_match_expr (COND_EXPR_ELSE (t));

          break;

        /* They have two operands. */
        case PREINCREMENT_EXPR:
        case PREDECREMENT_EXPR:
        case POSTINCREMENT_EXPR:
        case POSTDECREMENT_EXPR:
          ldv_match_expr (LDV_OP1);

          /* Second operand represents an integer number to be added or
             subtracted. It isn't interesting. */

          break;

        /* Do nothing for this auxliary entity. */
        case SAVE_EXPR:
          break;

        /* It has four operands. */
        case TARGET_EXPR:
          /* Nevertheless we are interested just in one operand. */
          ldv_match_expr (TARGET_EXPR_INITIAL (t));

          break;

        default:
          LDV_CONVERT_WARN (t);
        }

      break;

    /* They both has two operands to be examined. */
    case tcc_binary:
    case tcc_comparison:
      ldv_match_expr (LDV_OP1);
      ldv_match_expr (LDV_OP2);

      break;

    /* It has one operand. */
    case tcc_unary:
      ldv_match_expr (LDV_OP1);

      break;

    case tcc_reference:
      switch (code)
        {
        /* It has four operands. */
        case ARRAY_REF:
          /* But in fact just a first representing an array itself and a second
             responsible for index are considered. */
          ldv_match_expr (LDV_OP1);
          ldv_match_expr (LDV_OP2);

          break;

        /* It has three operands. But it so awful that we skip it. */
        case BIT_FIELD_REF:
          break;

        /* It has three operands. */
        case COMPONENT_REF:
          ldv_match_expr (LDV_OP1);
          ldv_match_expr (LDV_OP2);

          /* Third operand provides a field offset. */

          break;

        /* It has one operand. */
        case INDIRECT_REF:
          ldv_match_expr (LDV_OP1);

          break;

        default:
          LDV_CONVERT_WARN (t);
        }

      break;

    case tcc_statement:
      switch (code)
        {
        /* It has five operands. */
        case ASM_EXPR:
          /* We wouldn't like to deal with asm statements. */
          break;

        /* It has three operands. */
        case CASE_LABEL_EXPR:
          /* CASE_HIGH corresponds to default label while CASE_LOW to the usual
             one. */
          if (CASE_LOW (t) && !CASE_HIGH (t))
            ldv_match_expr (CASE_LOW (t));

          /* The third operand represents an auxliary label. */

          break;

        /* Do nothing for this auxliary entity. */
        case DECL_EXPR:
          break;

        /* It has one operand. */
        case GOTO_EXPR:
          ldv_match_expr (GOTO_DESTINATION (t));

          break;

        /* It has one operand. */
        case LABEL_EXPR:
          ldv_match_expr (LABEL_EXPR_LABEL (t));

          break;

        /* It has one operand. */
        case RETURN_EXPR:
          ldv_match_expr (LDV_OP1);

          break;

        /* It has three operands. */
        case SWITCH_EXPR:
          ldv_match_expr (SWITCH_COND (t));
          ldv_match_expr (SWITCH_BODY (t));

          /* The third operand in fact represents a collection of the second one
             case labels. */

          break;

        default:
          LDV_CONVERT_WARN (t);
        }

      break;

    /* They both have variable operand lists. */
    case tcc_exceptional:
      switch (code)
        {
        case STATEMENT_LIST:
          /* Match a statement step by step. */
          for (si = tsi_start (t); !tsi_end_p (si); tsi_next (&si))
            {
              if ((statement = tsi_stmt (si)))
                {
                  switch (TREE_CODE (statement))
                    {
                      /* Do nothing for these auxliary entities. */
                      case DECL_EXPR:
                      case SAVE_EXPR:
                      case PREDICT_EXPR:
                        break;

                      default:
                        ldv_match_expr (statement);
                    }
                }
            }

          break;

        case CONSTRUCTOR:
          FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (t), ix, index, value)
            ldv_match_expr (value);

          break;

        default:
          LDV_CONVERT_WARN (t);
        }

      break;

    /* It has variable more then three operands.*/
    case tcc_vl_exp:
      switch (code)
        {
        case CALL_EXPR:
          /* Try to get a called function. */
          if ((func_called_addr = CALL_EXPR_FN (t))
            && TREE_CODE (func_called_addr) == ADDR_EXPR
            && (func_called = TREE_OPERAND (func_called_addr, 0))
            && TREE_CODE (func_called) == FUNCTION_DECL)
            {
              ldv_func_arg_info_list = NULL;

              /* Store information on called function arguments values by walking
                 through tree lists. */
              arg_numb = 0;
              FOR_EACH_CALL_EXPR_ARG (arg, iter, t)
                {
                  arg_numb++;
                  func_arg_info_new = ldv_create_func_arg_info ();

                  func_arg_info_new->arg_numb = arg_numb;

                  ldv_list_push_back (&ldv_func_arg_info_list, func_arg_info_new);

                  /* Argument may be the first operand of the NOP expression. */
                  if (TREE_CODE (arg) == NOP_EXPR)
                    {
                      arg = TREE_OPERAND (arg, 0);
                    }

                  /* Argument name can be calculated just in case when some
                   * declaration name is passed as a function parameter
                   * directly. */
                  if (DECL_P (arg) && DECL_NAME (arg)
                    && (TREE_CODE (DECL_NAME (arg)) == IDENTIFIER_NODE))
                    func_arg_info_new->arg_name = IDENTIFIER_POINTER (DECL_NAME (arg));

                  func_arg_info_new->sign = ldv_get_arg_sign (arg, ldv_get_arg_sign_algo ());

                  switch (TREE_CODE (arg))
                    {
                    case ADDR_EXPR:
                      op1 = TREE_OPERAND (arg, 0);

                      /* Function, variable and field declarations that have an array
                         type are interest for us at the moment. */
                      switch (TREE_CODE (op1))
                        {
                        case VAR_DECL:
                          /* Just those variables for which we can obtain their
                             array type sizes are of interest. */
                          if ((array_size_tree = LDV_ARRAY_SIZE (op1)))
                            {
                              func_arg_info_new->func_arg_info_kind = LDV_FUNC_ARG_INFO_ONE_DIM_ARRAY_SIZE;

                              /* Store an one dimensional arrary size of a variable used as
                                 a value of a function argument. */
                              func_arg_info_new->one_dim_array_size = TREE_INT_CST_LOW (array_size_tree) + 1;
                            }

                          break;

                        case FUNCTION_DECL:
                          /* Just named functions are of interest. */
                          if (DECL_NAME (op1))
                            {
                              func_arg_info_new->func_arg_info_kind = LDV_FUNC_ARG_INFO_FUNC_NAME;
                              /* Store a function name used as a value of a
                                 function argument. */
                              func_arg_info_new->func_name = IDENTIFIER_POINTER (DECL_NAME (op1));
                            }

                           break;

                        default:
                          if ((array_size = ldv_array_field_size (arg)))
                            {
                              func_arg_info_new->func_arg_info_kind = LDV_FUNC_ARG_INFO_ONE_DIM_ARRAY_SIZE;

                              /* Store an one dimensional arrary size of a field used as a
                                 value of a function argument. */
                              func_arg_info_new->one_dim_array_size = array_size;
                            }
                        }

                      break;

                    case VAR_DECL:
                      /* See on whether for this variable size was stored. */
                      for (var_array_list = ldv_var_array_list
                        ; var_array_list
                        ; var_array_list = ldv_list_get_next (var_array_list))
                        {
                          var_array = (ldv_var_array_ptr) ldv_list_get_data (var_array_list);

                          /* Compare unique ids. */
                          if (var_array->uid == DECL_UID (arg)
                            && var_array->issize_specified)
                            {
                              func_arg_info_new->func_arg_info_kind = LDV_FUNC_ARG_INFO_ONE_DIM_ARRAY_SIZE;

                              /* Store an one dimensional arrary size refferenced by a
                                 variable used as a value of a function argument. */
                              func_arg_info_new->one_dim_array_size = var_array->array_size;
                            }
                        }

                      break;

                    default: ;
                    }
                }

              /* Try to match a function declaration for a call join point. */
              ldv_match_func (func_called, LDV_PP_CALL);

              if (ldv_i_match)
                {
                  /* Obtain a function close brace location. A function structure is
                     available for a function that has a definition. For a function
                     that has a declaration a place of its finishing ';' is used. */
                  if ((cfunc = DECL_STRUCT_FUNCTION (func_called)) != NULL)
                    func_close_brace_location = expand_location (cfunc->function_end_locus);
                  else
                    func_close_brace_location = expand_location (DECL_SOURCE_LOCATION (func_called));

                  /* Weave a matched advice. */
                  ldv_weave_advice (NULL, &func_close_brace_location);

                  /* Finish matching. */
                  ldv_i_match = NULL;

                  /* Change a function call to an aspect function call if it's
                     matched. */
                  if (ldv_func_called_matched)
                    {
                      TREE_OPERAND (func_called_addr, 0) = ldv_func_called_matched;
                      ldv_func_called_matched = NULL;
                    }
                }

              ldv_func_arg_info_list = NULL;
            }
          /* A function can be called not only by means of its name but also
             by means of a function pointer that may be a structure field. But
             at the moment we don't treat these cases. */

          break;

        default:
          LDV_CONVERT_WARN (t);
        }
      break;

    default:
      LDV_CONVERT_WARN (t);
    }
}

void
ldv_match_func (tree t, ldv_ppk pp_kind)
{
  ldv_adef_ptr adef = NULL;
  ldv_list_ptr adef_list = NULL;
  ldv_i_match_ptr match = NULL;
  ldv_i_func_ptr func = NULL;
  const char *func_decl_printed;

  /* There is no advice definitions at all. So nothing will be matched. */
  if (ldv_adef_list == NULL)
    {
      ldv_i_match = NULL;
      return;
    }

  match = ldv_create_info_match ();

  func = ldv_create_info_func ();

  match->i_kind = LDV_I_FUNC;
  match->pp_kind = pp_kind;
  match->i_func = func;

  /* Obtain information on a function signature. */
  func->name = ldv_create_id ();
  ldv_puts_id ((const char *) (IDENTIFIER_POINTER (DECL_NAME (t))), func->name);

  /* Remember whether a function is inline/static to add this information to
     a function return type. */
  if (DECL_DECLARED_INLINE_P (t) || !TREE_PUBLIC (t) || !TREE_STATIC (t))
    {
      ldv_entity_declspecs = ldv_create_declspecs ();

      if (DECL_DECLARED_INLINE_P (t))
        ldv_entity_declspecs->isinline = true;

      if (!TREE_PUBLIC (t))
        ldv_entity_declspecs->isstatic = true;
      /* Ignore an extern flag since it isn't stored in gcc internal
         representation in the same way as it's declared in source code.
      if (!TREE_STATIC (t))
        ldv_entity_declspecs->isextern = true;
      */
    }

  func->type = ldv_convert_type_tree_to_internal (TREE_TYPE (t), t);

  func->file_path = DECL_SOURCE_FILE (t);

  /* Walk through an advice definitions list to find matches. */
  for (adef_list = ldv_adef_list; adef_list; adef_list = ldv_list_get_next (adef_list))
    {
      adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

      if (ldv_match_cp (adef->a_declaration->c_pointcut, match))
        {
          /* Count advice weavings. */
          ++(adef->use_counter);

          ldv_print_info (LDV_INFO_MATCH, "match function \"%s\"", ldv_get_id_name (func->name));

          ldv_i_match = match;
          match->a_definition = adef;

          ldv_func_decl_matched = t;

          return;
        }
      /* Print signatures of matched by name but not by other signature
         functions if it's needed. */
      else if (match->ismatched_by_name)
        {
          if (ldv_isprint_signature_of_matched_by_name)
            {
              ldv_puts ("\nThese functions were matched by name but have different signatures:\n  source function declaration: ", LDV_MATCHED_BY_NAME);
              func_decl_printed = ldv_print_func_decl (match->i_func);
              ldv_puts (func_decl_printed, LDV_MATCHED_BY_NAME);
              ldv_puts ("\n  aspect function declaration: ", LDV_MATCHED_BY_NAME);
              func_decl_printed = ldv_print_func_decl (match->i_func_aspect);
              ldv_puts (func_decl_printed, LDV_MATCHED_BY_NAME);
              ldv_puts ("\n", LDV_MATCHED_BY_NAME);
            }

          match->ismatched_by_name = false;
        }
    }

  /* Nothing was matched. */
  ldv_i_match = NULL;

  return;
}

void
ldv_match_func_body (tree fndecl)
{
  tree body = NULL_TREE;

  /* Obtain a function body. */
  body = DECL_SAVED_TREE (fndecl);

  /* Visualize a body before matching and weaving. */
  ldv_visualize_body (fndecl);

  /* This list contains information on all generated by gcc auxiliary variables
     of a body that may be a pointer on arrays with sizes. */
  ldv_var_array_list = NULL;

  ldv_match_expr (body);

  /* Visualize a body after matching and weaving. */
  ldv_visualize_body (fndecl);
}

void
ldv_match_typedecl (tree t, const char *file_path)
{
  ldv_adef_ptr adef = NULL;
  ldv_list_ptr adef_list = NULL;
  ldv_i_match_ptr match = NULL;
  ldv_i_typedecl_ptr typedecl = NULL;
  const char *typedecl_printed;

  /* There is no advice definitions at all. So nothing will be matched. */
  if (ldv_adef_list == NULL)
    {
      ldv_i_match = NULL;
      return;
    }

  match = ldv_create_info_match ();

  typedecl = ldv_create_info_typedecl ();

  match->i_kind = LDV_I_TYPE;
  match->i_typedecl = typedecl;

  /* Obtain information on a type declaration signature. */
  typedecl->name = IDENTIFIER_POINTER (TYPE_NAME (t));

  switch (TREE_CODE (t))
    {
    case ENUMERAL_TYPE:
      typedecl->itd_kind = LDV_ITD_ENUM;
      break;

    case RECORD_TYPE:
      typedecl->itd_kind = LDV_ITD_STRUCT;
      break;

    case UNION_TYPE:
      typedecl->itd_kind = LDV_ITD_UNION;
      break;

    default:
      LDV_FATAL_ERROR ("incorrect type type");
    }

  typedecl->file_path = file_path;

  /* Walk through an advice definitions list to find matches. */
  for (adef_list = ldv_adef_list; adef_list; adef_list = ldv_list_get_next (adef_list))
    {
      adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

      if (ldv_match_cp (adef->a_declaration->c_pointcut, match))
        {
          /* Count advice weavings. */
          ++(adef->use_counter);

          ldv_print_info (LDV_INFO_MATCH, "match type \"%s\"", typedecl->name);

          ldv_i_match = match;
          match->a_definition = adef;

          return;
        }
      /* Print signatures of matched by name but not by other signature type
         declarations if it's needed. */
      else if (match->ismatched_by_name)
        {
          if (ldv_isprint_signature_of_matched_by_name)
            {
              ldv_puts ("\nThese type declarations were matched by name but have different signatures:\n  source type declaration: ", LDV_MATCHED_BY_NAME);
              typedecl_printed = ldv_print_type_decl (match->i_typedecl);
              ldv_puts (typedecl_printed, LDV_MATCHED_BY_NAME);
              ldv_puts ("\n  aspect type declaration: ", LDV_MATCHED_BY_NAME);
              typedecl_printed = ldv_print_type_decl (match->i_typedecl_aspect);
              ldv_puts (typedecl_printed, LDV_MATCHED_BY_NAME);
              ldv_puts ("\n", LDV_MATCHED_BY_NAME);
            }

          match->ismatched_by_name = false;
        }
    }

  /* Nothing was matched. */
  ldv_i_match = NULL;

  return;
}

void
ldv_match_var (tree t, ldv_ppk pp_kind)
{
  ldv_adef_ptr adef = NULL;
  ldv_list_ptr adef_list = NULL;
  ldv_i_match_ptr match = NULL;
  ldv_i_var_ptr var = NULL;
  ldv_i_func_ptr func_context = NULL;
  const char *var_decl_printed;

  /* There is no advice definitions at all. So nothing will be matched. */
  if (ldv_adef_list == NULL)
    {
      ldv_i_match = NULL;
      return;
    }

  match = ldv_create_info_match ();

  var = ldv_create_info_var ();

  match->i_kind = LDV_I_VAR;
  match->pp_kind = pp_kind;
  match->i_var = var;

  /* Obtain information on a variable signature. */
  var->name = ldv_create_id ();
  ldv_puts_id ((const char *) (IDENTIFIER_POINTER (DECL_NAME (t))), var->name);

  var->type = ldv_convert_type_tree_to_internal (TREE_TYPE (t), NULL);

  var->file_path = DECL_SOURCE_FILE (t);

  /* Add a function context for a variable if it's needed. */
  if (DECL_CONTEXT (t) && (TREE_CODE (DECL_CONTEXT (t)) == FUNCTION_DECL))
    {
      var->func_context = ldv_create_info_match ();

      func_context = ldv_create_info_func ();

      var->func_context->i_kind = LDV_I_FUNC;
      var->func_context->i_func = func_context;

      func_context->name = ldv_create_id ();
      ldv_puts_id (IDENTIFIER_POINTER (DECL_NAME (DECL_CONTEXT (t))), func_context->name);

      if (DECL_DECLARED_INLINE_P (DECL_CONTEXT (t)))
        {
          ldv_entity_declspecs = ldv_create_declspecs ();
          ldv_entity_declspecs->isinline = true;
        }

      func_context->type = ldv_convert_type_tree_to_internal (TREE_TYPE (DECL_CONTEXT (t)), DECL_CONTEXT (t));

      func_context->file_path = DECL_SOURCE_FILE (DECL_CONTEXT (t));
    }
  else
    var->func_context = NULL;

  /* Walk through an advice definitions list to find matches. */
  for (adef_list = ldv_adef_list; adef_list; adef_list = ldv_list_get_next (adef_list))
    {
      adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

      if (ldv_match_cp (adef->a_declaration->c_pointcut, match))
        {
          /* Count advice weavings. */
          ++(adef->use_counter);

          ldv_print_info (LDV_INFO_MATCH, "match variable \"%s\"", ldv_get_id_name (var->name));

          ldv_i_match = match;
          match->a_definition = adef;

          return;
        }
      /* Print signatures of matched by name but not by other signature
         variables if it's needed. */
      else if (match->ismatched_by_name)
        {
          if (ldv_isprint_signature_of_matched_by_name)
            {
              ldv_puts ("\nThese variables were matched by name but have different signatures:\n  source variable declaration: ", LDV_MATCHED_BY_NAME);
              var_decl_printed = ldv_print_var_decl (match->i_var);
              ldv_puts (var_decl_printed, LDV_MATCHED_BY_NAME);
              ldv_puts ("\n  aspect variable declaration: ", LDV_MATCHED_BY_NAME);
              var_decl_printed = ldv_print_var_decl (match->i_var_aspect);
              ldv_puts (var_decl_printed, LDV_MATCHED_BY_NAME);
              ldv_puts ("\n", LDV_MATCHED_BY_NAME);
            }

          match->ismatched_by_name = false;
        }
    }

  /* Nothing was matched. */
  ldv_i_match = NULL;

  return;
}

void
ldv_visualize_expr (tree t, int offset)
{
  enum tree_code code;
  enum tree_code_class code_class;
  const char* code_name = NULL;
  tree_stmt_iterator si;
  tree chain = NULL_TREE;
  unsigned HOST_WIDE_INT cnt;
  tree index = NULL_TREE, value = NULL_TREE;
  bool isprocessed = false;
  int i;

  /* Stop processing if there is no node. */
  if (!t)
    return;

  /* Obtain information on an expression node. */
  code = TREE_CODE (t);
  code_name = tree_code_name[(int) code];
  code_class = TREE_CODE_CLASS (code);

  /* Separate a new expression from the previos one. */
  fprintf (LDV_EXPR_VISUALIZATION_STREAM, "\n");

  /* Print identation spaces. */
  for (i = 0; i < offset; i++)
    fprintf (LDV_EXPR_VISUALIZATION_STREAM, " ");

  /* Print an expression name. */
  fprintf (LDV_EXPR_VISUALIZATION_STREAM, "%s", code_name);
  /* Print an expression name and a corresponding code and class. */
  /* fprintf (LDV_EXPR_VISUALIZATION_STREAM, "%s (code: %d, class: %d)", code_name, code, code_class); */

  /* Process groups of expressions. */
  switch (code_class)
    {
    case tcc_binary:
    case tcc_comparison:
      /* Print information on a first operand. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));

      /* Print information on a second operand. */
      ldv_visualize_expr (LDV_OP2, LDV_OFFSET_NEXT (offset));

      isprocessed = true;

      break;

    /* It seems that there won't be joinpoints on constants and types
       in near future. So, ignore it at all. */
    case tcc_constant:
    case tcc_type:
      isprocessed = true;
      break;

    case tcc_declaration:
      /* Print an entity name if so. */
      if (DECL_NAME (t) && (TREE_CODE (DECL_NAME (t)) == IDENTIFIER_NODE))
        fprintf (LDV_EXPR_VISUALIZATION_STREAM, " %s", IDENTIFIER_POINTER (DECL_NAME (t)));
      else
        /* Print 'name' for an unnamed entity like in c-pretty-print.c. */
        fprintf (LDV_EXPR_VISUALIZATION_STREAM, " %4x", ((unsigned)((unsigned long)(t) & 0xffff)));

      /* Mark a declaration as processed. */
      isprocessed = true;

      break;

    case tcc_unary:
      /* Print information on a first operand. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));

      isprocessed = true;

      break;

    default:
      break;
    }

  /* Process individual expressions. */
  switch (code)
    {
    /* Print standard unary nodes. */
    case TRUTH_NOT_EXPR:
    case ADDR_EXPR:
    case INDIRECT_REF:
/*
    case ALIGN_INDIRECT_REF:
    case MISALIGNED_INDIRECT_REF:
    case CLEANUP_POINT_EXPR:
    case SAVE_EXPR:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
*/
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));
      break;

    /* Print standard binary nodes. */
    case MODIFY_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_XOR_EXPR:
/*
    case INIT_EXPR:
    case COMPOUND_EXPR:
    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:
*/
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));
      ldv_visualize_expr (LDV_OP2, LDV_OFFSET_NEXT (offset));

      break;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
      /* Print an array itself. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));

      /* Print an array index (for a range is a starting index). */
      ldv_visualize_expr (LDV_OP2, LDV_OFFSET_NEXT (offset));

      /* Print copy of information on an index. */
      ldv_visualize_expr (LDV_OP3, LDV_OFFSET_NEXT (offset));

      /* Print an element size. */
      ldv_visualize_expr (LDV_OP4, LDV_OFFSET_NEXT (offset));

      break;

    case ASM_EXPR:
      /* Do nothing for asembler expressions. */
      break;

    case BIND_EXPR:
      /* Print information on bound variables. */
      for (chain = BIND_EXPR_VARS (t); chain; chain = TREE_CHAIN (chain))
        ldv_visualize_expr (chain, LDV_OFFSET_NEXT (offset));

      /* Print a statement list. */
      ldv_visualize_expr (BIND_EXPR_BODY (t), LDV_OFFSET_NEXT (offset));

      break;

    case CALL_EXPR:
      /* Print information on a called function. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));

      /* Print information on called function arguments values. */
      ldv_visualize_expr (LDV_OP2, LDV_OFFSET_NEXT (offset));

      break;

    case CASE_LABEL_EXPR:
      ldv_visualize_expr (CASE_LOW (t), LDV_OFFSET_NEXT (offset));
      ldv_visualize_expr (CASE_HIGH (t), LDV_OFFSET_NEXT (offset));

      break;

    case COMPONENT_REF:
      /* Print a structure or union. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));

      /* Print a field declaration. */
      ldv_visualize_expr (LDV_OP2, LDV_OFFSET_NEXT (offset));

      /* Print a field offset. */
      ldv_visualize_expr (LDV_OP3, LDV_OFFSET_NEXT (offset));

      break;

    case COND_EXPR:
      /* Print if condition. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));

      /* Print then expresion. */
      ldv_visualize_expr (LDV_OP2, LDV_OFFSET_NEXT (offset));

      /* Print else expression. */
      ldv_visualize_expr (LDV_OP3, LDV_OFFSET_NEXT (offset));

      break;

    case CONSTRUCTOR:
      FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (t), cnt, index, value)
        {
          /* Print an array index or a structure field to be initialized. */
          ldv_visualize_expr (index, LDV_OFFSET_NEXT (offset));

          /* Print a value. */
          ldv_visualize_expr (value, LDV_OFFSET_NEXT (offset));
        }

      break;

    case GOTO_EXPR:
      /* Print a label. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));
      break;

    case INTEGER_CST:
      /* Print an integer constant. */
      fprintf (LDV_EXPR_VISUALIZATION_STREAM, " %ld", TREE_INT_CST_LOW (t));
      break;

    case LABEL_EXPR:
      /* Print a label name. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));
      break;

    case RETURN_EXPR:
      /* Print a returned expression. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));
      break;

    case STATEMENT_LIST:
      /* Print a part of a statement. */
      for (si = tsi_start (t); !tsi_end_p (si); tsi_next (&si))
        ldv_visualize_expr (tsi_stmt (si), LDV_OFFSET_NEXT (offset));

      break;

    case SWITCH_EXPR:
      /* Print switching expression. */
      ldv_visualize_expr (LDV_OP1, LDV_OFFSET_NEXT (offset));

     /* Print switch body expresion. */
      ldv_visualize_expr (LDV_OP2, LDV_OFFSET_NEXT (offset));

      /* Print case label expression. */
      if (LDV_OP3)
        ldv_visualize_expr (LDV_OP3, LDV_OFFSET_NEXT (offset));

      break;

    case TREE_LIST:
      /* Print a specific purpose. */
      ldv_visualize_expr (TREE_PURPOSE (t), LDV_OFFSET_NEXT (offset));

      /* Print a specific value. */
      ldv_visualize_expr (TREE_VALUE (t), LDV_OFFSET_NEXT (offset));

      /* Print a next list element. */
      ldv_visualize_expr (TREE_CHAIN (t), LDV_OFFSET_NEXT (offset));

      break;

    case TREE_VEC:
      for (i = 0; i < TREE_VEC_LENGTH (t); i++)
        /* Print a vector element. */
        ldv_visualize_expr (TREE_VEC_ELT (t, i), LDV_OFFSET_NEXT (offset));

      break;

    default:
      if (!isprocessed)
        fprintf (LDV_EXPR_VISUALIZATION_STREAM, " UNPARSED");

      break;
    }
}

void
ldv_visualize_body (tree fndecl)
{
  tree body = NULL_TREE;

  /* Obtain a function body. */
  body = DECL_SAVED_TREE (fndecl);

  /* Visualize a function body if it's needed. */
  if (!ldv_isexpr_visualization)
    return;

  /* Print a name of a function which a body will be visualized. */
  if (DECL_NAME (fndecl) && TREE_CODE (DECL_NAME (fndecl)) == IDENTIFIER_NODE)
    fprintf (LDV_EXPR_VISUALIZATION_STREAM, "\nBody of function '%s':\n", IDENTIFIER_POINTER (DECL_NAME (fndecl)));

  /* Visualize a function body as an expression. At the beginning an identation
     is absent. */
  ldv_visualize_expr (body, 0);

  /* Ident after a function body virtualization is completed. */
  fprintf (LDV_EXPR_VISUALIZATION_STREAM, "\n\n");
}
