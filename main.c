#include "goopy.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main() {

  // TESTCASE - tranpose()

  array_t a = arange(1, 7, 1);
  reshape(&a, (size_t[]){3, 2}, 2);
  PRINT_ARRAY(a);
  printf("\nBefore transpose shape: ");
  for (size_t i = 0; i < a.ndim; i++)
    printf("%zu ", a.shape[i]);

  printf("\nStrides:");
  for (size_t i = 0; i < a.ndim; i++)
    printf("%zu ", a.strides[i]);

  transpose(&a);
  printf("\n-----------------------\n");
  PRINT_ARRAY(a);

  printf("\nAfter transpose shape: ");
  for (size_t i = 0; i < a.ndim; i++)
    printf("%zu ", a.shape[i]);

  printf("\nStrides:");
  for (size_t i = 0; i < a.ndim; i++)
    printf("%zu ", a.strides[i]);

  deinit_array(&a);
}
