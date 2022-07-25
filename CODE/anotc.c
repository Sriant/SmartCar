/*
 * anotc.c
 *
 *  Created on: 2021��3��17��
 *      Author: YAN
 */
#include "headfile.h"
#include "anotc.h"

//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

uint8 data_to_send[50];    //�������ݻ���

/**
  * @brief  �û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���
  * @param  dataToSend�����ݵ�ַ
  * @param  length�����ݳ���
    *   @note   ��
  * @retval ��
  */
void ANO_DT_Send_Data(uint8 *dataToSend , uint8 length)
{
    seekfree_wireless_send_buff(dataToSend, length);
}

/**
  * @brief  ����uint16��������
  * @param  data�����ݵ�ַ
  * @param  len�����ݳ���
    *   @note   ������F1
  * @retval ��
  */
void ANO_DT_Send_uint16(uint16 *data, uint32 len)
{
    uint8 _cnt = 0;
    uint32 i = 0;

    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF1;
    data_to_send[_cnt++] = 0;

    for(i = 0; i < len; i++)
    {
        data_to_send[_cnt++] = BYTE1(data[i]);
        data_to_send[_cnt++] = BYTE0(data[i]);
    }

    data_to_send[3] = _cnt - 4;

    uint8 sum = 0;
    for (uint8 i = 0; i < _cnt; i++)
        sum += data_to_send[i];

    data_to_send[_cnt++] = sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}

/**
  * @brief  ����int16��������
  * @param  data�����ݵ�ַ
  * @param  len�����ݳ���
    *   @note   ������F2
  * @retval ��
  */
void ANO_DT_Send_int16(int16 *data, uint32 len)
{
    uint8 _cnt = 0;
    uint32 i = 0;

    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF2;
    data_to_send[_cnt++] = 0;

    for(i = 0; i < len; i++)
    {
        data_to_send[_cnt++] = BYTE1(data[i]);
        data_to_send[_cnt++] = BYTE0(data[i]);
    }

    data_to_send[3] = _cnt - 4;

    uint8 sum = 0;
    for (uint8 i = 0; i < _cnt; i++)
        sum += data_to_send[i];

    data_to_send[_cnt++] = sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}

/**
  * @brief  ����float��������
  * @param  data�����ݵ�ַ
  * @param  len�����ݳ���
    *   @note   ������F3
  * @retval ��
  */
void ANO_DT_Send_float(float *data, uint32 len)
{
    uint8 _cnt = 0;
    uint32 i = 0;

    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xF3;
    data_to_send[_cnt++] = 0;

    for(i = 0; i < len; i++)
    {
        data_to_send[_cnt++] = BYTE3(data[i]);
        data_to_send[_cnt++] = BYTE2(data[i]);
        data_to_send[_cnt++] = BYTE1(data[i]);
        data_to_send[_cnt++] = BYTE0(data[i]);
    }

    data_to_send[3] = _cnt - 4;

    uint8 sum = 0;
    for (uint8 i = 0; i < _cnt; i++)
        sum += data_to_send[i];

    data_to_send[_cnt++] = sum;

    ANO_DT_Send_Data(data_to_send, _cnt);
}
