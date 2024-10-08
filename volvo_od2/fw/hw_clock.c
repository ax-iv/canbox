#include <libopencm3/stm32/rcc.h>

#include "hw_clock.h"

void hw_clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable AFIO clock. */
	rcc_periph_clock_enable(RCC_AFIO);
	#ifdef USE_SSD1306 
		rcc_periph_clock_enable(RCC_GPIOB);
		rcc_periph_clock_enable(RCC_I2C2);
	#endif
}

