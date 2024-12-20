/*
 * Copyright (c) 2017-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_tirtos.c ========
 */

/* RTOS header files */
#include <stdint.h>
#include <pthread.h>
#include <ti/sysbios/BIOS.h>
//#include <ti/display/Display.h>

#include <ti/drivers/Board.h>
#include "ph001.h"
extern void ti_ndk_config_Global_startupFxn();
Globals Glo = {
               .ver = "12.7.1",
               .author = "Nathan Bartley",
               .partners = "Shailendra Shrestha, Emilio Cervantez, Mark Dannemiller",
               .assignment = "HW13",
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
//Display_Handle display;
void __attribute__((weak)) NDK_hookInit(int32_t id) {}

/*
 *  ======== main ========
 */
int main(void)
{
    Board_init();

//    Display_init();

//    display = Display_open(Display_Type_UART, NULL);
//    if (display == NULL) {
//        /* Failed to open display driver */
//        while(1);
//    }

    ti_ndk_config_Global_startupFxn();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
