/*
 * mpid_ctrl.c
 *
 *  Created on: 2021年3月12日
 *      Author: YAN
 */
#include "headfile.h"
#include "pid_ctrl.h"
/**
  * @brief  设置目标值
  * @param       pid：目标pid参数
  * @param  temp_val：目标值
    *   @note   无
  * @retv0al 无
  */
void set_pid_target(_pid pid, float temp_val)
{
    pid.target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  pid：目标pid参数
    *   @note   无
  * @retval 目标值
  */
float get_pid_target(_pid pid)
{
    return pid.target_val;
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  pid：目标pid参数
  * @param    p：比例系数 P
  * @param    i：积分系数 i
  * @param    d：微分系数 d
    *   @note   无
  * @retval 无
  */
void set_p_i_d(_pid pid, float p, float i, float d)
{
    pid.Kp = p;
    pid.Ki = i;
    pid.Kd = d;
}

/**
  * @brief  增量式pid算法实现
  * @param       pid：目标pid参数
  * @param  temp_val：目标值
    *   @note   无
  * @retval 通过pid计算后的输出
  */
float pid_inc_realize(_pid pid, float temp_val)
{
    /*传递误差*/
    pid.err_last = pid.err_next;
    pid.err_next = pid.err;

    /*计算目标值与实际值的误差*/
    pid.err = pid.target_val - temp_val;

    /*pid算法实现*/
    pid.actual_val += pid.Kp * (pid.err - pid.err_next)
                   +  pid.Ki *  pid.err
                   +  pid.Kd * (pid.err - 2 * pid.err_next + pid.err_last);

    return pid.actual_val;
}
/**
  * @brief  位置式pid算法实现
  * @param       pid：目标pid参数
  * @param  temp_val：目标值
    *   @note   此处注释部分调整为PD运算
  * @retval 通过pid计算后的输出
  */
float pid_pos_realize(_pid pid, float temp_val)
{
    /*计算目标值与实际值的误差*/
    pid.err = pid.target_val - temp_val;

    /*误差累积*/
    //pid.integral += pid.err;

    /*PID算法实现*/
    //pid.actual_val = pid.Kp*pid.err + pid.Ki*pid.integral + pid.Kd*(pid.err - pid.err_last);
    pid.actual_val = pid.Kp*pid.err + pid.Kd*(pid.err - pid.err_last);

    pid.err_last = pid.err;

    return pid.actual_val;
}
