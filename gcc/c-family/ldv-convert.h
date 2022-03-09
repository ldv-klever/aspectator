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

#ifndef LDV_CONVERT
#define LDV_CONVERT

#include "tree.h"

#include "ldv-grammar.h"


#define LDV_CONVERT_EXPR_RECURSION_LIMIT 100


extern ldv_assignment_expr_ptr ldv_convert_assignment_expr (tree, unsigned int);
extern ldv_decl_ptr ldv_convert_decl (tree);
extern ldv_ext_decl_ptr ldv_convert_ext_decl (tree, bool);
extern ldv_struct_decl_ptr ldv_convert_struct_decl (tree);
extern ldv_identifier_ptr ldv_convert_identifier (tree);

#endif /* LDV_CONVERT */
