#include "nemu.h"
#include "include/isa/mmu.h"

#define PG ((cpu.CR0>>31)&0x1)
#define P(_) (_&0x1)
#define Base_Address(_) ((_>>12)&0xfffff)

paddr_t page_translate(vaddr_t vaddr, bool is_write){
  /* Get dir, page and pffset form vaddr */
  uint32_t dir = (vaddr>>22)&0x3ff;
  uint32_t page = (vaddr>>12)&0x3fff;
  uint32_t offset = vaddr&0xffff;

  /* Get CR3 Resgister */
  CR3 CR3;
  CR3.val = cpu.CR3;

/*-----------------Get Dir Entry from Page Directory-----------------*/
  paddr_t dir_context = paddr_read(CR3.page_directory_base + dir, 4);
  
  /*Check the Present Label 
    Only if the Present Label is 1, we do the following actions.*/
  if (P(dir_context) != 1){
    printf("The Dir Page Miss!\n");
    assert(P(dir_context) == 1);
  }

  // Revise the Accessed Label
  paddr_write(CR3.page_directory_base + dir, dir_context | 0x20, 4);

  // Get Dir Entry
  paddr_t dir_entry = Base_Address(dir_context);

/*-----------------Get PG TBL ENTRY from Page Table-----------------*/
  paddr_t pageTable_context = paddr_read(dir_entry + page, 4);
  /*Check the Present Label 
    Only if the Present Label is 1, we do the following actions.*/
  if (P(pageTable_context) != 1){
    printf("The Dir Page Miss!\n");
    assert(P(pageTable_context) == 1);
  }

  // Revise the Accessed Label
  paddr_write(dir_entry + page, pageTable_context | 0x20, 4);

  // Revise the Dirty Bit if Write
  if (is_write){
    paddr_write(dir_entry + page, pageTable_context | 0x40, 4);
  }

  // Get Physical Address Base
  paddr_t pa_base = Base_Address(pageTable_context);

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
      paddr_t paddr = page_translate(addr, false);
      return paddr_read(paddr, len);
    }
  }
  else{
    return paddr_read(addr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if (PG == 1){
    if ((addr&0xfff) + len > 0xfff + 1){
      printf("Data cross the page boundary\n");
      /*special case, handle later*/
      assert(0);
    }
    else
    {
      paddr_t paddr = page_translate(addr, true);
      paddr_write(paddr, data, len);
    }
  }
  else{
    paddr_write(addr, data, len);
  }  
}
