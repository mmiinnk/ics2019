#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <math.h>
#include "../../libs/klib/src/io.c"

//#define W 400
//#define H 300

static inline int min(int x, int y) {
  return (x < y) ? x : y;
}

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      
      int width_height = inl(SCREEN_ADDR);
      info->height = width_height & 0xffff;
      info->width = (width_height >> 16) & 0xffff;
      
      //info->width = inw(SCREEN_ADDR + 2);
      //info->height = inw(SCREEN_ADDR);
      
      //info->width = 400;
      //info->height = 300;
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
      int x = ctl->x, y = ctl->y, h = ctl->h, w = ctl->w;
      uint32_t *pixels = ctl->pixels;
      uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
      for (int i = 0; i < h; i++)
      {
        for (int j = 0; j < w; j++)
          fb[(y + i) * width + x + j] = pixels[i * w + j]; 
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
