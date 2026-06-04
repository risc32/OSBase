#pragma once

#include "types.cpp"

#define GENERAL_ONLY __attribute__((target("general-regs-only")))
#define INLINE __attribute__((always_inline))
extern "C" {
void GENERAL_ONLY INLINE memcpy(void* dest, const void* src, size_t size) {
    __builtin_memcpy(dest, src, size);
}

int GENERAL_ONLY INLINE memcmp(const void* a, const void* b, size_t size) {
    return __builtin_memcmp(a, b, size);
}

void GENERAL_ONLY INLINE memset(void* dest, int val, size_t size) {
    __builtin_memset(dest, val, size);
}

void GENERAL_ONLY INLINE memzero(void* dest, size_t size) {
    __builtin_memset(dest, 0, size);
}
}
