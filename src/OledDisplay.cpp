#include "OledDisplay.h"

#if BOARD_VERSION == 1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

OledDisplay::OledDisplay(TwoWire* wire, int address) 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, wire, OLED_RESET),
      address(address),
      _invertMhz(false),
      _invertPiDigit(0) {
}

void OledDisplay::init() {
    display.begin(SSD1306_SWITCHCAPVCC, address);
    display.setRotation(2);

    drawLoadingScreen();
}

void OledDisplay::invertAll(bool invert) {
    display.invertDisplay(invert);
}

void OledDisplay::invertMhz(bool invert) {
    _invertMhz = invert;
}

void OledDisplay::invertPiDigit(uint8_t digit) {
    _invertPiDigit = digit;
}

void OledDisplay::drawLeftBoundText(const char* text, int y) {
    int16_t x, y1;
    uint16_t w, h;
    display.setCursor(0, y);
    display.printf("%s", text);
}

void OledDisplay::drawCenteredText(const char* text, int y) {
    int16_t x, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, y);
    display.printf("%s", text);
}

void OledDisplay::drawCenteredNumber(unsigned int num, int y) {
    char buffer[15] = "";
    sprintf(buffer, "%d", num);
    drawCenteredText(buffer, y);
}

void OledDisplay::drawLoadingScreen() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    drawCenteredText("Loading...", 20);
    display.display();
}

void OledDisplay::show(ChannelState* state, int8_t level) {
    
    display.clearDisplay();

    if (!screenSaver) {
        display.setTextColor(SSD1306_WHITE); 

        display.setTextSize(2);
        char buffer[15] = "";
        sprintf(buffer, "%03.2fMHz", state->freq / 100.0);
        
        if (_invertMhz) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        } else {
            display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        }
        drawCenteredText(buffer, 1);

        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        display.setTextSize(3);
        
        for (int i = 0; i < 8; i++) {
            if (1 << i & _invertPiDigit) {
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            } else {
                display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
            }
            display.setCursor(0 + i * 15, 21);
            display.printf("%c", state->ps[i]);
        }
    }

    uint8_t levelBar = map(level, -60, 0, 0, SCREEN_WIDTH-4);

    display.drawRect(0, 55, SCREEN_WIDTH, 8, SSD1306_WHITE);
    display.fillRect(2, 57, levelBar, 4, SSD1306_WHITE);


    display.display();
}
#endif