#include "rtl/rtl.h"
//#include "../../../../nexus-am/am/include/x86.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags);
  rtl_push(&cpu.CS);
  rtl_push(&ret_addr);
  uint32_t p = cpu.idtr.baseAddress + NO*8;
  uint32_t gatedesc_lo = vaddr_read(p, 4);
  uint32_t gatedesc_hi = vaddr_read(p + 4, 4);
  rtl_j((gatedesc_lo & 0xffff) + (gatedesc_hi & 0xffff0000));
  //rtl_j(vaddr_read(p, 2) + (vaddr_read(p + 6, 2) << 16));
}

bool isa_query_intr(void) {
  return false;
}
