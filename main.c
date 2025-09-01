#include <stdio.h>
#include "hbird_sdk_hal.h"
#include "oled.h"
#include "gui.h"

// =============================================================================
//                             蓝牙/UART2 功能模块
// =============================================================================

#define RCV_BUFFER_SIZE 1 //UART通信，当缓冲区有一个字节的数据时，就触发一次中断

// 函数声明
void gpio_uart2_config(void);
void sendBluetoothChar(uint8_t item);
void sendBluetoothString(const char *items); // 参数类型改为 const char* 更标准
void UART2_IRQHandler(void);

/* 接收中断服务函数 ISR */
void UART2_IRQHandler(void) {
    char rcv = '\0';
    rcv = uart_read(UART2);
    while (rcv != '\0') {	//只要非空就一直读
        printf("Received from BT: %c\n", rcv); // 打印从手机蓝牙收到的数据
        rcv = uart_read(UART2);
    }
}

/* 配置GPIO为UART2功能 */
void gpio_uart2_config(void) {
    gpio_iof_config(GPIOA, IOF_UART2_MASK);
    uart_init(UART2, 9600);
    uart_set_rx_th(UART2, RCV_BUFFER_SIZE);
    //请UART2硬件在每接收到1个字节的数据后，就立刻向CPU发送一个中断请求信号
    uart_enable_rx_th_int(UART2);
}

/* 通过UART2发送单个字节 一次一位 串行接口 */
void send_uart2_byte(uint8_t data) {
    if (data == '\n') {
        uart_write(UART2, '\r');
    }
    uart_write(UART2, data);
}

/* 通过蓝牙发送单个字符 */
void sendBluetoothChar(uint8_t item) {
    send_uart2_byte(item);
}

/* 通过蓝牙发送字符串 */
void sendBluetoothString(const char *items) {
    while (*items) {
        sendBluetoothChar(*items);
        items++;
    }
}


// =============================================================================
//                             超声波测距功能模块
// =============================================================================

// us级延时函数
void delayMicro(unsigned int us){
    uint64_t count = (16 * us) / 3;	//16MHz的频率，一条指令就是1/16us，加上修正
    for(unsigned int i = 0; i<count; i++) {
        __asm__("nop"); //CPU 空转延时
    }
}

/* 经过标定的稳定版测距函数 */
int get_distance_calibrated() {
    gpio_write(GPIOA, SOC_TRIG_MASK, 0);	//先低电平
    delayMicro(5);
    gpio_write(GPIOA, SOC_TRIG_MASK, 1);	//TRIG接受高电平，至少10us
    delayMicro(20);
    gpio_write(GPIOA, SOC_TRIG_MASK, 0);
    delayMicro(300);	//需要一点时间来准备并发射出一串8个周期的40kHz声波

    int high_level_samples = 0;
    //传感器分辨率为3mm，约为17.7us
    const int sample_interval_us = 20;
    //Echo 引脚的高电平时间范围在 117μs~23529μs
    //“监听窗口”时长 20us * 2000 = 40000us = 40ms，以确保能覆盖传感器的最大量程
    const int total_samples = 2000;

    for (int i = 0; i < total_samples; i++) {
        if (gpio_read(GPIOA, SOC_ECHO_MASK) != 0) {
            high_level_samples++;	//如果当前是高电平，那么计数器就加1
        }
        delayMicro(sample_interval_us);
    }

    float duration_us = (float)high_level_samples * sample_interval_us;
    int distance_mm = (int)(duration_us * 0.343f / 2.0f);

    // 应用我们最终确定的线性回归修正模型
    const float M = 1.49f;
    const float C = 2.82f;
    int calibrated_distance_mm = (int)(distance_mm * M + C);

    return calibrated_distance_mm;
}


// =============================================================================
//                             硬件初始化
// =============================================================================

// 用于OLED和超声波传感器的GPIO初始化
void gpio_oled_sensor_config(void) {
    // 为超声波传感器配置GPIO
    gpio_enable_input(GPIOA, SOC_ECHO_MASK);
    gpio_enable_output(GPIOA, SOC_TRIG_MASK);

    // 为OLED屏幕配置GPIO
    gpio_enable_output(GPIOA, SOC_LED_0_GPIO_MASK);
    gpio_iof_config(GPIOA, IOF_SPI_MASK); // SPI引脚
    gpio_enable_output(GPIOA, SOC_DC_MASK);
    gpio_enable_output(GPIOA, SOC_RST_MASK);
}

void spi_config() {
    spi_setup_clk(SPI1, 0x4);
}

// =============================================================================
//                             主函数
// =============================================================================
int main(void) {
    // --- 1. 统一硬件初始化 ---
    spi_config();                   // 初始化SPI (为OLED)
    gpio_oled_sensor_config();      // 初始化OLED和超声波的GPIO
    gpio_uart2_config();            // 初始化UART2 (为蓝牙)
    OLED_Init();                    // 初始化OLED屏幕
    OLED_Clear(0);

    // --- 2. 中断设置 ---
    //如果将来有来自UART2的中断信号，请执行UART2_IRQHandler这个函数
    PLIC_Register_IRQ(PLIC_UART2_IRQn, 1, UART2_IRQHandler);
    __enable_irq();

    printf("System init complete. Bluetooth and Sensor are ready.\n");

    // --- 3. 准备显示和发送 ---
    //GUI_ShowString(int x, int y, const char *text, int font_size, int color);
    GUI_ShowString(24, 0, "Measurement", 16, 1);
    //GUI_DrawLine(int x1, int y1, int x2, int y2, int color);
    GUI_DrawLine(0, 17, 127, 17, 1);

    // 延时确保蓝牙模块启动完成
    delay_1ms(500);
    sendBluetoothString("Hello! Supersonic sensor is ready.\n");

    // --- 4. 进入主循环：测量、显示、并通过蓝牙发送 ---
    while (1) {
        // (1) 调用测距函数获取距离
        int distance = get_distance_calibrated();

        // (2) 准备用于显示和发送的字符串缓冲区
        char oled_buffer[32];
        char bluetooth_buffer[32];

        sprintf(oled_buffer, "Dist: %5d mm", distance);
        sprintf(bluetooth_buffer, "%d\n", distance);

        // (3) 在OLED上显示距离
        GUI_ShowString(8, 24, oled_buffer, 16, 1);

        // (4) 在串口终端打印距离 (用于调试)
        printf("Measured Distance: %d mm\n", distance);

        // (5) 通过蓝牙将距离发送到手机
        sendBluetoothString(bluetooth_buffer);

        // (6) 延时，控制发送频率
        delay_1ms(500); // 每500ms发送一次数据
    }

    return 0;
}
