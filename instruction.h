#pragma once

#include <stdint.h>

typedef struct r_instruction {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t funct7 : 7;
} r_instruction;

typedef struct i_instruction {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t imm : 12;
} i_instruction;

typedef struct s_instruction {
    uint32_t opcode : 7;
    uint32_t imm0 : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm1 : 7;
} s_instruction;

typedef struct b_instruction {
    uint32_t opcode : 7;
    uint32_t imm0 : 5;
    uint32_t funct3 : 3;
    uint32_t rs1 : 5;
    uint32_t rs2 : 5;
    uint32_t imm1 : 7;
} b_instruction;

typedef struct u_instruction {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm : 20;
} u_instruction;

typedef struct j_instruction {
    uint32_t opcode : 7;
    uint32_t rd : 5;
    uint32_t imm : 20;
} j_instruction;

typedef struct e_instruction {
    uint32_t opcode : 7;

}
