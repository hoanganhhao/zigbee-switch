/*
 * timer.h
 *
 *  Created on: Sep 5, 2022
 *      Author: Admin
 */

#ifndef SOURCE_MID_TIMER_TIMER_H_
#define SOURCE_MID_TIMER_TIMER_H_

#include "app/framework/include/af.h"
#include "em_timer.h"
#include "em_cmu.h"

void TIMER3_Init(void);
void TIMER3_IRQHandler(void);
uint64_t Get_Millisecond(void);

#endif /* SOURCE_MID_TIMER_TIMER_H_ */
