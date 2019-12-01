#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);
  
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  operand_write(id_dest, &id_dest->val);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  rtl_mv(&s0, &cpu.esp);
  rtl_push(&cpu.eax);
	rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
	rtl_push(&s0);
	rtl_push(&cpu.ebp);
	rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);

/*
  id_dest->width = 4;
  //if (decinfo.isa.is_operand_size_16){
  //  id_dest->width = 2;
  //}

  // s0 = old esp
  rtl_lr(&s0, 4, id_dest->width);

  for (int i = 0; i < 8; i++){
    if (i == 4){
      rtl_push(&s0);
      continue;
    }
    rtl_lr(&s1, i, id_dest->width);
    rtl_push(&s1);
  }*/

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


/*
  id_dest->width = 4;
  //if (decinfo.isa.is_operand_size_16){
  //  id_dest->width = 2;
  //}

  for (int i = 7; i >= 0; i--){ 
    rtl_pop(&s0);

    // if esp --> throw away
    if (i == 4){
      continue;
    }

    // else cpu.gpr[i] = s0
    rtl_sr(i, &s0, id_dest->width);
  }*/
  print_asm("popa");
}

make_EHelper(leave) {
  id_dest->width = 4;
  if (decinfo.isa.is_operand_size_16){
    id_dest->width = 2;
  }

  // s0 = ebp/bp
  rtl_lr(&s0, R_EBP, id_dest->width);
  //rtl_sr(4, &cpu.ebp, 4);

  // esp/sp = s0
  rtl_sr(R_ESP, &s0, id_dest->width);
  
  // s0 = pop()
  rtl_pop(&s0);

  // ebp/bp = s0
  rtl_sr(R_EBP, &s0, id_dest->width);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    rtl_lr(&s0, R_AX, 2);
    s0 = (((int)(s0<<16))>>31);
    rtl_sr(R_DX, &s0, 2);
  }
  else {
    rtl_lr(&s0, R_EAX, 4);
    s0 = (((int)(s0))>>31);
    rtl_sr(R_EDX, &s0, 4);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    rtl_lr(&s0, R_AL, 1);
    rtl_sext(&s0, &s0, 1);
    rtl_andi(&s0, &s0, 0x0000ffff);
    //s0 = (((int)s0 << 24)>>24);
    rtl_sr(R_AX, &s0, 2);
  }
  else {
    rtl_lr(&s0, R_AX, 2);
    rtl_sext(&s0, &s0, 2);
    //s0 = (((int)s0 << 16)>>16);
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

make_EHelper(movs){
  //TODO();
  //assert(0);
  /*

  if (id_dest->width != 1){
    id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  }

  rtl_lm(&s0, &cpu.esi, id_dest->width);
  rtl_sm(&cpu.edi, &s0, id_dest->width);
  rtl_addi(&cpu.esi, &cpu.esi, id_dest->width);
  rtl_addi(&cpu.edi, &cpu.edi, id_dest->width);
  */
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

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}

make_EHelper(xchg){
  s0 = id_dest->val;
  operand_write(id_dest, &id_src->val);
  operand_write(id_src, &s0);
}