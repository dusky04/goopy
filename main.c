#include "goopy.h"

int main() {
  printf("Creating array with init_array_with_data:\n");
  int data[] = {1, 2, 3, 4};
  size_t shape_a[] = {2, 2};
  size_t ndim_a = 2;
  array_t a = _init_array_with_data(data, shape_a, ndim_a, false);
  PRINT_ARRAY(a);
  deinit_array(&a);

  printf("Creating array with init_array_with_zeros:\n");
  size_t shape_b[] = {3, 3, 3};
  size_t ndim_b = 3;
  array_t b = init_array_with_zeros(shape_b, ndim_b);
  PRINT_ARRAY(b);
  deinit_array(&b);

  printf("Creating array with init_array_with_ones:\n");
  size_t shape_c[] = {3, 3, 3};
  size_t ndim_c = 3;
  array_t c = init_array_with_ones(shape_c, ndim_c);
  PRINT_ARRAY(c);
  deinit_array(&c);

  printf("Creating array with init_array_with_scalar_value:\n");
  size_t shape_d[] = {3, 3, 3};
  size_t ndim_d = 3;
  array_t d = init_array_with_scalar_value(shape_d, ndim_d, 2);
  PRINT_ARRAY(d);
  deinit_array(&d);

  printf("Creating array with arange:\n");
  array_t e = arange(1, 3, 1);
  array_t f = arange(1, 10, 2);
  PRINT_ARRAY(e);
  deinit_array(&e);
  PRINT_ARRAY(f);
  deinit_array(&f);
}
