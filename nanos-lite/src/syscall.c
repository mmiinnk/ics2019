#include "common.h"
#include "syscall.h"

int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);

static inline uintptr_t sys_yield(_Context *c){
  _yield();
  c->GPRx = 0;
  return 1;
}

static inline uintptr_t sys_exit(_Context *c){
  _halt(c->GPR2);
  return 1;
}

static inline uintptr_t sys_write(_Context *c){
  int fd = c->GPR2;
  char *buf = (char *)c->GPR3;
  size_t count = c->GPR4;
  
  //Log("%s\n", buf[0]);
  
  if (fd == 1 || fd == 2){
    for (int i = 0; i < count; ++i){
      _putc(buf[i]);
    }
    c->GPRx = count;
  }
  else{
    c->GPRx = -1;
  }
  return 1;
}

static inline uintptr_t sys_brk(_Context *c){
  c->GPRx = 0;
  return 1;
}

static inline uintptr_t sys_open(_Context *c){
  fs_open((char *)c->GPR1, c->GPR2, c->GPR3);
  return 1;
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  printf("%d\n", a[0]);
  switch (a[0]) {
    case SYS_yield: sys_yield(c); break;
    case SYS_exit:  sys_exit(c);  break;
    case SYS_write: sys_write(c); break;
    case SYS_brk:   sys_brk(c);   break;
    case SYS_open:  sys_open(c);  break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
