/*
 * motor_ctrl.c
 *
 *  Created on: 2021年3月12日
 *      Author: YAN
 */
#include "headfile.h"
#include "motor_ctrl.h"

#define SPEED_MAX   9000

int16 cur_pulse = 0;      //当前编码器计数值
int32 inte_pulse = 0;     //单位时间内编码器计数和

_pid mpid;
uint8 car_on = ON;
int16 speed_power = 3000;

int16 linear_speed = 3500;
int16 curve_speed = 3000;
/**
  * @brief  电机驱动初始化
    *   @note   无
  * @retval 无
  */
void motor_init(void)
{
    /* 编码器 */
    gpt12_init(GPT12_T2, GPT12_T2INB_P33_7, GPT12_T2EUDB_P33_6);

    /* 电机PWM，方向IO */
    gtm_pwm_init(MOTOR_PWM, 17000, speed_power);
    gpio_init(MOTOR_DIR, GPO, 0, PUSHPULL);
    mpid_param_init();
}
/**
  * @brief  电机pid参数初始化
    *   @note   无
  * @retval 无
  */
void mpid_param_init(void)
{
    /* 初始化参数 */
    mpid.target_val = 3500;
    mpid.actual_val = 0.0;
    mpid.err = 0.0;
    mpid.err_last = 0.0;
    mpid.err_next = 0.0;

    mpid.Kp = 0.6;
    mpid.Ki = 0.4;
    mpid.Kd = 0.2;
}
/**
  * @brief  电机转动
  * @@param  无
    *   @note   无
  * @retval 无
  */
void run(void)
{
    /* 限幅 */
    speed_power = limit_ab(speed_power, 0, SPEED_MAX);

    if(0 <= speed_power)  //电机正转
    {
        pwm_duty(MOTOR_PWM, speed_power);
        gpio_set(MOTOR_DIR, 0);
    }
    else                  //电机反转
    {
        pwm_duty(MOTOR_PWM, -speed_power);
        gpio_set(MOTOR_DIR, 1);
    }
}
/**
  * @brief  电机驱动控制
  * @@param  无
    *   @note   无
  * @retval 无
  */
void motor_control(void)
{
    if(car_on)
    {
        if((e1_nor[1] + e1_nor[5]) < 20)
            mpid.target_val = linear_speed;
        else if(fabs(turn_angle) > 550 && leftright_island == 0)
            mpid.target_val = curve_speed;
        /* PID闭环控制 */
        speed_power = (int16)( pid_inc_realize(mpid, fabs(cur_pulse)) );

        run();
    }
    else
        pwm_duty(MOTOR_PWM, 0);
}
