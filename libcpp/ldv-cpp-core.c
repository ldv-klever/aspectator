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


bool ldv_cpp = false;
bool ldv_cpp_isinfo_matching_table = false;
int ldv_cpp_stage = -1;

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
    return ldv_get_str (body->ab_text);

  LDV_CPP_FATAL_ERROR ("body pointer wasn't initialized");

  return NULL;
}

char *
ldv_cpp_get_file_name (ldv_file_ptr file)
{
  if (file)
    return ldv_get_str (file->file_name);

  LDV_CPP_FATAL_ERROR ("file pointer wasn't initialized");

  return NULL;
}

char *
ldv_cpp_get_id_name (ldv_id_ptr id)
{
  if (id)
    return ldv_get_str (id->id_name);

  LDV_CPP_FATAL_ERROR ("id pointer wasn't initialized");

  return NULL;
}

char *
ldv_get_str (ldv_str_ptr str)
{
  if (str)
    return str->text;

  LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");

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

ldv_id_ptr
ldv_create_id (void)
{
  ldv_id_ptr id = NULL;

  id = XCNEW (ldv_id);
  /* TODO #371 ldv_print_info (LDV_INFO_MEM, "identifier memory was released");*/

  id->id_name = ldv_create_str (LDV_T_ID);
  id->isany_chars = false;

  return id;
}

ldv_str_ptr
ldv_create_str (ldv_token_k token_kind)
{
  ldv_str_ptr string = NULL;
  unsigned int len_start;

  /* Start string length depends on a ldv token kind. */
  switch (token_kind)
    {
    case LDV_T_FILE:
      len_start = LDV_FILE_LEN_START;
      break;

    case LDV_T_B:
      len_start = LDV_B_LEN_START;
      break;

    case LDV_T_ID:
      len_start = LDV_ID_LEN_START;
      break;

    case LDV_T_STRING:
      len_start = LDV_STRING_LEN_START;
      break;

    case LDV_T_TEXT:
      len_start = LDV_TEXT_LEN_START;
      break;

    default:
      LDV_CPP_FATAL_ERROR ("unrecognize ldv token kind \"%d\"", token_kind);
    }

  string = XCNEW (ldv_string);
  /* TODO #371 ldv_print_info (LDV_INFO_MEM, "string memory was released");*/

  string->text = XCNEWVEC (char, (len_start + 1));
  /* TODO #371 ldv_print_info (LDV_INFO_MEM, "string text memory was released");*/

  string->text[0] = '\0';
  string->max_len = len_start;

  return string;
}

ldv_str_ptr
ldv_create_string (void)
{
  return ldv_create_str (LDV_T_STRING);
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

FILE *
ldv_open_file_stream (const char *file_name, const char *mode)
{
  FILE *file_stream;

  if ((file_stream = fopen (file_name, mode)) == NULL)
    {
      LDV_CPP_FATAL_ERROR ("can't open file \"%s\" in mode \"%s\"", file_name, mode);
    }

  /* TODO #371 ldv_print_info (LDV_INFO_IO, "file \"%s\" was successfully opened in mode \"%s\"", file_name, mode); */

  return file_stream;
}

void
ldv_putc_str (unsigned char c, ldv_str_ptr string, ldv_token_k token_kind)
{
  char *str_text = NULL;
  unsigned int len_add;

  if (!string)
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }

  /* An additional string length depends on a ldv token kind. */
  switch (token_kind)
    {
    case LDV_T_FILE:
      len_add = LDV_FILE_LEN_ADD;
      break;

    case LDV_T_B:
      len_add = LDV_B_LEN_ADD;
      break;

    case LDV_T_ID:
      len_add = LDV_ID_LEN_ADD;
      break;

    case LDV_T_STRING:
      len_add = LDV_STRING_LEN_ADD;
      break;

    case LDV_T_TEXT:
      len_add = LDV_TEXT_LEN_ADD;
      break;

    default:
      LDV_CPP_FATAL_ERROR ("unrecognize ldv token kind \"%d\"", token_kind);
    }

  /* If a character can be added to a current string text, do it. */
  if (string->len < string->max_len)
    {
      string->text[string->len] = c;
      string->text[string->len + 1] = '\0';
      string->len++;
    }
  /* Otherwise a new memory is allocated for a large string text. */
  else
    {
      /* Remember a current string text. */
      str_text = XCNEWVEC (char, (string->len + 1));
      memcpy (str_text, string->text, string->len + 1);

      /* Allocate a new memory for a large string text. */
      string->text = (char *) xrealloc (string->text, sizeof (char) * (string->len + len_add + 1));

      /* Back a string text and increase its maximum length. */
      memcpy (string->text, str_text, string->len + 1);
      string->max_len += len_add;

      /* Put a new character to a large string text. */
      ldv_putc_str (c, string, token_kind);
    }
}

void
ldv_putc_string (unsigned char c, ldv_str_ptr string)
{
  if (string)
    ldv_putc_str (c, string, LDV_T_STRING);
  else
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

void
ldv_puts_id (const char *str, ldv_id_ptr id)
{
  if (id)
    ldv_puts_str (str, id->id_name, LDV_T_ID);
  else
    {
      LDV_CPP_FATAL_ERROR ("id pointer wasn't initialized");
    }
}

void
ldv_puts_str (const char *str, ldv_str_ptr string, ldv_token_k token_kind)
{
  const char *c = NULL;

  if (!str)
    {
      LDV_CPP_FATAL_ERROR ("symbol pointer wasn't initialized");
    }

  if (!string)
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }

  /* Put every symbol of a string to an internal string. */
  for (c = str; c && *c; c++)
    ldv_putc_str (*c, string, token_kind);
}

void
ldv_puts_string (const char *str, ldv_str_ptr string)
{
  if (string)
    ldv_puts_str (str, string, LDV_T_STRING);
  else
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

void
ldv_set_ldv (bool ldv_gcc)
{
  /* Set an ldv flag from gcc to cpp. */
  ldv_cpp = ldv_gcc;
}

void
ldv_set_ldv_opts (bool ldv_gcc_isinfo_matching_table)
{
  ldv_cpp_isinfo_matching_table = ldv_gcc_isinfo_matching_table;
}

void
ldv_set_ldv_stage (int ldv_gcc_stage)
{
  /* Set an ldv stage from gcc to cpp. */
  ldv_cpp_stage = ldv_gcc_stage;
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

  LDV_CPP_FATAL_ERROR ("can't trunkate braces from \"%s\" string", str);

  return NULL;
}
