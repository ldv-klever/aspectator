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

/* General cpp core types and functions. */
#include "config.h"
#include "system.h"
#include "cpplib.h"
#include "internal.h"

#include "ldv-aspect-types.h"
#include "ldv-cpp-core.h"
#include "ldv-cpp-pointcut-matcher.h"


/* A stream where ldv cpp errors information to be printed. */
#define LDV_CPP_ERROR_STREAM    (stderr)


bool ldv_cpp = false;
int ldv_cpp_stage = -1;


static htab_t ldv_filename_htab = NULL;


static int ldv_cmp_str_any_chars (const char *, const char *);
static bool ldv_isany_chars(const char *);
static void ldv_enlarge_str (ldv_str_ptr, ldv_token_k);

int
ldv_cmp_str (ldv_id_ptr id, const char *str)
{
  char *id_name = ldv_cpp_get_id_name (id);

  /* We have to compare convinient strings in case when id doesn't
   * contain any $ wildcard. */
  if (!id->isany_chars)
    {
      return strcmp (id_name, str);
    }

  /* Otherwise id and string must be compared like for .. wildcard (see
   * libcpp/ldv-cpp-pointcut-matcher.c). */
  return ldv_cmp_str_any_chars (id_name, str);
}

static void
ldv_free_symbol_matching_table (bool **symbol_matching_table, unsigned int id_symbol_numb)
{
  unsigned int i;

  for (i = 0; i < id_symbol_numb; i++)
    free (symbol_matching_table[i]);

  free (symbol_matching_table);
}

static int
ldv_cmp_str_any_chars (const char *id, const char *str)
{
  char *c = NULL, *c_next = NULL;
  unsigned int any_chars_symbol_numb, id_symbol_numb, str_symbol_numb;
  bool **symbol_matching_table = NULL;
  unsigned int i, j, j_first_matched;
  bool ismatched;

  /* First of all count the number of $ wildcards and join continuous $
   * wildcards together if so. */
  for (c = (char *)id, any_chars_symbol_numb = 0; *c; c++)
    {
      if (ldv_isany_chars (c))
        {
          any_chars_symbol_numb++;

          /* Remove following $ wildcards if so. */
          while (*(c + 1))
            {
              if (ldv_isany_chars (c + 1))
                {
                  for (c_next = c + 1; *c_next; c_next++)
                    {
                      *c_next = *(c_next + 1);
                    }
                }
              else
                {
                  break;
                }
            }
        }
    }

  /* Create special matching table and try to find greedy 'true' path in
   * it to make correspondence between identifier and string. */

  /* Obtain sizes of matching table. Note that string symbols are along
   * x direction while identifier symbols are along y direction. */
  str_symbol_numb = strlen (str);
  id_symbol_numb = strlen (id);

  /* Finish if the number of id symbols (that are not $ wildcards) is
   * greater then the number of string symbols. */
  if (id_symbol_numb > str_symbol_numb + any_chars_symbol_numb)
    {
      return 1;
  }

  /* Finish if there is no string symbols at all. */
  if (str_symbol_numb == 0)
    {
      return 1;
  }

  /* Allocate and initialize matching table. */

  /* At the beginning allocate rows memory (corresponding to identifier
   * symbols) along y direction. */
  symbol_matching_table = (bool **) xmalloc (id_symbol_numb * sizeof (bool *));

  for (i = 0; i < id_symbol_numb; i++)
    {
      /* Fox each id symbol allocate memory corresponding to all string
       * symbols along x direction. */
      symbol_matching_table[i] = (bool *) xmalloc (str_symbol_numb * sizeof (bool));

      /* Initialize the whole matching table with false values at the
       * beginning. */
      for (j = 0; j < str_symbol_numb; j++)
        symbol_matching_table[i][j] = false;
    }

  /* Fill table by going through its rows (directly from the first to
   * the last) and updating information for it in accordence with the
   * previously obtained information. Note that the first row is
   * considered especially since there isn't previous row for it. */
  for (i = 0, j_first_matched = 0; i < id_symbol_numb; i++)
    {
      /* The first row is considered in the special way since there
       * isn't previous information for it. */
      if (i == 0)
        {
          /* Mark all first row elements as true if there is $ wildcard
           * at the first place. */
          if (ldv_isany_chars (id))
            {
              for (j = 0; j < str_symbol_numb; j++)
                symbol_matching_table[i][j] = true;
            }
          /* Otherwise the first row element must coinside with the
           * first column element. */
          else if (*id == *str)
            {
              symbol_matching_table[0][0] = true;
              j_first_matched++;
            }
          /* Identifier and string don't coinside. */
          else
            {
              ldv_free_symbol_matching_table (symbol_matching_table, id_symbol_numb);
              return 1;
            }
        }
      /* All next rows are processed in accordance with the previous
       * ones. Note that we see just on a previous row for each
       * row. */
      else
        {
          /* Mark row elements as true if there is $ wildcard. Begin
           * with an element that has marked previous (left-up)
           * element. */
          if (ldv_isany_chars (id + i))
            {
              for (j = j_first_matched; j < str_symbol_numb; j++)
                symbol_matching_table[i][j] = true;
            }
          /* Otherwise try to find all correspondence to column
           * elements for the given row element. */
          else
            {
              ismatched = false;

              /* Also begin with an element that has marked previous
               * (left-up) element. */
              for (j = j_first_matched; j < str_symbol_numb; j++)
                {
                  /* Mark elements that correspond to each other and
                   * that have previosly marked left-up or up
                   * element. */
                  if (j == 0
                    || symbol_matching_table[i - 1][j - 1] == true
                    || symbol_matching_table[i - 1][j] == true)
                    {
                      if (*(str + j) == *(id + i))
                        {
                          symbol_matching_table[i][j] = true;

                          /* We may see following rows beginning with
                           * number of column containing match. */
                          if (!ismatched)
                            {
                              j_first_matched = j + 1;
                            }

                          ismatched = true;
                        }
                    }
                }

              /* Finish because of can't find correspondence at all. */
              if (!ismatched)
                {
                  ldv_free_symbol_matching_table (symbol_matching_table, id_symbol_numb);
                  return 1;
                }
            }
        }
    }

  /* Dump matching table if needed. */
  if (LDV_DUMP_MATCHING_TABLE)
    {
      fprintf (stderr, "MATCHING TABLE (OX - STRING SYMBOLS, OY - IDENTIFIER SYMBOLS):\n");
      fprintf (stderr, "    ");

      /* Print Ox axis signatures. */
      for (j = 0; j < str_symbol_numb; j++)
        fprintf (stderr, "%2d  ", (j + 1));

      fprintf (stderr, "\n");

      /* Iterate over all matching table rows. */
      for (i = 0; i < id_symbol_numb; i++)
        {
          /* Print Oy axis signatures. */
          fprintf (stderr, "%2d: ", (i + 1));

          /* Iterate over all matching table columns. */
          for (j = 0; j < str_symbol_numb; j++)
            {
              fprintf (stderr, "%2d", (int)symbol_matching_table[i][j]);

              /* Print sepator between elements. */
              if (j != str_symbol_numb - 1)
                fprintf (stderr, ", ");
            }

          fprintf (stderr, "\n");
        }

      fprintf (stderr, "\n");
    }

  j = str_symbol_numb - 1;
  i = id_symbol_numb - 1;

  /* Skip $ wildcard that catches nothing and that is placed at the last
   * row. */
  if (ldv_isany_chars (id + i) && !symbol_matching_table[i][j])
    i--;

  /* There is no 'true' path in matching table. */
  if (!symbol_matching_table[i][j])
    {
      ldv_free_symbol_matching_table (symbol_matching_table, id_symbol_numb);
      return 1;
    }

  ldv_free_symbol_matching_table (symbol_matching_table, id_symbol_numb);

  /* Identifier correspond to the given string. */
  return 0;
}

char *
ldv_copy_str (const char *str)
{
  char *str_new = NULL;

  str_new = XCNEWVEC (char, (strlen (str) + 1));

  strcpy (str_new, str);

  return str_new;
}

void
ldv_cpp_fatal_error (const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  vfprintf (LDV_CPP_ERROR_STREAM, format, ap);
  va_end (ap);
  fputc ('\n', LDV_CPP_ERROR_STREAM);

  /* Terminate a program after a ldv cpp error. */
  exit (1);
}

char *
ldv_cpp_get_body_text (ldv_ab_ptr body)
{
  if (body)
    return ldv_get_str (body->ab_text);

  LDV_CPP_FATAL_ERROR ("body pointer wasn't initialized");

  return NULL;
}

char *
ldv_cpp_get_file_name (ldv_file_ptr file)
{
  if (file)
    return ldv_get_str (file->file_name);

  LDV_CPP_FATAL_ERROR ("file pointer wasn't initialized");

  return NULL;
}

char *
ldv_cpp_get_id_name (ldv_id_ptr id)
{
  if (id)
    return ldv_get_str (id->id_name);

  LDV_CPP_FATAL_ERROR ("id pointer wasn't initialized");

  return NULL;
}

char *
ldv_get_text (ldv_text_ptr text)
{
  if (text)
    return ldv_get_str (text->text);

  LDV_CPP_FATAL_ERROR ("text pointer wasn't initialized");

  return NULL;
}

const char *
ldv_itoa (unsigned int n)
{
  unsigned int int_digits = 1, order = 10;
  char *str = NULL;

  /* Obtain the number of digits that are contained in unsigned integer
     number. */
  for (;;) {
    if (order > UINT_MAX / 10)
      {
        int_digits++;
        break;
      }

    if (n / order >= 1)
      {
        int_digits++;
        order *= 10;
      }
    else
      break;
  }

  str = XCNEWVEC (char, (int_digits + 1));

  sprintf (str, "%u", n);

  return str;
}

char *
ldv_get_str (ldv_str_ptr str)
{
  if (str)
    return str->text;

  LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");

  return NULL;
}

ldv_pps_declspecs_ptr
ldv_create_declspecs (void)
{
  ldv_pps_declspecs_ptr declspecs = NULL;

  declspecs = XCNEW (ldv_primitive_pointcut_signature_declspecs);

  return declspecs;
}

ldv_pps_declspecs_ptr
ldv_copy_declspecs (ldv_pps_declspecs_ptr declspecs)
{
  ldv_pps_declspecs_ptr declspecs_aux = NULL, declspecs_new = NULL;

  /* Following is equivalent to coping declaration specifiers. */
  declspecs_aux = ldv_create_declspecs ();
  declspecs_new = ldv_merge_declspecs (declspecs_aux, declspecs);
  ldv_free_declspecs (declspecs_aux);

  return declspecs_new;
}

void
ldv_free_declspecs (ldv_pps_declspecs_ptr declspecs)
{
  if (declspecs->type_name)
    ldv_free_id (declspecs->type_name);

  free (declspecs);
}

ldv_pps_macro_func_param_ptr
ldv_create_macro_func_param (void)
{
  ldv_pps_macro_func_param_ptr macro_func_param = NULL;

  macro_func_param = XCNEW (ldv_primitive_pointcut_signature_macro_func_param);

  return macro_func_param;
}

void
ldv_free_macro_func_param (ldv_pps_macro_func_param_ptr macro_func_param)
{
  if (macro_func_param->name)
    ldv_free_id (macro_func_param->name);

  free (macro_func_param);
}

ldv_i_func_ptr
ldv_create_info_func (void)
{
  ldv_i_func_ptr i_func = NULL;

  i_func = XCNEW (ldv_info_func);

  return i_func;
}

void
ldv_free_info_func (ldv_i_func_ptr func)
{
  /* Aspect functions can omit actual identifier. */
  if (func->name && func->name->id_name)
    ldv_free_id (func->name);

  if (func->ptr_name)
    ldv_free_id (func->ptr_name);

  ldv_free_info_type (func->type);

  free (func->decl);

  free (func);
}

ldv_i_initializer_ptr
ldv_create_info_initializer (void)
{
  ldv_i_initializer_ptr i_initializer = NULL;

  i_initializer = XCNEW (ldv_info_initializer);

  return i_initializer;
}

void
ldv_free_info_initializer (ldv_i_initializer_ptr initializer)
{
  ldv_list_ptr struct_field_initializer_list = NULL;
  ldv_i_struct_field_initializer_ptr struct_field_initializer = NULL;
  ldv_list_ptr array_elem_initializer_list = NULL;
  ldv_i_array_elem_initializer_ptr array_elem_initializer = NULL;

  if (initializer->non_struct_or_array_initializer)
    free (initializer->non_struct_or_array_initializer);
  else if (initializer->struct_initializer)
    {
      for (struct_field_initializer_list = initializer->struct_initializer
        ; struct_field_initializer_list
        ; struct_field_initializer_list = ldv_list_get_next (struct_field_initializer_list))
        {
          struct_field_initializer = (ldv_i_struct_field_initializer_ptr) ldv_list_get_data (struct_field_initializer_list);
          free (struct_field_initializer->decl);
          /* Large initializers are not converted. */
          if (struct_field_initializer->initializer)
            ldv_free_info_initializer (struct_field_initializer->initializer);
        }

      ldv_list_delete_all (initializer->struct_initializer);
    }
  else
    {
      for (array_elem_initializer_list = initializer->array_initializer
        ; array_elem_initializer_list
        ; array_elem_initializer_list = ldv_list_get_next (array_elem_initializer_list))
        {
          array_elem_initializer = (ldv_i_array_elem_initializer_ptr) ldv_list_get_data (array_elem_initializer_list);
          /* Like for structure initializers. */
          if (array_elem_initializer->initializer)
            ldv_free_info_initializer (array_elem_initializer->initializer);
        }

      ldv_list_delete_all (initializer->array_initializer);
    }

  free (initializer);
}

ldv_i_macro_ptr
ldv_create_info_macro (void)
{
  ldv_i_macro_ptr i_macro = NULL;

  i_macro = XCNEW (ldv_info_macro);

  return i_macro;
}

void
ldv_free_info_macro (ldv_i_macro_ptr i_macro)
{
  ldv_list_ptr i_macro_param_list = NULL;

  ldv_free_id (i_macro->macro_name);

  for (i_macro_param_list = i_macro->macro_param
    ; i_macro_param_list
    ; i_macro_param_list = ldv_list_get_next (i_macro_param_list))
    ldv_free_info_macro_func_param ((ldv_i_macro_func_param_ptr) ldv_list_get_data (i_macro_param_list));

  ldv_list_delete_all (i_macro->macro_param);
  free (i_macro);
}

ldv_i_macro_func_param_ptr
ldv_create_info_macro_func_param (void)
{
  ldv_i_macro_func_param_ptr i_macro_func_param = NULL;

  i_macro_func_param = XCNEW (ldv_info_macro_func_param);

  return i_macro_func_param;
}

void
ldv_free_info_macro_func_param (ldv_i_macro_func_param_ptr i_macro_func_param)
{
  if (i_macro_func_param->name)
    ldv_free_id (i_macro_func_param->name);

  free (i_macro_func_param);
}

ldv_i_match_ptr
ldv_create_info_match (void)
{
  ldv_i_match_ptr match = NULL;

  match = XCNEW (ldv_info_match);

  match->i_kind = LDV_I_NONE;
  match->pp_kind = LDV_PP_NONE;

  return match;
}

void
ldv_free_info_match (ldv_i_match_ptr match)
{
  if (match->i_func_aspect)
    ldv_free_info_func (match->i_func_aspect);

  if (match->i_typedecl)
    ldv_free_info_typedecl (match->i_typedecl);

  free (match);
}

ldv_i_param_ptr
ldv_create_info_param (void)
{
  ldv_i_param_ptr param = NULL;

  param = XCNEW (ldv_info_param);

  return param;
}

ldv_i_param_ptr
ldv_copy_iparam (ldv_i_param_ptr iparam)
{
  ldv_i_param_ptr iparam_new = NULL;

  iparam_new = ldv_create_info_param ();

  /* Copy parameter name if so. */
  if (iparam->name)
    {
      iparam_new->name = ldv_create_id ();
      ldv_puts_id (ldv_cpp_get_id_name (iparam->name), iparam_new->name);
    }

  /* Copy parameter type. */
  iparam_new->type = ldv_copy_itype (iparam->type);

  /* Copy the rest of parameter information. */
  iparam_new->isany_params = iparam->isany_params;

  return iparam_new;
}

void
ldv_free_info_param (ldv_i_param_ptr param)
{
  if (param->name)
    ldv_free_id (param->name);

  ldv_free_info_type (param->type);

  free (param);
}

ldv_i_type_ptr
ldv_create_info_type (void)
{
  ldv_i_type_ptr type = NULL;

  type = XCNEW (ldv_info_type);

  type->it_kind = LDV_IT_NONE;

  return type;
}

ldv_i_type_ptr
ldv_copy_itype (ldv_i_type_ptr itype)
{
  ldv_i_type_ptr itype_new = NULL;
  ldv_list_ptr param_list = NULL;
  ldv_i_param_ptr iparam = NULL;

  itype_new = ldv_create_info_type ();

  itype_new->it_kind = itype->it_kind;

  if (itype->primitive_type)
    itype_new->primitive_type = ldv_copy_declspecs (itype->primitive_type);

  /* Copy array element types and the number of elements. */
  if (itype->array_type)
    itype_new->array_type = ldv_copy_itype (itype->array_type);
  itype_new->array_size = itype->array_size;
  itype_new->issize_specified = itype->issize_specified;
  itype_new->isany_size = itype->isany_size;

  /* Copy pointed type. */
  if (itype->ptr_type)
    itype_new->ptr_type = ldv_copy_itype (itype->ptr_type);
  itype_new->isconst = itype->isconst;
  itype_new->isvolatile = itype->isvolatile;
  itype_new->isrestrict = itype->isrestrict;

  /* Copy function return type and argument types. */
  if (itype->ret_type)
    itype_new->ret_type = ldv_copy_itype (itype->ret_type);
  if (itype->param)
    {
      for (param_list = itype->param
        ; param_list
        ; param_list = ldv_list_get_next (param_list))
        {
          iparam = (ldv_i_param_ptr) ldv_list_get_data (param_list);
          ldv_list_push_back (&itype_new->param, ldv_copy_iparam (iparam));
        }
    }
  itype_new->isva = itype->isva;

  return itype_new;
}

void
ldv_free_info_type (ldv_i_type_ptr type)
{
  ldv_list_ptr param_list = NULL;
  ldv_i_param_ptr param = NULL;

  switch (type->it_kind)
    {
    case LDV_IT_FUNC:
      ldv_free_info_type (type->ret_type);

      for (param_list = type->param
        ; param_list
        ; param_list = ldv_list_get_next (param_list))
      {
        param = (ldv_i_param_ptr) ldv_list_get_data (param_list);
        ldv_free_info_param (param);
      }

      ldv_list_delete_all (type->param);
      break;

    case LDV_IT_PRIMITIVE:
      ldv_free_declspecs (type->primitive_type);
      break;

    case LDV_IT_NONE:
      break;

    case LDV_IT_PTR:
      ldv_free_info_type (type->ptr_type);
      break;

    case LDV_IT_ARRAY:
      ldv_free_info_type (type->array_type);
      break;

    default:
      LDV_CPP_FATAL_ERROR ("incorrect type information kind \"%d\" is used", type->it_kind);
    }

  free (type);
}

ldv_i_typedecl_ptr
ldv_create_info_typedecl (void)
{
  ldv_i_typedecl_ptr i_typedecl = NULL;

  i_typedecl = XCNEW (ldv_info_typedecl);

  return i_typedecl;
}

void
ldv_free_info_typedecl (ldv_i_typedecl_ptr typedecl)
{
  if (typedecl->name)
    ldv_free_id (typedecl->name);

  ldv_free_info_type (typedecl->type);

  free (typedecl);
}

ldv_i_var_ptr
ldv_create_info_var (void)
{
  ldv_i_var_ptr i_var = NULL;

  i_var = XCNEW (ldv_info_var);

  return i_var;
}

void
ldv_free_info_var (ldv_i_var_ptr var)
{
  if (var->name)
    ldv_free_id (var->name);

  ldv_free_info_type (var->type);

  free (var->decl);

  free (var);
}

ldv_id_ptr
ldv_create_id (void)
{
  ldv_id_ptr id = NULL;

  id = XCNEW (ldv_id);
  id->id_name = ldv_create_str (LDV_T_ID);
  id->isany_chars = false;

  return id;
}

ldv_id_ptr
ldv_copy_id (ldv_id_ptr id)
{
  ldv_id_ptr id_new = NULL;

  id_new = XCNEW (ldv_id);

  id_new->id_name = ldv_copy_string (id->id_name);
  id_new->isany_chars = id->isany_chars;

  return id_new;
}

void
ldv_free_id (ldv_id_ptr id)
{
  ldv_free_str (id->id_name);
  free (id);
}

/* TODO: develop single internal representation for dynamic strings. */
ldv_str_ptr
ldv_create_str (ldv_token_k token_kind)
{
  ldv_str_ptr string = NULL;
  unsigned int len_start;

  /* Start string length depends on a ldv token kind. */
  switch (token_kind)
    {
    case LDV_T_FILE:
      len_start = LDV_FILE_LEN_START;
      break;

    case LDV_T_B:
      len_start = LDV_B_LEN_START;
      break;

    case LDV_T_ID:
      len_start = LDV_ID_LEN_START;
      break;

    case LDV_T_STRING:
      len_start = LDV_STRING_LEN_START;
      break;

    case LDV_T_TEXT:
      len_start = LDV_TEXT_LEN_START;
      break;

    default:
      LDV_CPP_FATAL_ERROR ("unrecognize ldv token kind \"%d\"", token_kind);
    }

  string = XCNEW (ldv_string);
  string->text = XCNEWVEC (char, (len_start + 1));

  string->text[0] = '\0';
  string->max_len = len_start;

  return string;
}

ldv_str_ptr
ldv_copy_string (ldv_str_ptr string)
{
  ldv_str_ptr string_new = NULL;

  string_new = XCNEW (ldv_string);

  string_new->text = ldv_copy_str (string->text);
  string_new->len = string->len;
  string_new->max_len = string->max_len;

  return string_new;
}

void
ldv_free_str (ldv_str_ptr string)
{
  if (string)
    {
      free (string->text);
      free (string);
    }
  else
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

ldv_str_ptr
ldv_create_string (void)
{
  return ldv_create_str (LDV_T_STRING);
}

void
ldv_free_string (ldv_str_ptr str)
{
  ldv_free_str (str);
}

ldv_text_ptr
ldv_create_text (void)
{
  ldv_text_ptr text = NULL;

  text = XCNEW (ldv_text);
  text->text = ldv_create_str (LDV_T_TEXT);

  return text;
}

void
ldv_free_text (ldv_text_ptr text)
{
  if (text)
    {
      ldv_free_str (text->text);
      free (text);
    }
  else
    {
      LDV_CPP_FATAL_ERROR ("text pointer wasn't initialized");
    }
}

static bool
ldv_isany_chars (const char *c)
{
  if (*c && *c == '$')
    return true;

  return false;
}

bool
ldv_isvoid (ldv_i_type_ptr type)
{
  if (type->it_kind != LDV_IT_PRIMITIVE)
    return false;

  if (!type->primitive_type->isvoid)
    return false;

  return true;
}

ldv_pps_declspecs_ptr
ldv_merge_declspecs (ldv_pps_declspecs_ptr declspecs_first, ldv_pps_declspecs_ptr declspecs_second)
{
  ldv_pps_declspecs_ptr declspecs = NULL;

  declspecs = ldv_create_declspecs ();

  /* Merge all declaration specifiers from two structures. */
  if (declspecs_first->type_name)
    declspecs->type_name = ldv_copy_id (declspecs_first->type_name);
  else if (declspecs_second->type_name)
    declspecs->type_name = ldv_copy_id (declspecs_second->type_name);
  declspecs->istypedef = declspecs_first->istypedef || declspecs_second->istypedef;
  declspecs->isextern = declspecs_first->isextern || declspecs_second->isextern;
  declspecs->isstatic = declspecs_first->isstatic || declspecs_second->isstatic;
  declspecs->isauto = declspecs_first->isauto || declspecs_second->isauto;
  declspecs->isregister = declspecs_first->isregister || declspecs_second->isregister;
  declspecs->isvoid = declspecs_first->isvoid || declspecs_second->isvoid;
  declspecs->ischar = declspecs_first->ischar || declspecs_second->ischar;
  declspecs->isint = declspecs_first->isint || declspecs_second->isint;
  declspecs->isfloat = declspecs_first->isfloat || declspecs_second->isfloat;
  declspecs->isdouble = declspecs_first->isdouble || declspecs_second->isdouble;

  /* Remove 'int' type specifier if 'double' or 'char' is encountered. It's done
     because of 'int' may be specified implicitly when something of 'short',
     'long', 'signed', 'unsigned' are specified. */
  if (declspecs->isdouble || declspecs->ischar)
    declspecs->isint = false;

  declspecs->isbool = declspecs_first->isbool || declspecs_second->isbool;
  declspecs->iscomplex = declspecs_first->iscomplex || declspecs_second->iscomplex;
  declspecs->isshort = declspecs_first->isshort || declspecs_second->isshort;

  /* Process "long long" as special case. */
  if (declspecs_first->islong && declspecs_second->islong)
    {
      declspecs->islong = false;
      declspecs->islonglong = true;
    }
  else
    {
      declspecs->islong = declspecs_first->islong || declspecs_second->islong;
      declspecs->islonglong = declspecs_first->islonglong || declspecs_second->islonglong;
    }

  declspecs->issigned = declspecs_first->issigned || declspecs_second->issigned;
  declspecs->isunsigned = declspecs_first->isunsigned || declspecs_second->isunsigned;
  declspecs->isstruct = declspecs_first->isstruct || declspecs_second->isstruct;
  declspecs->isunion = declspecs_first->isunion || declspecs_second->isunion;
  declspecs->isenum = declspecs_first->isenum || declspecs_second->isenum;
  declspecs->istypedef_name = declspecs_first->istypedef_name || declspecs_second->istypedef_name;
  declspecs->isconst = declspecs_first->isconst || declspecs_second->isconst;
  declspecs->isrestrict = declspecs_first->isrestrict || declspecs_second->isrestrict;
  declspecs->isvolatile = declspecs_first->isvolatile || declspecs_second->isvolatile;
  declspecs->isinline = declspecs_first->isinline || declspecs_second->isinline;

  declspecs->isuniversal_type_spec = declspecs_first->isuniversal_type_spec || declspecs_second->isuniversal_type_spec;
  declspecs->isany_params = declspecs_first->isany_params || declspecs_second->isany_params;
  declspecs->isvar_params = declspecs_first->isvar_params || declspecs_second->isvar_params;

  return declspecs;
}

FILE *
ldv_open_file_stream (const char *file_name, const char *mode)
{
  FILE *file_stream;

  if ((file_stream = fopen (file_name, mode)) == NULL)
    {
      LDV_CPP_FATAL_ERROR ("can't open file \"%s\" in mode \"%s\"", file_name, mode);
    }

  /* TODO #371 ldv_print_info (LDV_INFO_IO, "file \"%s\" was successfully opened in mode \"%s\"", file_name, mode); */

  return file_stream;
}

void
ldv_close_file_stream (FILE *file_stream)
{
  if (fclose (file_stream))
    {
      LDV_CPP_FATAL_ERROR ("can't close file stream");
    }
}

void
ldv_putc_id (unsigned char c, ldv_id_ptr id)
{
  if (id)
    ldv_putc_str (c, id->id_name, LDV_T_ID);
  else
    {
      LDV_CPP_FATAL_ERROR ("id pointer wasn't initialized");
    }
}

void
ldv_enlarge_str (ldv_str_ptr string, ldv_token_k token_kind)
{
  unsigned int len_add;

  /* Additional string length depends on token kind. */
  switch (token_kind)
    {
    case LDV_T_FILE:
      len_add = LDV_FILE_LEN_ADD;
      break;

    case LDV_T_B:
      len_add = LDV_B_LEN_ADD;
      break;

    case LDV_T_ID:
      len_add = LDV_ID_LEN_ADD;
      break;

    case LDV_T_STRING:
      len_add = LDV_STRING_LEN_ADD;
      break;

    case LDV_T_TEXT:
      len_add = LDV_TEXT_LEN_ADD;
      break;

    default:
      LDV_CPP_FATAL_ERROR ("unrecognize ldv token kind \"%d\"", token_kind);
    }

  /* Enlarge buffer lenght and reallocate memory for larger string text. */
  string->max_len += len_add;
  string->text = (char *) xrealloc (string->text, sizeof (char) * (string->max_len + 1));
}

void
ldv_putc_str (unsigned char c, ldv_str_ptr string, ldv_token_k token_kind)
{
  if (!string)
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }

  /* Enlarge internal string since it is already full. */
  if (string->len == string->max_len)
    ldv_enlarge_str (string, token_kind);

  /* Put character to the end of internal string. */
  string->text[string->len] = c;
  string->text[string->len + 1] = '\0';
  string->len++;
}

void
ldv_putc_string (unsigned char c, ldv_str_ptr string)
{
  if (string)
    ldv_putc_str (c, string, LDV_T_STRING);
  else
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

void
ldv_putc_text (unsigned char c, ldv_text_ptr text)
{
  if (text)
    ldv_putc_str (c, text->text, LDV_T_TEXT);
  else
    {
      LDV_CPP_FATAL_ERROR ("text pointer wasn't initialized");
    }
}

void
ldv_puts_id (const char *str, ldv_id_ptr id)
{
  if (id)
    ldv_puts_str (str, id->id_name, LDV_T_ID);
  else
    {
      LDV_CPP_FATAL_ERROR ("id pointer wasn't initialized");
    }
}

void
ldv_puts_str (const char *str, ldv_str_ptr string, ldv_token_k token_kind)
{
  const char *c = NULL;
  ssize_t len;

  if (!str)
    {
      LDV_CPP_FATAL_ERROR ("symbol pointer wasn't initialized");
    }

  if (!string)
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }

  len = strlen (str);

  /* Enlarge internal string until it will be enough for putting string. */
  while (len + string->len > string->max_len)
    ldv_enlarge_str (string, token_kind);

  /* Put all symbols of string to the end of internal string. */
  for (c = str; ; c++)
    {
      string->text[string->len] = *c;

      if (!*c)
        break;

      /* This field corresponds to the number of characters without
         including string terminator. */
      string->len++;
    }
}

void
ldv_puts_string (const char *str, ldv_str_ptr string)
{
  if (string)
    ldv_puts_str (str, string, LDV_T_STRING);
  else
    {
      LDV_CPP_FATAL_ERROR ("string pointer wasn't initialized");
    }
}

void
ldv_puts_text (const char *str, ldv_text_ptr text)
{
  if (text)
    ldv_puts_str (str, text->text, LDV_T_TEXT);
  else
    {
      LDV_CPP_FATAL_ERROR ("text pointer wasn't initialized");
    }
}

void
ldv_set_ldv (bool ldv_gcc)
{
  /* Set an ldv flag from gcc to cpp. */
  ldv_cpp = ldv_gcc;
}

void
ldv_set_ldv_stage (int ldv_gcc_stage)
{
  /* Set an ldv stage from gcc to cpp. */
  ldv_cpp_stage = ldv_gcc_stage;
}

char *
ldv_truncate_braces (char *str)
{
  if (str[0] == '{')
    {
      str++;

      if (str[strlen (str) - 1] == '}')
        {
          str[strlen (str) - 1] = '\0';
          return str;
        }
    }

  LDV_CPP_FATAL_ERROR ("can't truncate braces from \"%s\" string", str);

  return NULL;
}

/* CIF users prefer absolute normalized filenames for entities. But calling
   lrealpath for all filenames is too expensive. Use hashtable to accumulate
   already processed filenames. */
struct ldv_realpath
{
  const char *path;
  const char *realpath;
};

static hashval_t
ldv_htab_hash_path (const void *p)
{
  return htab_hash_string (((struct ldv_realpath *)p)->path);
}

static int
ldv_htab_eq_path (const void *p, const void *q)
{
  return strcmp (((const struct ldv_realpath *)p)->path, (const char *)q) == 0;
}

const char *
ldv_get_realpath (const char *filename)
{
  void **slot;

  if (!ldv_filename_htab)
    ldv_filename_htab = htab_create (127, ldv_htab_hash_path, ldv_htab_eq_path, NULL);

  slot = htab_find_slot_with_hash (ldv_filename_htab, filename, htab_hash_string (filename), INSERT);

  if (!*slot)
    {
      struct ldv_realpath *v;
      *slot = v = XCNEW (struct ldv_realpath);
      v->path = filename;
      v->realpath = lrealpath (filename);
    }

  return ((struct ldv_realpath *)*slot)->realpath;
}
