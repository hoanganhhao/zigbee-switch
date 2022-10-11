/*
 * receive.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#ifndef SOURCE_APP_RECEIVE_RECEIVE_H_
#define SOURCE_APP_RECEIVE_RECEIVE_H_

#include "app/framework/include/af.h"
#include "Source/Mid/Led/led.h"
#include "Source/App/Send/send.h"

void RECEIVE_HandleOnOffCluster(EmberAfClusterCommand* cmd);
boolean emberAfPreMessageReceivedCallback(EmberAfIncomingMessage* incomingMessage);
boolean emberAfPreCommandReceivedCallback(EmberAfClusterCommand* cmd);
bool CheckCommand(EmberAfClusterCommand* cmd);

#endif /* SOURCE_APP_RECEIVE_RECEIVE_H_ */
