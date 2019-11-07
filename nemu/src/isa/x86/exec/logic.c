#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  rtl_and(&s0, &id_dest->val, &id_src->val);
  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&s0, &id_dest->val, &id_src->val);
  
  operand_write(id_dest, &s0);
  
  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&s0, &id_dest->val, &id_src->val);

  operand_write(id_dest, &s0);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);


  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&s0, &id_dest->val, &id_src->val);

  operand_write(id_dest, &s0);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(or);
}

make_EHelper(sar) {
  rtl_sar(&s0, &id_dest->val, &id_src->val);

  operand_write(id_dest, &s0);

  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  rtl_shl(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(shl);
}

make_EHelper(shr) {
  rtl_shr(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  rtl_not(&s0, &id_dest->val);

  operand_write(id_dest, &s0);

  print_asm_template1(not);
}

make_EHelper(rol){
  /*
  rtl_mv(&s0, &id_dest->val);
  rtl_andi(&s0, &s0, 0xffffffffu >> ((4 - id_dest->width) * 8));
  s1 = 8 * id_dest->width - id_src->val;
  rtl_shr(&s0, &s0, &s1);
  rtl_shl(&id_dest->val, &id_dest->val, &id_src->val);
  rtl_or(&id_dest->val, &id_dest->val, &s0);
  rtl_andi(&id_dest->val, &id_dest->val, 0xffffffffu >> ((4 - id_dest->width) * 8));
  operand_write(id_dest, &id_dest->val);

  if (id_src->val == 0x1) {
    rtl_get_CF(&s1);
    rtl_msb(&s0, &id_dest->val, id_dest->width);
    rtl_setrelop(RELOP_NE, &s0, &s0, &s1);
    rtl_set_OF(&s0);
  }
*/
  
  rtl_rol(&s1, &id_dest->val, &id_src->val, id_dest->width);
  
  // set CF
  s0 = s1&0x1;
  rtl_set_CF(&s0);

  operand_write(id_dest, &s1);

  // set OF
  if (id_src->val == 1){
    rtl_msb(&s0, &s1, id_dest->width);
    if (s0 == (s1&0x1)){
      s0 = 0;
    }
    else{
      s0 = 1;
    }
    rtl_set_OF(&s0);
  }
}

make_EHelper(rcr) {
  rtlreg_t count;
  rtl_mv(&count, &id_src->val);
  for(; count; count--){
    rtl_get_CF(&s0);
    rtl_andi(&s1, &id_dest->val, 0x1);
    rtl_set_CF(&s1);
    rtl_shri(&id_dest->val, &id_dest->val, 0x1);
    rtl_shli(&s0, &s0, (8 * id_dest->width - 1));
    rtl_or(&id_dest->val, &id_dest->val, &s0);
  }
  rtl_andi(&id_dest->val, &id_dest->val, 0xffffffffu >> ((4 - id_dest->width) * 8));
  operand_write(id_dest, &id_dest->val);

  if (id_src->val == 0x1) {
    rtl_shri(&s1, &id_dest->val, 8*id_dest->width-2);
    rtl_andi(&s1, &s1, 0x1);
    rtl_msb(&s0, &id_dest->val, id_dest->width);
    rtl_setrelop(RELOP_NE, &s0, &s0, &s1);
    rtl_set_OF(&s0);
  }
}

make_EHelper(rcl) {
  rtlreg_t count;
  rtl_mv(&count, &id_src->val);
  for(; count; count--){
    rtl_get_CF(&s0);
    rtl_msb(&s1, &id_dest->val, id_dest->width);
    rtl_set_CF(&s1);
    rtl_shli(&id_dest->val, &id_dest->val, 0x1);
    rtl_or(&id_dest->val, &id_dest->val, &s0);
  }
  rtl_andi(&id_dest->val, &id_dest->val, 0xffffffffu >> ((4 - id_dest->width) * 8));
  operand_write(id_dest, &id_dest->val);

  if (id_src->val == 0x1) {
    rtl_get_CF(&s1);
    rtl_msb(&s0, &id_dest->val, id_dest->width);
    rtl_setrelop(RELOP_NE, &s0, &s0, &s1);
    rtl_set_OF(&s0);
  }
}

make_EHelper(ror) {
  rtl_andi(&id_dest->val, &id_dest->val, 0xffffffffu >> ((4 - id_dest->width) * 8));
  rtl_mv(&s0, &id_dest->val);
  s1 = 8 * id_dest->width - id_src->val;
  rtl_shl(&s0, &s0, &s1);
  rtl_shr(&id_dest->val, &id_dest->val, &id_src->val);
  rtl_or(&id_dest->val, &id_dest->val, &s0);
  rtl_andi(&id_dest->val, &id_dest->val, 0xffffffffu >> ((4 - id_dest->width) * 8));
  operand_write(id_dest, &id_dest->val);

  if (id_src->val == 0x1) {
    rtl_shri(&s1, &id_dest->val, 8*id_dest->width-2);
    rtl_andi(&s1, &s1, 0x1);
    rtl_msb(&s0, &id_dest->val, id_dest->width);
    rtl_setrelop(RELOP_NE, &s0, &s0, &s1);
    rtl_set_OF(&s0);
  }
}