/*
 * timer.c
 *
 *  Created on: Sep 5, 2022
 *      Author: Admin
 */

#include "timer.h"

uint64_t timerLoop = 0;

void TIMER3_Init(void)
{
	TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
	CMU_ClockEnable(cmuClock_TIMER3, true); // Cấp xung nhịp cho Timer 3
	timerInit.prescale = timerPrescale1024; // Thiết lập hệ số chia tần lớn nhất
	TIMER_Init(TIMER3, &timerInit); // Khởi tạo Timer 3
	TIMER_TopSet (TIMER3, 0xFFFFFFFF); // Thiết lập giá trị đếm tối đa lớn nhất có thể
	TIMER_IntEnable(TIMER3, TIMER_IF_OF); // Kích hoạt cờ ngắt với trạng thái đếm tràn giá trị tối đa
	NVIC_EnableIRQ(TIMER3_IRQn); // Kích hoạt hàm xử lý ngắt cho Timer 3
	TIMER_CounterSet(TIMER3, 0); // Thiết lập bắt đầu đếm từ 0
	TIMER_Enable(TIMER3, true); // Cho chạy Timer 3
}
void TIMER3_IRQHandler(void)
{
	// Mỗi khi hàm xử lý ngắt được gọi ra, biến đếm vòng lặp bộ đếm được tăng thêm 1
	TIMER_IntClear(TIMER3, TIMER_IF_OF);
	timerLoop++;
}
uint64_t Get_Millisecond(void)
{
	// Từ tần số đầu vào, hệ số chia tần, giá trị đếm tối đa và giá trị đếm hiện tại, có thể tính ra được số mili giây trôi qua kể từ lúc cấp nguồn thiết bị
	uint32_t freq = CMU_ClockFreqGet(cmuClock_EM01GRPACLK);
	freq /= (timerPrescale1024 + 1);
	uint32_t top = TIMER_TopGet(TIMER3);
	uint32_t counter = TIMER_CounterGet(TIMER3);
	float second = (float) (timerLoop * top + counter) / freq;
	uint64_t ms = second * 1000;
	return ms;
}
