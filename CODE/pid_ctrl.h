/*
 * pid_ctrl.h
 *
 *  Created on: 2021年3月12日
 *      Author: YAN
 */

#ifndef CODE_PID_CTRL_H_
#define CODE_PID_CTRL_H_


#include "common.h"

/*pid*/
typedef struct
{
    float target_val;     //目标值
    float actual_val;     //实际值
    float err;            //定义当前偏差值
    float err_next;       //定义下一个偏差值
    float err_last;       //定义最后一个偏差值
    float Kp, Ki, Kd;     //定义比例、积分、微分系数
    float integral;       //定义积分值
}_pid;

extern void set_pid_target(_pid pid, float temp_val);
extern float get_pid_target(_pid pid);
extern void set_p_i_d(_pid pid, float p, float i, float d);
extern float pid_inc_realize(_pid pid, float temp_val);
extern float pid_pos_realize(_pid pid, float temp_val);

#endif /* CODE_PID_CTRL_H_ */
