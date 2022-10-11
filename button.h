/*
 * button.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#ifndef SOURCE_MID_BUTTON_BUTTON_H_
#define SOURCE_MID_BUTTON_BUTTON_H_

#include "app/framework/include/af.h"
#include "hal-config.h"
#include "Source/Mid/Timer/timer.h"

#define BUTTON0					BSP_BUTTON0_PIN
#define BUTTON1					BSP_BUTTON1_PIN
#define TIME_MULTI_PRESS		2500 // Thời hạn ghi nhận tín hiệu nút bấm là 2,5s

typedef enum
{
	release,
	press_1,
	press_2,
	press_3,
	press_4,
	press_5
} buttonState_e;
typedef void (*buttonCallbackFunction)(uint8_t, buttonState_e); // Định nghĩa kiểu hàm xử lý nút bấm

void buttonInit(buttonCallbackFunction);

#endif /* SOURCE_MID_BUTTON_BUTTON_H_ */
