/*
 * elec_handle.h
 *
 *  Created on: 2021年3月27日
 *      Author: YAN
 */

#ifndef CODE_ELEC_HANDLE_H_
#define CODE_ELEC_HANDLE_H_

#include "common.h"

#define ELEC1_CNT   7
#define ELEC2_CNT   5

typedef struct
{
    float LastP;    //上次估算协方差 初始化值为0.02
    float Now_P;    //当前估算协方差 初始化值为0
    float out;      //卡尔曼滤波器输出 初始化值为0
    float Kg;       //卡尔曼增益 初始化值为0
    float Q;        //过程噪声协方差 初始化值为0.001
    float R;        //观测噪声协方差 初始化值为0.543
}KFP;       //Kalman Filter parameter

void elec_init(void);
void get_elec_data(void);
void normalized(uint16 *dat, uint16 *nor, uint16 *max, uint16 *min, uint16 cnt);
float kalmanFilter(KFP *kfp,float input);

extern uint8 ai_data_flag;              //1：ad数据采集完成   0：ad数据未采集完成
extern int8 elec_ai[7];
extern uint16 elec1_data[ELEC1_CNT];    //ad数据
extern uint16 elec2_data[ELEC2_CNT];
extern uint32 elec_T;

extern uint16 e1_max[ELEC1_CNT];
extern uint16 e1_min[ELEC1_CNT];
extern uint16 e1_nor[ELEC1_CNT];       //归一化数据

#endif /* CODE_ELEC_HANDLE_H_ */
