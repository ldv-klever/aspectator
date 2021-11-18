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

#include "ldv-cpp-advice-weaver.h"
#include "ldv-cpp-converter.h"
#include "ldv-cpp-core.h"
#include "ldv-cpp-pointcut-matcher.h"


/* Macro from ldv-io.h that's needed for printing of matched just by name macros. */
#define LDV_MATCHED_BY_NAME (stderr)


static htab_t ldv_query_result_htab = NULL;


static ldv_aspect_pattern_param_ptr ldv_consume_aspect_pattern_param (ldv_list_ptr_ptr, LDV_EVALUATE_ASPECT_PATTERN_FUNC);
static const char *ldv_get_aspect_pattern_str_param (ldv_aspect_pattern_param_ptr);
static void ldv_free_aspect_pattern_str_param (ldv_aspect_pattern_param_ptr);
static LDV_EVALUATE_ASPECT_PATTERN_FUNC ldv_cpp_evaluate_aspect_pattern;
static char *ldv_get_actual_args (void);
static char *ldv_cpp_print_macro_signature (ldv_i_macro_ptr i_macro);
static FILE *ldv_open_aspect_pattern_fprintf_file_stream (const char *);
static void ldv_store_query_results (ldv_aspect_pattern_param_ptr, ldv_aspect_pattern_param_ptr, ldv_list_ptr);

ldv_aspect_pattern_param_ptr
ldv_consume_aspect_pattern_param (ldv_list_ptr_ptr aspect_pattern_params, LDV_EVALUATE_ASPECT_PATTERN_FUNC evaluate_aspect_pattern_func)
{
  ldv_aspect_pattern_param_ptr param = NULL, param_cur = NULL;
  const char *text = NULL;
  unsigned int number;
  const char *sign = NULL;
  ldv_str_ptr str;

  while (1)
    {
      param_cur = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (*aspect_pattern_params);
      /* Move pointer to next aspect pattern parameter. */
      *aspect_pattern_params = ldv_list_get_next (*aspect_pattern_params);
      /* Forget previously evaluated strings if so. */
      param_cur->string_eval = NULL;

      /* We are interested here in parameters to be evaluated. */
      if (param_cur->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
        {
          /* To keep evaluated values of parameters use parameters themselves
             since parameter evaluation may lead to either string or integer,
             and parameters themselves may be either strings or integers.
             Parameter signatures are of interest here. */
          evaluate_aspect_pattern_func (param_cur->aspect_pattern, &text, &number, &sign);
          if (text)
            {
              param_cur->string = text;
              /* Forget about evaluated text since it is used in condition above. */
              text = NULL;
            }
          else
            param_cur->integer = number;
        }

      if (!param)
        param = param_cur;
      /* Aspect pattern parameter consists of several ones. This can be the
         case just when each of them is either string or can be evaluated as
         string. Concatenate these strings and store result into firs
         parameter. */
      else
        {
          if (!param->string || !param_cur->string)
            {
              LDV_CPP_FATAL_ERROR ("can't concatenate non-string aspect pattern patterns");
            }

          str = ldv_create_string ();

          /* See comment below. */
          if (param->string_eval)
            {
              ldv_puts_string (param->string_eval, str);
              /* param->string_eval will be overwritten below. */
              free ((void *)param->string_eval);
            }
          else
            ldv_puts_string (param->string, str);

          ldv_puts_string (param_cur->string, str);

          if (param_cur->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
            free ((void *)param_cur->string);

          /* We can not change pure string aspect pattern parameters since
             next time we will use already changed values. */
          if (param->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
            {
              free ((void *)param->string);
              param->string = ldv_get_str (str);
            }
          else
            param->string_eval = ldv_get_str (str);

          /* String text will be freed later. */
          free (str);
        }

      /* Aspect pattern parameter was parsed. */
      if (param_cur->next_param || !*aspect_pattern_params)
        break;
    }

  return param;
}

const char *
ldv_get_aspect_pattern_str_param (ldv_aspect_pattern_param_ptr param)
{
  /* String aspect pattern parameters can be additionally evaluated in
     ldv_consume_aspect_pattern_param(). */
  if (param->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
    return param->string;
  else if (param->kind == LDV_ASPECT_PATTERN_STRING)
    {
      if (param->string_eval)
        return param->string_eval;
      else
        return param->string;
    }

  LDV_CPP_FATAL_ERROR ("can not get aspect pattern string parameter");

  return NULL;
}

void
ldv_free_aspect_pattern_str_param (ldv_aspect_pattern_param_ptr param)
{
  if (param->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
    {
      free ((void *)param->string);
      return;
    }
  else if (param->kind == LDV_ASPECT_PATTERN_STRING)
    {
      if (param->string_eval)
        free ((void *)param->string_eval);

      return;
    }

  LDV_CPP_FATAL_ERROR ("can not get aspect pattern string parameter");
}

void
ldv_process_aspect_pattern_fprintf (ldv_list_ptr aspect_pattern_params, LDV_EVALUATE_ASPECT_PATTERN_FUNC evaluate_aspect_pattern_func)
{
  ldv_aspect_pattern_param_ptr param1 = NULL, param2 = NULL;
  ldv_list_ptr evaluated_aspect_patter_params = NULL;

  /* First parameter specifies file where information request result to be
     printed. */
  param1 = ldv_consume_aspect_pattern_param (&aspect_pattern_params, evaluate_aspect_pattern_func);
  /* Second parameter specifies format string like for standard printf
     function. */
  param2 = ldv_consume_aspect_pattern_param (&aspect_pattern_params, evaluate_aspect_pattern_func);

  /* Evaluate other aspect pattern parameters. */
  while (aspect_pattern_params)
    ldv_list_push_back (&evaluated_aspect_patter_params,
                        ldv_consume_aspect_pattern_param (&aspect_pattern_params, evaluate_aspect_pattern_func));

  /* Immediate printing query results is too inefficient since we need to open
     files and write there exclusively (the same files may be opened and
     written too very many times). So, let's accumulate all query results
     first and print them at the end of operation. */
  ldv_store_query_results (param1, param2, evaluated_aspect_patter_params);
}

void
ldv_cpp_define (struct cpp_reader *pfile)
{
  char *define = NULL;
  const char *name = NULL;
  ldv_list_ptr i_macro_param_list = NULL;
  ldv_i_macro_func_param_ptr i_macro_param = NULL;
  unsigned int param_numb;
  unsigned int param_len;
  unsigned int comma_numb;
  char *value = NULL;
  unsigned int printed_len;

  name = ldv_cpp_get_id_name (ldv_i_match->i_macro_aspect->macro_name);
  value = ldv_convert_body_to_macro (ldv_i_match->a_definition->a_body);

  /* Create a macro function definition. */
  if (ldv_i_match->i_macro_aspect->macro_kind == LDV_PPS_MACRO_FUNC)
    {
      /* Count a lenght and the number of all macro function definition parameters. */
      for (i_macro_param_list = ldv_i_match->i_macro_aspect->macro_param, param_len = 0, param_numb = 0
        ; i_macro_param_list
        ; i_macro_param_list = ldv_list_get_next (i_macro_param_list), ++param_numb)
        {
          i_macro_param = (ldv_i_macro_func_param_ptr) ldv_list_get_data (i_macro_param_list);
          param_len += strlen (ldv_cpp_get_id_name (i_macro_param->name));
        }

      /* A Macro function definition for cpp_define function is
         'name(param1,param2,...)=value'. Take into account that there may be
         empty list of macro parameters. In this case commas number should be 0
         (#3332). */
      comma_numb = param_numb ? param_numb - 1 : 0;
      define = XCNEWVEC (char, (strlen (name) + 1 + param_len + comma_numb + 1 + 1 + strlen (value) + 1));

      printed_len = 0;
      printed_len += sprintf (define + printed_len, "%s(", name);

      for (i_macro_param_list = ldv_i_match->i_macro_aspect->macro_param
        ; i_macro_param_list
        ; i_macro_param_list = ldv_list_get_next (i_macro_param_list))
        {
          i_macro_param = (ldv_i_macro_func_param_ptr) ldv_list_get_data (i_macro_param_list);
          printed_len += sprintf (define + printed_len, "%s", ldv_cpp_get_id_name (i_macro_param->name));

          if (ldv_list_get_next (i_macro_param_list))
            printed_len += sprintf (define + printed_len, ",");
        }

      printed_len += sprintf (define + printed_len, ")=%s", value);

      cpp_define (pfile, define);

      /* TODO print these messages just in case of debug
      fprintf (stderr, "Create cpp definition \"%s\".\n", define);
      */
    }
  /* Create a macro definition. */
  else
    {
      /* A Macro definition for cpp_define function is 'name=value'. */
      define = XCNEWVEC (char, (strlen (name) + 1 + strlen (value) + 1));

      sprintf (define, "%s=%s", name, value);

      cpp_define (pfile, define);

      /* TODO print these messages just in case of debug
      fprintf (stderr, "Create cpp definition \"%s\".\n", define);
      */
    }
}

void
ldv_cpp_undef (struct cpp_reader *pfile)
{
  const char *name = NULL;

  name = ldv_cpp_get_id_name (ldv_i_match->i_macro_aspect->macro_name);

  /* Undef a matched macro definition. It may be either a simple macro
     definition or a macro function definition. */
  cpp_undef (pfile, name);
}

void
ldv_cpp_evaluate_aspect_pattern (ldv_aspect_pattern_ptr pattern, const char **text, unsigned int *number, __attribute__((unused)) const char **sign)
{
  char *arg_value = NULL;
  ldv_list_ptr arg_value_list = NULL;
  unsigned int i;

  if ((!strcmp (pattern->name, "macro_signature")) || (!strcmp (pattern->name, "signature")))
    *text = ldv_cpp_print_macro_signature (ldv_i_match->i_macro);
  else if (!strcmp (pattern->name, "macro_name"))
    *text = ldv_copy_str (ldv_cpp_get_id_name (ldv_i_match->i_macro_aspect->macro_name));
  else if (!strcmp (pattern->name, "arg_val"))
    {
      for (i = 1, arg_value_list = ldv_i_match->i_macro->macro_param_value
      ; arg_value_list
      ; i++, arg_value_list = ldv_list_get_next (arg_value_list))
      {
        arg_value = (char *) ldv_list_get_data (arg_value_list);

        if (i == pattern->arg_numb)
        {
          *text = arg_value;
          break;
        }
      }
      if (!*text)
        {
          LDV_CPP_FATAL_ERROR ("required parameter has number \"%d\" that exceeds the maximum one \"%d\"", pattern->arg_numb, (i - 1));
        }
    }
  else if (!strcmp (pattern->name, "arg_numb"))
    *number = ldv_list_len (ldv_i_match->i_macro->macro_param);
  else if (!strcmp (pattern->name, "actual_args"))
    {
      *text = ldv_get_actual_args ();
      if (!*text)
          *text = ldv_copy_str ("NULL");
    }
  else if (!strcmp (pattern->name, "path"))
    *text = ldv_copy_str (ldv_i_match->i_macro->file_path);
  else if (!strcmp (pattern->name, "line"))
    *number = ldv_i_match->i_macro->line;
  else if (!strcmp (pattern->name, "expansion_path"))
    *text = ldv_copy_str (ldv_i_match->i_macro->expansion_file_path);
  else if (!strcmp (pattern->name, "expansion_line"))
    *number = ldv_i_match->i_macro->expansion_line;
  else
    {
      LDV_CPP_FATAL_ERROR ("aspect pattern \"%s\" wasn't weaved", pattern->name);
    }
}

char *
ldv_get_actual_args (void)
{
  unsigned int i;
  const char *i_str;
  ldv_list_ptr actual_args_list = NULL;
  char *actual_arg = NULL;
  ldv_str_ptr actual_args = NULL;
  char *res = NULL;

  /* Walk through macro function arguments. Result will look like:
   *   actual_arg1=actual_arg1_value, actual_arg2=actual_arg2_value
   */
  for (i = 1, actual_args_list = ldv_i_match->i_macro->macro_param_value
    ; actual_args_list
    ; i++, actual_args_list = ldv_list_get_next (actual_args_list))
    {
      actual_arg = (char *) ldv_list_get_data (actual_args_list);

      if (actual_args)
        ldv_puts_string (", ", actual_args);
      else
        actual_args = ldv_create_string ();

      ldv_puts_string ("actual_arg", actual_args);

      i_str = ldv_itoa (i);
      ldv_puts_string (i_str, actual_args);
      free ((void *)i_str);

      ldv_putc_string ('=', actual_args);
      ldv_puts_string (actual_arg, actual_args);
    }

  if (actual_args)
    {
      res = ldv_copy_str (ldv_get_str (actual_args));
      ldv_free_str (actual_args);
    }

  return res;
}

char *
ldv_cpp_print_macro_signature (ldv_i_macro_ptr i_macro)
{
  ldv_list_ptr i_macro_param_list = NULL;
  ldv_id_ptr i_macro_param = NULL;
  ldv_str_ptr text = ldv_create_string ();

  ldv_puts_string (ldv_cpp_get_id_name (i_macro->macro_name), text);

  if (i_macro->macro_kind == LDV_PPS_MACRO_FUNC)
    ldv_puts_string (" (", text);

  for (i_macro_param_list = i_macro->macro_param
    ; i_macro_param_list
    ; i_macro_param_list = ldv_list_get_next (i_macro_param_list))
    {
      i_macro_param = (ldv_id_ptr) ldv_list_get_data (i_macro_param_list);

      ldv_puts_string (ldv_cpp_get_id_name (i_macro_param), text);

      if (ldv_list_get_next (i_macro_param_list))
        ldv_puts_string (", ", text);
    }

  if (i_macro->macro_kind == LDV_PPS_MACRO_FUNC)
    ldv_puts_string (")", text);

  return ldv_get_str (text);
}

void
ldv_cpp_weave (void)
{
  ldv_ak a_kind;
  ldv_ppk pp_kind;
  ldv_ab_ptr body;
  ldv_list_ptr body_patterns = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;

  a_kind = ldv_i_match->a_definition->a_declaration->a_kind;
  pp_kind = ldv_i_match->p_pointcut->pp_kind;
  body = ldv_i_match->a_definition->a_body;

  /* Now here is supported just printing information on macro expansions or definitions.
   * Weaving macrodefinitions are implemented in ldv_cpp_define/undef. */
  if (a_kind == LDV_A_INFO && (pp_kind == LDV_PP_EXPAND || pp_kind == LDV_PP_DEFINE))
    {
      for (body_patterns = body->patterns
        ; body_patterns
        ; body_patterns = ldv_list_get_next (body_patterns))
        {
          body_pattern = (ldv_ab_aspect_pattern_ptr) ldv_list_get_data (body_patterns);
          pattern = body_pattern->pattern;

          if (!strcmp (pattern->name, "fprintf"))
            ldv_process_aspect_pattern_fprintf (pattern->params, ldv_cpp_evaluate_aspect_pattern);
        }
    }
}

FILE *
ldv_open_aspect_pattern_fprintf_file_stream (const char *file_name)
{
  char *tmp;
  FILE *aspect_pattern_param_file_stream = NULL;
  struct flock lock;

  /* Often Aspectator will print query results to the same files.
     So, do not "create"" corresponding directories each time. */
  if (access (file_name, F_OK) == -1)
    {
      /* Recursively create directories. */
      tmp = ldv_copy_str (file_name);

      for (char *p = tmp + 1; *p; p++)
        if (*p == '/')
          {
            *p = 0;
            if (mkdir(tmp, 0755) == -1)
              if (errno != EEXIST)
                {
                  LDV_CPP_FATAL_ERROR ("can't create directory '%s'", tmp);
                }

            *p = '/';
          }
    }

  /* Open file for write in append mode to avoid overwriting of already printed
     data. */
  aspect_pattern_param_file_stream = ldv_open_file_stream (file_name, "a+");

  /* Lock file for other writers. */
  memset (&lock, 0, sizeof(lock));
  lock.l_type = F_WRLCK;
  if (fcntl (fileno(aspect_pattern_param_file_stream), F_SETLKW, &lock) == -1)
    {
      LDV_CPP_FATAL_ERROR ("can't lock file");
    }

  return aspect_pattern_param_file_stream;
}

struct ldv_query_results
{
  const char *filename;
  ldv_text_ptr query_results;
};

static hashval_t
ldv_htab_hash_path (const void *p)
{
  return htab_hash_string (((struct ldv_query_results *)p)->filename);
}

static int
ldv_htab_eq_path (const void *p, const void *q)
{
  return strcmp (((const struct ldv_query_results *)p)->filename, (const char *)q) == 0;
}

void
ldv_store_query_results (ldv_aspect_pattern_param_ptr param1, ldv_aspect_pattern_param_ptr param2, ldv_list_ptr pattern_params)
{
  const char *filename = NULL, *format = NULL;
  void **slot = NULL;
  ldv_text_ptr text = NULL;
  ldv_list_ptr param_list = NULL;
  ldv_aspect_pattern_param_ptr param = NULL;
  const char *str = NULL;

  filename = ldv_get_aspect_pattern_str_param (param1);
  format = ldv_get_aspect_pattern_str_param (param2);

  if (!ldv_query_result_htab)
    ldv_query_result_htab = htab_create (127, ldv_htab_hash_path, ldv_htab_eq_path, NULL);

  slot = htab_find_slot_with_hash (ldv_query_result_htab, filename, htab_hash_string (filename), INSERT);

  if (!*slot)
    {
      struct ldv_query_results *v;
      *slot = v = XCNEW (struct ldv_query_results);
      v->filename = ldv_copy_str (filename);
      v->query_results = ldv_create_text ();
    }

  /* filename was copied above and corresponding memory will be freed after
     actual printing of query results. First aspect pattern parameter is not
     needed anymore. */
  ldv_free_aspect_pattern_str_param (param1);

  text = ((struct ldv_query_results *)*slot)->query_results;

  /* Unfortunately there is not built-in feature to printf array of integers and
     strings. So we extract all conversions from a given format manually and
     then use ???built-in fprintf??? for a single string or integer. Here is
     description of conversion syntax:
     http://www.gnu.org/software/libc/manual/html_node/Output-Conversion-Syntax.html
     % [ param-no $] flags width [ . precision ] type conversion
     % [ param-no $] flags width . * [ param-no $] type conversion
  */
  param_list = pattern_params;
  while (*format)
    {
      if (*format == '%')
        {
          /* Get the rest of conversion. */
          format++;
          switch (*format)
            {
            /* Formatted integer should be printed. */
            case 'd':
              if (!param_list)
                {
                  LDV_CPP_FATAL_ERROR ("format '%%d' expects a matching integer argument");
                }

              param = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (param_list);
              str = ldv_itoa (param->integer);
              ldv_puts_text (str, text);
              free ((void *)str);
              param_list = ldv_list_get_next (param_list);

              break;

            /* Formatted string should be printed. */
            case 's':
              if (!param_list)
                {
                  LDV_CPP_FATAL_ERROR ("format '%%s' expects a matching string argument");
                }

              param = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (param_list);

              str = ldv_get_aspect_pattern_str_param (param);

              if (!str)
                {
                  LDV_CPP_FATAL_ERROR ("format '%%s' expects a matching string argument (maybe you need to use '%%d')");
                }

              ldv_puts_text (str, text);
              ldv_free_aspect_pattern_str_param (param);
              param_list = ldv_list_get_next (param_list);

              break;

            case '%':
              ldv_putc_text ('%', text);

              break;

            default:
              LDV_CPP_FATAL_ERROR ("unsupported format '%c'", *format);
            }
        }
      /* Collect text without conversions. */
      else
        ldv_putc_text (*format, text);

      format++;
    }

  ldv_list_delete_all (pattern_params);
  ldv_free_aspect_pattern_str_param (param2);
}

void
ldv_print_macro (ldv_i_macro_ptr i_macro)
{
  ldv_list_ptr i_macro_param_list = NULL;
  ldv_i_macro_func_param_ptr i_macro_param = NULL;

  fprintf (LDV_MATCHED_BY_NAME, "%s", ldv_cpp_get_id_name (i_macro->macro_name));

  for (i_macro_param_list = i_macro->macro_param
    ; i_macro_param_list
    ; i_macro_param_list = ldv_list_get_next (i_macro_param_list))
    {
      i_macro_param = (ldv_i_macro_func_param_ptr) ldv_list_get_data (i_macro_param_list);

      if (i_macro_param_list == i_macro->macro_param)
        fprintf (LDV_MATCHED_BY_NAME, "(");

      if (i_macro_param->name)
        fprintf (LDV_MATCHED_BY_NAME, "%s", ldv_cpp_get_id_name (i_macro_param->name));
      else
        fprintf (LDV_MATCHED_BY_NAME, "...");

      if (ldv_list_get_next (i_macro_param_list))
        fprintf (LDV_MATCHED_BY_NAME, ", ");
      else
        fprintf (LDV_MATCHED_BY_NAME, ")");
    }
}

static int
ldv_print_filename_query_results (void **slot, void *d ATTRIBUTE_UNUSED)
{
  struct ldv_query_results *entry = (struct ldv_query_results *)*slot;
  FILE *file_stream = NULL;

  file_stream = ldv_open_aspect_pattern_fprintf_file_stream (entry->filename);
  fprintf (file_stream, "%s", ldv_get_text (entry->query_results));
  ldv_close_file_stream (file_stream);
  /* We will not need this filename anymore. */
  free ((void *)entry->filename);

  return 1;
}

void
ldv_print_query_results (void)
{
  /* Nothing to print. */
  if (!ldv_query_result_htab)
    return;

  htab_traverse (ldv_query_result_htab, ldv_print_filename_query_results, NULL);
}
