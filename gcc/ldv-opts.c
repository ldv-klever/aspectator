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

#include "ldv-core.h"
#include "ldv-io.h"
#include "ldv-opts.h"


#define LDV_ISENV_SET(env) (getenv (env) ? true : false)


#define LDV_ARG_SIGN_ENV                            "LDV_ARG_SIGN"
#define LDV_ASPECT_FILE_ENV                         "LDV_ASPECT_FILE"
#define LDV_DIR_RES_ENV                             "LDV_DIR_RES"
#define LDV_DIR_TEMP_ENV                            "LDV_DIR_TEMP"
#define LDV_INFO_ENV                                "LDV_INFO"
#define LDV_INFO_ALL_ENV                            "LDV_INFO_ALL"
#define LDV_INFO_BISON_ENV                          "LDV_INFO_BISON"
#define LDV_INFO_WEAVE_ENV                          "LDV_INFO_WEAVING"
#define LDV_INFO_LEX_ENV                            "LDV_INFO_LEX"
#define LDV_INFO_MATCH_ENV                          "LDV_INFO_MATCHING"
#define LDV_PRINT_SIGNATURE_OF_MATCHED_BY_NAME_ENV  "LDV_PRINT_SIGNATURE_OF_MATCHED_BY_NAME"
#define LDV_STAGE_ENV                               "LDV_STAGE"


/* Define four ldv stages constants. */
enum { LDV_ASPECT_PREPROCESSING = 0, LDV_FILE_PREPARATION, LDV_MACRO_INSTRUMENTATION, LDV_INSTRUMENTATION, LDV_COMPILATION };


/* Flag that is true if ldv modifications are made and false otherwise. */
static bool isldv;
/* One of argument signature extraction algorithms. */
static enum ldv_arg_signs ldv_arg_sign;
/* A current ldv stage. */
static int ldv_stage;


enum ldv_arg_signs
ldv_get_arg_sign_algo (void)
{
  return ldv_arg_sign;
}

int
ldv_get_ldv_stage (void)
{
  return ldv_stage;
}

void
ldv_handle_options (void)
{
  char *stage_str = NULL;
  char *arg_sign = NULL;
  int stage;
  char *output_fname = NULL;

  /* Obtain a ldv aspect file name. */
  ldv_aspect_fname = getenv (LDV_ASPECT_FILE_ENV);

  /* Specify whether a ldv aspecting will be done and do necessary
     initializations. */
  if (ldv_aspect_fname)
    {
      isldv = true;

      ldv_print_info (LDV_INFO, "aspect file \"%s\" is specified by means of environment variable \"%s\"", ldv_aspect_fname, LDV_ASPECT_FILE_ENV);

      /* Obtain a base name for an aspect file name. */
      ldv_aspect_fname_base = lbasename (ldv_aspect_fname);

      /* Check wether a corresponding debuggable infromation is needed. */
      ldv_isinfo = LDV_ISENV_SET (LDV_INFO_ENV);
      ldv_isinfo_bison = LDV_ISENV_SET (LDV_INFO_BISON_ENV);
      ldv_isinfo_weave = LDV_ISENV_SET (LDV_INFO_WEAVE_ENV);
      ldv_isinfo_lex = LDV_ISENV_SET (LDV_INFO_LEX_ENV);
      ldv_isinfo_match = LDV_ISENV_SET (LDV_INFO_MATCH_ENV);

      /* Specify that all debugs are needed. */
      if (LDV_ISENV_SET (LDV_INFO_ALL_ENV))
        {
          ldv_isinfo = true;
          ldv_isinfo_bison = true;
          ldv_isinfo_weave = true;
          ldv_isinfo_lex = true;
          ldv_isinfo_match = true;
        }

      /* Check wether a signature printing of matched by name entities is needed. */
      ldv_isprint_signature_of_matched_by_name = LDV_ISENV_SET (LDV_PRINT_SIGNATURE_OF_MATCHED_BY_NAME_ENV);

      /* Obtain a current ldv stage. */
      if ((stage_str = getenv (LDV_STAGE_ENV)))
        {
          stage = atoi (stage_str);

          if (stage >= LDV_ASPECT_PREPROCESSING && stage <= LDV_COMPILATION)
            {
              ldv_stage = stage;
              ldv_print_info (LDV_INFO, "stage \"%d\" is specified by means of environment variable \"%s\"", ldv_stage, LDV_STAGE_ENV);
            }
          else
            internal_error ("ldv stage specified by means of environment variable \"%s\" must be unsigned integer number between \"%d\" and \"%d\"", LDV_STAGE_ENV, LDV_ASPECT_PREPROCESSING, LDV_COMPILATION);
        }
      else
        internal_error ("specify ldv stage by means of environment variable \"%s\" since ldv aspect file was specified", LDV_STAGE_ENV);

      if ((ldv_dir_res = getenv (LDV_DIR_RES_ENV)))
        ldv_print_info (LDV_INFO, "directory for results \"%s\" is specified by means of environment variable \"%s\"", ldv_dir_res, LDV_DIR_RES_ENV);
      else
        {
          ldv_dir_res = ldv_empty_str ();
          ldv_print_info (LDV_INFO, "current working directory will be used for results");
        }

      if ((ldv_dir_temp = getenv (LDV_DIR_TEMP_ENV)))
        ldv_print_info (LDV_INFO, "directory for temporaries \"%s\" is specified by means of environment variable \"%s\"", ldv_dir_temp, LDV_DIR_TEMP_ENV);
      else
        {
          ldv_dir_temp = ldv_empty_str ();
          ldv_print_info (LDV_INFO, "current working directory will be used for temporaries");
        }

      if ((arg_sign = getenv (LDV_ARG_SIGN_ENV)))
        {
          if (!strcmp (arg_sign, "VARIABLE_ID"))
            {
              ldv_arg_sign = LDV_ARG_SIGN_VARIABLE_ID;
              ldv_print_info (LDV_INFO, "calculate argument signatures as \"variable identifiers\"");
            }
          else if (!strcmp (arg_sign, "SIMPLE_ID"))
            {
              ldv_arg_sign = LDV_ARG_SIGN_SIMPLE_ID;
              ldv_print_info (LDV_INFO, "calculate argument signatures as \"simple identifiers\"");
            }
          else if (!strcmp (arg_sign, "COMPLEX_ID"))
            {
              ldv_arg_sign = LDV_ARG_SIGN_COMPLEX_ID;
              ldv_print_info (LDV_INFO, "calculate argument signatures as \"complex identifiers\"");
            }
          else
            internal_error ("specified by means of environment variable \"%s\" algorithm of argument signatures extraction \"%s\" isn't supported", LDV_ARG_SIGN_ENV, arg_sign);
        }
      else
        {
          ldv_arg_sign = LDV_ARG_SIGN_COMPLEX_ID;
          ldv_print_info (LDV_INFO, "calculate argument signatures as \"complex identifiers\"");
        }

      /* Use output file name specified by means of environment variable instead
         of the one specified by -o. This is required at instrumentation stage
         for which -fsyntax-only is used and gcc removes -o value. */
      if ((output_fname = getenv ("LDV_OUT")))
        ldv_set_output_fname (output_fname);

      /* Open necessary file streams. */
      ldv_open_file_streams ();

      /* Initialize variables sharing information between core and aspectator parts. */
      ldv_decl_for_print_list = NULL;
      ldv_func_defs_for_print = NULL;
    }
  else
    isldv = false;
}

bool
ldv (void)
{
  return isldv;
}

bool
ldv_aspect_preprocessing (void)
{
  return (ldv_stage == LDV_ASPECT_PREPROCESSING);
}

bool
ldv_file_preparation (void)
{
  return (ldv_stage == LDV_FILE_PREPARATION);
}

bool
ldv_macro_instrumentation (void)
{
  return (ldv_stage == LDV_MACRO_INSTRUMENTATION);
}

bool
ldv_instrumentation (void)
{
  return (ldv_stage == LDV_INSTRUMENTATION);
}

bool
ldv_compilation (void)
{
  return (ldv_stage == LDV_COMPILATION);
}
