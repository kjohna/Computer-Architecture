#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(char *argc[1], char *argv[1])
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}