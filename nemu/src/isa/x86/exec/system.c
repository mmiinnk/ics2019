#include "cpu/exec.h"

make_EHelper(lidt) {
  TODO();

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) {
  TODO();

  print_asm_template2(in);
}

make_EHelper(out) {
  if (id_dest->width == 1){
    pio_write_b(cpu.gpr[2]._16, cpu.gpr[0]._8[0]);
  }

  else if (decinfo.isa.is_operand_size_16){
    pio_write_w(cpu.gpr[2]._16, cpu.gpr[0]._16);
  }

  else{
    pio_write_l(cpu.gpr[2]._16, cpu.eax);
  }
  print_asm_template2(out);
}
