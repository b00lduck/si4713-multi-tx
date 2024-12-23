#include <Wire.h>
#include "Transmitter.h"
#include "config.h"
#include "State.h"

Transmitter::Transmitter(TwoWire *wire, uint8_t addr, uint8_t resetPin) 
    : resetPin(resetPin), addr(addr), wire(wire) {
    radio = Adafruit_Si4713(resetPin);
}

void Transmitter::init() {
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, LOW);
    delay(100);
    digitalWrite(resetPin, HIGH);

    if (!radio.begin(addr, wire)) {
        Serial.println("ERROR: Couldn't find radio?");
    }
}

void Transmitter::configure(ChannelState* state) {    
    Serial.print("Configuring transmitter: ");
    Serial.printf("freq: %d, txPower: %d, ps: %s, enabled: %d\n", state->freq, state->txPower, state->ps, state->enabled);

    if (state->enabled) {
        radio.tuneFM(state->freq);
        radio.setTXpower(state->txPower,0);
    } else {
        radio.tuneFM(8750);
        radio.setTXpower(80,0); // minimum power
    }
    radio.setProperty(SI4713_PROP_TX_PREEMPHASIS, 0x0001); // 50us pre-emphasis
    radio.setProperty(SI4713_PROP_TX_ACOMP_THRESHOLD, 0xffd8); // -40dBFS
    radio.setProperty(SI4713_PROP_TX_ACOMP_GAIN, 0x000f); // 15dB
    radio.setProperty(SI4713_PROP_TX_LIMITER_RELEASE_TIME, 0x000D); // 39.38ms
    radio.setProperty(SI4713_PROP_TX_ACOMP_ENABLE, 0x0003); // enable limiter and compressor
    
    radio.beginRDS();
    radio.setRDSstation(state->ps);
    radio.setRDSbuffer("");
}

void Transmitter::readStatus() {
    radio.readTuneStatus();
    Serial.print("\tCurr freq: "); 
    Serial.println(radio.currFreq);
    Serial.print("\tCurr freqdBuV:"); 
    Serial.println(radio.currdBuV);
    Serial.print("\tCurr ANTcap:"); 
    Serial.println(radio.currAntCap);

    radio.readASQ();
    Serial.print("\tCurr ASQ: 0x"); 
    Serial.println(radio.currASQ, HEX);
    Serial.print("\tCurr InLevel:"); 
    Serial.println(radio.currInLevel);
}

int8_t Transmitter::getLevel() {
    radio.readASQ();
    return radio.currInLevel;
}

bool Transmitter::getTxStatus() {
    radio.readTuneStatus();

    Serial.printf("AntCap: %d\n", radio.currAntCap);
    Serial.printf("dBuV: %d\n", radio.currdBuV);
    return true;
}