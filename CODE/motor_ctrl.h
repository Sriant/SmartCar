/*
 * motor_ctrl.h
 *
 *  Created on: 2021年3月12日
 *      Author: YAN
 */

#ifndef CODE_MOTOR_CTRL_H_
#define CODE_MOTOR_CTRL_H_

#include "common.h"

#define MOTOR_DIR   P21_2               //定义电机方向控制引脚
#define MOTOR_PWM   ATOM0_CH1_P21_3     //定义电机PWM引脚

#define ON          1
#define OFF         0

void motor_init(void);
void mpid_param_init(void);
void motor_control(void);

extern _pid mpid;
extern int16 cur_pulse;
extern int32 inte_pulse;
extern int16 speed_power;
extern uint8 car_on;
extern int16 linear_speed;
extern int16 curve_speed;

#endif /* CODE_MOTOR_CTRL_H_ */
