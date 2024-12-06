//#include "ph001.h"
//
//void MPScript(char *input){
//    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
//    char *loc;
//    int index = -1;
//    int i;
//    char scrPayload[MSGLENGTH] = "";
//
//    // Get the first argument after `-script`
//    loc = getnextstring(input, true);
//
//    // Check if the argument is "c" for clearing scripts
//    if (loc && loc[0] == 'c') {
//        snprintf(buffer, buffer_size, "All Scripts Cleared\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//
//        for (i = 0; i < script_count; i++) {
//            create_script(i, "");  // Clear each script line
//            snprintf(buffer, buffer_size, "SCRIPT %d \t| \r\n", i);
//            UART_write(uart, buffer, strlen(buffer));
//        }
//        strcpy(buffer, "");
//        return;
//    }
//
//    // If no arguments, display the entire script
//    else if (!loc) {
//        snprintf(buffer, buffer_size, "-------------------------\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        for (i = 0; i < script_count; i++) {
//            snprintf(buffer, buffer_size, "SCRIPT %d \t| %s\r\n", i, scripts[i].scrPayload);
//            UART_write(uart, buffer, strlen(buffer));
//        }
//        strcpy(buffer, "");
//        return;
//    }
//
//    // Check if the first argument is a digit (line number)
//    else if (isdigit(*loc) == 0) {
//        snprintf(buffer, buffer_size, "ERROR: Invalid digit");
//        UART_write(uart, buffer, strlen(buffer));
//        Glo.ide++;
//        return;
//    }
//
//    // Parse the line number from the argument
//    index = atoi(loc);
//    if (index < 0 || index >= script_count) {
//        snprintf(buffer, buffer_size, "ERROR: Invalid script line number\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    }
//
//    // Get the next argument after the line number
//    loc = getnextstring(loc, true);
//
//    // If no additional argument, display the script line at `index`
//    if (!loc) {
//        sprintf(buffer, "SCRIPT %d \t| %s\r\n", index, scripts[index].scrPayload);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    }
//
//    // Execute script starting from `index`
//    else if (loc[0] == 'x') {
//        uint32_t gateKey = GateSwi_enter(gateSwi2);
//        for (i = index; i < script_count; i++) {
//            // Stop execution on an empty script line
//            enqueueMessage(scripts[i].scrPayload);  // Execute the command in the script line
//            if (scripts[i].scrPayload[0] == '\0') {
//                snprintf(buffer,buffer_size, "");
//                UART_write(uart, buffer, strlen(buffer));
//                break;
//            }
//        }
//        GateSwi_leave(gateSwi2, gateKey);
//        return;
//        strcpy(buffer, "");
//    }
//
//    // Write to a specific script line if `w` is the command
//    else if (loc[0] == 'w') {
//        loc = getnextstring(loc, true);  // Get the payload for the script line
//        if (loc) {
//            strcpy(scrPayload, loc);
//            create_script(index, scrPayload);  // Load the payload into the specified script line
//            snprintf(buffer, buffer_size, "Scr %d %s\r\n", index, scripts[index].scrPayload);
//            UART_write(uart, buffer, strlen(buffer));
//        }
//        return;
//        strcpy(buffer, "");
//    }
//    else if (loc[0] == 'c') {
//         create_script(index, "");  // Clear each script line
//         snprintf(buffer, buffer_size, "SCRIPT %d \t| \r\n", index);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//        strcpy(buffer, "");
//    }
//}
//
//// Function to initialize or clear a script line
//int create_script(int index, char *scrPayload) {
//    uint32_t gateKey = GateSwi_enter(gateSwi2);
//    strncpy(scripts[index].scrPayload, scrPayload, MSGLENGTH - 1);
//    scripts[index].scrPayload[MSGLENGTH - 1] = '\0';  // Ensure null termination
//    GateSwi_leave(gateSwi2, gateKey);
//    return index;
//}
