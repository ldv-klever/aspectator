#ifndef _LDV_ADVICE_WEAVER_H_
#define _LDV_ADVICE_WEAVER_H_


/* Tree conceptions. */
#include "tree.h"

#include "ldv-aspect-types.h"


extern expanded_location ldv_decl_beginning_location;


extern bool ldv_isweaved (const char *, bool);
extern const char *ldv_print_func_decl (ldv_i_func_ptr);
extern const char *ldv_print_type_decl (ldv_i_typedecl_ptr);
extern const char *ldv_print_var_decl (ldv_i_var_ptr);
extern void ldv_weave_advice (expanded_location *, expanded_location *);

#endif /* _LDV_ADVICE_WEAVER_H_ */
