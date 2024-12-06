//#include "ph001.h"
//const uint16_t SINETABLE[SINE_TABLE_SIZE+1] = {
// 8192,
// 8393,
// 8594,
// 8795,
// 8995,
// 9195,
// 9394,
// 9593,
// 9790,
// 9987,
// 10182,
// 10377,
// 10570,
// 10762,
// 10952,
// 11140,
//11327,
//11512,
//11695,
//11875,
//12054,
//12230,
//12404,
//12575,
//12743,
//12909,
//13072,
//13232,
//13389,
//13543,
//13693,
//13841,
//13985,
//14125,
//14262,
//14395,
//14525,
//14650,
//14772,
//14890,
//15003,
//15113,
//15219,
//15320,
//15417,
//15509,
//15597,
//15681,
//15760,
//15835,
//15905,
//15971,
//16031,
//16087,
//16138,
//16185,
//16227,
//16263,
//16295,
//16322,
//16345,
//16362,
//16374,
//16382,
//16383,
//16382,
//16374,
//16362,
//16345,
//16322,
//16295,
//16263,
//16227,
//16185,
//16138,
//16087,
//16031,
//15971,
//15905,
//15835,
//15760,
//15681,
//15597,
//15509,
//15417,
//15320,
//15219,
//15113,
//15003,
//14890,
//14772,
//14650,
//14525,
//14395,
//14262,
//14125,
//13985,
//13841,
//13693,
//13543,
//13389,
//13232,
//13072,
//12909,
//12743,
//12575,
//12404,
//12230,
//12054,
//11875,
//11695,
//11512,
//11327,
//11140,
//10952,
//10762,
//10570,
//10377,
//10182,
// 9987,
// 9790,
// 9593,
// 9394,
// 9195,
// 8995,
// 8795,
// 8594,
// 8393,
// 8192,
// 7991,
// 7790,
// 7589,
// 7389,
// 7189,
// 6990,
// 6791,
// 6594,
// 6397,
// 6202,
// 6007,
// 5814,
// 5622,
// 5432,
// 5244,
// 5057,
// 4872,
// 4689,
// 4509,
// 4330,
// 4154,
// 3980,
// 3809,
// 3641,
// 3475,
// 3312,
// 3152,
// 2995,
// 2841,
// 2691,
// 2543,
// 2399,
// 2259,
// 2122,
// 1989,
// 1859,
// 1734,
// 1612,
// 1494,
// 1381,
// 1271,
// 1165,
// 1064,
//  967,
//  875,
//  787,
//  703,
//  624,
//  549,
//  479,
//  413,
//  353
//,  297,
//  246,
//  199,
//  157,
//  121,
//   89,
//   62,
//   39,
//   22,
//   10,
//    2,
//    0,
//    2,
//   10,
//   22,
//   39,
//   62,
//   89,
// 121,
//  157,
//  199,
//  246,
//  297,
//  353,
//  413,
//  479,
//  549,
//  624,
//  703,
//  787,
//  875,
//  967,
// 1064,
// 1165,
// 1271,
// 1381,
// 1494,
// 1612,
// 1734,
// 1859,
// 1989,
// 2122,
// 2259,
//
// 2399,
// 2543,
// 2691,
// 2841,
// 2995,
// 3152,
// 3312,
// 3475,
// 3641,
// 3809,
// 3980,
// 4154,
// 4330,
// 4509,
// 4689,
// 4872,
// 5057,
// 5244,
// 5432,
// 5622,
// 5814,
// 6007,
// 6202,
// 6397,
// 6594,
// 6791,
// 6990,
// 7189,
// 7389,
// 7589,
// 7790,
// 7991,
// 8192};
//
//void MPSin(char *input)
//{
//    char *loc;
//    double lowerweight, upperweight, intermediateINT;
//    uint32_t lowerindex, upperindex;
//    uint16_t outputPower;
//    uint32_t freq;
//    char printer[50] = {0};
//    SPI_Transaction spiTransaction;
//    bool transferOK;
//
//    loc = getnextstring(input, true);
//    sscanf(loc, "%d", &freq);
//
//    /*Nyquist stuff */
//    if(freq >= 4000){
//        sprintf(printer, "timer 0");
//        MPtimer(printer);
//        sprintf(printer, "-callback 0 0 0");
//        MPCallback(printer);
////        enqueueMessage(printer);
//        Glo.incrementLookupTable = 0;
//        Glo.position = 0;
//    }
//
//    else if (input[0] != '\0' && freq >= 1 && Glo.Timer0Period > 0){
//        sprintf(printer, "-timer 125");
//
//        MPtimer(printer);
//        Glo.incrementLookupTable = (double) freq * (double) 256 * (double) Glo.Timer0Period / 1000000.;
//        sprintf(printer, "-callback 0 -1 -sine");
//        MPCallback(printer);
////        enqueueMessage(printer);
//        //UART_write(uart, '\r\n', 2);
//    }
//
//    lowerindex = (uint32_t)Glo.position;
//    upperindex = (lowerindex + 1) % LUTSIZE;
//    lowerweight = 1.0 - (Glo.position - (double) lowerindex);
//    upperweight = 1.0 - lowerweight;
//    intermediateINT = ((double)(SINETABLE[lowerindex] * lowerweight) + ((double) SINETABLE[upperindex] * upperweight));
//    outputPower = (int) intermediateINT;
//
//    spiTransaction.count = 1;
//    spiTransaction.txBuf = &outputPower;
//    spiTransaction.rxBuf = NULL;
//
//    transferOK = SPI_transfer(Glo.spi, &spiTransaction);
//    if (!transferOK){
//        while (1);
//    }
//    Glo.position += Glo.incrementLookupTable;
//    while(Glo.position >=  LUTSIZE){
//        Glo.position -=  LUTSIZE;
//    }
//}
//
