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

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *str = out;
  
  char *s;
  unsigned int i;

  for (; *fmt != '\0'; fmt++)
  {
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
		  str++;
		}
		s = convert(i, 10);
		strcpy(str, s);
		
		str += strlen(s);
		break;
	    case 'o':
		i = va_arg(ap, int);
		if (((int)i) < 0){
		  *str = '-';
		  str++;
		}
		s = convert(i, 8);
		strcpy(str, s);
		str += strlen(s);
		break;
	    case 'x':
		i = va_arg(ap, int);
		if (((int)i) < 0){
		  *str = '-';
		  str++;
		}
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
