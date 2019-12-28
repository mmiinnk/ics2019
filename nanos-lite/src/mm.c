#include "memory.h"
#include "proc.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  // if (brk + increment < current->max_brk) return 0;

  // // printf("brk = 0x%x\n", brk);
  // // printf("increment = 0x%x\n", increment);
  // // printf("max_brk = 0x%x\n", current->max_brk);
  // current->max_brk = (brk & ~0xfff) + 0x1000;
  // while(brk + increment >= current->max_brk){
  //   printf("brk = 0x%x\n", brk);
  //   printf("increment = 0x%x\n", increment);
  //   printf("max_brk = 0x%x\n", current->max_brk);
  //   void *pa = new_page(1);
  //   _map(&current->as, (void *)current->max_brk, pa, 1);
  //   current->max_brk += PGSIZE;
  // }
  // return 0;
  printf("Previous max_brk = 0x%x\n", current->max_brk);
  while (current->max_brk <= brk + increment){
    void *pa = new_page(1);
    _map(&current->as, (void *)current->max_brk, pa, 1);
    current->max_brk += PGSIZE;
  }
  printf("Current max_brk = 0x%x\n", current->max_brk);
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
