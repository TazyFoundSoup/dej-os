#include <stdint.h>
void putpixel(uint64_t x, uint64_t y, uint32_t colour);
void clearscreen(void);
struct limine_framebuffer * RequestFrameBuffer(uint8_t num);
