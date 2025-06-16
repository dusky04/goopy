#include "goopy.h"
#include <stddef.h>
#include <stdio.h>

int main() {
  printf("Creating array with init_array_with_data:\n");
  int data[] = {1, 2, 3, 4, 5, 6};
  size_t shape_a[] = {1, 3, 2};
  size_t ndim_a = 3;
  array_t a = init_array_with_data(data, shape_a, ndim_a);
  PRINT_ARRAY(a);

  printf("Creating array with init_array_with_zeros:\n");
  size_t shape_b[] = {3, 3, 3};
  size_t ndim_b = 3;
  array_t b = init_array_with_zeros(shape_b, ndim_b);
  PRINT_ARRAY(b);

  printf("Creating array with init_array_with_ones:\n");
  size_t shape_c[] = {3, 3, 3};
  size_t ndim_c = 3;
  array_t c = init_array_with_ones(shape_c, ndim_c);
  PRINT_ARRAY(c);

  printf("Creating array with init_array_with_scalar_value:\n");
  size_t shape_d[] = {3, 3, 3};
  size_t ndim_d = 3;
  array_t d = init_array_with_scalar_value(shape_d, ndim_d, 2);
  PRINT_ARRAY(d);

  printf("Creating array with arange:\n");
  // size_t shape_e[] = {3, 3, 3};
  // size_t ndim_e = 3;
  // array_t e = arange(1, 3, 1);
  array_t f = arange(1, -10, 2);
  // PRINT_ARRAY(e);
  PRINT_ARRAY(f);
}
