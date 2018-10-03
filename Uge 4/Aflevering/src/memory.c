#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "trace_read.h"
#include "support.h"

typedef uint64_t word;

struct block {
    word start_addr;
    word *data;
};

struct memory {
    struct block* blocks;
    int num_blocks;
    trace_p tracer;
};

mem_p memory_create() {
    mem_p res = (mem_p) malloc(sizeof(struct memory));
    res->num_blocks = 0;
    res->blocks = 0;
    res->tracer = 0;
    return res;
}

void memory_destroy(mem_p mem) {
    for (int i = 0; i < mem->num_blocks; ++i) {
        free(mem->blocks[i].data);
    }
    free(mem->blocks);
    if (mem->tracer) {
        if (!trace_all_matched(mem->tracer))
            error("Parts of trace for memory writes was not matched");
        trace_reader_destroy(mem->tracer);
    }
    free(mem);
}

void memory_tracefile(mem_p mem, const char* filename) {
    mem->tracer = trace_reader_create('M', filename);
}

struct block* get_block(mem_p mem, word start_addr) {
    struct block* bp = 0;
    for (int i = 0; i < mem->num_blocks; ++i) {
        if (mem->blocks[i].start_addr == start_addr) {
            bp = &mem->blocks[i];
            break;
        }
    }
    if (bp == 0) {
        mem->blocks = realloc(mem->blocks, (mem->num_blocks + 1) * sizeof(struct block));
        mem->blocks[mem->num_blocks].data = malloc(sizeof(word) * 256);
        mem->blocks[mem->num_blocks].start_addr = start_addr;
        bp = &mem->blocks[mem->num_blocks];
        mem->num_blocks++;
    }
    return bp;
}

/* uses word addresses */
word memory_read_quad(mem_p mem, word start_word_addr) {
    word offset_in_block = start_word_addr & 0x0FF;
    word start_of_block = start_word_addr - offset_in_block;
    struct block* bp = get_block(mem, start_of_block);
    return bp->data[offset_in_block];
}

void memory_write_quad(mem_p mem, word start_word_addr, word quad) {
    word offset_in_block = start_word_addr & 0x0FF;
    word start_of_block = start_word_addr - offset_in_block;
    struct block* bp = get_block(mem, start_of_block);
    bp->data[offset_in_block] = quad;
}

/* uses byte addresses */
int memory_read_byte(mem_p mem, word byte_addr) {
    int byte_number = byte_addr &0x7;
    word word_addr = byte_addr >> 3;
    word quad = memory_read_quad(mem, word_addr);
    int bit_number = byte_number * 8;
    return (quad >> bit_number) & 0x0FF;
}

void memory_write_byte(mem_p mem, word byte_addr, int byte) {
    int byte_number = byte_addr &0x7;
    word word_addr = byte_addr >> 3;
    word quad = memory_read_quad(mem, word_addr);
    int bit_number = byte_number * 8;
    word mask = ~(0x0FFULL << bit_number);
    byte &= 0x0FF;
    quad &= mask;
    quad |= ((word)byte) << bit_number;
    memory_write_quad(mem, word_addr, quad);
}

void error(const char*);

void memory_read_from_file(mem_p mem, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
	error("Failed to open file");
    }
    int res;
    do {
        word addr;
        char buf[21]; // most we'll need, plus terminating 0
        res = fscanf(f, "%lx : ", &addr);
        if (res != EOF) {
            res = fscanf(f, " %[0123456789ABCDEFabcdef]", buf);
            if (res == 1) {
                //printf("%lx : %s\n", addr, buf);
                char* p = buf;
                while (*p != 0) {
                    // convert byte by byte (= 2 char at a time)
                    char buf2[3];
                    buf2[0] = *p++;
                    buf2[1] = *p++;
                    buf2[2] = 0;
                    int byte_from_hex;
                    sscanf(buf2, "%x", &byte_from_hex);
                    memory_write_byte(mem, addr, byte_from_hex);
                    int check = memory_read_byte(mem, addr);
                    if (check != byte_from_hex)
                      printf("Memory error: at %lx, wrote %x, read back %x\n", addr, byte_from_hex, check);
                    addr++;
                }
            }
            fscanf(f,"#");
            while ('\n' != getc(f));
        }
    } while (res != EOF);
    fclose(f);
}

// return value in selected quadword (use quadword addresses)
val memory_read(mem_p mem, val address, bool enable) {
    if (enable)
        return from_int(memory_read_quad(mem, address.val));
    else
        return from_int(0);
}

// read 10 bytes from memory, unaligned, uses byte addressing
void memory_read_unaligned(mem_p mem, val address, val bytes[], bool enable) {
    word addr = address.val;
    for (int i = 0; i < 10; ++i) {
        if (enable)
            bytes[i] = from_int(memory_read_byte(mem, addr + i));
        else
            bytes[i] = from_int(0);
    }
}

// update selected cell with new value at rising edge of clock
// there are no internal forwarding from write to read in same clock period
void memory_write(mem_p mem, val address, val value, bool wr_enable) {
    if (wr_enable) {
        if (trace_match_next(mem->tracer, address, value))
            memory_write_quad(mem, address.val, value.val);
        else
            error("Trace mismatch on write to memory");
    }
}

