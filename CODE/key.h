/*
 * key.h
 *
 *  Created on: 2021年3月10日
 *      Author: YAN
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#include "common.h"

//定义拨码开关引脚
#define SW1     P33_12
#define SW2     P33_13
//定义按键引脚
#define KEY1    P22_0
#define KEY2    P22_1
#define KEY3    P22_2
#define KEY4    P22_3

#define KEY1PRESS 1
#define KEY2PRESS 2
#define KEY3PRESS 3
#define KEY4PRESS 4

void key_init(void);
uint8 key_scan(void);
void key_task(void);

extern uint8 sw1_status;
extern uint8 sw2_status;
extern uint8 index;

#endif /* CODE_KEY_H_ */
