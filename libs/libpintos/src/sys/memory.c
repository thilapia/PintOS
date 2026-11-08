#include <pintos/sys/memory.h>

void* k_memset(void* dest, uint8_t value, uint64_t size) {
  unsigned char* p = dest;

  while (size--) {
    *p++ = value;
  }

  return dest;
}

void* k_memcpy(void* dest, const void* src, uint64_t size) {
  unsigned char* d = dest;
  const unsigned char* s = src;

  while (size--) {
    *d++ = *s++;
  }

  return dest;
}
