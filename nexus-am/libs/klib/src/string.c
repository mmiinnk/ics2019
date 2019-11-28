#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if (s == NULL){
    return -1;
  }
  size_t count = 0;
  while(s[count] != '\0'){
    count++;
  }
  return count;
}

char *strcpy(char* dst,const char* src) {
  assert(dst != NULL && src != NULL);
  size_t i;

  for (i = 0; src[i] != '\0'; i++)
    dst[i] = src[i];
  
  dst[i] = '\0';

  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(dst != NULL && src != NULL);
  size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++)
    dst[i] = src[i];
  for ( ; i < n; i++)
    dst[i] = '\0';

  return dst;
}

char* strcat(char* dst, const char* src) {
  if (dst == NULL) return NULL;
  if (src == NULL) return dst;

  size_t dst_len = strlen(dst);
  size_t i;

  for (i = 0; src[i] != '\0'; i++)
    dst[dst_len + i] = src[i];
  
  dst[dst_len + i] = '\0';

  return dst;
}

int strcmp(const char* s1, const char* s2) {
  //assert(s1);
  //assert(s2);
  //if (s1 == NULL || s2 == NULL){
  //  return 0;
  //}
  //size_t i;
  //size_t len1 = strlen(s1);
  //size_t len2 = strlen(s2);

  //for (i = 0; i <= len1 && i <= len2; i++){
  //  int ret = (unsigned)s1[i] - (unsigned)s2[i];
  //  if (ret != 0)
  //    return ret;
  //}

  while (*s1++ == *s2++){
    if (*s1 == '\0') return 0;
  }
  if (*s1 > *s2) return 1;
  else
  {
    return -1;
  }
}

int strncmp(const char* s1, const char* s2, size_t n) {
  //if (s1 == NULL || s2 == NULL){
  //  return 0;
  //}
  //if (n == 0)
  //  return 0;

  size_t i;
  //size_t len1 = strlen(s1);
  //size_t len2 = strlen(s2);

  //for (i = 0; i < n && i <= len1 && i <= len2; i++){
  //  int ret = (unsigned)s1[i] - (unsigned)s2[i];
  //  if (ret != 0)
  //    return ret;
  //}

  for (i = 0; i < n && s1[i] == s2[i]; i++){
    if (s1[i] == '\0') return 0;
  }
  if (i >= n) return 0;
  if (s1[i] > s2[i]) return 1;
  else{
    return -1;
  } 
  //return 0;
}

void* memset(void* v,int c,size_t n) {
  if (v == NULL){
    return v;
  }

  const unsigned char uc = c;
  unsigned char *vu = v;
  for (; 0 < n; ++vu, --n)
    *vu = uc;

  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  if (out == NULL){
    return out;
  }
  if (n == 0)
    return out;

  unsigned char *p1 = out;
  const unsigned char *p2 = in;
  for ( ;n > 0; n--){
    //assert((int)p1 != 0x1c04820);
    *p1 = *p2;
    p1++;
    p2++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  //assert(s1 != NULL && s2 != NULL);
  if (n == 0)
    return 0;

  const unsigned char *p1 = s1, *p2 = s2;
  for (;n > 0; n--){
    int ret = *p1 - *p2;
    if (ret != 0)
      return ret;
    p1++;
    p2++;
  }

  return 0;
}

#endif
