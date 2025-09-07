#include "goopy.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

__attribute__((always_inline)) static inline int __add(int a, int b) {
  return a + b;
}
__attribute__((always_inline)) static inline int __sub(int a, int b) {
  return a - b;
}
__attribute__((always_inline)) static inline int __mul(int a, int b) {
  return a * b;
}
__attribute__((always_inline)) static inline int __div(int a, int b) {
  return a / b;
}

// ----------------------------------------------------------------
// Utility Functions
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

bool _check_equal_ndims(array_t *a, array_t *b) { return a->ndim == b->ndim; }

bool _check_equal_shapes(array_t *a, array_t *b) {
  return (a->ndim == b->ndim &&
          memcmp(a->shape, b->shape, a->ndim * sizeof(size_t)) == 0);
}

bool _check_broadcastable_shapes(array_t *a, array_t *b) {
  for (int i = MIN(a->ndim, b->ndim) - 1; i > -1; i--) {
    if (!(a->shape[i] == b->shape[i] || a->shape[i] == 1 || b->shape[i] == 1))
      return false;
  }
  return true;
}

size_t get_size_dtype(array_type dtype) {
  switch (dtype) {
  case GOOPY_INT32:
    return sizeof(int32_t);
  case GOOPY_INT64:
    return sizeof(int64_t);
  case GOOPY_FLOAT32:
    return sizeof(float);
  case GOOPY_FLOAT64:
    return sizeof(double);
  default:
    fprintf(stderr, "ERROR: Unknown data type");
    return 0;
  }
}

// ----------------------------------------------------------------
// Formatting Functions

// introduce a function pointer to handle printing different values
// NOTE: maybe we can improve this code
typedef void (*PrinterFn)(void *data, size_t offset);

static inline void _print_i32(void *data, size_t offset) {
  printf("%d ", ((int32_t *)data)[offset]);
}
static inline void _print_i64(void *data, size_t offset) {
  printf("%ld ", ((int64_t *)data)[offset]);
}
static inline void _print_f32(void *data, size_t offset) {
  printf("%.4f ", ((float *)data)[offset]);
}
static inline void _print_f64(void *data, size_t offset) {
  printf("%.4g ", ((double *)data)[offset]);
}

static PrinterFn _printers[GOOPY_TYPES_COUNT] = {
    [GOOPY_INT32] = _print_i32,
    [GOOPY_INT64] = _print_i64,
    [GOOPY_FLOAT32] = _print_f32,
    [GOOPY_FLOAT64] = _print_f64,
};

// FIX: Switch to a iterative algorithm
// FIX: Update the array pointer rather than using [] syntax
void _print_array(array_t *arr, size_t cur_depth, size_t offset) {
  if (cur_depth == arr->ndim - 1) {
    // dimension small enough that we can print each element
    printf("[");
    for (size_t i = 0; i < arr->shape[cur_depth]; i++) {
      size_t cur_offset = offset + (arr->strides[cur_depth] * i);
      _printers[arr->dtype](arr->data, cur_offset);
    }
    printf("]");
    return;
  }
  // we are the nth dimension, iterate over all the elements in this
  // dimension
  printf("[");
  for (size_t i = 0; i < arr->shape[cur_depth]; i++) {
    size_t new_offset = offset + (i * arr->strides[cur_depth]);
    _print_array(arr, cur_depth + 1, new_offset);
  }
  printf("]\n");
}

// ----------------------------------------------------------------

// static void _broadcast_binary_op(array_t *a, array_t *b, array_t *c, int
// depth,
//                                  size_t offset_a, size_t offset_b,
//                                  size_t offset_c, int (*op)(int, int)) {
//   if (depth == (int)c->ndim - 1) {
//     for (size_t i = 0; i < c->shape[depth]; i++) {
//       size_t base_a = offset_a + i * a->strides[depth];
//       size_t base_b = offset_b + i * b->strides[depth];
//       size_t base_c = offset_c + i * c->strides[depth];
//       c->data[base_c] = op(a->data[base_a], b->data[base_b]);
//     }
//     return;
//   }
//   // we are at the nth dimension iterate over all the elements
//   for (size_t i = 0; i < c->shape[depth]; i++) {
//     size_t new_offset_a = offset_a + i * a->strides[depth];
//     size_t new_offset_b = offset_b + i * b->strides[depth];
//     size_t new_offset_c = offset_c + (i * c->strides[depth]);
//     _broadcast_binary_op(a, b, c, depth + 1, new_offset_a, new_offset_b,
//                          new_offset_c, op);
//   }
// }

// ----------------------------------------------------------------
// Array Initialisation Functions

array_t _init_array_with_data(void *data, size_t *shape, size_t ndim,
                              array_type dtype, bool owns) {
  // TODO: Add a check for
  // number of elements in the data <= number of elements calculated
  // from shape
  array_t arr;
  arr.data = data;
  arr.owns = owns;
  arr.ndim = ndim;
  arr.dtype = dtype;
  arr.shape = malloc(sizeof(size_t) * ndim);
  memcpy(arr.shape, shape, ndim * sizeof(size_t));
  arr.strides = malloc(sizeof(size_t) * ndim);
  _calc_array_strides(&arr);
  return arr;
}

array_t _init_array_with_data_and_strides(int *data, size_t *shape,
                                          size_t *strides, size_t ndim,
                                          bool owns) {
  array_t arr;
  arr.data = data;
  arr.owns = owns;
  arr.ndim = ndim;
  arr.shape = malloc(sizeof(size_t) * ndim);
  memcpy(arr.shape, shape, ndim * sizeof(size_t));
  arr.strides = malloc(sizeof(size_t) * ndim);
  memcpy(arr.strides, strides, ndim * sizeof(size_t));
  return arr;
}

// LOOK: not a pretty function, see if something can be done
// CHECK: if these 3 functions can be cleaned
array_t init_array_with_scalar_value(size_t *shape, size_t ndim, int value) {
  size_t num_elements = _numel(shape, ndim);
  int *data = malloc(num_elements * sizeof(int));
  for (size_t i = 0; i < num_elements; i++)
    data[i] = value;
  return _init_array_with_data(data, shape, ndim, true, GOOPY_INT32);
}

array_t init_array_with_zeros(size_t *shape, size_t ndim) {
  return init_array_with_scalar_value(shape, ndim, 0);
}

array_t init_array_with_ones(size_t *shape, size_t ndim) {
  return init_array_with_scalar_value(shape, ndim, 1);
}

array_t arange(int start, int stop, int step, array_type dtype) {
  if (stop < start) {
    fprintf(stderr,
            "ERROR: 'stop' value (%d) must be greater than or equal to "
            "'start' "
            "value (%d) in arange().\n",
            stop, start);
  }

  int num_elements = (int)ceil((float)(stop - start) / (float)step);
  int *data = malloc(num_elements * get_size_dtype(dtype));
  for (int i = 0; i < num_elements; i++) {
    data[i] = start + (i * step);
  }
  return _init_array_with_data(data, (size_t[]){num_elements}, 1, true,
                               GOOPY_INT32);
}

size_t *_calc_broadcast_shape(array_t *a, array_t *b, size_t c_ndim) {
  size_t *result_shape = malloc(sizeof(size_t) * c_ndim);

  for (int i = c_ndim - 1; i >= 0; i--) {
    int a_idx = (int)a->ndim - (c_ndim - i);
    int b_idx = (int)b->ndim - (c_ndim - i);

    size_t a_dim = (a_idx >= 0) ? a->shape[a_idx] : 1;
    size_t b_dim = (b_idx >= 0) ? b->shape[b_idx] : 1;
    result_shape[i] = MAX(a_dim, b_dim);
  }
  return result_shape;
}

// array_t _init_broadcast_view(array_t *a, size_t *target_shape,
//                              size_t target_ndim) {
//   array_t view = _init_array_with_data(a->data, target_shape, target_ndim,
//                                        false, GOOPY_INT32);

//   for (int i = target_ndim - 1; i >= 0; i--) {
//     int idx = (int)a->ndim - (target_ndim - i);
//     if (idx >= 0) {
//       view.shape[i] = a->shape[idx];
//       view.strides[i] = (a->shape[idx] == 1) ? 0 : a->strides[idx];
//     } else {
//       view.shape[i] = target_shape[i];
//       view.strides[i] = 0;
//     }
//   }
//   return view;
// }

// // TODO: implement an efficient broadcasting algorithm
// array_t element_wise_add(array_t *a, array_t *b) {
//   // TODO: Add a check for data types
//   if (_check_equal_shapes(a, b)) {
//     int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
//     for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
//       data[i] = a->data[i] + b->data[i];
//     return _init_array_with_data(data, a->shape, a->ndim, true, GOOPY_INT32);
//   }

//   if (!_check_broadcastable_shapes(a, b)) {
//     fprintf(stderr, "ERROR: Arrays with incompatible shapes cannot be "
//                     "broadcast together.\n");
//     exit(EXIT_FAILURE);
//   }

//   size_t c_ndim = MAX(a->ndim, b->ndim);
//   size_t *c_shape = _calc_broadcast_shape(a, b, c_ndim);

//   array_t view_a = _init_broadcast_view(a, c_shape, c_ndim);
//   array_t view_b = _init_broadcast_view(b, c_shape, c_ndim);

//   int *c_data = malloc(sizeof(int) * _numel(c_shape, c_ndim));
//   array_t c = _init_array_with_data(c_data, c_shape, c_ndim, true,
//   GOOPY_INT32);

//   _broadcast_binary_op(&view_a, &view_b, &c, 0, 0, 0, 0, __add);

//   free(view_a.shape);
//   free(view_a.strides);
//   free(view_b.shape);
//   free(view_b.strides);
//   free(c_shape);
//   return c;
// }

// array_t element_wise_sub(array_t *a, array_t *b) {
//   if (_check_equal_shapes(a, b)) {
//     int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
//     for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
//       data[i] = a->data[i] - b->data[i];
//     return _init_array_with_data(data, a->shape, a->ndim, true, GOOPY_INT32);
//   }

//   if (!_check_broadcastable_shapes(a, b)) {
//     fprintf(stderr, "ERROR: Arrays with incompatible shapes cannot be "
//                     "broadcast together.\n");
//     exit(EXIT_FAILURE);
//   }

//   size_t c_ndim = MAX(a->ndim, b->ndim);
//   size_t *c_shape = _calc_broadcast_shape(a, b, c_ndim);

//   array_t view_a = _init_broadcast_view(a, c_shape, c_ndim);
//   array_t view_b = _init_broadcast_view(b, c_shape, c_ndim);

//   int *c_data = malloc(sizeof(int) * _numel(c_shape, c_ndim));
//   array_t c = _init_array_with_data(c_data, c_shape, c_ndim, true,
//   GOOPY_INT32);

//   _broadcast_binary_op(&view_a, &view_b, &c, 0, 0, 0, 0, __sub);

//   free(view_a.shape);
//   free(view_a.strides);
//   free(view_b.shape);
//   free(view_b.strides);
//   free(c_shape);
//   return c;
// }

// array_t element_wise_mul(array_t *a, array_t *b) {
//   if (_check_equal_shapes(a, b)) {
//     int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
//     for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
//       data[i] = a->data[i] * b->data[i];
//     return _init_array_with_data(data, a->shape, a->ndim, true, GOOPY_INT32);
//   }

//   if (!_check_broadcastable_shapes(a, b)) {
//     fprintf(stderr, "ERROR: Arrays with incompatible shapes cannot be "
//                     "broadcast together.\n");
//     exit(EXIT_FAILURE);
//   }

//   size_t c_ndim = MAX(a->ndim, b->ndim);
//   size_t *c_shape = _calc_broadcast_shape(a, b, c_ndim);

//   array_t view_a = _init_broadcast_view(a, c_shape, c_ndim);
//   array_t view_b = _init_broadcast_view(b, c_shape, c_ndim);

//   int *c_data = malloc(sizeof(int) * _numel(c_shape, c_ndim));
//   array_t c = _init_array_with_data(c_data, c_shape, c_ndim, true,
//   GOOPY_INT32);

//   _broadcast_binary_op(&view_a, &view_b, &c, 0, 0, 0, 0, __mul);

//   free(view_a.shape);
//   free(view_a.strides);
//   free(view_b.shape);
//   free(view_b.strides);
//   free(c_shape);
//   return c;
// }

// array_t element_wise_div(array_t *a, array_t *b) {
//   if (_check_equal_shapes(a, b)) {
//     int *data = malloc(sizeof(int) * _numel(a->shape, a->ndim));
//     for (size_t i = 0; i < _numel(a->shape, a->ndim); i++)
//       data[i] = a->data[i] / b->data[i];
//     return _init_array_with_data(data, a->shape, a->ndim, true, GOOPY_INT32);
//   }

//   if (!_check_broadcastable_shapes(a, b)) {
//     fprintf(stderr, "ERROR: Arrays with incompatible shapes cannot be "
//                     "broadcast together.\n");
//     exit(EXIT_FAILURE);
//   }

//   size_t c_ndim = MAX(a->ndim, b->ndim);
//   size_t *c_shape = _calc_broadcast_shape(a, b, c_ndim);

//   array_t view_a = _init_broadcast_view(a, c_shape, c_ndim);
//   array_t view_b = _init_broadcast_view(b, c_shape, c_ndim);

//   int *c_data = malloc(sizeof(int) * _numel(c_shape, c_ndim));
//   array_t c = _init_array_with_data(c_data, c_shape, c_ndim, true,
//   GOOPY_INT32);

//   _broadcast_binary_op(&view_a, &view_b, &c, 0, 0, 0, 0, __div);

//   free(view_a.shape);
//   free(view_a.strides);
//   free(view_b.shape);
//   free(view_b.strides);
//   free(c_shape);
//   return c;
// }

// // TODO: implement getting single element from a array to clean up this
// // code but VERY MUCH LATER DOWN the LINE
// static void _matmul_2D(array_t *a, array_t *b, array_t *c, size_t offset_a,
//                        size_t offset_b, size_t offset_c) {
//   size_t m = a->shape[a->ndim - 2];
//   size_t n = a->shape[a->ndim - 1];
//   size_t q = b->shape[b->ndim - 1];

//   // iterate over the rows of matrix a
//   for (size_t i = 0; i < m; i++) {
//     // iterate over the columns of matrix b
//     for (size_t j = 0; j < q; j++) {
//       int sum = 0;
//       for (size_t k = 0; k < n; k++) {
//         size_t ai = offset_a + i * a->strides[a->ndim - 2] +
//                     k * a->strides[a->ndim - 1];
//         size_t bi = offset_b + k * b->strides[b->ndim - 2] +
//                     j * b->strides[b->ndim - 1];
//         sum += a->data[ai] * b->data[bi];
//       }

//       size_t ci =
//           offset_c + i * c->strides[c->ndim - 2] + j * c->strides[c->ndim -
//           1];
//       c->data[ci] = sum;
//     }
//   }
// }

// static void _matmul(array_t *a, array_t *b, array_t *c, size_t offset_a,
//                     size_t offset_b, size_t offset_c, int depth) {
//   if (depth == (int)c->ndim - 2) {
//     _matmul_2D(a, b, c, offset_a, offset_b, offset_c);
//     return;
//   }

//   // we are at the nth dimension, move over every element in this
//   // dimension
//   for (size_t i = 0; i < c->shape[depth]; i++) {
//     size_t new_offset_a = offset_a + i * a->strides[depth];
//     size_t new_offset_b = offset_b + i * b->strides[depth];
//     size_t new_offset_c = offset_c + i * c->strides[depth];
//     _matmul(a, b, c, new_offset_a, new_offset_b, new_offset_c, depth + 1);
//   }
// }

// // TODO: research into if this could be made cache-friendlier
// array_t matmul(array_t *a, array_t *b) {
//   // shape of cols of mat a should match shape of rows of mat b
//   size_t m = a->shape[a->ndim - 2];
//   size_t n = a->shape[a->ndim - 1];
//   size_t p = b->shape[b->ndim - 2];
//   size_t q = b->shape[b->ndim - 1];

//   if (n != p) {
//     fprintf(stderr,
//             "ERROR: Cannot multiply matrices: number of columns in the "
//             "first "
//             "matrix (%zu) does not match number of rows in the second "
//             "matrix "
//             "(%zu).\n",
//             n, p);
//     exit(EXIT_FAILURE);
//   }
//   // calculate the new shape
//   size_t *c_shape = malloc(sizeof(size_t) * a->ndim);
//   for (size_t i = 0; i < a->ndim - 2; i++)
//     c_shape[i] = a->shape[i];
//   c_shape[a->ndim - 2] = m;
//   c_shape[a->ndim - 1] = q;
//   array_t c = init_array_with_zeros(c_shape, a->ndim);

//   _matmul(a, b, &c, 0, 0, 0, 0);
//   free(c_shape);
//   return c;
// }

// TODO: perhaps return a arrat view
// or return a new array_t
void reshape(array_t *arr, size_t *new_shape, size_t new_ndim) {
  size_t new_num_elements = _numel(new_shape, new_ndim);
  size_t old_num_elements = _numel(arr->shape, arr->ndim);
  if (new_num_elements != old_num_elements) {
    fprintf(stderr,
            "ERROR: Cannot reshape array of size %zu into shape with %zu "
            "elements.\n",
            old_num_elements, new_num_elements);
    exit(EXIT_FAILURE);
  }

  // TODO: add a check to only realloc with the ndims change
  arr->ndim = new_ndim;
  arr->shape = realloc(arr->shape, new_ndim * sizeof(size_t));
  arr->strides = realloc(arr->strides, new_ndim * sizeof(size_t));
  for (size_t i = 0; i < arr->ndim; i++)
    arr->shape[i] = new_shape[i];
  _calc_array_strides(arr);
}

// TODO: SHOULD return a array view
// or return a new array_t
// FIX: make it faster
void transpose(array_t *arr) {
  // free(arr->shape);
  size_t *old_shape = arr->shape;
  arr->shape = malloc(sizeof(size_t) * arr->ndim);
  for (size_t i = 0; i < arr->ndim; i++)
    arr->shape[i] = old_shape[arr->ndim - i - 1];

  size_t *old_strides = arr->strides;
  arr->strides = malloc(sizeof(size_t) * arr->ndim);
  for (size_t i = 0; i < arr->ndim; i++)
    arr->strides[i] = old_strides[arr->ndim - i - 1];
  // TODO: add a calculate backstrides function
  // _calc_array_strides(arr);
  free(old_shape);
  free(old_strides);
}

// TODO: return an array view
void flatten(array_t *arr) {
  size_t num_elements = _numel(arr->shape, arr->ndim);
  arr->ndim = 1;
  arr->shape = realloc(arr->shape, sizeof(size_t));
  arr->shape[0] = num_elements;
  arr->strides = realloc(arr->strides, sizeof(size_t));
  arr->strides[0] = 1;
}

// LOOK: issue of double free
void deinit_array(array_t *arr) {
  if (arr->owns)
    free(arr->data);
  free(arr->shape);
  free(arr->strides);
}

#undef MIN
#undef MAX
