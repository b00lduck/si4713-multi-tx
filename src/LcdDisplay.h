#pragma once
#include "config.h"

#if BOARD_VERSION == 2

#define SCREEN_HEIGHT 4

#include "LcdDisplayLine.h"

class LcdDisplay {
    public:
        LcdDisplay(TwoWire* wire, int address); 

        void init();  
        void clear();
        void selectCharset(uint8_t charset);
        void showLoadingProgress(uint8_t progress, char* text);
        bool screenSaver;

        LcdDisplayLine* line[SCREEN_HEIGHT];

    private:
        LiquidCrystal_PCF8574 lcd;
        int address;
        uint16_t tick;
        TwoWire* wire;

};

#endif