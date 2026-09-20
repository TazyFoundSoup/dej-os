// cpu.h
#pragma once
#include <dej/kernel.h>

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

typedef uint8_t ksil;


ksil RaiseSil(ksil new);
void LowerSil(ksil old);
