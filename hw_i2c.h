#ifndef HW_I2C_H
#define HW_I2C_H
#include <inttypes.h>

#define LONG_TIMEOUT 5000
#define CRUISE_SET_TIMEOUT 2000
#define SHORT_TIMEOUT 1500

#define TEMP_DATA 0 
#define FUEL_DATA 1
#define CRUISE_DATA 2

void hw_i2c_setup(void);
void hw_i2c_sleep(void);
void drawOLED(void);
void drawSpeed(uint16_t value);
void drawCruiseSet(uint16_t value, uint8_t reDrawAll);
void drawCruise(uint16_t value, uint8_t isActive, uint8_t reDrawAll);
void drawFuel(uint16_t value, uint8_t reDrawAll);
void drawTemp(int8_t value, uint8_t reDrawAll);

void rotateDraw(void);
#endif