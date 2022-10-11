/*
 * main.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#ifndef SOURCE_APP_MAIN_H_
#define SOURCE_APP_MAIN_H_

#include "Source/Mid/Led/led.h"
#include "Source/Mid/Button/button.h"
#include "Source/Mid/LDR/ldr.h"
#include "Source/Mid/Timer/timer.h"
#include "Source/App/Network/network.h"
#include "Source/App/Send/send.h"
#include "Source/App/Receive/receive.h"

typedef enum
{
	POWER_ON_STATE,
	REPORT_STATE,
	IDLE_STATE,
	REBOOT_STATE
} systemState_e; // Các trạng thái của thiết bị
#define ZDO_MESSAGE_OVERHEAD					1

void emberAfMainInitCallback(void);
void mainStateEventHandler(void);
void MAIN_NetworkCallbackHandler(uint8_t networkResult);
void buttonCallbackHandler(uint8_t button, buttonState_e state);

#endif /* SOURCE_APP_MAIN_H_ */
