#ifndef _LDV_CPP_CONVERTER_H_
#define _LDV_CPP_CONVERTER_H_


#include "ldv-aspect-types.h"

extern char *ldv_convert_body_to_macro (ldv_ab_ptr);
extern ldv_i_func_ptr ldv_convert_func_signature_to_internal (ldv_pps_decl_ptr);
extern ldv_i_macro_ptr ldv_convert_macro_signature_to_internal (ldv_pps_macro_ptr);
extern ldv_i_typedecl_ptr ldv_convert_typedecl_signature_to_internal (ldv_pps_decl_ptr);
extern ldv_i_var_ptr ldv_convert_var_signature_to_internal (ldv_pps_decl_ptr);

#endif /* _LDV_CPP_CONVERTER_H_ */
