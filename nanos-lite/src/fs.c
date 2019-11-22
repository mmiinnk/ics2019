#include "../include/fs.h"

typedef long int off_t;
typedef int ssize_t;
typedef unsigned int size_t;

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;         //文件名
  size_t size;        //文件大小
  off_t disk_offset;  //文件在ramdisk中的偏移
  off_t open_offset;  //文件被打开之后的读写指针
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode){
  //int fd = -1;
  printf("%s\n", pathname);
  for (int i = 0; i < (sizeof(file_table)/sizeof(file_table[0])); i++){
    if (strcmp(file_table[i].name, pathname) == 0){
      printf("%s\n", file_table[i].name);
      return i;
    }
  }
  //printf("Can't find the file!\n");
  assert(0);
}

ssize_t fs_read(int fd, void *buf, size_t len){
  Finfo *File = &file_table[fd];
  if ((File->open_offset + len) > File->size){
    printf("The open_offset will be over the Bound of File!\n");
    return -1;
  }
  File->open_offset += ramdisk_read(buf, File->disk_offset + File->open_offset, len);
  return len;
}

ssize_t fs_write(int fd, const void *buf, size_t len){
  Finfo *File = &file_table[fd];
  if ((File->open_offset + len) > File->size){
    printf("The open_offset will be over the Bound of File!\n");
    return -1;
  }
  File->open_offset += ramdisk_write(buf, File->disk_offset + File->open_offset, len);
  return len;
}

off_t fs_lseek(int fd, off_t offset, int whence){
  switch (whence){
    case SEEK_SET: file_table[fd].open_offset = offset; return file_table[fd].open_offset;
    case SEEK_CUR: file_table[fd].open_offset += offset; return file_table[fd].open_offset;
    case SEEK_END: file_table[fd].open_offset = file_table[fd].size; file_table[fd].open_offset += offset; return file_table[fd].open_offset;
    default: return -1;
  }
}

int fs_close(int fd){
  return 0;
}