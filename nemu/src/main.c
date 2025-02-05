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
	uint32_t true_num = 0;
	for (int i=0;result[i]!='\0';i++){
		true_num = true_num*10 + result[i] - '0';
	}
	return true_num;
}

int main(int argc, char *argv[]) {

  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

/*
  FILE *fp = NULL;
  fp = fopen("/home/victor/ics2019/nemu/tools/gen-expr/input","r");
  
  char result[11];
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
	

	//bool tmp = true;
	//bool *success = &tmp;
	bool* success = (bool*) malloc(sizeof(bool));
	
	*success = true;
	//unsigned int len = strlen(expression);
	//char* exp_for_test = (char*)malloc((len+1)*sizeof(char));
	char exp_for_test[65536] = {'\0'};
	int i = 0;
	for (;expression[i] != '\n';i++){
		exp_for_test[i] = expression[i];
	}
	exp_for_test[i] = '\0';
	uint32_t myAnswer = expr(exp_for_test, success);
	if (success){
		uint32_t trueAnswer = to_num(result);
		if (trueAnswer != myAnswer){
			printf("trueAnswer: %u\n", trueAnswer);
			printf("myAnswer: %u\n", myAnswer);
			expr_right = false;
		}
		printf("%u\n",myAnswer);
	}
	start = ftell(fp);
  }
  fclose(fp);

  if (expr_right)
	  printf("Bingo! You made it!!!\n");
  else
	  printf("Sorry! Maybe you have to debug again.\n");

*/
	//is_batch_mode = false;
  	/* Receive commands from user. */
	ui_mainloop(is_batch_mode);

  return 0;
}
