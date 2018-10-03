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
#define RETURN         0x0 // return              - size 2
#define REG_ARITHMETIC 0x1 // reg       ->  reg   - size 2
#define REG_MOVQ       0x2 // reg       ->  reg   - size 2
#define REG_MOVQ_MEM   0x3 // reg       <-> mem   - size 2
#define CFLOW          0x4 // ??
#define IMM_ARITHMETIC 0x5 // imm       ->  reg   - size 6
#define IMM_MOVQ       0x6 // imm       ->  reg   - size 6
#define IMM_MOVQ_MEM   0x7 // imm mem   <-> reg   - size 6
#define LEAQ2          0x8 // reg       ->  reg   - size 2
#define LEAQ3          0x9 // reg       ->  reg   - size 3
#define LEAQ6          0xA // reg       ->  reg   - size 6
#define LEAQ7          0xB // reg       ->  reg   - size 7
#define IMM_CBRANCH    0xF // 

// minor opcodes memory
#define MEM_FROM_REG         0x1 // reg      -> mem
#define IMM_TO_REG           0x4 // imm      -> reg
#define IMM_Reg_FROM_MEM     0x5 // reg      -> imm mem
#define REG_FROM_MEM         0x9 // mem      -> reg
#define IMM_MEM_FROM_REG     0xD // imm mem  -> reg


// minor opcodes
#define JMP     0xF
#define CALL    0xE



#define SIZE2  from_int(2)
#define SIZE3  from_int(3)
#define SIZE6  from_int(6)
#define SIZE7  from_int(7)
#define SIZE10 from_int(10)

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
        /* 2 */ bool is_return          = is(RETURN, major_op);
        
        // Add further decoding from here - DONE

        bool is_reg_arithmetic  = is(REG_ARITHMETIC, major_op);
        bool is_reg_movq        = is(REG_MOVQ, major_op);
        bool is_reg_movq_mem    = is(REG_MOVQ_MEM, major_op);
        //bool is_cflow           = is(CFLOW, major_op);
        bool is_imm_arithmetic  = is(IMM_ARITHMETIC, major_op);
        bool is_imm_movq        = is(IMM_MOVQ, major_op);
        bool is_imm_movq_mem    = is(IMM_MOVQ_MEM, major_op);
        bool is_leaq2           = is(LEAQ2, major_op);
        bool is_leaq3           = is(LEAQ3, major_op);
        bool is_leaq6           = is(LEAQ6, major_op);
        bool is_leaq7           = is(LEAQ7, major_op);

        val is_size2  =
            use_if(is_reg_arithmetic 
                || is_reg_movq 
                || is_reg_movq_mem 
                || is_leaq2 
                || is_return, SIZE2);

        val is_size3  = use_if(is_leaq3, SIZE3);
        val is_size6  = 
            use_if(is_leaq6
                || is_imm_arithmetic
                || is_imm_movq
                || is_imm_movq_mem, SIZE6);

        val is_size7  = use_if(is_leaq7, SIZE7);
        val is_size10 = use_if(false, SIZE10);// not implementet yet

        val ins_size =
            or(is_size2,
                or(is_size3,
                    or(is_size6,
                        or(is_size7, is_size10)
                    )
                )
            );


        // extra values that might (not) be needed
        val shiftamount = 
            use_if(is_leaq3 || is_leaq7, pick_bits(0, 4, inst_bytes[2]));
        
        val reg_z =
            use_if(is_leaq3 || is_leaq7, pick_bits(4, 4, inst_bytes[2]));

        // first pp .. 32 .. pp or ii .. 32 .. ii argument
        val arg1 = 
            or(
                use_if(is_size6.val == 6 || is_size10.val == 10,
                    add(put_bits(24, 8, inst_bytes[2]),
                        add(put_bits(16, 8, inst_bytes[3]),
                            add(put_bits(8,8,inst_bytes[4]),
                                put_bits(0,8,inst_bytes[5])
                            )
                        )
                    )
                ),
                use_if(is_size7.val == 7,
                    add(put_bits(24, 8, inst_bytes[3]),
                        add(put_bits(16, 8, inst_bytes[4]),
                            add(put_bits(8,8,inst_bytes[5]),
                                put_bits(0,8,inst_bytes[6])
                            )
                        )
                    )
                )
            );
            
        // second pp .. 32 .. pp argument not used yet
        /* val arg2 = 
            use_if(is_size10.val == 10,
                add(put_bits(24, 8, inst_bytes[6]),
                    add(put_bits(16, 8, inst_bytes[7]),
                        add(put_bits(8,8,inst_bytes[8]),
                            put_bits(0,8,inst_bytes[9])
                        )
                    )
                )
            );
        */

        // control signals for memory access - you will want to change these
        bool is_load  = (is_reg_movq_mem && is(REG_FROM_MEM, minor_op)) 
                     || (is_imm_movq_mem && is(IMM_Reg_FROM_MEM, minor_op));
                
        bool is_store = (is_reg_movq_mem && is(MEM_FROM_REG, minor_op)) 
                     || (is_imm_movq_mem && is(IMM_MEM_FROM_REG, minor_op)); 

        // control signal for immediate value
        bool is_imm = is_imm_arithmetic 
                   || is_imm_movq 
                   || is_imm_movq_mem 
                   || is_leaq6
                   || is_leaq7;

        // setting up register read and write - you will want to change these
        val reg_read_dz = 
            or(
                use_if(is_size3.val == 3 || is_size7.val == 7, reg_z),
                use_if(!(is_size3.val == 3 || is_size7.val == 7), reg_d)
            );


        // - other read port is always reg_s
        // - write is always to reg_d
        bool reg_wr_enable = !(is_store || is_load);
        // ^ above compute if we only use regs ^

        /*** EXECUTE ***/
        // read registers
        val reg_out_a = reg_read(regs, reg_read_dz);
        val reg_out_b = reg_read(regs, reg_s);

        // perform calculations - Return needs no calculation. you will want to change this.
        // Here you should hook up a call to compute_execute with all the proper
        // arguments in place
        val compute_result = 
            compute_execute(
                reg_out_a,                                 // either register d or z
                reg_out_b,                                 // always register s
                arg1,                                      // immediate value
                true,                                      // should be changed
                true,                                      // should be changed
                is_imm,                                    // if immediate value is used
                shiftamount,                               // amount to shift
                !(is_imm_arithmetic || is_reg_arithmetic), // if not arithmetic
                minor_op,
                minor_op
            ).result; // you will want to change this.

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
        val datapath_result = 
            or(
                use_if(is_load, mem_out),
                use_if(!is_load, compute_result)
            ); // no result for return - you will want to change this

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
