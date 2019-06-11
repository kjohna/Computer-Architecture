#include "cpu.h"

#define DATA_LEN 6
#define HLT 0b00000001 // 01
#define LDI 0b10000010 // 82, 2 operands

// helpers to read and write cpu's ram
unsigned int cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
};

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;
};

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int oper_count, operands[2];
  unsigned char instruction;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    instruction = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    oper_count = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    for (int i = 0; i < oper_count; i++)
    {
      operands[i] = cpu_ram_read(cpu, cpu->pc + i + 1);
    }
    // what's it up to?
    printf("pc: %d\n", cpu->pc);
    printf("instr: %02x\n", instruction);
    printf("oper_count: %d\n", oper_count);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (instruction)
    {
    case HLT:
      printf("HLT command received. Exiting.\n");
      running = 0;
      break;

    case LDI:
      printf("LDI command received, loading %02x register with %d.\n", operands[0], operands[1]);
      cpu->gp_registers[operands[0]] = operands[1];
      break;

    default:
      printf("Unknown command: %02x\n", instruction);
      break;
    }
    // 6. Move the PC to the next instruction.
    cpu->pc = cpu->pc + 1 + oper_count;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->ir = 0;
  cpu->mar = 0;
  cpu->mdr = 0;
  cpu->fl = 0;
}
