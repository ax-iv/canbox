#include <libopencm3/stm32/i2c.h>
#include <stdlib.h>
#include "charMap.h"
#include "gyver_oled.h"
#include <string.h>
#include <stdio.h>

/*
    Based on project: https://github.com/GyverLibs/GyverOLED/ v.1.6

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
*/


const uint8_t _charMap[][5]  = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, //   0x20 32
    {0x00, 0x00, 0x6f, 0x00, 0x00}, // ! 0x21 33
    {0x00, 0x07, 0x00, 0x07, 0x00}, // " 0x22 34
    {0x14, 0x7f, 0x14, 0x7f, 0x14}, // # 0x23 35
    {0x8C, 0x92, 0xFF, 0x92, 0x62}, // $ 0x24 36
    {0x23, 0x13, 0x08, 0x64, 0x62}, // % 0x25 37
    {0x36, 0x49, 0x56, 0x20, 0x50}, // & 0x26 38
    {0x00, 0x00, 0x07, 0x00, 0x00}, // ' 0x27 39
    {0x00, 0x1c, 0x22, 0x41, 0x00}, // ( 0x28 40
    {0x00, 0x41, 0x22, 0x1c, 0x00}, // ) 0x29 41
    {0x14, 0x08, 0x3e, 0x08, 0x14}, // * 0x2a 42
    {0x08, 0x08, 0x3e, 0x08, 0x08}, // + 0x2b 43
    {0x00, 0x50, 0x30, 0x00, 0x00}, // , 0x2c 44
    {0x08, 0x08, 0x08, 0x08, 0x08}, // - 0x2d 45
    {0x00, 0x60, 0x60, 0x00, 0x00}, // . 0x2e 46
    {0x20, 0x10, 0x08, 0x04, 0x02}, // / 0x2f 47
    {0x3e, 0x51, 0x49, 0x45, 0x3e}, // 0 0x30 48
    {0x00, 0x42, 0x7f, 0x40, 0x00}, // 1 0x31 49
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 0x32 50
    {0x21, 0x41, 0x45, 0x4b, 0x31}, // 3 0x33 51
    {0x18, 0x14, 0x12, 0x7f, 0x10}, // 4 0x34 52
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 0x35 53
    {0x3c, 0x4a, 0x49, 0x49, 0x30}, // 6 0x36 54
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 0x37 55
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 0x38 56
    {0x06, 0x49, 0x49, 0x29, 0x1e}, // 9 0x39 57
    {0x00, 0x36, 0x36, 0x00, 0x00}, // : 0x3a 58
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ; 0x3b 59
    {0x08, 0x14, 0x22, 0x41, 0x00}, // < 0x3c 60
    {0x14, 0x14, 0x14, 0x14, 0x14}, // = 0x3d 61
    {0x00, 0x41, 0x22, 0x14, 0x08}, // > 0x3e 62
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ? 0x3f 63
    {0x3e, 0x41, 0x5d, 0x49, 0x4e}, // @ 0x40 64
    {0x7e, 0x09, 0x09, 0x09, 0x7e}, // A 0x41 65
    {0x7f, 0x49, 0x49, 0x49, 0x36}, // B 0x42 66
    {0x3e, 0x41, 0x41, 0x41, 0x22}, // C 0x43 67
    {0x7f, 0x41, 0x41, 0x41, 0x3e}, // D 0x44 68
    {0x7f, 0x49, 0x49, 0x49, 0x41}, // E 0x45 69
    {0x7f, 0x09, 0x09, 0x09, 0x01}, // F 0x46 70
    {0x3e, 0x41, 0x49, 0x49, 0x7a}, // G 0x47 71
    {0x7f, 0x08, 0x08, 0x08, 0x7f}, // H 0x48 72
    {0x00, 0x41, 0x7f, 0x41, 0x00}, // I 0x49 73
    {0x20, 0x40, 0x41, 0x3f, 0x01}, // J 0x4a 74
    {0x7f, 0x08, 0x14, 0x22, 0x41}, // K 0x4b 75
    {0x7f, 0x40, 0x40, 0x40, 0x40}, // L 0x4c 76
    {0x7f, 0x02, 0x0c, 0x02, 0x7f}, // M 0x4d 77
    {0x7f, 0x04, 0x08, 0x10, 0x7f}, // N 0x4e 78
    {0x3e, 0x41, 0x41, 0x41, 0x3e}, // O 0x4f 79
    {0x7f, 0x09, 0x09, 0x09, 0x06}, // P 0x50 80
    {0x3e, 0x41, 0x51, 0x21, 0x5e}, // Q 0x51 81
    {0x7f, 0x09, 0x19, 0x29, 0x46}, // R 0x52 82
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S 0x53 83
    {0x01, 0x01, 0x7f, 0x01, 0x01}, // T 0x54 84
    {0x3f, 0x40, 0x40, 0x40, 0x3f}, // U 0x55 85
    {0x0f, 0x30, 0x40, 0x30, 0x0f}, // V 0x56 86
    {0x3f, 0x40, 0x30, 0x40, 0x3f}, // W 0x57 87
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X 0x58 88
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y 0x59 89
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z 0x5a 90
    {0x00, 0x00, 0x7f, 0x41, 0x00}, // [ 0x5b 91
    {0x02, 0x04, 0x08, 0x10, 0x20}, // \ 0x5c 92
    {0x00, 0x41, 0x7f, 0x00, 0x00}, // ] 0x5d 93
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^ 0x5e 94
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _ 0x5f 95
    {0x00, 0x00, 0x03, 0x04, 0x00}, // ` 0x60 96
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a 0x61 97
    {0x7f, 0x48, 0x44, 0x44, 0x38}, // b 0x62 98
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c 0x63 99
    {0x38, 0x44, 0x44, 0x48, 0x7f}, // d 0x64 100
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e 0x65 101
    {0x08, 0x7e, 0x09, 0x01, 0x02}, // f 0x66 102
    {0x0c, 0x52, 0x52, 0x52, 0x3e}, // g 0x67 103
    {0x7f, 0x08, 0x04, 0x04, 0x78}, // h 0x68 104
    {0x00, 0x44, 0x7d, 0x40, 0x00}, // i 0x69 105
    {0x20, 0x40, 0x44, 0x3d, 0x00}, // j 0x6a 106
    {0x00, 0x7f, 0x10, 0x28, 0x44}, // k 0x6b 107
    {0x00, 0x41, 0x7f, 0x40, 0x00}, // l 0x6c 108
    {0x7c, 0x04, 0x18, 0x04, 0x78}, // m 0x6d 109
    {0x7c, 0x08, 0x04, 0x04, 0x78}, // n 0x6e 110
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o 0x6f 111
    {0x7c, 0x14, 0x14, 0x14, 0x08}, // p 0x70 112
    {0x08, 0x14, 0x14, 0x18, 0x7c}, // q 0x71 113
    {0x7c, 0x08, 0x04, 0x04, 0x08}, // r 0x72 114
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s 0x73 115
    {0x04, 0x3f, 0x44, 0x40, 0x20}, // t 0x74 116
    {0x3c, 0x40, 0x40, 0x20, 0x7c}, // u 0x75 117
    {0x1c, 0x20, 0x40, 0x20, 0x1c}, // v 0x76 118
    {0x3c, 0x40, 0x30, 0x40, 0x3c}, // w 0x77 119
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x 0x78 120
    {0x0c, 0x50, 0x50, 0x50, 0x3c}, // y 0x79 121
    {0x44, 0x64, 0x54, 0x4c, 0x44}, // z 0x7a 122
    {0x00, 0x08, 0x36, 0x41, 0x41}, // { 0x7b 123
    {0x00, 0x00, 0x7f, 0x00, 0x00}, // | 0x7c 124
    {0x41, 0x41, 0x36, 0x08, 0x00}, // } 0x7d 125
    {0x04, 0x02, 0x04, 0x08, 0x04}, // ~ 0x7e 126
    
    {0x7E, 0x11, 0x11, 0x11, 0x7E},    //__А (0xC0).
    {0x7F, 0x49, 0x49, 0x49, 0x33},    //__Б (0xC1).
    {0x7F, 0x49, 0x49, 0x49, 0x36},    //__В (0xC2).
    {0x7F, 0x01, 0x01, 0x01, 0x03},    //__Г (0xC3).
    {0xE0, 0x51, 0x4F, 0x41, 0xFF},    //__Д (0xC4).
    {0x7F, 0x49, 0x49, 0x49, 0x41},    //__Е (0xC5).
    {0x77, 0x08, 0x7F, 0x08, 0x77},    //__Ж (0xC6).
    {0x41, 0x49, 0x49, 0x49, 0x36},    //__З (0xC7).
    {0x7F, 0x10, 0x08, 0x04, 0x7F},    //__И (0xC8).
    {0x7C, 0x21, 0x12, 0x09, 0x7C},    //__Й (0xC9).
    {0x7F, 0x08, 0x14, 0x22, 0x41},    //__К (0xCA).
    {0x20, 0x41, 0x3F, 0x01, 0x7F},    //__Л (0xCB).
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},    //__М (0xCC).
    {0x7F, 0x08, 0x08, 0x08, 0x7F},    //__Н (0xCD).
    {0x3E, 0x41, 0x41, 0x41, 0x3E},    //__О (0xCE).
    {0x7F, 0x01, 0x01, 0x01, 0x7F},    //__П (0xCF).
    {0x7F, 0x09, 0x09, 0x09, 0x06},    //__Р (0xD0).
    {0x3E, 0x41, 0x41, 0x41, 0x22},    //__С (0xD1).
    {0x01, 0x01, 0x7F, 0x01, 0x01},    //__Т (0xD2).
    {0x47, 0x28, 0x10, 0x08, 0x07},    //__У (0xD3).
    {0x1C, 0x22, 0x7F, 0x22, 0x1C},    //__Ф (0xD4).
    {0x63, 0x14, 0x08, 0x14, 0x63},    //__Х (0xD5).
    {0x7F, 0x40, 0x40, 0x40, 0xFF},    //__Ц (0xD6).
    {0x07, 0x08, 0x08, 0x08, 0x7F},    //__Ч (0xD7).
    {0x7F, 0x40, 0x7F, 0x40, 0x7F},    //__Ш (0xD8).
    {0x7F, 0x40, 0x7F, 0x40, 0xFF},    //__Щ (0xD9).
    {0x01, 0x7F, 0x48, 0x48, 0x30},    //__Ъ (0xDA).
    {0x7F, 0x48, 0x30, 0x00, 0x7F},    //__Ы (0xDB).
    {0x00, 0x7F, 0x48, 0x48, 0x30},    //__Ь (0xDC).
    {0x22, 0x41, 0x49, 0x49, 0x3E},    //__Э (0xDD).
    {0x7F, 0x08, 0x3E, 0x41, 0x3E},    //__Ю (0xDE).
    {0x46, 0x29, 0x19, 0x09, 0x7F},    //__Я (0xDF).

    {0x20, 0x54, 0x54, 0x54, 0x78},    //__а (0xE0).
    {0x3C, 0x4A, 0x4A, 0x49, 0x31},    //__б (0xE1).
    {0x7C, 0x54, 0x54, 0x28, 0x00},    //__в (0xE2).
    {0x7C, 0x04, 0x04, 0x0C, 0x00},    //__г (0xE3).
    {0xE0, 0x54, 0x4C, 0x44, 0xFC},    //__д (0xE4).
    {0x38, 0x54, 0x54, 0x54, 0x18},    //__е (0xE5).
    {0x6C, 0x10, 0x7C, 0x10, 0x6C},    //__ж (0xE6).
    {0x44, 0x54, 0x54, 0x28, 0x00},    //__з (0xE7).
    {0x7C, 0x20, 0x10, 0x08, 0x7C},    //__и (0xE8).
    {0x78, 0x42, 0x24, 0x12, 0x78},    //__й (0xE9).
    {0x7C, 0x10, 0x28, 0x44, 0x00},    //__к (0xEA).
    {0x20, 0x44, 0x3C, 0x04, 0x7C},    //__л (0xEB).
    {0x7C, 0x08, 0x10, 0x08, 0x7C},    //__м (0xEC).
    {0x7C, 0x10, 0x10, 0x10, 0x7C},    //__н (0xED).
    {0x38, 0x44, 0x44, 0x44, 0x38},    //__о (0xEE).
    {0x7C, 0x04, 0x04, 0x04, 0x7C},    //__п (0xEF).
    {0x7C, 0x14, 0x14, 0x14, 0x08},    //__р (0xF0).
    {0x38, 0x44, 0x44, 0x44, 0x00},    //__с (0xF1).
    {0x04, 0x04, 0x7C, 0x04, 0x04},    //__т (0xF2).
    {0x0C, 0x50, 0x50, 0x50, 0x3C},    //__у (0xF3).
    {0x30, 0x48, 0xFE, 0x48, 0x30},    //__ф (0xF4).
    {0x44, 0x28, 0x10, 0x28, 0x44},    //__х (0xF5).
    {0x7C, 0x40, 0x40, 0x7C, 0xC0},    //__ц (0xF6).
    {0x0C, 0x10, 0x10, 0x10, 0x7C},    //__ч (0xF7).
    {0x7C, 0x40, 0x7C, 0x40, 0x7C},    //__ш (0xF8).
    {0x7C, 0x40, 0x7C, 0x40, 0xFC},    //__щ (0xF9).
    {0x04, 0x7C, 0x50, 0x50, 0x20},    //__ъ (0xFA).
    {0x7C, 0x50, 0x50, 0x20, 0x7C},    //__ы (0xFB).
    {0x7C, 0x50, 0x50, 0x20, 0x00},    //__ь (0xFC).
    {0x28, 0x44, 0x54, 0x54, 0x38},    //__э (0xFD).
    {0x7C, 0x10, 0x38, 0x44, 0x38},    //__ю (0xFE).
    {0x08, 0x54, 0x34, 0x14, 0x7C},    //__я (0xFF). 
    {0x38, 0x55, 0x54, 0x55, 0x18},    //__ё (0xFF). 
    /*
    {0x7e, 0x09, 0x09, 0x09, 0x7e}, // А 192
    {0x7F, 0x49, 0x49, 0x49, 0x71}, // Б
    {0x7f, 0x49, 0x49, 0x49, 0x36}, // В
    {0x7F, 0x01, 0x01, 0x01, 0x01}, // Г
    {0x60, 0x3E, 0x21, 0x3F, 0x60}, // Д
    {0x7f, 0x49, 0x49, 0x49, 0x41}, // Е
    {0x76, 0x08, 0x7F, 0x08, 0x76}, // Ж
    {0x21, 0x41, 0x45, 0x4b, 0x31}, // З
    {0x7F, 0x20, 0x10, 0x08, 0x7F}, // И
    {0x7E, 0x20, 0x11, 0x08, 0x7E}, // Й
    {0x7f, 0x08, 0x14, 0x22, 0x41}, // К
    {0x70, 0x0E, 0x01, 0x01, 0x7F}, // Л
    {0x7f, 0x02, 0x0c, 0x02, 0x7f}, // М
    {0x7f, 0x08, 0x08, 0x08, 0x7f}, // Н
    {0x3e, 0x41, 0x41, 0x41, 0x3e}, // О
    {0x7F, 0x01, 0x01, 0x01, 0x7F}, // П
    {0x7f, 0x09, 0x09, 0x09, 0x06}, // Р
    {0x3e, 0x41, 0x41, 0x41, 0x22}, // С
    {0x01, 0x01, 0x7f, 0x01, 0x01}, // Т
    {0x07, 0x48, 0x48, 0x48, 0x7F}, // У
    {0x1C, 0x22, 0x7F, 0x22, 0x1C}, // Ф
    {0x63, 0x14, 0x08, 0x14, 0x63}, // Х
    {0x7F, 0x40, 0x40, 0x7F, 0xC0}, // Ц
    {0x07, 0x08, 0x08, 0x08, 0x7F}, // Ч
    {0x7F, 0x40, 0x7F, 0x40, 0x7F}, // Ш
    {0x7F, 0x40, 0x7F, 0x40, 0xFF}, // Щ
    {0x01, 0x7F, 0x48, 0x48, 0x70}, // Ъ
    {0x7F, 0x48, 0x70, 0x00, 0x7F}, // Ы
    {0x00, 0x7F, 0x48, 0x48, 0x70}, // Ь
    {0x22, 0x41, 0x49, 0x49, 0x3E}, // Э
    {0x7F, 0x08, 0x3E, 0x41, 0x3E}, // Ю
    {0x46, 0x29, 0x19, 0x09, 0x7F}, // Я 223

    {0x20, 0x54, 0x54, 0x54, 0x78}, //a 224
    {0x3c, 0x4a, 0x4a, 0x49, 0x31}, //б
    {0x7c, 0x54, 0x54, 0x28, 0x00}, //в
    {0x7c, 0x04, 0x04, 0x04, 0x0c}, //г
    {0xe0, 0x54, 0x4c, 0x44, 0xfc}, //д
    {0x38, 0x54, 0x54, 0x54, 0x18}, //e
    {0x6c, 0x10, 0x7c, 0x10, 0x6c}, //ж
    {0x44, 0x44, 0x54, 0x54, 0x28}, //з
    {0x7c, 0x20, 0x10, 0x08, 0x7c}, //и
    {0x7c, 0x41, 0x22, 0x11, 0x7c}, //й
    {0x7c, 0x10, 0x28, 0x44, 0x00}, //к
    {0x20, 0x44, 0x3c, 0x04, 0x7c}, //л
    {0x7c, 0x08, 0x10, 0x08, 0x7c}, //м
    {0x7c, 0x10, 0x10, 0x10, 0x7c}, //н
    {0x38, 0x44, 0x44, 0x44, 0x38}, //o
    {0x7c, 0x04, 0x04, 0x04, 0x7c}, //п
    {0x7C, 0x14, 0x14, 0x14, 0x08}, //p
    {0x38, 0x44, 0x44, 0x44, 0x20}, //c
    {0x04, 0x04, 0x7c, 0x04, 0x04}, //т
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, //у
    {0x30, 0x48, 0xfc, 0x48, 0x30}, //ф
    {0x44, 0x28, 0x10, 0x28, 0x44}, //x
    {0x7c, 0x40, 0x40, 0x40, 0xfc}, //ц
    {0x0c, 0x10, 0x10, 0x10, 0x7c}, //ч
    {0x7c, 0x40, 0x7c, 0x40, 0x7c}, //ш
    {0x7c, 0x40, 0x7c, 0x40, 0xfc}, //щ
    {0x04, 0x7c, 0x50, 0x50, 0x20}, //ъ
    {0x7c, 0x50, 0x50, 0x20, 0x7c}, //ы
    {0x7c, 0x50, 0x50, 0x20, 0x00}, //ь
    {0x28, 0x44, 0x54, 0x54, 0x38}, //э
    {0x7c, 0x10, 0x38, 0x44, 0x38}, //ю
    {0x08, 0x54, 0x34, 0x14, 0x7c}, //я 255
*/
};




// ==================== ПЕРЕМЕННЫЕ И КОНСТАНТЫ ====================
    uint8_t _address = 0x3C;
    const uint8_t _TYPE = 0;
    const uint8_t _BUFF = 1;
    const uint8_t _maxRow = (_TYPE ? 8 : 4) - 1;
    const uint8_t _maxY = (_TYPE ? 64 : 32) - 1;
    const uint8_t _maxX = OLED_WIDTH - 1;  // на случай добавления мелких дисплеев

    // софт. буфер
    //const int _bufSize = ((_BUFF == 1) ? (_TYPE ? BUFSIZE_128x64 : BUFSIZE_128x32) : 0);
    const int _bufSize = BUFSIZE_128x32;
    //uint8_t _oled_buffer[((_BUFF == 1) ? (_TYPE ? BUFSIZE_128x64 : BUFSIZE_128x32) : 0)];
    uint8_t _oled_buffer[BUFSIZE_128x32];

    bool _invState = 0;
    bool _println = false;
    bool _getn = false;
    uint8_t _scaleX = 1, _scaleY = 8;
    int8_t _x = 0, _y = 0;
    uint8_t _shift = 0;
    uint8_t _lastChar;
    uint8_t _writes = 0;
    uint8_t _mode = 2;

    // дин. буфер
    int _bufsizeX, _bufsizeY;
    int _bufX, _bufY;
    uint8_t* _buf_ptr;
    bool _buf_flag = false;



// список инициализации
static const uint8_t _oled_init[]  = {
    OLED_DISPLAY_OFF,
    OLED_CLOCKDIV,
    0x80,  // value
    OLED_CHARGEPUMP,
    0x14,  // value
    OLED_ADDRESSING_MODE,
    OLED_VERTICAL,
    OLED_NORMAL_H,
    OLED_NORMAL_V,
    OLED_CONTRAST,
    0x7F,  // value
    OLED_SETVCOMDETECT,
    0x40,  // value
    OLED_NORMALDISPLAY,
    OLED_DISPLAY_ON,
};

    // инициализация
    void oled_init( uint8_t address ) {
        
        //ire.begin();
        _address = address;
        

        oled_beginCommand();
        for (uint8_t i = 0; i < 15; i++) oled_sendByte(_oled_init[i]);
        oled_endTransm();
        oled_beginCommand();
        oled_sendByte(OLED_SETCOMPINS);
        oled_sendByte(_TYPE ? OLED_HEIGHT_64 : OLED_HEIGHT_32);
        oled_sendByte(OLED_SETMULTIPLEX);
        oled_sendByte(_TYPE ? OLED_64 : OLED_32);
        oled_endTransm();

        oled_setCursorXY(0, 0);
        if (_BUFF) oled_setWindow(0, 0, _maxX, _maxRow);  // для буфера включаем всю область
    }

    // очистить дисплей
    void oled_clear_all(void) { oled_fill(0); }

    // очистить область
    void oled_clear(int x0, int y0, int x1, int y1) {
        if (_TYPE < 2) {  // для SSD1306
            if (_BUFF) oled_rect(x0, y0, x1, y1, OLED_CLEAR);
            else {
                x1++;
                y1++;
                y0 >>= 3;
                y1 = (y1 - 1) >> 3;
                y0 = constrain(y0, 0, _maxRow);
                y1 = constrain(y1, 0, _maxRow);
                x0 = constrain(x0, 0, _maxX);
                x1 = constrain(x1, 0, _maxX);
                oled_setWindow(x0, y0, x1, y1);
                oled_beginData();
                for (int x = x0; x < x1; x++)
                    for (int y = y0; y < y1 + 1; y++)
                        oled_writeData(0, y, x, 2);
                oled_endTransm();
            }
        } else {
            // для SSH1108
        }
        oled_setCursorXY(_x, _y);
    }

    // яркость 0-255
    void oled_setContrast(uint8_t value) { oled_sendCommand_cmd(OLED_CONTRAST, value); }

    // вкл/выкл
    void oled_setPower(bool mode) { oled_sendCommand(mode ? OLED_DISPLAY_ON : OLED_DISPLAY_OFF); }

    // отразить по горизонтали
    void oled_flipH(bool mode) { oled_sendCommand(mode ? OLED_FLIP_H : OLED_NORMAL_H); }

    // инвертировать дисплей
    void oled_invertDisplay(bool mode) { oled_sendCommand(mode ? OLED_INVERTDISPLAY : OLED_NORMALDISPLAY); }

    // отразить по вертикали
    void oled_flipV(bool mode) { oled_sendCommand(mode ? OLED_FLIP_V : OLED_NORMAL_V); }

    // ============================= ПЕЧАТЬ ==================================
    size_t oled_write(uint8_t data) {
         // переносы и пределы
        bool newPos = false;
        if (data == '\r') {
            _x = 0;
            newPos = true;
            data = 0;
        }  // получен возврат каретки
        if (data == '\n') {
            _y += _scaleY;
            newPos = true;
            data = 0;
            _getn = 1;
        }  // получен перевод строки
        if (_println && (_x + 6 * _scaleX) >= _maxX) {
            _x = 0;
            _y += _scaleY;
            newPos = true;
        }                                        // строка переполненена, перевод и возврат
        if (newPos) oled_setCursorXY(_x, _y);         // переставляем курсор
        if (_y + _scaleY > _maxY + 1) data = 0;  // дисплей переполнен
        if (_getn && _println && data == ' ' && _x == 0) {
            _getn = 0;
            data = 0;
        }  // убираем первый пробел в строке

        // фикс русских букв и некоторых символов
        if (data > 127) {
            uint8_t thisData = data;
            // data = 0 - флаг на пропуск
            if (data > 191) data = 0;
            else if (_lastChar == 209 && data == 145) data = 192;  // ё кастомная
            else if (_lastChar == 208 && data == 129) data = 149;  // Е вместо Ё
            else if (_lastChar == 226 && data == 128) data = 0;    // тире вместо длинного тире (начало)
            else if (_lastChar == 128 && data == 148) data = 45;   // тире вместо длинного тире
            _lastChar = thisData;
        }
        if (data == 0) return 1;
        // если тут не вылетели - печатаем символ

        if (_TYPE < 2 || 1) {  // для SSD1306
            int newX = _x + _scaleX * 6;
            if (newX < 0 || _x > _maxX) _x = newX;  // пропускаем вывод "за экраном"
            else {
                if (!_BUFF) oled_beginData();
                for (uint8_t col = 0; col < 6; col++) {                        // 6 стобиков буквы
                    uint8_t bits = oled_getFont(data, col);                         // получаем байт
                    if (_invState) bits = ~bits;                               // инверсия
                    if (_scaleX == 1) {                                        // если масштаб 1
                        if (_x >= 0 && _x <= _maxX) {                          // внутри дисплея
                            if (_shift == 0) {                                 // если вывод без сдвига на строку
                                oled_writeData(bits, 0, 0, _mode);                  // выводим
                            } else {                                           // со сдвигом
                                oled_writeData(bits << _shift, 0, 0, _mode);        // верхняя часть
                                oled_writeData(bits >> (8 - _shift), 1, 0, _mode);  // нижняя часть
                            }
                        }
                    } else {                   // масштаб 2, 3 или 4 - растягиваем шрифт
                        uint32_t newData = 0;  // буфер
                        for (uint8_t i = 0, count = 0; i < 8; i++)
                            for (uint8_t j = 0; j < _scaleX; j++, count++)
                                _bitWrite(newData, count, _bitRead(bits, i));  // пакуем растянутый шрифт

                        for (uint8_t i = 0; i < _scaleX; i++) {  // выводим. По Х
                            uint8_t prevData = 0;
                            if (_x + i >= 0 && _x + i <= _maxX)                                                 // внутри дисплея
                                for (uint8_t j = 0; j < _scaleX; j++) {                                         // выводим. По Y
                                    uint8_t data = newData >> (j * 8);                                             // получаем кусок буфера
                                    if (_shift == 0) {                                                          // если вывод без сдвига на строку
                                        oled_writeData(data, j, i, _mode);                                           // выводим
                                    } else {                                                                    // со сдвигом
                                        oled_writeData((prevData >> (8 - _shift)) | (data << _shift), j, i, _mode);  // склеиваем и выводим
                                        prevData = data;                                                        // запоминаем предыдущий
                                    }
                                }
                            if (_shift != 0) oled_writeData(prevData >> (8 - _shift), _scaleX, i, _mode);  // выводим нижний кусочек, если со сдвигом
                        }
                    }
                    _x += _scaleX;  // двигаемся на ширину пикселя (1-4)
                }
                if (!_BUFF) oled_endTransm();
            }
        } else {
            // для SSH1106
        }

        return 1;
    }

    void printNumber(uint32_t data, uint8_t base){
        char buf[8 * sizeof(uint32_t) + 1]; // Assumes 8-bit chars plus zero byte.
        char *str = &buf[sizeof(buf) - 1];
        *str = '\0';
        if (base < 2) base = 10;

        do {
            char c = data % base;
            data /= base;
            *--str = c < 10 ? c + '0' : c + 'A' - 10;
        } while(data);
        oled_print_str(str,sizeof(buf));
    }

    void oled_print_i8(int8_t data){
        if (data < 0) {
            int t = oled_write('-');
            data = -data;
        }
        printNumber(data,10);
    }
    
    void oled_print_u8(uint8_t data){
        printNumber(data,10);
    }
    
    void oled_print_u16(uint16_t data){
        printNumber(data,10);
    } 

    void oled_print_u32(uint32_t data){
        printNumber(data,10);
    }

    void oled_print_str(char* data, uint8_t len){
        for(uint8_t i=0; i < len; i++)
            oled_write((uint8_t)data[i]);
    } 

    // автоматически переносить текст
    void oled_autoPrintln(bool mode) { _println = mode; }

    // отправить курсор в 0,0
    void oled_home() { oled_setCursorXY(0, 0); }

    // поставить курсор для символа 0-127, 0-8(4)
    void oled_setCursor(int x, int y) { oled_setCursorXY(x, y << 3); }

    // поставить курсор для символа 0-127, 0-63(31)
    void oled_setCursorXY(int x, int y) {
        _x = x;
        _y = y;
        oled_setWindowShift(x, y, _maxX, _scaleY);
    }

    // масштаб шрифта (1-4)
    void oled_setScale(uint8_t scale) {
        scale = constrain(scale, 1, 4);  // защита от нечитающих доку
        _scaleX = scale;
        _scaleY = scale * 8;
        oled_setCursorXY(_x, _y);
    }

    // инвертировать текст (0-1)
    void oled_invertText(bool inv) { _invState = inv; }

    void oled_textMode(uint8_t mode) { _mode = mode; }

    // возвращает true, если дисплей "кончился" - при побуквенном выводе
    bool oled_isEnd() { return (_y > _maxRow); }

    // ================================== ГРАФИКА ==================================
    // точка (заливка 1/0)
    void oled_dot(int x, int y, uint8_t fill) {
        if (x < 0 || x > _maxX || y < 0 || y > _maxY) return;
        _x = 0;
        _y = 0;
        if (_BUFF) {
            _bitWrite(_oled_buffer[_bufIndex(x, y)], y & 0b111, fill);
        } else {
            if (!_buf_flag) {
                oled_setWindow(x, y >> 3, _maxX, _maxRow);
                oled_beginData();
                oled_sendByte(!!fill << (y & 0b111));  // задвигаем 1 на высоту y
                oled_endTransm();
            } else {
                x -= _bufX;
                y -= _bufY;
                if (x < 0 || x > _bufsizeX || y < 0 || y > (_bufsizeY << 3)) return;
                _bitWrite(_buf_ptr[(y >> 3) + x * _bufsizeY], y & 0b111, fill);
            }
        }
    }

    // линия
    void oled_line(int x0, int y0, int x1, int y1, uint8_t fill) {
        _x = 0;
        _y = 0;
        if (x0 == x1) oled_fastLineV(x0, y0, y1, fill);
        else if (y0 == y1) oled_fastLineH(y0, x0, x1, fill);
        else {
            int sx, sy, e2, err;
            int dx = abs(x1 - x0);
            int dy = abs(y1 - y0);
            sx = (x0 < x1) ? 1 : -1;
            sy = (y0 < y1) ? 1 : -1;
            err = dx - dy;
            for (;;) {
                oled_dot(x0, y0, fill);
                if (x0 == x1 && y0 == y1) return;
                e2 = err << 1;
                if (e2 > -dy) {
                    err -= dy;
                    x0 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y0 += sy;
                }
            }
        }
    }

    // горизонтальная линия
    void oled_fastLineH(int y, int x0, int x1, uint8_t fill) {
        _x = 0;
        _y = 0;
        if (x0 > x1) _swap(x0, x1);
        if (y < 0 || y > _maxY) return;
        if (x0 == x1) {
            oled_dot(x0, y, fill);
            return;
        }
        x1++;
        x0 = constrain(x0, 0, _maxX);
        x1 = constrain(x1, 0, _maxX);
        if (_BUFF) {
            for (int x = x0; x < x1; x++) oled_dot(x, y, fill);
        } else {
            if (_buf_flag) {
                for (int x = x0; x < x1; x++) oled_dot(x, y, fill);
                return;
            }
            uint8_t data = 0b1 << (y & 0b111);
            y >>= 3;
            oled_setWindow(x0, y, x1, y);
            oled_beginData();
            for (int x = x0; x < x1; x++) oled_writeData(data, y, x, 0);
            oled_endTransm();
        }
    }

    // вертикальная линия
    void oled_fastLineV(int x, int y0, int y1, uint8_t fill) {
        _x = 0;
        _y = 0;
        if (y0 > y1) _swap(y0, y1);
        if (x < 0 || x > _maxX) return;
        if (y0 == y1) {
            oled_dot(x, y0, fill);
            return;
        }
        y1++;
        if (_BUFF) {
            for (int y = y0; y < y1; y++) oled_dot(x, y, fill);
        } else {
            if (_buf_flag) {
                for (int y = y0; y < y1; y++) oled_dot(x, y, fill);
                return;
            }
            if (fill) fill = 255;
            uint8_t shift = y0 & 0b111;
            uint8_t shift2 = 8 - (y1 & 0b111);
            if (shift2 == 8) shift2 = 0;
            int height = y1 - y0;
            y0 >>= 3;
            y1 = (y1 - 1) >> 3;
            uint8_t numBytes = y1 - y0;
            oled_setWindow(x, y0, x, y1);

            oled_beginData();
            if (numBytes == 0) {
                if (_inRange(y0, 0, _maxRow)) oled_writeData((fill >> (8 - height)) << shift, y0, x, 0);
            } else {
                if (_inRange(y0, 0, _maxRow)) oled_writeData(fill << shift, y0, x, 0);  // начальный кусок
                y0++;
                for (uint8_t i = 0; i < numBytes - 1; i++, y0++)
                    if (_inRange(y0, 0, _maxRow)) oled_writeData(fill, y0, x, 0);        // столбик
                if (_inRange(y0, 0, _maxRow)) oled_writeData(fill >> shift2, y0, x, 0);  // нижний кусок
            }
            oled_endTransm();
        }
    }

    // прямоугольник (лев. верхн, прав. нижн)
    void oled_rect(int x0, int y0, int x1, int y1, uint8_t fill) {
        _x = 0;
        _y = 0;
        if (x0 > x1) _swap(x0, x1);
        if (y0 > y1) _swap(y0, y1);
        if (fill == OLED_STROKE) {
            oled_fastLineH(y0, x0 + 1, x1 - 1, 1);
            oled_fastLineH(y1, x0 + 1, x1 - 1, 1);
            oled_fastLineV(x0, y0, y1, 1);
            oled_fastLineV(x1, y0, y1, 1);
        } else {
            if (x0 == x1 && y0 == y1) {
                oled_dot(x0, y0, fill);
                return;
            }
            if (x0 == x1) {
                oled_fastLineV(x0, y0, y1, fill);
                return;
            }
            if (y0 == y1) {
                oled_fastLineH(y0, x0, x1, fill);
                return;
            }
            if (!_BUFF && fill == OLED_CLEAR) {
                oled_clear(x0, y0, x1, y1);
                return;
            }

            // если рисуем в мини-буфер
            if (_buf_flag) {
                x0 = constrain(x0, 0, _maxX);
                x1 = constrain(x1, 0, _maxX);
                for (uint8_t x = x0; x <= x1; x++) oled_fastLineV(x, y0, y1, fill == OLED_FILL ? 1 : 0);
                return;
            }
            uint8_t thisFill = (fill == OLED_FILL ? 0 : 1);
            // рисуем в олед и в большой буфер
            x1++;
            y1++;
            uint8_t shift = y0 & 0b111;
            uint8_t shift2 = 8 - (y1 & 0b111);
            if (shift2 == 8) shift2 = 0;
            int height = y1 - y0;
            y0 >>= 3;
            y1 = (y1 - 1) >> 3;
            uint8_t numBytes = y1 - y0;
            x0 = constrain(x0, 0, _maxX);
            x1 = constrain(x1, 0, _maxX);

            if (!_BUFF) oled_setWindow(x0, y0, x1, y1);
            if (!_BUFF) oled_beginData();
            for (uint8_t x = x0; x <= x1; x++) {
                int y = y0;
                if (numBytes == 0) {
                    if (_inRange(y, 0, _maxRow)) oled_writeData((255 >> (8 - height)) << shift, y, x, thisFill);
                } else {
                    if (_inRange(y, 0, _maxRow)) oled_writeData(255 << shift, y, x, thisFill);  // начальный кусок
                    y++;
                    for (uint8_t i = 0; i < numBytes - 1; i++, y++)
                        if (_inRange(y, 0, _maxRow)) oled_writeData(255, y, x, thisFill);        // столбик
                    if (_inRange(y, 0, _maxRow)) oled_writeData(255 >> shift2, y, x, thisFill);  // нижний кусок
                }
            }
            if (!_BUFF) oled_endTransm();
        }
    }

    // прямоугольник скруглённый (лев. верхн, прав. нижн)
    void oled_roundRect(int x0, int y0, int x1, int y1, uint8_t fill) {
        if (fill == OLED_STROKE) {
            oled_fastLineV(x0, y0 + 2, y1 - 2, 1);
            oled_fastLineV(x1, y0 + 2, y1 - 2, 1);
            oled_fastLineH(y0, x0 + 2, x1 - 2, 1);
            oled_fastLineH(y1, x0 + 2, x1 - 2, 1);
            oled_dot(x0 + 1, y0 + 1, 1);
            oled_dot(x1 - 1, y0 + 1, 1);
            oled_dot(x1 - 1, y1 - 1, 1);
            oled_dot(x0 + 1, y1 - 1, 1);
        } else {
            oled_fastLineV(x0, y0 + 2, y1 - 2, fill);
            oled_fastLineV(x0 + 1, y0 + 1, y1 - 1, fill);
            oled_fastLineV(x1 - 1, y0 + 1, y1 - 1, fill);
            oled_fastLineV(x1, y0 + 2, y1 - 2, fill);
            oled_rect(x0 + 2, y0, x1 - 2, y1, fill);
        }
    }

    // окружность (центр х, центр у, радиус, заливка)
    void oled_circle(int x, int y, int radius, uint8_t fill ) {
        int f = 1 - radius;
        int ddF_x = 1;
        int ddF_y = -2 * radius;
        int x1 = 0;
        int y1 = radius;

        uint8_t fillLine = (fill == OLED_CLEAR) ? 0 : 1;
        oled_dot(x, y + radius, fillLine);
        oled_dot(x, y - radius, fillLine);
        oled_dot(x + radius, y, fillLine);
        oled_dot(x - radius, y, fillLine);
        // if (fill != OLED_STROKE) fastLineH(y, x - radius, x + radius-1, fillLine);
        if (fill != OLED_STROKE) oled_fastLineV(x, y - radius, y + radius - 1, fillLine);
        while (x1 < y1) {
            if (f >= 0) {
                y1--;
                ddF_y += 2;
                f += ddF_y;
            }
            x1++;
            ddF_x += 2;
            f += ddF_x;
            if (fill == OLED_STROKE) {
                oled_dot(x + x1, y + y1,1);
                oled_dot(x - x1, y + y1,1);
                oled_dot(x + x1, y - y1,1);
                oled_dot(x - x1, y - y1,1);
                oled_dot(x + y1, y + x1,1);
                oled_dot(x - y1, y + x1,1);
                oled_dot(x + y1, y - x1,1);
                oled_dot(x - y1, y - x1,1);
            } else {  // FILL / CLEAR

                oled_fastLineV(x + x1, y - y1, y + y1, fillLine);
                oled_fastLineV(x - x1, y - y1, y + y1, fillLine);
                oled_fastLineV(x + y1, y - x1, y + x1, fillLine);
                oled_fastLineV(x - y1, y - x1, y + x1, fillLine);

            }
        }
    }
    void oled_bezier(int* arr, uint8_t size, uint8_t dense, uint8_t fill) {
        int a[size * 2];
        for (int i = 0; i < (1 << dense); i++) {
            for (int j = 0; j < size * 2; j++) a[j] = arr[j];
            for (int j = (size - 1) * 2 - 1; j > 0; j -= 2)
                for (int k = 0; k <= j; k++)
                    a[k] = a[k] + (((a[k + 2] - a[k]) * i) >> dense);
            oled_dot(a[0], a[1], fill);
        }
    }

    // вывести битмап
    void oled_drawBitmap(int x, int y, const uint8_t* frame, int width, int height, uint8_t invert, uint8_t mode) {
                _x = 0;
        _y = 0;
        if (invert) invert = 255;
        uint8_t left = height & 0b111;
        if (left != 0) height += (8 - left);                              // округляем до ближайшего кратного степени 2
        int shiftY = (y >> 3) + (height >> 3);                            // строка (row) крайнего байта битмапа
        oled_setWindowShift(x, y, width, height);                              // выделяем окно
        bool bottom = (_shift != 0 && shiftY >= 0 && shiftY <= _maxRow);  // рисовать ли нижний сдвинутый байт

        if (!_BUFF) oled_beginData();
        for (int X = x, countX = 0; X < x + width; X++, countX++) {  // в пикселях
            uint8_t prevData = 0;
            if (_inRange(X, 0, _maxX)) {                                                                                    // мы внутри дисплея по X
                for (int Y = y >> 3, countY = 0; Y < shiftY; Y++, countY++) {                                               // в строках (пикс/8)
                    uint8_t data = (frame[countY * width + countX]) ^ invert;                                  // достаём байт
                    if (_shift == 0) {                                                                                      // без сдвига по Y
                        if (_inRange(Y, 0, _maxRow)) oled_writeData(data, Y, X, mode);                                           // мы внутри дисплея по Y
                    } else {                                                                                                // со сдвигом по Y
                        if (_inRange(Y, 0, _maxRow)) oled_writeData((prevData >> (8 - _shift)) | (data << _shift), Y, X, mode);  // задвигаем
                        prevData = data;
                    }
                }
                if (bottom) oled_writeData(prevData >> (8 - _shift), shiftY, X, mode);  // нижний байт
            }
        }
        if (!_BUFF) oled_endTransm();
    }

    // залить весь дисплей указанным байтом
    void oled_fill(uint8_t data) {
        if (_BUFF) memset(_oled_buffer, data, _bufSize);
        else {
            if (_TYPE < 2 || 1) {  // для SSD1306
                oled_setWindow(0, 0, _maxX, _maxRow);
                oled_beginData();
                for (int i = 0; i < (_TYPE ? 1024 : 512); i++) oled_sendByte(data);
                oled_endTransm();
            } else {  // для SSH1108
            }
        }
        oled_setCursorXY(_x, _y);
    }

    // шлёт байт в "столбик" setCursor() и setCursorXY()
    void oled_drawByte(uint8_t data) {
        if (++_x > _maxX) return;
        if (_TYPE < 2 || 1) {  // для SSD1306
            if (!_BUFF) oled_beginData();
            if (_shift == 0) {                       // если вывод без сдвига на строку
                oled_writeData(data,0,0,0);                     // выводим
            } else {                                 // со сдвигом
                oled_writeData(data << _shift,0,0,0);           // верхняя часть
                oled_writeData(data >> (8 - _shift), 1,0,0);  // нижняя часть со сдвигом на 1
            }
            if (!_BUFF) oled_endTransm();
        }
    }

    // вывести одномерный байтовый массив (линейный битмап высотой 8)
    void oled_drawBytes(uint8_t* data, uint8_t size) {
        if (!_BUFF) oled_beginData();
        for (uint8_t i = 0; i < size; i++) {
            if (++_x > _maxX) return;
            if (_shift == 0) {                          // если вывод без сдвига на строку
                oled_writeData(data[i],0,0,0);                     // выводим
            } else {                                    // со сдвигом
                oled_writeData(data[i] << _shift,0,0,0);           // верхняя часть
                oled_writeData(data[i] >> (8 - _shift), 1,0,0);  // нижняя часть со сдвигом на 1
            }
        }
        if (!_BUFF) oled_endTransm();
    }

    // ================================== СИСТЕМНОЕ ===================================
    // полностью обновить дисплей из буфера
    void oled_update_all(void) {
        if (_BUFF) {
            if (_TYPE < 2) {  // для 1306
                oled_setWindow(0, 0, _maxX, _maxRow);
                oled_beginData();
                for (int i = 0; i < (_TYPE ? 1024 : 512); i++) oled_sendByte(_oled_buffer[i]);
                oled_endTransm();
            } else {  // для 1106
                oled_sendCommand(0x00);
                oled_sendCommand(0x10);
                oled_sendCommand(0x40);
                uint16_t ptr = 0;
                for (uint8_t i = 0; i < (64 >> 3); i++) {
                    oled_sendCommand(0xB0 + i + 0);  // set page address
                    oled_sendCommand(2 & 0xf);       // set lower column address
                    oled_sendCommand(0x10);          // set higher column address
                    for (uint8_t a = 0; a < 8; a++) {
                        oled_beginData();
                        for (uint8_t b = 0; b < (OLED_WIDTH >> 3); b++) {
                            oled_sendByteRaw(_oled_buffer[((ptr & 0x7F) << 3) + (ptr >> 7)]);
                            ptr++;
                        }
                        oled_endTransm();
                    }
                }
            }
        }
    }

    // выборочно обновить дисплей из буфера (x0, y0, x1, y1)
    void oled_update(int x0, int y0, int x1, int y1) {
        if (_BUFF) {
            y0 >>= 3;
            y1 >>= 3;
            oled_setWindow(x0, y0, x1, y1);
            oled_beginData();
            for (int x = x0; x < x1; x++)
                for (int y = y0; y < y1 + 1; y++)
                    if (x >= 0 && x <= _maxX && y >= 0 && y <= _maxRow)
                        oled_sendByte(_oled_buffer[y + x * (_TYPE ? 8 : 4)]);
            oled_endTransm();
        }
    }

    // отправить байт по i2с или в буфер
    void oled_writeData(uint8_t data, uint8_t offsetY, uint8_t offsetX , uint8_t mode ) {
        if (_BUFF) {
            switch (mode) {
                case 0:
                    _oled_buffer[_bufIndex(_x + offsetX, _y) + offsetY] |= data;  // добавить
                    break;
                case 1:
                    _oled_buffer[_bufIndex(_x + offsetX, _y) + offsetY] &= ~data;  // вычесть
                    break;
                case 2:
                    _oled_buffer[_bufIndex(_x + offsetX, _y) + offsetY] = data;  // заменить
                    break;
            }
        } else {
            if (_buf_flag) {
                int x = _x - _bufX;
                int y = _y - _bufY;
                if (x < 0 || x > _bufsizeX || y < 0 || y > (_bufsizeY << 3)) return;
                switch (mode) {
                    case 0:
                        _buf_ptr[(y >> 3) + x * _bufsizeY] |= data;  // добавить
                        break;
                    case 1:
                        _buf_ptr[(y >> 3) + x * _bufsizeY] &= ~data;  // вычесть
                        break;
                    case 2:
                        _buf_ptr[(y >> 3) + x * _bufsizeY] = data;  // заменить
                        break;
                }
            } else {
                oled_sendByte(data);
            }
        }
    }

    // окно со сдвигом. x 0-127, y 0-63 (31), ширина в пикселях, высота в пикселях
    void oled_setWindowShift(int x0, int y0, int sizeX, int sizeY) {
        _shift = y0 & 0b111;
        if (!_BUFF) oled_setWindow(x0, (y0 >> 3), x0 + sizeX, (y0 + sizeY - 1) >> 3);
    }

    // ================== ДИНАМИЧЕСКИЙ БУФЕР ================
    // создать
    bool oled_createBuffer(int x0, int y0, int x1, int y1, uint8_t fill) {
        if (!_BUFF) {
            _bufX = x0;
            _bufY = y0;
            _bufsizeX = x1 - x0 + 1;
            _bufsizeY = ((y1 - y0) >> 3) + 1;

            int bufSize = _bufsizeX * _bufsizeY;
            _buf_ptr = (uint8_t*)malloc(bufSize);
            if (_buf_ptr != NULL) {
                _buf_flag = true;
                memset(_buf_ptr, fill, bufSize);
                return true;
            } else {
                _buf_flag = false;
                return false;
            }
        }
        return false;
    }

    // отправить
    void oled_sendBuffer() {
        if (!_BUFF) {
            if (_buf_flag) {
                oled_setWindow(_bufX, _bufY >> 3, _bufX + _bufsizeX, (_bufY >> 3) + _bufsizeY - 1);
                oled_beginData();
                for (int i = 0; i < _bufsizeX * _bufsizeY; i++) {
                    oled_sendByte(_buf_ptr[i]);
                }
                oled_endTransm();
                _buf_flag = false;
                free(_buf_ptr);
            }
        }
    }

    // ========= ЛОУ-ЛЕВЕЛ ОТПРАВКА =========

    // супер-костыль для либы ire. Привет индусам!
    void oled_sendByte(uint8_t data) {
        oled_sendByteRaw(data);

        //if (!_CONN) {
            _writes++;
            if (_writes >= 16) {
                oled_endTransm();
                oled_beginData();
            }
        //}

    }
    void oled_sendByteRaw(uint8_t data) {
        //ire.write(data);
        i2c_send_data(I2C2, data);
    }

    // отправить команду
    void oled_sendCommand(uint8_t cmd1) {
        oled_beginOneCommand();
        oled_sendByteRaw(cmd1);
        oled_endTransm();
    }

    // отправить код команды и команду
    void oled_sendCommand_cmd(uint8_t cmd1, uint8_t cmd2) {
        oled_beginCommand();
        oled_sendByteRaw(cmd1);
        oled_sendByteRaw(cmd2);
        oled_endTransm();
    }

    // выбрать "окно" дисплея
    void oled_setWindow(int x0, int y0, int x1, int y1) {
        oled_beginCommand();
        oled_sendByteRaw(OLED_COLUMNADDR);
        oled_sendByteRaw(constrain(x0, 0, _maxX));
        oled_sendByteRaw(constrain(x1, 0, _maxX));
        oled_sendByteRaw(OLED_PAGEADDR);
        oled_sendByteRaw(constrain(y0, 0, _maxRow));
        oled_sendByteRaw(constrain(y1, 0, _maxRow));
        oled_endTransm();
    }

    void oled_beginData(void) {
        oled_startTransm();        
        oled_sendByteRaw(OLED_DATA_MODE);
    }

    void oled_beginCommand(void) {
        oled_startTransm();
        oled_sendByteRaw(OLED_COMMAND_MODE);
    }

    void oled_beginOneCommand(void) {
        oled_startTransm();
        oled_sendByteRaw(OLED_ONE_COMMAND_MODE);
    }

    void oled_endTransm(void) {
        //ire.oled_endTransmission();
        i2c_send_stop(I2C2);
        while (_IF_BTF(I2C2));
        _writes = 0;
        //mdelay(2);         
        for (uint8_t i = 0; i < 100; i++) {
			__asm__("nop");
		} 
    }

    void oled_startTransm(void) {
        //ire.beginTransmission(_address);
        i2c_send_start(I2C2);
        while (_IF_SB(I2C2));
        i2c_send_7bit_address(I2C2, _address, 0);
        while (_IF_ADDR(I2C2));
        /* Cleaning ADDR condition sequence. */
        //reg32 = I2C_SR2(I2C2);
    }

    // получить "столбик-байт" буквы
    uint8_t oled_getFont(uint8_t font, uint8_t row) {
//#ifndef OLED_NO_PRINT
        if (row > 4) return 0;
        font = font - '0' + 16;  // перевод код символа из таблицы ASCII
        if (font <= 95) {
            return (_charMap[font][row]);  // для английских букв и символов
        } else if (font >= 96 && font <= 111) {            // и пизд*ц для русских
            return (_charMap[font + 47][row]);
        } else if (font <= 159) {
            return (_charMap[font - 17][row]);
        } else {
            return (_charMap[font - 1][row]);  // для кастомных (ё)
        }
//#endif
    }

    

   
    // всякое
    
    // индекс в буфере
    int _bufIndex(int x, int y) {
        return ((y) >> 3) + ((x) << (_TYPE ? 3 : 2));  // _y / 8 + _x * (4 или 8)
    }
    void _swap(int x, int y) {
        int z = x;
        x = y;
        y = z;
    }
    bool _inRange(int x, int mi, int ma) {
        return x >= mi && x <= ma;
    }

    

    uint8_t constrain(const uint8_t x, const uint8_t a, const uint8_t b) {
        if(x < a) {
            return a;
        }
        else if(b < x) {
            return b;
        }
        else
            return x;
    }