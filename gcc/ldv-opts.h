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

#ifndef _LDV_OPTS_H_
#define _LDV_OPTS_H_


extern int ldv_get_ldv_stage (void);
extern void ldv_handle_options (void);
extern bool ldv_isldv (void);
extern bool ldv_isldv_stage_preprocessing (void);
extern bool ldv_isldv_stage_first (void);
extern bool ldv_isldv_stage_second (void);
extern bool ldv_isldv_stage_third (void);
extern bool ldv_isldv_stage_fourth (void);


#endif /* _LDV_OPTS_H_ */
