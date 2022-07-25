/*
 * mpid_ctrl.c
 *
 *  Created on: 2021��3��12��
 *      Author: YAN
 */
#include "headfile.h"
#include "pid_ctrl.h"
/**
  * @brief  ����Ŀ��ֵ
  * @param       pid��Ŀ��pid����
  * @param  temp_val��Ŀ��ֵ
    *   @note   ��
  * @retv0al ��
  */
void set_pid_target(_pid pid, float temp_val)
{
    pid.target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  pid��Ŀ��pid����
    *   @note   ��
  * @retval Ŀ��ֵ
  */
float get_pid_target(_pid pid)
{
    return pid.target_val;
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  pid��Ŀ��pid����
  * @param    p������ϵ�� P
  * @param    i������ϵ�� i
  * @param    d��΢��ϵ�� d
    *   @note   ��
  * @retval ��
  */
void set_p_i_d(_pid pid, float p, float i, float d)
{
    pid.Kp = p;
    pid.Ki = i;
    pid.Kd = d;
}

/**
  * @brief  ����ʽpid�㷨ʵ��
  * @param       pid��Ŀ��pid����
  * @param  temp_val��Ŀ��ֵ
    *   @note   ��
  * @retval ͨ��pid���������
  */
float pid_inc_realize(_pid pid, float temp_val)
{
    /*�������*/
    pid.err_last = pid.err_next;
    pid.err_next = pid.err;

    /*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid.err = pid.target_val - temp_val;

    /*pid�㷨ʵ��*/
    pid.actual_val += pid.Kp * (pid.err - pid.err_next)
                   +  pid.Ki *  pid.err
                   +  pid.Kd * (pid.err - 2 * pid.err_next + pid.err_last);

    return pid.actual_val;
}
/**
  * @brief  λ��ʽpid�㷨ʵ��
  * @param       pid��Ŀ��pid����
  * @param  temp_val��Ŀ��ֵ
    *   @note   �˴�ע�Ͳ��ֵ���ΪPD����
  * @retval ͨ��pid���������
  */
float pid_pos_realize(_pid pid, float temp_val)
{
    /*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid.err = pid.target_val - temp_val;

    /*����ۻ�*/
    //pid.integral += pid.err;

    /*PID�㷨ʵ��*/
    //pid.actual_val = pid.Kp*pid.err + pid.Ki*pid.integral + pid.Kd*(pid.err - pid.err_last);
    pid.actual_val = pid.Kp*pid.err + pid.Kd*(pid.err - pid.err_last);

    pid.err_last = pid.err;

    return pid.actual_val;
}
