#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

enum{print, sprint};


char *convert(unsigned int num, int base){
  static char Representation[] = "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  ptr = &buffer[49];
  *ptr = '\0';

  do{
    ptr--;
    *ptr = Representation[num%base];
    num /= base;
  }while(num != 0);

  return ptr;
}

void print_str(char *s){
	for (int i=0; s[i] != 0; i++){
		_putc(s[i]);
	}
}

int is_digit(char c){
	if ('0' <= c && c <= '9')
		return 1;
	else
		return 0;
}

/**对字符串进行printf/sprintf的操作
 * 如果是printf函数-->直接打印
 * 如果是sprintf函数，将src的内容拷贝到dest中
 * type指示了不同的类型
 * @return 打印/拷贝的字符的长度
 **/
int operation_on_str(char *dest, char *src, int type){
	//int len = 0;
	switch (type)
	{
	case print:
		print_str(src);
		break;

	case sprint:
		strcpy(dest, src);
		break;
	
	//case test:
	//	break;

	default:
		printf("Operation_on_str went wrong!\n");
		//assert(1 == 0);
		break;
	}
	
	//len = strlen(src);
	return strlen(src);
}

/**对数字进行printf/sprintf的操作
 * 如果是printf函数-->直接打印
 * 如果是sprintf函数，将src的内容拷贝到dest中
 * type指示了不同的类型
 * @return 打印/拷贝的字符的长度
 **/
int operation_on_num(char *dest, unsigned int num,int type, int base){
	int len = 0;
	char *src;
	switch (type)
	{
	case print:
		if (base == 10){
			if (((int)num) < 0){
				_putc('-');
				len++;
				num = -num;
			}
		}
		src = convert(num, base);
		len += strlen(src);
		print_str(src);
		break;
	
	case sprint:
		if (base == 10){
			if (((int)num) < 0){
				*dest = '-';
				dest++;
				len++;
				num = -num;
			}
		}
		src = convert(num, base);
		len += strlen(src);
		strcpy(dest,src);	
		break;

/*
	case test:
		if (base == 10){
			if (((int)num) < 0){
				len++;
			}
		}
		src = convert(num, base);
		len += strlen(src);
		break;
*/	

	default:
		printf("Operation_on_num went wrong!\n");
		//assert(1 == 0);
		break;
	}
	return len;
}

int str_to_num(const char *str, int len){
	int result = 0;
	for (int i = 0; i < len; i++){
		result = result*10 + (str[i] - '0');
	}
	return result;
}


int choose_different_cases(const char **fmt_ptr,va_list ap, int type, char *str){
	char *s;
	char c = ' ';
	unsigned int i;
	int str_len = 0;
	switch(*(*fmt_ptr)){
		case 's':
			s = va_arg(ap, char*);
			str_len += operation_on_str(str, s, type);
			break;
			
		case 'c':
			i = va_arg(ap, int);
			switch (type)
			{
			case print:
				_putc(i);
				str_len++;
				break;

			case sprint:
				*str = (char)i;
				str_len++;
				break;

			default:
				printf("choose_different_cases went wrong!\n");
				//assert(1 == 0);
				break;
			}
			
			break;

		case 'd':
			i = va_arg(ap, int);
			str_len += operation_on_num(str, i, type, 10);
			break;
			
		case 'o':
			i = va_arg(ap, int);
			str_len += operation_on_num(str, i, type, 8);
			break;
			
		case 'x':
			i = va_arg(ap, int);
			str_len += operation_on_num(str, i, type, 16);
			break;
			
		case '0':
			c = '0';
			*(fmt_ptr) += 1;
			while (*(*fmt_ptr) == '0'){
				*(fmt_ptr) += 1;
			}
				

		case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		{
			const char *start = *(fmt_ptr);
			int num_len = 0;
			while(is_digit(*(*fmt_ptr))){
				num_len++;
				*(fmt_ptr) += 1;
			}
			int least_width = str_to_num(start, num_len);
			//va_list temp_ap = ap;
			const char *temp_fmt = *(fmt_ptr);
			char temp_str[100];
			int real_len = choose_different_cases(&temp_fmt, ap, sprint, temp_str);
			if (least_width > real_len){
				switch (type)
				{
				case print:
					for (int i = 0; i < least_width - real_len; i++){
						_putc(c);
						str_len++;
					}
					break;

				case sprint:
					for (int i = 0; i < least_width - real_len; i++){
						*str = c;
						str++;
						str_len++;
					}

				default:
					//assert(1 == 0);
					break;
				}
			}

			str_len += real_len;
			switch (type)
			{
			case print:
				printf(temp_str);
				break;
			
			case sprint:
				strncpy(str, temp_str, real_len);
				break;

			default:
				break;
			}
			break;
		}

		default:
			printf("Please Implement Me!!!\n");
		
	}
	return str_len;
}

int printf(const char *fmt, ...) {
	//unsigned int i;
	//char *s;

	va_list ap;
  	int len = 0;
	//const char *start = fmt;
  	va_start(ap, fmt);

	for (; *fmt != '\0'; fmt++){
    	if(*fmt != '%'){
			_putc(*fmt);
			len++;
			continue;
		}

    	fmt++;

		len += choose_different_cases(&fmt, ap, print, NULL);
	}

	va_end(ap);

  	return len;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  	char *str = out;
  
  	//char *s;
  	//unsigned int i;

	for (; *fmt != '\0'; fmt++){
    	if(*fmt != '%'){
			*str = *fmt;
			str++;
			continue;
		}

    	fmt++;

		str += choose_different_cases(&fmt, ap, sprint, str);
/*
		switch(*fmt){
			case 's':
				s = va_arg(ap, char*);
				strcpy(str, s);
				str += strlen(s);
				break;

			case 'c':
				i = va_arg(ap, int);
				*str = (char)i;
				str++;
				break;

			case 'd':
				i = va_arg(ap, int);
				if (((int)i) < 0){
					*str = '-';
					i = -i;
					str++;
				}
				s = convert(i, 10);
				strcpy(str, s);
				str += strlen(s);
				break;
			
			case 'o':
				i = va_arg(ap, int);
				s = convert(i, 8);
				strcpy(str, s);
				str += strlen(s);
				break;
			
			case 'X': case 'x':
				i = va_arg(ap, int);
				s = convert(i, 16);
				strcpy(str, s);
				str += strlen(s);
				break;
			
			default:				
				assert(1 == 0);
		}*/
	}

  	*str = '\0';
  	return str-out;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  int len;
  va_start(ap, fmt);

  len = vsprintf(out, fmt, ap);

  va_end(ap);

  return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
