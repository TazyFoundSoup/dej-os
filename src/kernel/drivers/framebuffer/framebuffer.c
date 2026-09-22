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


void clearscreen(void){
    for (uint32_t x = 0; x < framebuffer->width; x++){
        for (uint32_t y = 0; y < framebuffer->height; y++){
            putpixel(x, y, 0x0);
        }
    }
}
