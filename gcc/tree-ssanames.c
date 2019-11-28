/* Generic routines for manipulating SSA_NAME expressions
   Copyright (C) 2003-2017 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "gimple-iterator.h"
#include "stor-layout.h"
#include "tree-into-ssa.h"
#include "tree-ssa.h"

/* Rewriting a function into SSA form can create a huge number of SSA_NAMEs,
   many of which may be thrown away shortly after their creation if jumps
   were threaded through PHI nodes.

   While our garbage collection mechanisms will handle this situation, it
   is extremely wasteful to create nodes and throw them away, especially
   when the nodes can be reused.

   For PR 8361, we can significantly reduce the number of nodes allocated
   and thus the total amount of memory allocated by managing SSA_NAMEs a
   little.  This additionally helps reduce the amount of work done by the
   garbage collector.  Similar results have been seen on a wider variety
   of tests (such as the compiler itself).

   Right now we maintain our free list on a per-function basis.  It may
   or may not make sense to maintain the free list for the duration of
   a compilation unit.

   External code should rely solely upon HIGHEST_SSA_VERSION and the
   externally defined functions.  External code should not know about
   the details of the free list management.

   External code should also not assume the version number on nodes is
   monotonically increasing.  We reuse the version number when we
   reuse an SSA_NAME expression.  This helps keep arrays and bitmaps
   more compact.  */


/* Version numbers with special meanings.  We start allocating new version
   numbers after the special ones.  */
#define UNUSED_NAME_VERSION 0

unsigned int ssa_name_nodes_reused;
unsigned int ssa_name_nodes_created;

#define FREE_SSANAMES(fun) (fun)->gimple_df->free_ssanames
#define FREE_SSANAMES_QUEUE(fun) (fun)->gimple_df->free_ssanames_queue


/* Initialize management of SSA_NAMEs to default SIZE.  If SIZE is
   zero use default.  */

void
init_ssanames (struct function *fn, int size)
{
  if (size < 50)
    size = 50;

  vec_alloc (SSANAMES (fn), size);

  /* Version 0 is special, so reserve the first slot in the table.  Though
     currently unused, we may use version 0 in alias analysis as part of
     the heuristics used to group aliases when the alias sets are too
     large.

     We use vec::quick_push here because we know that SSA_NAMES has at
     least 50 elements reserved in it.  */
  SSANAMES (fn)->quick_push (NULL_TREE);
  FREE_SSANAMES (fn) = NULL;
  FREE_SSANAMES_QUEUE (fn) = NULL;

  fn->gimple_df->ssa_renaming_needed = 0;
  fn->gimple_df->rename_vops = 0;
}

/* Finalize management of SSA_NAMEs.  */

void
fini_ssanames (struct function *fn)
{
  vec_free (SSANAMES (fn));
  vec_free (FREE_SSANAMES (fn));
  vec_free (FREE_SSANAMES_QUEUE (fn));
}

/* Dump some simple statistics regarding the re-use of SSA_NAME nodes.  */

void
ssanames_print_statistics (void)
{
  fprintf (stderr, "SSA_NAME nodes allocated: %u\n", ssa_name_nodes_created);
  fprintf (stderr, "SSA_NAME nodes reused: %u\n", ssa_name_nodes_reused);
}

/* Verify the state of the SSA_NAME lists.

   There must be no duplicates on the free list.
   Every name on the free list must be marked as on the free list.
   Any name on the free list must not appear in the IL.
   No names can be leaked.  */

DEBUG_FUNCTION void
verify_ssaname_freelists (struct function *fun)
{
  if (!gimple_in_ssa_p (fun))
    return;

  bitmap names_in_il = BITMAP_ALLOC (NULL);

  /* Walk the entire IL noting every SSA_NAME we see.  */
  basic_block bb;
  FOR_EACH_BB_FN (bb, fun)
    {
      tree t;
      /* First note the result and arguments of PHI nodes.  */
      for (gphi_iterator gsi = gsi_start_phis (bb);
	   !gsi_end_p (gsi);
	   gsi_next (&gsi))
	{
	  gphi *phi = gsi.phi ();
	  t = gimple_phi_result (phi);
	  bitmap_set_bit (names_in_il, SSA_NAME_VERSION (t));

	  for (unsigned int i = 0; i < gimple_phi_num_args (phi); i++)
	    {
	      t = gimple_phi_arg_def (phi, i);
	      if (TREE_CODE (t) == SSA_NAME)
		bitmap_set_bit (names_in_il, SSA_NAME_VERSION (t));
	    }
	}

      /* Then note the operands of each statement.  */
      for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	   !gsi_end_p (gsi);
	   gsi_next (&gsi))
	{
	  ssa_op_iter iter;
	  gimple *stmt = gsi_stmt (gsi);
	  FOR_EACH_SSA_TREE_OPERAND (t, stmt, iter, SSA_OP_ALL_OPERANDS)
	    bitmap_set_bit (names_in_il, SSA_NAME_VERSION (t));
	}
    }

  /* Now walk the free list noting what we find there and verifying
     there are no duplicates.  */
  bitmap names_in_freelists = BITMAP_ALLOC (NULL);
  if (FREE_SSANAMES (fun))
    {
      for (unsigned int i = 0; i < FREE_SSANAMES (fun)->length (); i++)
	{
	  tree t = (*FREE_SSANAMES (fun))[i];

	  /* Verify that the name is marked as being in the free list.  */
	  gcc_assert (SSA_NAME_IN_FREE_LIST (t));

	  /* Verify the name has not already appeared in the free list and
	     note it in the list of names found in the free list.  */
	  gcc_assert (!bitmap_bit_p (names_in_freelists, SSA_NAME_VERSION (t)));
	  bitmap_set_bit (names_in_freelists, SSA_NAME_VERSION (t));
	}
    }

  /* Similarly for the names in the pending free list.  */
  if (FREE_SSANAMES_QUEUE (fun))
    {
      for (unsigned int i = 0; i < FREE_SSANAMES_QUEUE (fun)->length (); i++)
	{
	  tree t = (*FREE_SSANAMES_QUEUE (fun))[i];

	  /* Verify that the name is marked as being in the free list.  */
	  gcc_assert (SSA_NAME_IN_FREE_LIST (t));

	  /* Verify the name has not already appeared in the free list and
	     note it in the list of names found in the free list.  */
	  gcc_assert (!bitmap_bit_p (names_in_freelists, SSA_NAME_VERSION (t)));
	  bitmap_set_bit (names_in_freelists, SSA_NAME_VERSION (t));
	}
    }

  /* If any name appears in both the IL and the freelists, then
     something horrible has happened.  */
  bool intersect_p = bitmap_intersect_p (names_in_il, names_in_freelists);
  gcc_assert (!intersect_p);

  /* Names can be queued up for release if there is an ssa update
     pending.  Pretend we saw them in the IL.  */
  if (names_to_release)
    bitmap_ior_into (names_in_il, names_to_release);

  /* Function splitting can "lose" SSA_NAMEs in an effort to ensure that
     debug/non-debug compilations have the same SSA_NAMEs.  So for each
     lost SSA_NAME, see if it's likely one from that wart.  These will always
     be marked as default definitions.  So we loosely assume that anything
     marked as a default definition isn't leaked by pretending they are
     in the IL.  */
  for (unsigned int i = UNUSED_NAME_VERSION + 1; i < num_ssa_names; i++)
    if (ssa_name (i) && SSA_NAME_IS_DEFAULT_DEF (ssa_name (i)))
      bitmap_set_bit (names_in_il, i);

  unsigned int i;
  bitmap_iterator bi;
  bitmap all_names = BITMAP_ALLOC (NULL);
  bitmap_set_range (all_names, UNUSED_NAME_VERSION + 1, num_ssa_names - 1);
  bitmap_ior_into (names_in_il, names_in_freelists);

  /* Any name not mentioned in the IL and not in the feelists
     has been leaked.  */
  EXECUTE_IF_AND_COMPL_IN_BITMAP(all_names, names_in_il,
				 UNUSED_NAME_VERSION + 1, i, bi)
    gcc_assert (!ssa_name (i));

  BITMAP_FREE (all_names);
  BITMAP_FREE (names_in_freelists);
  BITMAP_FREE (names_in_il);
}

/* Move all SSA_NAMEs from FREE_SSA_NAMES_QUEUE to FREE_SSA_NAMES.

   We do not, but should have a mode to verify the state of the SSA_NAMEs
   lists.  In particular at this point every name must be in the IL,
   on the free list or in the queue.  Anything else is an error.  */

void
flush_ssaname_freelist (void)
{
  vec_safe_splice (FREE_SSANAMES (cfun), FREE_SSANAMES_QUEUE (cfun));
  vec_safe_truncate (FREE_SSANAMES_QUEUE (cfun), 0);
}

/* Return an SSA_NAME node for variable VAR defined in statement STMT
   in function FN.  STMT may be an empty statement for artificial
   references (e.g., default definitions created when a variable is
   used without a preceding definition).  If VERISON is not zero then
   allocate the SSA name with that version.  */

tree
make_ssa_name_fn (struct function *fn, tree var, gimple *stmt,
		  unsigned int version)
{
  tree t;
  use_operand_p imm;

  gcc_assert (VAR_P (var)
	      || TREE_CODE (var) == PARM_DECL
	      || TREE_CODE (var) == RESULT_DECL
	      || (TYPE_P (var) && is_gimple_reg_type (var)));

  /* Get the specified SSA name version.  */
  if (version != 0)
    {
      t = make_node (SSA_NAME);
      SSA_NAME_VERSION (t) = version;
      if (version >= SSANAMES (fn)->length ())
	vec_safe_grow_cleared (SSANAMES (fn), version + 1);
      gcc_assert ((*SSANAMES (fn))[version] == NULL);
      (*SSANAMES (fn))[version] = t;
      ssa_name_nodes_created++;
    }
  /* If our free list has an element, then use it.  */
  else if (!vec_safe_is_empty (FREE_SSANAMES (fn)))
    {
      t = FREE_SSANAMES (fn)->pop ();
      ssa_name_nodes_reused++;

      /* The node was cleared out when we put it on the free list, so
	 there is no need to do so again here.  */
      gcc_assert ((*SSANAMES (fn))[SSA_NAME_VERSION (t)] == NULL);
      (*SSANAMES (fn))[SSA_NAME_VERSION (t)] = t;
    }
  else
    {
      t = make_node (SSA_NAME);
      SSA_NAME_VERSION (t) = SSANAMES (fn)->length ();
      vec_safe_push (SSANAMES (fn), t);
      ssa_name_nodes_created++;
    }

  if (TYPE_P (var))
    {
      TREE_TYPE (t) = TYPE_MAIN_VARIANT (var);
      SET_SSA_NAME_VAR_OR_IDENTIFIER (t, NULL_TREE);
    }
  else
    {
      TREE_TYPE (t) = TREE_TYPE (var);
      SET_SSA_NAME_VAR_OR_IDENTIFIER (t, var);
    }
  SSA_NAME_DEF_STMT (t) = stmt;
  if (POINTER_TYPE_P (TREE_TYPE (t)))
    SSA_NAME_PTR_INFO (t) = NULL;
  else
    SSA_NAME_RANGE_INFO (t) = NULL;

  SSA_NAME_IN_FREE_LIST (t) = 0;
  SSA_NAME_IS_DEFAULT_DEF (t) = 0;
  imm = &(SSA_NAME_IMM_USE_NODE (t));
  imm->use = NULL;
  imm->prev = imm;
  imm->next = imm;
  imm->loc.ssa_name = t;

  return t;
}

/* Store range information RANGE_TYPE, MIN, and MAX to tree ssa_name NAME.  */

void
set_range_info (tree name, enum value_range_type range_type,
		const wide_int_ref &min, const wide_int_ref &max)
{
  gcc_assert (!POINTER_TYPE_P (TREE_TYPE (name)));
  gcc_assert (range_type == VR_RANGE || range_type == VR_ANTI_RANGE);
  range_info_def *ri = SSA_NAME_RANGE_INFO (name);
  unsigned int precision = TYPE_PRECISION (TREE_TYPE (name));

  /* Allocate if not available.  */
  if (ri == NULL)
    {
      size_t size = (sizeof (range_info_def)
		     + trailing_wide_ints <3>::extra_size (precision));
      ri = static_cast<range_info_def *> (ggc_internal_alloc (size));
      ri->ints.set_precision (precision);
      SSA_NAME_RANGE_INFO (name) = ri;
      ri->set_nonzero_bits (wi::shwi (-1, precision));
    }

  /* Record the range type.  */
  if (SSA_NAME_RANGE_TYPE (name) != range_type)
    SSA_NAME_ANTI_RANGE_P (name) = (range_type == VR_ANTI_RANGE);

  /* Set the values.  */
  ri->set_min (min);
  ri->set_max (max);

  /* If it is a range, try to improve nonzero_bits from the min/max.  */
  if (range_type == VR_RANGE)
    {
      wide_int xorv = ri->get_min () ^ ri->get_max ();
      if (xorv != 0)
	xorv = wi::mask (precision - wi::clz (xorv), false, precision);
      ri->set_nonzero_bits (ri->get_nonzero_bits () & (ri->get_min () | xorv));
    }
}


/* Gets range information MIN, MAX and returns enum value_range_type
   corresponding to tree ssa_name NAME.  enum value_range_type returned
   is used to determine if MIN and MAX are valid values.  */

enum value_range_type
get_range_info (const_tree name, wide_int *min, wide_int *max)
{
  gcc_assert (!POINTER_TYPE_P (TREE_TYPE (name)));
  gcc_assert (min && max);
  range_info_def *ri = SSA_NAME_RANGE_INFO (name);

  /* Return VR_VARYING for SSA_NAMEs with NULL RANGE_INFO or SSA_NAMEs
     with integral types width > 2 * HOST_BITS_PER_WIDE_INT precision.  */
  if (!ri || (GET_MODE_PRECISION (TYPE_MODE (TREE_TYPE (name)))
	      > 2 * HOST_BITS_PER_WIDE_INT))
    return VR_VARYING;

  *min = ri->get_min ();
  *max = ri->get_max ();
  return SSA_NAME_RANGE_TYPE (name);
}

/* Set nonnull attribute to pointer NAME.  */

void
set_ptr_nonnull (tree name)
{
  gcc_assert (POINTER_TYPE_P (TREE_TYPE (name)));
  struct ptr_info_def *pi = get_ptr_info (name);
  pi->pt.null = 0;
}

/* Return nonnull attribute of pointer NAME.  */
bool
get_ptr_nonnull (const_tree name)
{
  gcc_assert (POINTER_TYPE_P (TREE_TYPE (name)));
  struct ptr_info_def *pi = SSA_NAME_PTR_INFO (name);
  if (pi == NULL)
    return false;
  /* TODO Now pt->null is conservatively set to true in PTA
     analysis. vrp is the only pass (including ipa-vrp)
     that clears pt.null via set_ptr_nonull when it knows
     for sure. PTA will preserves the pt.null value set by VRP.

     When PTA analysis is improved, pt.anything, pt.nonlocal
     and pt.escaped may also has to be considered before
     deciding that pointer cannot point to NULL.  */
  return !pi->pt.null;
}

/* Change non-zero bits bitmask of NAME.  */

void
set_nonzero_bits (tree name, const wide_int_ref &mask)
{
  gcc_assert (!POINTER_TYPE_P (TREE_TYPE (name)));
  if (SSA_NAME_RANGE_INFO (name) == NULL)
    set_range_info (name, VR_RANGE,
		    TYPE_MIN_VALUE (TREE_TYPE (name)),
		    TYPE_MAX_VALUE (TREE_TYPE (name)));
  range_info_def *ri = SSA_NAME_RANGE_INFO (name);
  ri->set_nonzero_bits (mask);
}

/* Return a widest_int with potentially non-zero bits in SSA_NAME
   NAME, or -1 if unknown.  */

wide_int
get_nonzero_bits (const_tree name)
{
  unsigned int precision = TYPE_PRECISION (TREE_TYPE (name));
  if (POINTER_TYPE_P (TREE_TYPE (name)))
    {
      struct ptr_info_def *pi = SSA_NAME_PTR_INFO (name);
      if (pi && pi->align)
	return wi::shwi (-(HOST_WIDE_INT) pi->align
			 | (HOST_WIDE_INT) pi->misalign, precision);
      return wi::shwi (-1, precision);
    }

  range_info_def *ri = SSA_NAME_RANGE_INFO (name);
  if (!ri)
    return wi::shwi (-1, precision);

  return ri->get_nonzero_bits ();
}

/* Return TRUE is OP, an SSA_NAME has a range of values [0..1], false
   otherwise.

   This can be because it is a boolean type, any unsigned integral
   type with a single bit of precision, or has known range of [0..1]
   via VRP analysis.  */

bool
ssa_name_has_boolean_range (tree op)
{
  gcc_assert (TREE_CODE (op) == SSA_NAME);

  /* Boolean types always have a range [0..1].  */
  if (TREE_CODE (TREE_TYPE (op)) == BOOLEAN_TYPE)
    return true;

  /* An integral type with a single bit of precision.  */
  if (INTEGRAL_TYPE_P (TREE_TYPE (op))
      && TYPE_UNSIGNED (TREE_TYPE (op))
      && TYPE_PRECISION (TREE_TYPE (op)) == 1)
    return true;

  /* An integral type with more precision, but the object
     only takes on values [0..1] as determined by VRP
     analysis.  */
  if (INTEGRAL_TYPE_P (TREE_TYPE (op))
      && (TYPE_PRECISION (TREE_TYPE (op)) > 1)
      && wi::eq_p (get_nonzero_bits (op), 1))
    return true;

  return false;
}

/* We no longer need the SSA_NAME expression VAR, release it so that
   it may be reused.

   Note it is assumed that no calls to make_ssa_name will be made
   until all uses of the ssa name are released and that the only
   use of the SSA_NAME expression is to check its SSA_NAME_VAR.  All
   other fields must be assumed clobbered.  */

void
release_ssa_name_fn (struct function *fn, tree var)
{
  if (!var)
    return;

  /* Never release the default definition for a symbol.  It's a
     special SSA name that should always exist once it's created.  */
  if (SSA_NAME_IS_DEFAULT_DEF (var))
    return;

  /* If VAR has been registered for SSA updating, don't remove it.
     After update_ssa has run, the name will be released.  */
  if (name_registered_for_update_p (var))
    {
      release_ssa_name_after_update_ssa (var);
      return;
    }

  /* release_ssa_name can be called multiple times on a single SSA_NAME.
     However, it should only end up on our free list one time.   We
     keep a status bit in the SSA_NAME node itself to indicate it has
     been put on the free list.

     Note that once on the freelist you can not reference the SSA_NAME's
     defining statement.  */
  if (! SSA_NAME_IN_FREE_LIST (var))
    {
      tree saved_ssa_name_var = SSA_NAME_VAR (var);
      int saved_ssa_name_version = SSA_NAME_VERSION (var);
      use_operand_p imm = &(SSA_NAME_IMM_USE_NODE (var));

      if (MAY_HAVE_DEBUG_STMTS)
	insert_debug_temp_for_var_def (NULL, var);

      if (flag_checking)
	verify_imm_links (stderr, var);
      while (imm->next != imm)
	delink_imm_use (imm->next);

      (*SSANAMES (fn))[SSA_NAME_VERSION (var)] = NULL_TREE;
      memset (var, 0, tree_size (var));

      imm->prev = imm;
      imm->next = imm;
      imm->loc.ssa_name = var;

      /* First put back the right tree node so that the tree checking
	 macros do not complain.  */
      TREE_SET_CODE (var, SSA_NAME);

      /* Restore the version number.  */
      SSA_NAME_VERSION (var) = saved_ssa_name_version;

      /* Hopefully this can go away once we have the new incremental
         SSA updating code installed.  */
      SET_SSA_NAME_VAR_OR_IDENTIFIER (var, saved_ssa_name_var);

      /* Note this SSA_NAME is now in the first list.  */
      SSA_NAME_IN_FREE_LIST (var) = 1;

      /* And finally queue it so that it will be put on the free list.  */
      vec_safe_push (FREE_SSANAMES_QUEUE (fn), var);
    }
}

/* If the alignment of the pointer described by PI is known, return true and
   store the alignment and the deviation from it into *ALIGNP and *MISALIGNP
   respectively.  Otherwise return false.  */

bool
get_ptr_info_alignment (struct ptr_info_def *pi, unsigned int *alignp,
			unsigned int *misalignp)
{
  if (pi->align)
    {
      *alignp = pi->align;
      *misalignp = pi->misalign;
      return true;
    }
  else
    return false;
}

/* State that the pointer described by PI has unknown alignment.  */

void
mark_ptr_info_alignment_unknown (struct ptr_info_def *pi)
{
  pi->align = 0;
  pi->misalign = 0;
}

/* Store the power-of-two byte alignment and the deviation from that
   alignment of pointer described by PI to ALIOGN and MISALIGN
   respectively.  */

void
set_ptr_info_alignment (struct ptr_info_def *pi, unsigned int align,
			    unsigned int misalign)
{
  gcc_checking_assert (align != 0);
  gcc_assert ((align & (align - 1)) == 0);
  gcc_assert ((misalign & ~(align - 1)) == 0);

  pi->align = align;
  pi->misalign = misalign;
}

/* If pointer described by PI has known alignment, increase its known
   misalignment by INCREMENT modulo its current alignment.  */

void
adjust_ptr_info_misalignment (struct ptr_info_def *pi,
			      unsigned int increment)
{
  if (pi->align != 0)
    {
      pi->misalign += increment;
      pi->misalign &= (pi->align - 1);
    }
}

/* Return the alias information associated with pointer T.  It creates a
   new instance if none existed.  */

struct ptr_info_def *
get_ptr_info (tree t)
{
  struct ptr_info_def *pi;

  gcc_assert (POINTER_TYPE_P (TREE_TYPE (t)));

  pi = SSA_NAME_PTR_INFO (t);
  if (pi == NULL)
    {
      pi = ggc_cleared_alloc<ptr_info_def> ();
      pt_solution_reset (&pi->pt);
      mark_ptr_info_alignment_unknown (pi);
      SSA_NAME_PTR_INFO (t) = pi;
    }

  return pi;
}


/* Creates a new SSA name using the template NAME tobe defined by
   statement STMT in function FN.  */

tree
copy_ssa_name_fn (struct function *fn, tree name, gimple *stmt)
{
  tree new_name;

  if (SSA_NAME_VAR (name))
    new_name = make_ssa_name_fn (fn, SSA_NAME_VAR (name), stmt);
  else
    {
      new_name = make_ssa_name_fn (fn, TREE_TYPE (name), stmt);
      SET_SSA_NAME_VAR_OR_IDENTIFIER (new_name, SSA_NAME_IDENTIFIER (name));
    }

  return new_name;
}


/* Creates a duplicate of the ptr_info_def at PTR_INFO for use by
   the SSA name NAME.  */

void
duplicate_ssa_name_ptr_info (tree name, struct ptr_info_def *ptr_info)
{
  struct ptr_info_def *new_ptr_info;

  gcc_assert (POINTER_TYPE_P (TREE_TYPE (name)));
  gcc_assert (!SSA_NAME_PTR_INFO (name));

  if (!ptr_info)
    return;

  new_ptr_info = ggc_alloc<ptr_info_def> ();
  *new_ptr_info = *ptr_info;

  SSA_NAME_PTR_INFO (name) = new_ptr_info;
}

/* Creates a duplicate of the range_info_def at RANGE_INFO of type
   RANGE_TYPE for use by the SSA name NAME.  */
void
duplicate_ssa_name_range_info (tree name, enum value_range_type range_type,
			       struct range_info_def *range_info)
{
  struct range_info_def *new_range_info;

  gcc_assert (!POINTER_TYPE_P (TREE_TYPE (name)));
  gcc_assert (!SSA_NAME_RANGE_INFO (name));

  if (!range_info)
    return;

  unsigned int precision = TYPE_PRECISION (TREE_TYPE (name));
  size_t size = (sizeof (range_info_def)
		 + trailing_wide_ints <3>::extra_size (precision));
  new_range_info = static_cast<range_info_def *> (ggc_internal_alloc (size));
  memcpy (new_range_info, range_info, size);

  gcc_assert (range_type == VR_RANGE || range_type == VR_ANTI_RANGE);
  SSA_NAME_ANTI_RANGE_P (name) = (range_type == VR_ANTI_RANGE);
  SSA_NAME_RANGE_INFO (name) = new_range_info;
}



/* Creates a duplicate of a ssa name NAME tobe defined by statement STMT
   in function FN.  */

tree
duplicate_ssa_name_fn (struct function *fn, tree name, gimple *stmt)
{
  tree new_name = copy_ssa_name_fn (fn, name, stmt);
  if (POINTER_TYPE_P (TREE_TYPE (name)))
    {
      struct ptr_info_def *old_ptr_info = SSA_NAME_PTR_INFO (name);

      if (old_ptr_info)
	duplicate_ssa_name_ptr_info (new_name, old_ptr_info);
    }
  else
    {
      struct range_info_def *old_range_info = SSA_NAME_RANGE_INFO (name);

      if (old_range_info)
	duplicate_ssa_name_range_info (new_name, SSA_NAME_RANGE_TYPE (name),
				       old_range_info);
    }

  return new_name;
}


/* Reset all flow sensitive data on NAME such as range-info, nonzero
   bits and alignment.  */

void
reset_flow_sensitive_info (tree name)
{
  if (POINTER_TYPE_P (TREE_TYPE (name)))
    {
      /* points-to info is not flow-sensitive.  */
      if (SSA_NAME_PTR_INFO (name))
	{
	  /* [E]VRP can derive context sensitive alignment info and
	     non-nullness properties.  We must reset both.  */
	  mark_ptr_info_alignment_unknown (SSA_NAME_PTR_INFO (name));
	  SSA_NAME_PTR_INFO (name)->pt.null = 1;
	}
    }
  else
    SSA_NAME_RANGE_INFO (name) = NULL;
}

/* Clear all flow sensitive data from all statements and PHI definitions
   in BB.  */

void
reset_flow_sensitive_info_in_bb (basic_block bb)
{
  for (gimple_stmt_iterator gsi = gsi_start_bb (bb); !gsi_end_p (gsi);
       gsi_next (&gsi))
    {
      gimple *stmt = gsi_stmt (gsi);
      ssa_op_iter i;
      tree op;
      FOR_EACH_SSA_TREE_OPERAND (op, stmt, i, SSA_OP_DEF)
	reset_flow_sensitive_info (op);
    }

  for (gphi_iterator gsi = gsi_start_phis (bb); !gsi_end_p (gsi);
       gsi_next (&gsi))
    {
      tree phi_def = gimple_phi_result (gsi.phi ());
      reset_flow_sensitive_info (phi_def);
    }
}

/* Release all the SSA_NAMEs created by STMT.  */

void
release_defs (gimple *stmt)
{
  tree def;
  ssa_op_iter iter;

  FOR_EACH_SSA_TREE_OPERAND (def, stmt, iter, SSA_OP_ALL_DEFS)
    if (TREE_CODE (def) == SSA_NAME)
      release_ssa_name (def);
}


/* Replace the symbol associated with SSA_NAME with SYM.  */

void
replace_ssa_name_symbol (tree ssa_name, tree sym)
{
  SET_SSA_NAME_VAR_OR_IDENTIFIER (ssa_name, sym);
  TREE_TYPE (ssa_name) = TREE_TYPE (sym);
}

/* Release the vector of free SSA_NAMEs and compact the vector of SSA_NAMEs
   that are live.  */

static void
release_free_names_and_compact_live_names (function *fun)
{
  unsigned i, j;
  int n = vec_safe_length (FREE_SSANAMES (fun));

  /* Now release the freelist.  */
  vec_free (FREE_SSANAMES (fun));

  /* And compact the SSA number space.  We make sure to not change the
     relative order of SSA versions.  */
  for (i = 1, j = 1; i < fun->gimple_df->ssa_names->length (); ++i)
    {
      tree name = ssa_name (i);
      if (name)
	{
	  if (i != j)
	    {
	      SSA_NAME_VERSION (name) = j;
	      (*fun->gimple_df->ssa_names)[j] = name;
	    }
	  j++;
	}
    }
  fun->gimple_df->ssa_names->truncate (j);

  statistics_counter_event (fun, "SSA names released", n);
  statistics_counter_event (fun, "SSA name holes removed", i - j);
  if (dump_file)
    fprintf (dump_file, "Released %i names, %.2f%%, removed %i holes\n",
	     n, n * 100.0 / num_ssa_names, i - j);
}

/* Return SSA names that are unused to GGC memory and compact the SSA
   version namespace.  This is used to keep footprint of compiler during
   interprocedural optimization.  */

namespace {

const pass_data pass_data_release_ssa_names =
{
  GIMPLE_PASS, /* type */
  "release_ssa", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_TREE_SSA_OTHER, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  TODO_remove_unused_locals, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_release_ssa_names : public gimple_opt_pass
{
public:
  pass_release_ssa_names (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_release_ssa_names, ctxt)
  {}

  /* opt_pass methods: */
  virtual unsigned int execute (function *);

}; // class pass_release_ssa_names

unsigned int
pass_release_ssa_names::execute (function *fun)
{
  release_free_names_and_compact_live_names (fun);
  return 0;
}

} // anon namespace

gimple_opt_pass *
make_pass_release_ssa_names (gcc::context *ctxt)
{
  return new pass_release_ssa_names (ctxt);
}
