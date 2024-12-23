#include <Arduino.h>
#include "Transmitter.h"
#include "config.h"
#if BOARD_VERSION == 1
  #include "OledDisplay.h"
#elif BOARD_VERSION == 2
  #include "LcdDisplay.h"
#endif
#include "AiEsp32RotaryEncoder.h"
#include "State.h"

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_A_PIN,ROTARY_B_PIN,ROTARY_BUTTON_PIN,-1,4);

State state;

TwoWire wireA = TwoWire(0);
TwoWire wireB = TwoWire(1);

Transmitter tx[] = {
 Transmitter(&wireA,TX1_ADDR,TX1_RESET_PIN),
 Transmitter(&wireA,TX2_ADDR,TX2_RESET_PIN),
 Transmitter(&wireB,TX3_ADDR,TX3_RESET_PIN),
 Transmitter(&wireB,TX4_ADDR,TX4_RESET_PIN),
};

#define NUM_TX (sizeof(tx)/sizeof(tx[0]))
#if BOARD_VERSION == 1
 #define NUM_DISPLAYS NUM_TX
#elif BOARD_VERSION == 2
 #define NUM_DISPLAYS 1
#endif

#if BOARD_VERSION == 1
  OledDisplay display[NUM_TX] = {
    OledDisplay(&wireA, 0x3C),
    OledDisplay(&wireA, 0x3D),
    OledDisplay(&wireB, 0x3C),
    OledDisplay(&wireB, 0x3D),
  };
#elif BOARD_VERSION == 2
  LcdDisplay display[NUM_DISPLAYS] {
  LcdDisplay(&wireA, 0x27)
  };
#endif

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

uint8_t uiMode = 0;
uint8_t selectedChannel = 0;
uint8_t selectedDigit = 0;

uint32_t screenSaverTimer = millis();
bool screenSaver = false;

void enableScreenSaver() {
  for (int i = 0; i < NUM_DISPLAYS; i++) {
    display[i].screenSaver = true;
    screenSaver = true;
    uiMode = 0;
  }
}

void disableScreenSaver() {
  screenSaverTimer = millis();
  for (int i = 0; i < NUM_DISPLAYS; i++) {
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
  wireA.begin(TWI1_SDA, TWI1_SCL, 250000);
  wireB.begin(TWI2_SDA, TWI2_SCL, 250000);

  Serial.print("initializing displays... ");
  for (int i = 0; i < NUM_DISPLAYS; i++) {
    display[i].init();
  }

  #if BOARD_VERSION == 2
    display[0].showLoadingProgress(1, "load settings");
  #endif

  state.load(NUM_TX);

  Serial.print("\ninitializing transmitters... ");
  for (int i = 0; i < NUM_TX; i++) {
    Serial.printf("TX%d ", i+1);
    tx[i].init();
    tx[i].configure(&state.channelState[i]);
    tx[i].readStatus();
    #if BOARD_VERSION == 2
      display[0].showLoadingProgress(2+i, "Init TX");
    #endif
  }
  for(int i = 0; i < 4; i++) {
    sleep(1);
    #if BOARD_VERSION == 2
      display[0].showLoadingProgress(6+i, "Init TX");
    #endif
  }
  for (int i = 0; i < NUM_TX; i++) {
    tx[i].configure(&state.channelState[i]);
    #if BOARD_VERSION == 1
      display[i].show(&state.channelState[i], tx[i].getLevel());
    #endif
    #if BOARD_VERSION == 2
      display[0].showLoadingProgress(10+i*2, "Init TX");
    #endif
    tx[i].readStatus();
    sleep(1);
    #if BOARD_VERSION == 2
      display[0].showLoadingProgress(10+i*2+1, "Init TX");
    #endif
    sleep(1);  
  }

  #if BOARD_VERSION == 2
    display[0].showLoadingProgress(18, "Init Encoder");
  #endif
  
  Serial.println("\ninitializing rotary encoder...");
  rotaryEncoder.begin();
  rotaryEncoder.setAcceleration(20);
  rotaryEncoder.setup(readEncoderISR);

  Serial.println("initializing done.");

  resetScreenSaver();

  #if BOARD_VERSION == 2
    usleep(300000);
    display[0].clear();
    display[0].selectCharset(1);
  #endif
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
          case 4:
            Serial.printf("Selected value %d\n", rotaryEncoder.readEncoder());
            state.channelState[selectedChannel].enabled = !state.channelState[selectedChannel].enabled;
            tx[selectedChannel].configure(&state.channelState[selectedChannel]);
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
            Serial.printf("UI mode 2 - Frequency selection for CH%d\n", selectedChannel);
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
            if (selectedDigit == (PS_MAX_LENGTH-1)) {
              Serial.println("UI mode 4 - Enable/Disable TX");
              uiMode = 4;
              rotaryEncoder.setAcceleration(0);
              rotaryEncoder.setBoundaries(0,1,true);
              rotaryEncoder.setEncoderValue(state.channelState[selectedChannel].enabled);
              #if BOARD_VERSION == 1
                display[selectedChannel].invertPiDigit(0);
              #endif
            } else {
              selectedDigit++;
              Serial.printf("Selected digit %d\n", selectedDigit);
              rotaryEncoder.setEncoderValue(state.channelState[selectedChannel].ps[selectedDigit]);
            }
            break;
          case 4:
            state.channelState[selectedChannel].save();
            tx[selectedChannel].configure(&state.channelState[selectedChannel]);
            uiMode = 0;
        }    
      }
    }

    #if BOARD_VERSION == 1
      for (int i = 0; i < NUM_DISPLAYS; i++) {
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
              display[i].invertPiDigit(1<<selectedDigit);
            }
            break;
        }
        display[i].show(&state.channelState[i], tx[i].getLevel());   
      }
  #elif BOARD_VERSION == 2

        #define display display[0]

        uint8_t enc = rotaryEncoder.readEncoder();

        switch (uiMode) {
          case 0:
            for (int i = 0; i < NUM_TX; i++) {
              display.line[i]->visible(true);
              display.line[i]->show(&state.channelState[i], tx[i].getLevel());
            }
            break;
          case 1:
            for (int i = 0; i < NUM_TX; i++) {
              display.line[i]->visible(enc == i);
              display.line[i]->show(&state.channelState[enc], 0);              
            }           
            break;
          case 2:
            for (int i = 0; i < NUM_TX; i++) {
              display.line[i]->visible(selectedChannel == i);
              display.line[i]->blinkFrequency(selectedChannel == i);
              display.line[i]->show(&state.channelState[selectedChannel], 0);   
            }
              
            break;
          case 3:
            for (int i = 0; i < NUM_TX; i++) {
              display.line[i]->visible(selectedChannel == i);
              display.line[i]->blinkPiDigit(selectedDigit, selectedChannel == i);    
              display.line[i]->show(&state.channelState[selectedChannel], 0);   
            }
            break;
          case 4:
            for (int i = 0; i < NUM_TX; i++) {
              display.line[i]->visible(selectedChannel == i);
              display.line[i]->blinkEnabledFlag(selectedChannel == i);    
              display.line[i]->show(&state.channelState[selectedChannel], 0);   
            }
            break;

        }
        


  #endif


}