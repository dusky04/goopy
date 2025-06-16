#include "goopy.h"

int main() {
  int data[] = {1, 2, 3, 4, 5, 6};
  size_t shape[] = {1, 3, 2};
  size_t ndim = 3;

  array_t arr = init_array_with_data(data, shape, ndim);
  PRINT_ARRAY(arr);
}
