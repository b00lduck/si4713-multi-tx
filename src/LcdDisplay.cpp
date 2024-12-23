#include "LcdDisplay.h"

#if BOARD_VERSION == 2
#include "charset.h"
#include "LiquidCrystal_PCF8574.h"

LcdDisplay::LcdDisplay(TwoWire* wire, int address) 
    : lcd(0x27),
      wire(wire),
      address(address) {
};

void LcdDisplay::selectCharset(uint8_t charset) {
  switch(charset) {
    case 0:
      lcd.createChar(1, bar1);
      lcd.createChar(2, bar2);
      lcd.createChar(3, bar3);
      break;
    case 1:
      lcd.createChar(1, enabledOn);
      lcd.createChar(2, lvl1);
      lcd.createChar(3, lvl2);
      lcd.createChar(4, lvl3);
      lcd.createChar(5, lvl4);
      lcd.createChar(6, lvl5);
      lcd.createChar(7, lvl6);
      lcd.createChar(8, lvl7);
      break;
  }
} 

void LcdDisplay::init() {
    lcd.begin(SCREEN_WIDTH, SCREEN_HEIGHT, *wire);

    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();

    selectCharset(0);

    for(int i = 0; i < SCREEN_HEIGHT; i++) {
      line[i] = new LcdDisplayLine(lcd, i);
    }
}

void LcdDisplay::clear() {
  lcd.clear();
} 

// Draw Loading bar
void LcdDisplay::showLoadingProgress(uint8_t progress, char* text) {
  char buf1[21];
  snprintf(buf1, 21, "%-20s", text);
  lcd.setCursor(0, 0);
  lcd.print(buf1);
  lcd.setCursor(1, 2);
  char buf2[21] = "\1\2\2\2\2\2\2\2\2\2\2\2\2\2\2\2\2\3"; 
  for (int i = 0; i < progress; i++) {
    buf2[i] = 255;
  }
  lcd.print(buf2);
}

#endif