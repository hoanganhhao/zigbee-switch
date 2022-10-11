/*
 * send.c
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#include "send.h"

void SEND_ReportInfoToHc(void)
{
	uint8_t modelID[8] = { 7, 'S', 'W', '2', '_', 'L', 'M', '1' };
	uint8_t manufactureID[5] = { 4, 'L', 'u', 'm', 'i' };
	uint8_t version = 1;
	EmberNetworkStatus nwkStatusCurrent = emberAfNetworkState();
	if (nwkStatusCurrent != EMBER_JOINED_NETWORK)
	{
		// Nếu chưa gia nhập mạng thì không cần báo cáo
		return;
	}
	// Nếu đã gia nhập mạng, gửi thông tin về Model ID và ID nhà sản xuất cho HC
	SEND_FillBufferGlobalCommand(ZCL_BASIC_CLUSTER_ID, ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID, ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID, modelID, 8, ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
	SEND_SendCommandUnicast(1, HC_ENDPOINT, HC_NETWORK_ADDRESS);
	SEND_FillBufferGlobalCommand(ZCL_BASIC_CLUSTER_ID, ZCL_MANUFACTURER_NAME_ATTRIBUTE_ID, ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID, manufactureID, 5, ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
	SEND_SendCommandUnicast(1, HC_ENDPOINT, HC_NETWORK_ADDRESS);
}
void SEND_FillBufferGlobalCommand(EmberAfClusterId clusterID, EmberAfAttributeId attributeId, uint8_t globalCommand, uint8_t* value, uint8_t length, uint8_t dataType)
{
	// Hàm chuẩn bị lệnh gửi đi trong bộ đệm
	uint8_t data[MAX_DATA_COMMAND_SIZE];
	data[0] = (uint8_t) (attributeId & 0x00FF);
	data[1] = (uint8_t) ((attributeId & 0xFF00) >> 8);
	data[2] = EMBER_SUCCESS;
	data[3] = (uint8_t) dataType;
	memcpy(&data[4], value, length);
	(void) emberAfFillExternalBuffer(ZCL_GLOBAL_COMMAND | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER | ZCL_DISABLE_DEFAULT_RESPONSE_MASK, clusterID, globalCommand, "b", data, length + 4);
}
void SEND_SendCommandUnicast(uint8_t source, uint8_t destination, uint16_t address)
{
	// Hàm xác định điểm đi và điểm đến để thực hiện lệnh gửi
	emberAfSetCommandEndpoints (source, destination);
	(void) emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, address);
}
void SEND_OnOffStateReport(uint8_t endpoint, uint8_t ledState)
{
	// Hàm báo cáo trạng thái đèn led bật/tắt, bao gồm cập nhật Attribute trên server
	SEND_FillBufferGlobalCommand(ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID, ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID, (uint8_t*) &ledState, 1, ZCL_BOOLEAN_ATTRIBUTE_TYPE);
	SEND_SendCommandUnicast(endpoint, HC_ENDPOINT, HC_NETWORK_ADDRESS);
	emberAfWriteServerAttribute(endpoint, ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID, (uint8_t*) &ledState, ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}
void SEND_LDRReport(uint8_t endpoint, uint32_t light)
{
	// Hàm báo cáo giá trị LDR ghi nhận được, bao gồm cập nhật Attribute trên server
	SEND_FillBufferGlobalCommand(ZCL_ILLUM_MEASUREMENT_CLUSTER_ID, ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID, ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID, (uint32_t*) &light, 4, ZCL_INT32U_ATTRIBUTE_TYPE);
	SEND_SendCommandUnicast(endpoint, HC_ENDPOINT, HC_NETWORK_ADDRESS);
	emberAfWriteServerAttribute(endpoint, ZCL_ILLUM_MEASUREMENT_CLUSTER_ID, ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID, (uint32_t*) &light, ZCL_INT32U_ATTRIBUTE_TYPE);
}
