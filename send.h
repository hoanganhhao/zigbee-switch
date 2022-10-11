/*
 * send.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#ifndef SOURCE_APP_SEND_SEND_H_
#define SOURCE_APP_SEND_SEND_H_

#include "app/framework/include/af.h"

#define MAX_DATA_COMMAND_SIZE			30
#define HC_ENDPOINT						1
#define HC_NETWORK_ADDRESS				0x0000 // Địa chỉ HC.

void SEND_ReportInfoToHc(void);
void SEND_FillBufferGlobalCommand(EmberAfClusterId clusterID, EmberAfAttributeId attributeId, uint8_t globalCommand, uint8_t* value, uint8_t length, uint8_t dataType);
void SEND_SendCommandUnicast(uint8_t source, uint8_t destination, uint16_t address);
void SEND_OnOffStateReport(uint8_t sendEndpoint, uint8_t ledState);
void SEND_LDRReport(uint8_t endpoint, uint32_t light);

#endif /* SOURCE_APP_SEND_SEND_H_ */
