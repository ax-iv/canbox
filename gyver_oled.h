#ifndef GyverOLED_h
#define GyverOLED_h

// ===== константы =====
#define SSD1306_128x32 0
#define SSD1306_128x64 1
#define SSH1106_128x64 2

#define OLED_I2C 0

#define OLED_NO_BUFFER 0
#define OLED_BUFFER 1

#define OLED_CLEAR 0
#define OLED_FILL 1
#define OLED_STROKE 2

#define BUF_ADD 0
#define BUF_SUBTRACT 1
#define BUF_REPLACE 2

#define BITMAP_NORMAL 0
#define BITMAP_INVERT 1


// ===========================================================================

#include <libopencm3/stm32/i2c.h>
#include <stdlib.h>
#include "charMap.h"


// ============================ БЭКЭНД КОНСТАНТЫ ==============================
#define _IF_SB(i2c) ((I2C_SR1(i2c) & I2C_SR1_SB) == 0)
#define _IF_BTF(i2c) ((I2C_SR1(i2c) & I2C_SR1_BTF) == 0)
#define _IF_ADDR(i2c) ((I2C_SR1(i2c) & I2C_SR1_ADDR) == 0)
#define _IF_TxE(i2c) (I2C_SR1(i2c) & I2C_SR1_TxE) == 0
// внутренние константы
#define OLED_WIDTH 128
#define OLED_HEIGHT_32 0x02
#define OLED_HEIGHT_64 0x12
#define OLED_64 0x3F
#define OLED_32 0x1F

#define OLED_DISPLAY_OFF 0xAE
#define OLED_DISPLAY_ON 0xAF

#define OLED_COMMAND_MODE 0x00
#define OLED_ONE_COMMAND_MODE 0x80
#define OLED_DATA_MODE 0x40
#define OLED_ONE_DATA_MODE 0xC0

#define OLED_ADDRESSING_MODE 0x20
#define OLED_HORIZONTAL 0x00
#define OLED_VERTICAL 0x01

#define OLED_NORMAL_V 0xC8
#define OLED_FLIP_V 0xC0
#define OLED_NORMAL_H 0xA1
#define OLED_FLIP_H 0xA0

#define OLED_CONTRAST 0x81
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_CLOCKDIV 0xD5
#define OLED_SETMULTIPLEX 0xA8
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_CHARGEPUMP 0x8D

#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7

#define BUFSIZE_128x64 (128 * 64 / 8)
#define BUFSIZE_128x32 (128 * 32 / 8)

#define _bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define _bitSet(value, bit) ((value) |= (1UL << (bit)))
#define _bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define _bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define _bitWrite(value, bit, bitvalue) ((bitvalue) ? _bitSet(value, bit) : _bitClear(value, bit)) 

    // инициализация
    void oled_init( uint8_t address );

    // очистить дисплей
    void oled_clear_all(void);

    // очистить область
    void oled_clear(int x0, int y0, int x1, int y1) ;
    // яркость 0-255
    void oled_setContrast(uint8_t value);

    // вкл/выкл
    void oled_setPower(bool mode);

    // отразить по горизонтали
    void oled_flipH(bool mode);

    // инвертировать дисплей
    void oled_invertDisplay(bool mode);

    // отразить по вертикали
    void oled_flipV(bool mode);

    // ============================= ПЕЧАТЬ ==================================
    size_t oled_write(uint8_t data);   

    void oled_print_i8(int8_t data);
    
    void oled_print_u8(uint8_t data);
    
    void oled_print_u16(uint16_t data); 

    void oled_print_u32(uint32_t data);

    void oled_print_str(char* data, uint8_t len); 

    // автоматически переносить текст
    void oled_autoPrintln(bool mode);

    // отправить курсор в 0,0
    void oled_home(void);

    // поставить курсор для символа 0-127, 0-8(4)
    void oled_setCursor(int x, int y);

    // поставить курсор для символа 0-127, 0-63(31)
    void oled_setCursorXY(int x, int y);

    // масштаб шрифта (1-4)
    void oled_setScale(uint8_t scale) ;

    // инвертировать текст (0-1)
    void oled_invertText(bool inv);

    void oled_textMode(uint8_t mode);

    // возвращает true, если дисплей "кончился" - при побуквенном выводе
    bool oled_isEnd(void);

    // ================================== ГРАФИКА ==================================
    // точка (заливка 1/0)
    void oled_dot(int x, int y, uint8_t fill) ;

    // линия
    void oled_line(int x0, int y0, int x1, int y1, uint8_t fill) ;

    // горизонтальная линия
    void oled_fastLineH(int y, int x0, int x1, uint8_t fill) ;

    // вертикальная линия
    void oled_fastLineV(int x, int y0, int y1, uint8_t fill) ;

    // прямоугольник (лев. верхн, прав. нижн)
    void oled_rect(int x0, int y0, int x1, int y1, uint8_t fill) ;

    // прямоугольник скруглённый (лев. верхн, прав. нижн)
    void oled_roundRect(int x0, int y0, int x1, int y1, uint8_t fill) ;

    // окружность (центр х, центр у, радиус, заливка)
    void oled_circle(int x, int y, int radius, uint8_t fill) ;

    void oled_bezier(int* arr, uint8_t size, uint8_t dense, uint8_t fill) ;

    // вывести битмап
    void oled_drawBitmap(int x, int y, const uint8_t* frame, int width, int height, uint8_t invert, uint8_t mode ) ;

    // залить весь дисплей указанным байтом
    void oled_fill(uint8_t data);

    // шлёт байт в "столбик" setCursor() и setCursorXY()
    void oled_drawByte(uint8_t data) ;

    // вывести одномерный байтовый массив (линейный битмап высотой 8)
    void oled_drawBytes(uint8_t* data, uint8_t size) ;

    // ================================== СИСТЕМНОЕ ===================================
    // полностью обновить дисплей из буфера
    void oled_update_all(void);

    // выборочно обновить дисплей из буфера (x0, y0, x1, y1)
    void oled_update(int x0, int y0, int x1, int y1);

    // отправить байт по i2с или в буфер
    void oled_writeData(uint8_t data, uint8_t offsetY, uint8_t offsetX, uint8_t mode) ;

    // окно со сдвигом. x 0-127, y 0-63 (31), ширина в пикселях, высота в пикселях
    void oled_setWindowShift(int x0, int y0, int sizeX, int sizeY);



    bool oled_createBuffer(int x0, int y0, int x1, int y1, uint8_t fill) ;

    // отправить
    void oled_sendBuffer(void) ;

    // ========= ЛОУ-ЛЕВЕЛ ОТПРАВКА =========

    // супер-костыль для либы ire. Привет индусам!
    void oled_sendByte(uint8_t data) ;

    void oled_sendByteRaw(uint8_t data);

    // отправить команду
    void oled_sendCommand(uint8_t cmd1);

    // отправить код команды и команду
    void oled_sendCommand_cmd(uint8_t cmd1, uint8_t cmd2);

    // выбрать "окно" дисплея
    void oled_setWindow(int x0, int y0, int x1, int y1) ;

    void oled_beginData(void) ;

    void oled_beginCommand(void) ;

    void oled_beginOneCommand(void) ;

    void oled_endTransm(void) ;

    void oled_startTransm(void) ;

    // получить "столбик-байт" буквы
    uint8_t oled_getFont(uint8_t font, uint8_t row) ;

    

   
    // всякое
    
    // индекс в буфере
    int _bufIndex(int x, int y) ;

    void _swap(int x, int y);

    bool _inRange(int x, int mi, int ma) ;

    uint8_t constrain(const uint8_t x, const uint8_t a, const uint8_t b) ;


   
#endif