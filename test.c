#include "goopy.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
  // TEST CASE 1: 2x2 matrix multiplication
  printf("----------------------\n");
  printf("TEST CASE 1: 2x2 matrix multiply\n");
  int data_a1[] = {1, 2, 3, 4};
  size_t shape_a1[] = {2, 2};
  array_t a1 = _init_array_with_data(data_a1, shape_a1, 2, false);
  PRINT_ARRAY(a1);

  int data_b1[] = {5, 6, 7, 8};
  size_t shape_b1[] = {2, 2};
  array_t b1 = _init_array_with_data(data_b1, shape_b1, 2, false);
  PRINT_ARRAY(b1);

  array_t out1 = matmul(&a1, &b1);
  int expected1[] = {19, 22, 43, 50};
  assert(memcmp(out1.data, expected1, sizeof(expected1)) == 0);
  PRINT_ARRAY(out1);
  deinit_array(&a1);
  deinit_array(&b1);
  deinit_array(&out1);

  // TEST CASE 2: batched 2x2 matrix multiplication (2 batches)
  printf("----------------------\n");
  printf("TEST CASE 2: Batch 2x2 matrices multiply\n");
  int data_a2[] = {1, 2, 3, 4, 5, 6, 7, 8};
  size_t shape_a2[] = {2, 2, 2};
  array_t a2 = _init_array_with_data(data_a2, shape_a2, 3, false);
  PRINT_ARRAY(a2);

  int data_b2[] = {8, 7, 6, 5, 4, 3, 2, 1};
  size_t shape_b2[] = {2, 2, 2};
  array_t b2 = _init_array_with_data(data_b2, shape_b2, 3, false);
  PRINT_ARRAY(b2);

  array_t out2 = matmul(&a2, &b2);
  int expected2[] = {20, 17, 48, 41, 32, 21, 44, 29};
  assert(memcmp(out2.data, expected2, sizeof(expected2)) == 0);
  PRINT_ARRAY(out2);
  deinit_array(&a2);
  deinit_array(&b2);
  deinit_array(&out2);

  // TEST CASE 3: mismatched dimensions
  // printf("----------------------\n");
  // printf("TEST CASE 3: Mismatched dimensions\n");
  // int data_a3[] = {1, 2, 3, 4, 5, 6};
  // size_t shape_a3[] = {2, 3};
  // array_t a3 = _init_array_with_data(data_a3, shape_a3, 2, false);
  // PRINT_ARRAY(a3);

  // int data_b3[] = {1, 2, 3, 4, 5, 6};
  // size_t shape_b3[] = {2, 3}; // inner dims don't align
  // array_t b3 = _init_array_with_data(data_b3, shape_b3, 2, false);
  // PRINT_ARRAY(b3);

  // array_t out3 = matmul(&a3, &b3);
  // if (out3.data == NULL) {
  //   printf("Test passed: matmul handled mismatched dimensions correctly.\n");
  // } else {
  //   printf("Test failed: expected NULL on error, got non-null data.\n");
  //   deinit_array(&out3);
  // }
  // deinit_array(&a3);
  // deinit_array(&b3);

  // TEST CASE 4: 4D array multiplication
  printf("----------------------\n");
  printf("TEST CASE 4: 4D array multiply\n");
  int data_a4[16];
  for (int i = 0; i < 16; ++i)
    data_a4[i] = i + 1;
  size_t shape_a4[] = {2, 2, 2, 2};
  array_t a4 = _init_array_with_data(data_a4, shape_a4, 4, false);
  PRINT_ARRAY(a4);

  int data_b4[16];
  for (int i = 0; i < 16; ++i)
    data_b4[i] = 16 - i;
  size_t shape_b4[] = {2, 2, 2, 2};
  array_t b4 = _init_array_with_data(data_b4, shape_b4, 4, false);
  PRINT_ARRAY(b4);

  array_t out4 = matmul(&a4, &b4);
  PRINT_ARRAY(out4);
  deinit_array(&a4);
  deinit_array(&b4);
  deinit_array(&out4);

  printf("----------------------\n");

  // TESTCASE - 1
  // reshape() function

  int data[] = {1, 2, 3, 4, 5, 6};
  size_t shape[] = {2, 3};
  size_t ndim = 2;
  array_t a = _init_array_with_data(data, shape, ndim, false);
  PRINT_ARRAY(a);

  size_t new_shape[] = {3, 2};

  reshape(&a, new_shape, ndim);
  PRINT_ARRAY(a);

  deinit_array(&a);

  // TESTCASE - tranpose()

  // array_t a = arange(1, 25, 1);
  // PRINT_ARRAY(a);
  // reshape(&a, (size_t[]){4, 3, 2, 1}, 4);
  // PRINT_ARRAY(a);
  // printf("\nBefore transpose shape: ");
  // for (size_t i = 0; i < a.ndim; i++)
  //   printf("%zu ", a.shape[i]);

  // printf("\nStrides:");
  // for (size_t i = 0; i < a.ndim; i++)
  //   printf("%zu ", a.strides[i]);

  // transpose(&a);
  // printf("\n-----------------------\n");
  // PRINT_ARRAY(a);

  // printf("\nAfter transpose shape: ");
  // for (size_t i = 0; i < a.ndim; i++)
  //   printf("%zu ", a.shape[i]);

  // printf("\nStrides:");
  // for (size_t i = 0; i < a.ndim; i++)
  //   printf("%zu ", a.strides[i]);

  // deinit_array(&a);
}
