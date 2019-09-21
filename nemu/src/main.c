#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include "nemu.h"
//#include "monitor/debug/expr.c"

int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char*, bool*);

uint32_t to_num(char *result){
	int true_num = 0;
	for (int i=0;result[i]!='\0';i++){
		true_num = true_num*10 + result[i] - '0';
	}
	return true_num;
}

int main(int argc, char *argv[]) {
  FILE *fp = NULL;
  fp = fopen("/home/victor/ics2019/nemu/tools/gen-expr/input","r");
  
  char result[10];
  char expression[65536];
  
  fseek(fp, 0L, SEEK_END);
  long end = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  long start = ftell(fp);

  bool expr_right = true;

  while(end != start){
  	fscanf(fp, "%s", result);
  	fgets(expression, 65536, (FILE*)fp);
	
	//printf("result: %s\n", result);
	//printf("expression: %s\n", expression);
	
	bool* success = (bool*) malloc(1);
	uint32_t myAnswer = expr(expression, success);
	if (success){
		uint32_t trueAnswer = to_num(result);
		if (trueAnswer != myAnswer)
			expr_right = false;
	}
	start = ftell(fp);
  }
  fclose(fp);

  if (expr_right)
	  printf("Bingo! You made it!!!\n");


  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
