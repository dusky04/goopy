#include "goopy.h"

#include <math.h>
#include <stdbool.h>
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
  arr->strides[arr->ndim - 1] = 1;
  for (int i = arr->ndim - 2; i > -1; i--) {
    arr->strides[i] = arr->strides[i + 1] * arr->shape[i + 1];
  }
}

array_t _init_array_with_data(int *data, size_t *shape, size_t ndim,
                              bool owns) {
  // TODO: Add a check for
  // number of elements in the data <= number of elements calculated from shape
  array_t arr;
  arr.data = data;
  arr.owns = owns;
  arr.ndim = ndim;
  arr.shape = malloc(sizeof(size_t) * ndim);
  memcpy(arr.shape, shape, ndim * sizeof(size_t));
  arr.strides = malloc(sizeof(size_t) * ndim);
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

// LOOK: not a pretty function, see if something can be done
// CHECK: if these 3 functions can be cleaned
array_t init_array_with_scalar_value(size_t *shape, size_t ndim, int value) {
  size_t num_elements = _numel(shape, ndim);
  int *data = malloc(num_elements * sizeof(int));
  for (size_t i = 0; i < num_elements; i++)
    data[i] = value;
  return _init_array_with_data(data, shape, ndim, true);
}

array_t init_array_with_zeros(size_t *shape, size_t ndim) {
  return init_array_with_scalar_value(shape, ndim, 0);
}

array_t init_array_with_ones(size_t *shape, size_t ndim) {
  return init_array_with_scalar_value(shape, ndim, 0);
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
  return _init_array_with_data(data, (size_t[]){num_elements}, 1, true);
}

array_t element_wise_add(array_t *a, array_t *b) {
  if (a->ndim != b->ndim ||
      memcmp(a->shape, b->shape, a->ndim * sizeof(size_t)) != 0) {
    fprintf(stderr,
            "ERROR: Currently cannot add arrays with different shapes. "
            "a->ndim=%zu, "
            "b->ndim=%zu\n",
            a->ndim, b->ndim);
    exit(EXIT_FAILURE);
  }
  int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
  for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
    data[i] = a->data[i] + b->data[i];
  return _init_array_with_data(data, a->shape, a->ndim, true);
}

array_t element_wise_sub(array_t *a, array_t *b) {
  if (a->ndim != b->ndim ||
      memcmp(a->shape, b->shape, a->ndim * sizeof(size_t)) != 0) {
    fprintf(stderr,
            "ERROR: Currently cannot add arrays with different shapes. "
            "a->ndim=%zu, "
            "b->ndim=%zu\n",
            a->ndim, b->ndim);
    exit(EXIT_FAILURE);
  }
  int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
  for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
    data[i] = a->data[i] - b->data[i];
  return _init_array_with_data(data, a->shape, a->ndim, true);
}

array_t element_wise_mul(array_t *a, array_t *b) {
  if (a->ndim != b->ndim ||
      memcmp(a->shape, b->shape, a->ndim * sizeof(size_t)) != 0) {
    fprintf(stderr,
            "ERROR: Currently cannot add arrays with different shapes. "
            "a->ndim=%zu, "
            "b->ndim=%zu\n",
            a->ndim, b->ndim);
    exit(EXIT_FAILURE);
  }
  int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
  for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
    data[i] = a->data[i] * b->data[i];
  return _init_array_with_data(data, a->shape, a->ndim, true);
}

array_t element_wise_div(array_t *a, array_t *b) {
  if (a->ndim != b->ndim ||
      memcmp(a->shape, b->shape, a->ndim * sizeof(size_t)) != 0) {
    fprintf(stderr,
            "ERROR: Currently cannot add arrays with different shapes. "
            "a->ndim=%zu, "
            "b->ndim=%zu\n",
            a->ndim, b->ndim);
    exit(EXIT_FAILURE);
  }
  int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
  for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
    data[i] = a->data[i] / b->data[i];
  return _init_array_with_data(data, a->shape, a->ndim, true);
}

// LOOK: issue of double free
void deinit_array(array_t *arr) {
  if (arr->owns)
    free(arr->data);
  free(arr->shape);
  free(arr->strides);
}
