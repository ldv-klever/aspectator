/* Solaris 10 configuration.
   Copyright (C) 2004, 2006, 2007, 2008, 2009, 2010, 2011
   Free Software Foundation, Inc.
   Contributed by CodeSourcery, LLC.

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

#undef ASM_COMMENT_START
#define ASM_COMMENT_START "/"

/* binutils' GNU as understands --32 and --64, but the native Solaris
   assembler requires -xarch=generic or -xarch=generic64 instead.  */
#undef ASM_SPEC
#ifdef USE_GAS
#define ASM_SPEC "%{m32:--32} %{m64:--64} -s %(asm_cpu)"
#else
#define ASM_SPEC "%{v:-V} %{Qy:} %{!Qn:-Qy} %{Ym,*} " \
		 "%{m32:-xarch=generic} %{m64:-xarch=generic64} " \
		 "-s %(asm_cpu)"
#endif

/* The native Solaris assembler can't calculate the difference between
   symbols in different sections, which causes problems for -fPIC jump
   tables in .rodata.  */
#ifndef HAVE_AS_IX86_DIFF_SECT_DELTA
#undef JUMP_TABLES_IN_TEXT_SECTION
#define JUMP_TABLES_IN_TEXT_SECTION 1

/* The native Solaris assembler cannot handle the SYMBOL-. syntax, but
   requires SYMBOL@rel/@rel64 instead.  */
#define ASM_OUTPUT_DWARF_PCREL(FILE, SIZE, LABEL)	\
  do {							\
    fputs (integer_asm_op (SIZE, FALSE), FILE);		\
    assemble_name (FILE, LABEL);			\
    fputs (SIZE == 8 ? "@rel64" : "@rel", FILE);	\
  } while (0)
#endif

/* As in sol2.h, override the default from i386/x86-64.h to work around
   Sun as TLS bug.  */
#undef  ASM_OUTPUT_ALIGNED_COMMON
#define ASM_OUTPUT_ALIGNED_COMMON(FILE, NAME, SIZE, ALIGN)		\
  do									\
    {									\
      if (TARGET_SUN_TLS						\
	  && in_section							\
	  && ((in_section->common.flags & SECTION_TLS) == SECTION_TLS))	\
	switch_to_section (bss_section);				\
      x86_elf_aligned_common (FILE, NAME, SIZE, ALIGN);			\
    }									\
  while  (0)

#undef NO_PROFILE_COUNTERS

#undef MCOUNT_NAME
#define MCOUNT_NAME "_mcount"

#undef WCHAR_TYPE
#define WCHAR_TYPE (TARGET_64BIT ? "int" : "long int")
#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 32

#undef WINT_TYPE
#define WINT_TYPE (TARGET_64BIT ? "int" : "long int")
#undef WINT_TYPE_SIZE
#define WINT_TYPE_SIZE 32

#define USE_IX86_FRAME_POINTER 1
#define USE_X86_64_FRAME_POINTER 1

/* Override i386/sol2.h version: return 8-byte vectors in MMX registers if
   possible, matching Sun Studio 12 Update 1+ compilers and other x86
   targets.  */
#undef TARGET_SUBTARGET_DEFAULT
#define TARGET_SUBTARGET_DEFAULT \
	(MASK_80387 | MASK_IEEE_FP | MASK_FLOAT_RETURNS)

#define SUBTARGET_OPTIMIZATION_OPTIONS				\
  { OPT_LEVELS_1_PLUS, OPT_momit_leaf_frame_pointer, NULL, 1 }

#define MULTILIB_DEFAULTS { "m32" }

#undef LINK_ARCH64_SPEC_BASE
#define LINK_ARCH64_SPEC_BASE \
  "%{G:-G} \
   %{YP,*} \
   %{R*} \
   %{compat-bsd: \
     %{!YP,*:%{p|pg:-Y P,/usr/ucblib/64:/usr/lib/libp/64:/lib/64:/usr/lib/64} \
             %{!p:%{!pg:-Y P,/usr/ucblib/64:/lib:/usr/lib/64}}} \
             -R /usr/ucblib/64} \
   %{!compat-bsd: \
     %{!YP,*:%{p|pg:-Y P,/usr/lib/libp/64:/lib/64:/usr/lib/64} \
             %{!p:%{!pg:-Y P,/lib/64:/usr/lib/64}}}}"

#undef LINK_ARCH64_SPEC
#define LINK_ARCH64_SPEC LINK_ARCH64_SPEC_BASE

#ifdef TARGET_GNU_LD
/* Since binutils 2.21, GNU ld supports new *_sol2 emulations to strictly
   follow the Solaris 2 ABI.  Prefer them if present.  */
#ifdef HAVE_LD_SOL2_EMULATION
#define I386_EMULATION "elf_i386_sol2"
#define X86_64_EMULATION "elf_x86_64_sol2"
#else
#define I386_EMULATION "elf_i386"
#define X86_64_EMULATION "elf_x86_64"
#endif

#define TARGET_LD_EMULATION "%{m64:-m " X86_64_EMULATION "}" \
			    "%{!m64:-m " I386_EMULATION "} "
#else
#define TARGET_LD_EMULATION ""
#endif

#undef LINK_ARCH_SPEC
#define LINK_ARCH_SPEC TARGET_LD_EMULATION \
		       "%{m64:" LINK_ARCH64_SPEC "}%{!m64:" LINK_ARCH32_SPEC "}"

/* We do not need to search a special directory for startup files.  */
#undef MD_STARTFILE_PREFIX

#undef TARGET_ASM_NAMED_SECTION
#define TARGET_ASM_NAMED_SECTION i386_solaris_elf_named_section
