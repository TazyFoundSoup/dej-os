#pragma once
#include "x86/cpu.h"
#include <dej/cpu.h>
#include <stdint.h>



extern DEFINE_PERCPU(uint64_t, irq);
extern DEFINE_PERCPU(uint64_t, sil);
extern DEFINE_PERCPU(uint64_t, cpu_id);
extern DEFINE_PERCPU(uint64_t, cpu_state);
