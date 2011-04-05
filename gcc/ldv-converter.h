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

