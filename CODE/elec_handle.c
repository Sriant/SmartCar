/*
 * elec_handle.c
 *
 *  Created on: 2021年3月27日
 *      Author: YAN
 */
#include "elec_handle.h"
#include "headfile.h"

uint8 ai_data_flag;             //1：ad数据采集完成   0：ad数据未采集完成
int8 elec_ai[7];
uint16 elec1_data[ELEC1_CNT];   //ad数据
uint16 elec2_data[ELEC2_CNT];
uint32 elec_T = 0;              //磁场强度

uint16 e1_max[ELEC1_CNT] = { 0, 0, 0, 0, 0, 0, 0 };
uint16 e1_min[ELEC1_CNT] = { 255, 255, 255, 255, 255, 255, 255 };
uint16 e1_nor[ELEC1_CNT] = { 0 };

uint16 adc_mid_filter(VADCN_enum vadc_n, VADC_CHN_enum vadc_chn, VADC_RES_enum vadc_res, uint16 cnt);
KFP KFP_T = {0.02, 0, 0, 0, 0.001, 0.543};

/**
  * @brief  adc通带初始化
  * @param  无
    *   @note   无
  * @retval 无
  */
void elec_init(void)
{
    adc_init(ADC_0, ADC0_CH0_A0);
    adc_init(ADC_0, ADC0_CH1_A1);
    adc_init(ADC_0, ADC0_CH2_A2);
    adc_init(ADC_0, ADC0_CH3_A3);
    adc_init(ADC_0, ADC0_CH4_A4);
    adc_init(ADC_0, ADC0_CH5_A5);
    adc_init(ADC_0, ADC0_CH6_A6);

    adc_init(ADC_0, ADC0_CH7_A7);
    adc_init(ADC_0, ADC0_CH10_A10);
    adc_init(ADC_0, ADC0_CH11_A11);
    adc_init(ADC_0, ADC0_CH12_A12);
    adc_init(ADC_0, ADC0_CH13_A13);
}
/**
  * @brief  电感数据采集
  * @param  无
    *   @note   无
  * @retval 无
  */
void get_elec_data(void)
{
    elec1_data[0] = adc_mid_filter(ADC_0, ADC0_CH0_A0, ADC_8BIT, 22);
    elec1_data[1] = adc_mid_filter(ADC_0, ADC0_CH1_A1, ADC_8BIT, 22);
    elec1_data[2] = adc_mid_filter(ADC_0, ADC0_CH2_A2, ADC_8BIT, 22);
    elec1_data[3] = adc_mid_filter(ADC_0, ADC0_CH3_A3, ADC_8BIT, 22);
    elec1_data[4] = adc_mid_filter(ADC_0, ADC0_CH4_A4, ADC_8BIT, 22);
    elec1_data[5] = adc_mid_filter(ADC_0, ADC0_CH5_A5, ADC_8BIT, 22);
    elec1_data[6] = adc_mid_filter(ADC_0, ADC0_CH6_A6, ADC_8BIT, 22);

//    elec2_data[0] = adc_mid_filter(ADC_0, ADC0_CH7_A7, ADC_8BIT, 12);
//    elec2_data[1] = adc_mid_filter(ADC_0, ADC0_CH10_A10, ADC_8BIT, 12);
//    elec2_data[2] = adc_mid_filter(ADC_0, ADC0_CH11_A11, ADC_8BIT, 12);
//    elec2_data[3] = adc_mid_filter(ADC_0, ADC0_CH12_A12, ADC_8BIT, 12);
//    elec2_data[4] = adc_mid_filter(ADC_0, ADC0_CH13_A13, ADC_8BIT, 12);
//    elec2_data[1] = (elec2_data[1] < 20) ? 0 : elec2_data[1];
//    elec2_data[3] = (elec2_data[3] < 20) ? 0 : elec2_data[3];

    normalized(elec1_data, e1_nor, e1_max, e1_min, ELEC1_CNT);

    elec_T = elec1_data[0] + elec1_data[1] + elec1_data[2] + elec1_data[3]
                + elec1_data[4] + elec1_data[5] + elec1_data[6];

//    elec_T = (uint32)kalmanFilter(&KFP_T, (float)elec_T);

    elec_ai[0] = (int8)((int16)e1_nor[0] - 128);
    elec_ai[1] = (int8)((int16)e1_nor[1] - 128);
    elec_ai[2] = (int8)((int16)e1_nor[2] - 128);
    elec_ai[3] = (int8)((int16)e1_nor[3] - 128);
    elec_ai[4] = (int8)((int16)e1_nor[4] - 128);
    elec_ai[5] = (int8)((int16)e1_nor[5] - 128);
    elec_ai[6] = (int8)((int16)e1_nor[6] - 128);

    ai_data_flag = 1;
}
/**
  * @brief  寻找最值
  * @param dat：目标数据
  * @param max：最大值
  * @param min：最小值
  * @param cnt：数据长度
    *   @note   无
  * @retval 无
  */
void find_extremevalue(uint16 *data, uint16 *max, uint16 *min, uint16 cnt)
{
    uint16 i;

    /* 扫描极值 */
    for(i = 0; i < cnt; i++)
    {
        if(data[i] > max[i])
            max[i] = data[i];
        if(data[i] < min[i])
            min[i] = data[i];
    }
}

/**
  * @brief  归一化处理
  * @param dat：目标数据
  * @param nor：归一化后数据
  * @param max：最大值
  * @param min：最小值
  * @param cnt：数据长度
    *   @note   无
  * @retval 无
  */
void normalized(uint16 *dat, uint16 *nor, uint16 *max, uint16 *min, uint16 cnt)
{
    uint16 i;

    find_extremevalue(dat, max, min, cnt);

    /* 归一化 */
    for(i = 0; i < cnt; i++)
        nor[i] = 100 * (dat[i] - min[i]) / (max[i] - min[i]);
}

/**
  * @brief  中值均值滤波
  * @param      vadc_n     选择ADC模块(ADC_0、ADC_1、ADC_2)
  * @param      vadc_chn   选择ADC通道
  * @param      vadc_res   选择ADC分辨率
  * @param      cnt        均值滤波次数
  * @return     void
    *   @note   无
  * @retval 无
  */
uint16 adc_mid_filter(VADCN_enum vadc_n, VADC_CHN_enum vadc_chn, VADC_RES_enum vadc_res, uint16 cnt)
{
    uint32 sum = 0;
    int i, j;
    uint16 value_buf[cnt], temp;

    for(i = 0; i < cnt; i++)
        value_buf[i] = adc_convert(vadc_n, vadc_chn, vadc_res);

    for(j = 0; j < cnt -1; j++)
    {
        for(i = 0; i < cnt - 1 - j; i++)
        {
            if(value_buf[i] > value_buf[i + 1])
            {
                temp = value_buf[i];
                value_buf[i] = value_buf[i + 1];
                value_buf[i + 1] = temp;
            }
        }
    }

    for(i = 1; i < cnt - 1; i++)
        sum += value_buf[i];
    return (uint16)(sum/(cnt - 2));
}

/**
 *@brief 卡尔曼滤波器
 *@param KFP *kfp 卡尔曼结构体参数
 *   float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
float kalmanFilter(KFP *kfp,float input)
{
    //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
    kfp->Now_P = kfp->LastP + kfp->Q;
    //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
    //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
    kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
    return kfp->out;
}

