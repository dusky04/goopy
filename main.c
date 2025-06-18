#include "goopy.h"
#include <stddef.h>
#include <stdio.h>

int main() {

  // TESTCASE - tranpose()

  array_t a = arange(1, 5, 1);
  array_t b = arange(1, 2, 1);

  printf("--------------------------\n");
  printf("a: ");
  PRINT_ARRAY(a);
  printf("\n");

  printf("Shape of a: ");
  for (size_t i = 0; i < a.ndim; i++)
    printf("%zu\n", a.shape[i]);

  printf("Strides of a: ");
  for (size_t i = 0; i < a.ndim; i++)
    printf("%zu\n", a.strides[i]);

  printf("--------------------------\n");
  printf("b: ");
  PRINT_ARRAY(b);
  printf("\n");

  printf("Shape of b: ");
  for (size_t i = 0; i < b.ndim; i++)
    printf("%zu\n", b.shape[i]);

  printf("Strides of b: ");
  for (size_t i = 0; i < b.ndim; i++)
    printf("%zu\n", b.strides[i]);
  printf("--------------------------\n");

  array_t c = element_wise_add(&a, &b);
  printf("c: ");
  c.strides = (size_t[]){1};
  PRINT_ARRAY(c);
  printf("\n");

  deinit_array(&a);
  deinit_array(&b);
}
