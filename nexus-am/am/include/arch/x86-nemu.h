#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  uintptr_t eax, ecx, edx, ebx, esp, ebp, esi, edi, eip, eflags, cs;
  struct _AddressSpace *as;
  int irq;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
