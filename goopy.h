#pragma once

#include <stddef.h>
#include <stdlib.h>

// TODO: add dtype
typedef struct {
  // pointer to the data
  int *data;
  // represents the dimensions of the data (1D, 2D, ...)
  size_t *shape;
  // the number of bytes to skip in memory to proceed to the next element
  size_t *strides;
  // number of dimensions of the data
  size_t ndim;
} array_t;

// Array Initialisation Functions
array_t init_array_with_data(int *data, size_t *shape, size_t ndim);
array_t init_array_with_zeros(size_t *shape, size_t ndim);
array_t init_array_with_ones(size_t *shape, size_t ndim);
// TODO: implement init_array_with_value({2, 2}, {1, 2}) -> [[1, 2], [1, 2]]
// can pass in non-scalar values
array_t init_array_with_scalar_value(size_t *shape, size_t ndim, int value);
array_t arange(int start, int stop, int step); // 1D function

// Formatting Functions
void _print_array(array_t *arr, size_t cur_depth, size_t offset);
#define PRINT_ARRAY(arr) _print_array(&arr, 0, 0);

// Utils
void _calc_array_strides(array_t *arr);
size_t _numel(size_t *shape, size_t ndim);
