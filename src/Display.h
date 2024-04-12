#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "State.h"

class Display {
    public:
        Display(TwoWire* wire, int address); 

        void init();  
        void show(ChannelState* state, int8_t level); 
        void invertAll(bool invert);
        void invertMhz(bool invert);
        void invertPiDigit(uint8_t digit);

    private:
        Adafruit_SSD1306 display;
        int address;
        uint16_t tick;
        void drawCenteredText(const char* text, int y);
        void drawLeftBoundText(const char* text, int y);
        void drawCenteredNumber(unsigned int num, int y);
        bool _invertMhz;
        uint8_t _invertPiDigit;
};
