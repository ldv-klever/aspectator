/* PR inline-asm/84941 */
/* { dg-do compile } */
/* { dg-options "-O2" } */

void
foo (void)
{
  short *b[1] = { 0 };
  asm volatile ("" : "=m,m" (b), "=r,r" (b) : "1,p" (b));
}
