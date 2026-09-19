#include <stdint.h>
void idt_init(void);
int LoRegisterInterruptVector(uint8_t vector, void (*handler)(void));
