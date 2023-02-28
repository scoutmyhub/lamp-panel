#ifndef _H_FANLIGHT_H
#define H_FANLIGHT_H
#include "stdint.h"
#include "ws2812b.h"
#include "uart_recv.h"
#include "uart_send.h"
#define COLOR_SIZE 420

typedef struct fanlight_recv_data_t {
    uint8_t color, mode;
} fanlight_recv_data;

typedef struct fanlight_t {
    ws2812* light;
    fanlight_recv_data* data;
    fanlight_recv_data last_data;
    uart_recv* uart_recv_com;
    uart_send* uart_send_com;
    color_rgb colors[COLOR_SIZE];
    int start_index_block;
    int flow_lines;
    int flow_direction;
    uint8_t (* mask_array)[7];
} fanlight;

fanlight* Fanlight_APP_Init();

void FanLight_Update(fanlight* obj);
#endif