#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

//#define PMEM_SIZE (128*1024*1024)

void cpu_exec(uint64_t);
void isa_reg_display(void);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_steps(char *args){
	char* n_char = strtok(NULL, " ");
	if (n_char == NULL)
		cpu_exec(1);
	else
	{
		int n = 0;
		for (int i=0; *(n_char+i) != '\0'; i++)
		{
			n = n*10 + (*(n_char+i) - 48);
		}
		cpu_exec(n);
	}
	return 0;
}

static int cmd_info(char *args){
	char* cmd_plus = strtok(NULL," ");
	if (cmd_plus == NULL){
		printf("Please tell me want Information you want!\n");
	}
	else if (strcmp(cmd_plus,"r") == 0){
		isa_reg_display();
	}
	else if (strcmp(cmd_plus,"w") == 0){
		
	}
	else
	{
		printf("Info Format Wrong!! Right format: info r/w\n");
	}
	return 0;
}

unsigned int htd(char *str){
	unsigned int  num = 0;
	for (int i=2;*(str+i) != '\0';i++){
		num = num*16 + (*(str+i) - 48);
	}
	return num;
}

bool isnum(char *str){
	bool is_num = true;
	for (int i=0;*(str+i) != '\0'; i++)
	{
		if ((*str+i)<=0x30 || *(str+i) >= 0x39)
		{
			is_num = false;
			break;
		}
	}
	return is_num;	
}

void scan_mem_format_wrong(){
	printf("Wrong Format!!! Right Format: x [N] <address>\n");
}

static int cmd_scan_mem(char *args){
	int n = 0;
	unsigned int address;
	char *n_char = strtok(NULL, " ");
	if (n_char == NULL)
	{
		scan_mem_format_wrong();
		return 0;
	}
	
	if (*(n_char) == '0' && *(n_char+1) == 'x' && isnum(n_char+2))
	{
		address = htd(n_char);
		if ((address) > PMEM_SIZE)
		{
			printf("Cannot find the memory!\n'");
			return 0;
		}
		//printf("%x\n",address);
		printf("%s: 0x",n_char);
		for (int i=0;i<4;i++)
		{
			if ((address+i) > PMEM_SIZE)
			{
				printf("Cannot find the memory!\n'");
				return 0;
			}
			printf("%02x",pmem[address+i]);
		}
		printf("\n");
		return 0;
	}

	if (!isnum(n_char)){
		scan_mem_format_wrong();
		return 0;
	}

	char *address_char = strtok(NULL, " ");
	if (address_char == NULL ||  *(address_char) != '0' || *(address_char+1) != 'x' || !(isnum(address_char+2))){
		scan_mem_format_wrong();
		return 0;
	}

	address = htd(address_char);
	for (int i=0; *(n_char+i) != '\0'; i++)
	{
		n = n*10 + (*(n_char+i) - 48);
	}
	unsigned int count = 0;
	for (int i=0;i<n;i++)
	{
		if ((address+i) > PMEM_SIZE)
		{
			printf("Cannot find the memory!\n'");
			return 0;
		}
		if (count%16 == 0)
		{
			if (count != 0)
				printf("\n");
			printf("0x%x: 0x", (address+i));
		}
		else if (count%4 == 0)
		       printf("     0x");
		printf("%02x", pmem[address+i]);
		count++;
	}
	printf("\n");
	return 0;
	
}

static int cmd_evaluate(char *args){
	bool *success = NULL;
	*success = true;
	uint32_t result = expr(args, success);
	if (!success){
		printf("Evaluation failed!\n");
		return 0;
	}
	printf("%d\n",result);
	return 0;
}



static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single Step", cmd_steps},
  {"info", "Print Information", cmd_info},
  {"x", "Scan the memory", cmd_scan_mem},
  {"p", "Evaluate the value", cmd_evaluate},

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
