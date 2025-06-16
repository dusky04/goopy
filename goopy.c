#include "goopy.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t _numel(size_t *shape, size_t ndim) {
  size_t num_elements = 1;
  for (size_t i = 0; i < ndim; i++)
    num_elements *= shape[i];
  return num_elements;
}

void _calc_array_strides(array_t *arr) {
  // stride is the number of bytes to skip over
  // to get to the next element in that dimension
  // for a one dimensional array, it is just the size of the element
  arr->strides = malloc(sizeof(size_t) * arr->ndim);
  arr->strides[arr->ndim - 1] = 1;
  for (int i = arr->ndim - 2; i > -1; i--) {
    arr->strides[i] = arr->strides[i + 1] * arr->shape[i + 1];
  }
}

array_t init_array_with_data(int *data, size_t *shape, size_t ndim) {
  // TODO: Add a check for
  // number of elements in the data <= number of elements calculated from shape
  array_t arr = {.data = data, .shape = shape, .ndim = ndim};
  _calc_array_strides(&arr);
  return arr;
}

// FIX: Switch to a iterative algorithm
// FIX: Update the array pointer rather than using [] syntax
void _print_array(array_t *arr, size_t cur_depth, size_t offset) {
  if (cur_depth == arr->ndim - 1) {
    // dimension small enough that we can print each element
    printf("[");
    for (size_t i = 0; i < arr->shape[cur_depth]; i++) {
      size_t cur_offset = i + (arr->strides[cur_depth] * offset);
      printf("%d ", arr->data[cur_offset]);
    }
    printf("]");
    return;
  }
  // we are the nth dimension, iterate over all the elements in this dimension
  printf("[");
  for (size_t i = 0; i < arr->shape[cur_depth]; i++) {
    size_t new_offset = offset + (i * arr->strides[cur_depth]);
    _print_array(arr, cur_depth + 1, new_offset);
  }
  printf("]\n");
}

array_t init_array_with_zeros(size_t *shape, size_t ndim) {
  size_t num_elements = _numel(shape, ndim);
  int *data = calloc(num_elements, sizeof(int));
  return init_array_with_data(data, shape, ndim);
}

// LOOK: not a pretty function, see if something can be done
// CLEAN THIS TWO FUNCTIONS
array_t init_array_with_ones(size_t *shape, size_t ndim) {
  size_t num_elements = _numel(shape, ndim);
  int *data = malloc(num_elements * sizeof(int));
  for (size_t i = 0; i < num_elements; i++)
    data[i] = 1;
  return init_array_with_data(data, shape, ndim);
}

array_t init_array_with_scalar_value(size_t *shape, size_t ndim, int value) {
  size_t num_elements = _numel(shape, ndim);
  int *data = malloc(num_elements * sizeof(int));
  for (size_t i = 0; i < num_elements; i++)
    data[i] = value;
  return init_array_with_data(data, shape, ndim);
}

array_t arange(int start, int stop, int step) {
  if (stop < start) {
    fprintf(stderr,
            "ERROR: 'stop' value (%d) must be greater than or equal to 'start' "
            "value (%d) in arange().\n",
            stop, start);
  }

  int num_elements = (int)ceil((float)(stop - start) / (float)step);
  int *data = malloc(num_elements * sizeof(int));
  for (int i = 0; i < num_elements; i++) {
    data[i] = start + (i * step);
  }
  return init_array_with_data(data, (size_t[]){num_elements}, 1);
}
