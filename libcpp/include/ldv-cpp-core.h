#ifndef _LDV_CPP_CORE_H_
#define _LDV_CPP_CORE_H_


/* General cpp core types and functions. */
#include "config.h"
#include "system.h"
#include "cpplib.h"

#include "ldv-aspect-types.h"
#include "ldv-list.h"


/* A macrofunction compares ends of strings. The number of letters to be
   compared is a lenght of a string that is smaller that an another one. This's
   needed for a file pathes matching. */
#define LDV_STR_END_EQ(str_small, str_large) (strlen (str_small) <= strlen (str_large) ? !strncmp (str_small, str_large + (strlen (str_large) - strlen (str_small)), strlen (str_small)) : 0)


/* Terminal symbols that require some memory allocation. */
typedef enum { LDV_T_FILE, LDV_T_ID, LDV_T_B, LDV_T_STRING, LDV_T_TEXT } ldv_token_k;


/* Flags specify what ldv actions must be performed. */
extern bool ldv_cpp_isldv;
extern bool ldv_cpp_isinfo_matching_table;


extern char *ldv_copy_str (const char *);
extern ldv_pps_declspecs_ptr ldv_create_declspecs (void);
extern ldv_i_func_ptr ldv_create_info_func (void);
extern ldv_i_macro_ptr ldv_create_info_macro (void);
extern ldv_i_match_ptr ldv_create_info_match (void);
extern ldv_i_param_ptr ldv_create_info_param (void);
extern ldv_i_type_ptr ldv_create_info_type (void);
extern ldv_i_typedecl_ptr ldv_create_info_typedecl (void);
extern ldv_i_var_ptr ldv_create_info_var (void);
extern void ldv_cpp_fatal_error (const char *, ...) ATTRIBUTE_PRINTF_1;
extern char *ldv_cpp_get_body_text (ldv_ab_ptr);
extern char *ldv_cpp_get_file_name (ldv_file_ptr);
extern char *ldv_cpp_get_id_name (ldv_id_ptr);
extern char *ldv_cpp_get_str (ldv_str_ptr);
extern bool ldv_isvoid (ldv_i_type_ptr );
extern void ldv_set_ldv (bool);
extern void ldv_set_ldv_opts (bool);
extern char *ldv_trunkate_braces (char *);

#endif /* _LDV_CPP_CORE_H_ */
