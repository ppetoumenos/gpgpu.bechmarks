__kernel void A(__global float* a, __global float* b, __global float* c, const int d) {
  const int e = get_global_id(0);
  const int f = get_global_id(1);

  if (e < c) {
    a[e] += 1.0f;
  }
}