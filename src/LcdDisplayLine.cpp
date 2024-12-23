#include "LcdDisplayLine.h"

#if BOARD_VERSION == 2

LcdDisplayLine::LcdDisplayLine(LiquidCrystal_PCF8574 lcd, uint8_t lineNumber) : 
  lcd(lcd), 
  lineNumber(lineNumber) {
  visible(true);
}

void LcdDisplayLine::show(ChannelState* state, int8_t level) {
  char buf[SCREEN_WIDTH+1];
  if (enabled) {
    bool blink = (millis() / 300) % 2;  
    snprintf(buf, SCREEN_WIDTH, "%03.2f %.8s %c %d", state->freq / 100.0, 
      state->ps, state->enabled ? 1 : 219, level);

    if (blinkFrequencyState && blink) {
      for(int i = 0; i < 5; i++) {
        buf[i] = 255;
      }
    }

    for(int i = 0; i < PS_MAX_LENGTH; i++) {
      if (blinkPiDigitState[i] && blink) {
        buf[6+i] = 255;
      }
    }

    if (blinkEnabledFlagState && blink) {
      buf[15] = 255;
    }

  } else {
    snprintf(buf, SCREEN_WIDTH, "                    ");
  }
  lcd.setCursor(0, lineNumber); 
  lcd.print(buf);
}

void LcdDisplayLine::visible(bool state) {
  enabled = state;
  blinkFrequencyState = false;
  blinkEnabledFlagState = false;
  for (int i = 0; i < PS_MAX_LENGTH; i++) {
      blinkPiDigitState[i] = false;
  }
}

void LcdDisplayLine::blinkFrequency(bool blink) {
  blinkFrequencyState = blink;
}

void LcdDisplayLine::blinkPiDigit(uint8_t digit, bool blink) {
  blinkPiDigitState[digit] = blink;
}

void LcdDisplayLine::blinkEnabledFlag(bool blink) {
  blinkEnabledFlagState = blink;
}

#endif