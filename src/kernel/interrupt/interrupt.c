#include <stdint.h>
#include <dej/cpu.h>
#include <x86/x86.h>
#include <dej/stdio.h>
#include <dej/panic.h>

static bool vectors[256];       // if the vector is used or sno

typedef struct {
    // General-purpose registers (pushed manually by assembly)
    uint64_t rax, rcx, rdx, rbx, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15;

    // Hardware frame
    uint64_t ec, rip, cs, rflags;
} __attribute__((packed)) gp_registers_t;

struct InterruptDescriptor {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attributes;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t zero;
}__attribute__((packed));

_Static_assert(sizeof(struct InterruptDescriptor) == 16,
               "IDT entry must be 16 bytes");

struct IDTR {
    uint16_t limit;
    uint64_t base;
}__attribute__((packed));

_Static_assert(sizeof(struct IDTR) == 10, "IDTR must be 10 bytes");

struct InterruptDescriptor idt[256];

void idt_set_gate(uint8_t vector, void (*handler)(void))
{
    uint64_t addr = (uint64_t)handler;

    idt[vector].offset_1 = addr & 0xFFFF;
    idt[vector].selector = 0x28;
    idt[vector].ist = 0;
    idt[vector].type_attributes = 0x8E;
    idt[vector].offset_2 = (addr >> 16) & 0xFFFF;
    idt[vector].offset_3 = (addr >> 32) & 0xFFFFFFFF;
    idt[vector].zero = 0;

    vectors[vector] = true;
}



extern void int_divide_by_0(void);
void divide_by_0_handler(void){
    serial_puts("Division by 0 occured");
    cpu_stop();
}

extern void int_nmi(void);
//in nmi.c

extern void int_general_protection_fault(void);
void general_protection_fault_handler(gp_registers_t * frame){
    printf("gp fault:  rflags %lu cs %lu rip %lu error code %lu", frame->rflags, frame->cs, frame->rip, frame->ec);

    cpu_stop();
}


extern void int_page_fault(void);
void page_fault_handler(void){
    serial_puts("page fault");
    cpu_stop();
}





void InterruptInit(void){


    idt_set_gate(0, int_divide_by_0);
    idt_set_gate(2, int_nmi);
    idt_set_gate(13, int_general_protection_fault);
    idt_set_gate(14, int_page_fault);


    struct IDTR idtr = {
        .limit = sizeof(idt) - 1,
        .base = (uint64_t)idt
    };




    x86_load_idt(&idtr);
}


/*
 * LoRegisterInterruptVector: attempts to register a interrupt handler
 * Vector: Interrupt vector requested
 * *Handler: function pointer to le interrupt handler
 * Name: name of function not driver
 *
 */
void RegisterInterruptVector(uint8_t vector, void (*handler)(void), char * name){
    if (vector <= 64) {
        panic("Attempted register reserved interrupt vector");
    }


    if (vectors[vector] == true){
        printf("vector %u used crashing", vector);
        panic("Vector in use ");
    }

    idt_set_gate(vector, handler);

    printf("Interrupt vector %u registered succesfully to %s", vector, name);

    return;
}
