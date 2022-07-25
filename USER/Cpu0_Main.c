/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/


#include "headfile.h"
#pragma section all "cpu0_dsram"

uint8 send_buff[11]; //无线转串口发送的数组
uint8 Waiting_for_parking = 0;

int core0_main(void)
{
    get_clk();//获取时钟频率  务必保留
	//用户在此处调用各种初始化函数等

    seekfree_wireless_init();
    elec_init();
    servo_init();
    motor_init();
    lcd_init();
    key_init();
    gpio_init(P33_10, GPO, 0, PUSHPULL);
    gpio_init(P14_1,GPI,1,PULLUP);

    pit_interrupt_ms(CCU6_0, PIT_CH0, 2);
    pit_interrupt_ms(CCU6_0, PIT_CH1, 5);
    pit_interrupt_ms(CCU6_1, PIT_CH0, 100);
    //等待所有核心初始化完毕
	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
	enableInterrupts();


	while (TRUE)
	{
        key_task();
        if(Waiting_for_parking == 1)
        {
            if(gpio_get(P14_1) == 0)
                car_on = OFF;
        }
//        ANO_DT_Send_uint16(elec1_data, 7);
#if 0
        if(ai_data_flag)
        {
            //头两个字节发送系统时间，如果你的工程里没有系统时间这个参数，则可以不发送
            //发送时间的目的主要是判断两个包之间是否连续，没有时间参数也没有什么问题
            send_buff[0] = 0;
            send_buff[1] = 0;

            send_buff[2] = (int8)((int16)elec1_data[0] - 128);
            send_buff[3] = (int8)((int16)elec1_data[1] - 128);
            send_buff[4] = (int8)((int16)elec1_data[2] - 128);
            send_buff[5] = (int8)((int16)elec1_data[3] - 128);
            send_buff[6] = (int8)((int16)elec1_data[4] - 128);
            send_buff[7] = (int8)((int16)elec1_data[5] - 128);
            send_buff[8] = (int8)((int16)elec1_data[6] - 128);

            //将舵机数值转换为-128至+127  因为舵机转角被限制在-ANGLE_MAX至+ANGLE_MAX之间，
            //因此舵机转角除以ANGLE_MAX之后绝对值必定是小于1的，
            //然后在乘以127数据范围也就一定被限制在了-128至+127之间了。
            send_buff[9] = (int8)((int32)turn_angle * 127 / ANGLE_MAX);
            //固定字节，可以用于校验或者方便肉眼可辨每一个包是否正确
            send_buff[10] = 0x5a;
            ai_data_flag = 0;
            seekfree_wireless_send_buff(send_buff, 11);
        }
#endif
	}
}

#pragma section all restore


