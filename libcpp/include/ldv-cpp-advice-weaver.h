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

#ifndef _LDV_CPP_ADVICE_WEAVER_H_
#define _LDV_CPP_ADVICE_WEAVER_H_


#include "ldv-aspect-types.h"


struct cpp_reader;
extern void ldv_cpp_define (struct cpp_reader *);
extern void ldv_cpp_undef (struct cpp_reader *);
extern void ldv_cpp_weave (void);

extern const char *ldv_get_aspect_pattern_value_or_string (ldv_aspect_pattern_param_ptr);

extern FILE *ldv_open_aspect_pattern_param_file_stream (ldv_aspect_pattern_param_ptr);

extern void ldv_print_query_result (FILE *, const char *, ldv_list_ptr);

extern void ldv_print_macro (ldv_i_macro_ptr);

#endif /* _LDV_CPP_ADVICE_WEAVER_H_ */
