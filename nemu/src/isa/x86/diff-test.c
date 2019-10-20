#include "nemu.h"
#include "monitor/diff-test.h"
#include "../../../tools/qemu-diff/src/isa/x86/include/isa.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  union isa_gdb_regs r;
  ref_difftest_getregs(&r);

  if (r.eax != ref_r->eax){
    printf("Eax was Wrong!\n");
    return false;
  }
    
  if (r.ecx != ref_r->ecx){
    printf("Ecx was Wrong!\n");
    return false;
  }
    
  if (r.edx != ref_r->edx){
    printf("Edx was Wrong!\n");
    return false;
  }

  if (r.ebx != ref_r->ebx){
    printf("Ebx was Wrong!\n");
    return false;
  }
    
  if (r.esp != ref_r->esp){
    printf("Esp was Wrong!\n");
    return false;
  }
    
  if (r.ebp != ref_r->ebp){
    printf("Ebp was Wrong!\n");
    return false;
  }
    
  if (r.esi != ref_r->esi){
    printf("Esi was Wrong!\n");
    return false;
  }
    
  if (r.edi != ref_r->edi){
    printf("Edi was Wrong!\n");
    return false;
  }
    
  if (r.eip != ref_r->pc){
    printf("PC was Wrong!\n");
    return false;
  }
    
  return true;
}

void isa_difftest_attach(void) {
}
