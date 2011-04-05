#ifndef _LDV_POINTCUT_MATCHER_H_
#define _LDV_POINTCUT_MATCHER_H_


/* Tree conceptions. */
#include "tree.h"

#include "ldv-aspect-types.h"


extern ldv_pps_declspecs_ptr ldv_entity_declspecs;
extern ldv_list_ptr ldv_func_arg_info_list;
extern tree ldv_func_called_matched;
extern tree ldv_func_decl_matched;


extern void ldv_match_func (tree, ldv_ppk);
extern void ldv_match_func_body (tree);
extern void ldv_match_typedecl (tree, const char *);
extern void ldv_match_var (tree, ldv_ppk);


#endif /* _LDV_POINTCUT_MATCHER_H_ */
