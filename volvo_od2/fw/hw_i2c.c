#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <stm32-ssd1306/ssd1306/ssd1306.h>

#include "hw.h"
#include "hw_i2c.h"


void hw_i2c_setup(void){
    #ifdef USE_SSD1306
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ , GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN , GPIO10 | GPIO11);
        gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ, AFIO_MAPR_I2C2_REMAP);

        i2c_peripheral_disable(I2C2);
        i2c_set_own_7bit_slave_address(I2C2, 0x00);
        i2c_set_speed(I2C2, i2c_speed_sm_100k, rcc_apb1_frequency/1000000);
        i2c_peripheral_enable(I2C2);        
        ssd1306_Init();
    #endif
}

void hw_i2c_sleep(void){
    #ifdef USE_SSD1306
        i2c_peripheral_disable(I2C2);
    #endif
}