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

#include "Servo.h"

static bool initialized = false;
static pwm_config pwm_conf;
static uint32_t wrap_value;

Servo::Servo() {
  this->resetFields();
}

void Servo::attach(uint gpio, uint min_pulse, uint max_pulse) {
  if (this->attached()) {
    this->detach();
  }

  if (!initialized) {
    uint32_t clock_speed = clock_get_hz(clk_sys);
    wrap_value = clock_speed / PRESCALER / DEFAULT_FREQUENCY;

    pwm_conf = pwm_get_default_config();
    pwm_config_set_clkdiv(&pwm_conf, PRESCALER);
    pwm_config_set_wrap(&pwm_conf, wrap_value);

    initialized = true;
  }

  this->gpio = gpio;
  gpio_set_function(gpio, GPIO_FUNC_PWM);

  this->min_pulse = min_pulse;
  this->max_pulse = max_pulse;

  this->slice = pwm_gpio_to_slice_num(gpio);
  this->channel = pwm_gpio_to_channel(gpio);

  pwm_init(this->slice, &pwm_conf, true);
  this->writeMicroseconds(this->min_pulse);
}

void Servo::detach() {
  pwm_clear_irq(this->slice); // Clear the PWM interrupt
  pwm_set_irq_enabled(this->slice, false); // Disable the PWM block
  gpio_set_function(this->gpio, GPIO_FUNC_NULL); // Free the GPIO pin
  this->resetFields();
}

void Servo::write(uint value) {
  // If the provided value is smaller than the minimal pulse width, we assume we're dealing with degrees
  if (value < MIN_PULSE_WIDTH) {
    if (value > 180) {
      value = 180;
    }

    value = ((value - MIN_ANGLE) * (this->max_pulse - this->min_pulse) / (MAX_ANGLE - MIN_ANGLE) + this->min_pulse);
  }

  this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(uint value) {
  if (!this->attached()) {
    return;
  }

  // Constrain the provided value
  if (value < MIN_PULSE_WIDTH) {
    value = MIN_PULSE_WIDTH;
  }

  if (value > MAX_PULSE_WIDTH) {
    value = MAX_PULSE_WIDTH;
  }

  // We are converting microseconds to ticks
  float period = 1.f / DEFAULT_FREQUENCY * 1000000;
  this->current_pulse = value / period * wrap_value;

  pwm_clear_irq(this->slice);
  pwm_set_chan_level(this->slice, this->channel, this->current_pulse);
}

uint Servo::read() {
  return ((this->current_pulse - this->min_pulse) * (MAX_ANGLE - MIN_ANGLE) / (this->max_pulse - this->min_pulse) + MIN_ANGLE);
}

uint Servo::readMicroseconds() {
  return this->current_pulse;
}

void Servo::resetFields(void) {
  this->gpio = NOT_ATTACHED;
  this->min_pulse = MIN_PULSE_WIDTH;
  this->max_pulse = MAX_PULSE_WIDTH;
  this->slice = 0;
  this->channel = 0;
}
