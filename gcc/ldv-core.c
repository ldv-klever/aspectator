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
#include "diagnostic-core.h"
#include "toplev.h"

#include "ldv-aspect-types.h"
#include "ldv-core.h"
#include "ldv-io.h"


static ldv_str_ptr ldv_create_str (ldv_token_k);
static void ldv_putc_str (unsigned char, ldv_str_ptr, ldv_token_k);
static void ldv_puts_str (const char *, ldv_str_ptr, ldv_token_k);
static unsigned int ldv_unique_numb = 0;


ldv_aspect_pattern_ptr
ldv_create_aspect_pattern (void)
{
  ldv_aspect_pattern_ptr aspect_pattern = NULL;

  aspect_pattern = XCNEW (ldv_aspect_pattern);
  ldv_print_info (LDV_INFO_MEM, "aspect pattern memory was released");

  return aspect_pattern;
}

ldv_aspect_pattern_param_ptr
ldv_create_aspect_pattern_param (void)
{
  ldv_aspect_pattern_param_ptr param = NULL;

  param = XCNEW (ldv_aspect_pattern_param);
  ldv_print_info (LDV_INFO_MEM, "aspect pattern parameter memory was released");

  return param;
}

ldv_ab_ptr
ldv_create_body (void)
{
  ldv_ab_ptr body = NULL;

  body = XCNEW (ldv_advice_body);
  ldv_print_info (LDV_INFO_MEM, "body memory was released");

  body->ab_text = ldv_create_str (LDV_T_B);

  return body;
}

ldv_ab_aspect_pattern_ptr
ldv_create_body_aspect_pattern (void)
{
  ldv_ab_aspect_pattern_ptr pattern = NULL;

  pattern = XCNEW (ldv_ab_aspect_pattern);
  ldv_print_info (LDV_INFO_MEM, "advice body aspect pattern memory was released");

  return pattern;
}

ldv_pps_declarator_ptr
ldv_create_declarator (void)
{
  ldv_pps_declarator_ptr declarator = NULL;

  declarator = XCNEW (ldv_primitive_pointcut_signature_declarator);
  ldv_print_info (LDV_INFO_MEM, "declarator memory was released");

  declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_NONE;

  return declarator;
}

ldv_file_ptr
ldv_create_file (void)
{
  ldv_file_ptr file = NULL;

  file = XCNEW (ldv_file);
  ldv_print_info (LDV_INFO_MEM, "file memory was released");

  file->file_name = ldv_create_str (LDV_T_FILE);

  return file;
}

ldv_id_ptr
ldv_create_id (void)
{
  ldv_id_ptr id = NULL;

  id = XCNEW (ldv_id);
  ldv_print_info (LDV_INFO_MEM, "identifier memory was released");

  id->id_name = ldv_create_str (LDV_T_ID);

  return id;
}

ldv_int_ptr
ldv_create_int (void)
{
  ldv_int_ptr integer = NULL;

  integer = XCNEW (ldv_int);
  ldv_print_info (LDV_INFO_MEM, "integer number memory was released");

  return integer;
}

ldv_pps_array_size_ptr
ldv_create_pps_array_size (void)
{
  ldv_pps_array_size_ptr pps_array_size = NULL;

  pps_array_size = XCNEW (ldv_primitive_pointcut_signature_array_size);
  ldv_print_info (LDV_INFO_MEM, "array size memory was released");

  return pps_array_size;
}

ldv_pps_decl_ptr
ldv_create_pps_decl (void)
{
  ldv_pps_decl_ptr pps_decl = NULL;

  pps_decl = XCNEW (ldv_primitive_pointcut_signature_decl);
  ldv_print_info (LDV_INFO_MEM, "primitive pointcut signature declaration memory was released");

  pps_decl->pps_decl_kind = LDV_PPS_DECL_NONE;

  return pps_decl;
}

ldv_pps_func_arg_ptr
ldv_create_pps_func_arg (void)
{
  ldv_pps_func_arg_ptr pps_func_arg = NULL;

  pps_func_arg = XCNEW (ldv_primitive_pointcut_signature_func_arg);
  ldv_print_info (LDV_INFO_MEM, "function argument memory was released");

  return pps_func_arg;
}

ldv_pps_ptr_quals_ptr
ldv_create_ptr_quals (void)
{
  ldv_pps_ptr_quals_ptr ptr_quals = NULL;

  ptr_quals = XCNEW (ldv_primitive_pointcut_signature_ptr_quals);
  ldv_print_info (LDV_INFO_MEM, "declarator pointer qualifiers memory was released");

  return ptr_quals;
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
      LDV_FATAL_ERROR ("unrecognize ldv token kind \"%d\"", token_kind);
    }

  string = XCNEW (ldv_string);
  ldv_print_info (LDV_INFO_MEM, "string memory was released");

  string->text = XCNEWVEC (char, (len_start + 1));
  ldv_print_info (LDV_INFO_MEM, "string text memory was released");

  string->text[0] = '\0';
  string->max_len = len_start;

  return string;
}

ldv_str_ptr
ldv_create_string (void)
{
  return ldv_create_str (LDV_T_STRING);
}

ldv_text_ptr
ldv_create_text (void)
{
  ldv_text_ptr text = NULL;

  text = XCNEW (ldv_text);
  ldv_print_info (LDV_INFO_MEM, "text memory was released");

  text->text = ldv_create_str (LDV_T_TEXT);

  return text;
}

void
ldv_delete_body (ldv_ab_ptr body)
{
  if (body)
    {
      ldv_delete_str (body->ab_text);

      free (body);
      ldv_print_info (LDV_INFO_MEM, "body memory was free");
    }
  else
    {
      LDV_FATAL_ERROR ("body pointer wasn't initialized");
    }
}

void
ldv_delete_file (ldv_file_ptr file)
{
  if (file)
    {
      ldv_delete_str (file->file_name);

      free (file);
      ldv_print_info (LDV_INFO_MEM, "file memory was free");
    }
  else
    {
      LDV_FATAL_ERROR ("file pointer wasn't initialized");
    }
}

void
ldv_delete_id (ldv_id_ptr id)
{
  if (id)
    {
      ldv_delete_str (id->id_name);

      free (id);
      ldv_print_info (LDV_INFO_MEM, "identifier memory was free");
    }
  else
    {
      LDV_FATAL_ERROR ("id pointer wasn't initialized");
    }
}

void
ldv_delete_int (ldv_int_ptr integer)
{
  if (integer)
    {
      free (integer);
      ldv_print_info (LDV_INFO_MEM, "integer number memory was free");
    }
  else
    {
      LDV_FATAL_ERROR ("integer pointer wasn't initialized");
    }
}

void
ldv_delete_str (ldv_str_ptr str)
{
  if (str)
    {
      free (str->text);
      ldv_print_info (LDV_INFO_MEM, "string text memory was free");

      free (str);
      ldv_print_info (LDV_INFO_MEM, "string memory was free");
    }
  else
    {
      LDV_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

void
ldv_delete_text (ldv_text_ptr text)
{
  if (text)
    {
      ldv_delete_str (text->text);

      free (text);
      ldv_print_info (LDV_INFO_MEM, "text memory was free");
    }
  else
    {
      LDV_FATAL_ERROR ("text pointer wasn't initialized");
    }
}

const char *
ldv_itoa (unsigned int n)
{
  int int_digits, order;
  char *str = NULL;

  /* Obtain the number of digits that are contained in an unsigned integer
     number. */
  for (int_digits = 1, order = 10.0; n / order > 1.0; int_digits++, order *= 10) ;

  str = XCNEWVEC (char, (int_digits + 1));
  ldv_print_info (LDV_INFO_MEM, "string memory was released");

  sprintf (str, "%d", n);

  return str;
}

char *
ldv_get_aspect_pattern_env (ldv_aspect_pattern_ptr pattern)
{
  ldv_aspect_pattern_param_ptr param = NULL;
  char *value = NULL;

  if (!pattern->params || ldv_list_get_next (pattern->params)
    || !(param = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern->params))
    || !(param->kind == LDV_ASPECT_PATTERN_STRING))
    {
      LDV_FATAL_ERROR ("aspect pattern \"%s\" should have the only string parameter", pattern->name);
    }

  if (!(value = getenv (param->string)))
    {
      LDV_FATAL_ERROR ("couldn't obtain a value of environment variable \"%s\" corresponding to aspect pattern \"%s\"", param->string, pattern->name);
    }

  return value;
}

char *
ldv_get_body_text (ldv_ab_ptr body)
{
  return ldv_cpp_get_body_text (body);
}

char *
ldv_get_file_name (ldv_file_ptr file)
{
  return ldv_cpp_get_file_name (file);
}

char *
ldv_get_id_name (ldv_id_ptr id)
{
  return ldv_cpp_get_id_name (id);
}

int
ldv_get_int (ldv_int_ptr integer)
{
  if (integer)
    return integer->numb;
  else
    {
      LDV_FATAL_ERROR ("integer pointer wasn't initialized");
    }
}

char *
ldv_get_str (ldv_str_ptr str)
{
  return ldv_cpp_get_str (str);
}

char *
ldv_get_text (ldv_text_ptr text)
{
  if (text)
    return ldv_get_str (text->text);
  else
    {
      LDV_FATAL_ERROR ("text pointer wasn't initialized");
    }
}

unsigned int
ldv_get_unique_numb(void)
{
  const char *file_name = NULL;
  FILE *stream;
  unsigned int first_unique_numb = 0;

  /* Try to initialize first unique number on the basis of value stored in a
   * special file or use default value as initializer (0). */
  if (!ldv_unique_numb
    && (file_name = getenv ("LDV_UNIQUE_NUMB"))
    && (stream = ldv_open_file_stream (file_name, "r")))
    {
      if (fscanf (stream, "%u", &first_unique_numb) == 1)
        {
          ldv_unique_numb = first_unique_numb;
          fclose (stream);
        }
      else
        {
          LDV_FATAL_ERROR ("can't read unsigned integer from file '%s'", file_name);
        }
    }

  /* Next unique number is following for the current one. */
  ldv_unique_numb++;

  /* Try to dump a current unique number to the special file. */
  if ((file_name = getenv ("LDV_UNIQUE_NUMB"))
    && (stream = ldv_open_file_stream (file_name, "w")))
    {
      fprintf (stream, "%u", ldv_unique_numb);
      fclose (stream);
    }

  return ldv_unique_numb;
}

ldv_pps_declspecs_ptr
ldv_merge_declspecs (ldv_pps_declspecs_ptr declspecs_first, ldv_pps_declspecs_ptr declspecs_second)
{
  ldv_pps_declspecs_ptr declspecs = NULL;

  declspecs = ldv_create_declspecs ();

  /* In that case when merge isn't needed. */
  if (!declspecs_second)
    {
      declspecs = declspecs_first;
      return declspecs;
    }

  /* Merge all declaration specifiers from two structures. */
  declspecs->type_name = declspecs_first->type_name ? declspecs_first->type_name : declspecs_second->type_name;
  declspecs->istypedef = declspecs_first->istypedef || declspecs_second->istypedef;
  declspecs->isextern = declspecs_first->isextern || declspecs_second->isextern;
  declspecs->isstatic = declspecs_first->isstatic || declspecs_second->isstatic;
  declspecs->isauto = declspecs_first->isauto || declspecs_second->isauto;
  declspecs->isregister = declspecs_first->isregister || declspecs_second->isregister;
  declspecs->isvoid = declspecs_first->isvoid || declspecs_second->isvoid;
  declspecs->ischar = declspecs_first->ischar || declspecs_second->ischar;
  declspecs->isint = declspecs_first->isint || declspecs_second->isint;
  declspecs->isfloat = declspecs_first->isfloat || declspecs_second->isfloat;
  declspecs->isdouble = declspecs_first->isdouble || declspecs_second->isdouble;

  /* Remove 'int' type specifier if 'double' or 'char' is encountered. It's done
     because of 'int' may be specified implicitly when something of 'short',
     'long', 'signed', 'unsigned' are specified. */
  if (declspecs->isdouble || declspecs->ischar)
    declspecs->isint = false;

  declspecs->isbool = declspecs_first->isbool || declspecs_second->isbool;
  declspecs->iscomplex = declspecs_first->iscomplex || declspecs_second->iscomplex;
  declspecs->isshort = declspecs_first->isshort || declspecs_second->isshort;

  /* Process "long long" as special case. */
  if (declspecs_first->islong && declspecs_second->islong)
    {
      declspecs->islong = false;
      declspecs->islonglong = true;
    }
  else
    {
      declspecs->islong = declspecs_first->islong || declspecs_second->islong;
      declspecs->islonglong = declspecs_first->islonglong || declspecs_second->islonglong;
    }

  declspecs->issigned = declspecs_first->issigned || declspecs_second->issigned;
  declspecs->isunsigned = declspecs_first->isunsigned || declspecs_second->isunsigned;
  declspecs->isstruct = declspecs_first->isstruct || declspecs_second->isstruct;
  declspecs->isunion = declspecs_first->isunion || declspecs_second->isunion;
  declspecs->isenum = declspecs_first->isenum || declspecs_second->isenum;
  declspecs->istypedef_name = declspecs_first->istypedef_name || declspecs_second->istypedef_name;
  declspecs->isconst = declspecs_first->isconst || declspecs_second->isconst;
  declspecs->isrestrict = declspecs_first->isrestrict || declspecs_second->isrestrict;
  declspecs->isvolatile = declspecs_first->isvolatile || declspecs_second->isvolatile;
  declspecs->isinline = declspecs_first->isinline || declspecs_second->isinline;

  return declspecs;
}

void
ldv_putc_body (unsigned char c, ldv_ab_ptr body)
{
  if (body)
    ldv_putc_str (c, body->ab_text, LDV_T_B);
  else
    {
      LDV_FATAL_ERROR ("body pointer wasn't initialized");
    }
}

void
ldv_putc_file (unsigned char c, ldv_file_ptr file)
{
  if (file)
    ldv_putc_str (c, file->file_name, LDV_T_FILE);
  else
    {
      LDV_FATAL_ERROR ("file pointer wasn't initialized");
    }
}

void
ldv_putc_id (unsigned char c, ldv_id_ptr id)
{
  if (id)
    ldv_putc_str (c, id->id_name, LDV_T_ID);
  else
    {
      LDV_FATAL_ERROR ("id pointer wasn't initialized");
    }
}

void
ldv_putc_str (unsigned char c, ldv_str_ptr string, ldv_token_k token_kind)
{
  char *str_text = NULL;
  unsigned int len_add;

  if (!string)
    {
      LDV_FATAL_ERROR ("string pointer wasn't initialized");
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
      LDV_FATAL_ERROR ("unrecognize ldv token kind \"%d\"", token_kind);
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
      LDV_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

void
ldv_putc_text (unsigned char c, ldv_text_ptr text)
{
  if (text)
    ldv_putc_str (c, text->text, LDV_T_TEXT);
  else
    {
      LDV_FATAL_ERROR ("text pointer wasn't initialized");
    }
}

void
ldv_puts_body (const char *str, ldv_ab_ptr body)
{
  if (body)
    ldv_puts_str (str, body->ab_text, LDV_T_B);
  else
    {
      LDV_FATAL_ERROR ("body pointer wasn't initialized");
    }
}

void
ldv_puts_file (const char *str, ldv_file_ptr file)
{
  if (file)
    ldv_puts_str (str, file->file_name, LDV_T_FILE);
  else
    {
      LDV_FATAL_ERROR ("file pointer wasn't initialized");
    }
}

void
ldv_puts_id (const char *str, ldv_id_ptr id)
{
  if (id)
    ldv_puts_str (str, id->id_name, LDV_T_ID);
  else
    {
      LDV_FATAL_ERROR ("id pointer wasn't initialized");
    }
}

void
ldv_puts_str (const char *str, ldv_str_ptr string, ldv_token_k token_kind)
{
  const char *c = NULL;

  if (!str)
    {
      LDV_FATAL_ERROR ("symbol pointer wasn't initialized");
    }

  if (!string)
    {
      LDV_FATAL_ERROR ("string pointer wasn't initialized");
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
      LDV_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

void
ldv_puts_text (const char *str, ldv_text_ptr text)
{
  if (text)
    ldv_puts_str (str, text->text, LDV_T_TEXT);
  else
    {
      LDV_FATAL_ERROR ("text pointer wasn't initialized");
    }
}
