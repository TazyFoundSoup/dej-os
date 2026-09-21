#include <dej/kernel.h>
#include <dej/cpu.h>
#include <dej/limine.h>




_Noreturn void kmain(void){

    size_t frame = 0;

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    volatile uint32_t *fb_ptr = framebuffer->address;


    while (1) {
        for (size_t y = 0; y < framebuffer->height; y++) {
            for (size_t x = 0; x < framebuffer->width; x++) {

                uint32_t red = 256;
                uint32_t green = (y + frame) % 256;
                uint32_t blue =   (x + frame) % 256;
                fb_ptr[y * (framebuffer->pitch / 4) + x] =
                    (red << 16) | (green << 8) | blue;
            }
        }
        for (int i = 0; i < 100; i++) cpu_takebreak();
        frame++;
    }


    cpu_stop();
}
