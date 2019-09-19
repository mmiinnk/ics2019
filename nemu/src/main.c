#include <stdio.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  FILE *fp = NULL;
  fp = fopen("~/ics2019/nemu/tools/gen-expr/input","r");
  char result[10];
  char expression[655];
  fscanf(fp, "%s", result);
  fscanf(fp, "%s", expression);
  printf("result 1: %s\n", result);
  printf("expression 1: %s\n", expression);

  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
