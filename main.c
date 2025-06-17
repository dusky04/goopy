#include "goopy.h"

int main() {
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  size_t shape_a[] = {2, 2, 2, 2};
  size_t ndim_a = 4;
  array_t a = _init_array_with_data(data, shape_a, ndim_a, false);
  PRINT_ARRAY(a);

  int data_b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  size_t shape_b[] = {2, 2, 2, 2};
  size_t ndim_b = 4;
  array_t b = _init_array_with_data(data_b, shape_b, ndim_b, false);
  PRINT_ARRAY(b);

  array_t out = matmul(&a, &b);

  printf("-----------------------\n");
  PRINT_ARRAY(out);

  deinit_array(&a);
  deinit_array(&b);
  deinit_array(&out);
}
