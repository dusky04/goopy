#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// TODO: implement arena allocation for better ux

typedef enum {
  GOOPY_INT32,
  GOOPY_INT64,
  GOOPY_FLOAT32,
  GOOPY_FLOAT64
} array_type;

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
  // data type
  array_type dtype;
  // size of each element in the data buffer
  size_t itemsize;
  // determine if this it a view
  bool owns;
} array_t;

// LOOK: Maybe introduce array views which do not own any memory
// all the memory passed into a view will have to be externally managed

// Array Initialisation Functions
array_t _init_array_with_data(int *data, size_t *shape, size_t ndim, bool owns);
array_t _init_array_with_data_and_strides(int *data, size_t *shape,
                                          size_t *strides, size_t ndim,
                                          bool owns);
array_t init_array_with_scalar_value(size_t *shape, size_t ndim, int value);
array_t init_array_with_zeros(size_t *shape, size_t ndim);
array_t init_array_with_ones(size_t *shape, size_t ndim);
// TODO: maybe implement init_array_with_value({2, 2}, {1, 2})
// -> [[1, 2], [1, 2]] can pass in non-scalar values
array_t arange(int start, int stop, int step); // 1D function

// Array View Funcions
// array_t init_array_view(int*data, size)
array_t _init_broadcast_view(array_t *a, size_t *target_shape,
                             size_t target_ndim);

// Arithmetic Functions - supports broadcasting
array_t element_wise_add(array_t *a, array_t *b);
array_t element_wise_sub(array_t *a, array_t *b);
array_t element_wise_mul(array_t *a, array_t *b);
array_t element_wise_div(array_t *a, array_t *b);

array_t matmul(array_t *a, array_t *b); // Does not suppport broadcasting yet

// Array Utility Functions
void reshape(array_t *arr, size_t *new_shape, size_t new_ndim);
void transpose(array_t *arr);
void flatten(array_t *arr);

// Formatting Functions
void _print_array(array_t *arr, size_t cur_depth, size_t offset);
#define PRINT_ARRAY(arr) _print_array(&arr, 0, 0);

// Utility Functions
size_t _numel(size_t *shape, size_t ndim);
void _calc_array_strides(array_t *arr);
bool _check_equal_ndims(array_t *a, array_t *b);
bool _check_equal_shapes(array_t *a, array_t *b);
bool _check_broadcastable_shapes(array_t *a, array_t *b);

// Cleanup Functions
void deinit_array(array_t *arr);
