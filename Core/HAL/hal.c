#include "hal.h"
#include "crc16.h"
#include "bsp_delay.h"
#include "monitor.h"
#include "uart_recv.h"
//#include "motor.h"

void HAL_Layer_Init(){
    CRC16_Init();
    UartRecv_Driver_Init();
    Monitor_Init();
}

void HAL_Layer_Default_Loop(){

}