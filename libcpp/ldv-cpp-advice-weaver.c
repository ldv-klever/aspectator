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

  name = ldv_i_match->i_macro->macro_name;
  value = ldv_convert_body_to_macro (ldv_i_match->a_definition->a_body);

  /* Create a macro function definition. */
  if (ldv_i_match->i_macro->macro_kind == LDV_PPS_MACRO_FUNC)
    {
      /* Count a lenght and the number of all macro function definition parameters. */
      for (i_macro_param_list = ldv_i_match->i_macro->macro_param, param_len = 0, param_numb = 0
        ; i_macro_param_list
        ; i_macro_param_list = ldv_list_get_next (i_macro_param_list), ++param_numb)
        param_len += strlen ((const char *) ldv_list_get_data (i_macro_param_list));

      /* A Macro function definition for cpp_define function is 'name(param1,param2,...)=value'. */
      define = XCNEWVEC (char, (strlen (name) + 1 + param_len + param_numb - 1 + 1 + 1 + strlen (value) + 1));

      printed_len = 0;
      printed_len += sprintf (define + printed_len, "%s(", name);

      for (i_macro_param_list = ldv_i_match->i_macro->macro_param
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

  name = ldv_i_match->i_macro->macro_name;

  /* Undef a matched macro definition. It may be either a simple macro
     definition or a macro function definition. */
  cpp_undef (pfile, name);
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
