#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <stdio.h>

int main() {
  printf("haha hei!\n");
  NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  //printf("malloc successfully!\n");
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  //printf("loadbitmap successfully!\n");
  assert(bmp->pixels);
  //printf("assert successfully\n");
  NDL_OpenDisplay(bmp->w, bmp->h);
  //printf("opendisplay successfully!\n");
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  NDL_Render();
  NDL_CloseDisplay();
  while (1);
  return 0;
}
