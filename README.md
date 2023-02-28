# 能量机关的灯效控制代码



**开发环境**

vscode + CubeMX + ozone

**烧录方式**

jlink+ozone

## 1、灯板的使用

**概述：**

灯板由两块灯板拼接而成，每块集成了210个ws2812灯珠，共420个，灯珠之间串联，两板之间需要连接电源、地和信号。两块灯板其中一块灯板集成了一块stm32f030cc芯片，引出PA6（tim3_ch1）到灯珠的信号线给出pwm信号，引出uart接口PA10和PA9分别是USART1的RX和TX,引出swd供下载使用。

![img](https://user-images.githubusercontent.com/75213185/209503396-02d6fc7e-0fba-4816-af10-369ed6955a8e.png)

![image](https://user-images.githubusercontent.com/75213185/209503421-e36e77f1-81c2-43c6-a3b0-4c207d2674ee.png)

## 2、供电

外部接口5v供电，可以点亮灯珠。Swd接口3.3v供电，仅供下载使用。



##3、内置代码

灯板内置代码已经给出，可以参考我们的代码，重写写一份灯板代码，可以移植，也可以直接用串口控制。



## 4、控制逻辑

由主控板通过串口发送控制参数，灯板接收，灯板根据接收到的参数更改ws2812数据，通过pwm控制灯板灯效，从而达到主控控制灯板的效果。



## 5、串口协议

~~~c
//简述：
    obj->buf_len = obj->config.data_len + 7;
    obj->txbuf = (uint8_t*)malloc(obj->buf_len);
    obj->txbuf[0] = 's';
    obj->txbuf[1] = (uint8_t)(obj->config.uart_identifier & 0xFF);
    obj->txbuf[2] = (uint8_t)(obj->config.uart_identifier >> 8);
    obj->txbuf[3] = obj->config.data_len;
    obj->txbuf[obj->buf_len - 1] = 'e';
// 在第五位和倒数第一位之间就是要发送的数据，是一个定义好的结构体：
typedef struct fanlight_recv_data_t {
    uint8_t color, mode;
} fanlight_recv_data;
~~~

具体内容参考代码内uart_send和uart_recv文件。也可以重写灯板代码，更改串口协议。在写好的程序中，有一个串口自发自收的控制灯板的示例，可以参照来写。

结构体中的color可以赋值0~6，有六种颜色和全灭，mode可以赋值0~3，有三种亮灯模式。