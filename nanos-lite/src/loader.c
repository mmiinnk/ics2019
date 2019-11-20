#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr //ELF Header
# define Elf_Phdr Elf32_Phdr //程序头表
#endif

size_t get_ramdisk_size();
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ELFHeader;
  ramdisk_read(&ELFHeader, 0, 52);
  Elf_Phdr Phdr_Table[ELFHeader.e_phnum];
  ramdisk_read(Phdr_Table, ELFHeader.e_phoff, ELFHeader.e_phnum * ELFHeader.e_phentsize);
  printf("Success 1\n");
  Elf_Phdr *p;
  for (int i = 0; i < ELFHeader.e_phnum; ++i){
    p = &Phdr_Table[i];
    if (p->p_type == PT_LOAD){
      char buf[p->p_memsz];
      ramdisk_read(buf, p->p_offset, p->p_filesz);
      printf("Success 2\n");
      for (int i = 0; i < (p->p_memsz - p->p_filesz); ++i){
        buf[p->p_filesz + i] = 0;
      }
      memcpy((void *)p->p_vaddr, buf, p->p_memsz);
    }
  }
  return ELFHeader.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
