/*
 * display.h
 *
 *  Created on: 2021Äê3ÔÂ26ÈÕ
 *      Author: YAN
 */

#ifndef CODE_DISPLAY_H_
#define CODE_DISPLAY_H_

#include "common.h"

enum ui_e
{
    MAIN_UI,
    DEBUG_UI,
};

void display_task(void);
void setShow_ui(enum ui_e ui);

#endif /* CODE_LCD_H_ */
