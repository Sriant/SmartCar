/*
 * elec_handle.c
 *
 *  Created on: 2021��3��27��
 *      Author: YAN
 */
#include "elec_handle.h"
#include "headfile.h"

uint8 ai_data_flag;             //1��ad���ݲɼ����   0��ad����δ�ɼ����
int8 elec_ai[7];
uint16 elec1_data[ELEC1_CNT];   //ad����
uint16 elec2_data[ELEC2_CNT];
uint32 elec_T = 0;              //�ų�ǿ��

uint16 e1_max[ELEC1_CNT] = { 0, 0, 0, 0, 0, 0, 0 };
uint16 e1_min[ELEC1_CNT] = { 255, 255, 255, 255, 255, 255, 255 };
uint16 e1_nor[ELEC1_CNT] = { 0 };

uint16 adc_mid_filter(VADCN_enum vadc_n, VADC_CHN_enum vadc_chn, VADC_RES_enum vadc_res, uint16 cnt);
KFP KFP_T = {0.02, 0, 0, 0, 0.001, 0.543};

/**
  * @brief  adcͨ����ʼ��
  * @param  ��
    *   @note   ��
  * @retval ��
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
  * @brief  ������ݲɼ�
  * @param  ��
    *   @note   ��
  * @retval ��
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
  * @brief  Ѱ����ֵ
  * @param dat��Ŀ������
  * @param max�����ֵ
  * @param min����Сֵ
  * @param cnt�����ݳ���
    *   @note   ��
  * @retval ��
  */
void find_extremevalue(uint16 *data, uint16 *max, uint16 *min, uint16 cnt)
{
    uint16 i;

    /* ɨ�輫ֵ */
    for(i = 0; i < cnt; i++)
    {
        if(data[i] > max[i])
            max[i] = data[i];
        if(data[i] < min[i])
            min[i] = data[i];
    }
}

/**
  * @brief  ��һ������
  * @param dat��Ŀ������
  * @param nor����һ��������
  * @param max�����ֵ
  * @param min����Сֵ
  * @param cnt�����ݳ���
    *   @note   ��
  * @retval ��
  */
void normalized(uint16 *dat, uint16 *nor, uint16 *max, uint16 *min, uint16 cnt)
{
    uint16 i;

    find_extremevalue(dat, max, min, cnt);

    /* ��һ�� */
    for(i = 0; i < cnt; i++)
        nor[i] = 100 * (dat[i] - min[i]) / (max[i] - min[i]);
}

/**
  * @brief  ��ֵ��ֵ�˲�
  * @param      vadc_n     ѡ��ADCģ��(ADC_0��ADC_1��ADC_2)
  * @param      vadc_chn   ѡ��ADCͨ��
  * @param      vadc_res   ѡ��ADC�ֱ���
  * @param      cnt        ��ֵ�˲�����
  * @return     void
    *   @note   ��
  * @retval ��
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
 *@brief �������˲���
 *@param KFP *kfp �������ṹ�����
 *   float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
 *@return �˲���Ĳ���������ֵ��
 */
float kalmanFilter(KFP *kfp,float input)
{
    //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
    kfp->Now_P = kfp->LastP + kfp->Q;
    //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
    kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
    //����Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
    kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
    return kfp->out;
}

