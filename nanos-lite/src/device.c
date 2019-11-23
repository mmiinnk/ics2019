#include "common.h"
#include <amdev.h>

ssize_t fs_read(int fd, void *buf, size_t len);

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //printf("haha\n");
  //printf("len = %d\n", len);
  const char *tempbuf = buf;
  for (size_t i = 0; i < len; i++){
    //_putc(tempbuf[i]);
    printf("%c", tempbuf[i]);
  }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  char *tempbuf = buf;
  for (size_t i = 0; i < len; ++i){
    tempbuf[i] = dispinfo[offset + i];
  }
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  fs_read(4, dispinfo, 20);
}
