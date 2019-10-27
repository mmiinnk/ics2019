#include "nemu.h"
#include "monitor/diff-test.h"
//#include "../../../tools/qemu-diff/src/isa/x86/include/isa.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  //union isa_gdb_regs r;
  //ref_difftest_getregs(&r);

  if (cpu.eax != ref_r->eax){
    printf("Eax was Wrong!\n");
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
    printf("Esp was Wrong!\n");
    return false;
  }
    
  if (cpu.ebp != ref_r->ebp){
    printf("Ebp was Wrong!\n");
    return false;
  }
    
  if (cpu.esi != ref_r->esi){
    printf("Esi was Wrong!\n");
    return false;
  }
    
  if (cpu.edi != ref_r->edi){
    printf("Edi was Wrong!\n");
    return false;
  }
    
  if (cpu.pc != ref_r->pc){
    printf("PC was Wrong!\n");
    return false;
  }

  if ((cpu.eflags.CF) != ref_r->eflags.CF){
    printf("CF was wrong!\n");
    return false;
  }

  /*
  if (cpu.eflags.ZF != ref_r->eflags.ZF){
    printf("ZF was wrong!\n");
    return false;
  }

  if (((r.eflags>>7)&0x1) != ref_r->eflags.SF){
    printf("SF was wrong!\n");
    return false;
  }

  if (((r.eflags>>11)&0x1) != ref_r->eflags.OF){
    printf("OF was wrong!\n");
    return false;
  }
  */
  return true;
}

void isa_difftest_attach(void) {
}
