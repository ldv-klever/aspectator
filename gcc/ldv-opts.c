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

#include "ldv-io.h"
#include "ldv-opts.h"


#define LDV_ISENV_SET(env) (getenv (env) ? true : false)

#define LDV_ASPECT_FILE_ENV                         "LDV_ASPECT_FILE"
#define LDV_EXPRESSION_VISUALIZATON_ENV             "LDV_EXPRESSION_VISUALIZATON"
#define LDV_DIR_ORIG_ENV                            "LDV_DIR_ORIG"
#define LDV_DIR_RES_ENV                             "LDV_DIR_RES"
#define LDV_DIR_TEMP_ENV                            "LDV_DIR_TEMP"
#define LDV_INFO_ENV                                "LDV_INFO"
#define LDV_INFO_ALL_ENV                            "LDV_INFO_ALL"
#define LDV_INFO_BISON_ENV                          "LDV_INFO_BISON"
#define LDV_INFO_WEAVE_ENV                          "LDV_INFO_WEAVING"
#define LDV_INFO_IO_ENV                             "LDV_INFO_IO"
#define LDV_INFO_LEX_ENV                            "LDV_INFO_LEX"
#define LDV_INFO_LIST_ENV                           "LDV_INFO_LIST"
#define LDV_INFO_MATCH_ENV                          "LDV_INFO_MATCHING"
#define LDV_INFO_MATCHING_TABLE_ENV                 "LDV_INFO_MATCHING_TABLE"
#define LDV_INFO_MEM_ENV                            "LDV_INFO_MEMORY"
#define LDV_PRINT_SIGNATURE_OF_MATCHED_BY_NAME_ENV  "LDV_PRINT_SIGNATURE_OF_MATCHED_BY_NAME"
#define LDV_STAGE_ENV                               "LDV_STAGE"


/* Define four ldv stages constants. */
enum { LDV_STAGE_PREPROCESSING = 0, LDV_STAGE_FIRST, LDV_STAGE_SECOND, LDV_STAGE_THIRD, LDV_STAGE_FOURTH };


/* Flag that is true if ldv modifications are made and false otherwise. */
static bool ldv;
/* A current ldv stage. */
static int ldv_stage;


int
ldv_get_ldv_stage (void)
{
  return ldv_stage;
}

void
ldv_handle_options (void)
{
  char *stage_str = NULL;
  int stage;

  /* Obtain a ldv aspect file name. */
  ldv_aspect_fname = getenv (LDV_ASPECT_FILE_ENV);

  /* Specify whether a ldv aspecting will be done and do necessary
     initializations. */
  if (ldv_aspect_fname)
    {
      ldv = true;

      ldv_print_info (LDV_INFO_IO, "aspect file \"%s\" is specified by means of environment variable \"%s\"", ldv_aspect_fname, LDV_ASPECT_FILE_ENV);

      /* Obtain a base name for an aspect file name. */
      ldv_aspect_fname_base = lbasename (ldv_aspect_fname);

      /* Check wether a corresponding debuggable infromation is needed. */
      ldv_isinfo = LDV_ISENV_SET (LDV_INFO_ENV);
      ldv_isinfo_bison = LDV_ISENV_SET (LDV_INFO_BISON_ENV);
      ldv_isinfo_weave = LDV_ISENV_SET (LDV_INFO_WEAVE_ENV);
      ldv_isinfo_io = LDV_ISENV_SET (LDV_INFO_IO_ENV);
      ldv_isinfo_lex = LDV_ISENV_SET (LDV_INFO_LEX_ENV);
      ldv_isinfo_list = LDV_ISENV_SET (LDV_INFO_LIST_ENV);
      ldv_isinfo_match = LDV_ISENV_SET (LDV_INFO_MATCH_ENV);
      ldv_isinfo_matching_table = LDV_ISENV_SET (LDV_INFO_MATCHING_TABLE_ENV);
      ldv_isinfo_mem = LDV_ISENV_SET (LDV_INFO_MEM_ENV);

      /* Specify that all debugs are needed. */
      if (LDV_ISENV_SET (LDV_INFO_ALL_ENV))
        {
          ldv_isinfo = true;
          ldv_isinfo_bison = true;
          ldv_isinfo_weave = true;
          ldv_isinfo_io = true;
          ldv_isinfo_lex = true;
          ldv_isinfo_list = true;
          ldv_isinfo_match = true;
          ldv_isinfo_matching_table = true;
          ldv_isinfo_mem = true;
        }

      /* Check wether an expression virtualization is needed. */
      ldv_isexpr_visualization = LDV_ISENV_SET (LDV_EXPRESSION_VISUALIZATON_ENV);

      /* Check wether a signature printing of matched by name entities is needed. */
      ldv_isprint_signature_of_matched_by_name = LDV_ISENV_SET (LDV_PRINT_SIGNATURE_OF_MATCHED_BY_NAME_ENV);

      /* Obtain a current ldv stage. */
      if ((stage_str = getenv (LDV_STAGE_ENV)))
        {
          stage = atoi (stage_str);

          if (stage >= LDV_STAGE_PREPROCESSING && stage <= LDV_STAGE_FOURTH)
            {
              ldv_stage = stage;
              ldv_print_info (LDV_INFO_IO, "stage \"%d\" is specified by means of environment variable \"%s\"", ldv_stage, LDV_STAGE_ENV);
            }
          else
            fatal_error ("ldv stage specified by means of environment variable \"%s\" must be unsigned integer number between \"%d\" and \"%d\"", LDV_STAGE_ENV, LDV_STAGE_PREPROCESSING, LDV_STAGE_FOURTH);
        }
      else
        fatal_error ("specify ldv stage by means of environment variable \"%s\" since ldv aspect file was specified", LDV_STAGE_ENV);

      if ((ldv_dir_res = getenv (LDV_DIR_RES_ENV)))
        ldv_print_info (LDV_INFO_IO, "directory for results \"%s\" is specified by means of environment variable \"%s\"", ldv_dir_res, LDV_DIR_RES_ENV);
      else
        {
          ldv_dir_res = ldv_empty_str ();
          ldv_print_info (LDV_INFO_IO, "current working directory will be used for results");
        }

      if ((ldv_dir_temp = getenv (LDV_DIR_TEMP_ENV)))
        ldv_print_info (LDV_INFO_IO, "directory for temporaries \"%s\" is specified by means of environment variable \"%s\"", ldv_dir_temp, LDV_DIR_TEMP_ENV);
      else
        {
          ldv_dir_temp = ldv_empty_str ();
          ldv_print_info (LDV_INFO_IO, "current working directory will be used for temporaries");
        }

      if (getenv (LDV_DIR_ORIG_ENV))
        {
          ldv_isdir_orig = true;
          ldv_print_info (LDV_INFO_IO, "ldv files will be written to directory where source file is placed");
        }
      else
        ldv_isdir_orig = false;

      /* Open necessary file streams. */
      ldv_open_file_streams ();

      /* Initialize variables sharing information between core and aspectator parts. */
      ldv_decl_for_print_list = NULL;
      ldv_func_defs_for_print = NULL;
    }
  else
    ldv = false;
}

bool
ldv_isldv (void)
{
  return ldv;
}

bool
ldv_isldv_stage_preprocessing (void)
{
  return (ldv_stage == LDV_STAGE_PREPROCESSING);
}

bool
ldv_isldv_stage_first (void)
{
  return (ldv_stage == LDV_STAGE_FIRST);
}

bool
ldv_isldv_stage_second (void)
{
  return (ldv_stage == LDV_STAGE_SECOND);
}

bool
ldv_isldv_stage_third (void)
{
  return (ldv_stage == LDV_STAGE_THIRD);
}

bool
ldv_isldv_stage_fourth (void)
{
  return (ldv_stage == LDV_STAGE_FOURTH);
}
