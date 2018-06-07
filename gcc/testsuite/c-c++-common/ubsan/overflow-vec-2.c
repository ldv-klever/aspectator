/* { dg-do run } */
/* { dg-options "-Wno-psabi -fsanitize=signed-integer-overflow -Wno-unused-variable -fsanitize-recover=signed-integer-overflow" } */

#define SCHAR_MAX __SCHAR_MAX__
#define SCHAR_MIN (-__SCHAR_MAX__ - 1)
#define SHRT_MAX __SHRT_MAX__
#define SHRT_MIN (-__SHRT_MAX__ - 1)
#define INT_MAX __INT_MAX__
#define INT_MIN (-__INT_MAX__ - 1)

typedef signed char VC __attribute__((vector_size (16)));
typedef short VS __attribute__((vector_size (8 * sizeof (short))));
typedef int VI __attribute__((vector_size (4 * sizeof (int))));
typedef int VI2 __attribute__((vector_size (16 * sizeof (int))));

void __attribute__((noinline,noclone))
checkvc (VC i, VC j)
{
  if (__builtin_memcmp (&i, &j, sizeof (VC)))
    __builtin_abort ();
}

void __attribute__((noinline,noclone))
checkvs (VS i, VS j)
{
  if (__builtin_memcmp (&i, &j, sizeof (VS)))
    __builtin_abort ();
}

void __attribute__((noinline,noclone))
checkvi (VI i, VI j)
{
  if (__builtin_memcmp (&i, &j, sizeof (VI)))
    __builtin_abort ();
}

void __attribute__((noinline,noclone))
checkvi2 (VI2 i, VI2 j)
{
  if (__builtin_memcmp (&i, &j, sizeof (VI2)))
    __builtin_abort ();
}

VI __attribute__((noinline,noclone))
foo (VI i)
{
  return -i;
}

VS __attribute__((noinline,noclone))
bar (VS i, VS j)
{
  return i + j;
}

int
main (void)
{
  /* Check that for a vector operation, only the first element with UB is reported.  */
  volatile VC a = (VC) { 0, SCHAR_MAX - 2, SCHAR_MAX - 2, 3, 2, 3, 4, 5,  0, 7,  1,  2,  3, 4,  SCHAR_MAX - 13, SCHAR_MAX };
  volatile VC b = (VC) { 5, 2,		   3,		  5, 0, 1, 2, 7,  8, 9,  10, 11, 6, -2, 13,	        1 };
  volatile VC k = b + a;
  checkvc (k,	  (VC) { 5, SCHAR_MAX,     SCHAR_MIN,     8, 2, 4, 6, 12, 8, 16, 11, 13, 9, 2,  SCHAR_MAX,      SCHAR_MIN });
  k = a + b;
  checkvc (k,     (VC) { 5, SCHAR_MAX,     SCHAR_MIN,     8, 2, 4, 6, 12, 8, 16, 11, 13, 9, 2,  SCHAR_MAX,      SCHAR_MIN });

  volatile VS c = (VS) { 0, SHRT_MAX - 2, SHRT_MAX - 2, 3, 3, 4,  SHRT_MAX - 13, SHRT_MAX };
  volatile VS d = (VS) { 5, 2,		  3,		5, 6, -2, 13,	         1 };
  volatile VS l = d + c;
  checkvs (l,	  (VS) { 5, SHRT_MAX,     SHRT_MIN,     8, 9, 2,  SHRT_MAX,      SHRT_MIN });
  l = bar (c, d);
  checkvs (l,     (VS) { 5, SHRT_MAX,     SHRT_MIN,     8, 9, 2,  SHRT_MAX,      SHRT_MIN });

  volatile VI e = (VI) { INT_MAX - 4, INT_MAX - 5, INT_MAX - 13, INT_MAX };
  volatile VI f = (VI) { 4,	      6,	   13,	         1 };
  volatile VI m = f + e;
  checkvi (m,	  (VI) { INT_MAX,     INT_MIN,     INT_MAX,      INT_MIN });
  m = e + f;
  checkvi (m,     (VI) { INT_MAX,     INT_MIN,     INT_MAX,      INT_MIN });

  volatile VI2 g = (VI2) { 0, INT_MAX - 2, INT_MAX - 2, 3, 3, 4,  INT_MAX - 13, INT_MAX };
  volatile VI2 h = (VI2) { 5, 2,	   3,		5, 6, -2, 13,	        1 };
  volatile VI2 n = h + g;
  checkvi2 (n,	   (VI2) { 5, INT_MAX,     INT_MIN,     8, 9, 2,  INT_MAX,      INT_MIN });
  n = g + h;
  checkvi2 (n,     (VI2) { 5, INT_MAX,     INT_MIN,     8, 9, 2,  INT_MAX,      INT_MIN });

  volatile VC a2 = k - b;
  checkvc (a2, a);
  volatile VC b2 = k - a;
  checkvc (b2, b);

  volatile VS c2 = l - d;
  checkvs (c2, c);
  volatile VS d2 = l - c;
  checkvs (d2, d);

  volatile VI e2 = m - f;
  checkvi (e2, e);
  volatile VI f2 = m - e;
  checkvi (f2, f);

  volatile VI2 g2 = n - h;
  checkvi2 (g2, g);
  volatile VI2 h2 = n - g;
  checkvi2 (h2, h);

  a	    = (VC) { 0,         SCHAR_MAX / 4, SCHAR_MAX / 4, 3, 2, 3, 4, 5, 0, 7, 1, 2, 3, 4, SCHAR_MAX - 13, SCHAR_MAX };
  b	    = (VC) { SCHAR_MAX, 4,	       5,	      2, 3, 4, 5, 2, 9, 2, 9, 1, 0, 8, 1,	       2 };
  k = a * b;
  checkvc (k, (VC) { 0,		124,	       -101,	      6, 6,12,20,10, 0,14, 9, 2, 0,32, SCHAR_MAX - 13, -2 });

  c	    = (VS) { 0,		SHRT_MAX / 8, SHRT_MAX / 7, 5, 8, 9, SHRT_MAX - 10, SHRT_MAX };
  d	    = (VS) { SHRT_MAX,  8,	      17,	    2, 3, 4, 1,		    3 };
  l = c * d;
  checkvs (l, (VS) { 0,		32760,	      14041,	   10,24,36, SHRT_MAX - 10, 32765 });

  e	    = (VI) { INT_MAX,	INT_MAX / 5,  INT_MAX / 6, INT_MAX };
  f	    = (VI) { 0,		5,	      7,	   2 };
  m = e * f;
  checkvi (m, (VI) { 0,		2147483645,   -1789569709, -2 });

  g	    = (VI2) { INT_MAX,	INT_MAX / 9,  INT_MAX / 8, 5, 6, 7, 8, INT_MAX };
  h	    = (VI2) { 0,	10,	      8,	   2, 3, 4, 5, 1 };
  n = g * h;
  checkvi2 (n,(VI2) { 0,	-1908874356,  2147483640, 10,18,28,40, INT_MAX });

  a	    = (VC) { 5, 7, 8, 9, SCHAR_MAX, SCHAR_MIN, 24, 32, 0, 1, 2, 3, 4, 5, SCHAR_MAX, SCHAR_MIN };
  k = -a;
  checkvc (k, (VC) {-5,-7,-8,-9,-SCHAR_MAX, SCHAR_MIN,-24,-32, 0,-1,-2,-3,-4,-5,-SCHAR_MAX, SCHAR_MIN });

  c	    = (VS) { 0, 7, 23, SHRT_MIN, SHRT_MIN, SHRT_MAX, 2, 5 };
  l = -c;
  checkvs (l, (VS) { 0,-7,-23, SHRT_MIN, SHRT_MIN,-SHRT_MAX,-2,-5 });

  e	    = (VI) { 5, INT_MAX, INT_MIN, INT_MIN };
  m = foo (e);
  checkvi (m, (VI) {-5,-INT_MAX, INT_MIN, INT_MIN });

  g	     = (VI2) { 10, 11, 0, INT_MAX - 2, 1, INT_MIN + 1, 5, INT_MIN };
  n = -g;
  checkvi2 (n, (VI2) {-10,-11, 0,-INT_MAX + 2,-1, INT_MAX,    -5, INT_MIN });
  return 0;
}

/* { dg-output "signed integer overflow: 3 \\+ 125 cannot be represented in type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 125 \\+ 3 cannot be represented in type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 3 \\+ 32765 cannot be represented in type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 32765 \\+ 3 cannot be represented in type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 6 \\+ 2147483642 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 2147483642 \\+ 6 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 3 \\+ 2147483645 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 2147483645 \\+ 3 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -128 - 3 cannot be represented in type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -128 - 125 cannot be represented in type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -32768 - 3 cannot be represented in type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -32768 - 32765 cannot be represented in type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -2147483648 - 6 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -2147483648 - 2147483642 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -2147483648 - 3 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: -2147483648 - 2147483645 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 31 \\* 5 cannot be represented in type 'signed char'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 4681 \\* 17 cannot be represented in type 'short int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 357913941 \\* 7 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*signed integer overflow: 238609294 \\* 10 cannot be represented in type 'int'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*negation of -128 cannot be represented in type 'signed char'; cast to an unsigned type to negate this value to itself\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*negation of -32768 cannot be represented in type 'short int'; cast to an unsigned type to negate this value to itself\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*negation of -2147483648 cannot be represented in type 'int'; cast to an unsigned type to negate this value to itself\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*negation of -2147483648 cannot be represented in type 'int'; cast to an unsigned type to negate this value to itself" } */
