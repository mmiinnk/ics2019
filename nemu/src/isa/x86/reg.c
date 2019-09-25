#include "nemu.h"
#include <stdlib.h>
#include <time.h>

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  uint32_t sample[8];
  uint32_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
	printf("$eax = 0x%x\n",cpu.eax);
	printf("$ecx = 0x%x\n",cpu.ecx);
	printf("$edx = 0x%x\n",cpu.edx);
	printf("$ebx = 0x%x\n",cpu.ebx);
	printf("$esp = 0x%x\n",cpu.esp);
	printf("$ebp = 0x%x\n",cpu.ebp);
	printf("$esi = 0x%x\n",cpu.esi);
	printf("$edi = 0x%x\n\n",cpu.edi);
	printf("$pc = 0x%x\n",cpu.pc);
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
	switch (strlen(s)){
		case 3:{
			       for (int i=0;i<8;i++){
				       if (strcmp(s,regsl[i]) == 0){
					       return reg_l(i);
				       }
			       }
			       *success = false;
			       printf("Can't find the three bytes long regition!\n'");
			       return 0;
		       }
		case 2:{
			       for (int i=0;i<8;i++){
				       if (strcmp(s,regsw[i]) == 0){
					       return reg_w(i);
				       }
			       }
			       for (int i=0; i<9; i++){
				       if (strcmp(s,regsb[i]) == 0){
					       return reg_b(i);
				       }
			       }
			       const char str[] = "pc";
			       if (strcmp(s, str) == 0)
				       return cpu.pc;
			       *success = false;
			       printf("Can't find the two bytes long regition!\n");
			       return 0;

		       }
	}
  return 0;
}
