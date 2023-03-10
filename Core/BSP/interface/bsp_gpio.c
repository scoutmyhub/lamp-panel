#include "bsp_gpio.h"
#include "gpio.h"

#define DEVICE_GPIO_CNT 2

typedef struct BSP_GPIO_Typedef_t {
    GPIO_TypeDef *base;
    uint16_t pin;
} BSP_GPIO_Typedef;

BSP_GPIO_Typedef gpio_ports[DEVICE_GPIO_CNT];

void BSP_GPIO_Init(){
    // gpio_ports[0].base = GPIOA;
    // gpio_ports[0].pin = GPIO_PIN_4;

    // gpio_ports[1].base = GPIOB;
    // gpio_ports[1].pin = GPIO_PIN_0;
}

void BSP_GPIO_Set(uint8_t gpio_index,uint8_t status){
    HAL_GPIO_WritePin(gpio_ports[gpio_index].base,gpio_ports[gpio_index].pin,status);
    
}

void BSP_GPIO_Read(uint8_t gpio_index,uint8_t* data){
    (*data) = HAL_GPIO_ReadPin(gpio_ports[gpio_index].base,gpio_ports[gpio_index].pin);
}