/*
 * servo_ctrl.h
 *
 *  Created on: 2021��3��27��
 *      Author: YAN
 */

#ifndef CODE_SERVO_CTRL_H_
#define CODE_SERVO_CTRL_H_

#include "common.h"

#define S_MOTOR_PIN     ATOM1_CH1_P33_9       //����������
#define ANGLE_CENTER    4600                  //GTM_ATOM1_PWM_DUTY_MAX ��Ҫ����Ϊ100000
#define ANGLE_MAX       1650

void servo_init(void);
void turn(void);
void servo_control(uint16 *elec);
void island_handle(uint16 *elec);
void lose_line(uint16 *elec);

extern int16 turn_angle;//������Ʊ���
extern float error;
extern uint16 island_T;
extern uint8 leftright_island;

extern _pid spid;
extern uint8 dir;
extern uint8 back;
#endif /* CODE_SERVO_CTRL_H_ */
