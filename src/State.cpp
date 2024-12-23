#include "State.h"
#include <Preferences.h>

ChannelState::ChannelState() : freq(DEFAULT_FREQ), txPower(TXPOWER) {
    memset(ps, 32, 9);
    id = 0;
}

void ChannelState::load(uint8_t _id) {
    id = _id;
    Preferences preferences;
    char key[10];
    sprintf(key, "tx%d", id);
    preferences.begin(key, false);
    freq = preferences.getUInt("freq", DEFAULT_FREQ);
    txPower = TXPOWER;
    preferences.getBytes("ps", ps, 9);
    preferences.getBool("enabled", enabled);
    preferences.end();
}

void ChannelState::save() {
    Preferences preferences;
    char key[10];
    sprintf(key, "tx%d", id);
    preferences.begin(key, false);
    preferences.putUInt("freq", freq);
    preferences.putUInt("txPower", txPower);
    preferences.putBytes("ps", ps, 9);
    preferences.putBool("enabled", enabled);
    preferences.end();
    Serial.printf("Saved settings for tx%d\n", id);
}

State::State() {}  

void State::load(uint8_t numtx) {
    for (int i = 0; i < numtx; i++) {
        channelState[i].load(i);
    }
}

void State::save(uint8_t numtx) {
    for (int i = 0; i < numtx; i++) {
        channelState[i].save();
    }
}