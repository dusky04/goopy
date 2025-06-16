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
array_t init_array_with_data(int *data, size_t shape[], size_t ndim);

// Formatting Functions
void _print_array(array_t *arr, size_t cur_depth, size_t offset);
#define PRINT_ARRAY(arr) _print_array(&arr, 0, 0);

// Utils
void _calc_array_strides(array_t *arr);
size_t numel(array_t *arr);
