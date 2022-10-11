/*
 * button.c
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#include "button.h"

buttonCallbackFunction btnCallbackFunc = (buttonCallbackFunction) NULL;
buttonState_e buttonState0 = release, buttonState1 = release;
static unsigned int i;
uint64_t timeInit0 = 0, timeInit1 = 0, timePress = 0, timeWait = 0;
void buttonInit(buttonCallbackFunction callbackInit)
{
	if (callbackInit != NULL)
	{
		// Gán hàm xử lý nút bấm
		btnCallbackFunc = callbackInit;
	}
}
/** @brief Hal Button Isr
 *
 * This callback is called by the framework whenever a button is pressed on the
 * device. This callback is called within ISR context.
 *
 * @param button The button which has changed state, either BUTTON0 or BUTTON1
 * as defined in the appropriate BOARD_HEADER.  Ver.: always
 * @param state The new state of the button referenced by the button parameter,
 * either ::BUTTON_PRESSED if the button has been pressed or ::BUTTON_RELEASED
 * if the button has been released.  Ver.: always
 */
void emberAfHalButtonIsrCallback(uint8_t button, uint8_t state)
{
	if (state == BUTTON_PRESSED)
	{
		// Vòng lặp chờ tín hiệu nút bấm ổn định
		for (i = 0; i < 500000; i++);
		if (halButtonPinState(button) == BUTTON_PRESSED)
		{
			// Sau khi chờ ổn định, nếu nút vẫn ghi nhận trạng thái bấm, ghi nhận thời gian bấm nút
			emberAfCorePrintln("Button Pressed Confirmed");
			timePress = Get_Millisecond();
			emberAfCorePrintln("timePress: %lu", timePress);
			if (button == BUTTON0) // Trường hợp tín hiệu từ nút bấm 0
			{
				if (timePress - timeInit0 <= TIME_MULTI_PRESS)
				{
					// Nếu khoảng thời gian trôi qua vẫn trong thời hạn ghi nhận tín hiệu bấm, tăng biến đếm lần bấm nút 0
					buttonState0 += 1;
				}
				else
				{
					// Nếu khoảng thời gian trôi qua vượt quá thời hạn ghi nhận tín hiệu bấm, đến lần bấm này là 1 và gán lại thời gian bắt đầu ghi nhận
					buttonState0 = 1;
					timeInit0 = timePress;
				}
				btnCallbackFunc (button, buttonState0); // Thực hiện hàm xử lý theo số lần bấm nút 0 đếm được.
			}
			else if (button == BUTTON1) // Trường hợp tín hiệu từ nút bấm 1
			{
				if (timePress - timeInit1 <= TIME_MULTI_PRESS)
				{
					// Nếu khoảng thời gian trôi qua vẫn trong thời hạn ghi nhận tín hiệu bấm, tăng biến đếm lần bấm nút 1
					buttonState1 += 1;
				}
				else
				{
					// Nếu khoảng thời gian trôi qua vượt quá thời hạn ghi nhận tín hiệu bấm, đến lần bấm này là 1 và gán lại thời gian bắt đầu ghi nhận
					buttonState1 = 1;
					timeInit1 = timePress;
				}
				btnCallbackFunc (button, buttonState1); // Thực hiện hàm xử lý theo số lần bấm nút 1 đếm được.
			}
		}
	}
}
