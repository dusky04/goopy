#include "../goopy.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Helper to compare float arrays with a tolerance
void assert_f32_array_equal(array_t *arr, f32 *expected, size_t num_elements) {
  assert(_numel(arr->shape, arr->ndim) == num_elements);
  f32 *arr_data = (f32 *)arr->data;
  for (size_t i = 0; i < num_elements; ++i) {
    assert(fabsf(arr_data[i] - expected[i]) < 1e-6f);
  }
}

// Helper to compare double arrays with a tolerance
void assert_f64_array_equal(array_t *arr, f64 *expected, size_t num_elements) {
  assert(_numel(arr->shape, arr->ndim) == num_elements);
  f64 *arr_data = (f64 *)arr->data;
  for (size_t i = 0; i < num_elements; ++i) {
    assert(fabs(arr_data[i] - expected[i]) < 1e-9);
  }
}

int main() {
  printf("--- Testing Element-wise Addition ---\n");
  {
    // Test i32
    i32 a_data_i32[] = {1, 2, 3, 4};
    array_t a_i32 = init_unowned_i32_array(a_data_i32, (size_t[]){2, 2}, 2);
    i32 b_data_i32[] = {10, 20, 30, 40};
    array_t b_i32 = init_unowned_i32_array(b_data_i32, (size_t[]){2, 2}, 2);
    array_t c_i32 = element_wise_add(&a_i32, &b_i32);
    i32 expected_i32[] = {11, 22, 33, 44};
    assert(memcmp(c_i32.data, expected_i32, sizeof(expected_i32)) == 0);
    deinit_array(&a_i32);
    // deinit_array(&b_i32);
    // deinit_array(&c_i32);
    printf("\033[0;32mPASSED:\033[0m ADD i32 (same shape)\n");

    // Test f32 with broadcasting
    f32 a_data_f32[] = {1.5f, 2.5f, 3.5f, 4.5f};
    array_t a_f32 = init_unowned_f32_array(a_data_f32, (size_t[]){2, 2}, 2);
    f32 b_data_f32[] = {10.0f};
    array_t b_f32 = init_unowned_f32_array(b_data_f32, (size_t[]){1}, 1);
    array_t c_f32 = element_wise_add(&a_f32, &b_f32);
    f32 expected_f32[] = {11.5f, 12.5f, 13.5f, 14.5f};
    assert_f32_array_equal(&c_f32, expected_f32, 4);
    deinit_array(&a_f32);
    deinit_array(&b_f32);
    deinit_array(&c_f32);
    printf("\033[0;32mPASSED:\033[0m ADD f32 (broadcasting)\n");
  }

  printf("--- Testing Element-wise Subtraction ---\n");
  {
    // Test i64
    i64 a_data_i64[] = {100, 200, 300, 400};
    array_t a_i64 = init_unowned_i64_array(a_data_i64, (size_t[]){2, 2}, 2);
    i64 b_data_i64[] = {1, 2, 3, 4};
    array_t b_i64 = init_unowned_i64_array(b_data_i64, (size_t[]){2, 2}, 2);
    array_t c_i64 = element_wise_sub(&a_i64, &b_i64);
    i64 expected_i64[] = {99, 198, 297, 396};
    assert(memcmp(c_i64.data, expected_i64, sizeof(expected_i64)) == 0);
    deinit_array(&a_i64);
    deinit_array(&b_i64);
    deinit_array(&c_i64);
    printf("\033[0;32mPASSED:\033[0m SUB i64 (same shape)\n");

    // Test f64 with broadcasting
    f64 a_data_f64[] = {10.5, 20.5, 30.5, 40.5};
    array_t a_f64 = init_unowned_f64_array(a_data_f64, (size_t[]){2, 2}, 2);
    f64 b_data_f64[] = {0.5, 10.0};
    array_t b_f64 = init_unowned_f64_array(b_data_f64, (size_t[]){1, 2}, 2);
    array_t c_f64 = element_wise_sub(&a_f64, &b_f64);
    f64 expected_f64[] = {10.0, 10.5, 30.0, 30.5};
    assert_f64_array_equal(&c_f64, expected_f64, 4);
    deinit_array(&a_f64);
    deinit_array(&b_f64);
    deinit_array(&c_f64);
    printf("\033[0;32mPASSED:\033[0m SUB f64 (broadcasting)\n");
  }

  printf("--- Testing Element-wise Multiplication ---\n");
  {
    // Test i32 with broadcasting
    i32 a_data_i32[] = {1, 2, 3, 4, 5, 6};
    array_t a_i32 = init_unowned_i32_array(a_data_i32, (size_t[]){2, 3}, 2);
    i32 b_data_i32[] = {10, 100};
    array_t b_i32 = init_unowned_i32_array(b_data_i32, (size_t[]){2, 1}, 2);
    array_t c_i32 = element_wise_mul(&a_i32, &b_i32);
    i32 expected_i32[] = {10, 20, 30, 400, 500, 600};
    assert(memcmp(c_i32.data, expected_i32, sizeof(expected_i32)) == 0);
    deinit_array(&a_i32);
    deinit_array(&b_i32);
    deinit_array(&c_i32);
    printf("\033[0;32mPASSED:\033[0m MUL i32 (broadcasting)\n");

    // Test f32
    f32 a_data_f32[] = {0.5f, 1.5f, 2.0f, 10.0f};
    array_t a_f32 = init_unowned_f32_array(a_data_f32, (size_t[]){2, 2}, 2);
    f32 b_data_f32[] = {2.0f, 4.0f, 0.5f, 0.1f};
    array_t b_f32 = init_unowned_f32_array(b_data_f32, (size_t[]){2, 2}, 2);
    array_t c_f32 = element_wise_mul(&a_f32, &b_f32);
    f32 expected_f32[] = {1.0f, 6.0f, 1.0f, 1.0f};
    assert_f32_array_equal(&c_f32, expected_f32, 4);
    deinit_array(&a_f32);
    deinit_array(&b_f32);
    deinit_array(&c_f32);
    printf("\033[0;32mPASSED:\033[0m MUL f32 (same shape)\n");
  }

  printf("--- Testing Element-wise Division ---\n");
  {
    // Test i64 (integer division)
    i64 a_data_i64[] = {10, 20, 35};
    array_t a_i64 = init_unowned_i64_array(a_data_i64, (size_t[]){3}, 1);
    i64 b_data_i64[] = {2, 7, 5};
    array_t b_i64 = init_unowned_i64_array(b_data_i64, (size_t[]){3}, 1);
    array_t c_i64 = element_wise_div(&a_i64, &b_i64);
    i64 expected_i64[] = {5, 2, 7};
    assert(memcmp(c_i64.data, expected_i64, sizeof(expected_i64)) == 0);
    deinit_array(&a_i64);
    deinit_array(&b_i64);
    deinit_array(&c_i64);
    printf("\033[0;32mPASSED:\033[0m DIV i64 (same shape)\n");

    // Test f64 with broadcasting
    f64 a_data_f64[] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0};
    array_t a_f64 = init_unowned_f64_array(a_data_f64, (size_t[]){2, 3}, 2);
    f64 b_data_f64[] = {100.0};
    array_t b_f64 = init_unowned_f64_array(b_data_f64, (size_t[]){1}, 1);
    array_t c_f64 = element_wise_div(&a_f64, &b_f64);
    f64 expected_f64[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
    assert_f64_array_equal(&c_f64, expected_f64, 6);
    deinit_array(&a_f64);
    deinit_array(&b_f64);
    deinit_array(&c_f64);
    printf("\033[0;32mPASSED:\033[0m DIV f64 (broadcasting)\n");
  }

  return 0;
}
