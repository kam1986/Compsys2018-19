#include "compute.h"
#include "arithmetic.h"

bool bool_xor(bool a, bool b) { return a^b; }
bool bool_not(bool a) { return !a; }

val shift_left(val a, val v) { return from_int(a.val << v.val); }

val mul(val a, val b) { return from_int(a.val * b.val); }

typedef struct {
    bool cf;
    bool of;
    bool zf;
    bool sf;
} conditions;

bool eval_condition(conditions c, val op) { // from fig 3.15, p. 242,
    // modified to take into account that we have the operand order reversed
    bool zf = c.zf;
    bool sf = c.sf;
    bool of = c.of;
    bool cf = c.cf;

    bool res_e  = is(E, op)  & zf;
    bool res_ne = is(NE, op) & bool_not(zf);
/*
    bool res_l  = is(L, op)  & bool_xor(sf, of);
    bool res_le = is(LE, op) & (bool_xor(sf, of) || zf);
    bool res_g  = is(G, op)  & bool_not(bool_xor(sf, of)) & bool_not(zf);
    bool res_ge = is(GE, op) & bool_not(bool_xor(sf, of));
*/
    bool res_g  = is(G, op)  & bool_xor(sf, of);
    bool res_ge = is(GE, op) & (bool_xor(sf, of) || zf);
    bool res_l  = is(L, op)  & bool_not(bool_xor(sf, of)) & bool_not(zf);
    bool res_le = is(LE, op) & bool_not(bool_xor(sf, of));
/*
    bool res_a  = is(A, op)  & bool_not(cf || zf);
    bool res_ae = is(AE, op) & bool_not(cf);
    bool res_b  = is(B, op)  & cf;
    bool res_be = is(BE, op) & (cf || zf);
*/
    bool res_b  = is(B, op)  & bool_not(cf || zf);
    bool res_be = is(BE, op) & bool_not(cf);
    bool res_a  = is(A, op)  & cf;
    bool res_ae = is(AE, op) & (cf || zf);
    return res_e || res_ne || res_l || res_le || res_g || res_ge
        || res_a || res_ae || res_b || res_be;
}

typedef struct {
    val result;
    conditions cc;
} alu_execute_result;

alu_execute_result alu_execute(val op, val op_a, val op_b) {
    alu_execute_result result;
    bool is_sub = is(SUB, op);
    bool is_add = is(ADD, op);
    val val_b = or( use_if(!is_sub, op_b),
                use_if( is_sub, neg(64, op_b)));
    generic_adder_result adder_result = generic_adder(op_a, val_b, is_sub);
    bool adder_cf = adder_result.cf;
    val adder_res = adder_result.result;
    val res = or(use_if(is_add, adder_res),
              or(use_if(is_sub, adder_res),
              or(use_if(is(AND, op), and(op_a, op_b)),
              or(use_if(is(OR, op),  or(op_a, op_b)),
              or(use_if(is(MUL, op), mul(op_a, op_b)),
              use_if(is(XOR, op), xor(op_a, op_b)))))));

    bool a_sign = pick_one(63, op_a);
    bool b_sign = pick_one(63, val_b);
    bool r_sign = pick_one(63, res);
    bool overflow = (!(a_sign || b_sign) &&   r_sign) ||
                    (  a_sign && b_sign  && (!r_sign));

    result.cc.sf = r_sign;
    result.cc.zf = !reduce_or(res);
    result.cc.of = (is_sub || is_add) && overflow;
    result.cc.cf = (is_sub || is_add) && adder_cf;
    result.result = res;
    return result;
}

compute_execute_result compute_execute(val op_z_or_d, val op_s, val imm,
                                       bool sel_z_or_d, bool sel_s, bool sel_imm,
                                       val shift_amount, bool use_agen,
                                       val alu_op, val condition) {
    val val_a = use_if(sel_z_or_d, op_z_or_d);
    val val_b = use_if(sel_s, op_s);
    val val_imm = use_if(sel_imm, imm);

    val shifted_a = shift_left(val_a, shift_amount);

    val effective_addr = add(add(shifted_a, val_b), val_imm);

    val alu_input_b = or(val_b, val_imm);
    alu_execute_result alu_result = alu_execute(alu_op, op_z_or_d, alu_input_b);
    bool cond_met = eval_condition(alu_result.cc, condition);

    val result = or(use_if(!use_agen, alu_result.result),
                    use_if( use_agen, effective_addr));

    compute_execute_result retval;
    retval.result = result;
    retval.cond_met = cond_met;
    return retval;
}
