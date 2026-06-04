#pragma once

#include "types.cpp"

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile ("inb %w1, %b0"
            : "=a" (result)
            : "Nd" (port));
    return result;
}

static inline void outb(uint16_t port, uint8_t data) {
    asm volatile ("outb %b0, %w1"
            :
            : "a" (data), "Nd" (port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    asm volatile ("inw %w1, %w0"
            : "=a" (result)
            : "Nd" (port));
    return result;
}

static inline void outw(uint16_t port, uint16_t data) {
    asm volatile ("outw %w0, %w1"
            :
            : "a" (data), "Nd" (port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t value;
    __asm__ volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void outl(uint16_t port, uint32_t value) {
    __asm__ volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

static inline void memory_barrier() {
    asm volatile("mfence" ::: "memory");
}

static inline void load_barrier() {
    asm volatile("lfence" ::: "memory");
}

static inline void store_barrier() {
    asm volatile("sfence" ::: "memory");
}

static inline void compiler_barrier() {
    asm volatile("" ::: "memory");
}

// Из os/kernel/CPU/cpu.cpp
static inline void cli() {
    asm volatile("cli" ::: "memory");
}

static inline void sti() {
    asm volatile("sti" ::: "memory");
}

static inline void xsave(void* buffer) {
    uint64_t mask_edx = 0xFFFFFFFF;
    uint64_t mask_eax = 0xFFFFFFFF;
    __asm__ volatile (
        "xsave (%%rdi)\n\t"
        :
        : "D" (buffer), "a" (mask_eax), "d" (mask_edx)
        : "memory"
    );
}

static inline void xrstor(void* buffer) {
    uint64_t mask_edx = 0xFFFFFFFF;
    uint64_t mask_eax = 0xFFFFFFFF;
    __asm__ volatile (
        "xrstor (%%rdi)\n\t"
        :
        : "D" (buffer), "a" (mask_eax), "d" (mask_edx)
        : "memory"
    );
}
