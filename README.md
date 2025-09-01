# FPGA-Based Supersonic Distance Measurement with OLED and Bluetooth

该项目是一款基于FPGA（搭载Hummingbird E203 RISC-V核心）构建的多功能超声波测距系统。它采用HC-SR04传感器进行距离测量，通过OLED屏幕实时显示结果，并借助蓝牙模块将数据无线传输至移动设备。

## 核心功能 (Features)

* **实时距离测量**: 使用HC-SR04超声波传感器进行非接触式距离测量。
* **本地数据显示**: 通过SPI接口驱动OLED屏幕，实时显示测量结果。
* **无线数据传输**: 通过UART接口连接HC-06蓝牙模块，将数据发送到手机等移动终端。
* **高精度标定**: 采用**线性回归 (Linear Regression)** 数学模型对测量数据进行标定，有效修正系统误差，大幅提升准确度。
* **中断驱动接收**: 使用中断方式接收来自蓝牙的数据，实现了高效的双向通信基础。

## 所需硬件 (Hardware Required)

* FPGA开发板 (本项目基于 Nuclei DDR200T)
* HC-SR04 超声波传感器
* 0.96寸 SPI接口 OLED显示屏
* HC-06串口蓝牙模块
* 杜邦线若干

## 软件与开发环境 (Software & Environment)

* **IDE**: Nuclei Studio
* **SDK**: Hummingbird SDK
* **编程语言**: C

## 📌 关键：管脚配置 (Pin Configuration)

1. 打开 Nuclei Studio，选择工作区文件夹。

2. 创建 DDR200t 的 Helloworld 模板项目。

3. 修改SDK中的管脚定义头文件，以指定超声波传感器的`TRIG`和`ECHO`引脚。

Ⅰ.  **打开文件**:
    在您的SDK路径下，找到并打开文件：
    [cite_start]`hbird_sdk/SoC/hbirdv2/Borad/ddr200t/Include/board_ddr200t.h`

Ⅱ.  **添加宏定义**:
    [cite_start]在文件中添加以下代码行，将 `TRIG` 信号绑定到 `GPIOA[6]`，将 `ECHO` 信号绑定到 `GPIOA[11]` 

    ```c
    // 定义TRIG和ECHO信号的GPIO偏移量
    #define SOC_ECHO_OFS 11 // ECHO 绑定在 GPIOA[11]
    #define SOC_TRIG_OFS 6  // TRIG 绑定在 GPIOA[6]

    // 根据偏移量创建位掩码，方便程序进行位操作
    #define SOC_ECHO_MASK (1<<SOC_ECHO_OFS) 
    #define SOC_TRIG_MASK (1<<SOC_TRIG_OFS) 
    ```

4. 清空 main.c 文件，在 application 目录下导入项目文件夹的代码。

## 实现方法 (Methodology)

### 测距方法
为保证系统在各种信号条件下的稳定性，本项目采用**循环采样法**进行测距。程序在发送`TRIG`触发脉冲后，在一个固定的时间窗口内（40ms），以固定的时间间隔（20µs）对`ECHO`引脚进行采样，通过统计高电平的采样次数来估算回波脉冲的宽度。

### 精度优化
由于循环采样法受限于软件延时的不确定性，系统存在固有的比例误差和固定偏移误差。为解决此问题，我们采用**线性回归**模型 (`真实距离 = 测量距离 * M + C`) 对系统进行标定。通过采集多组真实距离和测量距离的数据对，利用电子表格工具的趋势线功能计算出最佳拟合的修正系数 `M` 和 `C`，并将其应用于最终的距离计算中，从而获得了高精度的测量结果。

## 如何使用 (How to Run)

1.  **硬件连接**: 按照`board_ddr200t.h`中的定义连接好超声波传感器、OLED和蓝牙模块。
2.  **修改配置**: 完成上述的“管脚配置”步骤。
3.  **编译和下载**: 使用Nuclei Studio编译本项目，并将生成的可执行文件下载到FPGA开发板。
4.  **手机连接**:
    * 在手机上打开蓝牙，搜索并配对您的蓝牙模块（如HC-06）。
    * 下载并打开任意一款“串口蓝牙终端” App。
    * 在App中连接到已配对的蓝牙设备。
5.  **查看结果**: 连接成功后，您将看到手机App持续接收到由FPGA发送的距离数据（单位：mm）。

## 项目演示 (Demonstration)

**实验装置:**
![我的实验装置图](https://imgur.com/a/h0NrGUK)

**手机接收数据:**
![手机蓝牙数据截图](https://imgur.com/a/tR68jlx)
