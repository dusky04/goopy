#include "goopy.h"

int main() {
  int data[] = {1, 2, 3, 4, 5, 6};
  size_t shape_a[] = {2, 3};
  size_t ndim_a = 2;
  array_t a = _init_array_with_data(data, shape_a, ndim_a, false);
  PRINT_ARRAY(a);

  int data_b[] = {1, 2, 3, 4, 5, 6};
  size_t shape_b[] = {3, 2};
  size_t ndim_b = 2;
  array_t b = _init_array_with_data(data_b, shape_b, ndim_b, false);
  PRINT_ARRAY(b);

  array_t out = init_array_with_zeros((size_t[]){2, 2}, 2);
  _matmul_2D(&a, &b, &out);
  PRINT_ARRAY(out);

  deinit_array(&a);
  deinit_array(&b);
  deinit_array(&out);
}
