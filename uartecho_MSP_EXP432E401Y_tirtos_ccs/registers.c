//#include "ph001.h"
//
//// Preload all registers with initial values
//void initialize_registers() {
//    int i;
//    for (i = 0; i < reg_count; i++) {
//        regs[i].value = i * 4;  // Set reg[0] = 0, reg[1] = 10, ..., reg[31] = 310
//    }
//}
//
//void MPReg(char *input) {
//    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
//    char *loc;
//    char *loc1;
//    char *loc2;
//    int regIndex = -1, regA = -1, regB = -1, i;
//    int32_t immediateA = 0, immediateB = 0;
//    bool isRegAImmediate = false, isRegBImmediate = false;
//
//    // Clear buffer to avoid leftover output from previous operations
//    //memset(buffer, 0, buffer_size);
//
//    // Get the destination register index (DST Reg)
//    loc = getnextstring(input, true);
//    if (!loc) {
//        sprintf(buffer, "Registers (Decimal, Hex):\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        for (i = 0; i < reg_count; i++) {
//            sprintf(buffer,"REG %2d: %3d (0x%08X)\r\n", i, regs[i].value, regs[i].value);
//            UART_write(uart, buffer, strlen(buffer));
//        }
//        return;
//    }
//    regIndex = atoi(loc);
//
//    // Ensure register index is valid
//    if (regIndex < 0 || regIndex >= reg_count) {
//        sprintf(buffer, "ERROR: Invalid Register Index\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        Glo.re++;
//        return;
//    }
//
//    // Get the operation (OP)
//    loc = getnextstring(loc, true);
//    if (!loc) {
//        //memset(buffer, 0, buffer_size);
//        sprintf(buffer, "REG %2d: %3d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    }
//
//    // Handle INC, DEC, NEG, NOT (Unchanged)
//    if (matchsub("INC", loc)) {
//        regs[regIndex].value++;
//        sprintf(buffer, "INC Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    } else if (matchsub("DEC", loc)) {
//        regs[regIndex].value--;
//        sprintf(buffer, "DEC Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    } else if (matchsub("NEG", loc)) {
//        regs[regIndex].value = -regs[regIndex].value;
//        sprintf(buffer, "NEG Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    } else if (matchsub("NOT", loc)) {
//        regs[regIndex].value = ~regs[regIndex].value;
//        sprintf(buffer, "NOT Register %d Value: %d (0x%08X)\r\n", regIndex, regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    }
//
//    // Get the first source (Reg A, immediate, or memory address)
//    loc1 = getnextstring(loc, true);
//    if (!loc1) {
//        sprintf(buffer, "ERROR: Missing Source Regs A & B/#Immediate(s)\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        Glo.re++;
//        return;
//    }
//
//        // Handle IMMEDIATE (Hex or Decimal)
//    if (*loc1 == '#') {
//        if (loc1[1] == 'x' || loc1[1] == 'X') {
//            uint32_t unsignedImmediate = strtoul(loc1 + 2, NULL, 16);  // Parse as unsigned hex
//            immediateA = (int32_t)unsignedImmediate;  // Cast to signed int32_t for storage in registers
//        } else if (isdigit(loc1[1])) {
//            immediateA = atoi(loc1 + 1);  // Parse as decimal if it's numeric
//        } else {
//            sprintf(buffer, "ERROR: Invalid immediate value\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        isRegAImmediate = true;
//
//        // Handle DIRECT memory access (@addr)
//    } else if (*loc1 == '@') {
//        uint32_t address = strtoul(loc1 + 1, NULL, 0);  // Convert to address
//
//        // Check for valid address and numeric input
//        if (!isdigit(loc1[1]) && loc1[1] != 'x' && loc1[1] != 'X') {
//            sprintf(buffer, "ERROR: Invalid memory address format\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//
//        // Check for valid address and memory alignment
//        if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
//                (address >= VALID_MIN_ADDRESS2 && address <= VALID_MAX_ADDRESS2)) {
//
//            if ((address % 4) != 0) {
//                sprintf(buffer, "ERROR: Unaligned memory address 0x%08X\r\n", address);
//                UART_write(uart, buffer, strlen(buffer));
//                Glo.re++;
//                return;
//            }
//
//            // Safe memory read
//            memcpy(&immediateA, (void*)address, sizeof(int32_t));
//            isRegAImmediate = true;
//
//        } else {
//            sprintf(buffer, "ERROR: Invalid memory address 0x%08X\r\n", address);
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        // Handle INDIRECT memory access (@REG)
//    } else if (*loc1 == 'R') {
//        if (!isdigit(loc1[1])) {
//            sprintf(buffer, "ERROR: Invalid register format for indirect memory access\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        regA = atoi(loc1 + 1);  // Convert to int after confirming it's numeric
//
//        if (regA >= 0 && regA < reg_count) {
//            uint32_t address = regs[regA].value;
//
//            // Check for valid address and memory alignment
//            if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
//                    (address >= VALID_MIN_ADDRESS2 && address <= VALID_MAX_ADDRESS2)) {
//
//                if ((address % 4) != 0) {
//                    sprintf(buffer, "ERROR: Unaligned memory address 0x%08X in REG %d\r\n", address, regA);
//                    UART_write(uart, buffer, strlen(buffer));
//                    Glo.re++;
//                    return;
//                        }
//
//                // Safe memory read
//                memcpy(&immediateA, (void*)address, sizeof(int32_t));
//                isRegAImmediate = true;
//
//            } else {
//                sprintf(buffer, "ERROR: Invalid memory address 0x%08X in REG %d\r\n", address, regA);
//                UART_write(uart, buffer, strlen(buffer));
//                Glo.re++;
//                return;
//            }
//        } else {
//            sprintf    (buffer, "ERROR: Invalid Register for indirect memory access\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        // Handle Register (regA)
//    } else {
//        if (!isdigit(*loc1)) {
//            sprintf(buffer, "ERROR: Invalid Register A format\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        regA = atoi(loc1);  // Convert to int after confirming it's numeric
//        if (regA < 0 || regA >= reg_count) {
//            sprintf(buffer, "ERROR: Invalid Register A\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//    }
//
//    // Handle MOV and other operations with regA or immediateA (unchanged)
//    if (matchsub("MOV", loc) && loc1 != 0) {
//        regs[regIndex].value = isRegAImmediate ? immediateA : regs[regA].value;
//        sprintf(buffer, "MOV Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    }
//
//    // Get the second source (Reg B or immediate)
//    loc2 = getnextstring(loc1, true);
//    if (!loc2) {
//        sprintf(buffer, "ERROR: Missing Source Reg B/#Immediate \r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        Glo.re++;
//        return;
//    }
//
//    // Handle IMMEDIATE (Hex or Decimal)
//    if (*loc2 == '#') {
//        if (loc2[1] == 'x' || loc2[1] == 'X') {
//            uint32_t unsignedImmediate = strtoul(loc2 + 2, NULL, 16);  // Parse as unsigned hex
//            immediateB = (int32_t)unsignedImmediate;  // Cast to signed int32_t for storage in registers
//        } else if (isdigit(loc2[1])) {
//            immediateB = atoi(loc2 + 1);  // Parse as decimal if it's numeric
//        } else {
//            sprintf(buffer, "ERROR: Invalid immediate value\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        isRegBImmediate = true;
//        // Handle DIRECT memory access (@addr)
//    } else if (*loc2 == '@') {
//        uint32_t address = strtoul(loc2 + 1, NULL, 0);  // Convert to address
//
//        // Check for valid address and numeric input
//        if (!isdigit(loc2[1]) && loc2[1] != 'x' && loc2[1] != 'X') {
//            sprintf(buffer, "ERROR: Invalid memory address format\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//
//        // Check for valid address and memory alignment
//        if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
//                (address >= VALID_MIN_ADDRESS2 && address <= VALID_MAX_ADDRESS2)) {
//
//            if ((address % 4) != 0) {
//                sprintf(buffer, "ERROR: Unaligned memory address 0x%08X\r\n", address);
//                UART_write(uart, buffer, strlen(buffer));
//                Glo.re++;
//                return;
//            }
//
//            // Safe memory read
//            memcpy(&immediateB, (void*)address, sizeof(int32_t));
//            isRegBImmediate = true;
//
//        } else {
//            sprintf(buffer, "ERROR: Invalid memory address 0x%08X\r\n", address);
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        // Handle INDIRECT memory access (@REG)
//    } else if (*loc2 == 'R') {
//        if (!isdigit(loc2[1])) {
//            sprintf(buffer,"ERROR: Invalid register format for indirect memory access\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        regB = atoi(loc2 + 1);  // Convert to int after confirming it's numeric
//
//        if (regB >= 0 && regB < reg_count) {
//            uint32_t address = regs[regB].value;
//
//            // Check for valid address and memory alignment
//            if ((address >= VALID_MIN_ADDRESS && address <= VALID_MAX_ADDRESS) ||
//                    (address >= VALID_MIN_ADDRESS2 && address <= VALID_MAX_ADDRESS2)) {
//
//                if ((address % 4) != 0) {
//                    sprintf(buffer, "ERROR: Unaligned memory address 0x%08X in REG %d\r\n", address, regB);
//                    UART_write(uart, buffer, strlen(buffer));
//                    Glo.re++;
//                    return;
//                        }
//
//                // Safe memory read
//                memcpy(&immediateB, (void*)address, sizeof(int32_t));
//                isRegBImmediate = true;
//
//            } else {
//                sprintf(buffer, "ERROR: Invalid memory address 0x%08X in REG %d\r\n", address, regB);
//                UART_write(uart, buffer, strlen(buffer));
//                Glo.re++;
//                return;
//            }
//        } else {
//            sprintf(buffer, "ERROR: Invalid Register for indirect memory access\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//    } else {
//        if (!isdigit(*loc2)) {
//            sprintf(buffer, "ERROR: Invalid Register B format\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//        regB = atoi(loc2);  // Convert to int after confirming it's numeric
//        if (regB < 0 || regB >= reg_count) {
//            sprintf(buffer, "ERROR: Invalid Register B\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//            return;
//        }
//    }
//    // Determine the operation and perform it
//    if (matchsub("ADD", loc)) {
//        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) +
//                               (isRegBImmediate ? immediateB : regs[regB].value);
//        sprintf(buffer, "ADD Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//
//    } else if (matchsub("SUB", loc)) {
//        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) -
//                               (isRegBImmediate ? immediateB : regs[regB].value);
//        sprintf(buffer, "SUB Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//    } else if (matchsub("AND", loc)) {
//        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) &
//                               (isRegBImmediate ? immediateB : regs[regB].value);
//        sprintf(buffer, "AND Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//    } else if (matchsub("IOR", loc)) {
//        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) |
//                               (isRegBImmediate ? immediateB : regs[regB].value);
//        sprintf(buffer, "IOR Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//    } else if (matchsub("XOR", loc)) {
//        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) ^
//                               (isRegBImmediate ? immediateB : regs[regB].value);
//        sprintf(buffer, "XOR Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//    } else if (matchsub("MUL", loc)) {
//        regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) *
//                               (isRegBImmediate ? immediateB : regs[regB].value);
//        sprintf(buffer, "MUL Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//    } else if (matchsub("DIV", loc)) {
//        if ((isRegBImmediate ? immediateB : regs[regB].value) != 0) {
//            regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) /
//                                   (isRegBImmediate ? immediateB : regs[regB].value);
//            sprintf(buffer, "DIV Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//            UART_write(uart, buffer, strlen(buffer));
//        } else {
//            sprintf(buffer, "ERROR: Division by zero\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//        }
//    } else if (matchsub("REM", loc)) {
//        if ((isRegBImmediate ? immediateB : regs[regB].value) != 0) {
//            regs[regIndex].value = (isRegAImmediate ? immediateA : regs[regA].value) %
//                                   (isRegBImmediate ? immediateB : regs[regB].value);
//            sprintf(buffer, "REM Result: %d (0x%08X)\r\n", regs[regIndex].value, regs[regIndex].value);
//            UART_write(uart, buffer, strlen(buffer));
//        } else {
//            sprintf(buffer, "ERROR: Division by zero\r\n");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.re++;
//        }
//    } else if (matchsub("MAX", loc)) {
//        regs[regIndex].value = (regs[regA].value > regs[regB].value) ? regs[regA].value : regs[regB].value;
//        sprintf(buffer, "MAX Result: %d\r\n", regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//    } else if (matchsub("MIN", loc)) {
//        regs[regIndex].value = (regs[regA].value < regs[regB].value) ? regs[regA].value : regs[regB].value;
//        sprintf(buffer, "MIN Result: %d\r\n", regs[regIndex].value);
//        UART_write(uart, buffer, strlen(buffer));
//    } else {
//        sprintf(buffer, "ERROR: Invalid Operation\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        Glo.re++;
//    }
//}
