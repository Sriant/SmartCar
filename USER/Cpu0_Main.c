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
#pragma section all "cpu0_dsram"

uint8 send_buff[11]; //����ת���ڷ��͵�����
uint8 Waiting_for_parking = 0;

int core0_main(void)
{
    get_clk();//��ȡʱ��Ƶ��  ��ر���
	//�û��ڴ˴����ø��ֳ�ʼ��������

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
    //�ȴ����к��ĳ�ʼ�����
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
            //ͷ�����ֽڷ���ϵͳʱ�䣬�����Ĺ�����û��ϵͳʱ���������������Բ�����
            //����ʱ���Ŀ����Ҫ���ж�������֮���Ƿ�������û��ʱ�����Ҳû��ʲô����
            send_buff[0] = 0;
            send_buff[1] = 0;

            send_buff[2] = (int8)((int16)elec1_data[0] - 128);
            send_buff[3] = (int8)((int16)elec1_data[1] - 128);
            send_buff[4] = (int8)((int16)elec1_data[2] - 128);
            send_buff[5] = (int8)((int16)elec1_data[3] - 128);
            send_buff[6] = (int8)((int16)elec1_data[4] - 128);
            send_buff[7] = (int8)((int16)elec1_data[5] - 128);
            send_buff[8] = (int8)((int16)elec1_data[6] - 128);

            //�������ֵת��Ϊ-128��+127  ��Ϊ���ת�Ǳ�������-ANGLE_MAX��+ANGLE_MAX֮�䣬
            //��˶��ת�ǳ���ANGLE_MAX֮�����ֵ�ض���С��1�ģ�
            //Ȼ���ڳ���127���ݷ�ΧҲ��һ������������-128��+127֮���ˡ�
            send_buff[9] = (int8)((int32)turn_angle * 127 / ANGLE_MAX);
            //�̶��ֽڣ���������У����߷������ۿɱ�ÿһ�����Ƿ���ȷ
            send_buff[10] = 0x5a;
            ai_data_flag = 0;
            seekfree_wireless_send_buff(send_buff, 11);
        }
#endif
	}
}

#pragma section all restore


