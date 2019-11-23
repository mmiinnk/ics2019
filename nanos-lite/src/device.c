#include "common.h"
#include <amdev.h>

ssize_t fs_read(int fd, void *buf, size_t len);

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //printf("haha\n");
  //printf("len = %d\n", len);
  const char *tempbuf = buf;
  for (size_t i = 0; i < len; i++){
    _putc(tempbuf[i]);
    //printf("%c", tempbuf[i]);
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
  strncpy(buf, dispinfo + offset, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  offset /= 4;
  int x = offset % screen_width();
  int y = offset / screen_width();
  draw_rect((uint32_t *)buf, x, y, len/4, 1);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d", screen_width(), screen_height());
  //Log("VGA WIDTH:%d HEIGHT:%d", screen_width(), screen_height());
}
