#pragma once
#include "config.h"
#include <Arduino.h>

class ChannelState {
    public:
        ChannelState();
        void load(uint8_t id);
        void save();
    
        // kHz/10, 8750-10800
        uint16_t freq;
        
        // Programme Service Name
        char ps[9];

        // dBuV, 88-115 max (120 uncalibrated)
        uint8_t txPower;

    private:
        uint8_t id;
};

class State {

    public:
        State();
        void load();
        void save();

        ChannelState channelState[NUM_TX];

};
