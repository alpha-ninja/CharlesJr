/*
	CharlesJr.h - Library to control
	Charles Jr, a tiny charlieplexing board.
	Created 2015, released into the Public Domain
*/

#ifndef charlesjr_h
#define charlesjr_h
#include "Arduino.h"

class CharlesJr {
	public:
		CharlesJr(byte offset, byte swapLeds, volatile uint8_t &ddr_register, volatile uint8_t &port_register);
		void flash();
		void ledOn(byte ledNum);
		void off();
		void display();
		void setPixel(byte x, byte y, bool state);
		void setAll(bool state);
		void setLeds(byte values);
	private:
		byte _ledcodes[6] = {0b00011001,
							0b00110100,
							0b00110010,
							0b00101001,
							0b00101100,
							0b00011010};
		byte _enabled;
		byte _i;
		byte _offset;
		volatile uint8_t *_ddr_register;
		volatile uint8_t *_port_register;
};

#endif