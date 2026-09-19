#include <stdint.h>
void InInterruptInit(void);
int LoRegisterInterruptVector(uint8_t vector, void (*handler)(void));
