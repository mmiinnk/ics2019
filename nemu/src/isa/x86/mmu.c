#include "nemu.h"

#define PG (cpu.CR0&0x1)

paddr_t page_translate(vaddr_t vaddr){
  uint32_t dir = (vaddr>>22)&0x3ff;
  uint32_t page = (vaddr>>12)&0x3fff;
  uint32_t offset = vaddr&0xffff;
  paddr_t dir_entry = paddr_read(cpu.CR3 + dir, 4);
  paddr_t pa_base = paddr_read(dir_entry + page, 4);
  return pa_base + offset;
}



uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if (PG == 1){
    if ((addr&0xfff) + len > 0xfff + 1){
      printf("Data cross the page boundary\n");
      /*special case, handle later*/
      assert(0);
    }
    else
    {
      paddr_t paddr = page_translate(addr);
      return paddr_read(paddr, len);
    }
  }
  else{
    return paddr_read(addr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_write(addr, data, len);
}
