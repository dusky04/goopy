#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// TODO: implement arena allocation for better ux

// TODO: add dtype
// All the members need to be explicitly own
typedef struct {
  // pointer to the data
  int *data;
  // represents the dimensions of the data (1D, 2D, ...)
  size_t *shape;
  // the number of bytes to skip in memory to proceed to the next element
  size_t *strides;
  // number of dimensions of the data
  size_t ndim;
  // TODO: remove this workaround
  bool owns;
} array_t;

// LOOK: Maybe introduce array views which do not own any memory
// all the memory passed into a view will have to be externally managed

// Array Initialisation Functions
array_t _init_array_with_data(int *data, size_t *shape, size_t ndim, bool owns);
// #define INIT_ARRAY_WITH_DATA(data, shape, ndim)
//   _init_array_with_data(data, shape, ndim, false)

array_t init_array_with_zeros(size_t *shape, size_t ndim);
array_t init_array_with_ones(size_t *shape, size_t ndim);
// TODO: maybe implement init_array_with_value({2, 2}, {1, 2})
// -> [[1, 2], [1, 2]] can pass in non-scalar values
array_t init_array_with_scalar_value(size_t *shape, size_t ndim, int value);
array_t arange(int start, int stop, int step); // 1D function

// Arithmetic Functions - rn same shape
void _add(array_t *a, array_t *b, array_t *c, int depth, size_t offset_a,
          size_t offset_b, size_t offset_c);
array_t element_wise_add(array_t *a, array_t *b);
array_t element_wise_sub(array_t *a, array_t *b);
array_t element_wise_mul(array_t *a, array_t *b);
array_t element_wise_div(array_t *a, array_t *b);

array_t matmul(array_t *a, array_t *b);

// Array Utility Functions
void reshape(array_t *arr, size_t *new_shape, size_t new_ndim);
void transpose(array_t *arr);
void flatten(array_t *arr);

// Formatting Functions
void _print_array(array_t *arr, size_t cur_depth, size_t offset);
#define PRINT_ARRAY(arr) _print_array(&arr, 0, 0);

// Utils
void _calc_array_strides(array_t *arr);
size_t _numel(size_t *shape, size_t ndim);
bool _check_equal_shapes(array_t *a, array_t *b);
array_t *_get_smaller_dims_array(array_t *a, array_t *b);

// Cleanup Functions
void deinit_array(array_t *arr);
