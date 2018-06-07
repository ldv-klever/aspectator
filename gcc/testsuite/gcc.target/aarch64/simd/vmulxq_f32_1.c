/* Test the vmulxq_f32 AArch64 SIMD intrinsic.  */

/* { dg-do run } */
/* { dg-options "-save-temps -O3" } */

#include "arm_neon.h"

extern void abort (void);

void __attribute__ ((noinline))
test_case (float32_t v1[4], float32_t v2[4], float32_t e[4])
{
  int i;
  float32x4_t vec1_1 = vld1q_f32 (v1);
  float32x4_t vec1_2 = vld1q_f32 (v2);

  float32x4_t actual1 = vmulxq_f32 (vec1_1, vec1_2);
  float32_t actual[4];
  vst1q_f32 (actual, actual1);

  for (i = 0; i < 4; ++i)
    if (actual[i] != e[i])
      abort ();
}

int
main (void)
{
  float32_t v1 = 3.14159265359;
  float32_t v2 = 1.383894;
  float32_t v3 = -2.71828;
  float32_t v4 = -3.4891931;

  float32_t v1_1[] = {v1, v2, v3, v4};
  float32_t v1_2[] = {v3, v4, v1, v2};
  float32_t e1[] = {v1 * v3, v2 * v4, v3 * v1, v4 * v2};
  test_case (v1_1, v1_2, e1);

  float32_t v2_1[] = {0, -0.0, 0, -0.0};
  float32_t v2_2[] = {-__builtin_huge_valf (), -__builtin_huge_valf (),
		      __builtin_huge_valf (), __builtin_huge_valf () };
  float32_t e2[] = {-2.0, 2.0, 2.0, -2.0};
  test_case (v2_1, v2_2, e2);

  return 0;
}

/* { dg-final { scan-assembler-times "fmulx\[ \t\]+\[vV\]\[0-9\]+\.4s, ?\[vV\]\[0-9\]+\.4s, ?\[vV\]\[0-9\]+\.4s\n" 1 } } */
