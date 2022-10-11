/*
 * led.c
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#include "led.h"

ledStruct ledRgbArray[LED_RGB_COUNT][LED_RGB_ELEMENT] = {LED_RGB_1, LED_RGB_2};
EmberEventControl led1ToggleEventControl, led2ToggleEventControl;
LedAction ledAction[LED_RGB_COUNT];

void ledInit(void)
{
	CMU_ClockEnable(cmuClock_GPIO, true); // Cấp xung nhịp cho các chân GPIO
	for (int i = 0; i < LED_RGB_COUNT; i++)
	{
		for (int j = 0; j < LED_RGB_ELEMENT; j++)
		{
			// Thiết lập chế độ Push Pull cho các chân Led
			GPIO_PinModeSet(ledRgbArray[i][j].port, ledRgbArray[i][j].pin, gpioModePushPull, 0);
		}
	}
	// Tắt các Led sau khi khởi động
	offLed(LED_1);
	offLed(LED_2);
}
void onLed(ledNumber ledIndex, ledColor color)
{
	// Rà soát xem thành phần màu Led nào có màu trùng khớp thì bật
	for (int i = 0; i < LED_RGB_ELEMENT; i++)
	{
		if (((color >> i) & 0x01) == 1)
		{
			GPIO_PinOutClear(ledRgbArray[ledIndex][i].port, ledRgbArray[ledIndex][i].pin);
		}
		else
		{
			GPIO_PinOutSet(ledRgbArray[ledIndex][i].port, ledRgbArray[ledIndex][i].pin);
		}
	}
}
void offLed(ledNumber ledIndex)
{
	for (int j = 0; j < LED_RGB_ELEMENT; j++)
	{
		// Tắt hết các thành phần màu Led
		GPIO_PinOutSet(ledRgbArray[ledIndex][j].port, ledRgbArray[ledIndex][j].pin);
	}
}
void toggleLed(ledNumber ledIndex, ledColor color, uint8_t toggleTime, uint32_t onTimeMs, uint32_t offTimeMs)
{
	emberAfCorePrintln("toggleLed");
	// Nhập các thông số nhận được vào biến hoạt động của Led, sau đó kích hoạt sự kiện nháy Led tương ứng
	ledAction[ledIndex].ledBlinkMode = LED_TOGGLE;
	ledAction[ledIndex].color = color;
	ledAction[ledIndex].ledAct.ledBlink.onTime = onTimeMs;
	ledAction[ledIndex].ledAct.ledBlink.offTime = offTimeMs;
	ledAction[ledIndex].ledAct.ledBlink.blinkCount = toggleTime * 2;
	if (ledIndex == LED_1)
	{
		emberEventControlSetInactive(led1ToggleEventControl);
		emberEventControlSetActive(led1ToggleEventControl);
	}
	else if (ledIndex == LED_2)
	{
		emberEventControlSetInactive(led2ToggleEventControl);
		emberEventControlSetActive(led2ToggleEventControl);
	}
}
void toggleLed1Handler(void)
{
	emberAfCorePrintln("toggleLed1Handler");
	emberEventControlSetInactive(led1ToggleEventControl); // Tắt sự kiện nháy Led 1 để chủ động thời gian quay vòng
	if (ledAction[LED_1].ledAct.ledBlink.blinkCount != 0)
	{
		// Lần lượt thực hiện bật/tắt đèn Led 1, và thiết lập thời gian quay vòng, cho đến khi biến đếm về 0
		if (ledAction[LED_1].ledAct.ledBlink.blinkCount % 2)
		{
			onLed(LED_1, ledAction[LED_1].color);
			emberEventControlSetDelayMS(led1ToggleEventControl, ledAction[LED_1].ledAct.ledBlink.onTime);
		}
		else
		{
			offLed(LED_1);
			emberEventControlSetDelayMS(led1ToggleEventControl, ledAction[LED_1].ledAct.ledBlink.offTime);
		}
		ledAction[LED_1].ledAct.ledBlink.blinkCount--;
	}
	else
	{
		// Khi biến đếm bằng 0 thì chuyển về trạng thái tĩnh và tắt đèn Led 1
		ledAction[LED_1].ledBlinkMode = LED_FREE;
		offLed(LED_1);
	}
}
void toggleLed2Handler(void)
{
	emberAfCorePrintln("toggleLed2Handler");
	emberEventControlSetInactive(led2ToggleEventControl); // Tắt sự kiện nháy Led 2 để chủ động thời gian quay vòng
	if (ledAction[LED_2].ledAct.ledBlink.blinkCount != 0)
	{
		// Lần lượt thực hiện bật/tắt đèn Led 2, và thiết lập thời gian quay vòng, cho đến khi biến đếm về 0
		if (ledAction[LED_2].ledAct.ledBlink.blinkCount % 2)
		{
			onLed(LED_2, ledAction[LED_2].color);
			emberEventControlSetDelayMS(led2ToggleEventControl, ledAction[LED_2].ledAct.ledBlink.onTime);
		}
		else
		{
			offLed(LED_2);
			emberEventControlSetDelayMS(led2ToggleEventControl, ledAction[LED_2].ledAct.ledBlink.offTime);
		}
		ledAction[LED_2].ledAct.ledBlink.blinkCount--;
	}
	else
	{
		// Khi biến đếm bằng 0 thì chuyển về trạng thái tĩnh và tắt đèn Led 2
		ledAction[LED_2].ledBlinkMode = LED_FREE;
		offLed(LED_2);
	}
}
