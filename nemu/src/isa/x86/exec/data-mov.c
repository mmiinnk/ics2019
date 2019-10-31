#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  if (id_dest->type == OP_TYPE_MEM){
    rtl_push(&id_dest->addr);
  }
  else
  {
    rtl_push(&id_dest->val);
  }
  
  
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  operand_write(id_dest, &id_dest->val);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  rtl_sr(4, &cpu.ebp, 4);
  rtl_pop(&cpu.ebp);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    rtl_lr(&s0, 0, 2);
    s0 = (((int)(s0<<16))>>31);
    rtl_sr(2, &s0, 2);
  }
  else {
    rtl_lr(&s0, 0, 4);
    s0 = (((int)(s0))>>31);
    rtl_sr(2, &s0, 4);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    rtl_lr(&s0, 0, 1);
    s0 = (((int)s0 << 24)>>24);
    rtl_sr(0, &s0, 2);
  }
  else {
    rtl_lr(&s0, 0, 2);
    s0 = (((int)s0 << 16)>>16);
    rtl_sr(0, &s0, 4);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}

make_EHelper(xchg){
  s0 = id_dest->val;
  operand_write(id_dest, &id_src->val);
  operand_write(id_src, &s0);
}