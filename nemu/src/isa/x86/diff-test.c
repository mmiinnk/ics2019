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

  return false;
}

void isa_difftest_attach(void) {
}
