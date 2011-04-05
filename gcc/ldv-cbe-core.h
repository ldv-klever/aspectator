#ifndef _LDV_CORE_H_
#define _LDV_CORE_H_


#define LDV_WARN(msg) warning (0, "LDV: %s: %d: %s", __FILE__, __LINE__, msg)


/* Names for different line directive levels. */
enum
{
  LDV_C_BACKEND_LINES_LEVEL_NO = 1,
  LDV_C_BACKEND_LINES_LEVEL_DECL,
  LDV_C_BACKEND_LINES_LEVEL_STATEMENT,
  LDV_C_BACKEND_LINES_LEVEL_EXPR,
  LDV_C_BACKEND_LINES_LEVEL_USEFUL,
  LDV_C_BACKEND_LINES_LEVEL_ALL
};


extern void ldv_c_backend_current_line_set (unsigned int);
extern bool ldv_c_backend_is_current_line (unsigned int);
extern bool ldv_c_backend_is_lines_level (int);
extern void ldv_c_backend_padding_cancel (void);
extern void ldv_c_backend_padding_force (void);
extern void ldv_c_backend_print (unsigned int, bool, const char *, ...) ATTRIBUTE_PRINTF_3;
extern void ldv_handle_options (void);
extern bool ldv_is_c_backend_enabled (void);
extern bool ldv_is_dump_ops (void);
extern const char *ldv_itoa (unsigned int);


#endif /* _LDV_CORE_H_ */
