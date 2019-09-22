#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

// this should be enough
static char buf[65536];

static unsigned buf_index = 0;

uint32_t choose(uint32_t n){
	return rand()%n;
}

void gen_num(){
	unsigned num = choose(100);
	char num_str[10];
	int len = 0;

	do{
		num_str[len] = (num%10) + '0';
		num /= 10;
		len++;
	}while(num != 0);

	for (int i=0;i<len;i++){
		buf[buf_index] = num_str[len-i-1];
		buf_index++;
	}

	int block = rand()%3;
	for (int i=0;i<block;i++){
		buf[buf_index] = ' ';
		buf_index++;
	}
	buf[buf_index] = '\0';
}

void gen(char c){
	buf[buf_index] = c;
	buf_index++;
	
	int block = rand()%3;
	for (int i=0;i<block;i++){
		buf[buf_index] = ' ';
		buf_index++;
	}
	buf[buf_index] = '\0';
}

void gen_rand_op(){
	uint32_t choice = choose(4);
	switch(choice){
		case 0: buf[buf_index] = '+';break;
		case 1: buf[buf_index] = '-';break;
		case 2: buf[buf_index] = '*';break;
		default: buf[buf_index] = '/';
	}
	buf_index++;

	int block = rand()%3;
	for (int i=0;i<block;i++){
		buf[buf_index] = ' ';
		buf_index++;
	}
	buf[buf_index] = '\0';
}

static unsigned int gen_count = 0;

static inline void gen_rand_expr() {
	gen_count++;
	if ((buf_index+gen_count*5+4)>65536){
		gen_num();
		return;
	}

	uint32_t choice = choose(3);
	
	switch(choice){
		case 0: gen_num();break;
		case 1: gen('('); gen_rand_expr(); gen(')'); break;
		default: gen_rand_expr(); gen_rand_op(); gen_rand_expr();break;
	}
	gen_count--;
}

void init_buf(){
    buf_index = 0;
    buf[0] = '\0';
    gen_count = 0;
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";


int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf); //insert the buf

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result = 0;
    fscanf(fp, "%d", &result);
    pclose(fp);


    if (result != 0){
	    printf("%u %s\n", result, buf);
    }
    init_buf();
  }
  return 0;
}
