// cpu.h
#pragma once
#include <stdint.h>
#include <stddef.h>

static inline void cpu_takebreak(void){
    __asm__ volatile ("pause" ::: "memory");
}
static inline void cpu_stop_interrupts(void){
    __asm__ volatile ("cli");
}
static inline void cpu_enable_interrupts(void){
    __asm__ volatile ("sti");
}
static inline void cpu_stop(void){
    cpu_stop_interrupts();
    while (1){
        __asm__ volatile ("hlt");
    }
}


/*
 * per cpu things
 * help
 *
 */
#define __percpu __attribute__((section(".percpu")))
#define DEFINE_PERCPU(type, name) \
    __percpu type name


extern char __percpu_start[];
extern char __percpu_end[];

#define percpu_size __percpu_end - __percpu_start



#define percpu_offsetof(var) \
    ((uint64_t)&(var) - (uint64_t)__percpu_start)


// Read a 64-bit per-CPU variable
#define percpu_read(var) ({ \
    uint64_t __val; \
    uint64_t __off = percpu_offsetof(var); \
    __asm__ volatile ( \
        "movq %%gs:(%1), %0" \
        : "=r"(__val) \
        : "r"(__off) \
        : "memory" \
    ); \
    __val; \
})

// Write to a 64-bit per-CPU variable
#define percpu_write(var, val) ({ \
    uint64_t __off = percpu_offsetof(var); \
    uint64_t __val = (uint64_t)(val); \
    __asm__ volatile ( \
        "movq %1, %%gs:(%0)" \
        : \
        : "r"(__off), "r"(__val) \
        : "memory" \
    ); \
})
