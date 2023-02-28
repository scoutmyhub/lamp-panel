#include "fanlight.h"

#include "stdlib.h"
uint8_t on_mask[1][7] = {
    {1, 1, 1, 1, 1, 1, 1},
};
uint8_t off_mask[1][7] = {
    {0, 0, 0, 0, 0, 0, 0},
};
uint8_t flow_mask[8][7] = {
    {1, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 1, 1},
    {1, 1, 1, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};
uint8_t flow_mask_hit[59][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 1, 0, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {1, 1, 1, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

uint8_t equal(fanlight_recv_data *a, fanlight_recv_data *b) {
    return a->color == b->color && a->mode == b->mode;
}

fanlight *Fanlight_APP_Init() {
    fanlight *obj = (fanlight *)malloc(sizeof(fanlight));
    ws2812_config fanconfig;
    fanconfig.max_len = COLOR_SIZE;
    fanconfig.pwm_id = 0;
    obj->light = ws2812_create(&fanconfig);

    uart_recv_config recv_config;
    recv_config.bsp_uart_index = 0;
    recv_config.data_len = sizeof(fanlight_recv_data);
    recv_config.uart_identifier = 0;
    recv_config.notify_func = NULL;
    obj->uart_recv_com = UartRecv_Create(&recv_config);
    obj->data = (fanlight_recv_data *)obj->uart_recv_com->data_rx.data;
    
    uart_send_config send_config;
    send_config.bsp_uart_index = 0;
    send_config.data_len = sizeof(fanlight_recv_data);
    send_config.uart_identifier = 0;
    obj->uart_send_com = UartSend_Create(&send_config);
    

    obj->start_index_block = 0;
    obj->flow_direction = 0;
    obj->flow_lines = 0;
    obj->mask_array = off_mask;
    obj->last_data =(fanlight_recv_data){0, 0};
    *(obj->data)= (fanlight_recv_data){0, 0};

    obj->data->color = 1; 
    obj->data->mode = 3; 
    return obj;
}

void FanLight_Update(fanlight *obj) {

    // //自发自收
    // fanlight_recv_data send;
    // send.color = 1;
    // send.mode = 1;
    // UartSend_Send(obj->uart_send_com, (uint8_t*)(&send));
    
    static color_rgb chs_color = (color_rgb){0, 0, 0};
    if (!equal(obj->data, &obj->last_data)) {
        obj->last_data = *(obj->data);
        obj->start_index_block = 0;
        if (obj->data->mode == 1) {
            obj->flow_direction = 0;
            obj->flow_lines = 8;
            obj->mask_array = flow_mask;
        } else if (obj->data->mode == 2) {
            obj->flow_direction = 0;
            obj->flow_lines = 1;
            obj->mask_array = on_mask;
        } else if (obj->data->mode == 3) {
            obj->flow_direction = 1;
            obj->flow_lines = 59;
            obj->mask_array = flow_mask_hit;
        } else {
            obj->flow_direction = 0;
            obj->mask_array = off_mask;
            obj->flow_lines = 1;
        }
        if (obj->data->color == 1) {
            chs_color = red;  
        } else if (obj->data->color == 2) {
            chs_color = blue;
        } else if (obj->data->color == 3) {
            chs_color = orange;
        } else if (obj->data->color == 4) {
            chs_color = cyan;
        } else if (obj->data->color == 5) {
            chs_color = white;
        } else if (obj->data->color == 6) 
            chs_color = green;
        else
            chs_color = off;
        }
    if (obj->flow_direction) {
        obj->start_index_block++;
        if (obj->start_index_block > obj->flow_lines) obj->start_index_block -= obj->flow_lines;
    } else {
        obj->start_index_block--;
        if (obj->start_index_block < 0) obj->start_index_block += obj->flow_lines;
    }

    for (int i = 0; i < 60; ++i) {
        int in_block_index = (obj->start_index_block + i) % obj->flow_lines;
        for (int col = 0; col < 7; ++col) {
            uint8_t mask_now = obj->mask_array[in_block_index][col];
            if (i % 2)
                obj->colors[i * 7 + col] = mask_now ? chs_color : off;
            else
                obj->colors[i * 7 + 6 - col] = mask_now ? chs_color : off;
        }
    }  
    ws2812_set_array(obj->light, obj->colors, 420);
    //ws2812_set_all(obj->light, blue);
}