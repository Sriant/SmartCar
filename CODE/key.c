/*
 * key.c
 *
 *  Created on: 2021年3月10日
 *      Author: YAN
 */
#include "headfile.h"
#include "key.h"

uint8 sw1_status = 0;
uint8 sw2_status = 0;
uint8 index = 1;

void key_init(void)
{
    //拨码开关初始化
    gpio_init(SW1,GPI,0,PULLUP);
    gpio_init(SW2,GPI,0,PULLUP);

    //微动按键初始化
    gpio_init(KEY1,GPI,0,PULLUP);
    gpio_init(KEY2,GPI,0,PULLUP);
    gpio_init(KEY3,GPI,0,PULLUP);
    gpio_init(KEY4,GPI,0,PULLUP);
}

uint8 key_scan(void)
{
    //开关状态变量
    static uint8 key1_status = 1;
    static uint8 key2_status = 1;
    static uint8 key3_status = 1;
    static uint8 key4_status = 1;

    //上一次开关状态变量
    static uint8 key1_last_status;
    static uint8 key2_last_status;
    static uint8 key3_last_status;
    static uint8 key4_last_status;

    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;

    key1_status = gpio_get(KEY1);
    key2_status = gpio_get(KEY2);
    key3_status = gpio_get(KEY3);
    key4_status = gpio_get(KEY4);

    if(key1_status && !key1_last_status)    return KEY1PRESS;
    if(key2_status && !key2_last_status)    return KEY2PRESS;
    if(key3_status && !key3_last_status)    return KEY3PRESS;
    if(key4_status && !key4_last_status)    return KEY4PRESS;

    return 0;
}

void key_task(void)
{
    static uint8 key = 0;

    //获取微动按键状态
    key = key_scan();

    switch(key)
    {
        case KEY1PRESS:
            switch(index)
            {
                case 1: linear_speed += 100; break;
                case 2: island_T += 10; break;
                case 3: spid.Kp += 0.1; break;
                case 4: spid.Kd += 0.1; break;
                case 5: curve_speed += 100; break;
                case 6: back = !back; break;
                default: break;
            }
            break;
        case KEY2PRESS:
            switch(index)
            {
                case 1: linear_speed -= 100; break;
                case 2: island_T -= 10; break;
                case 3: spid.Kp -= 0.1; break;
                case 4: spid.Kd -= 0.1; break;
                case 5: curve_speed -= 100; break;
                default: break;
            }
            break;
        case KEY3PRESS:
            index++;
            if(index > 6) index = 1;
            break;
        case KEY4PRESS:
            car_on = !car_on;
            break;
        default: break;
    }
}
