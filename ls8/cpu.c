#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

#define DATA_LEN 6
#define HLT 0b00000001  // 01
#define LDI 0b10000010  // 82, 2 operands
#define PRN 0b01000111  // 47, 1 operand
#define MUL 0b10100010  // A2, 2 operands
#define PUSH 0b01000101 // 45, 1 operand
#define POP 0b01000110  // 46, 1 operand

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
void cpu_load(struct cpu *cpu, char *prog_file)
{
  // something less hard-coded:
  // loads program (from prog_file) into memory
  FILE *fp;
  char line[1024];
  unsigned char ram_i = 0;

  fp = fopen(prog_file, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "File not found.\n");
    exit(1);
  };

  while (fgets(line, 1024, fp) != NULL)
  {
    char *end_ptr;
    unsigned char val = strtoul(line, &end_ptr, 2);
    if (end_ptr != line)
    {
      // printf("cpu->ram write val: %02x\n", val);
      // printf("cpu->ram write add: %d\n", ram_i);
      cpu_ram_write(cpu, ram_i, val);
      ram_i++;
    }
  };

  fclose(fp);
  // return ram_i - 0b1 == end of program
  // (mod2 arithmetic)
  // printf("end of prog: %d\n", ram_i - 0b1);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // printf("ALU: multiplies %d x %d\n", regA, regB);
    cpu->gp_registers[regA] = cpu->gp_registers[regA] * cpu->gp_registers[regB];
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
    // printf("pc: %d\n", cpu->pc);
    // printf("instr: %02x\n", instruction);
    // printf("oper_count: %d\n", oper_count);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (instruction)
    {
    case HLT:
      // Halt the CPU (and exit the emulator).
      // printf(">> HLT command received. Exiting.\n");
      running = 0;
      break;

    case LDI:
      // Set the value of a register to an integer.
      // printf(">> LDI command received, loading %02x register with %d.\n", operands[0], operands[1]);
      cpu->gp_registers[operands[0]] = operands[1];
      break;

    case PRN:
      // Print to the console the decimal integer value that is stored in the given register.
      // printf(">> PRN received..\n");
      printf("%d\n", cpu->gp_registers[operands[0]]);
      break;

    case MUL:
      // *This is an instruction handled by the ALU.*
      // Multiply the values in two registers together and store the result in registerA.
      alu(cpu, 0, operands[0], operands[1]);
      break;

    case PUSH:
      // Push the value in the given register on the stack.
      cpu->gp_registers[7]--;
      cpu->ram[cpu->gp_registers[7]] = cpu->gp_registers[operands[0]];
      break;

    case POP:
      // Pop the value at the top of the stack into the given register.
      cpu->gp_registers[operands[0]] = cpu->ram[cpu->gp_registers[7]];
      cpu->gp_registers[7]++;
      break;

    default:
      printf(">> Unknown command: %02x\n", instruction);
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
  // Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->ir = 0;
  cpu->mar = 0;
  cpu->mdr = 0;
  cpu->fl = 0;
  // R7 = SP, points at F4 when stack is empty
  cpu->gp_registers[7] = 0xF4;
}
