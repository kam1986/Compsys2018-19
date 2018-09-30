/*
  Memory elements

  Memory resizes dynamically to accomodate usage.

  For aligned access, addresses are word addresses.
  For unaligned access, addresses are byte addresses.
  TODO: More generous description of aligned vs. unaligned access.
*/

#include "wires.h"

struct memory;
typedef struct memory *mem_p;

mem_p memory_create();
void memory_destroy(mem_p);
void memory_read_from_file(mem_p, const char* filename);

// set a tracefile for validation of memory writes
void memory_tracefile(mem_p mem, const char* filename);

// return value in selected cell (NB! quadword addressing *)
val memory_read(mem_p, val address, bool enable);

// read 10 bytes unaligned, for instruction fetch (NB! Byte addressing *)
void memory_read_unaligned(mem_p, val address, val bytes[], bool enable);


// update selected cell with new value at rising edge of clock
// there are no internal forwarding from write to read in same clock period
void memory_write(mem_p, val address, val value, bool wr_enable);
