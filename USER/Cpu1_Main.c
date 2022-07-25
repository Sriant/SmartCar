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
#pragma section all "cpu1_dsram"
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中

extern const unsigned char model1[];

typedef struct _model_info_struct
{
    char author[30];
    int quant_bits;
    int frac_bits;
    const char* name_string;
}model_info_struct;
model_info_struct inf;

int16 temp;
int16 servo_value;

extern void get_model_info(const void *in_buf, model_info_struct *inf);
extern void* run_model(const void *model_buf, const void *in_buf, signed short int *out1_buf);


void core1_main(void)
{
	disableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    //用户在此处调用各种初始化函数等


    get_model_info(model1, &inf);


	//等待所有核心初始化完毕
	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();
    while (TRUE)
    {
        if(gpio_get(SW1) == 1)
        {
            if(ai_data_flag)
            {
                systick_start(STM1);

                //将采集到的电感数据送入模型进行转角计算
                run_model(model1, elec_ai, &temp);

                //根据模型信息对转角进行位移
                servo_value = temp >> (inf.quant_bits - inf.frac_bits - 1);

                //训练模型时，转角是被压缩到了-128至127， 压缩时使用的公式为*128/ANGLE_MAX，所以这里需要*ANGLE_MAX/128进行还原，然后再用于转角控制
                turn_angle = (int16)(servo_value * (int32)ANGLE_MAX / 127);

                turn();//控制舵机转向

                island_handle(e1_nor);
                lose_line(e1_nor);

                ai_data_flag = 0;
            }
        }
        else if(gpio_get(SW1) == 0)
        {
            if(ai_data_flag)
            {
                servo_control(e1_nor);
                ai_data_flag = 0;
            }
        }
    }
}

#pragma section all restore
