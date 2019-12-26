#include "nemu.h"

paddr_t page_translate(vaddr_t vaddr){
  TODO();
  return 0;
}



uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  // if ((addr&0xfff) + len > 0xfff + 1){
  //   printf("Data cross the page boundary\n");
  //   /*special case, handle later*/
  //   assert(0);
  // }
  // else
  // {
  //   paddr_t paddr = page_translate(addr);
  //   return paddr_read(paddr, len);
  // }
  
  return paddr_read(addr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_write(addr, data, len);
}
