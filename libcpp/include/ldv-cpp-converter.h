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

#ifndef _LDV_CPP_CONVERTER_H_
#define _LDV_CPP_CONVERTER_H_


#include "ldv-aspect-types.h"

extern char *ldv_convert_body_to_macro (ldv_ab_ptr);
extern ldv_i_func_ptr ldv_convert_func_signature_to_internal (ldv_pps_decl_ptr);
extern ldv_i_macro_ptr ldv_convert_macro_signature_to_internal (ldv_pps_macro_ptr);
extern ldv_i_typedecl_ptr ldv_convert_typedecl_signature_to_internal (ldv_pps_decl_ptr);
extern ldv_i_var_ptr ldv_convert_var_signature_to_internal (ldv_pps_decl_ptr);

#endif /* _LDV_CPP_CONVERTER_H_ */
