#include "common.h"
#include "syscall.h"
#include "proc.h"

int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
void naive_uload(PCB *pcb, const char *filename);

void sys_yield(_Context *c){
  _yield();
  c->GPRx = 0;
  //return 1;
}

void sys_exit(_Context *c){
  //_halt(c->GPR2);
  naive_uload(NULL, "/bin/init");
  //return 1;
}

void sys_write(_Context *c){
  //int fd = c->GPR2;
  //char *buf = (char *)c->GPR3;
  //size_t len = c->GPR4;
  //printf("len = %d\n", len);
  
  //Log("%s\n", buf[0]);
  
  //if (fd == 1 || fd == 2){
  //  for (int i = 0; i < count; ++i){
  //    _putc(buf[i]);
  //  }
  //  c->GPRx = count;
  //}
  //else{
  c->GPRx = fs_write(c->GPR2, (void *)c->GPR3, c->GPR4);
  //}
  //return 1;
}

void sys_brk(_Context *c){
  c->GPRx = 0;
  //return 1;
}

void sys_open(_Context *c){
  c->GPRx = fs_open((char *)c->GPR2, c->GPR3, c->GPR4);
  //printf("Successfully use sys_open!\n");
  //return 1;
}

void sys_close(_Context *c){
  c->GPRx = fs_close(c->GPR2);
  //return 1;
}

void sys_read(_Context *c){
  c->GPRx = fs_read(c->GPR2, (void *)c->GPR3, c->GPR4);
  //printf("Successfully use sys_open!\n");
  //return 1;
}

void sys_lseek(_Context *c){
  c->GPRx = fs_lseek(c->GPR2, (size_t)c->GPR3, c->GPR4);
  //printf("Successfully use sys_open!\n");
  //return 1;
}

void sys_execve(_Context *c){
  naive_uload(NULL, (const char *)c->GPR2);
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  //printf("%d\n", a[0]);
  switch (a[0]) {
    case SYS_yield: sys_yield(c); break;
    case SYS_exit:  sys_exit(c);  break;
    case SYS_write: sys_write(c); break;
    case SYS_brk:   sys_brk(c);   break;
    case SYS_open:  sys_open(c);  break;
    case SYS_close: sys_close(c); break;
    case SYS_read:  sys_read(c);  break;
    case SYS_lseek: sys_lseek(c); break;
    case SYS_execve:sys_execve(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
