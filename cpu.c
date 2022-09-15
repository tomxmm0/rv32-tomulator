#include <memory.h>

#include "opcodes.h"
#include "cpu.h"

#define CPU_DBG(__FMT__, ...) printf("[CPU]: "__FMT__ "\n", __VA_ARGS__)
#define CPU_FAULT(__FMT__, ...) printf("[CPU FAULT]: "__FMT__"\n", __VA_ARGS__); \
    getchar()

#define SIGNEX_BIT_11(n) n | ((n & 0x800) ? 0xfffff800 : 0)
#define SIGNEX_BIT_16(n) n | ((n & 0x8000) ? 0xffff0000 : 0)
#define SIGNEX_BIT_8(n) n | ((n & 0x80) ? 0xffffff00 : 0)

void instr_ADDI(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    cpu->registers[rd] = (int32_t)cpu->registers[rs1] + s_imm;
}

void instr_SLTI(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    if ((int32_t)cpu->registers[rs1] < s_imm)
    {
        cpu->registers[rd] = 1;
    }
    else
    {
        cpu->registers[rd] = 0;
    }
}

void instr_SLTIU(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    if (cpu->registers[rs1] < imm)
    {
        cpu->registers[rd] = 1;
    }
    else
    {
        cpu->registers[rd] = 0;
    }
}

void instr_ANDI(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    cpu->registers[rd] = cpu->registers[rs1] & s_imm;
}

void instr_ORI(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    cpu->registers[rd] = cpu->registers[rs1] | s_imm;
}

void instr_XORI(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    cpu->registers[rd] = cpu->registers[rs1] ^ s_imm;
}

void instr_SLLI(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t shamt)
{
    cpu->registers[rd] = cpu->registers[rs1] << shamt;
}

void instr_SRLI(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t shamt)
{
    cpu->registers[rd] = cpu->registers[rs1] >> shamt;
}

void instr_SRAI(CPU* cpu, uint32_t rd, uint32_t rs1, int32_t shamt)
{
    cpu->registers[rd] = (int32_t)cpu->registers[rs1] >> shamt;
}

void instr_LUI(CPU* cpu, uint32_t rd, uint32_t imm)
{
    cpu->registers[rd] = imm << 12;
}

void instr_AUIPC(CPU* cpu, uint32_t rd, uint32_t imm)
{
    cpu->registers[rd] = cpu->pc + (imm << 12);
}

void instr_ADD(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = cpu->registers[rs1] + cpu->registers[rs2];
}

void instr_SUB(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = cpu->registers[rs1] - cpu->registers[rs2];
}

void instr_SLL(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = cpu->registers[rs1] << (cpu->registers[rs2] & 0x1f);
}

void instr_SLT(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    if ((int32_t)cpu->registers[rs1] < (int32_t)cpu->registers[rs2])
    {
        cpu->registers[rd] = 1;
    }
    else
    {
        cpu->registers[rd] = 0;
    }
}

void instr_SLTU(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    if (cpu->registers[rs1] < cpu->registers[rs2])
    {
        cpu->registers[rd] = 1;
    }
    else
    {
        cpu->registers[rd] = 0;
    }
}

void instr_XOR(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = cpu->registers[rs1] ^ cpu->registers[rs2];
}

void instr_SRL(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = cpu->registers[rs1] >> (cpu->registers[rs2] & 0x1f);
}

void instr_SRA(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = (int32_t)cpu->registers[rs1] >> ((int32_t)cpu->registers[rs2] & 0x1f);
}

void instr_OR(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = cpu->registers[rs1] | cpu->registers[rs2];
}

void instr_AND(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    cpu->registers[rd] = cpu->registers[rs1] & cpu->registers[rs2];
}

void instr_JAL(CPU* cpu, uint32_t rd, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);

    cpu->registers[rd] = cpu->pc + 4;
    cpu->next_pc = (int32_t)cpu->pc + s_imm;
}

void instr_JALR(CPU* cpu, uint32_t rd, uint32_t rs1, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    cpu->registers[rd] = cpu->pc + 4;

    uint32_t target = ((int32_t)cpu->registers[rs1] + s_imm) & 0xfffffffe;
    cpu->next_pc = target;
}

void instr_BEQ(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    if (cpu->registers[rs1] == cpu->registers[rs2])
    {
        cpu->next_pc = (int32_t)cpu->pc + s_imm;  
    }
}

void instr_BNE(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    if (cpu->registers[rs1] != cpu->registers[rs2])
    {
        cpu->next_pc = (int32_t)cpu->pc + s_imm;  
    }
}

void instr_BLT(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    if ((int32_t)cpu->registers[rs1] < (int32_t)cpu->registers[rs2])
    {
        cpu->next_pc = (int32_t)cpu->pc + s_imm;
    }
}

void instr_BLTU(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    if (cpu->registers[rs1] < cpu->registers[rs2])
    {
        cpu->next_pc = (int32_t)cpu->pc + s_imm;
    }
}

void instr_BGE(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    if ((int32_t)cpu->registers[rs1] >= (int32_t)cpu->registers[rs2])
    {
        cpu->next_pc = (int32_t)cpu->pc + s_imm;
    }
}

void instr_BGEU(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    if (cpu->registers[rs1] >= cpu->registers[rs2])
    {
        cpu->next_pc = (int32_t)cpu->pc + s_imm;
    }
}

void instr_LW(CPU* cpu, uint32_t rd, uint32_t rs1, int32_t imm)
{
    cpu->registers[rd] = bus_load(&cpu->bus, (int32_t)cpu->registers[rs1] + imm, 4);
}

void instr_LH(CPU* cpu, uint32_t rd, uint32_t rs1, int32_t imm)
{
    uint32_t val = bus_load(&cpu->bus, (int32_t)cpu->registers[rs1] + imm, 2);
    cpu->registers[rd] = SIGNEX_BIT_16(val);
}

void instr_LHU(CPU* cpu, uint32_t rd, uint32_t rs1, int32_t imm)
{
    cpu->registers[rd] = bus_load(&cpu->bus, (int32_t)cpu->registers[rs1] + imm, 2);
}

void instr_LB(CPU* cpu, uint32_t rd, uint32_t rs1, int32_t imm)
{
    uint32_t val = bus_load(&cpu->bus, (int32_t)cpu->registers[rs1] + imm, 1);
    cpu->registers[rd] = SIGNEX_BIT_8(val);
}

void instr_LBU(CPU* cpu, uint32_t rd, uint32_t rs1, int32_t imm)
{
    cpu->registers[rd] = bus_load(&cpu->bus, (int32_t)cpu->registers[rs1] + imm, 1);
}

void instr_SW(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    bus_store(&cpu->bus, (int32_t)cpu->registers[rs1] + s_imm, 4, cpu->registers[rs2]);
}

void instr_SH(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    bus_store(&cpu->bus, (int32_t)cpu->registers[rs1] + s_imm, 2, cpu->registers[rs2] & 0xffff);
}

void instr_SB(CPU* cpu, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    int32_t s_imm = SIGNEX_BIT_11(imm);
    bus_store(&cpu->bus, (int32_t)cpu->registers[rs1] + s_imm, 1, cpu->registers[rs2] & 0xff);
}

void cpu_r_execute(CPU* cpu, uint32_t instr)
{
    struct rinst
    {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t funct7 : 7;
    }; 

    // dirty cast, FIX ME
    struct rinst* instruction = (struct rinst*)&instr;

    uint32_t rd = instruction->rd;
    uint32_t rs1 = instruction->rs1;
    uint32_t rs2 = instruction->rs2;
    

    switch (instruction->funct3)
    {
        case 0b000:
            if (instruction->funct7 == 0b0000000)
            {
                instr_ADD(cpu, rd, rs1, rs2);
            }
            else if (instruction->funct7 == 0b0100000)
            {
                instr_SUB(cpu, rd, rs1, rs2);
            }
            else
            {
                CPU_FAULT("Encountered unknown funct7 for ADD/SUB funct3: %x", instruction->funct7);
            }
            break;
        case 0b001:
            instr_SLL(cpu, rd, rs1, rs2);
            break;
        case 0b010:
            instr_SLT(cpu, rd, rs1, rs2);
            break;
        case 0b011:
            instr_SLTU(cpu, rd, rs1, rs2);
            break;
        case 0b100:
            instr_XOR(cpu, rd, rs1, rs2);
        case 0b101:
            if (instruction->funct7 = 0b0000000)
            {
                instr_SRL(cpu, rd, rs1, rs2);
            }
            else if (instruction->funct7 = 0b0100000)
            {
                instr_SRA(cpu, rd, rs1, rs2);
            }
            else
            {
                CPU_FAULT("Encountered unknown funct7 for SRL/SRA funct3: %x", instruction->funct7);
            }
            break;
        case 0b110:
            instr_OR(cpu, rd, rs1, rs2);
            break;
        case 0b111:
            instr_AND(cpu, rd, rs1, rs2);
            break;
        default:
            CPU_FAULT("Encountered unknown funct3 for r type instruction: %x", instruction->funct3);
    }
}

void cpu_i_execute(CPU* cpu, uint32_t instr)
{
    struct iinst
    {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t imm : 12;
    }; 

    // dirty cast, FIX ME
    struct iinst* instruction = (struct iinst*)&instr;

    uint32_t rd = instruction->rd;
    uint32_t rs1 = instruction->rs1;
    int32_t imm = SIGNEX_BIT_11(instruction->imm);

    if (instruction->opcode == 0b0000011)
    {
        switch (instruction->funct3)
        {
            case 0b00:
                instr_LB(cpu, rd, rs1, imm);
                break;
            case 0b001:
                instr_LH(cpu, rd, rs1, imm);
                break;
            case 0b010:
                instr_LW(cpu, rd, rs1, imm);
                break;
            case 0b100:
                instr_LBU(cpu, rd, rs1, imm);
                break;
            case 0b101:
                instr_LHU(cpu, rd, rs1, imm);
                break;
            default:
                CPU_FAULT("Encountered unknown funct3 for i type instruction: %x", instruction->funct3);
        }
    }
    else if (instruction->opcode == 0b0010011)
    {
        switch (instruction->funct3)
        {
            case 0b000:
                instr_ADDI(cpu, rd, rs1, imm);
                break;
            case 0b010:
                instr_SLTI(cpu, rd, rs1, imm);
                break;
            case 0b011:
                instr_SLTIU(cpu, rd, rs1, imm);
                break;
            case 0b100:
                instr_XORI(cpu, rd, rs1, imm);
                break;
            case 0b110:
                instr_ORI(cpu, rd, rs1, imm);
                break;
            case 0b111:
                instr_ANDI(cpu, rd, rs1, imm);
                break;
            case 0b001:
            {
                uint32_t funct7 = imm & 0xfe0;
                uint32_t shamt = imm & 0x1f;

                if (funct7 = 0b0000000)
                {
                    instr_SLLI(cpu, rd, rs1, imm & 0x1f);
                }
                else
                {
                    CPU_FAULT("Encountered unknown funct7 for SLLI funct3: %x", funct7);
                }
                break;
            }
            case 0b101:
            {
                uint32_t funct7 = imm & 0xfe0;
                uint32_t shamt = imm & 0x1f;

                if (funct7 == 0b0000000)
                {
                    instr_SRLI(cpu, rd, rs1, shamt);
                }
                else if (funct7 == 0b0100000)
                {
                    instr_SRAI(cpu, rd, rs1, shamt);
                }
                else
                {
                    CPU_FAULT("Encountered unknown funct7 for SRLI/SRAI funct3: %x", funct7);
                }
                break;
            }
            default:
                CPU_FAULT("Encountered unknown funct3 for i type instruction: %x", instruction->funct3);
        }
    }
    else
    {
        instr_JALR(cpu, rd, rs1, imm);
    }
}

void cpu_s_execute(CPU* cpu, uint32_t instr)
{
    struct sinstr
    {
        uint32_t opcode : 7;
        uint32_t imm0 : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t imm1 : 7;
    }; 

    // dirty cast, FIX ME
    struct sinstr* instruction = (struct sinstr*)&instr;

    uint32_t rs1 = instruction->rs1;
    uint32_t rs2 = instruction->rs2;

    uint32_t temp0 = instruction->imm0;
    uint32_t temp1 = instruction->imm1;
    uint32_t imm = (temp1 << 5) | temp0;

    switch (instruction->funct3)
    {
        case 0b000:
            instr_SB(cpu, rs1, rs2, imm);
            break;
        case 0b001:
            instr_SH(cpu, rs1, rs2, imm);
            break;
        case 0b010:
            instr_SW(cpu, rs1, rs2, imm);
            break;
        default:
            CPU_FAULT("Encountered unknown funct3 for s type instruction: %x", instruction->funct3);
    }
}

void cpu_b_execute(CPU* cpu, uint32_t instr)
{
    struct binstr
    {
        uint32_t opcode : 7;
        uint32_t imm0 : 5;
        uint32_t funct3 : 3;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t imm1 : 7;
    }; 

    struct binstr* instruction = (struct binstr*)&instr;

    uint32_t rs1 = instruction->rs1;
    uint32_t rs2 = instruction->rs2;

    uint32_t temp0 = instruction->imm0;
    uint32_t temp1 = instruction->imm1;
    uint32_t imm = (temp1 & 0x40) << 5;
    imm |= (temp0 & 0x1) << 11;
    imm |= (temp1 & 0x3f) << 5;
    imm |= (temp0 & 0x1e);

    switch (instruction->funct3)
    {
        case 0b000:
            instr_BEQ(cpu, rs1, rs2, imm);
            break;
        case 0b001:
            instr_BNE(cpu, rs1, rs2, imm);
            break;
        case 0b100:
            instr_BLT(cpu, rs1, rs2, imm);
            break;
        case 0b101:
            instr_BGE(cpu, rs1, rs2, imm);
            break;
        case 0b110:
            instr_BLTU(cpu, rs1, rs2, imm);
            break;
        case 0b111:
            instr_BGEU(cpu, rs1, rs2, imm);
            break;
        default:
            CPU_FAULT("Encountered unknown funct3 for b type instruction: %x", instruction->funct3);
    }
}

void cpu_u_execute(CPU* cpu, uint32_t instr)
{
    struct uinstr
    {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        uint32_t imm : 20;
    };

    struct uinstr* instruction = (struct uinstr*)&instr;

    uint32_t rd = instruction->rd;
    uint32_t imm = instruction->imm;

    switch (instruction->opcode)
    {
        case 0b0110111:
            instr_LUI(cpu, rd, imm);
            break;
        case 0b0010111:
            instr_AUIPC(cpu, rd, imm);
            break;
    }
}

void cpu_j_execute(CPU* cpu, uint32_t instr)
{
    struct jinstr
    {
        uint32_t opcode : 7;
        uint32_t rd : 5;
        uint32_t imm : 20;
    };

    struct jinstr* instruction = (struct jinstr*)&instr;

    uint32_t rd = instruction->rd;
    
    uint32_t temp = instruction->imm;
    uint32_t imm = (temp & 0x100000) << 20;
    imm |= (temp & 0xff) << 12;
    imm |= (temp & 0x100) << 2;
    imm |= (temp & 0xffc00) >> 11;

    instr_JAL(cpu, rd, imm);
}

void cpu_init(CPU* cpu)
{
    memset(&cpu->registers, 0, 32);
    cpu->pc = DRAM_BASE;

    cpu->registers[2] = DRAM_BASE + DRAM_SIZE; // set up stack pointer register    
}

uint32_t cpu_fetch(CPU* cpu)
{
    return bus_load(&cpu->bus, cpu->pc, 4);
}

void cpu_execute(CPU* cpu, uint32_t instruction)
{
    cpu->registers[0] = 0; // reset x0 each cycle
    cpu->next_pc = cpu->pc + 4;

    uint32_t opcode = instruction & 0x7f;

    switch (opcode)
    {
        case LUI_OP:
        case AUIPC_OP:
            cpu_u_execute(cpu, instruction);
            break;
        case JAL_OP:
            cpu_j_execute(cpu, instruction);
            break;
        case BRANCH_OP:
            cpu_b_execute(cpu, instruction);
            break;
        case LOAD_OP:
        case JALR_OP:
        case REGIMM_OP:
            cpu_i_execute(cpu, instruction);
            break;
        case STORE_OP:
            cpu_s_execute(cpu, instruction);
            break;
        case REGREG_OP:
            cpu_r_execute(cpu, instruction);
            break;
        default:
            CPU_FAULT("Encountered unknown opcode: %x", opcode);
    }

    cpu->pc = cpu->next_pc;
}
