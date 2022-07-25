/*
 * servo_ctrl.c
 *
 *  Created on: 2021年3月27日
 *      Author: YAN
 */
#include "headfile.h"
#include "servo_ctrl.h"

_pid spid;
int16 turn_angle = 0;   //舵机控制变量, 左打角+，右打角-
float error = 0;        //电感偏差

uint16 island_T = 950;
uint8 enter_island = 0;
uint8 exit_island = 0;
uint8 island_flag = 0;
uint8 sendbuff1[] = "RightIsland\r\n";
uint8 sendbuff2[] = "LeftIsland\r\n";
uint8 sendbuff3[] = "Exit\r\n";
uint8 leftright_island = 0; //1：左进环；2：右进环

uint8 cross_flag = 0;
uint8 sendbuff4[] = "Cross\r\n";

uint8 dir = 1;
uint8 back = 0;
/**
  * @brief  舵机pid参数初始化
  * @param  无
    *   @note   无
  * @retval 无
  */
void spid_param_init(void)
{
    /* 初始化参数 */
    spid.target_val = 0.0;
    spid.actual_val = 0.0;
    spid.err = 0.0;
    spid.err_last = 0.0;

    spid.Kp = 3.0;
    spid.Kd = 0.8;
}
/**
  * @brief  舵机初始化
  * @param  无
    *   @note   无
  * @retval 无
  */
void servo_init(void)
{
    gtm_pwm_init(S_MOTOR_PIN, 50, ANGLE_CENTER);
    spid_param_init();
}
/**
  * @brief  转向
  * @param  angle：转向角度, ++左打角，--右打角
    *   @note   无
  * @retval 无
  */
void turn(void)
{
    //限幅
    if(leftright_island == 1 && island_flag == 1)
        turn_angle = (int16)limit_ab(turn_angle, -300, ANGLE_MAX);
    else if(leftright_island == 2 && island_flag == 1)
        turn_angle = (int16)limit_ab(turn_angle, -ANGLE_MAX, 300);
    else if((e1_nor[1] + e1_nor[5]) < 10 && elec_T > 400)
        turn_angle = (int16)limit(turn_angle, 500);
    else
        turn_angle = (int16)limit(turn_angle, ANGLE_MAX);

    //转向
    pwm_duty(S_MOTOR_PIN, ANGLE_CENTER - turn_angle);
}
/**
  * @brief  环岛处理
  * @param  无
    *   @note   无
  * @retval 无
  */
void island_handle(uint16 *elec)
{
    if(elec_T > island_T && island_flag == 0 && leftright_island == 0)
    {
        Waiting_for_parking = 1;
        island_flag = 2;
        if(elec[1] + elec[0] + elec[2] > elec[5] + elec[4] + elec[6])
        {
            leftright_island = 1;
            seekfree_wireless_send_buff(sendbuff2, sizeof(sendbuff2));
        }
        else if(elec[1] + elec[0] + elec[2] < elec[5] + elec[4] + elec[6])
        {
            leftright_island = 2;
            seekfree_wireless_send_buff(sendbuff1, sizeof(sendbuff1));
        }
    }
    // 进岛处理
    if(island_flag == 2)
    {
        island_flag--;
        if(leftright_island == 1)
        {
            inte_pulse = 0;
            while(inte_pulse < 30000)
            {
                turn_angle = 0;
                turn();
            }
            inte_pulse = 0;
            while(inte_pulse < 25000)
            {
                turn_angle = +1600;
                turn();
            }
        }
        else if(leftright_island == 2)
        {
            inte_pulse = 0;
            while(inte_pulse < 30000)
            {
                turn_angle = 0;
                turn();
            }
            inte_pulse = 0;
            while(inte_pulse < 25000)
            {
                turn_angle = -1600;
                turn();
            }
        }
    }
    // 出岛处理
    if(elec_T > island_T && island_flag == 1)
    {
        island_flag--;
        seekfree_wireless_send_buff(sendbuff3, sizeof(sendbuff3));
//        if(leftright_island == 1)           //左环
//        {
//            inte_pulse = 0;
//            while(inte_pulse < 30000)
//            {
//                turn_angle = 0;
//                turn();
//            }
//        }
//        else if(leftright_island == 2)      //右环
//        {
//            inte_pulse = 0;
//            while(inte_pulse < 30000)
//            {
//                turn_angle = 0;
//                turn();
//            }
//        }
        inte_pulse = 0;
        while(inte_pulse < 10000)
        {
            turn_angle = 0;
            turn();
        }
        inte_pulse = 0;
        while(inte_pulse < 40000)
        {
            if(leftright_island == 1)
                error = 1000 * ( (float)(2 * elec[6] - elec[0]) + ( 2 * elec[4] - elec[2]) ) /
                        ( (2 * elec[6] + elec[0]) + ( 2 * elec[4] + elec[2]) + elec[3] );
            else if(leftright_island == 2)
                error = 1000 * ( (float)(elec[6] - 2 * elec[0]) + (elec[4] - 2 * elec[2]) ) /
                        ( (elec[6] + 2 * elec[0]) + (elec[4] + 2 * elec[2]) + elec[3] );
            turn_angle = pid_pos_realize(spid, error);
            turn();
        }
        leftright_island = 0;
    }
}

void vertical_handle(uint16 *elec)
{
    static uint8 vertical_flag = 0;
    if(elec_T < 100 && elec_T >=30 && leftright_island == 0 && vertical_flag == 0)
    {

        if(elec[1] + elec[0] + elec[2] > elec[5] + elec[4] + elec[6])
        {
            vertical_flag = 1;
        }
        else if(elec[1] + elec[0] + elec[2] < elec[5] + elec[4] + elec[6])
        {
            vertical_flag = 2;
        }
    }
    if(vertical_flag == 1)
    {
        inte_pulse = 0;
        while(fabs((float)inte_pulse) < 30000)
        {
            dir = 0;
            turn_angle = -1650;
            turn();
        }
        vertical_flag = 0;
    }
    else if(vertical_flag == 2)
    {
        inte_pulse = 0;
        while(fabs((float)inte_pulse) < 30000)
        {
            dir = 0;
            turn_angle = +1650;
            turn();
        }
        vertical_flag = 0;
    }
    dir = 1;
}

void lose_line(uint16 *elec)
{
    while(elec_T < 120)
    {
        turn();
    }
}

/**
  * @brief  舵机转向量控制
  * @param  无
    *   @note   无
  * @retval 无
  */
void servo_control(uint16 *elec)
{
    elec = e1_nor;
//    elec = elec2_data;

    //电感差比和
    error = 1000 * ( (elec[6] - elec[0]) + (2 * elec[5] - 2 * elec[1]) + (elec[4] - elec[2]) ) /
            ( (elec[6] + elec[0]) + fabs(2 * elec[5] - 2 * elec[1]) + (elec[4] + elec[2]) + elec[3] );

//    error = 1000 * ( (elec[4] - elec[0]) + 3.5 * (2 * elec[3] - elec[1]) ) /
//            ( (elec[4] + elec[0]) + fabs(2 * elec[3] - 2 * elec[1]) + elec[2] );

    turn_angle = pid_pos_realize(spid, error);

    turn();

    island_handle(e1_nor);
    if(back)
        vertical_handle(e1_nor);
    else
        lose_line(e1_nor);
}
