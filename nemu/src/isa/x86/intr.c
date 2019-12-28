#include "rtl/rtl.h"

#define IRQ_TIMER 32          // for x86

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags);
  cpu.IF = 0;
  rtl_push(&cpu.CS);
  rtl_push(&ret_addr);
  
  assert(NO <= cpu.idtr.length);

  uint32_t p = cpu.idtr.baseAddress + NO*8;
  uint32_t gatedesc_lo = vaddr_read(p, 4);
  uint32_t gatedesc_hi = vaddr_read(p + 4, 4);
  rtl_j((gatedesc_lo & 0xffff) + (gatedesc_hi & 0xffff0000));
  //rtl_j(vaddr_read(p, 2) + (vaddr_read(p + 6, 2) << 16));
}

bool isa_query_intr(void) {
  if (cpu.INTR & cpu.IF) {
    printf("Reach isa_query_intr\n");
    cpu.INTR = false;
    raise_intr(IRQ_TIMER, cpu.pc);
    return true;
  }
  return false;
}
