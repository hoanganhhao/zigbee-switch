/*
 * network.c
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#include "network.h"

EmberEventControl joinNetworkEventControl;
uint8_t timeFindAndJoin = 0;
bool joinFlag = false;
extern boolean networkReady;

void NETWORK_FindAndJoin(void)
{
	EmberNetworkStatus nwkStatusCurrent;
	nwkStatusCurrent = emberAfNetworkState();
	if (nwkStatusCurrent == EMBER_NO_NETWORK)
	{
		// Nếu chưa có mạng, kích hoạt sự kiện tìm mạng sau 2 giây chờ
		emberEventControlSetDelayMS(joinNetworkEventControl, 2000);
	}
}
void joinNetworkEventHandler(void)
{
	EmberNetworkStatus nwkStatusCurrent;
	nwkStatusCurrent = emberAfNetworkState();
	if (nwkStatusCurrent == EMBER_NO_NETWORK)
	{
		// Bắt đầu tìm mạng, đồng thời đếm số lần thử tìm. Vì không tắt sự kiện, hàm handler này sẽ quay vòng liên tục cho đến khi tìm thấy mạng.
		emberAfPluginNetworkSteeringStart();
		timeFindAndJoin++;
	}
}
void NETWORK_StopFindAndJoin(void)
{
	EmberNetworkStatus nwkStatusCurrent;
	nwkStatusCurrent = emberAfNetworkState();
	if (nwkStatusCurrent == EMBER_JOINED_NETWORK)
	{
		// Nếu đã gia nhập mạng, dừng tìm mạng và tắt sự kiện tìm mạng.
		emberAfPluginNetworkSteeringStop();
		emberEventControlSetInactive(joinNetworkEventControl);
	}
}
boolean emberAfStackStatusCallback(EmberStatus status)
{
	emberAfCorePrintln("emberAfStackStatusCallback");
	if (status == EMBER_NETWORK_UP) // Stack báo trạng thái có mạng
	{
		if (timeFindAndJoin > 0)
		{
			// Nếu có mạng qua steering, gọi hàm tắt steering, và xử lý sự kiện gia nhập mạng
			NETWORK_StopFindAndJoin();
			MAIN_NetworkCallbackHandler(NETWORK_JOIN_SUCCESS);
		}
		else
		{
			// Nếu có sẵn mạng mà không cần steering, xử lý theo trường hợp có thiết bị cha
			MAIN_NetworkCallbackHandler(NETWORK_HAS_PARENT);
		}
	}
	else // Stack báo trạng thái không có mạng hoặc bị lỗi
	{
		EmberNetworkStatus nwkStatusCurrent = emberAfNetworkState();
		if (nwkStatusCurrent == EMBER_NO_NETWORK) // Trạng thái là không có mạng
		{
			emberAfCorePrintln("EMBER_NO_NETWORK");
			if (networkReady == false)
			{
				// Nếu chưa gia nhập mạng, xử lý theo trường hợp gia nhập mạng không thành công, tương đương trở lại giai đoạn đầu tìm mạng
				MAIN_NetworkCallbackHandler(NETWORK_JOIN_FAIL);
			}
			else
			{
				// Nếu đã gia nhập mạng, xử lý trường hợp mất mạng
				MAIN_NetworkCallbackHandler(NETWORK_OUT_NETWORK);
			}
		}
		else if (nwkStatusCurrent == EMBER_JOINED_NETWORK_NO_PARENT) // Trạng thái là mất thiết bị cha, xử lý trường hợp tương ứng
		{
			MAIN_NetworkCallbackHandler(NETWORK_LOST_PARENT);
		}
	}
	return false;
}
