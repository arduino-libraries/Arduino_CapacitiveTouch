# CapacitiveTouch Library

The CapacitiveTouch Library is designed to simplify the use of capacitive touch sensors on Arduino UNO-R4 boards (Minima and WiFi). This library hides the low-level hardware details (CTSU, DTC, interrupts, etc.) and provides a simple, user-friendly API for beginners and educational projects.

## Features
- **Easy to Use API:**  
  Initialize your sensor with just one constructor call and use simple methods to read sensor values and detect touches.
- **Board Support:**  
  Compatible with Arduino UNO-R4 Minima and UNO-R4 WiFi boards.
- **Low-Level Abstraction:**  
  All complex tasks (hardware setup, measurement control, interrupt management) are abstracted away so you can focus on your project.

## Installation
1. Copy `CapacitiveTouch.h` and `CapacitiveTouch.cpp` into your Arduino libraries folder.
2. Restart your Arduino IDE so it recognizes the new library.

## Compatible Pins

### Arduino UNO-R4 Minima
| Arduino Pin  | Touch Sensor Channel (TS#) | Channel Control Index (CHAC idx) | Channel Control Bit Mask (CHAC val) |
|--------------|----------------------------|----------------------------------|-------------------------------------|
| D0           | 9                          | 1                                | (1 << 1)                           |
| D1           | 8                          | 1                                | (1 << 0)                           |
| D2           | 34                         | 4                                | (1 << 2)                           |
| D3           | 13                         | 1                                | (1 << 5)                           |
| D8           | 11                         | 1                                | (1 << 3)                           |
| D9           | 2                          | 0                                | (1 << 2)                           |
| D11          | 10                         | 1                                | (1 << 2)                           |
| D13          | 12                         | 1                                | (1 << 4)                           |
| A1 (D15)     | 21                         | 2                                | (1 << 5)                           |
| A2 (D16)     | 22                         | 2                                | (1 << 6)                           |
| LOVE_BUTTON  | 0                          | 0                                | (1 << 0)                           |

### Arduino UNO-R4 WiFi
| Arduino Pin  | Touch Sensor Channel (TS#) | Channel Control Index (CHAC idx) | Channel Control Bit Mask (CHAC val) |
|--------------|----------------------------|----------------------------------|-------------------------------------|
| D0           | 9                          | 1                                | (1 << 1)                           |
| D1           | 8                          | 1                                | (1 << 0)                           |
| D2           | 13                         | 1                                | (1 << 5)                           |
| D3           | 34                         | 4                                | (1 << 2)                           |
| D6           | 12                         | 1                                | (1 << 4)                           |
| D8           | 11                         | 1                                | (1 << 3)                           |
| D9           | 2                          | 0                                | (1 << 2)                           |
| D11          | 7                          | 0                                | (1 << 7)                           |
| D12          | 6                          | 0                                | (1 << 6)                           |
| A1 (D15)     | 21                         | 2                                | (1 << 5)                           |
| A2 (D16)     | 22                         | 2                                | (1 << 6)                           |
| LOVE_BUTTON  | 27                         | 3                                | (1 << 3)                           |

> **Note:** Only the above pins are supported. Other pins are not configured for capacitive touch sensing.

## Usage Example

```cpp
#include "CapacitiveTouch.h"

// Instantiate a capacitive touch sensor using the LOVE_BUTTON macro.
CapacitiveTouch ct = CapacitiveTouch(LOVE_BUTTON);

void setup() {
  Serial.begin(9600);
  ct.begin();
  ct.setThreshold(500); // Adjust threshold (lower values increase sensitivity)
  Serial.println("CapacitiveTouch Library: LOVE Button test");
}

void loop() {
  int sensorValue = ct.read();
  Serial.print("Raw Value: ");
  Serial.println(sensorValue);
  
  if (ct.isTouched()) {
    Serial.println("LOVE button touched!");
  }
  delay(100);
}
