/*
 * display.c
 *
 *  Created on: 2021年3月26日
 *      Author: YAN
 */

#include "display.h"
#include "headfile.h"

static enum ui_e show_ui = MAIN_UI;

void setShow_ui(enum ui_e ui)
{
    lcd_clear(WHITE);
    show_ui = ui;
}

void main_ui(void)
{
    //前排电感数据
    lcd_showstr(0, 0, "1:");
    lcd_showint32(15, 0, (int32)elec2_data[0], 3);
    lcd_showstr(0, 1, "2:");
    lcd_showint32(15, 1, (int32)elec2_data[1], 3);
    lcd_showstr(0, 2, "3:");
    lcd_showint32(15, 2, (int32)elec2_data[2], 3);
    lcd_showstr(0, 3, "4:");
    lcd_showint32(15, 3, (int32)elec2_data[3], 3);
    lcd_showstr(0, 4, "5:");
    lcd_showint32(15, 4, (int32)elec2_data[4], 3);
    //后排电感数据
    lcd_showstr(80, 0, "1:");
    lcd_showint32(95, 0, (int32)e1_nor[0], 3);
    lcd_showstr(80, 1, "2:");
    lcd_showint32(95, 1, (int32)e1_nor[1], 3);
    lcd_showstr(80, 2, "3:");
    lcd_showint32(95, 2, (int32)e1_nor[2], 3);
    lcd_showstr(80, 3, "4:");
    lcd_showint32(95, 3, (int32)e1_nor[3], 3);
    lcd_showstr(80, 4, "5:");
    lcd_showint32(95, 4, (int32)e1_nor[4], 3);
    lcd_showstr(80, 5, "6:");
    lcd_showint32(95, 5, (int32)e1_nor[5], 3);
    lcd_showstr(80, 6, "7:");
    lcd_showint32(95, 6, (int32)e1_nor[6], 3);

    lcd_showstr(0, 5, "E:");
    lcd_showint32(15, 5, (int32)error, 4);
    lcd_showstr(0, 6, "t:");
    lcd_showint32(15, 6, (int32)turn_angle, 4);
    lcd_showstr(0, 7, "i:");
    lcd_showint32(15, 7, (int32)inte_pulse, 5);
    lcd_showstr(80, 7, "T:");
    lcd_showint32(95, 7, (int32)elec_T, 5);

    if(gpio_get(SW2) == 1)
        setShow_ui(DEBUG_UI);
}

void debug_ui(void)
{
    lcd_showstr(0, 0, "1.linear_v:");
    lcd_showint32(90, 0, (int32)linear_speed, 5);
    lcd_showstr(0, 1, "2.island_T:");
    lcd_showint32(85, 1, (int32)island_T, 5);
    lcd_showstr(0, 2, "3.Kp:");
    lcd_showfloat(45, 2, (double)spid.Kp, 1, 1);
    lcd_showstr(0, 3, "4.Kd:");
    lcd_showfloat(45, 3, (double)spid.Kd, 1, 1);
    lcd_showstr(0, 4, "5.curve_v:");
    lcd_showint32(90, 4, (int32)curve_speed, 5);
    lcd_showstr(0, 5, "6.back:");
    lcd_showint32(60, 5, (int32)back, 1);

    lcd_showint32(130, 0, (int32)index, 2);
    if(gpio_get(SW2) == 0)
        setShow_ui(MAIN_UI);
}
void display_task(void)
{
    switch(show_ui)
    {
        case MAIN_UI: main_ui(); break;
        case DEBUG_UI: debug_ui(); break;
        default: break;
    }

}
