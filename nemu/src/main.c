#include <stdio.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  FILE *fp = NULL;
  fp = fopen("/home/victor/ics2019/nemu/tools/gen-expr/input","r");
  
  char result[10];
  char expression[65536];
  
  fseek(fp, 0L, SEEK_END);
  long end = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  long start = ftell(fp);

  while(end != start){
  	fscanf(fp, "%s", result);
  	fgets(expression, 65536, (FILE*)fp);
	
	printf("result: %s\n", result);
	printf("expression: %s\n", expression);

	start = ftell(fp);

  }
  
  //bool* success = (bool*)malloc(1);
  //expr(expression, success);
  //if (success){
//
  //}

  fclose(fp);


  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
