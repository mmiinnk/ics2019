#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#ifndef __ISA_NATIVE__
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
#endif

#ifdef __ISA_NATIVE__
char *convert(unsigned long long num, int base){
  static char Representation[] = "0123456789ABCDEF";
  static char buffer[20];
  char *ptr;

  ptr = &buffer[19];
  *ptr = '\0';

  do{
    ptr--;
    *ptr = Representation[num%base];
    num /= base;
  }while(num != 0);

  return ptr;
}
#endif

int is_digit(char c){
	if ('0' <= c && c <= '9')
		return 1;
	else
		return 0;
}

int str_to_num(const char *str, int len){
	int result = 0;
	for (int i = 0; i < len; i++){
		result = result*10 + (str[i] - '0');
	}
	return result;
}

int printf(const char *fmt, ...) {
	va_list ap;
  	int len;
	char out[500];
  	va_start(ap, fmt);

	len = vsprintf(out, fmt, ap);

	for (int i = 0; i < len; i++){
		_putc(out[i]);
	}
	
	va_end(ap);

  	return len;
}

int cases_choose(const char **fmt_ptr, va_list *ap, char *dest){
	char *s;
	unsigned int i;
	char c;
	char *start = dest;

	switch(*(*fmt_ptr)){
		case 's':
			s = va_arg(*ap, char*);
			strcpy(dest, s);
			dest += strlen(s);
			break;
			
		case 'c':
			i = va_arg(*ap, int);
			*dest = (char)i;
			dest++;
			break;

		case 'd':
			i = va_arg(*ap, int);
			if (((int)i) < 0){
				*dest = '-';
				i = -i;
				dest++;
			}
			s = convert(i, 10);
			strcpy(dest, s);
			dest += strlen(s);
			break;
			
		case 'o':
			i = va_arg(*ap, int);
			s = convert(i, 8);
			strcpy(dest, s);
			dest += strlen(s);
			break;
			
		case 'x':
			i = va_arg(*ap, int);
			s = convert(i, 16);
			strcpy(dest, s);
			dest += strlen(s);
			break;

		case 'u':
			i = va_arg(*ap, int);
			s = convert(i, 10);
			strcpy(dest, s);
			dest += strlen(s);
			break;
			
		case '0':
			c = '0';
			(*fmt_ptr) += 1;
			while (*(*fmt_ptr) == '0'){
				(*fmt_ptr) += 1;
			}
				
		case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		{
			const char *start = *fmt_ptr;
			//char *str_start = dest;
			//int num_len = 0;
			while(is_digit(*(*fmt_ptr))){
				//num_len++;
				(*fmt_ptr) += 1;
			}
			int least_width = str_to_num(start, (*fmt_ptr)-start);
			char temp_str[100];

			cases_choose(fmt_ptr, ap, temp_str);
			int str_length = strlen(temp_str);
			if (least_width > str_length){
				for (int i = 0; i < least_width - str_length; i++){
					*dest = c;
					dest++;
				}
			}
			strcpy(dest, temp_str);
			dest += str_length;
			break;
		}
		default:
			printf("Please Implement Me!!!\n");
	}
	return dest-start;
}



int vsprintf(char *out, const char *fmt, va_list ap) {
  	char *str = out;
  
  	char *s;
  	unsigned int i;
	char c = ' ';

	for (; *fmt != '\0'; fmt++){
    	if(*fmt != '%'){
			*str = *fmt;
			str++;
			continue;
		}

    	fmt++;

		//str += cases_choose(&fmt, &ap, str);

		switch(*fmt){
		case 'p':
			strcpy(str, "0x");
			str += 2;
			void* temp_point;
			temp_point = va_arg(ap, void*);
			#ifdef __ISA_NATIVE__
			unsigned long long temp = (unsigned long long)temp_point;
			s = convert(temp, 16);
			#endif
			#ifndef __ISA_NATIVE__
			s = convert((unsigned int)temp_point, 16);
			#endif

			strcpy(str, s);
			str += strlen(s);
			break;
		
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
			
		case 'x':
			i = va_arg(ap, int);
			s = convert(i, 16);
			strcpy(str, s);
			str += strlen(s);
			break;

		case 'u':
			i = va_arg(ap, int);
			s = convert(i, 10);
			strcpy(str, s);
			str += strlen(s);
			break;
			
		case '0':
			c = '0';
			fmt += 1;
			while (*fmt == '0'){
				fmt += 1;
			}
				
		case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		{
			const char *start = fmt;
			while(is_digit(*fmt)){
				fmt += 1;
			}
			int least_width = str_to_num(start, fmt-start);
			char temp_str[100];
			char *temp_p = temp_str;

			switch(*fmt){
				case 's':
					s = va_arg(ap, char*);
					strcpy(temp_str, s);
					break;
			
				case 'c':
					i = va_arg(ap, int);
					temp_str[0] = (char)i;
					break;

				case 'd':
					i = va_arg(ap, int);
					if (((int)i) < 0){
						*temp_p = '-';
						i = -i;
						temp_p++;
					}
					s = convert(i, 10);
					strcpy(temp_p, s);
					break;
			
				case 'o':
					i = va_arg(ap, int);
					s = convert(i, 8);
					strcpy(temp_str, s);
					break;
			
				case 'x':
					i = va_arg(ap, int);
					s = convert(i, 16);
					strcpy(temp_str, s);
					break;

				case 'u':
					i = va_arg(ap, int);
					s = convert(i, 10);
					strcpy(temp_str, s);
					break;
			}
			
			int str_length = strlen(temp_str);
			if (least_width > str_length){
				for (int i = 0; i < least_width - str_length; i++){
					*str = c;
					str++;
				}
			}

			strcpy(str, temp_str);
			str += str_length;
			break;
		}

		default:
			printf("Please Implement Me!!!!!\n");
		}
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
	printf("Please implement me!\n");
	assert(0);
	return 0;
}

#endif


/*
enum{print, sprint};

void print_str(char *s){
	for (int i=0; s[i] != 0; i++){
		_putc(s[i]);
	}
}
*/
/**对字符串进行printf/sprintf的操作
 * 如果是printf函数-->直接打印
 * 如果是sprintf函数，将src的内容拷贝到dest中
 * type指示了不同的类型
 * @return 打印/拷贝的字符的长度
 **/
/*
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
		assert(1 == 0);
		break;
	}
	
	//len = strlen(src);
	return strlen(src);
}
*/
/**对数字进行printf/sprintf的操作
 * 如果是printf函数-->直接打印
 * 如果是sprintf函数，将src的内容拷贝到dest中
 * type指示了不同的类型
 * @return 打印/拷贝的字符的长度
 **/
/*
int operation_on_num(char *dest, unsigned int num, int type, int base){
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

		if (base == -1){
			base = 10;
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

		if (base == -1){
			base = 10;
		}
		src = convert(num, base);
		len += strlen(src);
		strcpy(dest,src);	
		break;
*/
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
/*
	default:
		printf("Operation_on_num went wrong!\n");
		assert(1 == 0);
		break;
	}
	return len;
}




int choose_different_cases(const char **fmt_ptr, va_list ap, int type, char *str){
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
				assert(1 == 0);
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

		case 'u':
			i = va_arg(ap, int);
			str_len += operation_on_num(str, i, type, -1);
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
					assert(1 == 0);
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