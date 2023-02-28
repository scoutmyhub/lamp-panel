#include "app.h"

#include "bsp.h"
#include "hal.h"
#include "stdio.h"
#include "ws2812b.h"

// 定义APP
fanlight* fan;

void APP_Layer_Init() {
    fan = Fanlight_APP_Init();
}

void APP_Layer_default_loop() {
    FanLight_Update(fan);
}
