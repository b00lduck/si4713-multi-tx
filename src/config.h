#pragma once

#define NUM_TX 4

#define ROTARY_A_PIN 32
#define ROTARY_B_PIN 33
#define ROTARY_BUTTON_PIN 27

#define TWI1_SDA 21
#define TWI1_SCL 22
#define TWI2_SDA 19
#define TWI2_SCL 18



#define TX1_ADDR 0x63
#define TX1_RESET_PIN 23
#define TX2_ADDR 0x11
#define TX2_RESET_PIN 16
#define TX3_ADDR 0x63
#define TX3_RESET_PIN 17   
#define TX4_ADDR 0x11
#define TX4_RESET_PIN 15


#define DEFAULT_FREQ 8750
#define TXPOWER 120 
#define MIN_FREQ 8750
#define MAX_FREQ 10800

#define FREQENC_DIVISOR 10 // 10: Step 100kHz, 5: Step 50kHz
#define FREQENC_BOUND_LOW (MIN_FREQ / FREQENC_DIVISOR)
#define FREQENC_BOUND_HIGH (MAX_FREQ / FREQENC_DIVISOR)