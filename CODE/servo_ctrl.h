/*
 * servo_ctrl.h
 *
 *  Created on: 2021年3月27日
 *      Author: YAN
 */

#ifndef CODE_SERVO_CTRL_H_
#define CODE_SERVO_CTRL_H_

#include "common.h"

#define S_MOTOR_PIN     ATOM1_CH1_P33_9       //定义舵机引脚
#define ANGLE_CENTER    4600                  //GTM_ATOM1_PWM_DUTY_MAX 需要设置为100000
#define ANGLE_MAX       1650

void servo_init(void);
void turn(void);
void servo_control(uint16 *elec);
void island_handle(uint16 *elec);
void lose_line(uint16 *elec);

extern int16 turn_angle;//舵机控制变量
extern float error;
extern uint16 island_T;
extern uint8 leftright_island;

extern _pid spid;
extern uint8 dir;
extern uint8 back;
#endif /* CODE_SERVO_CTRL_H_ */
