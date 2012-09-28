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


void
ldv_cpp_define (struct cpp_reader *pfile)
{
  char *define = NULL;
  const char *name = NULL;
  ldv_list_ptr i_macro_param_list = NULL;
  unsigned int param_numb;
  unsigned int param_len;
  char *value = NULL;
  unsigned int printed_len;

  name = ldv_i_match->i_macro_aspect->macro_name;
  value = ldv_convert_body_to_macro (ldv_i_match->a_definition->a_body);

  /* Create a macro function definition. */
  if (ldv_i_match->i_macro_aspect->macro_kind == LDV_PPS_MACRO_FUNC)
    {
      /* Count a lenght and the number of all macro function definition parameters. */
      for (i_macro_param_list = ldv_i_match->i_macro_aspect->macro_param, param_len = 0, param_numb = 0
        ; i_macro_param_list
        ; i_macro_param_list = ldv_list_get_next (i_macro_param_list), ++param_numb)
        param_len += strlen ((const char *) ldv_list_get_data (i_macro_param_list));

      /* A Macro function definition for cpp_define function is 'name(param1,param2,...)=value'. */
      define = XCNEWVEC (char, (strlen (name) + 1 + param_len + param_numb - 1 + 1 + 1 + strlen (value) + 1));

      printed_len = 0;
      printed_len += sprintf (define + printed_len, "%s(", name);

      for (i_macro_param_list = ldv_i_match->i_macro_aspect->macro_param
        ; i_macro_param_list
        ; i_macro_param_list = ldv_list_get_next (i_macro_param_list))
        {
          printed_len += sprintf (define + printed_len, "%s", (char *)ldv_list_get_data (i_macro_param_list));

          if (ldv_list_get_next (i_macro_param_list))
            printed_len += sprintf (define + printed_len, ",");
        }

      printed_len += sprintf (define + printed_len, ")=%s", value);

      cpp_define (pfile, define);

      fprintf (stderr, "Create cpp definition \"%s\".\n", define);
    }
  /* Create a macro definition. */
  else
    {
      /* A Macro definition for cpp_define function is 'name=value'. */
      define = XCNEWVEC (char, (strlen (name) + 1 + strlen (value) + 1));

      sprintf (define, "%s=%s", name, value);

      cpp_define (pfile, define);

      fprintf (stderr, "Create cpp definition \"%s\".\n", define);
    }
}

void
ldv_cpp_undef (struct cpp_reader *pfile)
{
  const char *name = NULL;

  name = ldv_i_match->i_macro_aspect->macro_name;

  /* Undef a matched macro definition. It may be either a simple macro
     definition or a macro function definition. */
  cpp_undef (pfile, name);
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
  ldv_aspect_pattern_param_ptr param1 = NULL, param2 = NULL;
  const char *file_name = NULL;
  const char *text = NULL;
  ldv_list_ptr arg_value_list = NULL;
  const char *arg_value = NULL;
  unsigned int i;

  a_kind = ldv_i_match->a_definition->a_declaration->a_kind;
  pp_kind = ldv_i_match->p_pointcut->pp_kind;
  body = ldv_i_match->a_definition->a_body;

  /* Now here is supported just printing information on macro expansions.
   * Weaving macrodefinitions are implemented in ldv_cpp_define/undef. */
  if (a_kind == LDV_A_INFO && pp_kind == LDV_PP_EXPAND)
    {
      for (body_patterns = body->patterns
        ; body_patterns
        ; body_patterns = ldv_list_get_next (body_patterns))
        {
          body_pattern = (ldv_ab_aspect_pattern_ptr) ldv_list_get_data (body_patterns);
          pattern = body_pattern->pattern;

          /* TODO. Here is implemented very simple processing of aspect body
           * patterns. But it would be great to merge it with the one from
           * ldv-advice-weaver.c. */
          if (!strcmp (pattern->name, "fprintf"))
            {
              /* Output information specified by the 2d parameter to the
               * file specified by the 1st one. */
              param1 = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (pattern->params);
              param2 = (ldv_aspect_pattern_param_ptr) ldv_list_get_data (ldv_list_get_next (pattern->params));

              if (param1->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN)
                file_name = param1->aspect_pattern->value;
              else
                {
                  LDV_CPP_FATAL_ERROR ("Can't get a file name where information to be printed");
                }

              if (param2->kind == LDV_ASPECT_PATTERN_ASPECT_PATTERN && !strcmp (param2->aspect_pattern->name, "arg_val"))
                {
                  for (i = 1, arg_value_list = ldv_i_match->i_macro->macro_param_value
                    ; arg_value_list
                    ; i++, arg_value_list = ldv_list_get_next (arg_value_list))
                    {
                      arg_value = (char *) ldv_list_get_data (arg_value_list);

                      if (i == param2->aspect_pattern->arg_numb)
                      {
                        text = arg_value;
                        break;
                      }
                    }

                  if (!text)
                    {
                      LDV_CPP_FATAL_ERROR ("required parameter has number \"%d\" that exceeds the maximum one \"%d\"", param2->aspect_pattern->arg_numb, (i - 1));
                    }
                }
              else
                {
                  LDV_CPP_FATAL_ERROR ("Can't get information to be printed");
                }

              fprintf (ldv_open_file_stream (file_name, "a+"), "%s\n", text);
            }
        }
    }
}

void
ldv_print_macro (ldv_i_macro_ptr i_macro)
{
  char *i_macro_param = NULL;
  ldv_list_ptr i_macro_param_list = NULL;

  fprintf (LDV_MATCHED_BY_NAME, "%s", i_macro->macro_name);

  for (i_macro_param_list = i_macro->macro_param
    ; i_macro_param_list
    ; i_macro_param_list = ldv_list_get_next (i_macro_param_list))
    {
      i_macro_param = (char *) ldv_list_get_data (i_macro_param_list);

      if (i_macro_param_list == i_macro->macro_param)
        fprintf (LDV_MATCHED_BY_NAME, "(");

      fprintf (LDV_MATCHED_BY_NAME, "%s", i_macro_param);

      if (ldv_list_get_next (i_macro_param_list))
        fprintf (LDV_MATCHED_BY_NAME, ", ");
      else
        fprintf (LDV_MATCHED_BY_NAME, ")");
    }
}
