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

#ifndef _LDV_IO_H_
#define _LDV_IO_H_


/* To use functions and types needed to dump aspected preprocessed source code. */
#include "tree.h"

#include "ldv-aspect-types.h"


#define LDV_INFO                "INFO"
#define LDV_INFO_BISON          "INFO BISON"
#define LDV_INFO_WEAVE          "INFO WEAVING"
#define LDV_INFO_IO             "INFO IO"
#define LDV_INFO_LEX            "INFO LEX"
#define LDV_INFO_LIST           "INFO LIST"
#define LDV_INFO_MATCH          "INFO MATCHING"
#define LDV_INFO_MATCHING_TABLE "INFO MATCHING TABLE"
#define LDV_INFO_MEM            "INFO MEMORY"

#define LDV_ERROR_BISON     "ERROR BISON"
#define LDV_ERROR_LEX       "ERROR LEX"

#define LDV_INSTRUMENTED_FILE_STREAM    (ldv_get_instrumented_file_stream ())
#define LDV_ASPECT_STREAM               (ldv_get_aspect_stream ())
#define LDV_EXPR_VISUALIZATION_STREAM   (stderr)
#define LDV_INFO_STREAM                 (stderr)
#define LDV_MAIN_STREAM                 (ldv_get_main_stream ())
#define LDV_MATCHED_BY_NAME             (stderr)
#define LDV_FILE_PREPARED_STREAM        (ldv_get_file_prepared_stream ())


typedef struct ldv_decl_for_print_internal
{
  ldv_text_ptr decl;
  const char *file;
  int line;
  int column;
} ldv_decl_for_print;
typedef ldv_decl_for_print *ldv_decl_for_print_ptr;


extern const char *ldv_aspect_fname;
extern const char *ldv_aspect_fname_base;

extern bool ldv_isinfo;
extern bool ldv_isinfo_bison;
extern bool ldv_isinfo_weave;
extern bool ldv_isinfo_io;
extern bool ldv_isinfo_lex;
extern bool ldv_isinfo_list;
extern bool ldv_isinfo_match;
extern bool ldv_isinfo_matching_table;
extern bool ldv_isinfo_mem;
extern bool ldv_isexpr_visualization;
extern bool ldv_isprint_signature_of_matched_by_name;

extern const char *ldv_dir_res;
extern const char *ldv_dir_temp;

extern ldv_list_ptr ldv_decl_for_print_list;
extern ldv_text_ptr ldv_func_defs_for_print;


extern void ldv_create_files (void);
extern void ldv_copy_file (const char *, FILE *);
extern const char *ldv_empty_str (void);
extern char ldv_end_of_line (int);
extern FILE *ldv_get_instrumented_file_stream (void);
extern FILE *ldv_get_aspect_stream (void);
extern FILE *ldv_get_main_stream (void);
extern FILE *ldv_get_file_prepared_stream (void);
extern int ldv_getc (FILE *);
extern void ldv_make_includes (void);
extern void ldv_open_file_streams (void);
extern void ldv_print_info (const char *, const char *, ...) ATTRIBUTE_PRINTF_2;
extern void ldv_print_to_awfile (void);
extern void ldv_putc (int, FILE *);
extern void ldv_puts (const char *, FILE *);
extern int ldv_putsn (const char *, FILE *, int);
extern void ldv_set_nomatch (void);
extern void ldv_set_output_fname (const char *);
extern void ldv_ungetc (int, FILE *);


#endif /* _LDV_IO_H_ */
