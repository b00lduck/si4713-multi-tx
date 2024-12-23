#pragma once
#include "config.h"
#if BOARD_VERSION == 1
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "State.h"

class OledDisplay {
    public:
        OledDisplay(TwoWire* wire, int address); 

        void init();  
        void show(ChannelState* state, int8_t level); 
        void invertAll(bool invert);
        void invertMhz(bool invert);
        void invertPiDigit(uint8_t digit);
        bool screenSaver;

    private:
        Adafruit_SSD1306 display;
        int address;
        void drawCenteredText(const char* text, int y);
        void drawLeftBoundText(const char* text, int y);
        void drawCenteredNumber(unsigned int num, int y);
        void drawLoadingScreen();
        bool _invertMhz;
        uint8_t _invertPiDigit;


};
#endif