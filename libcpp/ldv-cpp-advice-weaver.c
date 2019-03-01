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

static int ldv_cpp_evaluate_aspect_pattern (ldv_aspect_pattern_ptr, char **, unsigned int *);
static char *ldv_get_actual_args (void);
static char *ldv_cpp_print_macro_path (ldv_i_macro_ptr);
static char *ldv_cpp_print_macro_signature (ldv_i_macro_ptr i_macro);


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

int
ldv_cpp_evaluate_aspect_pattern (ldv_aspect_pattern_ptr pattern, char **string, unsigned int *integer)
{
  char *text = NULL;
  char *arg_value = NULL;
  bool is_number = false;
  ldv_list_ptr arg_value_list = NULL;
  unsigned int number;
  unsigned int i;

  if ((!strcmp (pattern->name, "macro_signature")) || (!strcmp (pattern->name, "signature")))
    text = ldv_cpp_print_macro_signature (ldv_i_match->i_macro);
  else if (!strcmp (pattern->name, "macro_name"))
    text = ldv_cpp_get_id_name (ldv_i_match->i_macro_aspect->macro_name);
  else if (!strcmp (pattern->name, "arg_val"))
    {
      for (i = 1, arg_value_list = ldv_i_match->i_macro->macro_param_value
      ; arg_value_list
      ; i++, arg_value_list = ldv_list_get_next (arg_value_list))
      {
        arg_value = (char *) ldv_list_get_data (arg_value_list);

        if (i == pattern->arg_numb)
        {
          text = arg_value;
          break;
        }
      }
      if (!text)
        {
          LDV_CPP_FATAL_ERROR ("required parameter has number \"%d\" that exceeds the maximum one \"%d\"", pattern->arg_numb, (i - 1));
        }
    }
  else if (!strcmp (pattern->name, "arg_numb"))
    {
      arg_value_list = ldv_i_match->i_macro->macro_param;
      number = ldv_list_len (arg_value_list);
      is_number = true;
    }
  else if (!strcmp (pattern->name, "actual_args"))
    {
      text = ldv_get_actual_args ();
      if (!text)
          text = "NULL";
    }
  else if (!strcmp (pattern->name, "path"))
    {
      text = ldv_cpp_print_macro_path (ldv_i_match->i_macro);
    }
  else if (!strcmp (pattern->name, "line"))
    {
      text = ldv_cpp_itoa(ldv_i_match->i_macro->line);
    }

  if (text)
    {
      *string = text;
      return 1;
    }
  else if (is_number)
    {
      *integer = number;
      return 1;
    }

  return 0;
}

char *
ldv_get_actual_args (void)
{
  unsigned int i;
  char *i_str;
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

      i_str = ldv_cpp_itoa (i);
      ldv_puts_string (i_str, actual_args);
      free (i_str);

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
ldv_cpp_print_macro_path (ldv_i_macro_ptr i_macro)
{
  char *path = NULL;
  char *occurrence = NULL;

  /* TODO: fix ldv_print_func_path() like here. */
  path = xstrdup (i_macro->file_path);

  /* TODO: remove processing of ".prepared" after all intermediate code will
     reference the original one (there is the same trick in
     gcc/ldv-advice-weaver.c). */
  occurrence = strstr (path, ".prepared");
  if (occurrence)
    *occurrence = '\0';

  return path;
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
  ldv_list_ptr pattern_params = NULL, pattern_params_cur = NULL;
  ldv_aspect_pattern_param_ptr param1 = NULL, param2 = NULL, param_cur = NULL;
  char *text = NULL;
  unsigned int number;
  FILE *file_stream = NULL;

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
            {
              /* First parameter specifies file where information request
                 result to be printed. */
              pattern_params = pattern->params;
              param1 = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params);
              /* Second parameter specifies format string like for standard
                 printf function. */
              pattern_params = ldv_list_get_next (pattern_params);
              param2 = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params);

              /* Evaluate other parameters. */
              pattern_params = ldv_list_get_next (pattern_params);
              /* To keep evaluated values of parameters use parameters
                 themselves since parameter evaluation may lead to either
                 string or integer, and parameters themselves may be either
                 strings or integers. */
              for (pattern_params_cur = pattern_params
                ; pattern_params_cur
                ; pattern_params_cur = ldv_list_get_next (pattern_params_cur))
                {
                  param_cur = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params_cur);

                  /* We are interested here in parameters to be evaluated. */
                  if (param_cur->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
                    {
                     /* Aspect patterns that can be evaluated on the basis of matching
                        information and environment variable values. */
                      if (ldv_cpp_evaluate_aspect_pattern (param_cur->aspect_pattern, &text, &number))
                        {
                          if (text)
                            {
                              param_cur->string = text;
                              /* Forget about evaluated text since it is
                                 used in condition above. */
                              text = NULL;
                            }
                          else
                            param_cur->integer = number;
                        }
                      else
                        {
                          LDV_CPP_FATAL_ERROR ("body aspect pattern \"%s\" wasn't weaved", param_cur->aspect_pattern->name);
                        }
                    }
                }

              file_stream = ldv_open_aspect_pattern_param_file_stream (param1);
              ldv_print_query_result (file_stream, ldv_get_aspect_pattern_value_or_string (param2), pattern_params);
              ldv_close_file_stream (file_stream);
            }
        }
    }
}

const char *
ldv_get_aspect_pattern_value_or_string (ldv_aspect_pattern_param_ptr param)
{
  /* In fact just known environment variable can keep value. This value was
     already calculated in aspect parser. */
  if (param->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
    return param->aspect_pattern->value;
  /* Return string itself. */
  else if (param->kind == LDV_ASPECT_PATTERN_STRING)
    return param->string;

  LDV_CPP_FATAL_ERROR ("can't get aspect pattern value or string");

  return NULL;
}

FILE *
ldv_open_aspect_pattern_param_file_stream (ldv_aspect_pattern_param_ptr param)
{
  FILE *aspect_pattern_param_file_stream = NULL;
  struct flock lock;

  /* Open file for write in append mode to avoid overwriting of already printed
     data. */
  aspect_pattern_param_file_stream = ldv_open_file_stream (ldv_get_aspect_pattern_value_or_string (param), "a+");

  /* Lock file for other writers. */
  memset (&lock, 0, sizeof(lock));
  lock.l_type = F_WRLCK;
  if (fcntl (fileno(aspect_pattern_param_file_stream), F_SETLKW, &lock) == -1)
    {
      LDV_CPP_FATAL_ERROR ("can't lock file");
    }

  return aspect_pattern_param_file_stream;
}

void
ldv_print_query_result (FILE *file_stream, const char *format, ldv_list_ptr pattern_params)
{
  ldv_str_ptr conversion = NULL, text = NULL;
  ldv_aspect_pattern_param_ptr param = NULL;

  if (!file_stream)
    {
      LDV_CPP_FATAL_ERROR ("file stream where query result to be printed isn't specified");
    }

  if (!format)
    {
      LDV_CPP_FATAL_ERROR ("format for query result should be specified as second parameter of each $fprintf aspect body pattern");
    }

  /* Unfortunately there is not built-in feature to printf array of integers and
     strings. So we extract all conversions from a given format manually and
     then use built-in fprintf for a single string or integer. Here is
     description of conversion syntax:
     http://www.gnu.org/software/libc/manual/html_node/Output-Conversion-Syntax.html
     % [ param-no $] flags width [ . precision ] type conversion
     % [ param-no $] flags width . * [ param-no $] type conversion
  */
  while (*format)
    {
      if (*format == '%')
        {
          /* Print previously collected text. */
          if (text)
            {
              fprintf (file_stream, ldv_get_str (text));
              text = NULL;
            }

          conversion = ldv_create_string ();
          ldv_putc_string (*format, conversion);

          /* Get the rest of conversion. */
          format++;
          while (*format)
            {
              ldv_putc_string (*format, conversion);

              /* Formatted integer should be printed. */
              if (*format == 'd')
                {
                  if (!pattern_params)
                    {
                      LDV_CPP_FATAL_ERROR ("format '%%d' expects a matching integer argument");
                    }

                  param = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params);
                  fprintf (file_stream, ldv_get_str (conversion), param->integer);
                  pattern_params = ldv_list_get_next (pattern_params);
                  break;
                }
              /* Formatted string should be printed. */
              else if (*format == 's')
                {
                  if (!pattern_params)
                    {
                      LDV_CPP_FATAL_ERROR ("format '%%s' expects a matching string argument");
                    }

                  param = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern_params);
                  fprintf (file_stream, ldv_get_str (conversion), param->string);
                  pattern_params = ldv_list_get_next (pattern_params);
                  break;
                }
              else if (*format == '%')
                {
                  fprintf (file_stream, "%s", ldv_get_str (conversion));
                  break;
                }

              format++;
            }
        }
      /* Collect text without conversions. Note that we can't output single
         characters since this doesn't take into account multi character
         sequences. */
      else
        {
          if (!text)
            text = ldv_create_string ();

          ldv_putc_string (*format, text);
        }

      format++;
    }

  /* Print the rest of collected text. */
  if (text)
    fprintf (file_stream, ldv_get_str (text));
}

void
ldv_print_macro (ldv_i_macro_ptr i_macro)
{
  ldv_list_ptr i_macro_param_list = NULL;
  ldv_id_ptr i_macro_param = NULL;

  fprintf (LDV_MATCHED_BY_NAME, "%s", ldv_cpp_get_id_name (i_macro->macro_name));

  for (i_macro_param_list = i_macro->macro_param
    ; i_macro_param_list
    ; i_macro_param_list = ldv_list_get_next (i_macro_param_list))
    {
      i_macro_param = (ldv_id_ptr) ldv_list_get_data (i_macro_param_list);

      if (i_macro_param_list == i_macro->macro_param)
        fprintf (LDV_MATCHED_BY_NAME, "(");

      fprintf (LDV_MATCHED_BY_NAME, "%s", ldv_cpp_get_id_name (i_macro_param));

      if (ldv_list_get_next (i_macro_param_list))
        fprintf (LDV_MATCHED_BY_NAME, ", ");
      else
        fprintf (LDV_MATCHED_BY_NAME, ")");
    }
}
