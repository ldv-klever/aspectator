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

#include "ldv-cpp-core.h"
#include "ldv-list.h"


/* List elements consist of two fields: a pointer on abstract data and a pointer
   on a next element. */
struct ldv_list_element
{
  void *data;
  struct ldv_list_element *next;
};


static ldv_list_ptr ldv_list_create (void);


ldv_list_ptr
ldv_list_create (void)
{
  ldv_list_ptr list = NULL;

  list = XCNEW (ldv_list);

  list->data = NULL;
  list->next = NULL;

  return list;
}

ldv_list_ptr
ldv_list_delete (ldv_list_ptr_ptr list_first, ldv_list_ptr list)
{
  ldv_list_ptr list_prev;

  if (!list_first || !list)
    {
      LDV_CPP_FATAL_ERROR ("can't delete element of empty list or for empty list element");
    }

  /* Return 0 if a first element of a list is deleted. */
  if (*list_first == list)
    {
      *list_first = ldv_list_get_next (list);

      free (list);

      return NULL;
    }
  /* Otherwise change a next element for a previous one to a next for a given
     element and return this previous element. */
  else
    {
      list_prev = ldv_list_get_prev (*list_first, list);

      list_prev->next = list->next;

      free (list);

      return list_prev;
    }
}

void
ldv_list_delete_all (ldv_list_ptr list)
{
  ldv_list_ptr list_cur = NULL, list_delete = NULL;

  for (list_cur = list_delete = list
    ; list_cur
    ; list_delete = list_cur)
    {
      list_cur = ldv_list_get_next (list_cur);
      free (list_delete);
    }
}

void *
ldv_list_get_data (ldv_list_ptr list)
{
  if (!list)
    {
      LDV_CPP_FATAL_ERROR ("can't get data of empty list");
    }

  if (!list->data)
    {
      LDV_CPP_FATAL_ERROR ("can't get empty data of list");
    }

  return list->data;
}

ldv_list_ptr
ldv_list_get_next (ldv_list_ptr list)
{
  if (!list)
    {
      LDV_CPP_FATAL_ERROR ("can't get next element of empty list");
    }

  return list->next;
}

ldv_list_ptr
ldv_list_get_last (ldv_list_ptr list)
{
  ldv_list_ptr list_cur = NULL;

  if (!list)
    {
      LDV_CPP_FATAL_ERROR ("can't get the last element of empty list");
    }

  /* Walk to a last element of a list and return it. */
  for (list_cur = list
    ; list_cur->next
    ; list_cur = ldv_list_get_next (list_cur)) ;

  return list_cur;
}

ldv_list_ptr
ldv_list_get_prev (ldv_list_ptr list_first, ldv_list_ptr list)
{
  ldv_list_ptr list_cur = NULL;

  if (!list_first || !list)
    {
      LDV_CPP_FATAL_ERROR ("can't get previous element of empty list or for empty list element");
    }

  if (list_first == list)
    return list_first;

  for (list_cur = list_first
    ; list_cur
    ; list_cur = ldv_list_get_next (list_cur))
    {
      if (ldv_list_get_next (list_cur) == list)
        return list_cur;
    }

  LDV_CPP_FATAL_ERROR ("can't get the previous element of list");

  return NULL;
}

ldv_list_ptr
ldv_list_insert_data (ldv_list_ptr list, void *data)
{
  ldv_list_ptr list_next = NULL, list_new = NULL;

  ldv_list_push_back (&list_new, data);

  /* Insert a new list element with a given data to a non empty list. */
  if (list)
    {
      list_next = ldv_list_get_next (list);

      list->next = list_new;

      list_new->next = list_next;
    }

  return list_new;
}

unsigned int
ldv_list_len (ldv_list_ptr list)
{
  unsigned int list_len = 0;
  ldv_list_ptr list_cur;

  for (list_cur = list
    ; list_cur
    ; list_cur = ldv_list_get_next (list_cur), list_len++) ;

  return list_len;
}

void
ldv_list_push_back (ldv_list_ptr_ptr list, void *data)
{
  ldv_list_ptr list_cur = NULL, list_new = NULL;

  list_new = ldv_list_create ();

  list_new->data = data;

  /* Add a new element to an end of a list if a list already has some
     elements. */
  if (*list)
    {
      for (list_cur = *list
        ; list_cur->next
        ; list_cur = ldv_list_get_next (list_cur)) ;

      list_cur->next = list_new;
    }
   /* Otherwise make a new element as a beginning of a list. */
   else
     *list = list_new;
}

ldv_list_ptr
ldv_list_push_front (ldv_list_ptr list, void *data)
{
  ldv_list_ptr list_new = NULL;

  list_new = ldv_list_create ();

  list_new->data = data;

  /* Add a new element to the beginning of a list if it already has some
     elements. */
  if (list)
    list_new->next = list;

   return list_new;
}

ldv_list_ptr
ldv_list_reverse (ldv_list_ptr list)
{
  ldv_list_ptr list_cur = NULL, list_next = NULL, list_reverse = NULL;

  for (list_cur = list
    ; list_cur
    ; list_cur = list_next)
    {
      list_next = list_cur->next;
      list_cur->next = list_reverse;
      list_reverse = list_cur;
    }

  return list_reverse;
}

void
ldv_list_set_data (ldv_list_ptr list, void *data)
{
  if (!list)
    {
      LDV_CPP_FATAL_ERROR ("can't set data for empty list");
    }

  list->data = data;
}

ldv_list_ptr
ldv_list_splice (ldv_list_ptr list_first, ldv_list_ptr list_second)
{
  ldv_list_ptr list_first_last = NULL;

  /* Add a second list to the end of a first one and return a beginning of
     a spliced list if a first list contains some elements. */
  if (list_first)
    {
      list_first_last = ldv_list_get_last (list_first);

      list_first_last->next = list_second;

      return list_first;
    }

  /* Otherwise return a beginning of a second list. */
  return list_second;
}
