#include "nemu.h"
#include "monitor/diff-test.h"
//#include "../../../tools/qemu-diff/src/isa/x86/include/isa.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  //union isa_gdb_regs r;
  //ref_difftest_getregs(&r);

  if (cpu.eax != ref_r->eax){
    printf("Eax was Wrong! And it should be 0x%x.\n", ref_r->eax);
    return false;
  }
    
  if (cpu.ecx != ref_r->ecx){
    printf("Ecx was Wrong!\n");
    return false;
  }
    
  if (cpu.edx != ref_r->edx){
    printf("Edx was Wrong!\n");
    return false;
  }

  if (cpu.ebx != ref_r->ebx){
    printf("Ebx was Wrong!\n");
    return false;
  }
    
  if (cpu.esp != ref_r->esp){
    printf("Esp was Wrong!\n And it should be 0x%x.\n", ref_r->esp);
    return false;
  }
    
  if (cpu.ebp != ref_r->ebp){
    printf("Ebp was Wrong!\n");
    return false;
  }
    
  if (cpu.esi != ref_r->esi){
    printf("Esi was Wrong! And it should be 0x%x.\n", ref_r->esi);
    return false;
  }
    
  if (cpu.edi != ref_r->edi){
    printf("Edi was Wrong!\n");
    return false;
  }
    
  if (cpu.pc != ref_r->pc){
    printf("PC was Wrong! And it should be 0x%x.\n", ref_r->pc);
    return false;
  }

  /*
  if (cpu.eflag != ref_r->eflag){
    printf("EFLAGS was wrong!\n");
    return false;
  }
  */
  /*
  if (cpu.eflags.CF != ref_r->eflags.CF){
    printf("CF was wrong!\n");
    return false;
  }

  if (cpu.eflags.ZF != ref_r->eflags.ZF){
    printf("ZF was wrong!\n");
    return false;
  }

  if (cpu.eflags.SF != ref_r->eflags.SF){
    printf("SF was wrong!\n");
    return false;
  }

  if (cpu.eflags.OF != ref_r->eflags.OF){
    printf("OF was wrong!\n");
    return false;
  }
  */
  return true;
}

void isa_difftest_attach(void) {
  //difftest_init();
  ref_difftest_memcpy_from_dut(0, guest_to_host(0), 0x7c00);
  ref_difftest_memcpy_from_dut(0x100000, guest_to_host(0x100000), PMEM_SIZE - 0x100000);
  ref_difftest_memcpy_from_dut(0x7e00, &cpu.idtr, 6);
  uint8_t instruction[] = {0x0f, 0x01, 0x1D, 0x00, 0x7e, 0x00, 0x00, 0x0f, 0x22, 0xd8, 0x0f, 0x22, 0xc1};
  ref_difftest_memcpy_from_dut(0x7e40, instruction, sizeof(instruction));
  CPU_state temp = cpu;
  temp.pc = 0x7e40;
  temp.eax = cpu.CR3;
  temp.ecx = cpu.CR0;
  ref_difftest_setregs(&temp);
  ref_difftest_exec(3);
  ref_difftest_setregs(&cpu);
}
