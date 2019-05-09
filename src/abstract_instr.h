#ifndef __ABSTRACT_INSTR_H_
#define __ABSTRACT_INSTR_H_

#include "instr.h"
#include "reg.h"
#include "str_slice.h"
#include "vec.h"

/**
 * Abstract instructions
 * nop = skipped
 * add $d $s $t = $d <- $s + $t    -- binop
 * addi $t $s imm = $t <- $s + imm -- binop
 * andi $t $s imm = $t <- $s & imm -- binop
 * srl $d $t h = d <- t >> h       -- binop
 * srl $d $t h = d <- t >> h       -- binop
 * beq $s $t o = branch eq $s $t o -- branch
 * bne $s $t o = branch ne $s $t o -- branch
 */

enum __attribute__((__packed__)) abstract_storage_type {
    ABSTRACT_STORAGE_REG,
    ABSTRACT_STORAGE_IMM
};

extern const char *const abstract_storage_type_names[];

struct abstract_storage {
    enum abstract_storage_type type;
    union {
        uint16_t imm;
        enum reg_type reg;
    };
};

// TODO: implement abstract load instructions
// TODO: optimise d <- 0 + t into d <- t
enum __attribute__((__packed__)) abstract_instr_type {
    ABSTRACT_INSTR_BINOP,
    ABSTRACT_INSTR_BRANCH
};

extern const char *const abstract_instr_type_names[];

enum __attribute__((__packed__)) abstract_instr_binop_op {
    ABSTRACT_INSTR_BINOP_ADD,
    ABSTRACT_INSTR_BINOP_AND,
    ABSTRACT_INSTR_BINOP_SRL,
    ABSTRACT_INSTR_BINOP_SLL
};

extern const char *const abstract_instr_binop_op_names[];

struct abstract_instr_binop {
    enum reg_type dest;
    enum abstract_instr_binop_op op;
    struct abstract_storage lhs, rhs;
};

enum __attribute__((__packed__)) abstract_instr_branch_test_type {
    ABSTRACT_INSTR_BRANCH_TEST_NE,
    ABSTRACT_INSTR_BRANCH_TEST_EQ
};

extern const char *const abstract_instr_branch_test_type_names[];

struct abstract_instr_branch {
    enum abstract_instr_branch_test_type type;
    struct abstract_storage lhs, rhs;
    struct string_slice label; // we still use branch labels at this point
};

struct abstract_instr {
    enum abstract_instr_type type;
    union {
        struct abstract_instr_binop binop;
        struct abstract_instr_branch branch;
    };
};

/**
 * Translate MIPS instructions into our abstract instructions.
 */
struct abstract_instr_vec *translate_instructions(struct instr_vec *instrs);

void print_abstract_instr(struct abstract_instr i);

DEFINE_VEC(struct abstract_instr, abstract_instr);

#endif // __ABSTRACT_INSTR_H_