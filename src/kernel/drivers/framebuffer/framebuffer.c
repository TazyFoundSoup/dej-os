#include <dej/limine.h>
#include <dej/kernel.h>

extern struct limine_framebuffer *framebuffer;
extern volatile uint32_t *fb_ptr;                       // get main to do our work lol

void framebuffer_init(){

}

/*
 * tuffest function ever
 * allowed at all irql lol
 *
 */
void putpixel(uint64_t x, uint64_t y, uint32_t colour){
    fb_ptr[y * (framebuffer->pitch / 4) + x] = colour;
}
