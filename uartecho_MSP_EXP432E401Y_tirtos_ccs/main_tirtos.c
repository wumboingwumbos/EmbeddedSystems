#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>

/* Global Variables */
#include "ph001.h"
Globals Glo = {
               .ver = "12.7.1",
               .author = "Nathan Bartley",
               .partners = "Shailendra Shrestha, Emilio Cervantez",
               .assignment = "HW11",
               //helpful globals
               .home ="\033[H",
               .clear = "\033[2J",
               //Error globals
               .freq = 0,
               .uce = 0,
               .bfe = 0,
               .me = 0,
               .ide = 0,
               .qoe = 0,
               .re = 0,
               .Timer0Period = 1000000
};
#define MAIN
void __attribute__((weak)) NDK_hookInit(int32_t id) {}

/*
 *  ======== main ========
 */

int main(void)
{
    Board_init();
    BIOS_start();
    return (0);
}
