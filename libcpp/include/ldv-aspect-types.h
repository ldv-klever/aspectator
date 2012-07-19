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

#ifndef _LDV_ASPECT_TYPES_H_
#define _LDV_ASPECT_TYPES_H_


#include "ldv-list.h"

/* Some abbreviatioun that are used in aspect types:
   str = string
   id = identifier
   t = type
   k = kind
   l = length
   s = signature
   pp = primitive pointcut
   p = primitive
   c = composite
   cp = composite pointcut
   np = named pointcut
   decl = declaration
   func = function
   var = variable
   def = definition
   b = body
   a = advice
   l = length
   param = parameter
   arg = argument
   i = info
   ptr = pointer
   qual = qualifier
   int = integer number
 */

/* String = text terminated by '\0' + its length + maximum available
   length (i.e. memory size allocated for string). */
typedef struct ldv_string_internal
{
  char *text;
  unsigned int len;
  unsigned int max_len;
} ldv_string;
typedef ldv_string *ldv_str_ptr;

/* These constants are for a list of strings. */
enum { LDV_STRING_LEN_ADD = 10, LDV_STRING_LEN_START = 20 };

/* Integer number = integer number (unsigned). */
typedef struct ldv_int_internal
{
  unsigned int numb;
} ldv_int;
typedef ldv_int *ldv_int_ptr;

/* Identifier = string.. */
enum { LDV_ID_LEN_ADD = 5, LDV_ID_LEN_START = 10 };
typedef struct ldv_id_internal
{
  ldv_str_ptr id_name;
} ldv_id;
typedef ldv_id *ldv_id_ptr;

/* Text = string (large). */
enum { LDV_TEXT_LEN_ADD = 50, LDV_TEXT_LEN_START = 100 };
typedef struct ldv_text_internal
{
  ldv_str_ptr text;
} ldv_text;
typedef ldv_text *ldv_text_ptr;

/* File = string + information on whether a given file is processed. */
enum { LDV_FILE_LEN_ADD = 25, LDV_FILE_LEN_START = 50 };
typedef struct ldv_file_internal
{
  ldv_str_ptr file_name;
  bool isthis;
} ldv_file;
typedef ldv_file *ldv_file_ptr;

/* Macro primitive pointcut signature = macro definition name + macro
   function parameters. */
typedef enum { LDV_PPS_MACRO_DEF, LDV_PPS_MACRO_FUNC } ldv_pps_macro_k;
typedef struct ldv_primitive_pointcut_signature_macro_internal
{
  ldv_pps_macro_k pps_macro_kind;
  ldv_id_ptr macro_name;
  ldv_list_ptr macro_param_list;
} ldv_primitive_pointcut_signature_macro;
typedef ldv_primitive_pointcut_signature_macro *ldv_pps_macro_ptr;

/* File primitive pointcut signature = path to file. */
typedef struct ldv_primitive_pointcut_signature_file_internal
{
  ldv_file_ptr file_name;
} ldv_primitive_pointcut_signature_file;
typedef ldv_primitive_pointcut_signature_file *ldv_pps_file_ptr;

/* Primitive pointcut signature declaration specifiers =
   storage-class specifiers + type specifiers + type qualifiers +
   function specifiers + type name. */
typedef struct ldv_primitive_pointcut_signature_declspecs_internal
{
  /* Name of structure, union, enum or typedef. */
  ldv_id_ptr type_name;

  /* Storage-class specifiers. */
  bool istypedef;
  bool isextern;
  bool isstatic;
  bool isauto;
  bool isregister;

  /* Type specifiers. */
  bool istype_spec;
  bool isvoid;
  bool ischar;
  bool isint;
  bool isfloat;
  bool isdouble;
  bool isbool;
  bool iscomplex;
  bool isshort;
  bool islong;
  bool islonglong;
  bool issigned;
  bool isunsigned;
  bool isstruct;
  bool isunion;
  bool isenum;
  bool istypedef_name;

  /* Type qualifiers. */
  bool isconst;
  bool isrestrict;
  bool isvolatile;

  /* Function specifiers. */
  bool isinline;
} ldv_primitive_pointcut_signature_declspecs;
typedef ldv_primitive_pointcut_signature_declspecs *ldv_pps_declspecs_ptr;

struct ldv_primitive_pointcut_signature_decl_internal;

/* Primitive pointcut signature function arguments = function argument +
   next function argument [+ variable length]. */
typedef struct ldv_primitive_pointcut_signature_func_arg_internal
{
  struct ldv_primitive_pointcut_signature_decl_internal *pps_func_arg;
  bool isva;
  bool isany_params;
} ldv_primitive_pointcut_signature_func_arg;
typedef ldv_primitive_pointcut_signature_func_arg *ldv_pps_func_arg_ptr;

/* Primitive pointcut signature array size = size itself. */
typedef struct ldv_primitive_pointcut_signature_array_size_internal
{
  int pps_array_size;
  bool issize_specified;
} ldv_primitive_pointcut_signature_array_size;
typedef ldv_primitive_pointcut_signature_array_size *ldv_pps_array_size_ptr;

/* Primitive pointcut signature pointer qualifiers = const + restrict +
   volatile. */
typedef struct ldv_primitive_pointcut_signature_ptr_quals_internal
{
  bool isconst;
  bool isrestrict;
  bool isvolatile;
} ldv_primitive_pointcut_signature_ptr_quals;
typedef ldv_primitive_pointcut_signature_ptr_quals *ldv_pps_ptr_quals_ptr;

/* Primitive pointcut signature declarator = declarator kind + declarated entity
   + declarator name + function arguments + array size + pointer qualifiers. */
typedef enum { LDV_PPS_DECLARATOR_ARRAY, LDV_PPS_DECLARATOR_FUNC, LDV_PPS_DECLARATOR_ID, LDV_PPS_DECLARATOR_NONE, LDV_PPS_DECLARATOR_PTR } ldv_pps_declarator_k;
typedef struct ldv_primitive_pointcut_signature_declarator_internal
{
  ldv_pps_declarator_k pps_declarator_kind;
  ldv_id_ptr declarator_name;
  ldv_list_ptr func_arg_list;
  ldv_pps_array_size_ptr pps_array_size;
  ldv_pps_ptr_quals_ptr pps_ptr_quals;
} ldv_primitive_pointcut_signature_declarator;
typedef ldv_primitive_pointcut_signature_declarator *ldv_pps_declarator_ptr;

/* Primitive pointcut signature declaration = declaration kind +
   declaration specifiers + declarator. */
typedef enum { LDV_PPS_DECL_ANY_PARAMS, LDV_PPS_DECL_FUNC, LDV_PPS_DECL_NONE, LDV_PPS_DECL_PARAM, LDV_PPS_DECL_TYPE, LDV_PPS_DECL_VAR } ldv_pps_decl_k;
typedef struct ldv_primitive_pointcut_signature_decl_internal
{
  ldv_pps_decl_k pps_decl_kind;
  ldv_pps_declspecs_ptr pps_declspecs;
  ldv_list_ptr pps_declarator;
} ldv_primitive_pointcut_signature_decl;
typedef ldv_primitive_pointcut_signature_decl *ldv_pps_decl_ptr;

/* Primitive pointcut signature = primitive pointcut signature kind +
   macro signature
   or file signature
   or declaration (function or variable or type) signature.
   Matching depends on a primitive pointcut signature. */
typedef enum { LDV_PPS_DEFINE, LDV_PPS_FILE, LDV_PPS_DECL, LDV_PPS_NONE } ldv_ppsk;
typedef struct ldv_primitive_pointcut_signature_internal
{
  ldv_ppsk pps_kind;
  /* FIXME. Make this as union. */
  ldv_pps_macro_ptr pps_macro;
  ldv_pps_file_ptr pps_file;
  ldv_pps_decl_ptr pps_declaration;
} ldv_primitive_pointcut_signature;
typedef ldv_primitive_pointcut_signature *ldv_pps_ptr;

/* Primitive pointcut = primitive pointcut kind + primitive pointcut
   signature. Different primitive pointcut kinds may have the same
   primitive pointcut signature and vice versa. There are following relations
   between them:
   LDV_PPS_DEFINE - LDV_PP_DEFINE
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
   LDV_PPS_DECL - LDV_PP_SET_LOCAL
   Matching depends on a primitive pointcut kind. */
typedef enum { LDV_PP_CALL, LDV_PP_DEFINE, LDV_PP_EXECUTION, LDV_PP_FILE, LDV_PP_GET, LDV_PP_GET_GLOBAL, LDV_PP_GET_LOCAL, LDV_PP_INCALL, LDV_PP_INFILE, LDV_PP_INFUNC, LDV_PP_INTRODUCE, LDV_PP_NONE, LDV_PP_SET, LDV_PP_SET_GLOBAL, LDV_PP_SET_LOCAL } ldv_ppk;
typedef struct ldv_primitive_pointcut_internal
{
  ldv_ppk pp_kind;
  ldv_pps_ptr pp_signature;
} ldv_primitive_pointcut;
typedef ldv_primitive_pointcut *ldv_pp_ptr;

/* Composite pointcut = primitive pointcut
   or !composite pointcut
   or composite pointcut 1 || composite pointcut 2
   or composite pointcut 1 && composite pointcut 2. */
typedef enum { LDV_CP_AND, LDV_CP_NONE, LDV_CP_NOT, LDV_CP_OR, LDV_CP_PRIMITIVE } ldv_cpk;
typedef struct ldv_composite_pointcut_internal
{
  ldv_cpk cp_kind;
  struct ldv_composite_pointcut_internal *c_pointcut_first;
  struct ldv_composite_pointcut_internal *c_pointcut_second;
  ldv_pp_ptr p_pointcut;
} ldv_composite_pointcut;
typedef ldv_composite_pointcut *ldv_cp_ptr;

/* Named pointcut = name + composite pointcut. */
typedef struct ldv_named_pointcut_internal
{
  char *p_name;
  ldv_cp_ptr c_pointcut;
} ldv_named_pointcut;
typedef ldv_named_pointcut *ldv_np_ptr;

/* Advice declaration = advice kind + composite pointcut. Advice weaving
   depends on an advice kind.*/
typedef enum { LDV_A_AFTER, LDV_A_AROUND, LDV_A_BEFORE, LDV_A_NEW } ldv_ak;
typedef struct ldv_advice_declaration_internal
{
  ldv_ak a_kind;
  ldv_cp_ptr c_pointcut;
} ldv_advice_declaration;
typedef ldv_advice_declaration *ldv_adecl_ptr;

/* Aspect pattern has the following format:
     pattern = $name[arg_numb][<params>]
     name = [_a-zA-Z]+
     arg_numb = [0-9]+
     params = param | params param
     param = pattern | [0-9]+ | [_a-zA-Z][_a-zA-Z0-9]+
*/
typedef enum { LDV_ASPECT_PATTERN_ASPECT_PATTERN, LDV_ASPECT_PATTERN_INTEGER, LDV_ASPECT_PATTERN_STRING } ldv_aspect_pattern_param_kind;
struct ldv_aspect_pattern_internal;
typedef struct ldv_aspect_pattern_param_internal
{
  ldv_aspect_pattern_param_kind kind;
  struct ldv_aspect_pattern_internal *aspect_pattern;
  unsigned int integer;
  char *string;
} ldv_aspect_pattern_param;
typedef ldv_aspect_pattern_param *ldv_aspect_pattern_param_ptr;

typedef struct ldv_aspect_pattern_internal
{
  char *name;
  unsigned int arg_numb;
  ldv_list_ptr params;
} ldv_aspect_pattern;
typedef ldv_aspect_pattern *ldv_aspect_pattern_ptr;

/* Body aspect pattern to be weaved = aspect pattern + pattern place in body. */
typedef struct ldv_ab_aspect_pattern_internal
{
  ldv_aspect_pattern_ptr pattern;
  int place;
} ldv_ab_aspect_pattern;
typedef ldv_ab_aspect_pattern *ldv_ab_aspect_pattern_ptr;

/* Body string beginning maximum length and its growth are specified. */
typedef enum { LDV_B_LEN_ADD = 50, LDV_B_LEN_START = 100 } ldv_abl;
/* Body = string + aspect patterns to be weaved. */
typedef struct ldv_advice_body_internal
{
  ldv_str_ptr ab_text;
  ldv_list_ptr patterns;
} ldv_advice_body;
typedef ldv_advice_body *ldv_ab_ptr;

/* Advice definition = advice declaration + advice body. Also it contains
   reference to a following advice. */
typedef struct ldv_advice_definition_internal
{
  ldv_adecl_ptr a_declaration;
  ldv_ab_ptr a_body;
  unsigned int use_counter;
} ldv_advice_definition;
typedef ldv_advice_definition *ldv_adef_ptr;

typedef enum { LDV_IT_ARRAY, LDV_IT_FUNC, LDV_IT_NONE, LDV_IT_PRIMITIVE, LDV_IT_PTR } ldv_itk;
struct ldv_info_param_internal;
typedef struct ldv_info_type_internal
{
  ldv_itk it_kind;

  /* Primitive type is a set of declaration specifiers. */
  ldv_pps_declspecs_ptr primitive_type;

  /* Array elements types and the number of elements. */
  struct ldv_info_type_internal *array_type;
  int array_size;
  bool issize_specified;

  /* Pointed type. */
  struct ldv_info_type_internal *ptr_type;
  bool isconst;
  bool isvolatile;
  bool isrestrict;

  /* Function return type and arguments types. */
  struct ldv_info_type_internal *ret_type;
  ldv_list_ptr param;
  bool isva;
} ldv_info_type;
typedef ldv_info_type *ldv_i_type_ptr;

/* File information = file name. */
typedef struct ldv_info_file_internal
{
  const char *name;
  bool isthis;
} ldv_info_file;
typedef ldv_info_file *ldv_i_file_ptr;

typedef enum { LDV_FUNC_ARG_INFO_FUNC_NAME, LDV_FUNC_ARG_INFO_NONE, LDV_FUNC_ARG_INFO_ONE_DIM_ARRAY_SIZE } ldv_func_arg_info_k;
/* Function arguments information corresponds to arguments values at a
   function call. It includes a function name (if an argument value is a
   function) and a size of one dimensional array. */
typedef struct ldv_func_arg_info_internal
{
  ldv_func_arg_info_k func_arg_info_kind;
  unsigned int arg_numb;
  const char *func_name;
  unsigned int one_dim_array_size;
} ldv_func_arg_info;
typedef ldv_func_arg_info *ldv_func_arg_info_ptr;

/* Function declaration information = function name + function type. */
typedef struct ldv_info_func_internal
{
  const char *name;
  ldv_i_type_ptr type;
  const char *file_path;
} ldv_info_func;
typedef ldv_info_func *ldv_i_func_ptr;

/* Macro declaration information = macro name + macroparameters + scope file. */
typedef struct ldv_info_macro_internal
{
  ldv_pps_macro_k macro_kind;
  const char *macro_name;
  ldv_list_ptr macro_param;
  const char *file_path;
} ldv_info_macro;
typedef ldv_info_macro *ldv_i_macro_ptr;

/* Parameter declaration information = parameter name (optional) + parameter type. */
typedef struct ldv_info_param_internal
{
  const char *name;
  const char *name_aux;
  ldv_i_type_ptr type;
  bool isany_params;
} ldv_info_param;
typedef ldv_info_param *ldv_i_param_ptr;

/* Type declaration information = type name + type type. */
typedef enum { LDV_ITD_ENUM, LDV_ITD_NONE, LDV_ITD_STRUCT, LDV_ITD_UNION } ldv_itdk;
typedef struct ldv_info_typedecl_internal
{
  const char *name;
  ldv_itdk itd_kind;
  const char *file_path;
} ldv_info_typedecl;
typedef ldv_info_typedecl *ldv_i_typedecl_ptr;

struct ldv_info_match_internal;
/* Variable declaration information = variable name + variable type. */
typedef struct ldv_info_var_internal
{
  const char *name;
  ldv_i_type_ptr type;
  const char *file_path;
  struct ldv_info_match_internal *func_context;
} ldv_info_var;
typedef ldv_info_var *ldv_i_var_ptr;

/* Structure intended to store information on matched entities. */
typedef enum { LDV_I_FILE, LDV_I_FUNC, LDV_I_MACRO, LDV_I_NONE, LDV_I_PARAM, LDV_I_TYPE, LDV_I_VAR } ldv_ik;
typedef struct ldv_info_match_internal
{
  ldv_ik i_kind;
  ldv_ppk pp_kind;

  /* FIXME. Make this as union. */
  ldv_i_file_ptr i_file;
  ldv_i_file_ptr i_file_aspect;
  ldv_i_func_ptr i_func;
  ldv_i_func_ptr i_func_aspect;
  ldv_i_macro_ptr i_macro;
  ldv_i_macro_ptr i_macro_aspect;
  ldv_i_param_ptr i_param;
  ldv_i_param_ptr i_param_aspect;
  ldv_i_typedecl_ptr i_typedecl;
  ldv_i_typedecl_ptr i_typedecl_aspect;
  ldv_i_var_ptr i_var;
  ldv_i_var_ptr i_var_aspect;
  ldv_adef_ptr a_definition;
  ldv_pp_ptr p_pointcut;

  bool ismatched_by_name;
} ldv_info_match;
typedef ldv_info_match *ldv_i_match_ptr;

#endif /* _LDV_ASPECT_TYPES_H_ */
