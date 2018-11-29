#include "x86prime_lib.h"

void quick_sort(long array[], long lo, long hi) {
  if (lo >= hi) return;

  long mid = (lo + hi) >> 1;
  long pivot = array[mid];
  long left = lo - 1;
  long right = hi + 1;
  while (1) {
    do {
      left++;
    } while (array[left] < pivot);
    do {
      right--;
    } while (array[right] > pivot);

    if (left >= right) {
      break;
    }
    Swap(array, left, right);
  }
  quick_sort(array, lo, right);
  quick_sort(array, right + 1, hi);
}

long* run() {
  init_allocator();
  // Read it the program should print
  // 0: Silent mode
  // 1: Print sorted list
  // 2: Get input from stdin
  // 3: Test mode (print + input)
  long mode = read_long();
  long is_printing = mode & 1;
  long get_input = mode & 2;
  // Read number of elements in array
  long num_entries = read_long();
  long* p = get_random_array(num_entries);
  // Run the algorithm
  if (get_input) {
    p = read_array(num_entries);
  }
  else {
    p = get_random_array(num_entries);
  }

  quick_sort(p, 0, num_entries - 1);

  if (is_printing) {
    print_array(num_entries, p);
  }
  return p; // <-- prevent optimization
}
