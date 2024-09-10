#include <Arduino.h>
#include "Transmitter.h"
#include "config.h"
#include "Display.h"
#include "AiEsp32RotaryEncoder.h"
#include "State.h"

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_A_PIN,ROTARY_B_PIN,ROTARY_BUTTON_PIN,-1,4);

State state;

TwoWire wireA = TwoWire(0);
TwoWire wireB = TwoWire(1);

Transmitter tx[NUM_TX] = {
  Transmitter(&wireA,TX1_ADDR,TX1_RESET_PIN),
  Transmitter(&wireA,TX2_ADDR,TX2_RESET_PIN),
  Transmitter(&wireB,TX3_ADDR,TX3_RESET_PIN),
  Transmitter(&wireB,TX4_ADDR,TX4_RESET_PIN),
};

Display display[NUM_TX] = {
  Display(&wireA, 0x3C),
  Display(&wireA, 0x3D),
  Display(&wireB, 0x3C),
  Display(&wireB, 0x3D),
};

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

uint8_t uiMode = 0;
uint8_t selectedChannel = 0;
uint8_t selectedDigit = 0;


uint32_t screenSaverTimer = millis();
bool screenSaver = false;

void enableScreenSaver() {
  for (int i = 0; i < NUM_TX; i++) {
    display[i].screenSaver = true;
    screenSaver = true;
    uiMode = 0;
  }
}

void disableScreenSaver() {
  screenSaverTimer = millis();
  for (int i = 0; i < NUM_TX; i++) {
    display[i].screenSaver = false;
    screenSaver = false;
  }
}

void resetScreenSaver() {
  screenSaverTimer = millis();
}

void setup() {

  Serial.begin(9600);

  Serial.println("initializing TWI...");
  wireA.begin(TWI1_SDA, TWI1_SCL);
  wireB.begin(TWI2_SDA, TWI2_SCL);

  Serial.print("initializing displays... ");
  for (int i = 0; i < NUM_TX; i++) {
    Serial.printf("#%d ", i);
    display[i].init();
  }

  state.load();

  Serial.print("\ninitializing transmitters... ");
  for (int i = 0; i < NUM_TX; i++) {
    tx[i].init();
    tx[i].configure(&state.channelState[i]);
    tx[i].readStatus();  
  }
  sleep(5);
  for (int i = 0; i < NUM_TX; i++) {
    tx[i].configure(&state.channelState[i]);
    display[i].show(&state.channelState[i], tx[i].getLevel());
    tx[i].readStatus();  
    sleep(2);  
  }

  
  Serial.println("\ninitializing rotary encoder...");
  rotaryEncoder.begin();
  rotaryEncoder.setAcceleration(20);
  rotaryEncoder.setup(readEncoderISR);

  Serial.println("initializing done.");

  resetScreenSaver();
}



void loop() {

  uint32_t inactivity = millis() - screenSaverTimer;
  if (inactivity > 10000) {
    enableScreenSaver();
  }

  if (rotaryEncoder.encoderChanged()) {
    Serial.printf("Selected channel: %d, Encoder value %d\n", selectedChannel, rotaryEncoder.readEncoder());
    if (screenSaver) {
      disableScreenSaver();
    } else {
      resetScreenSaver();
      switch(uiMode) {
        case 0:
          break;
        case 1:
          break;
        case 2:
          Serial.printf("Selected value %d\n", rotaryEncoder.readEncoder());
          state.channelState[selectedChannel].freq = rotaryEncoder.readEncoder() * FREQENC_DIVISOR;
          Serial.printf("New freq: %d\n", state.channelState[selectedChannel].freq);
          tx[selectedChannel].configure(&state.channelState[selectedChannel]);
          break;
        case 3:
          Serial.printf("Selected value %d\n", rotaryEncoder.readEncoder());
          state.channelState[selectedChannel].ps[selectedDigit] = rotaryEncoder.readEncoder();
          tx[selectedChannel].configure(&state.channelState[selectedChannel]);
          break;
      }
    }
  }

  if (rotaryEncoder.isEncoderButtonClicked()) {
    if (screenSaver) {
      disableScreenSaver();
    } else {
      resetScreenSaver();
      switch(uiMode) {
        case 0:
          Serial.println("UI mode 1 - Channel selection");
          uiMode = 1;
          rotaryEncoder.setAcceleration(0);
          rotaryEncoder.setBoundaries(0, NUM_TX-1, false);
          rotaryEncoder.setEncoderValue(0);
          break;
        case 1:
          selectedChannel = rotaryEncoder.readEncoder();
          Serial.println("UI mode 2 - Frequency selection");
          uiMode = 2;
          rotaryEncoder.setAcceleration(250);
          rotaryEncoder.setBoundaries(FREQENC_BOUND_LOW,FREQENC_BOUND_HIGH,false);
          rotaryEncoder.setEncoderValue(state.channelState[selectedChannel].freq / FREQENC_DIVISOR);
          break;
        case 2:
          Serial.println("UI mode 3 - Name selection");
          uiMode = 3;
          selectedDigit = 0;
          Serial.printf("Selected digit %d\n", selectedDigit);
          rotaryEncoder.setAcceleration(50);
          rotaryEncoder.setBoundaries(32,90,false);
          rotaryEncoder.setEncoderValue(state.channelState[selectedChannel].ps[selectedDigit]);
          break;
        case 3:
          if (selectedDigit == 7) {
            state.channelState[selectedChannel].save();
            tx[selectedChannel].configure(&state.channelState[selectedChannel]);
            uiMode = 0;
            display[selectedChannel].invertPiDigit(0);
          } else {
            selectedDigit++;
            Serial.printf("Selected digit %d\n", selectedDigit);
            rotaryEncoder.setEncoderValue(state.channelState[selectedChannel].ps[selectedDigit]);
          }
          break;
      }    
    }
  }

  for (int i = 0; i < NUM_TX; i++) {

    switch (uiMode) {
      case 0:
        display[i].invertAll(false);
        display[i].invertMhz(false);
        break;
      case 1:
        if (i == rotaryEncoder.readEncoder()) {
          display[i].invertAll(true);
        } else {
          display[i].invertAll(false);
        }
        break;
      case 2:
        display[i].invertAll(false);
        if (i == selectedChannel) {
          display[i].invertMhz(true);
        } else {
          display[i].invertMhz(false);
        }
        break;
      case 3:
        display[i].invertMhz(false);
        if (i == selectedChannel) {
          display[selectedChannel].invertPiDigit(1<<selectedDigit);
        }
        break;
    }

    display[i].show(&state.channelState[i], tx[i].getLevel());   
  }

}