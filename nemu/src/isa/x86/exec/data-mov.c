#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  // push dest->val
  rtl_push(&id_dest->val);

  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  operand_write(id_dest, &id_dest->val);
  
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  rtl_lr(&s0, 4, 4);
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&s0);
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);

  print_asm("pusha");
}

make_EHelper(popa) {
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  rtl_pop(&s0);
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);

  print_asm("popa");
}

make_EHelper(leave) {
	// esp = ebp; ebp = pop()
	rtl_sr(4, &cpu.ebp, 4);
	rtl_pop(&cpu.ebp);

  print_asm("leave");
}

make_EHelper(cltd) {
  // if AX<0 then DX = 0xffff else 0
  // if EAX<0 then EDX = 0xffffffff else 0
  if (decinfo.isa.is_operand_size_16) {
    cpu.gpr[R_DX]._16 = (cpu.gpr[R_AX]._16 & 0x8000)? 0xffff : 0;
  }
  else {
    cpu.gpr[R_EDX]._32 = (cpu.gpr[R_EAX]._32 & 0x80000000) ? 0xffffffff : 0;
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    rtl_lr(&s0, R_EAX, 1);
    s0 = (int32_t)(s0 << 24) >> 24;
    rtl_sr(R_EAX, &s0, 2);
  }
  else {
    rtl_lr(&s0, R_EAX, 2);
    s0 = (int32_t)(s0 << 16) >> 16;
    rtl_sr(R_EAX, &s0, 4);
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

make_EHelper(movs) {
  if (id_dest->width != 1)
    id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  s0 = cpu.esi;
  rtl_lm(&s1, &s0, id_dest->width);
  s0 = cpu.edi;
  rtl_sm(&s0, &s1, id_dest->width);
  cpu.esi += id_dest->width;
  cpu.edi += id_dest->width;
  print_asm_template2(movs);
}