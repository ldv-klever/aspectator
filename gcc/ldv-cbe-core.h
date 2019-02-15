/* Copyright (C) 2010-2012
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

#ifndef _LDV_CBE_CORE_H_
#define _LDV_CBE_CORE_H_


#define LDV_WARN(msg) warning (0, "LDV: %s: %d: %s", __FILE__, __LINE__, msg)
/* Like in ldv-core.h. */
#define LDV_FATAL_ERROR(msg) do { fprintf (stderr, "LDV: %s: %d: fatal error took place here\n", __FILE__, __LINE__); internal_error(msg); } while(0)

/* Names for different line directive levels. */
enum
{
  LDV_C_BACKEND_LINES_LEVEL_NO = 1,
  LDV_C_BACKEND_LINES_LEVEL_DECL,
  LDV_C_BACKEND_LINES_LEVEL_STATEMENT,
  LDV_C_BACKEND_LINES_LEVEL_EXPR,
  LDV_C_BACKEND_LINES_LEVEL_USEFUL,
  LDV_C_BACKEND_LINES_LEVEL_ALL
};


extern bool ldv_c_backend_printing_disabled;


extern void ldv_c_backend_current_line_set (unsigned int);
extern bool ldv_c_backend_is_current_line (unsigned int);
extern bool ldv_c_backend_is_lines_level (int);
extern char *ldv_c_backend_get_buffer (void);
extern int ldv_c_backend_get_lines_level (void);
extern void ldv_c_backend_padding_cancel (void);
extern void ldv_c_backend_padding_force (void);
extern void ldv_c_backend_print (unsigned int, bool, const char *, ...) ATTRIBUTE_PRINTF_3;
extern void ldv_c_backend_print_to_buffer (void);
extern void ldv_c_backend_print_to_file (void);
extern void ldv_c_backend_set_lines_level (int);
extern void ldv_cbe_handle_options (void);
extern bool ldv_is_c_backend_enabled (void);
extern bool ldv_is_dump_ops (void);
extern char *ldv_cbe_itoa (unsigned int);


#endif /* _LDV_CBE_CORE_H_ */
