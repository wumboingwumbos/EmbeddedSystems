//#include "ph001.h"
//
//void MPUart7(char *input) {
//    char *loc;
//    char xmitbuf[MSGLENGTH] = {0};  // Independent buffer for UART7 transmission
//
//    // Extract the command from input specific to UART7
//    loc = getnextstring(input, true);
//
////    if (loc) {
//        // Format the message for UART7 transmission with newlines
//        snprintf(xmitbuf, sizeof(xmitbuf), "\r\n%s\r\n", loc);
//
//        // Send the message through UART7
//        UART_write(uart7, xmitbuf, strlen(xmitbuf));
//
//        // Debugging output if UART7 write fails
////        if (status < 0) {
////            snprintf(buffer, buffer_size, "ERROR: UART7 write failed.\r\n");
////            UART_write(uart, buffer, strlen(buffer));  // Send error to primary UART
////        }
////    }
//}
