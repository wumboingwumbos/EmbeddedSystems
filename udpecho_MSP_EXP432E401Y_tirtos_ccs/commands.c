#include "ph001.h"
char message[128] = "\0";
#define MESSAGE_SIZE 128
void process_command(const char* input) {
    // Comparing inputs to list of commands

    if (matchsub("-about", input) == true) {
        MPabout(input);
    }
    else if (matchsub("-clear", input) == true) {
        sprintf(message, "%s%s\r\n-----------Screen Cleared------------\r\n",Glo.clear,Glo.home);
        UART_write(uart, message, strlen(message));
    }
    else if (matchsub("-help", input) == true) {
        MPhelp(input);
    }
    else if (matchsub("-print ", input) == true){
        snprintf(message, MESSAGE_SIZE, input);
        UART_write(uart, message+7, strlen(message)-7);
        UART_write(uart, "\r\n", 2);

    }
    else if (matchsub("-print", input) == true){
        sprintf(message, "Error: Put space then characters after '-print' ");
        UART_write(uart, message, strlen(message));
        UART_write(uart, "\r\n", 2);
    }
    else if (matchsub("-memr", input) == true) {
        MPmemr(input);  // Added -memr command handler
    }
    else if (matchsub("-gpio", input) == true){
        MPGPIO(input);
    }
    else if (matchsub("-error", input) == true){
        Error(input);
    }
    else if (matchsub("-timer", input) == true){
        MPtimer(input);
    }
    else if (matchsub("-reg", input) == true){
        MPReg(input);
    }
    else if (matchsub("-callback", input) == true){
        MPCallback(input);
    }
    else if (matchsub("-script",input)==true){
        MPScript(input);
    }else if (matchsub("-ticker", input) == true){
        MPTicker(input);
    }else if (matchsub("-rem",input)==true){
        //nothing
    }else if (matchsub("-if", input) == true){
        MPIf(input);
    }else if (matchsub("-uart", input) == true){
        MPUart7(input);
    }else if (matchsub("-sine", input) == true){
        MPSin(input);
    }else if (matchsub("-stream", input) == true){
        MPstream(input);
    }else if (matchsub("-audio",input)==true){
        MPaudio(input);
    }else if (matchsub("-netudp",input)==true){
        MPNetUDP(input,0);
    }
    else if (matchsub("-dial", input) == true) {
        MPDial(input);
    }
    else if (matchsub("-voice", input) == true) {
        // Do nothing at all
    }

    // Error if doesn't match commands
    else {
        snprintf(message, MESSAGE_SIZE, "Error Unknown Command:%s", input);
        UART_write(uart, message, strlen(message));
        Glo.uce = Glo.uce + 1;
    }
}
