/*
	CharlesJr.cpp - Library to control
	Charles Jr, a tiny charlieplexing board.
	Created 2015, released into the Public Domain
*/

#include "Arduino.h"
#include "charlesjr.h"
								
// Pinout patterns for each LED contain: 
/*\
|*|
|*| 00 - for padding, bits are unused
|*|
|*| XXX - for DDR. 1 is output, 0 is input.
|*|       necessary for high-impedance.
|*|
|*| XXX - for PORT. 1 is HIGH, 0 is LOW.
|*|
|*|  DEFAULT PIN ASSIGNMENT
|*|
|*|     x  y  |  #  |  pinout      |  group
|*|     0  0  |  0  |  0b00011001  |  1
|*|     0  1  |  1  |  0b00110100  |  3
|*|     0  2  |  2  |  0b00110010  |  3
|*|     1  0  |  3  |  0b00101001  |  2
|*|     1  1  |  4  |  0b00101100  |  2
|*|     1  2  |  5  |  0b00011010  |  1
|*|
|*| Use swapLeds to swap LED codes within a group.
|*|
|*|	LSB is group 1, then group 2, then group 3.
|*|
|*| For example, to swap groups 1 and 2: swapLeds = 0b011
|*|
\*/

CharlesJr::CharlesJr(byte offset, byte swapLeds, volatile uint8_t &ddr_register, volatile uint8_t &port_register) {
	// Set the offset to what is passed in the constructor
	_offset = offset;
	// _i is used as a counter
	_i = 0;
	// Set the private register variables (pointers) to the address of the registers that are passed
	_ddr_register = &ddr_register;
	_port_register = &port_register;
	// Enable all LEDs at first
	_enabled = 0b00111111;
	// Swap LEDs in _ledcodes as requested
	byte temp;
	// For each bit in swapLeds:
	for (_i = 0; _i < 3; _i++) {
		// If the bit is enabled
		if (swapLeds >> _i & 1 == 1) {
			// Swap the bits.
			// The whole modulo thing is necessary because
			// group 1 contains #0 and #5. 
			// (_i * 2 + 5) % 6 is equivalent to doing
			// the following, but is quicker:
			//
			// _i * 2 - 1;
			// if (_i < 0) {
			//     _i += 6;
			// }
			temp = _ledcodes[(_i * 2 + 5) % 6];
			_ledcodes[(_i * 2 + 5) % 6] = _ledcodes[_i * 2];
			_ledcodes[_i * 2] = temp;
		}
	}
}

void CharlesJr::setLeds(byte enabled) {
	// Sets the private enabled byte to whatever is passed on.
	_enabled = enabled;
}

void CharlesJr::flash() {
	// Turns each LED on for 10 us, then turns them all off.
	for (_i = 0; _i <= 5; _i++) {
		ledOn(_i);
		delayMicroseconds(10);
	}
	off();
}

void CharlesJr::display() {
	// Flashes all enabled LEDs.
	// For every LED:
	for (_i = 0; _i <= 5; _i++) {
		// Check whether that LED's bit is set in _enabled 
		if ((_enabled >> _i) & 1 == 1) {
			// If yes, turn that LED on and wait 10 us
			ledOn(_i);
			delayMicroseconds(10);
		} else {
			// If not, turn all LEDs off.
			off();
		}
	}
	// When done, turn all LEDs off.
	off();
}

void CharlesJr::setPixel(byte x, byte y, bool state) {
	// to understand some of this logic, read http://stackoverflow.com/a/47990
	if (state == false) {
		// if state is false, set that bit of enabled to 0.
		_enabled &= ~(1 << (3 * x + y));
	} else {
		// if state is true, set that bit of enabled to 0
		_enabled |= 1 << (3 * x + y);
	}
}

void CharlesJr::setAll(bool state) {
	// set all to 1 or 0 depending on state.
	// Multiplication gives us either 0b111111 or 0.
	_enabled = state * 0b111111;
}

void CharlesJr::ledOn(byte ledNum) {
	// to understand this logic, read http://stackoverflow.com/a/47990
	// turns one LED on (and others off)
	*_ddr_register = *_ddr_register & ~(0b111 << _offset) | _ledcodes[ledNum] >> 3 << _offset;
	*_port_register = *_port_register & ~(0b111 << _offset) | (_ledcodes[ledNum] & 0b111) << _offset;
}

void CharlesJr::off() {
	// to understand this logic, read http://stackoverflow.com/a/47990
	// sets 3 bits (offset, offset + 1, offset + 2) to input and low
	*_ddr_register = *_ddr_register & ~(0b111 << _offset);
	*_port_register = *_port_register & ~(0b111 << _offset);
}