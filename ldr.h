/*
 * ldr.h
 *
 *  Created on: Sep 3, 2022
 *      Author: Admin
 */

#ifndef SOURCE_MID_LDR_LDR_H_
#define SOURCE_MID_LDR_LDR_H_

#include "app/framework/include/af.h"
#include <stdio.h>
#include "em_cmu.h"
#include "em_iadc.h"
#include "em_gpio.h"
#include "hal-config.h"
#include "kalman_filter.h"
#include "Source/Mid/Led/led.h"

#define CLK_SRC_ADC_FREQ            20000000
#define CLK_ADC_FREQ                10000000

#define IADC_INPUT_0_PORT_PIN     iadcPosInputPortCPin5;
#define IADC_INPUT_1_PORT_PIN     iadcNegInputGnd;

#define IADC_INPUT_0_BUS          CDBUSALLOC
#define IADC_INPUT_0_BUSALLOC     GPIO_CDBUSALLOC_CDODD0_ADC0
#define IADC_INPUT_1_BUS          CDBUSALLOC
#define IADC_INPUT_1_BUSALLOC     GPIO_CDBUSALLOC_CDEVEN0_ADC0

#define LIGHT_DELAY				1000 // Thời gian nghỉ giữa 2 lần đo ánh sáng là 1 giây
#define LIGHT_THRESHOLD			30 // Ngưỡng cập nhật giá trị đo là 30 lux

void ADCLight_Init(void);
uint32_t ADCLight(void);
uint32_t Get_Light(void);
void lightUpdateEventHandler(void);


#endif /* SOURCE_MID_LDR_LDR_H_ */
