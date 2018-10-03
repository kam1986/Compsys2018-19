#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "support.h"
#include "wires.h"
#include "arithmetic.h"
#include "memory.h"
#include "registers.h"
#include "ip_reg.h"
#include "compute.h"

// major opcodes
#define RETURN         0x0
#define REG_ARITHMETIC 0x1
#define REG_MOVQ       0x2
#define REG_MOVQ_MEM   0x3
#define CFLOW          0x4
#define IMM_ARITHMETIC 0x5
#define IMM_MOVQ       0x6
#define IMM_MOVQ_MEM   0x7
#define LEAQ2          0x8
#define LEAQ3          0x9
#define LEAQ6          0xA
#define LEAQ7          0xB
#define IMM_CBRANCH    0xF

#define JMP 0xF
#define CALL 0xE

int main(int argc, char* argv[]) {
    // Check command line parameters.
    if (argc < 2)
        error("missing name of programfile to simulate");

    if (argc < 3)
        error("Missing starting address (in hex notation)");

    /*** SETUP ***/
    // We set up global state through variables that are preserved between
    // cycles.

    // Program counter / Instruction Pointer
    ip_reg_p ip = ip_reg_create();

    // Register file:
    reg_p regs = regs_create();

    // Memory:
    // Shared memory for both instructions and data.
    mem_p mem = memory_create();
    memory_read_from_file(mem, argv[1]);

    int start;
    int scan_res = sscanf(argv[2],"%x", &start);
    if (scan_res != 1)
        error("Unable to interpret starting address");

    if (argc == 4) { // tracefile specified
        memory_tracefile(mem, argv[3]);
        regs_tracefile(regs, argv[3]);
        ip_reg_tracefile(ip, argv[3]);
    }
    ip_write(ip, from_int(start), true);

    // a stop signal for stopping the simulation.
    bool stop = false;

    while (!stop) { // for each cycle:
        val pc = ip_read(ip);

        // NO CHANGES BEFORE THIS LINE

        printf("%lx\n", pc.val);  // <---- You may want to silence this
        /*** FETCH ***/

        // We're fetching 10 bytes in the form of 10 vals with one byte each
        val inst_bytes[10];
        memory_read_unaligned(mem, pc, inst_bytes, true);

        /*** DECODE ***/
        // read 4 bit values
        val major_op = pick_bits(4,  4, inst_bytes[0]);
        val minor_op = pick_bits(0,  4, inst_bytes[0]); // not actually used yet

        val reg_d = pick_bits(4, 4, inst_bytes[1]);
        val reg_s = pick_bits(0, 4, inst_bytes[1]);

        // decode instruction type
        // read major operation code
        bool is_return = is(RETURN, major_op);
        // Add further decoding from here

        // determine instruction size - we only understand return, so fix it at 2
        val ins_size = from_int(2);

        // control signals for memory access - you will want to change these
        bool is_load = false;
        bool is_store= false;

        // setting up register read and write - you will want to change these
        val reg_read_dz = reg_d; // for return we just need reg_d
        // - other read port is always reg_s
        // - write is always to reg_d
        bool reg_wr_enable = false;

        /*** EXECUTE ***/
        // read registers
        val reg_out_a = reg_read(regs, reg_read_dz);
        val reg_out_b = reg_read(regs, reg_s);

        // perform calculations - Return needs no calculation. you will want to change this.
        // Here you should hook up a call to compute_execute with all the proper
        // arguments in place
        val compute_result = from_int(0); // you will want to change this.

        // succeeding instruction in memory
        val pc_inc  = add(pc, ins_size);

        // determine the next position of the program counter - you'll want to change this
        // to handle more instructions. Here we only distinguish between return and all other insns
        val pc_next = or(use_if(is_return, reg_out_b),
                         use_if(!is_return, pc_inc));

        /*** MEMORY ***/
        // read from memory if needed
        // (Not used for simulating return, so "mem_out" will be unused initially)
        val mem_out = memory_read(mem, compute_result, is_load);

        /*** WRITE ***/
        // choose result to write back to register
        val datapath_result = from_int(0); // no result for return - you will want to change this

        // NO CHANGES NEEDED AFTER THIS LINE

        // write to register if needed
        reg_write(regs, reg_d, datapath_result, reg_wr_enable);

        // write to memory if needed
        memory_write(mem, compute_result, reg_out_a, is_store);

        // update program counter
        ip_write(ip, pc_next, true);

        // terminate when setting pc to 0
        if (pc_next.val == 0) stop = true;
    }
    memory_destroy(mem);
    regs_destroy(regs);

    printf("Done\n");
}
