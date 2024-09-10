#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Si4713.h>
#include "State.h"


class Transmitter {
    public:
        Transmitter(TwoWire* wire, uint8_t addr, uint8_t resetPin);
        void init();
        void readStatus();
        bool getTxStatus();
        
        int8_t getLevel();

        void configure(ChannelState* state);

    private:
        TwoWire* wire;
        uint8_t addr;
        uint8_t resetPin;
        Adafruit_Si4713 radio;

};
