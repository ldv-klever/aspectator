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

#ifndef _LDV_ADVICE_WEAVER_H_
#define _LDV_ADVICE_WEAVER_H_


/* Tree conceptions. */
#include "tree.h"

#include "ldv-aspect-types.h"


#define CIF_AUX_FUNC_NAME_PREFIX "cif_"


extern expanded_location ldv_decl_beginning_location;


extern bool ldv_isweaved (const char *, bool);
extern const char *ldv_print_func_decl (ldv_i_func_ptr);
extern void ldv_diagnostics (void);
extern const char *ldv_print_type_decl (ldv_i_typedecl_ptr);
extern char *ldv_print_var_decl (ldv_i_var_ptr);
extern void ldv_weave_advice (expanded_location *, expanded_location *);

#endif /* _LDV_ADVICE_WEAVER_H_ */
