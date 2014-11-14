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


/* Output file name. */
const char *ldv_output_fname = NULL;

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

/* Directories for results and temporaries. */
const char *ldv_dir_res;
const char *ldv_dir_temp;
/* List contains declarations to be printed. */
ldv_list_ptr ldv_decl_for_print_list;
/* Auxiliary function definitions to be printed. */
ldv_text_ptr ldv_func_defs_for_print;

static FILE *ldv_instrumented_file_stream = NULL;
static FILE *ldv_aspect_stream = NULL;
static FILE *ldv_main_stream = NULL;
static FILE *ldv_file_prepared_stream = NULL;


static void ldv_open_instrumented_file_stream (void);
static void ldv_open_aspect_stream (void);
static void ldv_open_main_stream (void);
static void ldv_open_file_prepared_stream (void);


void
ldv_create_files (void)
{
  ldv_adef_ptr adef = NULL;
  ldv_list_ptr adef_list = NULL;
  char *fname = NULL;
  char *fcontent = NULL;
  ldv_text_ptr body_with_patterns = NULL;
  const char *body_c = NULL;
  int body_p;
  ldv_list_ptr body_patterns = NULL;
  ldv_ab_aspect_pattern_ptr body_pattern = NULL;
  ldv_aspect_pattern_ptr pattern = NULL;
  FILE *fstream = NULL;

  for (adef_list = ldv_adef_list; adef_list; adef_list = ldv_list_get_next (adef_list))
    {
      adef = (ldv_adef_ptr) ldv_list_get_data (adef_list);

      if (adef->a_declaration->a_kind == LDV_A_NEW)
        {
          /* Count advice weavings. */
          ++(adef->use_counter);

          fname = ldv_get_str (adef->a_declaration->c_pointcut->p_pointcut->pp_signature->pps_file->file_name->file_name);
          fcontent = ldv_copy_str (ldv_get_body_text (adef->a_body));
          fcontent = ldv_truncate_braces (fcontent);

          /* Instance different body patterns if it's needed. */
          body_with_patterns = ldv_create_text ();

          /* Walk through a body text to find places body patterns (just $sign,
           * $env and isempty_sign_foreach_sign) to be printed. */
          for (body_c = fcontent, body_p = 1; *body_c; body_c++, body_p++)
            {
              /* Copy a non pattern text of a body. */
              ldv_putc_text (*body_c, body_with_patterns);

              for (body_patterns = adef->a_body->patterns
                ; body_patterns
                ; body_patterns = ldv_list_get_next (body_patterns))
                {
                  body_pattern = (ldv_ab_aspect_pattern_ptr) ldv_list_get_data (body_patterns);

                  if (body_pattern->place == body_p)
                    {
                      pattern = body_pattern->pattern;

                      if (!strcmp (pattern->name, "env"))
                        ldv_puts_text (pattern->value, body_with_patterns);
                      else
                        {
                          LDV_FATAL_ERROR ("body aspect pattern \"%s\" wasn't weaved", pattern->name);
                        }

                      ldv_print_info (LDV_INFO_WEAVE, "weave body aspect pattern \"%s\"", pattern->name);

                      break;
                    }
                }
            }

          /* In truncating braces this pointer was moved ahead exactly
             by 1. */
          free (fcontent - 1);

          if ((fstream = fopen (fname, "w")) == NULL)
            {
              LDV_FATAL_ERROR ("can%'t open file \"%s\" for write: %m", fname);
            }

          ldv_print_info (LDV_INFO_IO, "file \"%s\" was created successfully", fname);

          if (fputs (ldv_get_text (body_with_patterns), fstream) == EOF)
            {
              LDV_FATAL_ERROR ("can%'t write to file \"%s\": %m", fname);
            }

          ldv_free_text (body_with_patterns);

          fclose (fstream);

          ldv_print_info (LDV_INFO_IO, "file \"%s\" was written successfully", fname);
        }
    }

  ldv_print_info (LDV_INFO_IO, "finish files creature");
}

void
ldv_copy_file (const char *fname, FILE *stream)
{
  FILE *fstream = NULL;
  int c;

  if ((fstream = fopen (fname, "r")) == NULL)
    {
      LDV_FATAL_ERROR ("can%'t open file \"%s\" for read: %m", fname);
    }

  ldv_print_info (LDV_INFO_IO, "file \"%s\" that content will be copied was successfully opened for write", fname);

  /* Refer to the original file (fix #5431). */
  ldv_puts ("#line 1 \"", stream);
  ldv_puts (fname, stream);
  ldv_puts ("\"\n", stream);

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
ldv_get_instrumented_file_stream (void)
{
  return ldv_instrumented_file_stream;
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
ldv_get_file_prepared_stream (void)
{
  return ldv_file_prepared_stream;
}

int
ldv_getc (FILE *stream)
{
  int c = getc (stream);

#ifdef LDV_CHAR_DEBUG

  /* Print information on an obtained character. */
  if (c == EOF)
    ldv_print_info (LDV_INFO_IO, "get char \"end of file\"");
  else
    ldv_print_info (LDV_INFO_IO, "get char \"%c\"", ldv_end_of_line (c));

#endif /* LDV_CHAR_DEBUG */

  return c;
}

char *
ldv_gets (FILE *stream)
{
  int c;
  ldv_text_ptr line = NULL;
  char *str = NULL;

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

  str = ldv_get_text (line);

  free (line->text);
  free (line);

  return str;
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
  unsigned int line_cur = 0;
  char *line_cur_str = NULL;

  ldv_open_file_prepared_stream ();

  /* There is no advice definitions at all. So nothing will be matched. */
  if ((ldv_adef_list == NULL) || getenv ("LDV_DIAGNOSTICS"))
    {
      /* Just copy a file itself. This is needed in case when no advices were
         specified. */
      ldv_copy_file (main_input_filename, LDV_FILE_PREPARED_STREAM);

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
          include = ldv_truncate_braces (include);

          ldv_puts (include, LDV_FILE_PREPARED_STREAM);
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

              /* TODO: let's keep body without braces? */
              include = ldv_copy_str (ldv_get_body_text (adef->a_body));
              include = ldv_truncate_braces (include);

              ldv_puts (include, LDV_FILE_PREPARED_STREAM);

              /* In truncating braces this pointer was moved ahead
                 exactly by 1. */
              free (include - 1);

              ldv_print_info (LDV_INFO_WEAVE, "include was weaved for before advice");
            }
        }

      /* Remember current line to print line directive below. */
      if (fflush (LDV_FILE_PREPARED_STREAM))
      {
        LDV_FATAL_ERROR ("can%'t flush to file \"%s\": %m", ldv_output_fname);
      }
      line_cur = ldv_get_current_line_number(ldv_output_fname);

      /* Then copy a file itself. */
      ldv_copy_file (main_input_filename, LDV_FILE_PREPARED_STREAM);

      /* Refer to the prepared file (fix #5431). */
      line_cur_str = ldv_itoa (line_cur);
      ldv_puts ("\n#line ", LDV_FILE_PREPARED_STREAM);
      ldv_puts (line_cur_str, LDV_FILE_PREPARED_STREAM);
      ldv_puts (" \"", LDV_FILE_PREPARED_STREAM);
      ldv_puts (ldv_output_fname, LDV_FILE_PREPARED_STREAM);
      ldv_puts ("\"\n", LDV_FILE_PREPARED_STREAM);
      free (line_cur_str);

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
              include = ldv_truncate_braces (include);

              ldv_puts (include, LDV_FILE_PREPARED_STREAM);
              ldv_print_info (LDV_INFO_WEAVE, "include was weaved for after advice");
            }
        }
    }

  fclose (LDV_FILE_PREPARED_STREAM);

  free (file);
  ldv_free_info_match (match);

  ldv_print_info (LDV_INFO_IO, "finish make includes");
}

void
ldv_open_instrumented_file_stream (void)
{
  if ((ldv_instrumented_file_stream = fopen (ldv_output_fname, "w")) == NULL)
    {
      LDV_FATAL_ERROR ("can%'t open file \"%s\" for write: %m", ldv_output_fname);
    }

  ldv_print_info (LDV_INFO_IO, "Instrumented file \"%s\" was successfully opened for write", ldv_output_fname);
}

void
ldv_open_aspect_stream (void)
{
  if ((ldv_aspect_stream = fopen (ldv_aspect_fname, "r")) == NULL)
    {
      LDV_FATAL_ERROR ("can%'t open file \"%s\" for read: %m", ldv_aspect_fname);
    }

  ldv_print_info (LDV_INFO_IO, "aspect file \"%s\" was successfully opened for read", ldv_aspect_fname);
}

void
ldv_open_main_stream (void)
{
  if ((ldv_main_stream = fopen (main_input_filename, "r")) == NULL)
    {
      LDV_FATAL_ERROR ("can%'t open file \"%s\" for read: %m", main_input_filename);
    }

  ldv_print_info (LDV_INFO_IO, "main file \"%s\" was successfully opened for read", main_input_filename);
}

static void
ldv_open_file_prepared_stream (void)
{
  if ((ldv_file_prepared_stream = fopen (ldv_output_fname, "w")) == NULL)
    {
      LDV_FATAL_ERROR ("can%'t open file \"%s\" for write: %m", ldv_output_fname);
    }

  ldv_print_info (LDV_INFO_IO, "Prepared file \"%s\" was successfully opened for write", ldv_output_fname);
}

void
ldv_open_file_streams (void)
{
  /* Open file streams in depend on a given ldv stage. */
  if (ldv_aspect_preprocessing ())
    /* Preprocess a given aspect file as well as the standard C file. */;
  else if (ldv_file_preparation ())
    ldv_open_aspect_stream ();
  else if (ldv_macro_instrumentation () || ldv_instrumentation () || ldv_compilation ())
    {
      /* Open an aspect stream for read. */
      ldv_open_aspect_stream ();

      if (ldv_instrumentation ())
        {
          ldv_open_main_stream ();
          ldv_open_instrumented_file_stream ();
        }
    }
  else
    {
      LDV_FATAL_ERROR ("ldv stage '%d' wasn't processed", ldv_get_ldv_stage ());
    }
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
  char *line = NULL, *quote_left = NULL, *quote_right = NULL, *c = NULL;
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
              /* Free previously stored file. */
              if (file)
                ldv_free_file (file);

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
                          symbol_printed_numb = ldv_putsn (line, LDV_INSTRUMENTED_FILE_STREAM, decl_for_print->column);
                          isline_beginning_printed = true;
                        }

                      ldv_puts (ldv_get_text (decl_for_print->decl), LDV_INSTRUMENTED_FILE_STREAM);
                    }
                }
            }
        }

      /* Print the rest of a source code line if its beginning was already
         printed. */
      if (isline_beginning_printed)
        ldv_puts (line + symbol_printed_numb, LDV_INSTRUMENTED_FILE_STREAM);
      else
        ldv_puts (line, LDV_INSTRUMENTED_FILE_STREAM);

      free (line);

      /* Put the end of a line to the end. */
      ldv_putc ('\n', LDV_INSTRUMENTED_FILE_STREAM);

      /* Enlarge a line number in a currently processed file. Special
         preprocessor directives are skipped. */
      if (!ispreprocessor_directive)
        line_numb++;
    }

  if (file)
    ldv_free_file (file);

  /* Add auxiliary function definitions to the end of an advice weaved file if
     it's needed. */
  if (ldv_func_defs_for_print)
    ldv_puts (ldv_get_text (ldv_func_defs_for_print), LDV_INSTRUMENTED_FILE_STREAM);
}

void
ldv_putc (int c, FILE *stream)
{
  /* Finish work if can not write a given character to a given stream. */
  if (putc (c, stream) != (unsigned char) c)
    {
      LDV_FATAL_ERROR ("character \"%c\" wasn't put to stream", c);
    }

#ifdef LDV_CHAR_DEBUG

  ldv_print_info (LDV_INFO_IO, "put character \"%c\"", ldv_end_of_line (c));

#endif /* LDV_CHAR_DEBUG */
}

void
ldv_puts (const char *str, FILE *stream)
{
  const char *c = NULL;

  if (!str)
    {
      LDV_FATAL_ERROR ("symbol pointer wasn't initialized");
    }

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
    {
      LDV_FATAL_ERROR ("symbol pointer wasn't initialized");
    }

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
ldv_set_output_fname (const char *fname)
{
  /* Output file name is required to produce prepared and instrumented files. */
  ldv_output_fname = fname;
}

void
ldv_ungetc (int c, FILE *stream)
{
  /* Finish work if can not back a character to a stream. */
  if (ungetc (c, stream) != c)
    {
      LDV_FATAL_ERROR ("character \"%c\" wasn't back to stream", c);
    }

#ifdef LDV_CHAR_DEBUG

  ldv_print_info (LDV_INFO_IO, "unget character \"%c\"", ldv_end_of_line (c));

#endif /* LDV_CHAR_DEBUG */
}
