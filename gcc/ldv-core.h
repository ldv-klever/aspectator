#ifndef _LDV_CORE_H_
#define _LDV_CORE_H_


#include "ldv-aspect-types.h"
/* Core types and functions from preprocessor extensions. */
#include "ldv-cpp-core.h"


/* Obtain a pointer to a string end. */
#define LDV_STR_END(str) (str + (strlen (str) - 1))
/* Obtain an offset to a string end. */
#define LDV_STR_OFFSET(str) ((strlen (str) - 1))


extern ldv_ab_ptr ldv_create_body (void);
extern ldv_ab_arg_ptr ldv_create_body_arg (void);
extern ldv_ab_general_ptr ldv_create_body_general (void);
extern ldv_pps_declarator_ptr ldv_create_declarator (void);
extern ldv_file_ptr ldv_create_file (void);
extern ldv_id_ptr ldv_create_id (void);
extern ldv_int_ptr ldv_create_int (void);
extern ldv_pps_array_size_ptr ldv_create_pps_array_size (void);
extern ldv_pps_decl_ptr ldv_create_pps_decl (void);
extern ldv_pps_func_arg_ptr ldv_create_pps_func_arg (void);
extern ldv_pps_ptr_quals_ptr ldv_create_ptr_quals (void);
extern ldv_str_ptr ldv_create_string (void);
extern ldv_text_ptr ldv_create_text (void);

extern void ldv_delete_body (ldv_ab_ptr);
extern void ldv_delete_file (ldv_file_ptr);
extern void ldv_delete_id (ldv_id_ptr);
extern void ldv_delete_int (ldv_int_ptr);
extern void ldv_delete_str (ldv_str_ptr);
extern void ldv_delete_text (ldv_text_ptr);

extern const char *ldv_itoa (unsigned int);

extern char *ldv_get_body_text (ldv_ab_ptr);
extern char *ldv_get_file_name (ldv_file_ptr);
extern char *ldv_get_id_name (ldv_id_ptr);
extern int ldv_get_int (ldv_int_ptr);
extern char *ldv_get_str (ldv_str_ptr);
extern char *ldv_get_text (ldv_text_ptr);


extern ldv_pps_declspecs_ptr ldv_merge_declspecs (ldv_pps_declspecs_ptr, ldv_pps_declspecs_ptr);

extern void ldv_putc_body (unsigned char, ldv_ab_ptr);
extern void ldv_putc_file (unsigned char, ldv_file_ptr);
extern void ldv_putc_id (unsigned char, ldv_id_ptr);
extern void ldv_puts_id (const char *, ldv_id_ptr);
extern void ldv_puts_string (const char *, ldv_str_ptr);
extern void ldv_putc_text (unsigned char, ldv_text_ptr);
extern void ldv_puts_text (const char *, ldv_text_ptr);


#endif /* _LDV_CORE_H_ */
