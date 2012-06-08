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
#include "ldv-cpp-core.h"
#include "ldv-cpp-pointcut-matcher.h"


/* A stream where ldv cpp errors information to be printed. */
#define LDV_CPP_ERROR_STREAM    (stderr)


bool ldv_cpp_isldv = false;
bool ldv_cpp_isinfo_matching_table = false;
int ldv_cpp_ldv_stage = -1;

char *
ldv_copy_str (const char *str)
{
  char *str_new = NULL;

  str_new = XCNEWVEC (char, (strlen (str) + 1));

  strcpy (str_new, str);

  return str_new;
}

void
ldv_cpp_fatal_error (const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  vfprintf (LDV_CPP_ERROR_STREAM, format, ap);
  va_end (ap);
  fputc ('\n', LDV_CPP_ERROR_STREAM);

  /* Terminate a program after a ldv cpp error. */
  exit (1);
}

char *
ldv_cpp_get_body_text (ldv_ab_ptr body)
{
  if (body)
    return ldv_cpp_get_str (body->ab_text);

  ldv_cpp_fatal_error ("body pointer wasn't initialized");

  return NULL;
}

char *
ldv_cpp_get_file_name (ldv_file_ptr file)
{
  if (file)
    return ldv_cpp_get_str (file->file_name);

  ldv_cpp_fatal_error ("file pointer wasn't initialized");

  return NULL;
}

char *
ldv_cpp_get_id_name (ldv_id_ptr id)
{
  if (id)
    return ldv_cpp_get_str (id->id_name);

  ldv_cpp_fatal_error ("id pointer wasn't initialized");

  return NULL;
}

char *
ldv_cpp_get_str (ldv_str_ptr str)
{
  if (str)
    return str->text;

  ldv_cpp_fatal_error ("string pointer wasn't initialized");

  return NULL;
}

ldv_pps_declspecs_ptr
ldv_create_declspecs (void)
{
  ldv_pps_declspecs_ptr declspecs = NULL;

  declspecs = XCNEW (ldv_primitive_pointcut_signature_declspecs);

  return declspecs;
}

ldv_i_func_ptr
ldv_create_info_func (void)
{
  ldv_i_func_ptr i_func = NULL;

  i_func = XCNEW (ldv_info_func);

  return i_func;
}

ldv_i_macro_ptr
ldv_create_info_macro (void)
{
  ldv_i_macro_ptr i_macro = NULL;

  i_macro = XCNEW (ldv_info_macro);

  return i_macro;
}

ldv_i_match_ptr
ldv_create_info_match (void)
{
  ldv_i_match_ptr match = NULL;

  match = XCNEW (ldv_info_match);

  match->i_kind = LDV_I_NONE;
  match->pp_kind = LDV_PP_NONE;

  return match;
}

ldv_i_param_ptr
ldv_create_info_param (void)
{
  ldv_i_param_ptr i_param = NULL;

  i_param = XCNEW (ldv_info_param);

  return i_param;
}

ldv_i_type_ptr
ldv_create_info_type (void)
{
  ldv_i_type_ptr type = NULL;

  type = XCNEW (ldv_info_type);

  type->it_kind = LDV_IT_NONE;

  return type;
}

ldv_i_typedecl_ptr
ldv_create_info_typedecl (void)
{
  ldv_i_typedecl_ptr i_typedecl = NULL;

  i_typedecl = XCNEW (ldv_info_typedecl);

  i_typedecl->itd_kind = LDV_ITD_NONE;

  return i_typedecl;
}

ldv_i_var_ptr
ldv_create_info_var (void)
{
  ldv_i_var_ptr i_var = NULL;

  i_var = XCNEW (ldv_info_var);

  return i_var;
}

bool
ldv_isvoid (ldv_i_type_ptr type)
{
  if (type->it_kind != LDV_IT_PRIMITIVE)
    return false;

  if (!type->primitive_type->isvoid)
    return false;

  return true;
}

void
ldv_set_ldv (bool gcc_ldv)
{
  /* Set an ldv flag from gcc to cpp. */
  ldv_cpp_isldv = gcc_ldv;
}

void
ldv_set_ldv_opts (bool ldv_isinfo_matching_table)
{
  ldv_cpp_isinfo_matching_table = ldv_isinfo_matching_table;
}

void
ldv_set_ldv_stage (int gcc_ldv_stage)
{
  /* Set an ldv stage from gcc to cpp. */
  ldv_cpp_ldv_stage = gcc_ldv_stage;
}

char *
ldv_trunkate_braces (char *str)
{
  if (str[0] == '{')
    {
      str++;

      if (str[strlen (str) - 1] == '}')
        {
          str[strlen (str) - 1] = '\0';
          return str;
        }
    }

  ldv_cpp_fatal_error ("can't trunkate braces from \"%s\" string", str);

  return NULL;
}
