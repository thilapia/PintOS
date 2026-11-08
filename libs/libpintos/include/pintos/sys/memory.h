#ifndef PINTOS_SYS_MEMORY_H
#define PINTOS_SYS_MEMORY_H

#include <stdint.h>

/**
 * Set a block of memory to an specific value by `size` bytes.
 *
 * This function does not validate that the destination can be correctly
 * indexed by `size`.
 */
void* k_memset(void* dest, uint8_t value, uint64_t size);

/**
 * Copy memory sequentially from the source to the destination.
 *
 * The amount of bytes are bound by the size provided, but no checks are
 * actually done in the source and destination if the indexes are valid.
 */
void* k_memcpy(void* dest, const void* src, uint64_t size);

#endif /* PINTOS_SYS_MEMORY_H */
