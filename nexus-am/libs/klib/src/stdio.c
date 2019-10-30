#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

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

void print_num(unsigned int i, int base){
	if (base == 10){
		if (((int)i) < 0){
			_putc('-');
			i = -i;
		}
	}
	
	char* s = convert(i, base);
	print_str(s);
}

int printf(const char *fmt, ...) {
	unsigned int i;
	char *s;

	va_list ap;
  	int len;
	const char *start = fmt;
  	va_start(ap, fmt);

	for (; *fmt != '\0'; fmt++){
    	if(*fmt != '%'){
			_putc(*fmt);
			continue;
		}

    	fmt++;

		switch(*fmt){
			case 's':
				s = va_arg(ap, char*);
				print_str(s);
				break;

			case 'd':
				i = va_arg(ap, int);
				print_num(i, 10);
				break;
			
			case 'o':
				i = va_arg(ap, int);
				print_num(i, 8);
				break;
			
			case 'x':
				i = va_arg(ap, int);
				print_num(i, 16);
				break;
			
			default:
				assert(1 == 0);
		}
	}

	va_end(ap);

	len = fmt - start;

  	return len;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  	char *str = out;
  
  	char *s;
  	unsigned int i;

	for (; *fmt != '\0'; fmt++){
    	if(*fmt != '%'){
			*str = *fmt;
			str++;
			continue;
		}

    	fmt++;

		switch(*fmt){
			case 's':
				s = va_arg(ap, char*);
				strcpy(str, s);
				str += strlen(s);
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
			
			default:
				assert(1 == 0);
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
  return 0;
}

#endif
