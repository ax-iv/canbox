#include "hw_tick.h"

volatile tick_t timer = { 0, 0, 0, 0, 0, 0, 0, 0,0};

void hw_systick_callback(void)
{
	static uint16_t div_10000ms = 0;
	static uint16_t div_1000ms = 0;
	static uint16_t div_250ms = 0;
	static uint16_t div_100ms = 0;
	static uint16_t div_5ms = 0;

	timer.flag_tick = 1;
	timer.msec++;
	timer.millis++;
	

	if (++div_1000ms >= SEC_TO_TICK(1)) {

		div_1000ms = 0;
		timer.flag_1000ms = 1;
		timer.sec++;
		timer.msec = 0;
	}

	if (++div_10000ms >= SEC_TO_TICK(10)) {

		div_10000ms = 0;
		timer.flag_10000ms = 1;
		
	}

	if (++div_250ms >= MSEC_TO_TICK(250)) {

		div_250ms = 0;
		timer.flag_250ms = 1;
	}

	if (++div_100ms >= MSEC_TO_TICK(100)) {

		div_100ms = 0;
		timer.flag_100ms = 1;
	}

	if (++div_5ms >= MSEC_TO_TICK(5)) {

		div_5ms = 0;
		timer.flag_5ms = 1;
	}
}

