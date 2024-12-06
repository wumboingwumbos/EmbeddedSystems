//#include "ph001.h"
//
//#define EXT_BUFFER_SIZE 256
//#define AUDIO_SAMPLE_PERIOD_US 125  // 125 µs for 8 kHz sampling
//
//// Ping-Pong Buffers
//static uint16_t pingBuffer[BUFFER_SIZE];
//static uint16_t pongBuffer[BUFFER_SIZE];
//
//// ADCBuf Handle and Conversion
//static bool usePingBuffer = true;  // Tracks active buffer
//
//void MPaudio(char *input) {
//    if (!Glo.streamActive) {
//        UART_write(uart, "ERROR: Stream not active. Use -stream 1/2.\r\n", 45);
//        return;
//    }
//
//    SPI_Transaction spiTransaction;
//    bool transferOK;
//    uint16_t outputValue;
//
//    // Retrieve data from the active buffer for playback
//    if (Glo.usePingBuffer) {
//        outputValue = Glo.pingBuffer[Glo.bufferIndex];
//    } else {
//        outputValue = Glo.pongBuffer[Glo.bufferIndex];
//    }
//
//    // Send the sample to the DAC
//    spiTransaction.count = 2;  // 2 bytes for 16-bit DAC
//    spiTransaction.txBuf = &outputValue;
//    spiTransaction.rxBuf = NULL;
//
//    transferOK = SPI_transfer(Glo.spi, &spiTransaction);
//    if (!transferOK) {
//        UART_write(uart, "ERROR: SPI transfer failed\r\n", 28);
//        //GPIO_write(GPIO_AUDIO_AMP, 0);  // Turn off amp on failure
//        return;
//    }
//
//    // Increment buffer index and switch buffers if necessary
//    Glo.bufferIndex++;
//    if (Glo.bufferIndex >= BUFFER_SIZE) {
//        Glo.bufferIndex = 0;
//        Glo.usePingBuffer = !Glo.usePingBuffer;  // Toggle between ping and pong
//    }
//}
//
//void MPstream(char *input) {
//    char *loc = getnextstring(input, true);
//    int mode = 0;
//
//    if (loc && sscanf(loc, "%d", &mode) == 1) {
//        switch (mode) {
//        case 0:  // Turn OFF ADC stream
//            if (adcBufHandle) {
//                ADCBuf_convertCancel(adcBufHandle);  // Stop ADC conversions
//                ADCBuf_close(adcBufHandle);         // Close ADCBuf handle
//                adcBufHandle = NULL;
//            }
//            GPIO_write(CONFIG_GPIO_4, 0);  // Turn OFF audio amp
//            GPIO_write(CONFIG_GPIO_5, 0);    // Turn OFF microphone amp
//            break;
//
//        case 1:  // Prepare ADC stream
//            // Configure GPIO
//            GPIO_write(CONFIG_GPIO_4, 1);  // Turn ON audio amp
//            GPIO_write(CONFIG_GPIO_5, 1);    // Turn ON microphone amp
//
//            // Open ADCBuf
//            adcBufHandle = ADCBuf_open(CONFIG_ADCBUF_0, NULL);  // CONFIG_ADCBUF_0 from SysConfig
//            if (!adcBufHandle) {
//                UART_write(uart, "ERROR: Failed to open ADCBuf\r\n", 31);
//                return;
//            }
//
//            // Set up ADC conversion
//            adcConversion.arg = NULL;
//            adcConversion.adcChannel = ADCBUF_CHANNEL_0;  // Use microphone channel
//            adcConversion.sampleBuffer = pingBuffer;
//            adcConversion.sampleBufferTwo = pongBuffer;
//            adcConversion.samplesRequestedCount = BUFFER_SIZE;
//            break;
//
//        case 2:  // Start ADC streaming
//            if (adcBufHandle) {
//                int_fast16_t result = ADCBuf_convert(adcBufHandle, &adcConversion, 1);
//                if (result != ADCBuf_STATUS_SUCCESS) {
//                    UART_write(uart, "ERROR: Failed to start ADC conversion\r\n", 39);
//                }
//            }
//            break;
//
//        default:
//            UART_write(uart, "ERROR: Invalid mode\r\n", 22);
//        }
//    } else {
//        UART_write(uart, "ERROR: Invalid input\r\n", 22);
//    }
//}
//
//void ADCBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion, void *completedBuffer, uint_fast16_t completedChannel) {
//    uint16_t *buffer = (uint16_t *)completedBuffer;
//    int i;
//    // Process the completed buffer
//    for (i = 0; i < EXT_BUFFER_SIZE; i++) {
//        uint16_t outputValue = buffer[i];
//
//        // Send the sample to the DAC
//        SPI_Transaction spiTransaction = {0};
//        spiTransaction.count = 2; // 2 bytes for 16-bit DAC
//        spiTransaction.txBuf = &outputValue;
//        spiTransaction.rxBuf = NULL;
//
//        if (!SPI_transfer(Glo.spi, &spiTransaction)) {
//            UART_write(uart, "ERROR: SPI transfer failed\r\n", 28);
//            GPIO_write(CONFIG_GPIO_4, 0);  // Turn off amp on failure
//            return;
//        }
//    }
//
//    // ADCBuf will automatically alternate between ping and pong buffers
//}
//
//#include <ti/drivers/ADCBuf.h>
//
//void setupADCBuf() {
//    ADCBuf_Params adcBufParams;
//    ADCBuf_Conversion adcConversion;
//
//    // Initialize ADCBuf driver
//    ADCBuf_init();
//
//    // Initialize ADCBuf parameters
//    ADCBuf_Params_init(&adcBufParams);
//    adcBufParams.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
//    adcBufParams.recurrenceMode = ADCBuf_RECURRENCE_MODE_CONTINUOUS;
//    adcBufParams.samplingFrequency = 8000; // 8 kHz for audio
//    adcBufParams.callbackFxn = ADCBufCallback;
//
//    // Open ADCBuf with the specified parameters
//    adcBufHandle = ADCBuf_open(CONFIG_ADCBUF_0, &adcBufParams);
//    if (!adcBufHandle) {
//        UART_write(uart, "ERROR: Failed to open ADCBuf\r\n", 31);
//        return;
//    }
//
//    // Configure ADCBuf conversion
//    adcConversion.adcChannel = ADCBUF_CHANNEL_0;
//    adcConversion.sampleBuffer = pingBuffer;
//    adcConversion.sampleBufferTwo = pongBuffer;
//    adcConversion.samplesRequestedCount = BUFFER_SIZE;
//
//    // Start ADCBuf conversion
//    if (ADCBuf_convert(adcBufHandle, &adcConversion, 1) != ADCBuf_STATUS_SUCCESS) {
//        UART_write(uart, "ERROR: Failed to start ADCBuf conversion\r\n", 43);
//    }
//}
//
