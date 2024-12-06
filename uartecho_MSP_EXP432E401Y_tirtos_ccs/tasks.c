#include "ph001.h"
void *TSKUart0Read(void *arg0) {
    char input_buffer[BUFFER_SIZE] = {0};  // Buffer for storing input
    char input;                           // Character read from UART
    int buffer_index = 0;                 // Current index in input buffer
    //UART_init();
    while (1) {
        // Read a single character from UART
        UART_read(uart, &input, 1);

        // Echo the character back to the terminal for user feedback
        UART_write(uart, &input, 1);

        // Process the input when Enter is pressed
        if (input == '\r' || input == '\n') {
            input_buffer[buffer_index] = '\0';  // Null-terminate the input string
            UART_write(uart, "\r\n", 2);        // Move to the next line

            if (buffer_index > 0) {
                enqueueMessage(input_buffer);  // Send command for processing
            }

            // Reset the buffer for the next command
            buffer_index = 0;
            memset(input_buffer, 0, sizeof(input_buffer));
        }
        // Handle backspace or delete key
        else if (input == '\b' || input == 127) {
            if (buffer_index > 0) {
                buffer_index--;  // Remove the last character
                UART_write(uart, " \b", 2);  // Erase the character on the terminal
            }
        }
        // Store valid characters in the buffer
        else {
            if (buffer_index < BUFFER_SIZE - 1) {
                input_buffer[buffer_index++] = input;
            } else {
                // Handle buffer overflow
                buffer_index = 0;
                UART_write(uart, "\r\nBuffer Overflow. Enter a shorter command.\r\n", 45);
            }
        }
    }
}


void TaskAAA() {
    // GPIO Initialization
    GPIO_init();
    //Board_init();
    UART_init();
    Timer_init();
    SPI_init();
    ADCBuf_init();
    // UART Initialization
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);
    if (uart == NULL) {
        while (1);  // UART0 failed
    }

    uartParams.readEcho = UART_ECHO_OFF;  // UART7 specific configuration
    uart7 = UART_open(CONFIG_UART_1, &uartParams);
    if (uart7 == NULL) {
        while (1);  // UART7 failed
    }

    // Timer Initialization

    // Timer0 Initialization
    Timer_Params_init(&timerParams);
    timerParams.period = 1000000;  // 1 second
    timerParams.periodUnits = Timer_PERIOD_US;
    timerParams.timerCallback = Timer0Callback;
    timerParams.timerMode = Timer_CONTINUOUS_CALLBACK;

    timer0 = Timer_open(CONFIG_TIMER_0, &timerParams);
    if (timer0 == NULL || Timer_start(timer0) == Timer_STATUS_ERROR) {
        UART_write(uart, "ERROR: Timer0 initialization failed.\r\n", 38);
        while (1);
    }

    // Timer1 Initialization
    Timer_Params_init(&timerParams);
    timerParams.period = 10000;  // 10 ms
    timerParams.periodUnits = Timer_PERIOD_US;
    timerParams.timerCallback = Timer1Callback;  // Ensure correct callback
    timerParams.timerMode = Timer_CONTINUOUS_CALLBACK;

    timer1 = Timer_open(CONFIG_TIMER_1, &timerParams);
    if (timer1 == NULL || Timer_start(timer1) == Timer_STATUS_ERROR) {
        UART_write(uart, "ERROR: Timer1 initialization failed.\r\n", 38);
        while (1);
    }


    // SPI Initialization
    SPI_Params_init(&Glo.spiParams);
    Glo.spiParams.dataSize = 16;
    Glo.spiParams.frameFormat = SPI_POL0_PHA1;
    Glo.spi = SPI_open(CONFIG_SPI_0, &Glo.spiParams);
    if (Glo.spi == NULL) {
        while (1);  // SPI initialization failed
    }
    // ADCBuf Initialization

    ADCBuf_Params_init(&Glo.ADCBuf.params);
    Glo.ADCBuf.params.callbackFxn = ADCBufCallback;
    Glo.ADCBuf.params.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
    Glo.ADCBuf.params.recurrenceMode = ADCBuf_RECURRENCE_MODE_CONTINUOUS;
    Glo.ADCBuf.params.samplingFrequency = 8000;  // 8 kHz sampling rate

    Glo.ADCBuf.Handle = ADCBuf_open(CONFIG_ADCBUF_0, &Glo.ADCBuf.params);
    if (Glo.ADCBuf.Handle == NULL) {
        while (1);  // ADCBuf initialization failed
    }

    Glo.ADCBufCtrl.conversion.adcChannel = 0;  // Channel 0
    Glo.ADCBufCtrl.conversion.sampleBuffer = Glo.ADCBufCtrl.RX_Ping;
    Glo.ADCBufCtrl.conversion.sampleBufferTwo = Glo.ADCBufCtrl.RX_Pong;
    Glo.ADCBufCtrl.conversion.samplesRequestedCount = DATABLOCK_SIZE;

    initialize_registers();
    GPIO_enableInt(CONFIG_GPIO_6);
    GPIO_enableInt(CONFIG_GPIO_7);
    char message[64] = "\0";
    sprintf(message, "%s%s\t\t\r\n-----------RESET------------\r\n",Glo.clear,Glo.home);
    UART_write(uart, message, strlen(message));
    //MPabout(NULL);

    Task_exit();  // Terminate TaskAAA
}

void TSKPayload(){
    int32_t payloadnext;
    int32_t payloadindex;
    char *payload;
    uint32_t gateKey;
//    char output[BUFFER_SIZE];

    while(1){
        Semaphore_pend(semaphore0, BIOS_WAIT_FOREVER);
        payloadindex = readIndex;
        payload = payloads[payloadindex];
        process_command(payload);
//        UART_write(uart, output, strlen(output)); // send response
//        UART_write(uart, "\r\n", 2);
//        memset(payload, 0, PAYLOAD_SIZE);
        gateKey = GateSwi_enter(gateSwi0);
        payloadnext = payloadindex + 1;
        if(payloadnext >= PAYLOADCOUNT)
            payloadnext = 0;
        readIndex = payloadnext;
        GateSwi_leave(gateSwi0, gateKey);


    }

}
void TSKUart7Read() {
    char rxbuffer[BUFFER_SIZE] = {0};  // Independent buffer for UART7 reception
    int_fast32_t len;

    for (;;) {
        // Read from UART7 into rxbuffer
        len = UART_read(uart7, rxbuffer, sizeof(rxbuffer));

        // Check if data was read successfully
        if (len > 0 && len < BUFFER_SIZE) {
            // Null-terminate and remove trailing newline if present
            rxbuffer[len - 1] = '\0';

            // Write the received data back to UART7 for debugging
//            UART_write(uart7, "\r\nUART7>> ", 10);  // Fixed length for debugging
//            UART_write(uart7, rxbuffer, strlen(rxbuffer));  // Send received data
//            UART_write(uart7, "\r\n", 2);

            // Enqueue message for processing
            enqueueMessage(rxbuffer);

            // Clear buffer for next read
            memset(rxbuffer, '\0', sizeof(rxbuffer));
        }
    }
}
void ADCStream() {
    uint16_t *source;
    char longload[sizeof(uint16_t)*DATABLOCKSIZE+320];
    int32_t dest_choice;
    int32_t hdrlen;
    bool local = true;

    while(1){
        Semaphore_pend(semaphore1, BIOS_WAIT_FOREVER);

        if(Glo.ADCBufCtrl.RX_Completed == Glo.ADCBufCtrl.RX_Ping){
            source = Glo.ADCBufCtrl.RX_Completed;
            dest_choice = 0;
            Glo.ADCBufCtrl.ping_count++;
        }
        else if(Glo.ADCBufCtrl.RX_Completed == Glo.ADCBufCtrl.RX_Pong){
            source = Glo.ADCBufCtrl.RX_Completed;
            dest_choice = 1;
            Glo.ADCBufCtrl.pong_count++;
        }
        else{
            enqueueMessage("-print RX_Ping and RX_Pong not completed, lost pointer");
            return;
        }
        //Eventually add net packaging here

        if(local == true){
            sprintf(longload, "-voice %d 128  ", dest_choice);
            hdrlen = strlen(longload)+1;
            memcpy(&longload[hdrlen], source, sizeof(uint16_t)*DATABLOCKSIZE);
            VoiceParse(longload);
        }
    }
}

//
