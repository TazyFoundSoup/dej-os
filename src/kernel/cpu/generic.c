#include <limine.h>
#include <dej/cpu.h>
#include <stdint.h>
#include "cpu1/temprature.h"
#include "../memory/memory.h"
#include <dej/panic.h>
#include <dej/string.h>
#include <dej/msr.h>
#include <dej/interrupt.h>
#include <dej/percpu.h>


#define MSR_GS_BASE 0xC0000101
/*
 *  irq is basically like which interrupt is being handled
 * sil is interrupt level to like mask stuff
 *  id is cpu id (laptic)
 *
 */

DEFINE_PERCPU(uint64_t, irq);
DEFINE_PERCPU(uint64_t, sil);
DEFINE_PERCPU(uint64_t, cpu_id);
DEFINE_PERCPU(_Atomic uint64_t, cpu_state);

void ap_entry(struct limine_mp_info *cpu){
    cpu_stop_interrupts();
    idt_init();

    if (percpu_size >= 4096){
        panic("percpu tables too big prob like something wrong or ill fix it later or something");
    }

    char * n_block = givemeapage();
    memset(n_block, 0, 4096);           // zero out

    memcpy(n_block, __percpu_start, percpu_size);


    wrmsr(MSR_GS_BASE, (uint64_t)n_block);

    percpu_write(cpu_id, cpu->lapic_id);

    cpu_enable_interrupts();
    temperature_entry() ;
}
