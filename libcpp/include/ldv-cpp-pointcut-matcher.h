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

#ifndef _LDV_CPP_POINTCUT_MATCHER_H_
#define _LDV_CPP_POINTCUT_MATCHER_H_

#include "ldv-aspect-types.h"
#include "ldv-list.h"


/* A variable contains all information on advice definitions obtained from an
   aspect file. */
extern ldv_list_ptr ldv_adef_list;
extern ldv_i_match_ptr ldv_i_match;


extern bool ldv_match_cp (ldv_cp_ptr, ldv_i_match_ptr);
extern bool ldv_match_file_signature (const char *, ldv_pps_file_ptr);
extern bool ldv_match_func_signature (ldv_i_match_ptr, ldv_pps_decl_ptr);
extern void ldv_match_macro (ldv_i_macro_ptr, ldv_ppk);
extern bool ldv_match_macro_signature (ldv_i_match_ptr, ldv_pps_macro_ptr);
extern bool ldv_match_typedecl_signature (ldv_i_match_ptr, ldv_pps_decl_ptr);
extern bool ldv_match_var_signature (ldv_i_match_ptr, ldv_pps_decl_ptr);
extern void ldv_set_isprint_signature_of_matched_by_name (bool);

#endif /* _LDV_CPP_POINTCUT_MATCHER_H_ */
