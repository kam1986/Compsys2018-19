#include "x86prime_lib.h"

/*
void swap (long array[], long i, long j) {
  long tmp = array[i];
  array[i] = array[j];
  array[j] = tmp; 
}
*/

void dual_pivot_quick_sort(long array[], long lo, long hi) {
  long left = lo;
  long right = hi;

  if (right > left) {

    // Choosing the outermost elements as to be pivots
    if (array[left] > array[right]) {
      Swap(array, left, right);
    }

    // The left pivot and the right pivot
    long pivot1 = array[left];                  // Pivot 1 (P1)
    long pivot2 = array[right];                 // Pivot 2 (P2)

    /* Partitioning:
       
        Pivot1 (P1)            l->                 k->        <-g             Pivot2 (P2)
        +------------------------------------------------------------------------------+
        |   P1   |    < P1    | ︙ P1 <= & <= P2  | ︙    ?    ︙ |    > P2    |   P2   |
        +------------------------------------------------------------------------------+
                   left part       Center part                     right part

        Invariants:
        
                    all in (left   , less   )  <   pivot1
        pivot1  <=  all in (less   , greater)  <=  pivot2
                    all in (greater, right  )  >   pivot2

    */

    // Partitioning array according to invariant below
    long p_low = left   + 1;      // l (less)
    long p_high = right - 1;      // g (greater)

    // k is the first part of ?-part
    long k = left       + 1;      // k


    while (k <= p_high) {
      // Move array[k] to left part
      if (array[k] < pivot1) {
        Swap(array, k, p_low);
        p_low++;
      }
      // Move array[k] to right part
      else if (array[k] > pivot2) {
        while (array[p_high] > pivot2 && k < p_high) {
          p_high--;
        }
        Swap(array, k, p_high);
        p_high--;

        // array[k] <= pivot2
        if (array[k] < pivot1) {
          Swap(array, k, p_low);
          p_low++;
        }
      }
      k++;
    }
    p_low--;
    p_high++;

    // Swapping pivots to final place
    Swap(array, lo, p_low);
    Swap(array, hi, p_high);

    // Sorting left and right parts recursively (recursively sorting partitions)
    dual_pivot_quick_sort(array, lo           , p_low  - 1);
    dual_pivot_quick_sort(array, p_low     + 1, p_high - 1);
    dual_pivot_quick_sort(array, p_high    + 1, hi        );
  }
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

  dual_pivot_quick_sort(p, 0, num_entries - 1);

  if (is_printing) {
    print_array(num_entries, p);
  }
  return p; // <-- prevent optimization
}
