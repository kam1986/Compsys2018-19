#include "wires.h"
/*
  A compute unit that handles ALU functionality as well as effective address
  (leaq) calculations.
 */

// Encoding of ALU operations
#define ADD  0
#define SUB  1
#define AND  2
#define OR   3
#define XOR  4
#define MUL  5

// Encoding of conditions
#define E  0x0
#define NE 0x1
#define L  0x4
#define LE 0x5
#define G  0x6
#define GE 0x7
#define A  0x8
#define AE 0x9
#define B  0xA
#define BE 0xB

typedef struct {
    val result;
    bool cond_met;
} compute_execute_result;

compute_execute_result compute_execute(
    val op_z_or_d,        // operand from register selected by z or d field
    val op_s,             // operand from register selected by s field
    val imm,              // inline immediate operand
    bool sel_z_or_d,      // enable op_z_or_d for effective address computation
    bool sel_s,           // enable or_s for effective address computation
    bool sel_imm,         // enable inline immediate for effective address computation
    val shift_amount,     // shift amount for effective address computation
    bool use_agen,        // pick result from effective address computation - if not pick from ALU
    val alu_op,           // operation requested from ALU, from 'aaaa' field in instructions
    val condition         // condition to evalue, from 'cccc' field in instructions
);
/*
  this returns a struct with two values:
    result - the result of the calculation
    cond_met - true if the condition was met, otherwise false
--------------------------------------------------------------------------------
  for example, to calculate 40 - 13, call with:

    a = from_int(13)
    b = from_int(40)
    sel_a   = true
    sel_b   = true
    sel_imm = false
    is_leaq = false
    alu_op  = SUB

  the rest of the parameters don't matter in this instance
*/

