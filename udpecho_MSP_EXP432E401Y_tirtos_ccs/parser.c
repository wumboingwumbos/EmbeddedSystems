#include "ph001.h"


//;;;;;;;;;;;;;;;;;;;;;;;;MP Functions;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

const uint16_t SINETABLE[SINE_TABLE_SIZE+1] = {
 8192,
 8393,
 8594,
 8795,
 8995,
 9195,
 9394,
 9593,
 9790,
 9987,
 10182,
 10377,
 10570,
 10762,
 10952,
 11140,
11327,
11512,
11695,
11875,
12054,
12230,
12404,
12575,
12743,
12909,
13072,
13232,
13389,
13543,
13693,
13841,
13985,
14125,
14262,
14395,
14525,
14650,
14772,
14890,
15003,
15113,
15219,
15320,
15417,
15509,
15597,
15681,
15760,
15835,
15905,
15971,
16031,
16087,
16138,
16185,
16227,
16263,
16295,
16322,
16345,
16362,
16374,
16382,
16383,
16382,
16374,
16362,
16345,
16322,
16295,
16263,
16227,
16185,
16138,
16087,
16031,
15971,
15905,
15835,
15760,
15681,
15597,
15509,
15417,
15320,
15219,
15113,
15003,
14890,
14772,
14650,
14525,
14395,
14262,
14125,
13985,
13841,
13693,
13543,
13389,
13232,
13072,
12909,
12743,
12575,
12404,
12230,
12054,
11875,
11695,
11512,
11327,
11140,
10952,
10762,
10570,
10377,
10182,
 9987,
 9790,
 9593,
 9394,
 9195,
 8995,
 8795,
 8594,
 8393,
 8192,
 7991,
 7790,
 7589,
 7389,
 7189,
 6990,
 6791,
 6594,
 6397,
 6202,
 6007,
 5814,
 5622,
 5432,
 5244,
 5057,
 4872,
 4689,
 4509,
 4330,
 4154,
 3980,
 3809,
 3641,
 3475,
 3312,
 3152,
 2995,
 2841,
 2691,
 2543,
 2399,
 2259,
 2122,
 1989,
 1859,
 1734,
 1612,
 1494,
 1381,
 1271,
 1165,
 1064,
  967,
  875,
  787,
  703,
  624,
  549,
  479,
  413,
  353
,  297,
  246,
  199,
  157,
  121,
   89,
   62,
   39,
   22,
   10,
    2,
    0,
    2,
   10,
   22,
   39,
   62,
   89,
 121,
  157,
  199,
  246,
  297,
  353,
  413,
  479,
  549,
  624,
  703,
  787,
  875,
  967,
 1064,
 1165,
 1271,
 1381,
 1494,
 1612,
 1734,
 1859,
 1989,
 2122,
 2259,

 2399,
 2543,
 2691,
 2841,
 2995,
 3152,
 3312,
 3475,
 3641,
 3809,
 3980,
 4154,
 4330,
 4509,
 4689,
 4872,
 5057,
 5244,
 5432,
 5622,
 5814,
 6007,
 6202,
 6397,
 6594,
 6791,
 6990,
 7189,
 7389,
 7589,
 7790,
 7991,
 8192};

void MPhelp(char* input){
    char buffer[BUFFER_SIZE];
    size_t buffer_size;
    char *loc;
    loc = getnextstring(input, true);

    if(!loc){
        sprintf(buffer, "\tList of All Commands | -help \"command\" for more details.\r\n-about\r\n-callback\r\n-error"
                "\r\n-gpio\r\n-help\r\n-memr\r\n-if\r\n-print\r\n-reg\r\n-script\r\n-ticker\r\n-timer\r\n-stream\r\n"
                "-dial\r\n-netudp\r\n-sine\r\n");
        UART_write(uart, buffer, strlen(buffer));
        return;
    }else if(strstr(loc, "help") != NULL){
        sprintf(buffer, "-help: Lists all commands\r\n");
        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "about") != NULL){
        sprintf(buffer, "-about: Shows the author, partners, assignment, compile timestamp \r\n");
        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "memr") != NULL){
        sprintf(buffer, "-memr: Prints contents of valid memory spaces:\r\n [0x20000000-0x2003FFFC] "
                "and [0x00000000-0x000FFFFC]to console \r\n");
        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "print") != NULL){
        sprintf(buffer, "-print: Prints characters to console \r\n");

        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "error") != NULL){
        sprintf(buffer, "-error: shows the counts of each error \r\nerror c: clears error count \r\n");

        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "gpio") != NULL){
        sprintf(buffer, "\tgpio command can read, write, and toggle gpios (0-7) \r\n"
                 "-gpio\t\t | lists all gpio states\t0-3: LED | 6&7: SW1&SW2\r\n"
                 "-gpio X Y\t | X: operation (r,w,t), Y: input (0,1)");

        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "callback") != NULL){
        sprintf(buffer, "-callback\t| shows callback entries\r\n"
                     "-callback X Y Z\t| X: timer(0) | SW1(1) | SW2(2), Y: count, Z: command\r\n"
                     "-callback c\t| clears all callbacks");
        UART_write(uart, buffer, strlen(buffer));
    }else if (strstr(loc, "reg") !=NULL){
        sprintf(buffer, "\t\t| allows for operations with registers|\r\n"
                "-reg (DST) (OP) (SA SB)\r\n"
                "    DST:    destination register (0-31)\r\n"
                "    OP:     Operation (list below)\r\n"
                "    SA/SB:  Source register OR immediate value (# or #x)\r\n"
                "             also direct | indirect memory addressing\r\n"
                "    @0x(0, 4, C, etc...) | R(1, 2, etc...)\r\n"
                "Operations: \tMOVe XChanGe\r\n"
                "\t\tINCrement DECrement\r\n"
                "\t\tADD SUBtract NEGative\r\n"
                "\t\tNOT AND IOR XOR\r\n"
                "\t\tMULtiply DIVision REMainder\r\n"
                "\t\tMINimum MAXimum\r\n");
        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "script") != NULL){
        sprintf(buffer, "-script\t\t| Used to sequence commands\r\n"
                "-script\t\t shows scripts\r\n"
                "-script 3  \t show script at line #3 (0 to 63)\r\n"
                "-script 17 w -gpio 0 t \t loads payloa to script #17\r\n"
                "-script 17 x \t runs script sequence starting at #17\r\n");

        UART_write(uart, buffer, strlen(buffer));
    }else if (strstr(loc, "sine") != NULL){
        sprintf(buffer, "-sine\t\t\t| Produces a frequency through the Audio Board\r\n"
                "-sine (0<freq<3999)\t | produces a sound of (freq) Mhz");
        UART_write(uart,buffer, strlen(buffer));
    }else if(strstr(loc, "timer") != NULL){
        sprintf(buffer, "-timer\t\t| print current timer period (us) for CALLBACK0\r\n"
                     "-timer X\t| sets timer period to X (us) for CALLBACK 0\r\n"
                     "-timer 0\t| turn off timer for CALLBACK 0\r\n");
        UART_write(uart, buffer, strlen(buffer));
    }else if(strstr(loc, "ticker")!=NULL){
        sprintf(buffer, "-ticker\t\t| Used to schedule commands\r\n"
                "-ticker (index) (delay) (period) (count) (payload)"
                 "\tIndex Range: 0-15\r\n\t\t");
        UART_write(uart, buffer, strlen(buffer));

    }else if(strstr(loc, "stream")!=NULL){
            sprintf(buffer, "-stream\t| Toggle ADC stream for ADCBUF_CHANNEL_0\r\n"
                    "\t\t| -audio will use BOOSTXL-AUDIO mike tied to internal ADC\r\n"
                    "\t\t| -audio will use DAC of BOOSTXL -AUDIO\r\n"
                    "-stream 0\t| turn OFF ADC stream for ADCBUF_CHANNEL_0\r\n"
                    "-stream 1\t| prepare ADC stream for ADCBUT_CHANNEL_0\r\n"
                    "\t\t| -stream 1 will use a ticker to get delayed -stream 2\r\n");
                    //"-stream 2\t| turn ON ADC stream conversations for ADCBUF_CHANNEL_0\r\n");
            UART_write(uart, buffer, strlen(buffer));

    }else if(strstr(loc, "if")!=NULL){
            sprintf(buffer, "-ticker\t\t| Conditional Statements\r\n"
                    "-if (A COND B) ? DESTT : DESTF");
            UART_write(uart, buffer, strlen(buffer));
    }else{
        sprintf(buffer, "Not something I can help with!");
        UART_write(uart, buffer, strlen(buffer));
        Glo.uce = Glo.uce + 1;
    }
}
void MPabout(char* input){
    char buffer[BUFFER_SIZE];
    //size_t buffer_size;
    char *loc;
    loc = getnextstring(input, true);

    if(!loc){
        sprintf(buffer, "Author: %s,\r\n"
                        "Partners: %s\r\n"
                        "Assignment: %s\r\n"
                        "Version: %s "
                        "Compile Date/Time: %s %s\r\n", Glo.author, Glo.partners, Glo.assignment, Glo.ver, __DATE__, __TIME__);
        UART_write(uart, buffer, strlen(buffer));
    }else{
        sprintf(buffer, "Error: Need something after '-about' ");
        Glo.uce = Glo.uce + 1;
        UART_write(uart, buffer, strlen(buffer));
    }
}

//void print_function(char* buffer, size_t buffer_size, const char* input){
//    sprintf(buffer, "%s", input + 7);
//}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void MPtimer(char *input) {
    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
    int32_t ftn, val;
    char *loc;


    loc = getnextstring(input, true);
    if(!loc){
        sprintf(buffer, "Timer Period: %u \r\n", Glo.Timer0Period);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    val = atoi(loc);
    //sprintf(buffer, "%u is p", val);
    //UART_write(uart, buffer, strlen(buffer));
    if (val<0){
        sprintf(buffer, "ERROR: Value cant be negative");
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    loc = getnextstring(input, true);
    if(!loc){
        ftn =Timer_PERIOD_US;
    }else{
        ftn = atoi(loc);
    }
    if(ftn<Timer_PERIOD_US || ftn>Timer_PERIOD_COUNTS){
        ftn = Timer_PERIOD_US;

    }

    if(ftn == Timer_PERIOD_US &&  val < 100 && val > 0){
        sprintf(buffer, "ERROR: Value too small %u", val);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    Glo.Timer0Period = val;
   if(Timer_setPeriod(timer0, (Timer_PeriodUnits) ftn, Glo.Timer0Period) == Timer_STATUS_ERROR){
       sprintf(buffer, "ERROR: set period falied");
       UART_write(uart, buffer, strlen(buffer));
       return;
   }
    sprintf(buffer, "Timer Period: %u \r\n", val);
    UART_write(uart, buffer, strlen(buffer));
}
void MPmemr(char* input) {
    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
    char *loc;
    uintptr_t address;
    uint32_t *mem;
    // Find the address after the command "-memr"
    loc = getnextstring(input, true);
    if (!loc) {
        sprintf(buffer, "Error: No address provided");
        UART_write(uart, buffer, strlen(buffer));
        Glo.me = Glo.me + 1;
        return;
    }
    // Convert the address from string to a valid memory address
    address = strtol(loc, NULL, 16);
    // Check if the address conversion was successful
    if (address == 0 && loc[0] != '0') {
        sprintf(buffer, "Error: Invalid address format");
        UART_write(uart, buffer, strlen(buffer));
        Glo.me = Glo.me + 1;
        return;
    }
    // Check if the address is within a valid range
    if (address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) {
        // Cast the address to a pointer and read the memory content
         mem = (uint32_t *)address;
         // Output the content at the memory location
         sprintf(buffer, "Memory at 0x%08lx: 0x%08x", (unsigned long)address, *mem);
         UART_write(uart, buffer, strlen(buffer));
        return;
    }
        // Check if the address is within a valid range
    else if (address >= VALID_MIN_ADDRESS2 && address <= VALID_MAX_ADDRESS2) {
        // Cast the address to a pointer and read the memory content
         mem = (uint32_t *)address;
         // Output
         sprintf(buffer, "Memory at 0x%08lx: 0x%08x", (unsigned long)address, *mem);
         UART_write(uart, buffer, strlen(buffer));
         return;

    } else{
        sprintf(buffer, "Error: Address out of range");
        UART_write(uart, buffer, strlen(buffer));
        Glo.me = Glo.me + 1;
    }

}
// START GPIO HERE

void MPGPIO(char* input) {
    char buffer[BUFFER_SIZE];
    size_t buffer_size;
    int index;
    char *loc;
    char *operation = NULL;
    int value;
    int i;
    size_t current_length = 0;  // To keep track of the buffer length

    // Get the next part of the input after the "-gpio" command
    loc = getnextstring(input, true);

    // If no specific GPIO number is provided, set index to -1 to print all GPIOs
    if (!loc) {
        sprintf(buffer, "Error: No GPIO number provided");
        UART_write(uart, buffer, strlen(buffer));
        current_length = sprintf(buffer, "Listing all GPIO states:\r\n");

                for (i = 0; i < GPIOCOUNT; i++) {
                    int gpio_value = GPIO_read(i);
                    current_length += snprintf(buffer + current_length, buffer_size - current_length, "GPIO %d: %d\r\n", i, gpio_value);

                    // Break if the buffer is full
                    if (current_length >= buffer_size) {
                        snprintf(buffer + current_length - 1, buffer_size - current_length, "\r\n[Buffer full]\r\n");
                        UART_write(uart, buffer, strlen(buffer));
                        break;
                    }
                }
        return;
    }

    // Check if the input contains a valid digit for GPIO index
    if (isdigit(*loc) == 0) {
        sprintf(buffer, "Error: '%s' is not a valid GPIO number", loc);
        UART_write(uart, buffer, strlen(buffer));
        Glo.ide = Glo.ide + 1;
        return;
    }
    index = atoi(loc); // Convert input string to an integer

    // Get the operation (if provided)
    loc = getnextstring(loc, true);
    if (loc) {
        operation = loc;
    }

    // Perform the operation based on the command provided
    if (!operation || strncmp(operation, "r", 1) == 0) {  // Default to read if no operation is specified
        if (index < 0 || index >= GPIOCOUNT) {
            sprintf(buffer, "Error: GPIO index %d out of range", index);
            UART_write(uart, buffer, strlen(buffer));
            Glo.ide = Glo.ide + 1;
            return;
        }
        int gpio_value = GPIO_read(index);
        sprintf(buffer, "GPIO %d: %d\r\n", index, gpio_value);
        UART_write(uart, buffer, strlen(buffer));
    } else if (strncmp(operation, "w", 1) == 0) {
        // Skip the space to get the value to write
        loc = getnextstring(operation, true);
        if (!loc) {
            sprintf(buffer, "Error: No value provided to write");
            UART_write(uart, buffer, strlen(buffer));
            Glo.uce = Glo.uce + 1;
            return;
        }
        value = atoi(loc); // Convert input string to an integer
        if (index < 0 || index >= GPIOCOUNT) {
            sprintf(buffer, "Error: GPIO %d does not exist", index);
            UART_write(uart, buffer, strlen(buffer));
            Glo.ide = Glo.ide + 1;
            return;
        }
        GPIO_write(index, value);
//        sprintf(buffer, "", index, GPIO_read(index));
//        UART_write(uart, buffer, buffer_size);
    } else if (strncmp(operation, "t", 1) == 0) {
        if (index < 0 || index >= GPIOCOUNT) {
            sprintf(buffer, "Error: GPIO index %d out of range", index);
            UART_write(uart, buffer, strlen(buffer));
            Glo.ide = Glo.ide + 1;
            return;
        }
        GPIO_toggle(index);
        int gpio_value = GPIO_read(index); //ADDED IN
        sprintf(buffer, "", index, gpio_value);
        UART_write(uart, buffer, strlen(buffer));
    } else {
        sprintf(buffer, "Error: Invalid operation '%s'", operation);
        UART_write(uart, buffer, strlen(buffer));
        Glo.uce = Glo.uce + 1;
    }
}
//END GPIO HERE

void MPCallback(char *input){
    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
    char payload[BUFFER_SIZE];
    int32_t i, cbindex = -1, count=0;
    char *loc;

    loc = getnextstring(input, true);
    if(!loc)
        cbindex = -1;
    else if(loc[0] == 'c'){
            sprintf(buffer, "Callbacks Cleared\r\n");
            UART_write(uart, buffer, strlen(buffer));
            for(i = 0; i < 3; i++){
                AddCallBack(i, 0, "");
                sprintf(buffer, "Callback: %d\t C:%d\t Function:\r\n",
                         i, callbacks[i].count);
                UART_write(uart, buffer, strlen(buffer));
            }
            sprintf(buffer, "\r\n");
            return;
    }
    else{

        if(isdigit(*loc) == 0 & *loc != '-'){
            sprintf(buffer, "Invalid Command: try -callback (source) (count) (command)");
            //Add error
            UART_write(uart, buffer, strlen(buffer));
            return;
        }
        cbindex = atoi(loc);
    }
    if(cbindex < 0 || cbindex >= CALLBACKCOUNT){
        // print call back info with all callbacks
        sprintf(buffer, "Callback  0   C:%d    Function:%s \r\n"
                                      "Callback  1   C:%d    Function:%s \r\n"
                                      "Callback  2   C:%d    Function:%s \r\n",
                                      callbacks[0].count,callbacks[0].cbpayload,
                                      callbacks[1].count,callbacks[1].cbpayload,
                                      callbacks[2].count,callbacks[2].cbpayload);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    loc = getnextstring(loc, true);
    if(!loc){
        sprintf(buffer, "specific callback info");
        //specific callback
        sprintf(buffer, "Callback  %d   C:%d    Function:%s \r\n",cbindex, callbacks[cbindex].count,callbacks[cbindex].cbpayload);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }else{

        if(*loc == 'c'){
            count = 0;
            strcpy(payload, "");
            goto ADDCB;
        }
        if(isdigit(*loc) == 0 && *loc != '-'){
            sprintf(buffer, "ERROR: Missing digits");
            UART_write(uart, buffer, strlen(buffer));
            Glo.ide = Glo.ide + 1;
            return;
        }
        count = atoi(loc);
    }

    strcpy(payload, "");
    loc = getnextstring(loc, true);
    if(loc)
        strcpy(payload, loc);

ADDCB:
    AddCallBack(cbindex, count, payload);
    sprintf(buffer, "Callback %u: %s", cbindex, loc);
    UART_write(uart, buffer, strlen(buffer));
}

void MPReg(char *input) {
    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
    char *loc;
    char *loc1;
    char *loc2;
    int regIndex = -1, regA = -1, regB = -1, i;
    int32_t immediateA = 0, immediateB = 0;
    bool isRegAImmediate = false, isRegBImmediate = false;

    // Clear buffer to avoid leftover output from previous operations
    //memset(buffer, 0, buffer_size);

    // Get the destination register index (DST Reg)
    loc = getnextstring(input, true);
    if (!loc) {
        sprintf(buffer, "Registers (Decimal, Hex):\r\n");
        UART_write(uart, buffer, strlen(buffer));
        for (i = 0; i < reg_count; i++) {
            sprintf(buffer,"REG %2d: %3d (0x%08X)\r\n", i, regs[i].value, regs[i].value);
            UART_write(uart, buffer, strlen(buffer));
        }
        return;
    }
    regIndex = atoi(loc);

    // Ensure register index is valid
    if (regIndex < 0 || regIndex >= reg_count) {
        sprintf(buffer, "ERROR: Invalid Register Index\r\n");
        UART_write(uart, buffer, strlen(buffer));
        Glo.re++;
        return;
    }

    // Get the operation (OP)
    loc = getnextstring(loc, true);
    if (!loc) {
        //memset(buffer, 0, buffer_size);
        sprintf(buffer, "REG %2d: %3d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    // Handle INC, DEC, NEG, NOT (Unchanged)
    if (matchsub("INC", loc)) {
        regs[regIndex].value++;
        sprintf(buffer, "INC Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
        return;
    } else if (matchsub("DEC", loc)) {
        regs[regIndex].value--;
        sprintf(buffer, "DEC Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
        return;
    } else if (matchsub("NEG", loc)) {
        regs[regIndex].value = -regs[regIndex].value;
        sprintf(buffer, "NEG Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
        return;
    } else if (matchsub("NOT", loc)) {
        regs[regIndex].value = ~regs[regIndex].value;
        sprintf(buffer, "NOT Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    // Get the first source (Reg A, immediate, or memory address)
    loc1 = getnextstring(loc, true);
    if (!loc1) {
        sprintf(buffer, "ERROR: Missing Source Regs A & B/#Immediate(s)\r\n");
        UART_write(uart, buffer, strlen(buffer));
        Glo.re++;
        return;
    }

        // Handle IMMEDIATE (Hex or Decimal)
    if (*loc1 == '#') {
        if (loc1[1] == 'x' || loc1[1] == 'X') {
            uint32_t unsignedImmediate = strtoul(loc1 + 2, NULL, 16);  // Parse as unsigned hex
            immediateA = (int32_t)unsignedImmediate;  // Cast to signed int32_t for storage in registers
        } else if (isdigit(loc1[1])) {
            immediateA = atoi(loc1 + 1);  // Parse as decimal if it's numeric
        } else {
            sprintf(buffer, "ERROR: Invalid immediate value\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        isRegAImmediate = true;

        // Handle DIRECT memory access (@addr)
    } else if (*loc1 == '@') {
        uint32_t address = strtoul(loc1 + 1, NULL, 0);  // Convert to address

        // Check for valid address and numeric input
        if (!isdigit(loc1[1]) && loc1[1] != 'x' && loc1[1] != 'X') {
            sprintf(buffer, "ERROR: Invalid memory address format\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }

        // Check for valid address and memory alignment
        if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
                (address >= VALID_MIN_ADDRESS2 && address <= VALID_MAX_ADDRESS2)) {

            if ((address % 4) != 0) {
                sprintf(buffer, "ERROR: Unaligned memory address 0x%08X\r\n", address);
                UART_write(uart, buffer, strlen(buffer));
                Glo.re++;
                return;
            }

            // Safe memory read
            memcpy(&immediateA, (void*)address, sizeof(int32_t));
            isRegAImmediate = true;

        } else {
            sprintf(buffer, "ERROR: Invalid memory address 0x%08X\r\n", address);
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        // Handle INDIRECT memory access (@REG)
    } else if (*loc1 == 'R') {
        if (!isdigit(loc1[1])) {
            sprintf(buffer, "ERROR: Invalid register format for indirect memory access\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        regA = atoi(loc1 + 1);  // Convert to int after confirming it's numeric

        if (regA >= 0 && regA < reg_count) {
            uint32_t address = regs[regA].value;

            // Check for valid address and memory alignment
            if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
                    (address >= VALID_MIN_ADDRESS2 && address <= VALID_MAX_ADDRESS2)) {

                if ((address % 4) != 0) {
                    sprintf(buffer, "ERROR: Unaligned memory address 0x%08X in REG %d\r\n", address, regA);
                    UART_write(uart, buffer, strlen(buffer));
                    Glo.re++;
                    return;
                        }

                // Safe memory read
                memcpy(&immediateA, (void*)address, sizeof(int32_t));
                isRegAImmediate = true;

            } else {
                sprintf(buffer, "ERROR: Invalid memory address 0x%08X in REG %d\r\n", address, regA);
                UART_write(uart, buffer, strlen(buffer));
                Glo.re++;
                return;
            }
        } else {
            sprintf    (buffer, "ERROR: Invalid Register for indirect memory access\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        // Handle Register (regA)
    } else {
        if (!isdigit(*loc1)) {
            sprintf(buffer, "ERROR: Invalid Register A format\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        regA = atoi(loc1);  // Convert to int after confirming it's numeric
        if (regA < 0 || regA >= reg_count) {
            sprintf(buffer, "ERROR: Invalid Register A\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
    }

    // Handle MOV and other operations with regA or immediateA (unchanged)
    if (matchsub("MOV", loc) && loc1 != 0) {
        regs[regIndex].value = isRegAImmediate ? immediateA : regs[regA].value;
        sprintf(buffer, "MOV Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    // Get the second source (Reg B or immediate)
    loc2 = getnextstring(loc1, true);
    if (!loc2) {
        sprintf(buffer, "ERROR: Missing Source Reg B/#Immediate \r\n");
        UART_write(uart, buffer, strlen(buffer));
        Glo.re++;
        return;
    }

    // Handle IMMEDIATE (Hex or Decimal)
    if (*loc2 == '#') {
        if (loc2[1] == 'x' || loc2[1] == 'X') {
            uint32_t unsignedImmediate = strtoul(loc2 + 2, NULL, 16);  // Parse as unsigned hex
            immediateB = (int32_t)unsignedImmediate;  // Cast to signed int32_t for storage in registers
        } else if (isdigit(loc2[1])) {
            immediateB = atoi(loc2 + 1);  // Parse as decimal if it's numeric
        } else {
            sprintf(buffer, "ERROR: Invalid immediate value\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        isRegBImmediate = true;
        // Handle DIRECT memory access (@addr)
    } else if (*loc2 == '@') {
        uint32_t address = strtoul(loc2 + 1, NULL, 0);  // Convert to address

        // Check for valid address and numeric input
        if (!isdigit(loc2[1]) && loc2[1] != 'x' && loc2[1] != 'X') {
            sprintf(buffer, "ERROR: Invalid memory address format\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }

        // Check for valid address and memory alignment
        if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
                (address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS2)) {

            if ((address % 4) != 0) {
                sprintf(buffer, "ERROR: Unaligned memory address 0x%08X\r\n", address);
                UART_write(uart, buffer, strlen(buffer));
                Glo.re++;
                return;
            }

            // Safe memory read
            memcpy(&immediateB, (void*)address, sizeof(int32_t));
            isRegBImmediate = true;

        } else {
            sprintf(buffer, "ERROR: Invalid memory address 0x%08X\r\n", address);
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        // Handle INDIRECT memory access (@REG)
    } else if (*loc2 == 'R') {
        if (!isdigit(loc2[1])) {
            sprintf(buffer,"ERROR: Invalid register format for indirect memory access\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        regB = atoi(loc2 + 1);  // Convert to int after confirming it's numeric

        if (regB >= 0 && regB < reg_count) {
            uint32_t address = regs[regB].value;

            // Check for valid address and memory alignment
            if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
                    (address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS2)) {

                if ((address % 4) != 0) {
                    sprintf(buffer, "ERROR: Unaligned memory address 0x%08X in REG %d\r\n", address, regB);
                    UART_write(uart, buffer, strlen(buffer));
                    Glo.re++;
                    return;
                        }

                // Safe memory read
                memcpy(&immediateB, (void*)address, sizeof(int32_t));
                isRegBImmediate = true;

            } else {
                sprintf(buffer, "ERROR: Invalid memory address 0x%08X in REG %d\r\n", address, regB);
                UART_write(uart, buffer, strlen(buffer));
                Glo.re++;
                return;
            }
        } else {
            sprintf(buffer, "ERROR: Invalid Register for indirect memory access\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
    } else {
        if (!isdigit(*loc2)) {
            sprintf(buffer, "ERROR: Invalid Register B format\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
        regB = atoi(loc2);  // Convert to int after confirming it's numeric
        if (regB < 0 || regB >= reg_count) {
            sprintf(buffer, "ERROR: Invalid Register B\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
            return;
        }
    }
    // Determine the operation and perform it
    if (matchsub("ADD", loc)) {
        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) +
                               (isRegBImmediate ? immediateB : regs[regB].value);
        sprintf(buffer, "ADD Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));

    } else if (matchsub("SUB", loc)) {
        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) -
                               (isRegBImmediate ? immediateB : regs[regB].value);
        sprintf(buffer, "SUB Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
    } else if (matchsub("AND", loc)) {
        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) &
                               (isRegBImmediate ? immediateB : regs[regB].value);
        sprintf(buffer, "AND Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
    } else if (matchsub("IOR", loc)) {
        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) |
                               (isRegBImmediate ? immediateB : regs[regB].value);
        sprintf(buffer, "IOR Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
    } else if (matchsub("XOR", loc)) {
        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) ^
                               (isRegBImmediate ? immediateB : regs[regB].value);
        sprintf(buffer, "XOR Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
    } else if (matchsub("MUL", loc)) {
        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) *
                               (isRegBImmediate ? immediateB : regs[regB].value);
        sprintf(buffer, "MUL Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
    } else if (matchsub("DIV", loc)) {
        if ((isRegBImmediate ? immediateB : regs[regB].value) != 0) {
            regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) /
                                   (isRegBImmediate ? immediateB : regs[regB].value);
            sprintf(buffer, "DIV Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
            UART_write(uart, buffer, strlen(buffer));
        } else {
            sprintf(buffer, "ERROR: Division by zero\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
        }
    } else if (matchsub("REM", loc)) {
        if ((isRegBImmediate ? immediateB : regs[regB].value) != 0) {
            regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) %
                                   (isRegBImmediate ? immediateB : regs[regB].value);
            sprintf(buffer, "REM Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
            UART_write(uart, buffer, strlen(buffer));
        } else {
            sprintf(buffer, "ERROR: Division by zero\r\n");
            UART_write(uart, buffer, strlen(buffer));
            Glo.re++;
        }
    } else if (matchsub("MAX", loc)) {
        regs[regIndex].value = (regs[regA].value > regs[regB].value) ? regs[regA].value : regs[regB].value;
        sprintf(buffer, "MAX Result: %d\r\n", regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
    } else if (matchsub("MIN", loc)) {
        regs[regIndex].value = (regs[regA].value < regs[regB].value) ? regs[regA].value : regs[regB].value;
        sprintf(buffer, "MIN Result: %d\r\n", regs[regIndex].value);
        UART_write(uart, buffer, strlen(buffer));
    } else {
        sprintf(buffer, "ERROR: Invalid Operation\r\n");
        UART_write(uart, buffer, strlen(buffer));
        Glo.re++;
    }
}

void MPScript(char *input){
    char buffer[BUFFER_SIZE];
    size_t buffer_size;
    char *loc;
    int index = -1;
    int i;
    char scrPayload[MSGLENGTH] = "";

    // Get the first argument after `-script`
    loc = getnextstring(input, true);

    // Check if the argument is "c" for clearing scripts
    if (loc && loc[0] == 'c') {
        sprintf(buffer, "All Scripts Cleared\r\n");
        UART_write(uart, buffer, strlen(buffer));

        for (i = 0; i < script_count; i++) {
            create_script(i, "");  // Clear each script line
            snprintf(buffer, buffer_size, "SCRIPT %d \t| \r\n", i);
            UART_write(uart, buffer, strlen(buffer));
        }
        strcpy(buffer, "");
        return;
    }

    // If no arguments, display the entire script
    else if (!loc) {
        snprintf(buffer, buffer_size, "-------------------------\r\n");
        UART_write(uart, buffer, strlen(buffer));
        for (i = 0; i < script_count; i++) {
            snprintf(buffer, buffer_size, "SCRIPT %d \t| %s\r\n", i, scripts[i].scrPayload);
            UART_write(uart, buffer, strlen(buffer));
        }
        strcpy(buffer, "");
        return;
    }

    // Check if the first argument is a digit (line number)
    else if (isdigit(*loc) == 0) {
        snprintf(buffer, buffer_size, "ERROR: Invalid digit");
        UART_write(uart, buffer, strlen(buffer));
        Glo.ide++;
        return;
    }

    // Parse the line number from the argument
    index = atoi(loc);
    if (index < 0 || index >= script_count) {
        snprintf(buffer, buffer_size, "ERROR: Invalid script line number\r\n");
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    // Get the next argument after the line number
    loc = getnextstring(loc, true);

    // If no additional argument, display the script line at `index`
    if (!loc) {
        sprintf(buffer, "SCRIPT %d \t| %s\r\n", index, scripts[index].scrPayload);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    // Execute script starting from `index`
    else if (loc[0] == 'x') {
        uint32_t gateKey = GateSwi_enter(gateSwi2);
        for (i = index; i < script_count; i++) {
            // Stop execution on an empty script line
            enqueueMessage(scripts[i].scrPayload);  // Execute the command in the script line
            if (scripts[i].scrPayload[0] == '\0') {
                snprintf(buffer,buffer_size, "");
                UART_write(uart, buffer, strlen(buffer));
                break;
            }
        }
        GateSwi_leave(gateSwi2, gateKey);
        strcpy(buffer, "");
        return;

    }

    // Write to a specific script line if `w` is the command
    else if (loc[0] == 'w') {
        loc = getnextstring(loc, true);  // Get the payload for the script line
        if (loc) {
            strcpy(scrPayload, loc);
            create_script(index, scrPayload);  // Load the payload into the specified script line
            snprintf(buffer, buffer_size, "Scr %d %s\r\n", index, scripts[index].scrPayload);
            UART_write(uart, buffer, strlen(buffer));
        }
        strcpy(buffer, "");
        return;

    }
    else if (loc[0] == 'c') {
         create_script(index, "");  // Clear each script line
         snprintf(buffer, buffer_size, "SCRIPT %d \t| \r\n", index);
        UART_write(uart, buffer, strlen(buffer));
        strcpy(buffer, "");
        return;

    }
}

void MPSin(char *input)
{
    char *loc;
    double lowerweight, upperweight, intermediateINT;
    uint32_t lowerindex, upperindex;
    uint16_t outputPower;
    uint32_t freq;
    char printer[50] = {0};
    SPI_Transaction spiTransaction;
    bool transferOK;

    loc = getnextstring(input, true);
    sscanf(loc, "%d", &freq);

    /*Nyquist stuff */
    if(freq >= 4000){
        sprintf(printer, "timer 0");
        MPtimer(printer);
        sprintf(printer, "-callback 0 0 0");
        MPCallback(printer);
//        enqueueMessage(printer);
        Glo.incrementLookupTable = 0;
        Glo.position = 0;
    }

    else if (input[0] != '\0' && freq >= 1 && Glo.Timer0Period > 0){
        sprintf(printer, "-timer 125");
        //MPtimer(printer);
        enqueueMessage(printer);
        Glo.incrementLookupTable = (double) freq * (double) 256 * (double) Glo.Timer0Period / 1000000.;
        sprintf(printer, "-callback 0 -1 -sine");
        enqueueMessage(printer);
//        enqueueMessage(printer);
        //UART_write(uart, '\r\n', 2);
    }

    lowerindex = (uint32_t)Glo.position;
    upperindex = (lowerindex + 1) % LUTSIZE;
    lowerweight = 1.0 - (Glo.position - (double) lowerindex);
    upperweight = 1.0 - lowerweight;
    intermediateINT = ((double)(SINETABLE[lowerindex] * lowerweight) + ((double) SINETABLE[upperindex] * upperweight));
    outputPower = (int) intermediateINT;

    spiTransaction.count = 1;
    spiTransaction.txBuf = &outputPower;
    spiTransaction.rxBuf = NULL;

    transferOK = SPI_transfer(Glo.spi, &spiTransaction);
    if (!transferOK){
        while (1);
    }
    Glo.position += Glo.incrementLookupTable;
    while(Glo.position >=  LUTSIZE){
        Glo.position -=  LUTSIZE;
    }
}

void MPTicker(char *input){
    char buffer[BUFFER_SIZE];
    size_t buffer_size;
    char *loc;
    int index = -1, delay = 0, count = 0, period = 0;
    int i;
    char payload[MSGLENGTH];

    loc = getnextstring(input, true);
    if(!loc){
        index = -1;
    }
    else if(loc[0] == 'c'){
        sprintf(buffer, "Tickers Cleared\r\n");

        UART_write(uart, buffer, strlen(buffer));
//        for(i = 0; i < ticker_count; i++){
//            index = create_ticker(i, 0, 0, 0, "");
//            snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d           \r\n",
//                     i, tickers[i].delay, tickers[i].period, tickers[i].count);
//            UART_write(uart, buffer, strlen(buffer));
//        }
        sprintf(buffer, "\r\n");
        return;


    }else{
        if(isdigit(*loc) == 0 && *loc != '-'){
            snprintf(buffer, buffer_size, "ERROR: Invalid digit");
            UART_write(uart, buffer, strlen(buffer));
            Glo.ide++;
            return;

        }
        index = atoi(loc);

    }
    if(index < 0 || index >= ticker_count){
        snprintf(buffer, buffer_size, "-----------------------------------\r\n");
        UART_write(uart, buffer, strlen(buffer));
        for(i = 0; i < ticker_count; i++){
            snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d \r\n",
                     i, tickers[i].delay, tickers[i].period, tickers[i].count);
            UART_write(uart, buffer, strlen(buffer));
        }
        sprintf(buffer, "\r\n");
        return;

    }


    loc = getnextstring(loc, true);
    if(!loc){
        snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d \r\n",
                 index, tickers[index].delay, tickers[index].period, tickers[index].count);
        UART_write(uart, buffer, strlen(buffer));
        return;
    }else if(loc[0] == 'c'){
        index = create_ticker(index, 0, 0, 0, "");
        snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d          \r\n",
                         index, tickers[index].delay, tickers[index].period, tickers[index].count);
        UART_write(uart, buffer, strlen(buffer));
                return;
    }
    if(isdigit(*loc) == 0){
        snprintf(buffer, buffer_size, "ERROR: Invalid Digit");
        Glo.ide++;
        return;
    }
    delay = atoi(loc);
    strcpy(payload, "");
    // conditions for the delay limits
    loc = getnextstring(loc, true);

    if(!loc){
        goto step1;
    }
    if(isdigit(*loc) == 0){
            snprintf(buffer, buffer_size, "ERROR: Invalid Digit");
            UART_write(uart, buffer, strlen(buffer));
            Glo.ide++;
            return;
        }
    period = atoi(loc);
    loc = getnextstring(loc, true);

    if(!loc){
           goto step1;
       }
    if(isdigit(*loc) == 0 && *loc != '-'){
        snprintf(buffer, buffer_size, "ERROR: Invalid Digit");
        UART_write(uart, buffer, strlen(buffer));
        Glo.ide++;
        return;

           }
    count = atoi(loc);

    loc = getnextstring(loc, true);

    if(loc)
        strcpy(payload, loc);

step1:
    index = create_ticker(index, delay, period, count, payload);
    snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d %s\r\n",
                     index, tickers[index].delay, tickers[index].period, tickers[index].count, tickers[index].payload);
    UART_write(uart, buffer, strlen(buffer));
}
void MPUart7(char *input) {
    char *loc;
    char xmitbuf[MSGLENGTH] = {0};  // Independent buffer for UART7 transmission

    // Extract the command from input specific to UART7
    loc = getnextstring(input, true);

        // Format the message for UART7 transmission with newlines
        snprintf(xmitbuf, sizeof(xmitbuf), "\r\n%s\r\n", loc);

        // Send the message through UART7
        UART_write(uart7, xmitbuf, strlen(xmitbuf));

}

void Error(char* input){
    char buffer[BUFFER_SIZE];
    //size_t buffer_size=128;
    char *loc;

    loc = getnextstring(input, true);

    if(!loc){
       sprintf(buffer, "----------------------------\r\n"
                                     "Unknown Command Errors: | %d\r\n"
                                     "Memory Errors:          | %d\r\n"
                                     "Invalid Digit Errors:   | %d\r\n"
                                     "Buffer Overflow Errors: | %d\r\n"
                                     "Queue Overfow Errors:   | %d\r\n"
                                     "Register Errors:        | %d\r\n",
                                     Glo.uce, Glo.me, Glo.ide, Glo.bfe, Glo.qoe,Glo.re);
       UART_write(uart, buffer, strlen(buffer));
       return;
    }
    else if(strstr(loc, "c") != NULL){
        Glo.uce=0;
        Glo.me=0;
        Glo.ide=0;
        Glo.bfe=0;
        Glo.qoe=0;
        Glo.re=0;
        sprintf(buffer, "----   ERRORS CLEARED   ----\r\n"
                                             "Unknown Command Errors: | %d\r\n"
                                             "Memory Errors:          | %d\r\n"
                                             "Invalid Digit Errors:   | %d\r\n"
                                             "Buffer Overflow Errors: | %d\r\n"
                                             "Queue Overflow Errors:  | %d\r\n"
                                             "Register Errors:        | %d\r\n",
                                                    Glo.uce, Glo.me, Glo.ide, Glo.bfe,Glo.qoe,Glo.re);

        UART_write(uart, buffer, strlen(buffer));
        return;
    }
}

int32_t AddCallBack(int32_t cbindex, int32_t count, char* payload){
    uint32_t gateKey;


//    int payloadnext;
//    payloadnext = writeIndex + 1;
//
//    if(payloadnext == readIndex){
//            return;
//    }

    if(cbindex< 0 || cbindex >= CALLBACKCOUNT)
        return -1;
    gateKey = GateSwi_enter(gateSwi1);
    callbacks[cbindex].count = count;
    strcpy(callbacks[cbindex].cbpayload, payload);
    GateSwi_leave(gateSwi1, gateKey);
    return cbindex;
}
//################## TIMERS AND SWI STUFF #################
void TIMER0SWI(){ //Callback 0 Timer
    int32_t i = 0;
    uint32_t gateKey;
    gateKey = GateSwi_enter(gateSwi1);
    if(callbacks[i].count != 0){
        if(callbacks[i].count > 0)
            callbacks[i].count--;
        enqueueMessage(callbacks[i].cbpayload);

        GateSwi_leave(gateSwi1, gateKey);
    }

}


void SWIbuttonSW1(){

    int32_t i = 1;
    uint32_t gateKey;

    gateKey = GateSwi_enter(gateSwi1);
    if(callbacks[i].count != 0){
        if(callbacks[i].count > 0)
            callbacks[i].count--;
        enqueueMessage(callbacks[i].cbpayload);
    }
    GateSwi_leave(gateSwi1, gateKey);
}

void SWIbuttonSW2(){

    int32_t i = 2;
    uint32_t gateKey;

    gateKey = GateSwi_enter(gateSwi1);
    if(callbacks[i].count != 0){
        if(callbacks[i].count > 0)
            callbacks[i].count--;
        enqueueMessage(callbacks[i].cbpayload);
    }
    GateSwi_leave(gateSwi1, gateKey);
}

//POSTING SWI's
void MPIf(char *input) {
    char buffer[BUFFER_SIZE];
    size_t buffer_size;
    char *loc;
    int regA = -1, regB = -1, valA = 0, valB = 0;
    bool isImmediateA = false, isImmediateB = false;
    bool conditionMet = false;
    char condOperator;
    char trueAction[MSGLENGTH] = {0};
    char falseAction[MSGLENGTH] = {0};

    // Parse A
    loc = getnextstring(input, true);
    if (loc[0] == '#') {
        isImmediateA = true;
        valA = atoi(loc + 1);  // Parse immediate value
    } else {
        regA = atoi(loc);
        if (regA < 0 || regA >= reg_count) {
            snprintf(buffer, buffer_size, "ERROR: Invalid Register A\r\n");
            UART_write(uart, buffer, strlen(buffer));
            return;
        }
        valA = regs[regA].value;  // Get value from register
    }

    // Parse condition operator (COND)
    loc = getnextstring(loc, true);
    if (loc[0] == '<') {
        condOperator = '<';
    } else if (loc[0] == '=') {
        condOperator = '=';
    } else if (loc[0] == '>') {
        condOperator = '>';
    } else {
        snprintf(buffer, buffer_size, "ERROR: Invalid Condition Operator\r\n");
        UART_write(uart, buffer, strlen(buffer));
        return;
    }

    // Parse B
    loc = getnextstring(loc, true);
    if (loc[0] == '#') {
        isImmediateB = true;
        valB = atoi(loc + 1);  // Parse immediate value
    } else {
        regB = atoi(loc);
        if (regB < 0 || regB >= reg_count) {
            snprintf(buffer, buffer_size, "ERROR: Invalid Register B\r\n");
            UART_write(uart, buffer, strlen(buffer));
            return;
        }
        valB = regs[regB].value;  // Get value from register
    }

    // Evaluate condition
    if (condOperator == '>') {
        conditionMet = (valA > valB);
    } else if (condOperator == '=') {
        conditionMet = (valA == valB);
    } else if (condOperator == '<') {
        conditionMet = (valA < valB);
    }

    // Parse DESTT and DESTF actions
    loc = getnextstring(loc, true);  // Skip the '?' symbol
    char *destTrue = getnextstring(loc, true);  // True action
    char *destFalse = strstr(destTrue, ":");  // Find separator for false action

    if (destFalse) {
        *destFalse = '\0';  // Terminate true action
        destFalse++;  // Move pointer to start of false action
        while (*destFalse == ' ') destFalse++;  // Skip spaces after ':'
    } else {
        destFalse = "";  // No false action provided
    }

    strncpy(trueAction, destTrue, sizeof(trueAction) - 1);
    strncpy(falseAction, destFalse, sizeof(falseAction) - 1);

    // Execute based on condition
    if (conditionMet) {
        if (trueAction[0] != '\0') {
            enqueueMessage(trueAction);  // Execute true action
            strcpy(buffer, "");
        }
    } else {
        if (falseAction[0] != '\0') {
            enqueueMessage(falseAction);  // Execute false action
            strcpy(buffer, "");
        }
    }
    strcpy(buffer, "");

}
void MPstream(char *input) { ////NEEEDS FIXES HERE

    char *loc;
    int streamCommand = -1;

    // Parse the input to determine the stream command (0, 1, or 2)
    loc = getnextstring(input, true);
    if (loc) {
        streamCommand = atoi(loc);
    }

    switch (streamCommand) {
        case 0: // Stop ADC stream
            if(Glo.ADCBufCtrl.converting == 2){
                        if(ADCBuf_convertCancel(Glo.ADCBuf.Handle) != ADCBuf_STATUS_SUCCESS){
                            enqueueMessage("-print error: ADCBuf Status");
                        }
                        Glo.ADCBufCtrl.converting = 0;
//                        if(Glo.LUTCtrl.lutDelta > 0.0){
//                            SineParse("-sine 0");
//                        }
                        MPCallback("-callback 0 0");
                        MPtimer("-timer 0");
                        Glo.ADCBufCtrl.RX_Completed = NULL;
                        int i;
                        for(i = 0; i < 2; i++){
                            Glo.TXBufCtrl[i].TX_Completed = NULL;
                            Glo.TXBufCtrl[i].TX_index = -1;
                        }
             }
            break;


        case 1: // Prepare ADC stream
            MPGPIO("-gpio 4 w 0");
            MPGPIO("-gpio 5 w 1");
            MPCallback("-callback 0 -1 -audio");
            MPtimer("-timer 125");
//            MPtimer("-timer 1");
            Glo.ADCBufCtrl.converting = 1;
            MPTicker("-ticker 5 50 0 1 -stream 2");
            //MPtimer("-ticker 16 50 0 1 -stream 2");
            ClearAudioBuffers();
            break;
        case 2: // Start ADC stream
            if(ADCBuf_convert(Glo.ADCBuf.Handle, &Glo.ADCBufCtrl.conversion, 1) != ADCBuf_STATUS_SUCCESS){
                enqueueMessage("-print error: ADCBuf Status");
            }
            else{
                Glo.ADCBufCtrl.converting = 2;
                Glo.ADCBufCtrl.RX_Completed = NULL;
                int i;
                for(i = 0; i < 2; i++){
                    Glo.TXBufCtrl[i].TX_Completed = NULL;
                    Glo.TXBufCtrl[i].TX_index = -1;
                    Glo.TXBufCtrl[i].TX_delay = 8;
                    Glo.TXBufCtrl[i].TX_correction = 0;
                }
            }
            break;
        default: // Invalid stream command
            UART_write(uart, "ERROR: Invalid stream mode. Use 0, 1, or 2.\r\n", 44);
            break;
    }
}

void MPaudio(){
    SPI_Transaction spiTransaction;
    bool transferOK;
    int32_t i;
    uint16_t outval = 0;

    for(i = 0; i < 2; i++){
        if(Glo.TXBufCtrl[i].TX_Completed != NULL){
            if(Glo.TXBufCtrl[i].TX_index >= 0){
                if(Glo.TXBufCtrl[i].TX_delay > 0){ //make sure to go initialize delays to 8
                    Glo.TXBufCtrl[i].TX_delay--;
                    return;
                }
                outval += Glo.TXBufCtrl[i].TX_Completed[Glo.TXBufCtrl[i].TX_index++];

                Glo.TXBufCtrl[i].TX_index += Glo.TXBufCtrl[i].TX_correction;
                Glo.TXBufCtrl[i].TX_correction = 0;

                if(Glo.TXBufCtrl[i].TX_index >= DATABLOCKSIZE){
                    Glo.TXBufCtrl[i].TX_index = 0;
                    if(Glo.TXBufCtrl[i].TX_Completed == Glo.TXBufCtrl[i].TX_Ping){
                        Glo.TXBufCtrl[i].TX_Completed = Glo.TXBufCtrl[i].TX_Pong;
                    }
                    else{
                        Glo.TXBufCtrl[i].TX_Completed = Glo.TXBufCtrl[i].TX_Ping;
                    }
                }
            }
        }
    }
    spiTransaction.count = 1;
    spiTransaction.txBuf = (void *) &outval;
    spiTransaction.rxBuf = (void *) NULL;
    transferOK = SPI_transfer(Glo.spi, &spiTransaction);
    if(!transferOK){
        while(1);
    }
}
void VoiceParse(char *ch){
//
//    -VoiceParse
//    move 128 samples from message buffer to TX ping/pong MsgBufferSize
//    knows local vs long distance by -reg contents
//    makes -VoiceParse
//    or
//    makes -netudp -VoiceParse
//    local and long distance have same number of memcpy
//
    int32_t dest_choice;
    int32_t bufflen;
    int32_t TX01;
    int32_t current;
    uint16_t  *dest;
    char    *StrBuffPTR;
//    char posbuf[100];
    static int32_t last[2] = {-2, -2};
    static int32_t lastlast[2] = {-4, -4};

    StrBuffPTR = getnextstring(ch,true);

    if(!StrBuffPTR)
        return;

    dest_choice = atoi(StrBuffPTR);

    StrBuffPTR = getnextstring(StrBuffPTR, true);

    bufflen = atoi(StrBuffPTR);

    if(bufflen != DATABLOCKSIZE){
        enqueueMessage("-print blocksize err");
        return;
    }
//    StrBuffPTR = NextSubString(StrBuffPTR, false);
    StrBuffPTR++;
    StrBuffPTR++;
    StrBuffPTR++;
    StrBuffPTR++;
    StrBuffPTR++;
    StrBuffPTR++;

    if(dest_choice == 0){
        TX01 = 0;
        dest = Glo.TXBufCtrl[TX01].TX_Ping;
    }
    else if(dest_choice == 1){
        TX01 = 0;
        dest = Glo.TXBufCtrl[TX01].TX_Pong;
    }
    else if(dest_choice == 2){
        TX01 = 1;
        dest = Glo.TXBufCtrl[TX01].TX_Ping;
    }
    else if(dest_choice == 3){
        TX01 = 1;
        dest = Glo.TXBufCtrl[TX01].TX_Pong;
    }
    else{
        enqueueMessage("-print dest_choice err");
        return;
    }

    memcpy(dest, StrBuffPTR, sizeof(uint16_t)*DATABLOCKSIZE);

    if(Glo.TXBufCtrl[TX01].TX_Completed == NULL){
        Glo.TXBufCtrl[TX01].TX_Completed = dest;
        Glo.TXBufCtrl[TX01].TX_index = 0;
    }
    current = Glo.TXBufCtrl[TX01].TX_index;
    if(last[TX01] != current && lastlast[TX01] != current){
        lastlast[TX01] = last[TX01];
        last[TX01] = current;
        if(current >= DATABLOCKSIZE - 8 + 4){
            Glo.TXBufCtrl[TX01].TX_correction = -1;
        }
        else if( current <= DATABLOCKSIZE - 8 - 4){
            Glo.TXBufCtrl[TX01].TX_correction = +1;
        }
    }
}


void Timer0Callback(){ //FOR CALLBACK 0
    Swi_post(swi0);
}

void ADCBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion, void *buffer, uint32_t channel, int_fast16_t status) {
    if (buffer == Glo.ADCBufCtrl.RX_Ping) {
        Glo.ADCBufCtrl.RX_Completed = Glo.ADCBufCtrl.RX_Ping;
    } else if (buffer == Glo.ADCBufCtrl.RX_Pong) {
        Glo.ADCBufCtrl.RX_Completed = Glo.ADCBufCtrl.RX_Pong;
    } else {
        UART_write(uart, "ERROR: Invalid ADC buffer\r\n", 27);
        return;
    }
    Semaphore_post(semaphore1);  // Notify the audio task to process the buffer
}
void CallbackSW1(){ //Button callback
    Swi_post(swi1);
}
void CallbackSW2(){ //button callback
    Swi_post(swi2);
}
//Enqueueing Messages/Decrement callbacks[i].count
void Timer1Callback(){
    Swi_post(swi3);
}
void ADC_Swi(){

}

void SWI_Timer0(){ //FOR CALLBCACK 0
    TIMER0SWI();
}

void SWI_Left(){ //FOR LEFT BUTTON
    SWIbuttonSW1();
}
void SWI_Right(){ //FOR RIGHT BUTTON
    SWIbuttonSW2();
}


// Preload all registers with initial values
void initialize_registers() {
    int i;
    for (i = 0; i < reg_count; i++) {
        regs[i].value = i * 4;  // Set reg[0] = 0, reg[1] = 10, ..., reg[31] = 310
    }
}

// Function to initialize or clear a script line
int create_script(int index, char *scrPayload) {
    uint32_t gateKey = GateSwi_enter(gateSwi2);
    strncpy(scripts[index].scrPayload, scrPayload, MSGLENGTH - 1);
    scripts[index].scrPayload[MSGLENGTH - 1] = '\0';  // Ensure null termination
    GateSwi_leave(gateSwi2, gateKey);
    return index;
}
/// Create the ticker

int create_ticker(int index, int delay, int period, int count, char *payload){

    int32_t gateKey;

    gateKey = GateSwi_enter(gateSwi1);
    tickers[index].delay = delay;
    tickers[index].period = period;
    tickers[index].count = count;
    strcpy(tickers[index].payload, payload);

    GateSwi_leave(gateSwi1, gateKey);
    return index;

}//END


void TIMER1SWI(){
    int i;
    int32_t gateKey;
    gateKey = GateSwi_enter(gateSwi1);
    for(i = 0; i < ticker_count; i++){
        if(tickers[i].delay > 0){
            tickers[i].delay--;
            if(tickers[i].delay <=0){
                if(tickers[i].count != 0){
                    tickers[i].delay = tickers[i].period;
                    if(tickers[i].count > 0){
                        tickers[i].count--;
                    }
                }
                enqueueMessage(tickers[i].payload);
            }
        }
    }
    GateSwi_leave(gateSwi1, gateKey);
}

void SWI_Timer1(){
    TIMER1SWI();
}
int writeIndex = 0;
int readIndex = 0;
char payloads[PAYLOADCOUNT][PAYLOAD_SIZE];
bool matchsub(const char* command, const char* input){
    size_t c_l = strlen(command);
    return strncmp(command, input, c_l) == 0;
}

void *getnextstring(char* input, bool Whites){
    char *loc;
    if(input == NULL)
        return NULL;
    loc = strchr(input, ' ');
    if(!loc)
        return NULL;
    if(Whites)
        while(*loc == ' ')
            loc++;
    else
        if(*loc == ' ')
            loc++;

    if(!*loc)
        return NULL;

    return loc;
}

void enqueueMessage(const char* payload){
    int32_t payloadnext;
    int32_t qindex;
    uint32_t gateKey;

    if(!payload || payload[0] == 0)
        return;

    gateKey = GateSwi_enter(gateSwi0);
    qindex = writeIndex;
    payloadnext = qindex + 1;
    if(payloadnext >= PAYLOADCOUNT)
        payloadnext = 0;

    if(payloadnext == readIndex){
        GateSwi_leave(gateSwi0, gateKey);
        Glo.qoe++;
        return;
    }else{
        strcpy(payloads[qindex], payload);
        writeIndex = payloadnext;
    }

    GateSwi_leave(gateSwi0, gateKey);
    Semaphore_post(semaphore0);
    return;

}

void ClearAudioBuffers(){
    int i = 0;
    int j = 0;
    for(i = 0; i < DATABLOCKSIZE; i++){
        Glo.ADCBufCtrl.RX_Ping[i] = 0;
        Glo.ADCBufCtrl.RX_Pong[i] = 0;
    }
    for(i = 0; i < 2; i++){
        for(j = 0; j < DATABLOCKSIZE; j++){
            Glo.TXBufCtrl[i].TX_Ping[j] = 0;
            Glo.TXBufCtrl[i].TX_Pong[j] = 0;
        }
    }
}
