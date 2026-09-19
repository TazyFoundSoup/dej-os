//percpu.c
//
#include <stdint.h>
#include <dej/percpu.h>


uint8_t *cpu_percpu[MAX_CPUS];
