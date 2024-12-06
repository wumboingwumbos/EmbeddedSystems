//#include "ph001.h"
//
//void MPIf(char *input) {
//    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
//    char *loc;
//    int regA = -1, regB = -1, valA = 0, valB = 0;
//    bool isImmediateA = false, isImmediateB = false;
//    bool conditionMet = false;
//    char condOperator;
//    char trueAction[MSGLENGTH] = {0};
//    char falseAction[MSGLENGTH] = {0};
//
//    // Parse A
//    loc = getnextstring(input, true);
//    if (loc[0] == '#') {
//        isImmediateA = true;
//        valA = atoi(loc + 1);  // Parse immediate value
//    } else {
//        regA = atoi(loc);
//        if (regA < 0 || regA >= reg_count) {
//            snprintf(buffer, buffer_size, "ERROR: Invalid Register A\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            return;
//        }
//        valA = regs[regA].value;  // Get value from register
//    }
//
//    // Parse condition operator (COND)
//    loc = getnextstring(loc, true);
//    if (loc[0] == '<') {
//        condOperator = '<';
//    } else if (loc[0] == '=') {
//        condOperator = '=';
//    } else if (loc[0] == '>') {
//        condOperator = '>';
//    } else {
//        snprintf(buffer, buffer_size, "ERROR: Invalid Condition Operator\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    }
//
//    // Parse B
//    loc = getnextstring(loc, true);
//    if (loc[0] == '#') {
//        isImmediateB = true;
//        valB = atoi(loc + 1);  // Parse immediate value
//    } else {
//        regB = atoi(loc);
//        if (regB < 0 || regB >= reg_count) {
//            snprintf(buffer, buffer_size, "ERROR: Invalid Register B\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            return;
//        }
//        valB = regs[regB].value;  // Get value from register
//    }
//
//    // Evaluate condition
//    if (condOperator == '>') {
//        conditionMet = (valA > valB);
//    } else if (condOperator == '=') {
//        conditionMet = (valA == valB);
//    } else if (condOperator == '<') {
//        conditionMet = (valA < valB);
//    }
//
//    // Parse DESTT and DESTF actions
//    loc = getnextstring(loc, true);  // Skip the '?' symbol
//    char *destTrue = getnextstring(loc, true);  // True action
//    char *destFalse = strstr(destTrue, ":");  // Find separator for false action
//
//    if (destFalse) {
//        *destFalse = '\0';  // Terminate true action
//        destFalse++;  // Move pointer to start of false action
//        while (*destFalse == ' ') destFalse++;  // Skip spaces after ':'
//    } else {
//        destFalse = "";  // No false action provided
//    }
//
//    strncpy(trueAction, destTrue, sizeof(trueAction) - 1);
//    strncpy(falseAction, destFalse, sizeof(falseAction) - 1);
//
//    // Execute based on condition
//    if (conditionMet) {
//        if (trueAction[0] != '\0') {
//            enqueueMessage(trueAction);  // Execute true action
//            strcpy(buffer, "");
//        }
//    } else {
//        if (falseAction[0] != '\0') {
//            enqueueMessage(falseAction);  // Execute false action
//            strcpy(buffer, "");
//        }
//    }
//    strcpy(buffer, "");
//
//}
