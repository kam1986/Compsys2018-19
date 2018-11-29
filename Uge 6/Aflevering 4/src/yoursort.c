#include "x86prime_lib.h"

void your_sort(long num_elem, long array[]) {
  return;
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

  // Run the algorithm
  your_sort(num_entries, p);

  if (is_printing) {
    print_array(num_entries, p);
  }
  return p; // <-- prevent optimization
}
