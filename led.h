/*
 * led.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#ifndef SOURCE_MID_LED_LED_H_
#define SOURCE_MID_LED_LED_H_

#include "app/framework/include/af.h"

#define LED_RGB_COUNT			2
#define LED_RGB_ELEMENT			3
#define LED_TOGGLE_TIME			500

#define LED_PORT_1				gpioPortA
#define LED_BLUE_PIN_1			(0U)
#define LED_GREEN_PIN_1			(3U)
#define LED_RED_PIN_1			(4U)

#define LED_PORT_2				gpioPortD
#define LED_BLUE_PIN_2			(2U)
#define LED_GREEN_PIN_2			(1U)
#define LED_RED_PIN_2			(0U)

#define LED_RGB_1				{ {LED_PORT_1,LED_RED_PIN_1} , {LED_PORT_1,LED_GREEN_PIN_1} , {LED_PORT_1,LED_BLUE_PIN_1} }
#define LED_RGB_2				{ {LED_PORT_2,LED_RED_PIN_2} , {LED_PORT_2,LED_GREEN_PIN_2} , {LED_PORT_2,LED_BLUE_PIN_2} }

#define LED_FREE						0
#define LED_TOGGLE						1

#define LED_OFF							0
#define LED_ON							1

typedef struct
{
	GPIO_Port_TypeDef port;
	unsigned int pin;
} ledStruct;
typedef struct
{
	unsigned int onTime;
	unsigned int offTime;
	unsigned int blinkCount;
} LedBlink;
typedef struct
{
	LedBlink ledBlink;
} LedAct;

typedef enum
{
	ledOff = 0x0,
	ledRed = (1U << 0),
	ledGreen = (1U << 1),
	ledBlue = (1U << 2),
	ledPink = (1U << 0) | (1U << 2),
	ledYellow = (1U << 0) | (1U << 1),
	ledRGB = (1U << 0) | (1U << 1) | (1U << 2)
} ledColor;

typedef struct
{
	unsigned int ledBlinkMode;
	ledColor color;
	LedAct ledAct;
} LedAction; // Biến cấu trúc hoạt động dành cho Led

typedef enum
{
	off,
	green,
	red,
	blue
} RGB_state;
typedef enum
{
	LED_1,
	LED_2,
} ledNumber;

void ledInit(void);
void onLed(ledNumber ledIndex, ledColor color);
void offLed(ledNumber ledIndex);
void toggleLed(ledNumber ledIndex, ledColor color, uint8_t toggleTime, uint32_t onTimeMs, uint32_t offTimeMs);
void toggleLed1Handler(void);
void toggleLed2Handler(void);

#endif /* SOURCE_MID_LED_LED_H_ */
