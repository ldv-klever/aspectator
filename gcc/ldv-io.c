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

/* For file names. */
#include "diagnostic-core.h"
#include "toplev.h"

#include "ldv-aspect-parser.h"
#include "ldv-aspect-types.h"
#include "ldv-core.h"
#include "ldv-cpp-pointcut-matcher.h"
#include "ldv-io.h"
#include "ldv-opts.h"


/* An aspect file name. */
const char *ldv_aspect_fname;
const char *ldv_aspect_fname_base;

/* These flags specify whether a corresponding debuggable information will be
   printed. */
bool ldv_isinfo;
bool ldv_isinfo_bison;
bool ldv_isinfo_weave;
bool ldv_isinfo_io;
bool ldv_isinfo_lex;
bool ldv_isinfo_list;
bool ldv_isinfo_match;
bool ldv_isinfo_matching_table;
bool ldv_isinfo_mem;
/* This flag specifies whether an expression visualization is needed. */
bool ldv_isexpr_visualization;
/* This flag specifies whether a signature printing of matched by name entities
   is needed. */
bool ldv_isprint_signature_of_matched_by_name;

/* Flag specifies whether files obtaining during a ldv work will be
   written to a directory where a source code file is placed or to results and
   temporaries directories. */
bool ldv_isdir_orig;
/* Directories for results and temporaries. */
const char *ldv_dir_res;
const char *ldv_dir_temp;
/* List contains declarations to be printed. */
ldv_list_ptr ldv_decl_for_print_list;
/* Auxiliary function definitions to be printed. */
ldv_text_ptr ldv_func_defs_for_print;

static FILE *ldv_advice_weaved_stream = NULL;
static FILE *ldv_aspect_stream = NULL;
static FILE *ldv_main_stream = NULL;
static FILE *ldv_preprocessed_stream = NULL;


static void ldv_open_advice_weaved_stream (void);
static void ldv_open_aspect_stream (void);
static void ldv_open_main_stream (void);
static void ldv_open_preprocessed_stream (bool);


void
ldv_copy_file (const char *fname, FILE *stream)
{
  FILE *fstream = NULL;
  int c;

  if ((fstream = fopen (fname, "r")) == NULL)
    fatal_error ("can%'t open file \"%s\" for read: %m", fname);

  ldv_print_info (LDV_INFO_IO, "file \"%s\" that content will be copied was successfully opened for write", fname);

  /* Copy a content of a file to a stream. */
  while ((c = ldv_getc (fstream)) != EOF)
    ldv_putc (c, stream);

  fclose (fstream);
}

const char *
ldv_empty_str (void)
{
  char *empty_str = NULL;

  empty_str = XCNEW (char);
  ldv_print_info (LDV_INFO_MEM, "empty string memory was released");

  empty_str[0] = '\0';

  return empty_str;
}

FILE *
ldv_get_advice_weaved_stream (void)
{
  return ldv_advice_weaved_stream;
}

FILE *
ldv_get_aspect_stream (void)
{
  return ldv_aspect_stream;
}

FILE *
ldv_get_main_stream (void)
{
  return ldv_main_stream;
}

FILE *
ldv_get_preprocessed_stream (void)
{
  return ldv_preprocessed_stream;
}

int
ldv_getc (FILE *stream)
{
  int c = getc (stream);

  /* Print information on an obtained character. */
  if (c == EOF)
    ldv_print_info (LDV_INFO_IO, "get char \"end of file\"");
  else
    ldv_print_info (LDV_INFO_IO, "get char \"%c\"", ldv_end_of_line (c));

  return c;
}

const char *
ldv_gets (FILE *stream)
{
  int c;
  ldv_text_ptr line = NULL;

  if ((c = ldv_getc (stream)) == EOF)
    return NULL;
  else
    ldv_ungetc (c, stream);

  line = ldv_create_text ();

  while ((c = ldv_getc (stream)) != EOF)
    {
      if (c == '\n')
        break;

      ldv_putc_text (c, line);
    }

  return ldv_get_text (line);
}

char
ldv_end_of_line (int c)
{
  if (c == '\n')
    return 0;
  else
    return c;
}

void
ldv_make_includes (void)
{
  ldv_adef_ptr adef = NULL;
  ldv_list_ptr adef_list = NULL;
  ldv_i_match_ptr match = NULL;
  ldv_i_file_ptr file = NULL;
  char *include = NULL;
  bool isaround = false;

  ldv_open_preprocessed_stream (true);

  /* There is no advice definitions at all. So nothing will be matched. */
  if (ldv_adef_list == NULL)
    {
      /* Just copy a file itself. This is needed in case when no advices were
         specified. */
      ldv_copy_file (main_input_filename, LDV_PREPROCESSED_STREAM);

      ldv_i_match = NULL;

      return;
    }

  ldv_print_info (LDV_INFO_IO, "begin make includes");

  match = ldv_create_info_match ();
  file = XCNEW (ldv_info_file);

  match->i_kind = LDV_I_FILE;
  match->pp_kind = LDV_PP_FILE;
  match->i_file = file;

  /* Walk through an advice definitions list to find matches. */
  /* First of all apply arround includes i.e. use an advice body text instead of
     an original file text. Note that justa  first around advice is applied and
     other advices on a file are ignored since around was used. */
  for (adef_list = ldv_adef_list; adef_list && !isaround; adef_list = ldv_list_get_next (adef_list))
    {
      adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

      if (ldv_match_cp (adef->a_declaration->c_pointcut, match) && adef->a_declaration->a_kind == LDV_A_AROUND)
        {
          isaround = true;

          /* Count advice weavings. */
          ++(adef->use_counter);

          ldv_print_info (LDV_INFO_MATCH, "match file \"%s\"", match->i_file->name);

          include = ldv_copy_str (ldv_get_body_text (adef->a_body));
          include = ldv_trunkate_braces (include);

          ldv_puts (include, LDV_PREPROCESSED_STREAM);
          ldv_print_info (LDV_INFO_WEAVE, "include was weaved for around advice");
        }
    }

  /* Other file advices are applied just when an arround advice wasn't used. */
  if (!isaround)
    {
      /* Copy all before includes. */
      for (adef_list = ldv_adef_list; adef_list; adef_list = ldv_list_get_next (adef_list))
        {
          adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

          if (ldv_match_cp (adef->a_declaration->c_pointcut, match) && adef->a_declaration->a_kind == LDV_A_BEFORE)
            {
              /* Count advice weavings. */
              ++(adef->use_counter);

              ldv_print_info (LDV_INFO_MATCH, "match file \"%s\"", match->i_file->name);

              include = ldv_copy_str (ldv_get_body_text (adef->a_body));
              include = ldv_trunkate_braces (include);

              ldv_puts (include, LDV_PREPROCESSED_STREAM);
              ldv_print_info (LDV_INFO_WEAVE, "include was weaved for before advice");
            }
        }

      /* Then copy a file itself. */
      ldv_copy_file (main_input_filename, LDV_PREPROCESSED_STREAM);

      /* Then copy all after includes. */
      for (adef_list = ldv_adef_list; adef_list; adef_list = ldv_list_get_next (adef_list))
        {
          adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

          if (ldv_match_cp (adef->a_declaration->c_pointcut, match) && adef->a_declaration->a_kind == LDV_A_AFTER)
            {
              /* Count advice weavings. */
              ++(adef->use_counter);

              ldv_print_info (LDV_INFO_MATCH, "match file \"%s\"", match->i_file->name);

              include = ldv_copy_str (ldv_get_body_text (adef->a_body));
              include = ldv_trunkate_braces (include);

              ldv_puts (include, LDV_PREPROCESSED_STREAM);
              ldv_print_info (LDV_INFO_WEAVE, "include was weaved for after advice");
            }
        }
    }

  fclose (LDV_PREPROCESSED_STREAM);
  ldv_print_info (LDV_INFO_IO, "finish make includes");
}

void
ldv_open_advice_weaved_stream (void)
{
  char *advice_weaved_fname = NULL;
  const char *fname = NULL;
  const char *dname = NULL;

  if (ldv_isdir_orig)
    {
      fname = main_input_filename;

      /* Separate an absolute path to a file with the relative one. */
      if (fname && fname[0] == '/')
        {
          /* An advice weaved file name is 'file name + LDV_ADVICE_WEAVED_EXTENSION'. */
          advice_weaved_fname = XCNEWVEC (char, (strlen (fname) + strlen (LDV_ADVICE_WEAVED_EXTENSION) + 2));

          sprintf (advice_weaved_fname, "%s%s", fname, LDV_ADVICE_WEAVED_EXTENSION);
        }
      else
        {
          dname = get_src_pwd ();

          /* An advice weaved file name is 'directory name + / + file name + LDV_ADVICE_WEAVED_EXTENSION'. */
          advice_weaved_fname = XCNEWVEC (char, (strlen (dname) + strlen (fname) + strlen (LDV_ADVICE_WEAVED_EXTENSION) + 2));

          sprintf (advice_weaved_fname, "%s/%s%s", dname, fname, LDV_ADVICE_WEAVED_EXTENSION);
        }

      if ((ldv_advice_weaved_stream = fopen (advice_weaved_fname, "w")) == NULL)
        fatal_error ("can%'t open file \"%s\" for write: %m", advice_weaved_fname);

      ldv_print_info (LDV_INFO_IO, "advice weaved file \"%s\" was successfully opened for write", advice_weaved_fname);
    }
  else
    {
      fname = lbasename (main_input_filename);

      /* An advice weaved file name is 'results directory + file base name + LDV_ADVICE_WEAVED_EXTENSION'. */
      advice_weaved_fname = XCNEWVEC (char, (strlen (ldv_dir_res) + strlen (fname) + strlen (LDV_ADVICE_WEAVED_EXTENSION) + 1));

      sprintf (advice_weaved_fname, "%s%s%s", ldv_dir_res, fname, LDV_ADVICE_WEAVED_EXTENSION);

      if ((ldv_advice_weaved_stream = fopen (advice_weaved_fname, "w")) == NULL)
        fatal_error ("can%'t open file \"%s\" for write: %m", advice_weaved_fname);

      ldv_print_info (LDV_INFO_IO, "advice weaved file \"%s\" was successfully opened for write", advice_weaved_fname);
    }
}

void
ldv_open_aspect_stream (void)
{
  if ((ldv_aspect_stream = fopen (ldv_aspect_fname, "r")) == NULL)
    fatal_error ("can%'t open file \"%s\" for read: %m", ldv_aspect_fname);

  ldv_print_info (LDV_INFO_IO, "aspect file \"%s\" was successfully opened for read", ldv_aspect_fname);
}

void
ldv_open_main_stream (void)
{
  if ((ldv_main_stream = fopen (main_input_filename, "r")) == NULL)
    fatal_error ("can%'t open file \"%s\" for read: %m", main_input_filename);

  ldv_print_info (LDV_INFO_IO, "main file \"%s\" was successfully opened for read", main_input_filename);
}

static void
ldv_open_preprocessed_stream (bool iswrite)
{
  char *preprocessed_fname = NULL;
  const char *fname = NULL;
  const char *dname = NULL;

  if (ldv_isdir_orig)
    {
      fname = main_input_filename;

      /* Separate an absolute path to a file with the relative one. */
      if (fname && fname[0] == '/')
        {
          /* A preprocessed file name is 'file name + LDV_PREPROCESSED_EXTENSION'. */
          preprocessed_fname = XCNEWVEC (char, (strlen (fname) + strlen (LDV_PREPROCESSED_EXTENSION) + 2));

          sprintf (preprocessed_fname, "%s%s", fname, LDV_PREPROCESSED_EXTENSION);
        }
      else
        {
          dname = get_src_pwd ();

          /* A preprocessed file name is 'directory name + / + file name + LDV_PREPROCESSED_EXTENSION'. */
          preprocessed_fname = XCNEWVEC (char, (strlen (dname) + strlen (fname) + strlen (LDV_PREPROCESSED_EXTENSION) + 2));

          sprintf (preprocessed_fname, "%s/%s%s", dname, fname, LDV_PREPROCESSED_EXTENSION);
        }
    }
  else
    {
      fname = lbasename (main_input_filename);

      /* A preprocessed file name is 'results directory + file name base + LDV_PREPROCESSED_EXTENSION'. */
      preprocessed_fname = XCNEWVEC (char, (strlen (ldv_dir_res) + strlen (fname) + strlen (LDV_PREPROCESSED_EXTENSION) + 1));

      sprintf (preprocessed_fname, "%s%s%s", ldv_dir_res, fname, LDV_PREPROCESSED_EXTENSION);
    }

  if (iswrite)
    {
      if ((ldv_preprocessed_stream = fopen (preprocessed_fname, "w+")) == NULL)
        fatal_error ("can%'t open file \"%s\" for read and write: %m", preprocessed_fname);

      ldv_print_info (LDV_INFO_IO, "Preprocessed file \"%s\" was successfully opened for read and write", preprocessed_fname);
    }
  else
    {
      if ((ldv_preprocessed_stream = fopen (preprocessed_fname, "r")) == NULL)
        fatal_error ("can%'t open file \"%s\" for read: %m", preprocessed_fname);

      ldv_print_info (LDV_INFO_IO, "Preprocessed file \"%s\" was successfully opened for read", preprocessed_fname);
    }
}

void
ldv_open_file_streams (void)
{
  /* Open file streams in depend on a given ldv stage. */
  if (ldv_isldv_stage_preprocessing ())
    /* Preprocess a given aspect file as well as the standard C file. */;
  else if (ldv_isldv_stage_first ())
    ldv_open_aspect_stream ();
  else if (ldv_isldv_stage_second () || ldv_isldv_stage_third () || ldv_isldv_stage_fourth ())
    {
      /* Open an aspect stream for read. */
      ldv_open_aspect_stream ();

      if (ldv_isldv_stage_third ())
        {
          ldv_open_main_stream ();
          ldv_open_advice_weaved_stream ();
        }
    }
  else
    fatal_error ("ldv stage '%d' wasn't processed", ldv_get_ldv_stage ());
}

void
ldv_print_info (const char *info_kind, const char *format, ...)
{
  va_list ap;

  if (!ldv_isinfo && !strcmp (info_kind, LDV_INFO))
    return;
  else if (!ldv_isinfo_bison && !strcmp (info_kind, LDV_INFO_BISON))
    return;
  else if (!ldv_isinfo_weave && !strcmp (info_kind, LDV_INFO_WEAVE))
    return;
  else if (!ldv_isinfo_io && !strcmp (info_kind, LDV_INFO_IO))
    return;
  else if (!ldv_isinfo_lex && !strcmp (info_kind, LDV_INFO_LEX))
    return;
  else if (!ldv_isinfo_list && !strcmp (info_kind, LDV_INFO_LIST))
    return;
  else if (!ldv_isinfo_match && !strcmp (info_kind, LDV_INFO_MATCH))
    return;
  else if (!ldv_isinfo_matching_table && !strcmp (info_kind, LDV_INFO_MATCHING_TABLE))
    return;
  else if (!ldv_isinfo_mem && !strcmp (info_kind, LDV_INFO_MEM))
    return;

  va_start (ap, format);
  fprintf (LDV_INFO_STREAM, "%s: ", info_kind);
  vfprintf (LDV_INFO_STREAM, format, ap);
  va_end (ap);

  fputc ('\n', LDV_INFO_STREAM);
}

void
ldv_print_to_awfile (void)
{
  const char *line = NULL, *quote_left = NULL, *quote_right = NULL, *c = NULL;
  int line_numb = 0;
  ldv_file_ptr file = NULL;
  ldv_decl_for_print_ptr decl_for_print = NULL;
  ldv_list_ptr decl_for_print_list = NULL;
  bool isdecl_for_file = false;
  bool ispreprocessor_directive = false;
  bool isline_beginning_printed;
  int symbol_printed_numb;

  /* Add needed declarations untill a line remains in a main file. */
  while ((line = ldv_gets (LDV_MAIN_STREAM)))
    {
      /* Specify that an obtained line beginning wasn't printed yet. */
      isline_beginning_printed = false;
      symbol_printed_numb = 0;

      /* Peform special actions in case when a special preprocessor directive
         switch a source code file and a line in it. The special directive has
         the following form: '# (\d+) "([^"]+)"...' where $2 is an absolute
         file path, $1 is a line in this file (a next line from a scanned file
         has such a number). */
      if (line[0] == '#')
        {
          ispreprocessor_directive = true;

          line_numb = atoi (line + 1);

          /* If the format of a file name in quotes is correct. */
          if ((quote_left = strchr (line, '"')) && (quote_right = strrchr (line, '"')))
            {
              file = ldv_create_file ();

              for (c = quote_left + 1; c != quote_right; c++)
                ldv_putc_file (*c, file);
            }

          isdecl_for_file = false;

          for (decl_for_print_list = ldv_decl_for_print_list, isdecl_for_file = false
            ; decl_for_print_list
            ; decl_for_print_list = ldv_list_get_next (decl_for_print_list))
            {
              decl_for_print = (ldv_decl_for_print_ptr) ldv_list_get_data (decl_for_print_list);

              if (!strcmp (ldv_get_file_name (file), decl_for_print->file))
                {
                  isdecl_for_file = true;
                  break;
                }
            }
        }
      else
        ispreprocessor_directive = false;

      if (!ispreprocessor_directive && isdecl_for_file)
        {
          for (decl_for_print_list = ldv_decl_for_print_list
            ; decl_for_print_list
            ; decl_for_print_list = ldv_list_get_next (decl_for_print_list))
            {
              decl_for_print = (ldv_decl_for_print_ptr) ldv_list_get_data (decl_for_print_list);

              /* Peform following actions if file names are coinside. */
              if (!strcmp (ldv_get_file_name (file), decl_for_print->file))
                {
                  /* Peform following actions if line numbers are coinside. */
                  if (line_numb == decl_for_print->line)
                    {
                      /* Print the beginning of a source line and an addtional
                         string. */
                      if (!isline_beginning_printed)
                        {
                          symbol_printed_numb = ldv_putsn (line, LDV_ADVICE_WEAVED_STREAM, decl_for_print->column);
                          isline_beginning_printed = true;
                        }

                      ldv_puts (ldv_get_text (decl_for_print->decl), LDV_ADVICE_WEAVED_STREAM);
                    }
                }
            }
        }

      /* Print the rest of a source code line if its beginning was already
         printed. */
      if (isline_beginning_printed)
        ldv_puts (line + symbol_printed_numb, LDV_ADVICE_WEAVED_STREAM);
      else
        ldv_puts (line, LDV_ADVICE_WEAVED_STREAM);

      /* Put the end of a line to the end. */
      ldv_putc ('\n', LDV_ADVICE_WEAVED_STREAM);

      /* Enlarge a line number in a currently processed file. Special
         preprocessor directives are skipped. */
      if (!ispreprocessor_directive)
        line_numb++;
    }

  /* Add auxiliary function definitions to the end of an advice weaved file if
     it's needed. */
  if (ldv_func_defs_for_print)
    ldv_puts (ldv_get_text (ldv_func_defs_for_print), LDV_ADVICE_WEAVED_STREAM);
}

void
ldv_putc (int c, FILE *stream)
{
  /* Finish work if can not write a given character to a given stream. */
  if (putc (c, stream) != c)
    fatal_error ("character \"%c\" wasn't put to stream", c);

  ldv_print_info (LDV_INFO_IO, "put character \"%c\"", ldv_end_of_line (c));
}

void
ldv_puts (const char *str, FILE *stream)
{
  const char *c = NULL;

  if (!str)
    fatal_error ("symbol pointer wasn't initialized");

  /* Put each symbol of a string to a stream. */
  for (c = str; c && *c; c++)
    ldv_putc (*c, stream);
}

int
ldv_putsn (const char *str, FILE *stream, int n)
{
  const char *c = NULL;
  int i;

  if (!str)
    fatal_error ("symbol pointer wasn't initialized");

  /* Put each symbol of a string to a stream. */
  for (c = str, i = 0; c && *c && i < n; c++, i++)
    ldv_putc (*c, stream);

  return i;
}

void
ldv_set_nomatch (void)
{
  /* Specify that nothing was matched at the beginning. */
  ldv_i_match = NULL;
}

void
ldv_ungetc (int c, FILE *stream)
{
  /* Finish work if can not back a character to a stream. */
  if (ungetc (c, stream) != c)
    fatal_error ("character \"%c\" wasn't back to stream", c);

  ldv_print_info (LDV_INFO_IO, "unget character \"%c\"", ldv_end_of_line (c));
}
