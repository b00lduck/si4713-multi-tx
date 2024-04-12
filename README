# 4-CH FM modulator with ESP32 and si4713

This project is a FM modulator with 4 channels, consisting of four si4713 FM transmitters controlled by an ESP32.

The setup is used in an installation where four audio sources are modulated to FM and injected into a FM distribution system. The four outputs are connected to a 4-channel RF combiner, which is then connected to an antenna amplifier along with other RF sources i.e. aerials.

Programming is done with one rotary encoder and four OLED displays.

For a detailed schematic see the kicad project in the `kicad` folder.

3d printed parts can be found in the `openscad` folder.

## Hardware

- Stripboard
- ESP32
- 4x si4713 FM transmitter
- 4x SSD1306 OLED display
- Rotary encoder
- 8x cinch connector
- 4x BNC connector
- 19" rack case with custom front panel
- 3d printed back panel

The ESP32 is connected to the si4713 FM transmitters via I2C and to the OLED displays via two I2C buses.
The rotary encoder is connected to the ESP32 via three GPIO pins, two for the encoder and one for the push button.

## Software

The software is written in C++ using the Arduino framework and PlatformIO.


![Schematic](kicad/schematic.png)

