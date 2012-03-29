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

#ifndef _LDV_CONVERTER_H_
#define _LDV_CONVERTER_H_


/* Tree conceptions. */
#include "tree.h"

#include "ldv-aspect-types.h"


extern ldv_pps_decl_ptr ldv_convert_internal_to_declaration (ldv_i_type_ptr, const char *);
extern ldv_list_ptr ldv_convert_internal_to_declarator (ldv_i_type_ptr);
extern ldv_list_ptr ldv_convert_internal_to_declarator_reverse (ldv_i_type_ptr);
extern ldv_pps_declspecs_ptr ldv_convert_internal_to_declspecs (ldv_i_type_ptr);
extern ldv_i_type_ptr ldv_convert_type_tree_to_internal (tree, tree);

#endif /* _LDV_CONVERTER_H_ */

