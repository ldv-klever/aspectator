/* Test for constant expressions: overflow and constant expressions;
   see also overflow-warn-*.c for some other cases.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

#include <float.h>
#include <limits.h>

int a = DBL_MAX; /* { dg-warning "overflow in implicit constant conversion" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */
int b = (int) DBL_MAX; /* { dg-error "overflow" "" } */
unsigned int c = -1.0; /* { dg-warning "overflow in implicit constant conversion" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */
unsigned int d = (unsigned)-1.0; /* { dg-error "overflow" } */

int e = 0 << 1000; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */
int f = 0 << -1; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */
int g = 0 >> 1000; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */
int h = 0 >> -1; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */

int b1 = (0 ? (int) DBL_MAX : 0);
unsigned int d1 = (0 ? (unsigned int)-1.0 : 0);
int e1 = (0 ? 0 << 1000 : 0);
int f1 = (0 ? 0 << -1 : 0);
int g1 = (0 ? 0 >> 1000 : 0);
int h1 = (0 ? 0 >> -1: 0);

int i = -1 << 0;
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */

int j[1] = { DBL_MAX }; /* { dg-warning "overflow in implicit constant conversion" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */

int array[2] = { [0 * (INT_MAX + 1)] = 0 }; /* { dg-warning "integer overflow in expression" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */

_Bool k = INT_MAX + 1; /* { dg-warning "integer overflow in expression" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */
