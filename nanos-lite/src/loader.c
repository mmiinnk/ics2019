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
  fs_read(fd, &ELFHeader, sizeof(ELFHeader));
  // printf("%s\n",filename);
  // printf("Successfully read ELFHeader!\n");
  // printf("ELFHeader.e_phoff = 0x%x\n", ELFHeader.e_phoff);
  // printf("ELFHeader.e_version = 0x%x\n", ELFHeader.e_version);
  // printf("ELFHeader.e_entry = 0x%x\n", ELFHeader.e_entry);
  
  Elf_Phdr Phdr_Table[ELFHeader.e_phnum];

  fs_lseek(fd, ELFHeader.e_phoff, SEEK_SET);
  fs_read(fd, Phdr_Table, ELFHeader.e_phnum * ELFHeader.e_phentsize);
  //printf("Successfully read Phdr_Table!\n");
  //ramdisk_read(Phdr_Table, disk_offset + ELFHeader.e_phoff, ELFHeader.e_phnum * ELFHeader.e_phentsize);
  //printf("Success 1\n");
  
  Elf_Phdr *p;
  for (int i = 0; i < ELFHeader.e_phnum; ++i){
    p = &Phdr_Table[i];
    if (p->p_type == PT_LOAD){
      fs_lseek(fd, p->p_offset, SEEK_SET);
      uint32_t zero_len = p->p_memsz - p->p_filesz;
      
      #ifndef HAS_VME
      fs_read(fd, (void *)p->p_vaddr, p->p_filesz);
      if (zero_len > 0){
        memset((void *)(p->p_vaddr + p->p_filesz), 0, zero_len);
      }
      #endif

      #ifdef HAS_VME
      uint32_t length = 0;
      printf("filesz = 0x%x\n", p->p_filesz);
      printf("memsz = 0x%x\n", p->p_memsz);
      
      void *pa = NULL;
      while(length + PGSIZE < p->p_filesz){
        printf("length = %d\n", length);
        pa = new_page(1);
        _map(&pcb->as, (void *)(p->p_vaddr + length), pa, 1);
        fs_read(fd, (void *)pa, PGSIZE);
        length += PGSIZE;
      }

      pa = new_page(1);
      _map(&(pcb->as), (void *)(p->p_vaddr + length), pa, 1);
      fs_read(fd, pa, p->p_filesz - length);
      length += PGSIZE;

      printf("Length before set zero = %d\n", length);
      if (zero_len > 0){
        Log("Set Zero");
        if (p->p_memsz <= length){
          memset((void *)(pa + (p->p_filesz - (length - PGSIZE))), 0, zero_len);
        }
        else{
          Log("p->p_memsz > length");
          memset((void *)(pa + p->p_filesz / PGSIZE), 0, length - p->p_filesz);
          while(length + PGSIZE< p->p_memsz){
            pa = new_page(1);
            _map(&pcb->as, (void *)(p->p_vaddr + length), pa, 1);
            memset(pa, 0, PGSIZE);
            length += PGSIZE;
          }
          pa = new_page(1);
          _map(&pcb->as, (void *)(p->p_vaddr + length), pa, 1);
          memset(pa, 0, p->p_memsz - length);
          length += PGSIZE;
        }
        pcb->max_brk = p->p_vaddr + length;
      }
      #endif
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
  //printf("entry = %p\n", pcb->cp->eip);
}

void context_uload(PCB *pcb, const char *filename) {
  _protect(&pcb->as);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
