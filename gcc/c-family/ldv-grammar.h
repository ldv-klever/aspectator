/* Copyright (C) 2010-2012
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

#ifndef LDV_GRAMMAR
#define LDV_GRAMMAR

/* This is a constuction needed for #line directives generation. It doesn't.
   belong to the C grammar but is related with a lot of entities (expressions,
   statements and declarations). */
struct ldv_location
{
  const char *file;
  unsigned int line;
};
typedef struct ldv_location *ldv_location_ptr;
#define LDV_LOCATION_FILE(location) (location->file)
#define LDV_LOCATION_LINE(location) (location->line)

/* String literal is considered as the sequence of symbols.
string-literal:
    " s-char-sequenceopt "
    L" s-char-sequenceopt "
*/
struct ldv_str_literal
{
  const char *str;
};
typedef struct ldv_str_literal *ldv_str_literal_ptr;
#define LDV_STR_LITERAL_STR(str_literal) (str_literal->str)

/* Identifier is considered as the sequence of nondigit and digit symbols.
identifier:
    identifier-nondigit
    identifier identifier-nondigit
    identifier digit
*/
struct ldv_identifier
{
  char *str;
};
typedef struct ldv_identifier *ldv_identifier_ptr;
#define LDV_IDENTIFIER_STR(identifier) (identifier->str)

/*
enumeration-constant:
    identiﬁer
*/
struct ldv_enumeration_constant
{
  ldv_identifier_ptr identifier;
};
typedef struct ldv_enumeration_constant *ldv_enumeration_constant_ptr;
#define LDV_ENUMERATION_CONSTANT_IDENTIFIER(enumeration_constant) (enumeration_constant->identifier)

/* ! NOT FINISHED ! Floating constant is treated as a string.
floating-constant:
    decimal-floating-constant
    hexadecimal-floating-constant
*/
enum ldv_floating_constant_kind
{
  LDV_FLOATING_CONSTANT_FIRST = 1,
  LDV_FLOATING_CONSTANT_SECOND,
};
struct ldv_floating_constant
{
  enum ldv_floating_constant_kind kind;
  char cst[100];
};
typedef struct ldv_floating_constant *ldv_floating_constant_ptr;
#define LDV_FLOATING_CONSTANT_KIND(floating_constant) (floating_constant->kind)
#define LDV_FLOATING_CONSTANT_CST(floating_constant)  (floating_constant->cst)

/*
long-long-suffix: one of
    ll LL
*/
enum ldv_long_long_suffix_kind
{
  LDV_LONG_LONG_SUFFIX_FIRST = 1,
  LDV_LONG_LONG_SUFFIX_SECOND
};
struct ldv_long_long_suffix
{
  enum ldv_long_long_suffix_kind kind;
};
typedef struct ldv_long_long_suffix *ldv_long_long_suffix_ptr;
#define LDV_LONG_LONG_SUFFIX_KIND(long_long_suffix)             (long_long_suffix->kind)

/*
long-suffix: one of
    l L
*/
enum ldv_long_suffix_kind
{
  LDV_LONG_SUFFIX_FIRST = 1,
  LDV_LONG_SUFFIX_SECOND
};
struct ldv_long_suffix
{
  enum ldv_long_suffix_kind kind;
};
typedef struct ldv_long_suffix *ldv_long_suffix_ptr;
#define LDV_LONG_SUFFIX_KIND(long_suffix)             (long_suffix->kind)

/*
unsigned-suffix: one of
    u U
*/
enum ldv_unsigned_suffix_kind
{
  LDV_UNSIGNED_SUFFIX_FIRST = 1,
  LDV_UNSIGNED_SUFFIX_SECOND
};
struct ldv_unsigned_suffix
{
  enum ldv_unsigned_suffix_kind kind;
};
typedef struct ldv_unsigned_suffix *ldv_unsigned_suffix_ptr;
#define LDV_UNSIGNED_SUFFIX_KIND(unsigned_suffix)             (unsigned_suffix->kind)

/*
integer-suffix:
    unsigned-suffix long-suffixopt
    unsigned-suffix long-long-suffix
    long-suffix unsigned-suffixopt
    long-long-suffix unsigned-suffixopt
*/
enum ldv_integer_suffix_kind
{
  LDV_INTEGER_SUFFIX_FIRST = 1,
  LDV_INTEGER_SUFFIX_SECOND,
  LDV_INTEGER_SUFFIX_THIRD,
  LDV_INTEGER_SUFFIX_FOURTH
};
struct ldv_integer_suffix
{
  enum ldv_integer_suffix_kind kind;
  ldv_unsigned_suffix_ptr unsigned_suffix;
  ldv_long_suffix_ptr long_suffix;
  ldv_long_long_suffix_ptr long_long_suffix;
};
typedef struct ldv_integer_suffix *ldv_integer_suffix_ptr;
#define LDV_INTEGER_SUFFIX_KIND(integer_suffix)             (integer_suffix->kind)
#define LDV_INTEGER_SUFFIX_UNSIGNED_SUFFIX(integer_suffix)  (integer_suffix->unsigned_suffix)
#define LDV_INTEGER_SUFFIX_LONG_SUFFIX(integer_suffix)      (integer_suffix->long_suffix)
#define LDV_INTEGER_SUFFIX_LONG_LONG_SUFFIX(integer_suffix) (integer_suffix->long_long_suffix)

/* All integer constants are represented as signed HOST_WIDE_INT. So for example
   large unsigned long constants become negative numbers. Unsigned constants
   require the integer suffix to be printed. Other integer constants may be
   printed either with or without suffixes.
integer-constant:
    decimal-constant integer-suffixopt
    octal-constant integer-suffixopt
    hexadecimal-constant integer-suffixopt
*/
enum ldv_integer_constant_kind
{
  LDV_INTEGER_CONSTANT_FIRST = 1,
  LDV_INTEGER_CONSTANT_SECOND,
  LDV_INTEGER_CONSTANT_THIRD
};
struct ldv_integer_constant
{
  enum ldv_integer_constant_kind kind;
  HOST_WIDE_INT decimal_constant;
  bool issigned;
  ldv_integer_suffix_ptr integer_suffix;
};
typedef struct ldv_integer_constant *ldv_integer_constant_ptr;
#define LDV_INTEGER_CONSTANT_KIND(integer_constant)              (integer_constant->kind)
#define LDV_INTEGER_CONSTANT_DECIMAL_CONSTANT(integer_constant)  (integer_constant->decimal_constant)
#define LDV_INTEGER_CONSTANT_ISSIGNED(integer_constant)          (integer_constant->issigned)
#define LDV_INTEGER_CONSTANT_INTEGER_SUFFIX(integer_constant)    (integer_constant->integer_suffix)

/* ! NOT FINISHED ! Character constants replaced with int one... see pp_c_character_constant
constant:
    integer-constant
    floating-constant
    enumeration-constant
    character-constant
*/
enum ldv_constant_kind
{
  LDV_CONSTANT_FIRST = 1,
  LDV_CONSTANT_SECOND,
  LDV_CONSTANT_THIRD,
  LDV_CONSTANT_FOURTH
};
struct ldv_constant
{
  enum ldv_constant_kind kind;
  ldv_integer_constant_ptr integer_constant;
  ldv_floating_constant_ptr floating_constant;
  ldv_enumeration_constant_ptr enumeration_constant;
};
typedef struct ldv_constant *ldv_constant_ptr;
#define LDV_CONSTANT_KIND(constant)                 (constant->kind)
#define LDV_CONSTANT_INTEGER_CONSTANT(constant)     (constant->integer_constant)
#define LDV_CONSTANT_FLOATING_CONSTANT(constant)    (constant->floating_constant)
#define LDV_CONSTANT_ENUMERATION_CONSTANT(constant) (constant->enumeration_constant)

/*
primary-expression:
    identifier
    constant
    string-literal
    ( expression )

GNU extension

primary-expression:
    ( compound-statement )
    __builtin_va_arg ( assignment-expression , type-name )
*/
enum ldv_primary_expr_kind
{
  LDV_PRIMARY_EXPR_FIRST = 1,
  LDV_PRIMARY_EXPR_SECOND,
  LDV_PRIMARY_EXPR_THIRD,
  LDV_PRIMARY_EXPR_FOURTH,
  LDV_PRIMARY_EXPR_FIFTH,
  LDV_PRIMARY_EXPR_SIXTH
};
struct ldv_primary_expr
{
  enum ldv_primary_expr_kind kind;
  ldv_identifier_ptr identifier;
  ldv_constant_ptr constant;
  ldv_str_literal_ptr str_literal;
  struct ldv_expr *expr;
  struct ldv_compound_statement *compound_statement;
  struct ldv_assignment_expr *assignment_expr;
  struct ldv_type_name *type_name;
};
typedef struct ldv_primary_expr *ldv_primary_expr_ptr;
#define LDV_PRIMARY_EXPR_KIND(primary_expr)               (primary_expr->kind)
#define LDV_PRIMARY_EXPR_IDENTIFIER(primary_expr)         (primary_expr->identifier)
#define LDV_PRIMARY_EXPR_CONSTANT(primary_expr)           (primary_expr->constant)
#define LDV_PRIMARY_EXPR_STR_LITERAL(primary_expr)        (primary_expr->str_literal)
#define LDV_PRIMARY_EXPR_EXPR(primary_expr)               (primary_expr->expr)
#define LDV_PRIMARY_EXPR_COMPOUND_STATEMENT(primary_expr) (primary_expr->compound_statement)
#define LDV_PRIMARY_EXPR_ASSIGNMENT_EXPR(primary_expr)    (primary_expr->assignment_expr)
#define LDV_PRIMARY_EXPR_TYPE_NAME(primary_expr)          (primary_expr->type_name)

/*
designator:
    [ constant-expression ]
    . identifier
*/
enum ldv_designator_kind
{
  LDV_DESIGNATOR_FIRST = 1,
  LDV_DESIGNATOR_SECOND
};
struct ldv_designator
{
  enum ldv_designator_kind kind;
  struct ldv_const_expr *const_expr;
  ldv_identifier_ptr identifier;
};
typedef struct ldv_designator *ldv_designator_ptr;
#define LDV_DESIGNATOR_KIND(designator)       (designator->kind)
#define LDV_DESIGNATOR_CONST_EXPR(designator) (designator->const_expr)
#define LDV_DESIGNATOR_IDENTIFIER(designator) (designator->identifier)

/*
designator-list:
    designator
    designator-list designator
*/
enum ldv_designator_list_kind
{
  LDV_DESIGNATOR_LIST_FIRST = 1,
  LDV_DESIGNATOR_LIST_SECOND
};
struct ldv_designator_list
{
  enum ldv_designator_list_kind kind;
  ldv_designator_ptr designator;
  struct ldv_designator_list *designator_list;
};
typedef struct ldv_designator_list *ldv_designator_list_ptr;
#define LDV_DESIGNATOR_LIST_KIND(designator_list)                (designator_list->kind)
#define LDV_DESIGNATOR_LIST_DESIGNATOR(designator_list)          (designator_list->designator)
#define LDV_DESIGNATOR_LIST_DESIGNATOR_LIST(designator_list_arg) (designator_list_arg->designator_list)

/*
designation:
    designator-list =
*/
struct ldv_designation
{
  ldv_designator_list_ptr designator_list;
};
typedef struct ldv_designation *ldv_designation_ptr;
#define LDV_DESIGNATION_DESIGNATOR_LIST(designation) (designation->designator_list)

/*
initializer-list:
    designationopt initializer
    initializer-list , designationopt initializer
*/
enum ldv_initializer_list_kind
{
  LDV_INITIALIZER_LIST_FIRST = 1,
  LDV_INITIALIZER_LIST_SECOND
};
struct ldv_initializer_list
{
  enum ldv_initializer_list_kind kind;
  ldv_designation_ptr designation;
  struct ldv_initializer *initializer;
  struct ldv_initializer_list *initializer_list;
};
typedef struct ldv_initializer_list *ldv_initializer_list_ptr;
#define LDV_INITIALIZER_LIST_KIND(initializer_list)                 (initializer_list->kind)
#define LDV_INITIALIZER_LIST_DESIGNATION(initializer_list)          (initializer_list->designation)
#define LDV_INITIALIZER_LIST_INITIALIZER(initializer_list)          (initializer_list->initializer)
#define LDV_INITIALIZER_LIST_INITIALIZER_LIST(initializer_list_arg) (initializer_list_arg->initializer_list)

/*
type-name:
    specifier-qualifier-list abstract-declaratoropt
*/
struct ldv_type_name
{
  struct ldv_spec_qual_list *spec_qual_list;
  struct ldv_abstract_declarator *abstract_declarator;
};
typedef struct ldv_type_name *ldv_type_name_ptr;
#define LDV_TYPE_NAME_SPEC_QUAL_LIST(type_name)      (type_name->spec_qual_list)
#define LDV_TYPE_NAME_ABSTRACT_DECLARATOR(type_name) (type_name->abstract_declarator)

/*
argument-expression-list:
    assignment-expression
    argument-expression-list , assignment-expression
*/
enum ldv_arg_expr_list_kind
{
  LDV_ARG_EXPR_LIST_FIRST = 1,
  LDV_ARG_EXPR_LIST_SECOND
};
struct ldv_arg_expr_list
{
  enum ldv_arg_expr_list_kind kind;
  struct ldv_assignment_expr *assignment_expr;
  struct ldv_arg_expr_list *arg_expr_list;
};
typedef struct ldv_arg_expr_list *ldv_arg_expr_list_ptr;
#define LDV_ARG_EXPR_LIST_KIND(arg_expr_list)              (arg_expr_list->kind)
#define LDV_ARG_EXPR_LIST_ASSIGNEMT_EXPR(arg_expr_list)    (arg_expr_list->assignment_expr)
#define LDV_ARG_EXPR_LIST_ARG_EXPR_LIST(arg_expr_list_arg) (arg_expr_list_arg->arg_expr_list)

/* There is no '->' in gcc.
postfix-expression:
    primary-expression
    postfix-expression [ expression ]
    postfix-expression ( argument-expression-listopt )
    postfix-expression . identifier
    postfix-expression -> identifier
    postfix-expression ++
    postfix-expression --
    ( type-name ) { initializer-list }
    ( type-name ) { initializer-list , }
*/
enum ldv_postfix_expr_kind
{
  LDV_POSTFIX_EXPR_FIRST = 1,
  LDV_POSTFIX_EXPR_SECOND,
  LDV_POSTFIX_EXPR_THIRD,
  LDV_POSTFIX_EXPR_FOURTH,
  LDV_POSTFIX_EXPR_FIFTH,
  LDV_POSTFIX_EXPR_SIXTH,
  LDV_POSTFIX_EXPR_SEVENTH,
  LDV_POSTFIX_EXPR_EIGHTH
};
struct ldv_postfix_expr
{
  enum ldv_postfix_expr_kind kind;
  ldv_primary_expr_ptr primary_expr;
  struct ldv_postfix_expr *postfix_expr;
  struct ldv_expr *expr;
  ldv_arg_expr_list_ptr arg_expr_list;
  ldv_identifier_ptr identifier;
  ldv_type_name_ptr type_name;
  ldv_initializer_list_ptr initializer_list;
  ldv_location_ptr location;
};
typedef struct ldv_postfix_expr *ldv_postfix_expr_ptr;
#define LDV_POSTFIX_EXPR_KIND(equality_expr)            (postfix_expr->kind)
#define LDV_POSTFIX_EXPR_PRIMARY_EXPR(postfix_expr)     (postfix_expr->primary_expr)
#define LDV_POSTFIX_EXPR_POSTFIX_EXPR(postfix_expr_arg) (postfix_expr_arg->postfix_expr)
#define LDV_POSTFIX_EXPR_EXPR(postfix_expr)             (postfix_expr->expr)
#define LDV_POSTFIX_EXPR_ARG_EXPR_LIST(postfix_expr)    (postfix_expr->arg_expr_list)
#define LDV_POSTFIX_EXPR_IDENTIFIER(postfix_expr)       (postfix_expr->identifier)
#define LDV_POSTFIX_EXPR_TYPE_NAME(postfix_expr)        (postfix_expr->type_name)
#define LDV_POSTFIX_EXPR_INITIALIZER_LIST(postfix_expr) (postfix_expr->initializer_list)
#define LDV_POSTFIX_EXPR_LOCATION(postfix_expr)         (postfix_expr->location)

/*
unary-operator: one of
    & * + - ~ !
*/
enum ldv_unary_operator_kind
{
  LDV_UNARY_OPERATOR_FIRST = 1,
  LDV_UNARY_OPERATOR_SECOND,
  LDV_UNARY_OPERATOR_THIRD,
  LDV_UNARY_OPERATOR_FOURTH,
  LDV_UNARY_OPERATOR_FIVTH,
  LDV_UNARY_OPERATOR_SIXTH
};
struct ldv_unary_operator
{
  enum ldv_unary_operator_kind kind;
};
typedef struct ldv_unary_operator *ldv_unary_operator_ptr;
#define LDV_UNARY_OPERATOR_KIND(unary_operator) (unary_operator->kind)

/* There is no sizeof constructions in gcc tree.
unary-expression:
    postfix-expression
    ++ unary-expression
    -- unary-expression
    unary-operator cast-expression
    sizeof unary-expression
    sizeof ( type-name )

GNU extension

    && identifier
*/
enum ldv_unary_expr_kind
{
  LDV_UNARY_EXPR_FIRST = 1,
  LDV_UNARY_EXPR_SECOND,
  LDV_UNARY_EXPR_THIRD,
  LDV_UNARY_EXPR_FOURTH,
  LDV_UNARY_EXPR_FIFTH
};
struct ldv_unary_expr
{
  enum ldv_unary_expr_kind kind;
  ldv_postfix_expr_ptr postfix_expr;
  struct ldv_unary_expr *unary_expr;
  ldv_unary_operator_ptr unary_operator;
  struct ldv_cast_expr *cast_expr;
  ldv_location_ptr location;
  ldv_identifier_ptr identifier;
};
typedef struct ldv_unary_expr *ldv_unary_expr_ptr;
#define LDV_UNARY_EXPR_KIND(unary_expr)           (unary_expr->kind)
#define LDV_UNARY_EXPR_POSTFIX_EXPR(unary_expr)   (unary_expr->postfix_expr)
#define LDV_UNARY_EXPR_UNARY_EXPR(unary_expr_arg) (unary_expr_arg->unary_expr)
#define LDV_UNARY_EXPR_UNARY_OPERATOR(unary_expr) (unary_expr->unary_operator)
#define LDV_UNARY_EXPR_CAST_EXPR(unary_expr)      (unary_expr->cast_expr)
#define LDV_UNARY_EXPR_IDENTIFIER(unary_expr)     (unary_expr->identifier)
#define LDV_UNARY_EXPR_LOCATION(unary_expr)       (unary_expr->location)

/*
cast-expression:
    unary-expression
    ( type-name ) cast-expression
*/
enum ldv_cast_expr_kind
{
  LDV_CAST_EXPR_FIRST = 1,
  LDV_CAST_EXPR_SECOND
};
struct ldv_cast_expr
{
  enum ldv_cast_expr_kind kind;
  ldv_unary_expr_ptr unary_expr;
  ldv_type_name_ptr type_name;
  struct ldv_cast_expr *cast_expr;
  ldv_location_ptr location;
};
typedef struct ldv_cast_expr *ldv_cast_expr_ptr;
#define LDV_CAST_EXPR_KIND(cast_expr)          (cast_expr->kind)
#define LDV_CAST_EXPR_UNARY_EXPR(cast_expr)    (cast_expr->unary_expr)
#define LDV_CAST_EXPR_TYPE_NAME(cast_expr)     (cast_expr->type_name)
#define LDV_CAST_EXPR_CAST_EXPR(cast_expr_arg) (cast_expr_arg->cast_expr)
#define LDV_CAST_EXPR_LOCATION(cast_expr)      (cast_expr->location)

/*
multiplicative-expression:
    cast-expression
    multiplicative-expression * cast-expression
    multiplicative-expression / cast-expression
    multiplicative-expression % cast-expression
*/
enum ldv_multiplicative_expr_kind
{
  LDV_MULTIPLICATIVE_EXPR_FIRST = 1,
  LDV_MULTIPLICATIVE_EXPR_SECOND,
  LDV_MULTIPLICATIVE_EXPR_THIRD,
  LDV_MULTIPLICATIVE_EXPR_FOURTH
};
struct ldv_multiplicative_expr
{
  enum ldv_multiplicative_expr_kind kind;
  ldv_cast_expr_ptr cast_expr;
  struct ldv_multiplicative_expr *multiplicative_expr;
  ldv_location_ptr location;
};
typedef struct ldv_multiplicative_expr *ldv_multiplicative_expr_ptr;
#define LDV_MULTIPLICATIVE_EXPR_KIND(multiplicative_expr)                    (multiplicative_expr->kind)
#define LDV_MULTIPLICATIVE_EXPR_CAST_EXPR(multiplicative_expr)               (multiplicative_expr->cast_expr)
#define LDV_MULTIPLICATIVE_EXPR_MULTIPLICATIVE_EXPR(multiplicative_expr_arg) (multiplicative_expr_arg->multiplicative_expr)
#define LDV_MULTIPLICATIVE_EXPR_LOCATION(multiplicative_expr)                (multiplicative_expr->location)

/*
additive-expression:
    multiplicative-expression
    additive-expression + multiplicative-expression
    additive-expression - multiplicative-expression
*/
enum ldv_additive_expr_kind
{
  LDV_ADDITIVE_EXPR_FIRST = 1,
  LDV_ADDITIVE_EXPR_SECOND,
  LDV_ADDITIVE_EXPR_THIRD
};
struct ldv_additive_expr
{
  enum ldv_additive_expr_kind kind;
  ldv_multiplicative_expr_ptr multiplicative_expr;
  struct ldv_additive_expr *additive_expr;
  ldv_location_ptr location;
};
typedef struct ldv_additive_expr *ldv_additive_expr_ptr;
#define LDV_ADDITIVE_EXPR_KIND(additive_expr)                (additive_expr->kind)
#define LDV_ADDITIVE_EXPR_MULTIPLICATIVE_EXPR(additive_expr) (additive_expr->multiplicative_expr)
#define LDV_ADDITIVE_EXPR_ADDITIVE_EXPR(additive_expr_arg)   (additive_expr_arg->additive_expr)
#define LDV_ADDITIVE_EXPR_LOCATION(additive_expr)            (additive_expr->location)

/*
shift-expression:
    additive-expression
    shift-expression << additive-expression
    shift-expression >> additive-expression

LDV extension

shift-expression:
    LDV_LROTATE (shift-expression, additive-expression)
    LDV_RROTATE (shift-expression, additive-expression)
*/
enum ldv_shift_expr_kind
{
  LDV_SHIFT_EXPR_FIRST = 1,
  LDV_SHIFT_EXPR_SECOND,
  LDV_SHIFT_EXPR_THIRD,
  LDV_SHIFT_EXPR_FOURTH,
  LDV_SHIFT_EXPR_FIFTH
};
struct ldv_shift_expr
{
  enum ldv_shift_expr_kind kind;
  ldv_additive_expr_ptr additive_expr;
  struct ldv_shift_expr *shift_expr;
  ldv_location_ptr location;
};
typedef struct ldv_shift_expr *ldv_shift_expr_ptr;
#define LDV_SHIFT_EXPR_KIND(shift_expr)           (shift_expr->kind)
#define LDV_SHIFT_EXPR_ADDITIVE_EXPR(shift_expr)  (shift_expr->additive_expr)
#define LDV_SHIFT_EXPR_SHIFT_EXPR(shift_expr_arg) (shift_expr_arg->shift_expr)
#define LDV_SHIFT_EXPR_LOCATION(shift_expr)       (shift_expr->location)

/*
relational-expression:
    shift-expression
    relational-expression < shift-expression
    relational-expression > shift-expression
    relational-expression <= shift-expression
    relational-expression >= shift-expression
*/
enum ldv_relational_expr_kind
{
  LDV_RELATIONAL_EXPR_FIRST = 1,
  LDV_RELATIONAL_EXPR_SECOND,
  LDV_RELATIONAL_EXPR_THIRD,
  LDV_RELATIONAL_EXPR_FOURTH,
  LDV_RELATIONAL_EXPR_FIFTH
};
struct ldv_relational_expr
{
  enum ldv_relational_expr_kind kind;
  ldv_shift_expr_ptr shift_expr;
  struct ldv_relational_expr *relational_expr;
  ldv_location_ptr location;
};
typedef struct ldv_relational_expr *ldv_relational_expr_ptr;
#define LDV_RELATIONAL_EXPR_KIND(relational_expr)                (relational_expr->kind)
#define LDV_RELATIONAL_EXPR_SHIFT_EXPR(relational_expr)          (relational_expr->shift_expr)
#define LDV_RELATIONAL_EXPR_RELATIONAL_EXPR(relational_expr_arg) (relational_expr_arg->relational_expr)
#define LDV_RELATIONAL_EXPR_LOCATION(relational_expr)            (relational_expr->location)

/*
equality-expression:
    relational-expression
    equality-expression == relational-expression
    equality-expression != relational-expression
*/
enum ldv_equality_expr_kind
{
  LDV_EQUALITY_EXPR_FIRST = 1,
  LDV_EQUALITY_EXPR_SECOND,
  LDV_EQUALITY_EXPR_THIRD
};
struct ldv_equality_expr
{
  enum ldv_equality_expr_kind kind;
  ldv_relational_expr_ptr relational_expr;
  struct ldv_equality_expr *equality_expr;
  ldv_location_ptr location;
};
typedef struct ldv_equality_expr *ldv_equality_expr_ptr;
#define LDV_EQUALITY_EXPR_KIND(equality_expr)              (equality_expr->kind)
#define LDV_EQUALITY_EXPR_RELATIONAL_EXPR(equality_expr)   (equality_expr->relational_expr)
#define LDV_EQUALITY_EXPR_EQUALITY_EXPR(equality_expr_arg) (equality_expr_arg->equality_expr)
#define LDV_EQUALITY_EXPR_LOCATION(equality_expr)          (equality_expr->location)

/*
AND-expression:
    equality-expression
    AND-expression & equality-expression
*/
enum ldv_and_expr_kind
{
  LDV_AND_EXPR_FIRST = 1,
  LDV_AND_EXPR_SECOND
};
struct ldv_and_expr
{
  enum ldv_and_expr_kind kind;
  ldv_equality_expr_ptr equality_expr;
  struct ldv_and_expr *and_expr;
  ldv_location_ptr location;
};
typedef struct ldv_and_expr *ldv_and_expr_ptr;
#define LDV_AND_EXPR_KIND(and_expr)          (and_expr->kind)
#define LDV_AND_EXPR_EQUALITY_EXPR(and_expr) (and_expr->equality_expr)
#define LDV_AND_EXPR_AND_EXPR(and_expr_arg)  (and_expr_arg->and_expr)
#define LDV_AND_EXPR_LOCATION(and_expr)      (and_expr->location)

/*
exclusive-OR-expression:
    AND-expression
    exclusive-OR-expression ^ AND-expression
*/
enum ldv_exclusive_or_expr_kind
{
  LDV_EXCLUSIVE_OR_EXPR_FIRST = 1,
  LDV_EXCLUSIVE_OR_EXPR_SECOND
};
struct ldv_exclusive_or_expr
{
  enum ldv_exclusive_or_expr_kind kind;
  ldv_and_expr_ptr and_expr;
  struct ldv_exclusive_or_expr *exclusive_or_expr;
  ldv_location_ptr location;
};
typedef struct ldv_exclusive_or_expr *ldv_exclusive_or_expr_ptr;
#define LDV_EXCLUSIVE_OR_EXPR_KIND(exclusive_or_expr)                  (exclusive_or_expr->kind)
#define LDV_EXCLUSIVE_OR_EXPR_AND_EXPR(exclusive_or_expr)              (exclusive_or_expr->and_expr)
#define LDV_EXCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR(exclusive_or_expr_arg) (exclusive_or_expr_arg->exclusive_or_expr)
#define LDV_EXCLUSIVE_OR_EXPR_LOCATION(exclusive_or_expr)              (exclusive_or_expr->location)

/*
inclusive-OR-expression:
    exclusive-OR-expression
    inclusive-OR-expression | exclusive-OR-expression
*/
enum ldv_inclusive_or_expr_kind
{
  LDV_INCLUSIVE_OR_EXPR_FIRST = 1,
  LDV_INCLUSIVE_OR_EXPR_SECOND
};
struct ldv_inclusive_or_expr
{
  enum ldv_inclusive_or_expr_kind kind;
  ldv_exclusive_or_expr_ptr exclusive_or_expr;
  struct ldv_inclusive_or_expr *inclusive_or_expr;
  ldv_location_ptr location;
};
typedef struct ldv_inclusive_or_expr *ldv_inclusive_or_expr_ptr;
#define LDV_INCLUSIVE_OR_EXPR_KIND(inclusive_or_expr)                  (inclusive_or_expr->kind)
#define LDV_INCLUSIVE_OR_EXPR_EXCLUSIVE_OR_EXPR(inclusive_or_expr)     (inclusive_or_expr->exclusive_or_expr)
#define LDV_INCLUSIVE_OR_EXPR_INCLUSIVE_OR_EXPR(inclusive_or_expr_arg) (inclusive_or_expr_arg->inclusive_or_expr)
#define LDV_INCLUSIVE_OR_EXPR_LOCATION(inclusive_or_expr)              (inclusive_or_expr->location)

/*
logical-AND-expression:
    inclusive-OR-expression
    logical-AND-expression && inclusive-OR-expression
*/
enum ldv_logical_and_expr_kind
{
  LDV_LOGICAL_AND_EXPR_FIRST = 1,
  LDV_LOGICAL_AND_EXPR_SECOND
};
struct ldv_logical_and_expr
{
  enum ldv_logical_and_expr_kind kind;
  ldv_inclusive_or_expr_ptr inclusive_or_expr;
  struct ldv_logical_and_expr *logical_and_expr;
  ldv_location_ptr location;
};
typedef struct ldv_logical_and_expr *ldv_logical_and_expr_ptr;
#define LDV_LOGICAL_AND_EXPR_KIND(logical_and_expr)                 (logical_and_expr->kind)
#define LDV_LOGICAL_AND_EXPR_INCLUSIVE_OR_EXPR(logical_and_expr)    (logical_and_expr->inclusive_or_expr)
#define LDV_LOGICAL_AND_EXPR_LOGICAL_AND_EXPR(logical_and_expr_arg) (logical_and_expr_arg->logical_and_expr)
#define LDV_LOGICAL_AND_EXPR_LOCATION(logical_and_expr)             (logical_and_expr->location)

/*
logical-OR-expression:
    logical-AND-expression
    logical-OR-expression || logical-AND-expression
*/
enum ldv_logical_or_expr_kind
{
  LDV_LOGICAL_OR_EXPR_FIRST = 1,
  LDV_LOGICAL_OR_EXPR_SECOND
};
struct ldv_logical_or_expr
{
  enum ldv_logical_or_expr_kind kind;
  ldv_logical_and_expr_ptr logical_and_expr;
  struct ldv_logical_or_expr *logical_or_expr;
  ldv_location_ptr location;
};
typedef struct ldv_logical_or_expr *ldv_logical_or_expr_ptr;
#define LDV_LOGICAL_OR_EXPR_KIND(logical_or_expr)                (logical_or_expr->kind)
#define LDV_LOGICAL_OR_EXPR_LOGICAL_AND_EXPR(logical_or_expr)    (logical_or_expr->logical_and_expr)
#define LDV_LOGICAL_OR_EXPR_LOGICAL_OR_EXPR(logical_or_expr_arg) (logical_or_expr_arg->logical_or_expr)
#define LDV_LOGICAL_OR_EXPR_LOCATION(logical_or_expr)            (logical_or_expr->location)

/*
conditional-expression:
    logical-OR-expression
    logical-OR-expression ? expression : conditional-expression

LDV extension

conditional-expression:
    LDV_MIN (expression, conditional-expression)
    LDV_MAX (expression, conditional-expression)
    LDV_ABS (expression)
*/
enum ldv_cond_expr_kind
{
  LDV_COND_EXPR_FIRST = 1,
  LDV_COND_EXPR_SECOND,
  LDV_COND_EXPR_THIRD,
  LDV_COND_EXPR_FOURTH,
  LDV_COND_EXPR_FIFTH
};
struct ldv_cond_expr
{
  enum ldv_cond_expr_kind kind;
  ldv_logical_or_expr_ptr logical_or_expr;
  struct ldv_expr *expr;
  struct ldv_cond_expr *cond_expr;
  ldv_location_ptr location;
};
typedef struct ldv_cond_expr *ldv_cond_expr_ptr;
#define LDV_COND_EXPR_KIND(cond_expr)            (cond_expr->kind)
#define LDV_COND_EXPR_LOGICAL_OR_EXPR(cond_expr) (cond_expr->logical_or_expr)
#define LDV_COND_EXPR_EXPR(cond_expr)            (cond_expr->expr)
#define LDV_COND_EXPR_COND_EXPR(cond_expr_arg)   (cond_expr_arg->cond_expr)
#define LDV_COND_EXPR_LOCATION(cond_expr)        (cond_expr->location)

/*
assignment-operator: one of
    = *= /= %= += -= <<= >>= &= ^= |=
*/
enum ldv_assignment_operator_kind
{
  LDV_ASSIGNMENT_OPERATOR_FIRST = 1,
  LDV_ASSIGNMENT_OPERATOR_SECOND = 2,
  LDV_ASSIGNMENT_OPERATOR_THIRD = 3,
  LDV_ASSIGNMENT_OPERATOR_FOURTH = 4,
  LDV_ASSIGNMENT_OPERATOR_FIVTH = 5,
  LDV_ASSIGNMENT_OPERATOR_SIXTH = 6,
  LDV_ASSIGNMENT_OPERATOR_SEVENTH = 7,
  LDV_ASSIGNMENT_OPERATOR_EIGHTH = 8,
  LDV_ASSIGNMENT_OPERATOR_NINTH = 9,
  LDV_ASSIGNMENT_OPERATOR_TENTH = 10,
  LDV_ASSIGNMENT_OPERATOR_ELEVEN = 11
};
struct ldv_assignment_operator
{
  enum ldv_assignment_operator_kind kind;
};
typedef struct ldv_assignment_operator *ldv_assignment_operator_ptr;
#define LDV_ASSIGNMENT_OPERATOR_KIND(assignment_operator) (assignment_operator->kind)

/*
assignment-expression:
    conditional-expression
    unary-expression assignment-operator assignment-expression
*/
enum ldv_assignment_expr_kind
{
  LDV_ASSIGNMENT_EXPR_FIRST = 1,
  LDV_ASSIGNMENT_EXPR_SECOND
};
struct ldv_assignment_expr
{
  enum ldv_assignment_expr_kind kind;
  ldv_cond_expr_ptr cond_expr;
  ldv_unary_expr_ptr unary_expr;
  ldv_assignment_operator_ptr assignment_operator;
  struct ldv_assignment_expr *assignment_expr;
  ldv_location_ptr location;
};
typedef struct ldv_assignment_expr *ldv_assignment_expr_ptr;
#define LDV_ASSIGNMENT_EXPR_KIND(assignment_expr)                (assignment_expr->kind)
#define LDV_ASSIGNMENT_EXPR_COND_EXPR(assignment_expr)           (assignment_expr->cond_expr)
#define LDV_ASSIGNMENT_EXPR_UNARY_EXPR(assignment_expr)          (assignment_expr->unary_expr)
#define LDV_ASSIGNMENT_EXPR_ASSIGNMENT_OPERATOR(assignment_expr) (assignment_expr->assignment_operator)
#define LDV_ASSIGNMENT_EXPR_ASSIGNMENT_EXPR(assignment_expr_arg) (assignment_expr_arg->assignment_expr)
#define LDV_ASSIGNMENT_EXPR_LOCATION(assignment_expr)            (assignment_expr->location)

/*
expression:
    assignment-expression
    expression , assignment-expression
*/
enum ldv_expr_kind
{
  LDV_EXPR_FIRST = 1,
  LDV_EXPR_SECOND
};
struct ldv_expr
{
  enum ldv_expr_kind kind;
  ldv_assignment_expr_ptr assignment_expr;
  struct ldv_expr *expr;
};
typedef struct ldv_expr *ldv_expr_ptr;
#define LDV_EXPR_KIND(expr)            (expr->kind)
#define LDV_EXPR_ASSIGNMENT_EXPR(expr) (expr->assignment_expr)
#define LDV_EXPR_EXPR(expr_arg)        (expr_arg->expr)

/*
GNU extension

asm-goto-operands:
    identifier
    asm-goto-operands, identifier
*/
enum ldv_asm_goto_operands_kind
{
  LDV_ASM_GOTO_OPERANDS_FIRST = 1,
  LDV_ASM_GOTO_OPERANDS_SECOND
};
struct ldv_asm_goto_operands
{
  enum ldv_asm_goto_operands_kind kind;
  ldv_identifier_ptr identifier;
  struct ldv_asm_goto_operands *asm_goto_operands;
};
typedef struct ldv_asm_goto_operands *ldv_asm_goto_operands_ptr;
#define LDV_ASM_GOTO_OPERANDS_KIND(asm_goto_operands)                  (asm_goto_operands->kind)
#define LDV_ASM_GOTO_OPERANDS_IDENTIFIER(asm_goto_operands)            (asm_goto_operands->identifier)
#define LDV_ASM_GOTO_OPERANDS_ASM_GOTO_OPERANDS(asm_goto_operands_arg) (asm_goto_operands_arg->asm_goto_operands)

/*
GNU extension

asm-string-literal:
    string-literal
*/
struct ldv_asm_str_literal
{
  ldv_str_literal_ptr str_literal;
};
typedef struct ldv_asm_str_literal *ldv_asm_str_literal_ptr;
#define LDV_ASM_STR_LITERAL_STR_LITERAL(asm_str_literal) (asm_str_literal->str_literal)

/*
GNU extension

asm-clobbers:
    asm-string-literal
    asm-clobbers , asm-string-literal
*/
enum ldv_asm_clobbers_kind
{
  LDV_ASM_CLOBBERS_FIRST = 1,
  LDV_ASM_CLOBBERS_SECOND
};
struct ldv_asm_clobbers
{
  enum ldv_asm_clobbers_kind kind;
  ldv_asm_str_literal_ptr asm_str_literal;
  struct ldv_asm_clobbers *asm_clobbers;
};
typedef struct ldv_asm_clobbers *ldv_asm_clobbers_ptr;
#define LDV_ASM_CLOBBERS_KIND(asm_clobbers)             (asm_clobbers->kind)
#define LDV_ASM_CLOBBERS_ASM_STR_LITERAL(asm_clobbers)  (asm_clobbers->asm_str_literal)
#define LDV_ASM_CLOBBERS_ASM_CLOBBERS(asm_clobbers_arg) (asm_clobbers_arg->asm_clobbers)

/*
GNU extension

asm-operand:
    asm-string-literal ( expression )
    [ identifier ] asm-string-literal ( expression )
*/
enum ldv_asm_operand_kind
{
  LDV_ASM_OPERAND_FIRST = 1,
  LDV_ASM_OPERAND_SECOND
};
struct ldv_asm_operand
{
  enum ldv_asm_operand_kind kind;
  ldv_asm_str_literal_ptr asm_str_literal;
  ldv_expr_ptr expr;
  ldv_identifier_ptr identifier;
};
typedef struct ldv_asm_operand *ldv_asm_operand_ptr;
#define LDV_ASM_OPERAND_KIND(asm_operand)            (asm_operand->kind)
#define LDV_ASM_OPERAND_ASM_STR_LITERAL(asm_operand) (asm_operand->asm_str_literal)
#define LDV_ASM_OPERAND_EXPR(asm_operand)            (asm_operand->expr)
#define LDV_ASM_OPERAND_IDENTIFIER(asm_operand)      (asm_operand->identifier)

/*
GNU extension

asm-operands:
    asm-operand
    asm-operands , asm-operand
*/
enum ldv_asm_operands_kind
{
  LDV_ASM_OPERANDS_FIRST = 1,
  LDV_ASM_OPERANDS_SECOND
};
struct ldv_asm_operands
{
  enum ldv_asm_operands_kind kind;
  ldv_asm_operand_ptr asm_operand;
  struct ldv_asm_operands *asm_operands;
};
typedef struct ldv_asm_operands *ldv_asm_operands_ptr;
#define LDV_ASM_OPERANDS_KIND(asm_operands)             (asm_operands->kind)
#define LDV_ASM_OPERANDS_ASM_OPERAND(asm_operands)      (asm_operands->asm_operand)
#define LDV_ASM_OPERANDS_ASM_OPERANDS(asm_operands_arg) (asm_operands_arg->asm_operands)

/*
GNU extension

asm-goto-argument:
    asm-string-literal : : asm-operandsopt : asm-clobbersopt : asm-goto-operands
*/
struct ldv_asm_goto_arg
{
  ldv_asm_str_literal_ptr asm_str_literal;
  ldv_asm_operands_ptr asm_operands;
  ldv_asm_clobbers_ptr asm_clobbers;
  ldv_asm_goto_operands_ptr asm_goto_operands;
};
typedef struct ldv_asm_goto_arg *ldv_asm_goto_arg_ptr;
#define LDV_ASM_GOTO_ARG_ASM_STR_LITERAL(asm_goto_arg)   (asm_goto_arg->asm_str_literal)
#define LDV_ASM_GOTO_ARG_ASM_OPERANDS(asm_goto_arg)      (asm_goto_arg->asm_operands)
#define LDV_ASM_GOTO_ARG_ASM_CLOBBERS(asm_goto_arg)      (asm_goto_arg->asm_clobbers)
#define LDV_ASM_GOTO_ARG_ASM_GOTO_OPERANDS(asm_goto_arg) (asm_goto_arg->asm_goto_operands)

/*
GNU extension

asm-argument:
    asm-string-literal
    asm-string-literal : asm-operandsopt
    asm-string-literal : asm-operandsopt : asm-operandsopt
    asm-string-literal : asm-operandsopt : asm-operandsopt : asm-clobbersopt
*/
enum ldv_asm_arg_kind
{
  LDV_ASM_ARG_FIRST = 1,
  LDV_ASM_ARG_SECOND,
  LDV_ASM_ARG_THIRD,
  LDV_ASM_ARG_FOURTH
};
struct ldv_asm_arg
{
  enum ldv_asm_arg_kind kind;
  ldv_asm_str_literal_ptr asm_str_literal;
  ldv_asm_operands_ptr asm_operands1, asm_operands2;
  ldv_asm_clobbers_ptr asm_clobbers;
};
typedef struct ldv_asm_arg *ldv_asm_arg_ptr;
#define LDV_ASM_ARG_KIND(asm_arg)            (asm_arg->kind)
#define LDV_ASM_ARG_ASM_STR_LITERAL(asm_arg) (asm_arg->asm_str_literal)
#define LDV_ASM_ARG_ASM_OPERANDS1(asm_arg)   (asm_arg->asm_operands1)
#define LDV_ASM_ARG_ASM_OPERANDS2(asm_arg)   (asm_arg->asm_operands2)
#define LDV_ASM_ARG_ASM_CLOBBERS(asm_arg)    (asm_arg->asm_clobbers)

/*
GNU extension

asm-statement:
    asm type-qualifieropt ( asm-argument ) ;
    asm type-qualifieropt goto ( asm-goto-argument ) ;
*/
enum ldv_asm_statement_kind
{
  LDV_ASM_STATEMENT_FIRST = 1,
  LDV_ASM_STATEMENT_SECOND
};
struct ldv_asm_statement
{
  enum ldv_asm_statement_kind kind;
  struct ldv_type_qual *type_qual;
  ldv_asm_arg_ptr asm_arg;
  ldv_asm_goto_arg_ptr asm_goto_arg;
  ldv_location_ptr location;
};
typedef struct ldv_asm_statement *ldv_asm_statement_ptr;
#define LDV_ASM_STATEMENT_KIND(asm_statement)         (asm_statement->kind)
#define LDV_ASM_STATEMENT_TYPE_QUAL(asm_statement)    (asm_statement->type_qual)
#define LDV_ASM_STATEMENT_ASM_ARG(asm_statement)      (asm_statement->asm_arg)
#define LDV_ASM_STATEMENT_ASM_GOTO_ARG(asm_statement) (asm_statement->asm_goto_arg)
#define LDV_ASM_STATEMENT_LOCATION(asm_statement)     (asm_statement->location)

/*
expression-statement:
    expressionopt ;
*/
struct ldv_expr_statement
{
  ldv_expr_ptr expr;
  ldv_location_ptr location;
};
typedef struct ldv_expr_statement *ldv_expr_statement_ptr;
#define LDV_EXPR_STATEMENT_EXPR(expr_statement)     (expr_statement->expr)
#define LDV_EXPR_STATEMENT_LOCATION(expr_statement) (expr_statement->location)

/* ! NOT FINISHED !
labeled-statement:
    identifier : statement
    case constant-expression : statement
    default : statement
*/
enum ldv_labeled_statement_kind
{
  LDV_LABELED_STATEMENT_LABEL = 1,
  LDV_LABELED_STATEMENT_CASE,
  LDV_LABELED_STATEMENT_DEFAULT
};
struct ldv_labeled_statement
{
  enum ldv_labeled_statement_kind kind;
  const char *id;
  struct ldv_statement *statement;
  int const_expr;
  ldv_location_ptr location;
};
typedef struct ldv_labeled_statement *ldv_labeled_statement_ptr;
#define LDV_LABELED_STATEMENT_KIND(labeled_statement)       (labeled_statement->kind)
#define LDV_LABELED_STATEMENT_ID(labeled_statement)         (labeled_statement->id)
#define LDV_LABELED_STATEMENT_STATEMENT(labeled_statement)  (labeled_statement->statement)
#define LDV_LABELED_STATEMENT_CONST_EXPR(labeled_statement) (labeled_statement->const_expr)
#define LDV_LABELED_STATEMENT_LOCATION(labeled_statement)   (labeled_statement->location)

/* Gcc replaces 'continue' and 'break' statements with 'goto'.
jump-statement:
    goto identifier ;
    continue ;
    break ;
    return expressionopt ;
*/
enum ldv_jump_statement_kind
{
  LDV_JUMP_STATEMENT_GOTO = 1,
  LDV_JUMP_STATEMENT_RETURN
};
struct ldv_jump_statement
{
  enum ldv_jump_statement_kind kind;
  const char *id;
  ldv_expr_ptr expr;
  ldv_location_ptr location;
};
typedef struct ldv_jump_statement *ldv_jump_statement_ptr;
#define LDV_JUMP_STATEMENT_KIND(jump_statement)     (jump_statement->kind)
#define LDV_JUMP_STATEMENT_ID(jump_statement)       (jump_statement->id)
#define LDV_JUMP_STATEMENT_EXPR(jump_statement)     (jump_statement->expr)
#define LDV_JUMP_STATEMENT_LOCATION(jump_statement) (jump_statement->location)

/*
selection-statement:
    if ( expression ) statement
    if ( expression ) statement else statement
    switch ( expression ) statement
*/
enum ldv_selection_statement_kind
{
  LDV_SELECTION_STATEMENT_IF_THEN = 1,
  LDV_SELECTION_STATEMENT_IF_THEN_ELSE,
  LDV_SELECTION_STATEMENT_SWITCH
};
struct ldv_selection_statement
{
  enum ldv_selection_statement_kind kind;
  ldv_expr_ptr expr;
  struct ldv_statement *statement1, *statement2;
  ldv_location_ptr location;
};
typedef struct ldv_selection_statement *ldv_selection_statement_ptr;
#define LDV_SELECTION_STATEMENT_KIND(selection_statement)       (selection_statement->kind)
#define LDV_SELECTION_STATEMENT_EXPR(selection_statement)       (selection_statement->expr)
#define LDV_SELECTION_STATEMENT_STATEMENT1(selection_statement) (selection_statement->statement1)
#define LDV_SELECTION_STATEMENT_STATEMENT2(selection_statement) (selection_statement->statement2)
#define LDV_SELECTION_STATEMENT_LOCATION(selection_statement)   (selection_statement->location)

/* There is no iteration statement in gcc.
statement:
    labeled-statement
    compound-statement
    expression-statement
    selection-statement
    iteration-statement
    jump-statement

GNU extension

statement:
    asm-statement
*/
enum ldv_statement_kind
{
  LDV_LABELED_STATEMENT = 1,
  LDV_COMPOUND_STATEMENT,
  LDV_EXPR_STATEMENT,
  LDV_SELECTION_STATEMENT,
  LDV_ITERATION_STATEMENT,
  LDV_JUMP_STATEMENT,
  LDV_ASM_STATEMENT
};
struct ldv_statement
{
  enum ldv_statement_kind kind;
  ldv_labeled_statement_ptr labeled_statement;
  struct ldv_compound_statement *compound_statement;
  ldv_expr_statement_ptr expr_statement;
  ldv_selection_statement_ptr selection_statement;
  ldv_jump_statement_ptr jump_statement;
  ldv_asm_statement_ptr asm_statement;
};
typedef struct ldv_statement *ldv_statement_ptr;
#define LDV_STATEMENT_KIND(statement)                (statement->kind)
#define LDV_STATEMENT_LABELED_STATEMENT(statement)   (statement->labeled_statement)
#define LDV_STATEMENT_COMPOUND_STATEMENT(statement)  (statement->compound_statement)
#define LDV_STATEMENT_EXPR_STATEMENT(statement)      (statement->expr_statement)
#define LDV_STATEMENT_SELECTION_STATEMENT(statement) (statement->selection_statement)
#define LDV_STATEMENT_JUMP_STATEMENT(statement)      (statement->jump_statement)
#define LDV_STATEMENT_ASM_STATEMENT(statement)       (statement->asm_statement)

/* Treated in the same way as function-definition.
GNU extensions:

nested-function-definition:
    declaration-specifiers declarator declaration-listopt compound-statement
*/
struct ldv_nested_func_def
{
  struct ldv_func_def *func_def;
};
typedef struct ldv_nested_func_def *ldv_nested_func_def_ptr;
#define LDV_NESTED_FUNC_DEF_FUNC_DEF(nested_func_def) (nested_func_def->func_def)

/*
GNU extensions:

nested-declaration:
    declaration
    nested-function-definition
*/
enum ldv_nested_decl_kind
{
  LDV_NESTED_DECL_FIRST = 1,
  LDV_NESTED_DECL_SECOND
};
struct ldv_nested_decl
{
  enum ldv_nested_decl_kind kind;
  struct ldv_decl *decl;
  ldv_nested_func_def_ptr nested_func_def;
};
typedef struct ldv_nested_decl *ldv_nested_decl_ptr;
#define LDV_NESTED_DECL_KIND(nested_decl)            (nested_decl->kind)
#define LDV_NESTED_DECL_DECL(nested_decl)            (nested_decl->decl)
#define LDV_NESTED_DECL_NESTED_FUNC_DEF(nested_decl) (nested_decl->nested_func_def)

/*
block-item:
    declaration
    statement

GNU extensions:

block-item:
    declaration -> nested-declaration
*/
enum ldv_block_item_kind
{
  LDV_BLOCK_ITEM_FIRST = 1,
  LDV_BLOCK_ITEM_SECOND
};
struct ldv_block_item
{
  enum ldv_block_item_kind kind;
  struct ldv_nested_decl *nested_decl;
  ldv_statement_ptr statement;
};
typedef struct ldv_block_item *ldv_block_item_ptr;
#define LDV_BLOCK_ITEM_KIND(block_item)        (block_item->kind)
#define LDV_BLOCK_ITEM_NESTED_DECL(block_item) (block_item->nested_decl)
#define LDV_BLOCK_ITEM_STATEMENT(block_item)   (block_item->statement)

/*
block-item-list:
    block-item
    block-item-list block-item
*/
struct ldv_block_item_list
{
  ldv_block_item_ptr block_item;
  struct ldv_block_item_list *block_item_list;
};
typedef struct ldv_block_item_list *ldv_block_item_list_ptr;
#define LDV_BLOCK_ITEM_LIST_BLOCK_ITEM(block_item_list)          (block_item_list->block_item)
#define LDV_BLOCK_ITEM_LIST_BLOCK_ITEM_LIST(block_item_list_arg) (block_item_list_arg->block_item_list)

/*
GNU extensions:

label-declaration:
    __label__ identifier-list ;
*/
struct ldv_label_decl
{
  ldv_identifier_ptr identifier;
};
typedef struct ldv_label_decl *ldv_label_decl_ptr;
#define LDV_LABEL_DECL_IDENTIFIER(label_decl) (label_decl->identifier)

/*
GNU extensions:

label-declarations:
    label-declaration
    label-declarations label-declaration
*/
struct ldv_label_decls
{
  ldv_label_decl_ptr label_decl;
  struct ldv_label_decls *label_decls;
};
typedef struct ldv_label_decls *ldv_label_decls_ptr;
#define LDV_LABEL_DECLS_LABEL_DECL(label_decls)      (label_decls->label_decl)
#define LDV_LABEL_DECLS_LABEL_DECLS(label_decls_arg) (label_decls_arg->label_decls)

/*
compound-statement:
    { block-item-listopt }

GNU extensions:

compound-statement:
    { label-declarations block-item-list }
*/
struct ldv_compound_statement
{
  ldv_block_item_list_ptr block_item_list;
  ldv_label_decls_ptr label_decls;
};
typedef struct ldv_compound_statement *ldv_compound_statement_ptr;
#define LDV_COMPOUND_STATEMENT_BLOCK_ITEM_LIST(compound_statement) (compound_statement->block_item_list)
#define LDV_COMPOUND_STATEMENT_LABEL_DECLS(compound_statement)     (compound_statement->label_decls)

/* There is no declaration list in gcc.
declaration-list:
    declaration
    declaration-list declaration
*/

/* ! NOT FINISHED !
direct-abstract-declarator:
    ( abstract-declarator )
    direct-abstract-declaratoropt [ assignment-expressionopt ]
    direct-abstract-declaratoropt [ * ]
    direct-abstract-declaratoropt ( parameter-type-listopt )
*/
struct ldv_direct_abstract_declarator
{
  struct ldv_abstract_declarator *abstract_declarator;
  struct ldv_direct_abstract_declarator *direct_abstract_declarator;
  int a;
  struct ldv_param_type_list *param_type_list;
};
typedef struct ldv_direct_abstract_declarator *ldv_direct_abstract_declarator_ptr;
#define LDV_DIRECT_ABSTRACT_DECLARATOR_ABSTRACT_DECLARATOR(direct_abstract_declarator)            (direct_abstract_declarator->abstract_declarator)
#define LDV_DIRECT_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR(direct_abstract_declarator_arg) (direct_abstract_declarator_arg->direct_abstract_declarator)
#define LDV_DIRECT_ABSTRACT_DECLARATOR_ASSIGN_EXPR(direct_abstract_declarator)                    (direct_abstract_declarator->a)
#define LDV_DIRECT_ABSTRACT_DECLARATOR_PARAM_TYPE_LIST(direct_abstract_declarator)                (direct_abstract_declarator->param_type_list)

/*
abstract-declarator:
    pointer
    pointeropt direct-abstract-declarator
*/
struct ldv_abstract_declarator
{
  struct ldv_pointer *pointer;
  ldv_direct_abstract_declarator_ptr direct_abstract_declarator;
};
typedef struct ldv_abstract_declarator *ldv_abstract_declarator_ptr;
#define LDV_ABSTRACT_DECLARATOR_POINTER(abstract_declarator)                    (abstract_declarator->pointer)
#define LDV_ABSTRACT_DECLARATOR_DIRECT_ABSTRACT_DECLARATOR(abstract_declarator) (abstract_declarator->direct_abstract_declarator)

/*
parameter-declaration:
    declaration-specifiers declarator
    declaration-specifiers abstract-declaratoropt
*/
struct ldv_param_decl
{
  struct ldv_decl_spec *decl_spec;
  struct ldv_declarator *declarator;
  ldv_abstract_declarator_ptr abstract_declarator;
};
typedef struct ldv_param_decl *ldv_param_decl_ptr;
#define LDV_PARAM_DECL_DECL_SPEC(param_decl)           (param_decl->decl_spec)
#define LDV_PARAM_DECL_DECLARATOR(param_decl)          (param_decl->declarator)
#define LDV_PARAM_DECL_ABSTRACT_DECLARATOR(param_decl) (param_decl->abstract_declarator)

/*
parameter-list:
    parameter-declaration
    parameter-list , parameter-declaration
*/
struct ldv_param_list
{
  ldv_param_decl_ptr param_decl;
  struct ldv_param_list *param_list;
};
typedef struct ldv_param_list *ldv_param_list_ptr;
#define LDV_PARAM_LIST_PARAM_DECL(param_list)     (param_list->param_decl)
#define LDV_PARAM_LIST_PARAM_LIST(param_list_arg) (param_list_arg->param_list)

/*
parameter-type-list:
    parameter-list
    parameter-list , ...
*/
struct ldv_param_type_list
{
  ldv_param_list_ptr param_list;
  bool is_va;
};
typedef struct ldv_param_type_list *ldv_param_type_list_ptr;
#define LDV_PARAM_TYPE_LIST_PARAM_LIST(param_type_list) (param_type_list->param_list)
#define LDV_PARAM_TYPE_LIST_IS_VA(param_type_list)      (param_type_list->is_va)

/* ! NOT FINISHED !
direct-declarator:
    identifier
    ( declarator )
    direct-declarator [ type-qualifier-listopt assignment-expressionopt ]
    direct-declarator [ static type-qualifier-listopt assignment-expression ]
    direct-declarator [ type-qualifier-list static assignment-expression ]
    direct-declarator [ type-qualifier-listopt * ]
    direct-declarator ( parameter-type-list )
    direct-declarator ( identifier-listopt )
*/
enum ldv_direct_declarator_kind
{
  LDV_DIRECT_DECLARATOR_FIRST = 1,
  LDV_DIRECT_DECLARATOR_SECOND,
  LDV_DIRECT_DECLARATOR_THIRD,
  LDV_DIRECT_DECLARATOR_FOURTH,
  LDV_DIRECT_DECLARATOR_FIFTH,
  LDV_DIRECT_DECLARATOR_SIXTH,
  LDV_DIRECT_DECLARATOR_SEVENTH,
  LDV_DIRECT_DECLARATOR_EIGHT
};
struct ldv_direct_declarator
{
  enum ldv_direct_declarator_kind kind;
  ldv_identifier_ptr identifier;
  struct ldv_declarator *declarator;
  struct ldv_direct_declarator *direct_declarator;
  ldv_assignment_expr_ptr assignment_expr;
  ldv_param_type_list_ptr param_type_list;
  ldv_location_ptr location;
};
typedef struct ldv_direct_declarator *ldv_direct_declarator_ptr;
#define LDV_DIRECT_DECLARATOR_KIND(direct_declarator)                  (direct_declarator->kind)
#define LDV_DIRECT_DECLARATOR_IDENTIFIER(direct_declarator)            (direct_declarator->identifier)
#define LDV_DIRECT_DECLARATOR_DECLARATOR(direct_declarator)            (direct_declarator->declarator)
#define LDV_DIRECT_DECLARATOR_DIRECT_DECLARATOR(direct_declarator_arg) (direct_declarator_arg->direct_declarator)
#define LDV_DIRECT_DECLARATOR_ASSIGNMENT_EXPR(direct_declarator)       (direct_declarator->assignment_expr)
#define LDV_DIRECT_DECLARATOR_PARAM_TYPE_LIST(direct_declarator)       (direct_declarator->param_type_list)
#define LDV_DIRECT_DECLARATOR_LOCATION(direct_declarator)              (direct_declarator->location)

/*
type-qualifier-list:
    type-qualifier
    type-qualifier-list type-qualifier
*/
struct ldv_type_qual_list
{
  struct ldv_type_qual *type_qual;
  struct ldv_type_qual_list *type_qual_list;
};
typedef struct ldv_type_qual_list *ldv_type_qual_list_ptr;
#define LDV_TYPE_QUAL_LIST_TYPE_QUAL(type_qual_list) (type_qual_list->type_qual)
#define LDV_TYPE_QUAL_LIST_TYPE_QUAL_LIST(type_qual_list_arg) (type_qual_list_arg->type_qual_list)

/*
pointer:
    * type-qualifier-listopt
    * type-qualifier-listopt pointer
*/
struct ldv_pointer
{
  ldv_type_qual_list_ptr type_qual_list;
  struct ldv_pointer *pointer;
};
typedef struct ldv_pointer *ldv_pointer_ptr;
#define LDV_POINTER_TYPE_QUAL_LIST(pointer) (pointer->type_qual_list)
#define LDV_POINTER_POINTER(pointer_arg) (pointer_arg->pointer)

/*
declarator:
   pointeropt direct-declarator
*/
struct ldv_declarator
{
  ldv_pointer_ptr pointer;
  ldv_direct_declarator_ptr direct_declarator;
};
typedef struct ldv_declarator *ldv_declarator_ptr;
#define LDV_DECLARATOR_POINTER(declarator) (declarator->pointer)
#define LDV_DECLARATOR_DIRECT_DECLARATOR(declarator) (declarator->direct_declarator)

/*
function-specifier:
    inline
*/
enum ldv_func_spec_kind
{
  LDV_FUNC_SPEC_INLINE = 1
};
struct ldv_func_spec
{
  enum ldv_func_spec_kind kind;
};
typedef struct ldv_func_spec *ldv_func_spec_ptr;
#define LDV_FUNC_SPEC_KIND(func_spec) (func_spec->kind)

/*
type-qualifier:
    const
    restrict
    volatile
*/
enum ldv_type_qual_kind
{
  LDV_TYPE_QUAL_UNKNOWN,
  LDV_TYPE_QUAL_CONST,
  LDV_TYPE_QUAL_RESTRICT,
  LDV_TYPE_QUAL_VOLATILE
};
struct ldv_type_qual
{
  enum ldv_type_qual_kind kind;
};
typedef struct ldv_type_qual *ldv_type_qual_ptr;
#define LDV_TYPE_QUAL_KIND(type_qual) (type_qual->kind)

/*
typedef-name:
    identifier
*/
struct ldv_typedef_name
{
  ldv_identifier_ptr identifier;
};
typedef struct ldv_typedef_name *ldv_typedef_name_ptr;
#define LDV_TYPEDEF_NAME_IDENTIFIER(typedef_name) (typedef_name->identifier)

/*
constant-expression:
    conditional-expression
*/
struct ldv_const_expr
{
  ldv_cond_expr_ptr cond_expr;
};
typedef struct ldv_const_expr *ldv_const_expr_ptr;
#define LDV_CONST_EXPR_COND_EXPR(const_expr) (const_expr->cond_expr)

/*
enumeration-constant:
    identifier
*/
struct ldv_enum_const
{
  const char *id;
};
typedef struct ldv_enum_const *ldv_enum_const_ptr;
#define LDV_ENUM_CONST_ID(enum_const) (enum_const->id)

/*
enumerator:
    enumeration-constant
    enumeration-constant = constant-expression
*/
enum ldv_enum_kind
{
  LDV_ENUM_FIRST = 1,
  LDV_ENUM_SECOND
};
struct ldv_enum
{
  enum ldv_enum_kind kind;
  ldv_enum_const_ptr enum_const;
  ldv_const_expr_ptr const_expr;
};
typedef struct ldv_enum *ldv_enum_ptr;
#define LDV_ENUM_KIND(ldv_enum)       (ldv_enum->kind)
#define LDV_ENUM_ENUM_CONST(ldv_enum) (ldv_enum->enum_const)
#define LDV_ENUM_CONST_EXPR(ldv_enum) (ldv_enum->const_expr)

/*
enumerator-list:
    enumerator
    enumerator-list , enumerator
*/
struct ldv_enum_list
{
  ldv_enum_ptr ldv_enum;
  struct ldv_enum_list *enum_list;
};
typedef struct ldv_enum_list *ldv_enum_list_ptr;
#define LDV_ENUM_LIST_ENUM(enum_list)          (enum_list->ldv_enum)
#define LDV_ENUM_LIST_ENUM_LIST(enum_list_arg) (enum_list_arg->enum_list)

/*
enum-specifier:
    enum identifieropt { enumerator-list }
    enum identifieropt { enumerator-list , }
    enum identifier
*/
struct ldv_enum_spec
{
  ldv_identifier_ptr identifier;
  ldv_enum_list_ptr enum_list;
};
typedef struct ldv_enum_spec *ldv_enum_spec_ptr;
#define LDV_ENUM_SPEC_IDENTIFIER(enum_spec) (enum_spec->identifier)
#define LDV_ENUM_SPEC_ENUM_LIST(enum_spec)  (enum_spec->enum_list)

/* ! NOT FINISHED !
struct-declarator:
    declarator
    declaratoropt : constant-expression
*/
struct ldv_struct_declarator
{
  ldv_declarator_ptr declarator;
  int a;
};
typedef struct ldv_struct_declarator *ldv_struct_declarator_ptr;
#define LDV_STRUCT_DECLARATOR_DECLARATOR(struct_declarator) (struct_declarator->declarator)
#define LDV_STRUCT_DECLARATOR_CONST_EXPR(struct_declarator) (struct_declarator->a)

/*
struct-declarator-list:
    struct-declarator
    struct-declarator-list , struct-declarator
*/
struct ldv_struct_declarator_list
{
  ldv_struct_declarator_ptr struct_declarator;
  struct ldv_struct_declarator_list *struct_declarator_list;
};
typedef struct ldv_struct_declarator_list *ldv_struct_declarator_list_ptr;
#define LDV_STRUCT_DECLARATOR_LIST_STRUCT_DECLARATOR(struct_declarator_list)          (struct_declarator_list->struct_declarator)
#define LDV_STRUCT_DECLARATOR_LIST_STRUCT_DECLARATOR_LIST(struct_declarator_list_arg) (struct_declarator_list_arg->struct_declarator_list)

/*
specifier-qualifier-list:
    type-specifier specifier-qualifier-listopt
    type-qualifier specifier-qualifier-listopt
*/
struct ldv_spec_qual_list
{
  struct ldv_type_spec *type_spec;
  ldv_type_qual_ptr type_qual;
  struct ldv_spec_qual_list *spec_qual_list;
};
typedef struct ldv_spec_qual_list *ldv_spec_qual_list_ptr;
#define LDV_SPEC_QUAL_LIST_TYPE_SPEC(spec_qual_list)          (spec_qual_list->type_spec)
#define LDV_SPEC_QUAL_LIST_TYPE_QUAL(spec_qual_list)          (spec_qual_list->type_qual)
#define LDV_SPEC_QUAL_LIST_SPEC_QUAL_LIST(spec_qual_list_arg) (spec_qual_list_arg->spec_qual_list)

/*
struct-declaration:
    specifier-qualifier-list struct-declarator-list ;
*/
struct ldv_struct_decl
{
  ldv_spec_qual_list_ptr spec_qual_list;
  struct ldv_struct_declarator_list *struct_declarator_list;
};
typedef struct ldv_struct_decl *ldv_struct_decl_ptr;
#define LDV_STRUCT_DECL_SPEC_QUAL_LIST(struct_decl)         (struct_decl->spec_qual_list)
#define LDV_STRUCT_DECL_STRUCT_DECLARATOR_LIST(struct_decl) (struct_decl->struct_declarator_list)

/*
struct-declaration-list:
    struct-declaration
    struct-declaration-list struct-declaration
*/
struct ldv_struct_decl_list
{
  ldv_struct_decl_ptr struct_decl;
  struct ldv_struct_decl_list *struct_decl_list;
};
typedef struct ldv_struct_decl_list *ldv_struct_decl_list_ptr;
#define LDV_STRUCT_DECL_LIST_STRUCT_DECL(struct_decl_list)          (struct_decl_list->struct_decl)
#define LDV_STRUCT_DECL_LIST_STRUCT_DECL_LIST(struct_decl_list_arg) (struct_decl_list_arg->struct_decl_list)

/*
struct-or-union:
    struct
    union
*/
enum ldv_struct_or_union_kind
{
  LDV_STRUCT_OR_UNION_STRUCT = 1,
  LDV_STRUCT_OR_UNION_UNION
};
struct ldv_struct_or_union
{
  enum ldv_struct_or_union_kind struct_or_union;
};
typedef struct ldv_struct_or_union *ldv_struct_or_union_ptr;
#define LDV_STRUCT_OR_UNION_STRUCT_OR_UNION(struct_or_union_arg) (struct_or_union_arg->struct_or_union)

/*
struct-or-union-specifier:
    struct-or-union identifieropt { struct-declaration-list }
    struct-or-union identifier

GNU extension

struct-or-union-specifier:
    struct-or-union identifieropt { }
*/
enum ldv_struct_or_union_spec_kind
{
  LDV_STRUCT_OR_UNION_SPEC_FIRST = 1,
  LDV_STRUCT_OR_UNION_SPEC_SECOND,
  LDV_STRUCT_OR_UNION_SPEC_THIRD
};
struct ldv_struct_or_union_spec
{
  enum ldv_struct_or_union_spec_kind kind;
  ldv_struct_or_union_ptr struct_or_union;
  const char *id;
  ldv_struct_decl_list_ptr struct_decl_list;
};
typedef struct ldv_struct_or_union_spec *ldv_struct_or_union_spec_ptr;
#define LDV_STRUCT_OR_UNION_SPEC_KIND(struct_or_union_spec)             (struct_or_union_spec->kind)
#define LDV_STRUCT_OR_UNION_SPEC_STRUCT_OR_UNION(struct_or_union_spec)  (struct_or_union_spec->struct_or_union)
#define LDV_STRUCT_OR_UNION_SPEC_ID(struct_or_union_spec)               (struct_or_union_spec->id)
#define LDV_STRUCT_OR_UNION_SPEC_STRUCT_DECL_LIST(struct_or_union_spec) (struct_or_union_spec->struct_decl_list)

/*
type-specifier:
    void
    char
    short
    int
    long
    float
    double
    signed
    unsigned
    _Bool
    _Complex
    struct-or-union-specifier
    enum-specifier
    typedef-name
*/
enum ldv_type_spec_kind
{
  LDV_TYPE_SPEC_UNKNOWN,
  LDV_TYPE_SPEC_VOID,
  LDV_TYPE_SPEC_CHAR,
  LDV_TYPE_SPEC_SHORT,
  LDV_TYPE_SPEC_INT,
  LDV_TYPE_SPEC_LONG,
  LDV_TYPE_SPEC_FLOAT,
  LDV_TYPE_SPEC_DOUBLE,
  LDV_TYPE_SPEC_SIGNED,
  LDV_TYPE_SPEC_UNSIGNED,
  LDV_TYPE_SPEC_BOOL,
  LDV_TYPE_SPEC_COMPLEX,
  LDV_TYPE_SPEC_FIRST,
  LDV_TYPE_SPEC_SECOND,
  LDV_TYPE_SPEC_THIRD
};
struct ldv_type_spec
{
  enum ldv_type_spec_kind kind;
  ldv_struct_or_union_spec_ptr struct_or_union_spec;
  ldv_enum_spec_ptr enum_spec;
  ldv_typedef_name_ptr typedef_name;
};
typedef struct ldv_type_spec *ldv_type_spec_ptr;
#define LDV_TYPE_SPEC_KIND(type_spec)                 (type_spec->kind)
#define LDV_TYPE_SPEC_STRUCT_OR_UNION_SPEC(type_spec) (type_spec->struct_or_union_spec)
#define LDV_TYPE_SPEC_ENUM_SPEC(type_spec)            (type_spec->enum_spec)
#define LDV_TYPE_SPEC_TYPEDEF_NAME(type_spec)         (type_spec->typedef_name)

/*
storage-class-specifier:
    typedef
    extern
    static
    auto
    register
*/
enum ldv_storage_class_spec_kind
{
  LDV_STORAGE_CLASS_SPEC_TYPEDEF = 1,
  LDV_STORAGE_CLASS_SPEC_EXTERN,
  LDV_STORAGE_CLASS_SPEC_STATIC,
  LDV_STORAGE_CLASS_SPEC_AUTO,
  LDV_STORAGE_CLASS_SPEC_REGISTER
};
struct ldv_storage_class_spec
{
  enum ldv_storage_class_spec_kind kind;
};
typedef struct ldv_storage_class_spec *ldv_storage_class_spec_ptr;
#define LDV_STORAGE_CLASS_SPEC_KIND(storage_class_spec) (storage_class_spec->kind)

/*
declaration-specifiers:
    storage-class-specifier declaration-specifiersopt
    type-specifier declaration-specifiersopt
    type-qualifier declaration-specifiersopt
    function-specifier declaration-specifiersopt
*/
struct ldv_decl_spec
{
  ldv_storage_class_spec_ptr storage_class_spec;
  ldv_type_spec_ptr type_spec;
  ldv_type_qual_ptr type_qual;
  ldv_func_spec_ptr func_spec;
  struct ldv_decl_spec *decl_spec;
};
typedef struct ldv_decl_spec *ldv_decl_spec_ptr;
#define LDV_DECL_SPEC_STORAGE_CLASS_SPEC(decl_spec) (decl_spec->storage_class_spec)
#define LDV_DECL_SPEC_TYPE_SPEC(decl_spec)          (decl_spec->type_spec)
#define LDV_DECL_SPEC_TYPE_QUAL(decl_spec)          (decl_spec->type_qual)
#define LDV_DECL_SPEC_FUNC_SPEC(decl_spec)          (decl_spec->func_spec)
#define LDV_DECL_SPEC_DECL_SPEC(decl_spec_arg)      (decl_spec_arg->decl_spec)

/*
initializer:
    assignment-expression
    { initializer-list }
    { initializer-list , }
*/
enum ldv_initializer_kind
{
  LDV_INITIALIZER_FIRST = 1,
  LDV_INITIALIZER_SECOND,
  LDV_INITIALIZER_THIRD
};
struct ldv_initializer
{
  enum ldv_initializer_kind kind;
  ldv_assignment_expr_ptr assignment_expr;
  ldv_initializer_list_ptr initializer_list;
};
typedef struct ldv_initializer *ldv_initializer_ptr;
#define LDV_INITIALIZER_KIND(initializer)             (initializer->kind)
#define LDV_INITIALIZER_ASSIGNMENT_EXPR(initializer)  (initializer->assignment_expr)
#define LDV_INITIALIZER_INITIALIZER_LIST(initializer) (initializer->initializer_list)

/*
init-declarator:
    declarator
    declarator = initializer
*/
enum ldv_init_declarator_kind
{
  LDV_INIT_DECLARATOR_FIRST = 1,
  LDV_INIT_DECLARATOR_SECOND
};
struct ldv_init_declarator
{
  enum ldv_init_declarator_kind kind;
  ldv_declarator_ptr declarator;
  ldv_initializer_ptr initializer;
};
typedef struct ldv_init_declarator *ldv_init_declarator_ptr;
#define LDV_INIT_DECLARATOR_KIND(init_declarator)        (init_declarator->kind)
#define LDV_INIT_DECLARATOR_DECLARATOR(init_declarator)  (init_declarator->declarator)
#define LDV_INIT_DECLARATOR_INITIALIZER(init_declarator) (init_declarator->initializer)

/*
init-declarator-list:
    init-declarator
    init-declarator-list , init-declarator
*/
struct ldv_init_declarator_list
{
  ldv_init_declarator_ptr init_declarator;
  struct ldv_init_declarator_list *init_declarator_list;
};
typedef struct ldv_init_declarator_list *ldv_init_declarator_list_ptr;
#define LDV_INIT_DECLARATOR_LIST_INIT_DECLARATOR(init_declarator_list)          (init_declarator_list->init_declarator)
#define LDV_INIT_DECLARATOR_LIST_INIT_DECLARATOR_LIST(init_declarator_list_arg) (init_declarator_list_arg->init_declarator_list)

/*
declaration:
    declaration-specifiers init-declarator-listopt ;
*/
struct ldv_decl
{
  ldv_decl_spec_ptr decl_spec;
  ldv_init_declarator_list_ptr init_declarator_list;
};
typedef struct ldv_decl *ldv_decl_ptr;
#define LDV_DECL_DECL_SPEC(decl)            (decl->decl_spec)
#define LDV_DECL_INIT_DECLARATOR_LIST(decl) (decl->init_declarator_list)

/*
function-definition:
    declaration-specifiers declarator declaration-listopt compound-statement
*/
struct ldv_func_def
{
  ldv_decl_spec_ptr decl_spec;
  ldv_declarator_ptr declarator;
  ldv_compound_statement_ptr compound_statement;
};
typedef struct ldv_func_def *ldv_func_def_ptr;
#define LDV_FUNC_DEF_DECL_SPEC(func_def)          (func_def->decl_spec)
#define LDV_FUNC_DEF_DECLARATOR(func_def)         (func_def->declarator)
#define LDV_FUNC_DEF_COMPOUND_STATEMENT(func_def) (func_def->compound_statement)

/*
external-declaration:
    function-definition
    declaration
*/
enum ldv_ext_decl_kind
{
  LDV_EXT_DECL_FIRST = 1,
  LDV_EXT_DECL_SECOND
};
struct ldv_ext_decl
{
  enum ldv_ext_decl_kind kind;
  ldv_func_def_ptr func_def;
  ldv_decl_ptr decl;
};
typedef struct ldv_ext_decl *ldv_ext_decl_ptr;
#define LDV_EXT_DECL_KIND(ext_decl)     (ext_decl->kind)
#define LDV_EXT_DECL_FUNC_DEF(ext_decl) (ext_decl->func_def)
#define LDV_EXT_DECL_DECL(ext_decl)     (ext_decl->decl)

/*
translation-unit:
    external-declaration
    translation-unit external-declaration
*/
struct ldv_translation_unit
{
  ldv_ext_decl_ptr ext_decl;
  struct ldv_translation_unit *translation_unit;
};
typedef struct ldv_translation_unit *ldv_translation_unit_ptr;
#define LDV_TRANSLATION_UNIT_EXT_DECL(translation_unit)             (translation_unit->ext_decl)
#define LDV_TRANSLATION_UNIT_TRANSLATION_UNIT(translation_unit_arg) (translation_unit_arg->translation_unit)

#endif /* LDV_GRAMMAR */
