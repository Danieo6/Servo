# API

## Methods

### `attach`

Attach the selected GPIO pin to Pico's PWM generator to control your servo. Calling it twice will [detach](#detach) the servo.

#### Overloads
```cpp
attach(uint gpio)
attach(uint gpio, uint min_pulse, uint max_pulse)
```

#### Arguments

**gpio** - The GPIO pin your servo's PWM signal line would be attached.

**min_pulse (optional)** - The minimal pulse width your servo can deal with. Default is set to 544.

**max_pulse (optional)** - The maximal pulse width your servo can deal with. Default is 2400.

#### Use case
```cpp
#include "Servo.h"

int main() {
  Servo servo;
  servo.attach(15);
}
```

### `detach`

Detaches the servo from your GPIO pin, frees it and disables this pins PWM slice.

**WARNING** Calling this method will disable **BOTH** channels on this PWM slice.

#### Overloads
```cpp
detach()
```

#### Use case
```cpp
int main() {
  Servo servo;
  servo.attach(15);
  servo.detach();
}
```

### `write`

Set the PWM channel level. You can provide a value from 0 to 180 to move the servo to a specific position in degress. Providing a higher value will assume that you want to use microseconds (pulse width in microseconds).

#### Overloads
```cpp
write(uint value)
```

#### Arguments

**value** - the position you want your servo to move to.

#### Use case
```cpp
int main() {
  Servo servo;
  servo.attach(15);

  while(true) {
    servo.write(0);
    sleep_ms(5000);
    servo.write(180);
    sleep_ms(5000);
  }
}
```

### `writeMicroseconds`

It's simillar to [write](#write) but it uses microseconds.

#### Overloads
```cpp
writeMicroseconds(uint value)
```

#### Arguments

**value** - pulse width in microseconds.

#### Use case
```cpp
int main() {
  Servo servo;
  servo.attach(15);

  while(true) {
    servo.writeMicroseconds(544);
    sleep_ms(5000);
    servo.writeMicroseconds(2400);
    sleep_ms(5000);
  }
}
```

### `read`

It return the current PWM pulse width in servo angles.

### Overloads

```cpp
read()
```

#### Use case
```cpp
int main() {
  Servo servo;
  servo.attach(15);

  while(true) {
    uint position = servo.read();
  }
}
```

### `readMicroseconds`

It return the current PWM pulse width in microseconds.

### Overloads

```cpp
readMicroseconds()
```

#### Use case
```cpp
int main() {
  Servo servo;
  servo.attach(15);

  while(true) {
    uint pulseWidth = servo.readMicroseconds();
  }
}
```

