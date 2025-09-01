//////////////////////////////////////////////////////////////////////////////////
// 原作者注释：
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
// QDtech-OLED液晶驱动 for STM32
// xiao冯@ShenZhen QDtech co.,LTD
// 公司网站:www.qdtft.com
// 淘宝网站：http://qdtech.taobao.com
// wiki技术网站：http://www.lcdwiki.com
// 我司提供技术支持，任何技术问题欢迎随时交流学习
// 固话(传真) :+86 0755-23594567
// 手机:15989313508（冯工）
// 邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
// 技术支持QQ:3002773612  3002778157
// 技术交流QQ群:324828016
// 创建日期:2018/8/27
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// All rights reserved

// 改版注释：
// 针对E203处理器修改
// 测试硬件：博宸精芯ZYNQ MINI开发板，搭载Xilinx Zynq7010 SoC，SoC的FPGA软核搭载E203处理器。
// 修改人：杨以宁
// 修改时间：2024年4月

/****************************************************************************************************
// 数据总线类型为4线制SPI
//=======================================液晶屏数据线接线==========================================//
// OLED模块               	E203端口
//   D1          接        GPIOA[10]       	//OLED屏SPI写信号
//=======================================液晶屏控制线接线==========================================//
// OLED模块               	E203端口
//   RES         接        GPIOA[12]        //OLED屏复位控制信号
//   DC          接        GPIOA[13]      	//OLED屏数据/命令选择控制信号
//   D0          接        GPIOA[8]      	//OLED屏SPI时钟信号
****************************************************************************************************/

#ifndef __OLED_H
#define __OLED_H

//--------------OLED参数定义---------------------
#define PAGE_SIZE 8
#define XLevelL 0x00
#define XLevelH 0x10
#define YLevel 0xB0
#define Brightness 0xFF
#define WIDTH 128
#define HEIGHT 64

//-------------写命令和数据定义-------------------
#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据

//-----------------OLED端口定义----------------
#define SOC_DC_MASK 1 << 13  // 数据/命令控制信号  13
#define SOC_RST_MASK 1 << 12 // 复位信号           12

//-----------------OLED端口操作定义----------------
#define OLED_DC_Clr() gpio_write(GPIOA, SOC_DC_MASK, 0);
#define OLED_DC_Set() gpio_write(GPIOA, SOC_DC_MASK, 1);

#define OLED_RST_Clr() gpio_write(GPIOA, SOC_RST_MASK, 0);
#define OLED_RST_Set() gpio_write(GPIOA, SOC_RST_MASK, 1);

// OLED控制用函数
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Reset(void);
void OLED_Init(void);
void OLED_Set_Pixel(unsigned char x, unsigned char y, unsigned char color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);
#endif
