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
/***************************************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 ****************************************************************************************************/
#ifndef __GUI_H__
#define __GUI_H__
#include <stdint.h>

void GUI_DrawPoint(uint8_t x, uint8_t y, uint8_t color);
void GUI_Fill(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey, uint8_t color);
void GUI_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void GUI_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void GUI_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void GUI_DrawCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r);
void GUI_FillCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r);
void GUI_DrawTriangel(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void GUI_FillTriangel(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void GUI_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, uint8_t mode);
void GUI_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t Size, uint8_t mode);
void GUI_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, uint8_t mode);
void GUI_ShowFont16(uint8_t x, uint8_t y, uint8_t *s, uint8_t mode);
void GUI_ShowFont24(uint8_t x, uint8_t y, uint8_t *s, uint8_t mode);
void GUI_ShowFont32(uint8_t x, uint8_t y, uint8_t *s, uint8_t mode);
void GUI_ShowCHinese(uint8_t x, uint8_t y, uint8_t hsize, uint8_t *str, uint8_t mode);
void GUI_DrawBMP(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t BMP[], uint8_t mode);
#endif
