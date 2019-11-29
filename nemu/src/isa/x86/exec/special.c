#include "cpu/exec.h"
#include "monitor/monitor.h"

make_EHelper(nop) {
  print_asm("nop");
}

make_EHelper(inv) {
  /* invalid opcode */

  uint32_t temp[2];
  *pc = cpu.pc;
  temp[0] = instr_fetch(pc, 4);
  temp[1] = instr_fetch(pc, 4);

  uint8_t *p = (void *)temp;
  printf("invalid opcode(PC = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n\n",
      cpu.pc, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

  display_inv_msg(cpu.pc);

  rtl_exit(NEMU_ABORT, cpu.pc, -1);

  print_asm("invalid opcode");
}

make_EHelper(nemu_trap) {
  difftest_skip_ref();

  rtl_exit(NEMU_END, cpu.pc, cpu.eax);

  print_asm("nemu trap");
  return;
}

make_EHelper(rol) {
  // rotate left
  // $cl: src
  s0 = id_src->val;
  for (; s0 != 0; s0--)
  {
    s1 = (id_dest->val >> (id_dest->width * 8 - 1)) & 0x1;
    id_dest->val *= 2;
    id_dest->val += s1;
  }
  operand_write(id_dest, &id_dest->val);
  if (id_src->val == 1)
  {
    s1 = (id_dest->val >> (id_dest->width * 8 - 1)) & 0x1;
    if (s1 != cpu.CF) cpu.OF = 1;
    else cpu.OF = 0;
  }

  print_asm("rol");
  return;
  
}