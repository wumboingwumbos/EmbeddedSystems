#ifndef PH001_H_
#define PH001_H_

/* POSIX Header files */
#include <pthread.h>

/*Standard Library Includes */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

/* TI-RTOS Kernel and Driver Includes */
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/gates/GateSwi.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/Board.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>

//#include <ti/Drivers/Timer.h>
#include <ti/sysbios/knl/Swi.h>
#include "ti_drivers_config.h"
#include <ti/drivers/SPI.h>
#include <ti/drivers/ADCBuf.h>
//#include <ti/devices/msp432e4/driverlib/driverlib.h>

// Define valid memory address boundaries (adjust based on your system)
#define VALID_MIN_ADDRESS 0x20000000  // Start valid  region
#define VALID_MAX_ADDRESS 0x2003FFFC  // End valid region
#define VALID_MIN_ADDRESS2 0x00000000 // Start valid  region
#define VALID_MAX_ADDRESS2 0x000FFFFC // End valid region
#define GPIOCOUNT 8
#define SINE_TABLE_SIZE 256
#define DATABLOCK_SIZE 256
#define AUDIO_SAMPLE_PERIOD_US 125  // 125 µs for 8 kHz
//ADDED IN 10/3/24 for HW4
//#define BUFFER_SIZE 320
#define AUDIO_BUFFER_SIZE 256
#define QUEUE_SIZE 10
#define PAYLOAD_SIZE 256
#define CBC 3
#define PAYLOADCOUNT 32
#define MSGLENGTH 320
#define MSGCOUNT 8
#define CALLBACKCOUNT 3
#define MAX_CALLBACKS 3
#define LUTSIZE 256
#define DATABLOCKSIZE 128
extern GateSwi_Handle gateSwi0, gateSwi1, gateSwi2, gateSwi3;

#define NetQueueLen 32
#define NetQueueSize 320
#define REG_DIAL1 0
#define REG_DIAL2 1
#define DEFAULTPORT 1000

extern Semaphore_Handle semaphore0;
extern Semaphore_Handle semaphore1;
extern Semaphore_Handle semaphore2;
extern Task_Handle task0; //UART_Thread
extern Task_Handle task1; //Payload Thread
extern Task_Handle task2; //Task AAA
extern Task_Handle task3; //UART Thread
extern Task_Handle task4; //ADC Stream
extern Swi_Handle swi0;
extern Swi_Handle swi1;
extern Swi_Handle swi2;
extern Swi_Handle swi3;
extern Swi_Handle swi4;

extern int writeIndex;
extern int readIndex;
extern int errorCount;
extern char payloads[PAYLOADCOUNT][PAYLOAD_SIZE];

UART_Handle uart;
UART_Handle uart7;
UART_Params uartParams;
Timer_Handle timer0;
Timer_Handle timer1;
Timer_Params timerParams;
typedef struct Bios{
//    Semaphore_Handle QueueSemaphore;
    Semaphore_Handle ADCSemaphore;
//    GateSwi_Handle   PayloadGate;
//    GateSwi_Handle   CallbackGate;
//    Task_Handle      TSKUart0Read;
//    Task_Handle      TSKPayload;
//    Task_Handle      TSKUart7Read;
//    Task_Handle      TaskAAA;
//    Task_Handle      ADCStream;
//    Swi_Handle       SWI_Timer0;
//    Swi_Handle       SWI_Left;
//    Swi_Handle       SWI_Right;
//    Swi_Handle       SWI_Timer1;
//    Swi_Handle       ADC_Swi;
} Bios;
typedef struct ADCBuf{
    ADCBuf_Params params;
    ADCBuf_Handle Handle;
} ADCBuf;
typedef struct ADCBufControl{
    ADCBuf_Conversion conversion;
    uint16_t *RX_Completed;
    uint32_t converting;
    uint32_t ping_count;
    uint32_t pong_count;
    uint16_t RX_Ping[DATABLOCKSIZE];
    uint16_t RX_Pong[DATABLOCKSIZE];
} ADCBufControl;
typedef struct TXBufControl{
    uint16_t *TX_Completed;
    int32_t  TX_index;
    int32_t  TX_correction;
    uint32_t TX_delay;
    uint32_t TX_sample_count;
    uint16_t TX_Ping[DATABLOCKSIZE];
    uint16_t TX_Pong[DATABLOCKSIZE];
} TXBufControl;
typedef struct NetOutQ{
    int32_t payloadWriting, payloadReading;
    char    payloads[NetQueueLen][NetQueueSize];
    int32_t binaryCount[320];
} NetOutQ;
typedef struct Discoveries{
    uint32_t IP_address;
} Discoveries;
#define reg_count 32

typedef struct{
    int32_t value;
} Register;

/* Structure for Globals */
typedef struct {
    //about glos
    const char *author;
    const char *partners;
    const char *assignment;
    const char *ver;
    //constant glos
    const char *clear;
    const char *home;
    //Error globals (as int's)
    int uce;
    int bfe;
    int me;
    int ide;
    int qoe;
    int re;
    int Timer0Period;
    //other glos
    int incrementLookupTable;
    double position;
    double sinelookUpTable;
    int freq;
    SPI_Handle      spi;
    SPI_Params      spiParams;
    SPI_Transaction spiTransaction;
    //audio
    ADCBufControl ADCBufCtrl;     // Control structure for ADC buffers
    uint16_t pingBuffer[DATABLOCK_SIZE];  // Ping buffer
    uint16_t pongBuffer[DATABLOCK_SIZE];  // Pong buffer
    ADCBuf        ADCBuf;
    TXBufControl  TXBufCtrl[2];
    Bios Bios;
    NetOutQ       NetOutQ;
    Discoveries   Discoveries[32];
    uint32_t      Multicast;
    int32_t       GlobTail;
    Register regs[reg_count];  // 32 signed registers
} Globals;
typedef struct{
    int count;
    char cbpayload[320];
}Callback;



Callback callbacks[MAX_CALLBACKS];
#define BUFFER_SIZE 1024
#define ticker_count 16

typedef struct{
    int delay;
    int period;
    int count;
    char payload[320];
}Ticker;
Ticker tickers[ticker_count];

#define script_count 64

typedef struct{
    char scrPayload[640];
}Script;
Script scripts[script_count];

// Defined in main else other files have access to Globals as variables
#ifndef MAIN
extern
#endif
Globals Glo;

//Function Prototypes:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void process_command(const char* command); //
bool matchsub(const char* command, const char* input);
void *getnextstring(char* input, bool Whites);
void MPhelp(char* input);
void MPabout(char* input);
//void print_function(const char* input);
void MPtimer(char *input);
void MPCallback(char *input);
void MPmemr(char* input);
void MPGPIO(char* input);
void Error(char* input);
                            //Timer and Callback fns
void enqueueMessage(const char* payload);
void SWI_Timer0();
void Timer0Callback();
void CallbackSW1();
void CallbackSW2();
void SWI_Left();
void SWI_Right();
//void ADC_Swi();
int32_t AddCallBack(int32_t cbindex, int32_t count, char* payload);
void TIMER0SWI();
void SWIbuttonSW1();
void SWIbuttonSW2();
                             //Ticker Addition fns
void MPTicker(char *input);
int create_ticker(int index, int delay, int period, int count, char *payload);
void TIMER1SWI();
void SWI_Timer1();
void Timer1Callback();
                           //Registers Fns
void MPReg(char *input);
void initialize_registers();

void MPScript(char *input);
int create_script(int index, char *scrPayload);

void MPIf(char *input);

void MPUart7(char *input);

void MPSin(char *input);

void MPstream(char *input);
void MPaudio();
void ADCBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion, void *buffer, uint32_t channel, int_fast16_t status); // ADCBuf callback
void AudioTask();
void VoiceParse(char *ch);
void ParseNetUDP(char *ch, int32_t binaryCount);
#endif  // End of PH001_H_
