/* An Alternative Software Serial Library which uses Input Capture pins
 * http://www.pjrc.com/teensy/td_libs_NeoICSerial.html
 * Copyright (c) 2014 PJRC.COM, LLC, Paul Stoffregen, paul@pjrc.com
 * Modified November 2015 by SlashDev
 *
 * AltSoftSerial always uses these pins:
 *
 * Board          Transmit  Receive   PWM Unusable
 * -----          --------  -------   ------------
 * Teensy 3.0 & 3.1  21        20         22
 * Teensy 2.0         9        10       (none)
 * Teensy++ 2.0      25         4       26, 27
 * Arduino Uno        9         8         10
 * Arduino Leonardo   5        13       (none)
 * Arduino Mega      46        48       44, 45
 * Wiring-S           5         6          4
 * Sanguino          13        14         12
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef NeoICSerial_h
#define NeoICSerial_h

#include <inttypes.h>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

#if defined(__arm__) && defined(CORE_TEENSY)
#define ALTSS_BASE_FREQ F_BUS
#else
#define ALTSS_BASE_FREQ F_CPU
#endif

class NeoICSerial : public Stream
{
public:
	NeoICSerial() { }
	~NeoICSerial() { end(); }
	static void begin(uint32_t baud) { init((ALTSS_BASE_FREQ + baud / 2) / baud); }
	static void end();
	int peek();
	int read();
	int available();
#if ARDUINO >= 100
	size_t write(uint8_t byte) { writeByte(byte); return 1; }
	void flush() { flushOutput(); }
#else
	void write(uint8_t byte) { writeByte(byte); }
	void flush() { flushInput(); }
#endif
	using Print::write;
	static void flushInput();
	static void flushOutput();
	bool listen() { return false; }
	bool isListening() { return true; }
	bool overflow() { bool r = timing_error; timing_error = false; return r; }
	static int library_version() { return 1; }
	static void enable_timer0(bool enable) { }
	static bool timing_error;

  typedef void (* isr_t)( uint8_t );
  static void attachInterrupt( isr_t fn );
  static void detachInterrupt() { attachInterrupt( (isr_t) NULL ); };
private:
	static void init(uint32_t cycles_per_bit);
	static void writeByte(uint8_t byte);
};

#endif
