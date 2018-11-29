/* The following functions bind a few operations to in/out instructions
   which x86prime will intercept and execute
*/
__inline__ long read_long() {
  long result;
  __asm__ volatile ("in (0),%[result]" : [result] "=r" (result) : );
  return result;
}

__inline__ long gen_random() {
  long result;
  __asm__ volatile ("in (1),%[result]" : [result] "=r" (result) : );
  return result;
}

__inline__ void write_long(long value) {
  __asm__ volatile ("out %[value],(0)" : : [value] "r" (value) );
}

/* A rudimentary allocator (you can't free anything)
*/
volatile long* cur_allocator;
long allocator_base;

__inline__ void init_allocator() {
  cur_allocator = &allocator_base;
}

__inline__ long* allocate(long num_entries) {
  volatile long* res = cur_allocator;
  cur_allocator = &cur_allocator[num_entries];
  return (long*) res;
}

/* Create an array of random numbers */
__inline__ long* get_random_array(long num_entries) {
  volatile long* p = allocate(num_entries);
  for (long i = 0; i < num_entries; ++i) {
    p[i] = gen_random();
  }
  return (long*)p;
}

__inline__ long* read_array(long num_entries) {
  volatile long* p = allocate(num_entries);
  for (long i = 0; i < num_entries; ++i) {
    p[i] = read_long();
  }
  return (long*)p;
}

// useful helper
#define Swap(tab,x,y) { long tmp = tab[x]; tab[x] = tab[y]; tab[y] = tmp; }

__inline__ void print_array(long num_elem, long array[]) {

  for (long i = 0; i < num_elem; ++i) {
    write_long(array[i]);
  }
}
