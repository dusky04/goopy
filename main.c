#include "goopy.h"
#include <stddef.h>
#include <stdio.h>

int main() {

  // TESTCASE - tranpose()
  array_t a = arange(1, 7, 1);
  reshape(&a, (size_t[]){1, 2, 3}, 3);
  PRINT_ARRAY(a);

  array_t b = arange(1, 7, 1);
  reshape(&b, (size_t[]){3, 2, 1}, 3);
  PRINT_ARRAY(b);

  // array_t c = init_array_with_zeros((size_t[]){3, 2, 3}, 3);
  array_t c = element_wise_mul(&a, &b);
  PRINT_ARRAY(c);
}
