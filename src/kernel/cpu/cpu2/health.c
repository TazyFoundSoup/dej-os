#include <dej/kernel.h>
#include <dej/cpu.h>
#include <dej/percpu.h>
#include <dej/stdio.h>
#include <dej/panic.h>

extern _Atomic uint64_t temperature;
/*
 * checks system health
 *
 */


static int16_t health;
_Noreturn void HealthMonitor(void){

    health = 0;


    printf("Health monitor waiting for the nerds\n");
    while ((uint64_t)(cpu_percpu[1]) & 0x01) cpu_takebreak();        // we need temperature to be on to check system health
    printf("Starting health monitoring\n");

    while (true){
        if (temperature > 90) {
            printf("High temperature temperature is at %i\n", atomic_load(&temperature) );
            health -= 10;
        }
        else if (temperature < 20) {
            printf("bit chilly eh temperature is at %llu\n", atomic_load(&temperature));
            health -= 10;
        }

        else {
            health += 20;
        }

        cpu_takebreak();

        if (health < -50) {
            panic("Doctor needed computer health dropped to extremly low levels");
        }
        cpu_takebreak();


    }

}
