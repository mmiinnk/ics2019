#include "nemu.h" 

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

#define NEGATIVE 'N'

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"-", '-'},         // minus
  {"\\+", '+'},         // plus
  {"[0-9]+", TK_NUM},      // numbers
  {"\\(", '('},         // left bracket
  {"\\)", ')'},         // right bracket
  {"/", '/'},           // divide
  {"\\*", '*'},         // multiply
  {"==", TK_EQ}         // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )
/*NR_REGEX is the number of rules*/

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

//static Token tokens[32] __attribute__((used)) = {};
static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0){
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //    i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
		case TK_NOTYPE: break;
		case TK_NUM: {
				if (substr_len > 31) printf("The Number is too big! The precision may be lost!\n");
				for (int index = 0; index < substr_len; index++)
				     tokens[nr_token].str[index] = substr_start[index];
				tokens[nr_token].str[substr_len] = '\0';
			     };
                default: tokens[nr_token].type = rules[i].token_type; nr_token++;
	
	}

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_brackets(int p, int q){
	int match = 0;
	for (int i=p; i<=q; i++){
		if (tokens[i].type == '(')
			match++;
		if (tokens[i].type == ')')
			match--;
		if (match < 0){
			//printf("位置在%d\n",i);
			return false;
		}
	}
	if (match == 0)
		return true;
	else{
		//printf("match = %d\n",match);
		//printf("p = %d\n",p);
		//printf("q = %d\n",q);
		return false;
	}
}


bool check_parentheses(int p, int q){
	if (tokens[p].type != '(' || tokens[q].type != ')')
		return false;
	return check_brackets(p+1, q-1);

}

int str_to_num(char *str, int p){
	int num = 0;
	int sign = 1;
	if ((p != 0) && (tokens[p-1].type == NEGATIVE)){
		sign = -1;
	}	
	for (int i=0; i<strlen(str); i++){
		num = num*10 + (str[i] - '0');
	}
	return num*sign;
}

int find_main_op(int p, int q){
	struct{
		int priority;
		int loc;
	} ops[32];
	int i = p;
	int op_num = 0;
	while(i <= q){		
		if (tokens[i].type == '+' || tokens[i].type == '-' ){
			ops[op_num].priority = 0;
			ops[op_num].loc = i;
			i++;
			op_num++;
		}
		else if (tokens[i].type == '*' || tokens[i].type == '/'){
			ops[op_num].priority = 1;
			ops[op_num].loc = i;
			i++;
			op_num++;
		}
		else if ((tokens[i].type == '(')){
			int temp = i;
			i++;
			while(!check_brackets(temp,i))
				i++;
		}
		else{
			i++;
		}
	}
	int priority_min = 1;
	for (int i=0;i<op_num;i++){
		if (ops[i].priority < priority_min)
			priority_min = ops[i].priority;
	}
	int main_op = 0;
	for (int i=0;i<op_num;i++){
		if (ops[i].priority == priority_min)
			main_op = ops[i].loc;
	}
	return main_op;
}

uint32_t eval(int p, int q){
	if (p > q){
		//*success = false;
		assert(0);
	}
	else if (p == q){
		return str_to_num(tokens[p].str, p);
	}
	else if (check_parentheses(p,q) == true){
		return eval(p+1, q-1);
	}
	else if (!check_brackets(p,q)){
		printf("Invalid Expression!\n");
		//*success = false;
		assert(0);
	} else if (tokens[p].type == NEGATIVE)
		return eval(p+1,q);
	else{
		int op = find_main_op(p,q);
		uint32_t val1 = eval(p, op-1);
		uint32_t val2 = eval(op+1, q);

		switch(tokens[op].type){
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': if (val2 == 0){
					  printf("Can't divide zero!\n");
					  printf("p = %d\n",p);
					  printf("q = %d\n",q);
					  printf("op = %d\n",op);
					  //*success = false;
					  assert(0);
				  } return val1 / val2;
			default: assert(0);
		}
	}

}



uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  for (int i=0;i<nr_token;i++){
	  if (tokens[i].type == '-' && (i == 0 || tokens[i-1].type == '+' || tokens[i-1].type == '-' || tokens[i-1].type == '*' || tokens[i-1].type == '/'))
		  tokens[i].type = NEGATIVE;
  }

  return eval(0,nr_token-1);


  //return 0;
}
