//#include "ph001.h"
//
//void MPStream(char *input) {
//    char *loc = getnextstring(input, true);
//    int mode = 0;
//
//    if (loc && sscanf(loc, "%d", &mode) == 1) {
//        switch (mode) {
//        case 0:  // Turn OFF ADC stream
//            Glo.streamActive = false;
//            ADC_disable();  // Disable ADC
////            GPIO_write(GPIO_AUDIO_AMP, 0);  // Turn OFF audio amp
////            GPIO_write(GPIO_, 0);    // Turn OFF microphone amp
//            break;
//
//        case 1:  // Prepare ADC stream
//            ADC_init();  // Initialize ADC
////            GPIO_write(GPIO_AUDIO_AMP, 1);  // Turn ON audio amp
////            GPIO_write(GPIO_MIC_AMP, 1);    // Turn ON microphone amp
//            Glo.streamActive = false;
//            break;
//
//        case 2:  // Start ADC streaming
//            if (!Glo.streamActive) {
//                Glo.streamActive = true;
//                Glo.bufferIndex = 0;
//                ADC_enable();  // Start ADC conversions
//            }
//            break;
//
//        default:
//            UART_write(uart, "ERROR: Invalid mode\r\n", 21);
//        }
//    } else {
//        UART_write(uart, "ERROR: Invalid input\r\n", 22);
//    }
//}
