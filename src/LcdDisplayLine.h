#pragma once
#include "config.h"

#if BOARD_VERSION == 2

#define SCREEN_WIDTH 20 

#include "LiquidCrystal_PCF8574.h"
#include "State.h"

class LcdDisplayLine {
    public:
        LcdDisplayLine(LiquidCrystal_PCF8574 lcd, uint8_t lineNumber);

        void show(ChannelState* state, int8_t level); 

        void visible(bool state);
        void blinkFrequency(bool blink);
        void blinkPiDigit(uint8_t digit, bool blink);
        void blinkEnabledFlag(bool blink);

    private:
        LiquidCrystal_PCF8574 lcd;
        uint8_t lineNumber;

        bool blinkFrequencyState;
        bool blinkPiDigitState[PS_MAX_LENGTH];
        bool blinkEnabledFlagState;
        bool enabled;
};


#endif