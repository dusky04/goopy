#include "goopy.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t shape[] = {2, 2, 2};
  int32_t data[] = {1, 2, 3, 4, 5, 6, 7, 8};
  array_t a = _init_array_with_data(data, shape, 3, GOOPY_INT32, false);
  PRINT_ARRAY(a);

  printf("-----------------------------------------------------------------\n");
  printf("B ARRAY\n");
  int32_t data2[] = {1, 2, 3, 4};
  array_t b =
      _init_array_with_data(data2, (size_t[]){2, 2}, 2, GOOPY_INT32, false);
  PRINT_ARRAY(b);
  printf(
      "\n-----------------------------------------------------------------\n");

  printf("C ARRAY\n");

  array_t c = element_wise_add(&a, &b);
  PRINT_ARRAY(c);
}

// int main() {

//   // TESTCASE - tranpose()
//   array_t a = arange(0, 10, 1, GOOPY_INT32);
//   reshape(&a, (size_t[]){10}, 1);
//   // PRINT_ARRAY(a);

//   array_t b = arange(0, 10, 1, GOOPY_INT32);
//   reshape(&b, (size_t[]){10}, 1);
//   // PRINT_ARRAY(b);

//   // array_t c = init_array_with_zeros((size_t[]){3, 2, 3}, 3);
//   array_t c = element_wise_add(&a, &b);
//   PRINT_ARRAY(c);
// }
