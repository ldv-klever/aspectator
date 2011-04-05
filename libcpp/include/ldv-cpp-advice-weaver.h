#ifndef _LDV_CPP_ADVICE_WEAVER_H_
#define _LDV_CPP_ADVICE_WEAVER_H_


#include "ldv-aspect-types.h"


struct cpp_reader;
extern void ldv_cpp_define (struct cpp_reader *);
extern void ldv_cpp_undef (struct cpp_reader *);
extern void ldv_print_macro (ldv_i_macro_ptr);

#endif /* _LDV_CPP_ADVICE_WEAVER_H_ */
