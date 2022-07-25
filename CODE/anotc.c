/*
 * anotc.c
 *
 *  Created on: 2021年3月17日
 *      Author: YAN
 */
#include "headfile.h"
#include "anotc.h"

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

uint8 data_to_send[50];    //发送数据缓存

/**
  * @brief  用户应根据自身应用的情况，根据使用的通信方式，实现此函数
  * @param  dataToSend：数据地址
  * @param  length：数据长度
    *   @note   无
  * @retval 无
  */
void ANO_DT_Send_Data(uint8 *dataToSend , uint8 length)
{
    seekfree_wireless_send_buff(dataToSend, length);
}

/**
  * @brief  发送uint16类型数据
  * @param  data：数据地址
  * @param  len：数据长度
    *   @note   数据祯F1
  * @retval 无
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
  * @brief  发送int16类型数据
  * @param  data：数据地址
  * @param  len：数据长度
    *   @note   数据祯F2
  * @retval 无
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
  * @brief  发送float类型数据
  * @param  data：数据地址
  * @param  len：数据长度
    *   @note   数据祯F3
  * @retval 无
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
