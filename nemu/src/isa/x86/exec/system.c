#include "cpu/exec.h"
void raise_intr(uint32_t, vaddr_t);

make_EHelper(lidt) {
  //TODO();
  rtl_lm(&s0, &id_dest->addr,2);
  cpu.idtr.length = s0;
  rtl_addi(&s1, &id_dest->addr, 2);
  rtl_lm(&s0, &s1 ,4);
  if (id_dest->width == 2){
    rtl_andi(&s0, &s0, 0xffffff);
  }
  cpu.idtr.baseAddress = s0;

/*
  cpu.idtr.length = vaddr_read(id_dest->addr, 2);
  cpu.idtr.baseAddress = vaddr_read(id_dest->addr + 2, 2);
  if (id_dest->width == 2){
    cpu.idtr.baseAddress += (vaddr_read(id_dest->addr + 4, 1) << 16);
  }
  else{
    cpu.idtr.baseAddress += (vaddr_read(id_dest->addr + 4, 2) << 16);
  }
*/

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
  //TODO();
  if (id_dest->val == 0x80){
    s0 = cpu.pc;
  }
  else{
    s0 = decinfo.seq_pc;
  }
  raise_intr(id_dest->val, decinfo.seq_pc);

  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  //TODO();
  /*
  rtl_pop(&decinfo.jmp_pc);
  rtl_pop(&cpu.CS);
  rtl_pop(&cpu.eflags);
  rtl_j(decinfo.jmp_pc);
  */

  rtl_pop(&s0);
	rtl_pop(&cpu.CS);
	rtl_pop(&cpu.eflags);
	rtl_j(s0);
  
  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) {
  switch (id_dest->width){
    case 1:{
      rtl_li(&s1, pio_read_b(id_src->val));
      break;
    }
      
    case 2:{
      rtl_li(&s1, pio_read_w(id_src->val));
      break;
    }
    case 4:{
      rtl_li(&s1, pio_read_l(id_src->val));
      break;
    default: 
      assert(0);
    }
  }
  operand_write(id_dest, &s1);
  print_asm_template2(in);
}

make_EHelper(out) {
  switch (id_src->width){
    case 1:
      pio_write_b(id_dest->val, id_src->val); break;
    case 2:
      pio_write_w(id_dest->val, id_src->val); break;
    case 4:
      pio_write_l(id_dest->val, id_src->val); break;
    default:
      assert(0);
    }
  print_asm_template2(out);
}