__kernel void A(__global float* a, __global float* b, const unsigned int c) {
  unsigned int d = get_group_id(0);
  unsigned int e = get_num_groups(0);
  float16 f = (float16)(0.0);
  for (unsigned int g = 0; g < c; g++) {
    float16 h = a[g];
    f.s0 += h.s0;
    f.s1 += h.s1;
    f.s2 += h.s2;
    f.s3 += h.s3;
    f.s4 += h.s4;
    f.s5 += h.s5;
    f.s6 += h.s6;
    f.s7 += h.s7;
    f.s8 += h.s8;
    f.s9 += h.s9;
    f.sA += h.sA;
    f.sB += h.sB;
    f.sC += h.sC;
    f.sD += h.sD;
    f.sE += h.sE;
    f.sF += h.sF;
  }
  b[d] = f.s0 + f.s1 + f.s2 + f.s3 + f.s4 + f.s5 + f.s6 + f.s7 + f.s8 + f.s9 + f.sA + f.sB + f.sC + f.sD + f.sE + f.sF;
}