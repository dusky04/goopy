#include "goopy.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main() {

  // TESTCASE - tranpose()

  array_t a = arange(1, 25, 1);
  reshape(&a, (size_t[]){4, 3, 2, 1}, 4);
  PRINT_ARRAY(a);

  flatten(&a);
  PRINT_ARRAY(a);

  deinit_array(&a);
}
