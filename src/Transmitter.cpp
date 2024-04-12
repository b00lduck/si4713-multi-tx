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
        Serial.println("Couldn't find radio?");
        while (1);
    }
}

void Transmitter::configure(ChannelState* state) {    
    Serial.print("Configuring transmitter: ");
    Serial.printf("freq: %d, txPower: %d, ps: %s\n", state->freq, state->txPower, state->ps);

    radio.tuneFM(state->freq);
    radio.setTXpower(state->txPower,100);

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
