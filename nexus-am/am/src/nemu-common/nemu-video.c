#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include "../../libs/klib/src/io.c"

#define W 400
#define H 300
//static uint32_t fb[W * H] = {};

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      int width_height = inl(SCREEN_ADDR);
      info->width = width_height & 0xffff;
      info->height = width_height >> 16;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      int width = screen_width();
      
      uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;

      uint32_t *start = fb + ctl->y*width + ctl->x;
      for (int y = 0; y < ctl->h; y++){
        for (int x = 0; x < ctl->w; x++){
          start[y * width + x] = ctl->pixels[y * ctl->w + x];
        }
      }

      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < size; i ++) fb[i] = i;
  draw_sync();
}
