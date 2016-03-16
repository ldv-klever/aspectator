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

#ifndef _LDV_POINTCUT_MATCHER_H_
#define _LDV_POINTCUT_MATCHER_H_


/* Tree conceptions. */
#include "tree.h"

#include "ldv-aspect-types.h"


extern ldv_pps_declspecs_ptr ldv_entity_declspecs;
extern ldv_list_ptr ldv_func_arg_info_list;
extern tree ldv_func_called_matched;
extern tree ldv_func_decl_matched;


extern ldv_i_func_ptr ldv_match_func (tree, unsigned int, ldv_ppk);
extern void ldv_match_func_body (tree, ldv_i_func_ptr);
extern void ldv_match_typedecl (tree, ldv_ppk);
extern void ldv_match_var (tree, ldv_ppk);


#endif /* _LDV_POINTCUT_MATCHER_H_ */
