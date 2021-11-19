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

#include "ldv-aspect-types.h"
#include "ldv-core.h"
#include "ldv-io.h"
#include "ldv-opts.h"


ldv_aspect_pattern_ptr
ldv_create_aspect_pattern (void)
{
  ldv_aspect_pattern_ptr aspect_pattern = NULL;

  aspect_pattern = XCNEW (ldv_aspect_pattern);

  return aspect_pattern;
}

void
ldv_free_aspect_pattern (ldv_aspect_pattern_ptr aspect_pattern)
{
  ldv_list_ptr params;
  ldv_aspect_pattern_param_ptr param;

  free (CONST_CAST (char *, aspect_pattern->name));

  for (params = aspect_pattern->params
    ; params
    ; params = ldv_list_get_next (params))
    {
      param = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (params);
      free (CONST_CAST (char *, param->string));
      free (param);
    }

  ldv_list_delete_all (aspect_pattern->params);
  free (aspect_pattern);
}

ldv_aspect_pattern_param_ptr
ldv_create_aspect_pattern_param (void)
{
  ldv_aspect_pattern_param_ptr param = NULL;

  param = XCNEW (ldv_aspect_pattern_param);

  return param;
}

ldv_ab_ptr
ldv_create_body (void)
{
  ldv_ab_ptr body = NULL;

  body = XCNEW (ldv_advice_body);
  body->ab_text = ldv_create_str (LDV_T_B);

  return body;
}

ldv_ab_aspect_pattern_ptr
ldv_create_body_aspect_pattern (void)
{
  ldv_ab_aspect_pattern_ptr pattern = NULL;

  pattern = XCNEW (ldv_ab_aspect_pattern);

  return pattern;
}

ldv_pps_declarator_ptr
ldv_create_declarator (void)
{
  ldv_pps_declarator_ptr declarator = NULL;

  declarator = XCNEW (ldv_primitive_pointcut_signature_declarator);
  declarator->pps_declarator_kind = LDV_PPS_DECLARATOR_NONE;

  return declarator;
}

void
ldv_free_declarator (ldv_pps_declarator_ptr declarator)
{
  ldv_list_ptr func_arg_list = NULL;
  ldv_pps_func_arg_ptr func_arg = NULL;

  switch (declarator->pps_declarator_kind)
    {
    case LDV_PPS_DECLARATOR_FUNC:
      for (func_arg_list = declarator->func_arg_list
        ; func_arg_list
        ; func_arg_list = ldv_list_get_next (func_arg_list))
        {
          func_arg = (ldv_pps_func_arg_ptr) ldv_list_get_data (func_arg_list);
          ldv_free_pps_func_arg (func_arg);
        }

      ldv_list_delete_all (declarator->func_arg_list);
      break;

    case LDV_PPS_DECLARATOR_ID:
      ldv_free_id (declarator->declarator_name);
      break;

    case LDV_PPS_DECLARATOR_PTR:
      ldv_free_ptr_quals (declarator->pps_ptr_quals);
      break;

    case LDV_PPS_DECLARATOR_ARRAY:
      ldv_free_pps_array_size (declarator->pps_array_size);
      break;

    case LDV_PPS_DECLARATOR_NONE:
      break;

    default:
      internal_error ("incorrect primitive pointcut signature declarator kind \"%d\" is used", declarator->pps_declarator_kind);
    }

  free (declarator);
}

ldv_file_ptr
ldv_create_file (void)
{
  ldv_file_ptr file = NULL;

  file = XCNEW (ldv_file);
  file->file_name = ldv_create_str (LDV_T_FILE);

  return file;
}

void
ldv_free_file (ldv_file_ptr file)
{
  if (file)
    {
      ldv_free_str (file->file_name);
      free (file);
    }
  else
    internal_error ("file pointer wasn't initialized");
}

ldv_int_ptr
ldv_create_int (void)
{
  ldv_int_ptr integer = NULL;

  integer = XCNEW (ldv_int);

  return integer;
}

ldv_pps_array_size_ptr
ldv_create_pps_array_size (void)
{
  ldv_pps_array_size_ptr pps_array_size = NULL;

  pps_array_size = XCNEW (ldv_primitive_pointcut_signature_array_size);

  return pps_array_size;
}

void
ldv_free_pps_array_size (ldv_pps_array_size_ptr pps_array_size)
{
  free (pps_array_size);
}

ldv_pps_decl_ptr
ldv_create_pps_decl (void)
{
  ldv_pps_decl_ptr pps_decl = NULL;

  pps_decl = XCNEW (ldv_primitive_pointcut_signature_decl);
  pps_decl->pps_decl_kind = LDV_PPS_DECL_NONE;

  return pps_decl;
}

void
ldv_free_pps_decl (ldv_pps_decl_ptr pps_decl)
{
  ldv_pps_declarator_ptr declarator = NULL;
  ldv_list_ptr declarator_list = NULL;

  ldv_free_declspecs (pps_decl->pps_declspecs);

  for (declarator_list = pps_decl->pps_declarator; declarator_list; declarator_list = ldv_list_get_next (declarator_list))
    {
      declarator = (ldv_pps_declarator_ptr) ldv_list_get_data (declarator_list);
      ldv_free_declarator (declarator);
    }

  ldv_list_delete_all (pps_decl->pps_declarator);
  free (pps_decl);
}

/* TODO: clarify names for these 2 functions, since below there is pps_func_arg->pps_func_arg. */
ldv_pps_func_arg_ptr
ldv_create_pps_func_arg (void)
{
  ldv_pps_func_arg_ptr pps_func_arg = NULL;

  pps_func_arg = XCNEW (ldv_primitive_pointcut_signature_func_arg);

  return pps_func_arg;
}

void
ldv_free_pps_func_arg (ldv_pps_func_arg_ptr pps_func_arg)
{
  ldv_free_pps_decl (pps_func_arg->pps_func_arg);
  free (pps_func_arg);
}

ldv_pps_ptr_quals_ptr
ldv_create_ptr_quals (void)
{
  ldv_pps_ptr_quals_ptr ptr_quals = NULL;

  ptr_quals = XCNEW (ldv_primitive_pointcut_signature_ptr_quals);

  return ptr_quals;
}

void
ldv_free_ptr_quals (ldv_pps_ptr_quals_ptr ptr_quals)
{
  free (ptr_quals);
}

void
ldv_delete_body (ldv_ab_ptr body)
{
  if (body)
    {
      ldv_free_str (body->ab_text);
      free (body);
    }
  else
    internal_error ("body pointer wasn't initialized");
}

void
ldv_delete_id (ldv_id_ptr id)
{
  if (id)
    {
      ldv_free_str (id->id_name);
      free (id);
    }
  else
    internal_error ("id pointer wasn't initialized");
}

void
ldv_delete_int (ldv_int_ptr integer)
{
  if (integer)
    free (integer);
  else
    internal_error ("integer pointer wasn't initialized");
}

char *
ldv_get_aspect_pattern_env (ldv_aspect_pattern_ptr pattern)
{
  ldv_aspect_pattern_param_ptr param = NULL;
  char *value = NULL;

  if (!pattern->params || ldv_list_get_next (pattern->params)
    || !(param = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern->params))
    || !(param->kind == LDV_ASPECT_PATTERN_STRING))
    internal_error ("aspect pattern \"%s\" should have the only string parameter", pattern->name);

  if (!(value = getenv (param->string)))
    fatal_error (UNKNOWN_LOCATION, "couldn't obtain a value of environment variable \"%s\" corresponding to aspect pattern \"%s\"", param->string, pattern->name);

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
    internal_error ("integer pointer wasn't initialized");
}

unsigned int
ldv_get_current_line_number(const char *fname)
{
  FILE *fstream = NULL;
  int c;
  unsigned int line_number = 1;

  if ((fstream = fopen (fname, "r")) == NULL)
    internal_error ("can%'t open file \"%s\" for read: %m", fname);

  while ((c = ldv_getc (fstream)) != EOF)
    {
      if (c == '\n')
        line_number++;
    }

  fclose (fstream);

  return line_number;
}

void
ldv_putc_body (unsigned char c, ldv_ab_ptr body)
{
  if (body)
    ldv_putc_str (c, body->ab_text, LDV_T_B);
  else
    internal_error ("body pointer wasn't initialized");
}

void
ldv_putc_file (unsigned char c, ldv_file_ptr file)
{
  if (file)
    ldv_putc_str (c, file->file_name, LDV_T_FILE);
  else
    internal_error ("file pointer wasn't initialized");
}

void
ldv_puts_body (const char *str, ldv_ab_ptr body)
{
  if (body)
    ldv_puts_str (str, body->ab_text, LDV_T_B);
  else
    internal_error ("body pointer wasn't initialized");
}

void
ldv_puts_file (const char *str, ldv_file_ptr file)
{
  if (file)
    ldv_puts_str (str, file->file_name, LDV_T_FILE);
  else
    internal_error ("file pointer wasn't initialized");
}
