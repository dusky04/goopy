# Goopy

A simple, lightweight NumPy-like array library implemented in C for basic multi-dimensional array operations.

## Features

- [x] supports n-dimensional arrays
- [x] broadcasting support for basic arithmetic operations
- [x] batched matrix multiplication

```c
#include <goopy.h>

int main() {

  // TESTCASE - tranpose()
  array_t a = arange(1, 7, 1);
  reshape(&a, (size_t[]){1, 2, 3}, 3);
  PRINT_ARRAY(a);

  array_t b = arange(1, 7, 1);
  reshape(&b, (size_t[]){3, 2, 1}, 3);
  PRINT_ARRAY(b);

  array_t c = element_wise_mul(&a, &b);
  PRINT_ARRAY(c);

  deinit_array(&a);
  deinit_array(&b);
  deinit_array(&c);
  return 0;
}
```

## API Reference

### Array Creation

- `arange(start, stop, step)` - Create 1D array with range of values
- `init_array_with_zeros(shape, ndim)` - Create array filled with zeros
- `init_array_with_ones(shape, ndim)` - Create array filled with ones
- `init_array_with_scalar_value(shape, ndim, value)` - Create array filled with scalar

### Operations

- `element_wise_add(a, b)` - Element-wise addition with broadcasting
- `element_wise_sub(a, b)` - Element-wise subtraction with broadcasting
- `element_wise_mul(a, b)` - Element-wise multiplication with broadcasting
- `element_wise_div(a, b)` - Element-wise division with broadcasting
- `matmul(a, b)` - Matrix multiplication with batch support

### Array Manipulation

- `reshape(arr, new_shape, new_ndim)` - Reshape array
- `transpose(arr)` - Transpose array (reverse all dimensions)
- `flatten(arr)` - Flatten to 1D array

### Utilities

- `PRINT_ARRAY(arr)` - Print array contents
- `deinit_array(arr)` - Free array memory

## Notes

- Arrays use `int` data type only
- Memory management is manual - always call `deinit_array()` for arrays you create
- Broadcasting follows NumPy-like rules
- Matrix multiplication works on the last two dimensions for batched operations
