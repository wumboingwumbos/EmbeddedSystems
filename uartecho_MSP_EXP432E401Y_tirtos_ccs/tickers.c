//#include "ph001.h"
//
//void MPTicker(char *input){
//    char buffer[BUFFER_SIZE];
//    size_t buffer_size;
//    char *loc;
//    int index = -1, delay = 0, count = 0, period = 0, tic = 0;
//    int i;
//    char payload[MSGLENGTH];
//
//    loc = getnextstring(input, true);
//    if(!loc){
//        index = -1;
//    }
//    else if(loc[0] == 'c'){
//        snprintf(buffer, buffer_size, "Tickers Cleared\r\n");
//
//        UART_write(uart, buffer, strlen(buffer));
////        for(i = 0; i < ticker_count; i++){
////            index = create_ticker(i, 0, 0, 0, "");
////            snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d           \r\n",
////                     i, tickers[i].delay, tickers[i].period, tickers[i].count);
////            UART_write(uart, buffer, strlen(buffer));
////        }
//        sprintf(buffer, "\r\n");
//        return;
//
//
//    }else{
//        if(isdigit(*loc) == 0 && *loc != '-'){
//            snprintf(buffer, buffer_size, "ERROR: Invalid digit");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.ide++;
//            return;
//
//        }
//        index = atoi(loc);
//
//    }
//    if(index < 0 || index >= ticker_count){
//        snprintf(buffer, buffer_size, "-----------------------------------\r\n");
//        UART_write(uart, buffer, strlen(buffer));
//        for(i = 0; i < ticker_count; i++){
//            snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d \r\n",
//                     i, tickers[i].delay, tickers[i].period, tickers[i].count);
//            UART_write(uart, buffer, strlen(buffer));
//        }
//        sprintf(buffer, "\r\n");
//        return;
//
//    }
//
//
//    loc = getnextstring(loc, true);
//    if(!loc){
//        snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d \r\n",
//                 index, tickers[index].delay, tickers[index].period, tickers[index].count);
//        UART_write(uart, buffer, strlen(buffer));
//        return;
//    }else if(loc[0] == 'c'){
//        index = create_ticker(index, 0, 0, 0, "");
//        snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d          \r\n",
//                         index, tickers[index].delay, tickers[index].period, tickers[index].count);
//        UART_write(uart, buffer, strlen(buffer));
//                return;
//    }
//    if(isdigit(*loc) == 0){
//        snprintf(buffer, buffer_size, "ERROR: Invalid Digit");
//        Glo.ide++;
//        return;
//    }
//    delay = atoi(loc);
//    strcpy(payload, "");
//    // conditions for the delay limits
//    loc = getnextstring(loc, true);
//
//    if(!loc){
//        goto step1;
//    }
//    if(isdigit(*loc) == 0){
//            snprintf(buffer, buffer_size, "ERROR: Invalid Digit");
//            UART_write(uart, buffer, strlen(buffer));
//            Glo.ide++;
//            return;
//        }
//    period = atoi(loc);
//    loc = getnextstring(loc, true);
//
//    if(!loc){
//           goto step1;
//       }
//    if(isdigit(*loc) == 0 && *loc != '-'){
//        snprintf(buffer, buffer_size, "ERROR: Invalid Digit");
//        UART_write(uart, buffer, strlen(buffer));
//        Glo.ide++;
//        return;
//
//           }
//    count = atoi(loc);
//
//    loc = getnextstring(loc, true);
//
//    if(loc)
//        strcpy(payload, loc);
//
//step1:
//    index = create_ticker(index, delay, period, count, payload);
//    snprintf(buffer, buffer_size, "TICKER %d Delay: %d Period: %d Count: %d %s\r\n",
//                     index, tickers[index].delay, tickers[index].period, tickers[index].count, tickers[index].payload);
//    UART_write(uart, buffer, strlen(buffer));
//}
///// Create the ticker
//
//int create_ticker(int index, int delay, int period, int count, char *payload){
//
//    int32_t gateKey;
//
//    gateKey = GateSwi_enter(gateSwi1);
//    tickers[index].delay = delay;
//    tickers[index].period = period;
//    tickers[index].count = count;
//    strcpy(tickers[index].payload, payload);
//
//    GateSwi_leave(gateSwi1, gateKey);
//    return index;
//
//}//END
//
//
//void TIMER1SWI(){
//    int i;
//    int32_t gateKey;
//    gateKey = GateSwi_enter(gateSwi1);
//    for(i = 0; i < ticker_count; i++){
//        if(tickers[i].delay > 0){
//            tickers[i].delay--;
//            if(tickers[i].delay <=0){
//                if(tickers[i].count != 0){
//                    tickers[i].delay = tickers[i].period;
//                    if(tickers[i].count > 0){
//                        tickers[i].count--;
//                    }
//                }
//                enqueueMessage(tickers[i].payload);
//            }
//        }
//    }
//    GateSwi_leave(gateSwi1, gateKey);
//}
//
//void SWI_Timer1(){
//    TIMER1SWI();
//}
//void Timer1Callback(){
//    Swi_post(swi3);
//}
//
