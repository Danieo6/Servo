/*
  A library allowing for easy servo motor control using the Raspberry Pi Pico.
  Inspired by the Arduino Servo library.

  MIT License

  Copyright (c) 2021 Daniel Budziński
  https://github.com/Danieo6

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef SERVO_H
#define SERVO_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define VERSION 1 // Library version
#define MIN_PULSE_WIDTH 544 // Min. pulse width sent to a servo
#define MAX_PULSE_WIDTH 2400 // Max. pulse width sent to a servo
#define MIN_ANGLE 0 // Min. angle the servo can rotate to
#define MAX_ANGLE 180 // Max. angle the servo can rotate to
#define PRESCALER 64 // The prescaler used to control the servos
#define DEFAULT_FREQUENCY 50.f // Set the default pulse frequency to 50Hz (20ms period)
#define NOT_ATTACHED 32 // Constant used when the servo is not attached to any gpio pin

class Servo {
  public:
    Servo();
    void attach(uint gpio, uint min_pulse = MIN_PULSE_WIDTH, uint max_pulse = MAX_PULSE_WIDTH);
    void detach();
    void write(uint value);
    void writeMicroseconds(uint value);
    uint read();
    uint readMicroseconds();
    bool attached() const { return this->gpio != NOT_ATTACHED; }
  private:
    uint gpio;
    uint min_pulse;
    uint max_pulse;
    uint current_pulse;
    uint slice;
    uint channel;
    void resetFields();
};
#endif
