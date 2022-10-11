/*
 * receive.c
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#include "receive.h"

EmberBindingTableEntry binding; // Biến lưu giá trị từng hàng trong bảng Binding Table
uint16_t bindingNodeID, targetNodeID;
uint8_t initiatorEndpoint, targetEndpoint;
bool flagBinding = false;

void RECEIVE_HandleOnOffCluster(EmberAfClusterCommand* cmd)
{
	uint8_t commandID = cmd->commandId;
	uint8_t endPoint = cmd->apsFrame->destinationEndpoint;
	emberAfCorePrintln("RECEIVE_HandleOnOffCluster: Endpoint = %d, CommandID = %d", endPoint, commandID);
	if (CheckCommand(cmd) == true)
	{
		// Nếu hàm CheckCommand() trả về true, bắt đầu thực hiện lệnh bật/tắt đèn tùy theo Command ID
		if (endPoint == 1)
		{
			switch(commandID)
			{
			case ZCL_ON_COMMAND_ID:
				onLed(LED_1, ledBlue);
				SEND_OnOffStateReport(1, LED_ON);
				if (flagBinding == true)
				{
					// Nếu cờ flagBinding cũng được set, thực hiện chuyển tiếp lệnh sang Target
					emberAfFillCommandOnOffClusterOn();
					emberAfSetCommandEndpoints(initiatorEndpoint, targetEndpoint);
					emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, targetNodeID);
					flagBinding = false;
				}
				break;
			case ZCL_OFF_COMMAND_ID:
				offLed(LED_1);
				SEND_OnOffStateReport(1, LED_OFF);
				if (flagBinding == true)
				{
					// Nếu cờ flagBinding cũng được set, thực hiện chuyển tiếp lệnh sang Target
					emberAfFillCommandOnOffClusterOff();
					emberAfSetCommandEndpoints(initiatorEndpoint, targetEndpoint);
					emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, targetNodeID);
					flagBinding = false;
				}
				break;
			default:
				break;
			}
		}
		else if (endPoint == 2)
		{
			switch(commandID)
			{
			case ZCL_ON_COMMAND_ID:
				onLed(LED_2, ledBlue);
				SEND_OnOffStateReport(2, LED_ON);
				if (flagBinding == true)
				{
					// Nếu cờ flagBinding cũng được set, thực hiện chuyển tiếp lệnh sang Target
					emberAfFillCommandOnOffClusterOn();
					emberAfSetCommandEndpoints(initiatorEndpoint, targetEndpoint);
					emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, targetNodeID);
					flagBinding = false;
				}
				break;
			case ZCL_OFF_COMMAND_ID:
				offLed(LED_2);
				SEND_OnOffStateReport(2, LED_OFF);
				if (flagBinding == true)
				{
					// Nếu cờ flagBinding cũng được set, thực hiện chuyển tiếp lệnh sang Target
					emberAfFillCommandOnOffClusterOff();
					emberAfSetCommandEndpoints(initiatorEndpoint, targetEndpoint);
					emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, targetNodeID);
					flagBinding = false;
				}
				break;
			default:
				break;
			}
		}
	}
}
bool CheckCommand(EmberAfClusterCommand* cmd)
{
	uint8_t type = cmd->type; // Kiểu câu lệnh là Unicast, Multicast, hoặc Many To One
	uint16_t source = cmd->source; // Địa chỉ Node ID của nguồn gửi đến
	uint8_t endpoint = cmd->apsFrame->destinationEndpoint; // Endpoint nhận được lệnh điều khiển
	uint8_t bindingUnused = 0, bindingMulticast = 0, bindingManyToOne = 0, bindingUnicastOther = 0;
	bool flag = false;
	if (type == EMBER_INCOMING_UNICAST)
	{
		// Nếu câu lệnh thuộc kiểu Unicast, chạy vòng lặp để kiểm tra thông tin bảng Binding Table
		for (uint8_t i = 0; i < EMBER_BINDING_TABLE_SIZE; i++)
		{
			uint8_t status = emberGetBinding(i, &binding); // Lấy từng hàng trong bảng Binding Table
			if (status == EMBER_SUCCESS) // Nếu lấy thông tin thành công...
			{
				if (binding.type == EMBER_UNUSED_BINDING)
				{
					// Nếu binding này chưa sử dụng, tăng biến đếm bindingUnusedCount
					bindingUnused += 1;
				}
				else if (binding.type == EMBER_MULTICAST_BINDING)
				{
					// Nếu binding này thuộc kiểu Multicast, tăng biến đếm tương ứng
					bindingMulticast += 1;
				}
				else if (binding.type == EMBER_MANY_TO_ONE_BINDING)
				{
					// Nếu binding này thuộc kiểu Many To One, tăng biến đếm tương ứng
					bindingManyToOne += 1;
				}
				else
				{
					// Nếu binding này thuộc kiểu Unicast và được sử dụng, bắt đầu lấy NodeID ra kiểm tra
					bindingNodeID = emberGetBindingRemoteNodeId(i);
					if ((binding.clusterId == ZCL_ON_OFF_CLUSTER_ID) && (binding.local == endpoint))
					{
						if (bindingNodeID != source)
						{
							// Trong bảng Binding, nếu Cluster ID là của lệnh On-Off, endpoint khớp với endpoint nhận lệnh, và Node ID không phải của nguồn gửi...
							// Set cờ thực hiện lệnh, và cờ binding; đồng thời lưu các giá trị Endpoint và Node ID để chuyển tiếp lệnh sang Target
							flag = true;
							flagBinding = true;
							initiatorEndpoint = binding.local;
							targetEndpoint = binding.remote;
							targetNodeID = bindingNodeID;
						}
					}
					else
					{
						// Nếu Cluster ID hoặc endpoint sai khác, tăng biến đếm Unicast không khớp lệnh điều khiển
						bindingUnicastOther += 1;
					}
				}
			}
		}
		if (bindingUnused + bindingMulticast + bindingManyToOne + bindingUnicastOther == EMBER_BINDING_TABLE_SIZE)
		{
			// Sau khi chạy hết vòng lặp, nếu không có binding kiểu Unicast trùng khớp thông tin, vẫn set cờ thực hiện lệnh nhưng không chuyển tiếp
			flag = true;
		}
	}
	else if (type == EMBER_INCOMING_MULTICAST)
	{
		// Nếu câu lệnh nhận được thuộc kiểu Multicast thì set cờ thực hiện lệnh
		flag = true;
	}
	else if (type == EMBER_INCOMING_BROADCAST)
	{
		// Nếu câu lệnh nhận được thuộc kiểu Broadcast thì set cờ thực hiện lệnh
		flag = true;
	}
	return flag;
}
boolean emberAfPreMessageReceivedCallback(EmberAfIncomingMessage* incomingMessage)
{
	emberAfCorePrintln("emberAfPreMessageReceivedCallback");
	// Trừ trường hợp Cluster ID thuộc kiểu Active Endpoint Response, còn lại đều chuyển hàm emberAfPreCommandReceivedCallback xử lý
	if (incomingMessage->apsFrame->clusterId == ACTIVE_ENDPOINTS_RESPONSE)
	{
		emberAfCorePrintln("RSSI = %d", incomingMessage->lastHopRssi);
		return true;
	}
	return false;
}
boolean emberAfPreCommandReceivedCallback(EmberAfClusterCommand* cmd)
{
	// Hàm xử lý khi nhận được command
	emberAfCorePrintln("emberAfPreCommandReceivedCallback");
	if (cmd->clusterSpecific)
	{
		// Tùy theo Cluster ID để chuyển hàm xử lý tương ứng
		switch(cmd->apsFrame->clusterId)
		{
		case ZCL_ON_OFF_CLUSTER_ID:
			RECEIVE_HandleOnOffCluster(cmd);
			break;
		case ZCL_LEVEL_CONTROL_CLUSTER_ID:
			break;
		default:
			break;
		}
	}
	return false;
}
