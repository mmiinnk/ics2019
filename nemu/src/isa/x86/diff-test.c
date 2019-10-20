#include "nemu.h"
#include "monitor/diff-test.h"
#include "../../../tools/qemu-diff/src/isa/x86/include/isa.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  union isa_gdb_regs r;
  ref_difftest_getregs(&r);

  if (r.eax != ref_r->eax)
    return false;
  if (r.ecx != ref_r->ecx)
    return false;
  if (r.edx != ref_r->edx)
    return false; 
  if (r.ebx != ref_r->ebx)
    return false;
  if (r.esp != ref_r->esp)
    return false;
  if (r.ebp != ref_r->ebp)
    return false;
  if (r.esi != ref_r->esi)
    return false;
  if (r.edi != ref_r->edi)
    return false;
  if (r.eip != ref_r->pc)
    return false;
  return true;
}

void isa_difftest_attach(void) {
}
