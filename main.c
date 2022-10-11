/*
 * main.c
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#include "main.h"

systemState_e systemState;
EmberEventControl mainStateEventControl;
boolean networkReady = false;

void emberAfMainInitCallback(void)
{
	// Khi mới khởi động lên, khởi tạo hệ thống led và nút bấm trước, đồng thời khởi tạo bộ đếm, sau đó bắt đầu giai đoạn đầu tiên của chương trình (Power On)
	emberAfCorePrintln("emberAfMainInitCallback");
	ledInit();
	buttonInit(buttonCallbackHandler);
	TIMER3_Init();
	systemState = POWER_ON_STATE;
	emberEventControlSetActive(mainStateEventControl); // Kích hoạt sự kiện đưa đến hàm mainStateEventHandler
}
void mainStateEventHandler(void)
{
	EmberNetworkStatus nwkStatusCurrent;
	emberEventControlSetInactive(mainStateEventControl);
	switch (systemState)
	{
	case POWER_ON_STATE:
		nwkStatusCurrent = emberAfNetworkState();
		if (nwkStatusCurrent == EMBER_NO_NETWORK)
		{
			// Nếu ban đầu chưa có mạng thì nháy đỏ 3 lần và bắt đầu tìm mạng
			toggleLed(LED_1, ledRed, 3, 200, 200);
			NETWORK_FindAndJoin();
		}
		break;
	case REPORT_STATE:
		// Thực hiện báo cáo thông tin thiết bị lên HC; đồng thời khởi tạo cảm biến ánh sáng, cũng như bộ lọc Kalman
		emberAfCorePrintln("REPORT_STATE");
		SEND_ReportInfoToHc();
		ADCLight_Init();
		KalmanFilterInit(5, 5, 0.01);
		// Chuyển hệ thống sang trạng thái rảnh
		systemState = IDLE_STATE;
		emberEventControlSetDelayMS(mainStateEventControl, 2000);
		break;
	case IDLE_STATE: // Trạng thái rảnh
		break;
	case REBOOT_STATE:
		// Xóa bảng Binding Table, thực hiện rời mạng trước khi reboot
		emberAfCorePrintln("REBOOT_STATE");
		uint8_t contents[ZDO_MESSAGE_OVERHEAD + 1];
		contents[0] = 0x00;
		emberSendZigDevRequest(0x0000, LEAVE_RESPONSE, EMBER_AF_DEFAULT_APS_OPTIONS, contents, sizeof(contents));
		emberClearBindingTable();
		EmberNetworkStatus nwkStatusCurrent;
		nwkStatusCurrent = emberAfNetworkState();
		if (nwkStatusCurrent == EMBER_JOINED_NETWORK)
		{
			emberLeaveNetwork();
		}
		halReboot();
		break;
	default:
		break;
	}
}
void MAIN_NetworkCallbackHandler(uint8_t networkResult)
{
	emberAfCorePrintln("MAIN_NetworkCallbackHandler");
	switch (networkResult)
	{
	case NETWORK_HAS_PARENT:
		// Nếu có thiết bị cha, đèn Led 1 sẽ nháy hồng 3 lần, bật cờ mạng sẵn sàng, và chuyển sang trạng thái báo cáo
		toggleLed(LED_1, ledPink, 3, 300, 300);
		networkReady = true;
		systemState = REPORT_STATE;
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
		break;
	case NETWORK_JOIN_SUCCESS:
		// Nếu gia nhập mạng thành công, đèn Led 1 sẽ nháy hồng 3 lần, bật cờ mạng sẵn sàng, và chuyển sang trạng thái báo cáo
		toggleLed(LED_1, ledPink, 3, 300, 300);
		networkReady = true;
		systemState = REPORT_STATE;
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
		break;
	case NETWORK_JOIN_FAIL:
		systemState = POWER_ON_STATE;
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
		break;
	case NETWORK_LOST_PARENT:
		// Nếu mất thiết bị cha, thiết bị sẽ nháy vàng 3 lần
		toggleLed(LED_1, ledYellow, 3, 300, 300);
		break;
	case NETWORK_OUT_NETWORK:
		if (networkReady)
		{
			// Nếu đang có mạng mà mất mạng, đèn Led 1 sẽ nháy đỏ 3 lần, sau đó chuyển về trạng thái khởi động đầu tiên
			emberAfCorePrintln("NETWORK_OUT_NETWORK");
			toggleLed(LED_1, ledRed, 3, 300, 300);
			networkReady = false;
			systemState = REBOOT_STATE;
			emberEventControlSetDelayMS(mainStateEventControl, 3000);
		}
		break;
	default:
		break;
	}
}
void buttonCallbackHandler(uint8_t button, buttonState_e state)
{
	if (button == BUTTON0)
	{
		switch (state)
		{
		case press_1:
			// Nếu nút 0 được bấm 1 lần, bật đèn Led 1, đồng thời báo cáo trạng thái đèn bật lên HC
			onLed(LED_1, ledBlue);
			SEND_OnOffStateReport(1, LED_ON);
			break;
		case press_2:
			// Nếu nút 0 được bấm 2 lần, tắt đèn Led 1, đồng thời báo cáo trạng thái tắt bật lên HC
			offLed(LED_1);
			SEND_OnOffStateReport(1, LED_OFF);
			break;
		case press_3:
			// Nếu nút 0 được bấm 3 lần, thiết lập endpoint 1 làm target cho binding
			emberAfPluginFindAndBindTargetStart(1);
			emberAfCorePrintln("Start Target Endpoint 1");
			break;
		case press_4:
			// Nếu nút 0 được bấm 4 lần, thiết lập endpoint 1 làm initiator cho binding
			emberAfPluginFindAndBindInitiatorStart(1);
			emberAfCorePrintln("Start Initiator Endpoint 1");
			break;
		case press_5:
			// Nếu nút 0 được bấm 5 lần, chuyển tới trạng thái reboot
			systemState = REBOOT_STATE;
			emberEventControlSetDelayMS(mainStateEventControl, 3000);
			break;
		case release:
			break;
		default:
			break;
		}
	}
	else if (button == BUTTON1)
	{
		switch (state)
		{
		case press_1:
			// Nếu nút 1 được bấm 1 lần, bật đèn Led 2, đồng thời báo cáo trạng thái đèn bật lên HC
			onLed(LED_2, ledBlue);
			SEND_OnOffStateReport(2, LED_ON);
			break;
		case press_2:
			// Nếu nút 1 được bấm 2 lần, tắt đèn Led 2, đồng thời báo cáo trạng thái tắt bật lên HC
			offLed(LED_2);
			SEND_OnOffStateReport(2, LED_OFF);
			break;
		case press_3:
			// Nếu nút 1 được bấm 3 lần, thiết lập endpoint 2 làm target cho binding
			emberAfPluginFindAndBindTargetStart(2);
			emberAfCorePrintln("Start Target Endpoint 2");
			break;
		case press_4:
			// Nếu nút 1 được bấm 4 lần, thiết lập endpoint 2 làm initiator cho binding
			emberAfPluginFindAndBindInitiatorStart(2);
			emberAfCorePrintln("Start Initiator Endpoint 2");
			break;
		case press_5:
			break;
		case release:
			break;
		default:
			break;
		}
	}
}
