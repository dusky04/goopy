#include "goopy.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  // TESTCASE - tranpose()
  array_t a = arange(0, 10 * 10 * 10, 1);
  reshape(&a, (size_t[]){10, 10, 10}, 3);
  // PRINT_ARRAY(a);

  array_t b = arange(0, 10 * 10 * 10, 1);
  reshape(&b, (size_t[]){10, 10, 10}, 3);
  // PRINT_ARRAY(b);

  // array_t c = init_array_with_zeros((size_t[]){3, 2, 3}, 3);
  array_t c = element_wise_add(&a, &b);
  PRINT_ARRAY(c);
}
