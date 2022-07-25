/*
 * pid_ctrl.h
 *
 *  Created on: 2021��3��12��
 *      Author: YAN
 */

#ifndef CODE_PID_CTRL_H_
#define CODE_PID_CTRL_H_


#include "common.h"

/*pid*/
typedef struct
{
    float target_val;     //Ŀ��ֵ
    float actual_val;     //ʵ��ֵ
    float err;            //���嵱ǰƫ��ֵ
    float err_next;       //������һ��ƫ��ֵ
    float err_last;       //�������һ��ƫ��ֵ
    float Kp, Ki, Kd;     //������������֡�΢��ϵ��
    float integral;       //�������ֵ
}_pid;

extern void set_pid_target(_pid pid, float temp_val);
extern float get_pid_target(_pid pid);
extern void set_p_i_d(_pid pid, float p, float i, float d);
extern float pid_inc_realize(_pid pid, float temp_val);
extern float pid_pos_realize(_pid pid, float temp_val);

#endif /* CODE_PID_CTRL_H_ */
