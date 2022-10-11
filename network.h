/*
 * network.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#ifndef SOURCE_APP_NETWORK_NETWORK_H_
#define SOURCE_APP_NETWORK_NETWORK_H_

#include "app/framework/include/af.h"

typedef enum
{
	NETWORK_HAS_PARENT,
	NETWORK_JOIN_FAIL,
	NETWORK_JOIN_SUCCESS,
	NETWORK_LOST_PARENT,
	NETWORK_OUT_NETWORK
} networkResult_e; // Các trạng thái kết nối mạng

void NETWORK_FindAndJoin(void);
void joinNetworkEventHandler(void);
void NETWORK_StopFindAndJoin(void);
boolean emberAfStackStatusCallback(EmberStatus status);

#endif /* SOURCE_APP_NETWORK_NETWORK_H_ */
