#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <stddef.h>
#include "gyver_oled.h"
#include "hw_tick.h"

#include "hw.h"
#include "car.h"
#include "hw_i2c.h"

 
uint8_t isUpdateCruise = 0;

uint16_t Cruise = 0;

int8_t   LightOn = 0;

uint16_t lastSpeed = 0xFFFF;
uint16_t lastCruise = 0xFFFF;
uint8_t  lastCruiseActive = 0xFF;
uint16_t lastFuel = 0xFFFF;
int8_t   lastTemp = 0;

uint8_t currentView = TEMP_DATA;
uint8_t lastView = FUEL_DATA ;
uint32_t aTimer = 0;

const uint8_t frameCruise32[]  = {
	0x00, 0x04, 0x0E, 0x9C, 0xF8, 0xF0, 0xF0, 0xF8, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0x60, 0x70, 0xF0, 0xF0, 0x70, 0x60, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0x7C, 0x1E, 0x07, 0x0B, 0x1D, 0x39, 0x70, 0xE0, 0xC0, 0x80, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x07, 0x1E, 0x7C, 0xF8, 0xE0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x7F, 0xFF, 0xE6, 0x86, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x86, 0xE6, 0xFF, 0x7F, 0x0F, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0E, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x0E, 0x1C, 0x1C, 0x0E, 0x07, 0x03, 0x01, 0x00, 0x00, 
};
const uint8_t frameCruise24[]  = {
	0x02, 0x07, 0x2E, 0x3C, 0x38, 0x3C, 0x80, 0x40, 0x20, 0x20, 0x10, 0x10, 0x70, 0x70, 0x10, 0x10, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xFC, 0x33, 0x30, 0x00, 0x01, 0x03, 0x06, 0x3C, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const uint8_t frameFuel[]  = {
	0x00, 0xC0, 0xE0, 0x00, 0xFE, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3F, 0xE1, 0x81, 0x00, 0xFF, 0xE7, 0x5B, 0x5B, 0x87, 0xFF, 0x63, 0x6B, 0x9B, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x19, 0x1B, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const uint8_t frameTemp[]  = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x50, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x55, 0x55, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0E, 0x1B, 0x35, 0x2E, 0x2F, 0x2E, 0x35, 0x1B, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};



void hw_i2c_setup(void){
    #ifdef USE_SSD1306
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ , GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN , GPIO10 | GPIO11);
        //gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ, AFIO_MAPR_I2C2_REMAP);

        i2c_peripheral_disable(I2C2);
        i2c_set_clock_frequency(I2C2, I2C_CR2_FREQ_36MHZ);
        i2c_set_fast_mode(I2C2);
        i2c_set_ccr(I2C2, 0x1e);
        i2c_set_trise(I2C2, 0x0b);
        i2c_enable_ack(I2C2);
        i2c_set_own_7bit_slave_address(I2C2, 0x32);
        //i2c_set_speed(I2C2, i2c_speed_sm_100k, rcc_apb1_frequency/1000000);
        i2c_peripheral_enable(I2C2);        
        oled_init(0x3C);
    #endif
}

void hw_i2c_sleep(void){
    #ifdef USE_SSD1306
        oled_setPower(0);
        i2c_peripheral_disable(I2C2);
    #endif
}
void drawOLED(void){
    #ifdef USE_SSD1306
        drawSpeed(car_get_speed());
        rotateDraw();
        //;
    #endif
}

void drawSpeed(uint16_t value){
  if(isUpdateCruise == 0){
    if(value != lastSpeed){    
      lastSpeed = value;
      oled_clear(0,0,70,31);
      oled_setScale(4); 
      oled_setCursor(0, 0);
      oled_print_u16(value);
      oled_update(0,0,70,31);
    }
  }
}
void drawCruiseSet(uint16_t value, uint8_t reDrawAll){
  if(car_get_cruise_enable() == 1 && isUpdateCruise == 1){
    if(reDrawAll==1){
      lastCruise=value;
      lastCruiseActive = 1;
      oled_clear_all();
      oled_drawBitmap(0, 0, frameCruise32, 32, 32,0,0);
      oled_setScale(4); 
      oled_setCursorXY(40, 0);
      oled_print_u16(value);
      oled_update_all();
    }else{
      if(lastCruise != value ){
        lastCruise = value;
        lastCruiseActive = 1;
        oled_clear(32,0,127,31);
        oled_setScale(4); 
        oled_setCursorXY(40, 0);
        oled_print_u16(value);
        oled_update(32,0,127,31);
      }
    }
  }
} 
void drawCruise(uint16_t value, uint8_t isActive, uint8_t reDrawAll){
  if(isUpdateCruise == 0){    
    if(reDrawAll==1){
      lastCruise=value;
      lastCruiseActive = isActive;
      oled_clear(70,0,127,31);
      oled_drawBitmap(75, 0, frameCruise24, 24, 24,0,0);
      oled_setScale(2); 
      oled_setCursorXY(90, 15);

      if(value == 0)
        oled_print_str("---",3);
      else
        oled_print_u16(value);
      //oled_fastLineH(0,0,0,0);
      oled_update_all();
    } else {
      if(lastCruise != value || lastCruiseActive != isActive){
        lastCruise = value;
        lastCruiseActive = isActive;
        oled_clear(90,15,127,31);
        oled_setScale(2); 
        oled_setCursorXY(90, 15);
        
        if(value == 0)
          oled_print_str("---",3);
        else
          oled_print_u16(value);
        //oled_fastLineH(0,0,0,0);
        oled_update(70,0,127,31);
      }
    }
  }
}
void drawFuel(uint16_t value, uint8_t reDrawAll){
  if(isUpdateCruise == 0){    
    if(reDrawAll==1){
      oled_clear(70,0,127,31);
      oled_drawBitmap(75, 0, frameFuel, 24, 24,0,0);
      oled_clear(91,14,127,31);
      oled_setScale(2);     
      oled_setCursorXY(92, 15);
      oled_print_u16(value);
      lastFuel = value;
      oled_update_all();
    } else{
      if(lastFuel != value){
        lastFuel = value;
        oled_clear(91,14,127,31);
        oled_setScale(2);     
        oled_setCursorXY(92, 15);
        oled_print_u16(value);
        oled_update(91,14,127,31);
      }
    }
  }
 } 

 void drawTemp(int8_t value, uint8_t reDrawAll){
  if(isUpdateCruise == 0){    
    if(reDrawAll==1){
      oled_clear(70,0,127,31);
      oled_drawBitmap(75, 0, frameTemp, 24, 24,0,0);
      oled_setScale(2);     
      oled_setCursorXY(92, 15);
      oled_print_i8(value);
      lastTemp = value;
      oled_update_all();
    } else{
      if(lastTemp != value){
        lastTemp = value;
        oled_clear(92,15,127,31);
        oled_setScale(2);     
        oled_setCursorXY(92, 15);
        oled_print_i8(value);
        oled_update(92,15,127,31);
      }
    }
  }
 }

 void rotateDraw(void){
  if (isUpdateCruise == 0){
      uint8_t isCruiseEnable = car_get_cruise_enable();
        if(currentView != CRUISE_DATA && isCruiseEnable==1 && timer.millis-aTimer>SHORT_TIMEOUT){
          lastView = currentView;
          currentView++;
          aTimer=timer.millis;
          if(isCruiseEnable == 0 && currentView == CRUISE_DATA) currentView=0;          
          
        }
        
        if(currentView != CRUISE_DATA && isCruiseEnable==0 && timer.millis-aTimer>LONG_TIMEOUT){
          lastView = currentView;
          currentView++;
          aTimer=timer.millis;
          if(isCruiseEnable == 0 && currentView == CRUISE_DATA) currentView=0;       
          
        }

        if(currentView == CRUISE_DATA && timer.millis-aTimer>LONG_TIMEOUT){
          lastView = currentView;
          currentView=0;
          aTimer=timer.millis;    
            
        }
      

      switch(currentView){
        case TEMP_DATA:   drawTemp(car_get_temp(),!(currentView==lastView)); break;
        case FUEL_DATA:   drawFuel(car_get_fuel_level(),!(currentView==lastView));break;
        case CRUISE_DATA: drawCruise(car_get_cruise_speed(),car_get_cruise_active(),!(currentView==lastView));break;
        default : break;
      }
      lastView=currentView;
      
  }
}