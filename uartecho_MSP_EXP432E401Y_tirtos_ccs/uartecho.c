///*
// *  ======== uartecho.c ========
// */
//#include <stdint.h>
//#include <stddef.h>
//#include <string.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <stdlib.h>
//#include <ctype.h>
//
///* Driver Header files */
//#include <ti/drivers/GPIO.h>
//#include <ti/drivers/UART.h>
////
///* Driver configuration */
//#include "ti_drivers_config.h"
//#include <ti/drivers/Board.h>
///* Header file */
//#include "ph001.h"
//extern Globals Glo;
////
////int writeIndex = 0;
////int readIndex = 0;
////char payloads[PAYLOADCOUNT][PAYLOAD_SIZE];
////bool matchsub(const char* command, const char* input){
////    size_t c_l = strlen(command);
////    return strncmp(command, input, c_l) == 0;
////}
////
////void *getnextstring(char* input, bool Whites){
////    char *loc;
////    if(input == NULL)
////        return NULL;
////    loc = strchr(input, ' ');
////    if(!loc)
////        return NULL;
////    if(Whites)
////        while(*loc == ' ')
////            loc++;
////    else
////        if(*loc == ' ')
////            loc++;
////
////    if(!*loc)
////        return NULL;
////
////    return loc;
////}
////
////void enqueueMessage(const char* payload){
////    int32_t payloadnext;
////    int32_t qindex;
////    uint32_t gateKey;
////
////    if(!payload || payload[0] == 0)
////        return;
////
////    gateKey = GateSwi_enter(gateSwi0);
////    qindex = writeIndex;
////    payloadnext = qindex + 1;
////    if(payloadnext >= PAYLOADCOUNT)
////        payloadnext = 0;
////
////    if(payloadnext == readIndex){
////        GateSwi_leave(gateSwi0, gateKey);
////        Glo.qoe++;
////        return;
////    }else{
////        strcpy(payloads[qindex], payload);
////        writeIndex = payloadnext;
////    }
////
////    GateSwi_leave(gateSwi0, gateKey);
////    Semaphore_post(semaphore0);
////    return;
////
////}
