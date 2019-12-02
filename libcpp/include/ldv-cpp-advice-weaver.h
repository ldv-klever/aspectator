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


typedef void LDV_EVALUATE_ASPECT_PATTERN_FUNC (ldv_aspect_pattern_ptr, const char **, unsigned int *, const char **);

struct cpp_reader;
extern void ldv_cpp_define (struct cpp_reader *);
extern void ldv_cpp_undef (struct cpp_reader *);
extern void ldv_cpp_weave (void);

extern void ldv_process_aspect_pattern_fprintf (ldv_list_ptr, LDV_EVALUATE_ASPECT_PATTERN_FUNC);
extern void ldv_print_macro (ldv_i_macro_ptr);
extern void ldv_print_query_results (void);

#endif /* _LDV_CPP_ADVICE_WEAVER_H_ */
