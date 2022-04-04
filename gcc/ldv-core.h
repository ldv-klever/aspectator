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

#ifndef _LDV_CORE_H_
#define _LDV_CORE_H_

#include "tree.h"

#include "ldv-aspect-types.h"
/* Core types and functions from preprocessor extensions. */
#include "ldv-cpp-core.h"

/* Obtain a pointer to a string end. */
#define LDV_STR_END(str) (str + (strlen (str) - 1))
/* Obtain an offset to a string end. */
#define LDV_STR_OFFSET(str) ((strlen (str) - 1))


extern ldv_aspect_pattern_ptr ldv_create_aspect_pattern (void);
extern ldv_aspect_pattern_param_ptr ldv_create_aspect_pattern_param (void);
extern ldv_ab_ptr ldv_create_body (void);
extern ldv_ab_aspect_pattern_ptr ldv_create_body_aspect_pattern (void);
extern ldv_pps_declarator_ptr ldv_create_declarator (void);
extern ldv_file_ptr ldv_create_file (void);
extern ldv_int_ptr ldv_create_int (void);
extern ldv_pps_array_size_ptr ldv_create_pps_array_size (void);
extern ldv_pps_decl_ptr ldv_create_pps_decl (void);
extern ldv_pps_func_arg_ptr ldv_create_pps_func_arg (void);
extern ldv_pps_ptr_quals_ptr ldv_create_ptr_quals (void);

extern void ldv_free_aspect_pattern (ldv_aspect_pattern_ptr);
extern void ldv_free_declarator (ldv_pps_declarator_ptr);
extern void ldv_free_file (ldv_file_ptr);
extern void ldv_free_pps_array_size (ldv_pps_array_size_ptr);
extern void ldv_free_pps_decl (ldv_pps_decl_ptr);
extern void ldv_free_pps_func_arg (ldv_pps_func_arg_ptr);
extern void ldv_free_ptr_quals (ldv_pps_ptr_quals_ptr);

extern void ldv_delete_body (ldv_ab_ptr);
extern void ldv_delete_id (ldv_id_ptr);
extern void ldv_delete_int (ldv_int_ptr);

extern char *ldv_get_aspect_pattern_env (ldv_aspect_pattern_ptr);

extern char *ldv_get_body_text (ldv_ab_ptr);
extern char *ldv_get_file_name (ldv_file_ptr);
extern char *ldv_get_id_name (ldv_id_ptr);
extern int ldv_get_int (ldv_int_ptr);

extern unsigned int ldv_get_current_line_number(const char *);

extern void ldv_putc_body (unsigned char, ldv_ab_ptr);
extern void ldv_putc_file (unsigned char, ldv_file_ptr);
extern void ldv_puts_body (const char *, ldv_ab_ptr);
extern void ldv_puts_file (const char *, ldv_file_ptr);

void ldv_keep_original_string (tree, const cpp_string *, char *);

#endif /* _LDV_CORE_H_ */
