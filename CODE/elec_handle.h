/*
 * elec_handle.h
 *
 *  Created on: 2021��3��27��
 *      Author: YAN
 */

#ifndef CODE_ELEC_HANDLE_H_
#define CODE_ELEC_HANDLE_H_

#include "common.h"

#define ELEC1_CNT   7
#define ELEC2_CNT   5

typedef struct
{
    float LastP;    //�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;    //��ǰ����Э���� ��ʼ��ֵΪ0
    float out;      //�������˲������ ��ʼ��ֵΪ0
    float Kg;       //���������� ��ʼ��ֵΪ0
    float Q;        //��������Э���� ��ʼ��ֵΪ0.001
    float R;        //�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP;       //Kalman Filter parameter

void elec_init(void);
void get_elec_data(void);
void normalized(uint16 *dat, uint16 *nor, uint16 *max, uint16 *min, uint16 cnt);
float kalmanFilter(KFP *kfp,float input);

extern uint8 ai_data_flag;              //1��ad���ݲɼ����   0��ad����δ�ɼ����
extern int8 elec_ai[7];
extern uint16 elec1_data[ELEC1_CNT];    //ad����
extern uint16 elec2_data[ELEC2_CNT];
extern uint32 elec_T;

extern uint16 e1_max[ELEC1_CNT];
extern uint16 e1_min[ELEC1_CNT];
extern uint16 e1_nor[ELEC1_CNT];       //��һ������

#endif /* CODE_ELEC_HANDLE_H_ */
