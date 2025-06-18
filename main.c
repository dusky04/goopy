#include "goopy.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main() {

  // TESTCASE - tranpose()

  array_t a = arange(1, 25, 1);
  array_t b = arange(1, 25, 1);

  PRINT_ARRAY(a);
  printf("\n");
  PRINT_ARRAY(b);
  printf("\n");

  array_t c = element_wise_add(&a, &b);
  PRINT_ARRAY(c);
  printf("\n");

  deinit_array(&a);
  deinit_array(&b);
  deinit_array(&c);
}
