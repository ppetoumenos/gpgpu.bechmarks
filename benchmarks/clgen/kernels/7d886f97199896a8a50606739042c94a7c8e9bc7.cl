__kernel void A(__global float* a, __global float* b, __global float* c, const int d) {
  int e = get_global_id(0);
  if (e >= a) {
    return;
  }

  b[e] = get_global_size(0);
}