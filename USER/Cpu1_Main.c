/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/

#include "headfile.h"
#pragma section all "cpu1_dsram"
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��

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
    //�û��ڴ˴����ø��ֳ�ʼ��������


    get_model_info(model1, &inf);


	//�ȴ����к��ĳ�ʼ�����
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

                //���ɼ����ĵ����������ģ�ͽ���ת�Ǽ���
                run_model(model1, elec_ai, &temp);

                //����ģ����Ϣ��ת�ǽ���λ��
                servo_value = temp >> (inf.quant_bits - inf.frac_bits - 1);

                //ѵ��ģ��ʱ��ת���Ǳ�ѹ������-128��127�� ѹ��ʱʹ�õĹ�ʽΪ*128/ANGLE_MAX������������Ҫ*ANGLE_MAX/128���л�ԭ��Ȼ��������ת�ǿ���
                turn_angle = (int16)(servo_value * (int32)ANGLE_MAX / 127);

                turn();//���ƶ��ת��

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
