/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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
 *    ======== udpEchoHooks.c ========
 */
#include "ph001.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <pthread.h>

#include <ti/ndk/inc/netmain.h>

#include <ti/ndk/slnetif/slnetifndk.h>
#include <ti/net/slnet.h>
#include <ti/net/slnetif.h>
#include <ti/net/slnetutils.h>

//#include <ti/display/Display.h>
#include <ti/drivers/emac/EMACMSP432E4.h>

#define UDPPORT 1000

#define UDPHANDLERSTACK 4096
#define IFPRI  4   /* Ethernet interface priority */

extern void *ListenFxn(void *arg0);
extern void *TransmitFxn(void *arg0);

/*
 *  ======== netIPAddrHook ========
 *  user defined network IP address hook
 */
void netIPAddrHook(uint32_t IPAddr, unsigned int IfIdx, unsigned int fAdd)
{
    char message[128];
    size_t buffer_size;
    pthread_t          thread;
    pthread_attr_t     attrs;
    struct sched_param priParam;
    int                retc;
    int                detachState;
    uint32_t           hostByteAddr;
    static uint16_t    arg0 = UDPPORT;
    static bool        createTask = true;
    int32_t            status = 0;

    if (fAdd) {
//        Display_printf(display, 0, 0, "Network Added: ");
        sprintf(message, "Network Added\r\n");
        UART_write(uart, message, strlen(message));
    }
    else {
//        Display_printf(display, 0, 0, "Network Removed: ");
        sprintf(message, "Network Removed\r\n");
        UART_write(uart, message, strlen(message));
    }

    /* print the IP address that was added/removed */
    hostByteAddr = NDK_ntohl(IPAddr);
//    Display_printf(display, 0, 0, "If-%d:%d.%d.%d.%d\n", IfIdx,
//            (uint8_t)(hostByteAddr>>24)&0xFF, (uint8_t)(hostByteAddr>>16)&0xFF,
//            (uint8_t)(hostByteAddr>>8)&0xFF, (uint8_t)hostByteAddr&0xFF);
    sprintf(message, "If-%d:%d.%d.%d.%d\r\n", IfIdx,
            (uint8_t)(hostByteAddr>>24)%0xFF, (uint8_t)(hostByteAddr>>16)&0xFF,
            (uint8_t)(hostByteAddr>>8)&0xFF, (uint8_t)hostByteAddr&0xFF);
    UART_write(uart, message, strlen(message));
    /* initialize SlNet interface(s) */
    status = ti_net_SlNet_initConfig();
    if (status < 0)
    {
//        Display_printf(display, 0, 0, "Failed to initialize SlNet interface(s)"
//                       "- status (%d)\n", status);

        while (1);
    }

    if (fAdd && createTask) {
        /*
         *  Create the Task that handles incoming UDP packets.
         *  arg0 will be the port that this task listens to.
         */

        /* Set priority and stack size attributes */
        pthread_attr_init(&attrs);
        priParam.sched_priority = 1;

        detachState = PTHREAD_CREATE_DETACHED;
        retc = pthread_attr_setdetachstate(&attrs, detachState);
        if (retc != 0) {
//            Display_printf(display, 0, 0,
//                    "netIPAddrHook: pthread_attr_setdetachstate() failed\n");
            while (1);
        }

        pthread_attr_setschedparam(&attrs, &priParam);

        retc |= pthread_attr_setstacksize(&attrs, UDPHANDLERSTACK);
        if (retc != 0) {
//            Display_printf(display, 0, 0,
//                    "netIPAddrHook: pthread_attr_setstacksize() failed\n");
            while (1);
        }
        retc = pthread_create(&thread, &attrs, ListenFxn/*Keep Listening*/, (void *)&arg0);
                if (retc != 0) {
//                    sprintf(MsgBuffer, "netIPAddrHook: pthread_create() failed");
//                    AddPayload(MsgBuffer);
                    while (1);
        }
        retc = pthread_create(&thread, &attrs, TransmitFxn, (void *)&arg0);
        if (retc != 0) {
//            Display_printf(display, 0, 0,
//                    "netIPAddrHook: pthread_create() failed\n");
            while (1);
        }

        createTask = false;
    }
}

/*
 *  ======== serviceReport ========
 *  NDK service report.  Initially, just reports common system issues.
 */
void serviceReport(uint32_t item, uint32_t status, uint32_t report, void *h)
{
    char message[128];
    size_t buffer_size;
    static char *taskName[] = {"Telnet", "", "NAT", "DHCPS", "DHCPC", "DNS"};
    static char *reportStr[] = {"", "Running", "Updated", "Complete", "Fault"};
    static char *statusStr[] =
        {"Disabled", "Waiting", "IPTerm", "Failed","Enabled"};

//    Display_printf(display, 0, 0, "Service Status: %-9s: %-9s: %-9s: %03d\n",
//            taskName[item - 1], statusStr[status], reportStr[report / 256],
//            report & 0xFF);
    sprintf(message, "-print Service Status: %-9s: %-9s: %-9s: %03d\r\n",
            taskName[item - 1], statusStr[status], reportStr[report / 256],
            report & 0xFF);
    enqueueMessage(message);
    //UART_write(uart, message, strlen(message));
    /* report common system issues */
    if ((item == CFGITEM_SERVICE_DHCPCLIENT) &&
            (status == CIS_SRV_STATUS_ENABLED) &&
            (report & NETTOOLS_STAT_FAULT)) {
//        Display_printf(display, 0, 0,
//                "DHCP Client initialization failed; check your network.\n");
        sprintf(message, "DHCP Client Initialization failed: Check your network\r\n");
        UART_write(uart, message, strlen(message));
        //while (1);
    }
}
