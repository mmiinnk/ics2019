#include "proc.h"
#include "fs.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr //ELF Header
# define Elf_Phdr Elf32_Phdr //程序头表
#endif

//typedef unsigned int size_t;

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);

  Elf_Ehdr ELFHeader;
  //printf("ELFHeader = 0x%x\n", &ELFHeader);
  fs_read(fd, &ELFHeader, sizeof(ELFHeader));
  printf("Successfully read ELFHeader!\n");
  
  Elf_Phdr Phdr_Table[ELFHeader.e_phnum];

  fs_lseek(fd, ELFHeader.e_phoff, SEEK_SET);
  fs_read(fd, Phdr_Table, ELFHeader.e_phnum * ELFHeader.e_phentsize);
  printf("Successfully read Phdr_Table!\n");
  //ramdisk_read(Phdr_Table, disk_offset + ELFHeader.e_phoff, ELFHeader.e_phnum * ELFHeader.e_phentsize);
  //printf("Success 1\n");
  
  Elf_Phdr *p;
  for (int i = 0; i < ELFHeader.e_phnum; ++i){
    p = &Phdr_Table[i];
    if (p->p_type == PT_LOAD){
      char buf[p->p_memsz];

      fs_lseek(fd, p->p_offset, SEEK_SET);
      fs_read(fd, buf, p->p_filesz);
      //ramdisk_read(buf, disk_offset + p->p_offset, p->p_filesz);
      //printf("Success 2\n");
      for (int j = 0; j < (p->p_memsz - p->p_filesz); ++j){
        buf[p->p_filesz + j] = 0;
      }
      printf("p_memsz = 0x%x\n", p->p_memsz);
      printf("p->vaddr = 0x%x\n", p->p_vaddr);
      printf("size of buf = 0x%x\n", sizeof(buf));
      memcpy((void *)p->p_vaddr, buf, p->p_memsz);
    }
  }
  if (fs_close(fd) != 0){
    printf("Fail to close the File!\n");
    assert(0);
  }
  printf("Successfully Loaded!\n");
  //printf("Entry = 0x%x\n", ELFHeader.e_entry);
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
