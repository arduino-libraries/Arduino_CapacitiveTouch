# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class ` [`CapacitiveTouch`](#class_capacitive_touch) | A user-friendly class for capacitive touch sensing.
`struct ` [`CapTouchPinMapping`](#struct_cap_touch_pin_mapping) | Defines the mapping between an Arduino pin and its capacitive touch hardware settings.

# class `CapacitiveTouch` <a id="class_capacitive_touch" class="anchor"></a>

A user-friendly class for capacitive touch sensing.

Create an instance with: <br/>[CapacitiveTouch](#class_capacitive_touch) ct = [CapacitiveTouch(LOVE_BUTTON)](#class_capacitive_touch);

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`CapacitiveTouch`](#class_capacitive_touch_1ac22af64579b04c74118d037b1a8715f2) | Constructs a capacitive touch sensor for the given pin. |
| [`begin`](#class_capacitive_touch_1a0cce24fac4a98cecf7840ebf435a5d9c) | Initializes the sensor. |
| [`read`](#class_capacitive_touch_1a5c7b56cc0ad47963fa0b97abe2909aba) | Reads the raw sensor value. |
| [`isTouched`](#class_capacitive_touch_1aab0faf01526c57e1f0f57aa19919d9fa) | Checks if the sensor is touched. |
| [`setThreshold`](#class_capacitive_touch_1a5f773cd9472164c698811962070f903c) | Sets the detection threshold. |
| [`getThreshold`](#class_capacitive_touch_1a64a1533e5ea3109c2ff15a3abf22bbba) | Retrieves the current detection threshold. |

## Members

### `CapacitiveTouch` <a id="class_capacitive_touch_1ac22af64579b04c74118d037b1a8715f2" class="anchor"></a>

```cpp
CapacitiveTouch(uint8_t pin)
```

Constructs a capacitive touch sensor for the given pin.

#### Parameters
* `pin` The Arduino pin number (such as LOVE_BUTTON).
<hr />

### `begin` <a id="class_capacitive_touch_1a0cce24fac4a98cecf7840ebf435a5d9c" class="anchor"></a>

```cpp
void begin()
```

Initializes the sensor.

Configures the pin and initializes the CTSU/DTC hardware.
<hr />

### `read` <a id="class_capacitive_touch_1a5c7b56cc0ad47963fa0b97abe2909aba" class="anchor"></a>

```cpp
int read()
```

Reads the raw sensor value.

Initiates a measurement cycle and returns the raw value. 
#### Returns
int The raw sensor value.
<hr />

### `isTouched` <a id="class_capacitive_touch_1aab0faf01526c57e1f0f57aa19919d9fa" class="anchor"></a>

```cpp
bool isTouched()
```

Checks if the sensor is touched.

Compares the raw value to the set threshold. 
#### Returns
true if the sensor is touched, false otherwise.
<hr />

### `setThreshold` <a id="class_capacitive_touch_1a5f773cd9472164c698811962070f903c" class="anchor"></a>

```cpp
void setThreshold(int threshold)
```

Sets the detection threshold.

#### Parameters
* `threshold` The new threshold value.
<hr />

### `getThreshold` <a id="class_capacitive_touch_1a64a1533e5ea3109c2ff15a3abf22bbba" class="anchor"></a>

```cpp
int getThreshold()
```

Retrieves the current detection threshold.

#### Returns
int The current threshold value.
<hr />

# struct `CapTouchPinMapping` <a id="struct_cap_touch_pin_mapping" class="anchor"></a>

Defines the mapping between an Arduino pin and its capacitive touch hardware settings.

Each entry includes the Arduino pin number, a flag indicating support, and additional hardware parameters needed for the CTSU.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`arduinoPin`](#struct_cap_touch_pin_mapping_1a8f558f88fa5298ded8cbbdd4830b4cbe) | Arduino pin number (e.g., D4, D3, A4) |
| [`supported`](#struct_cap_touch_pin_mapping_1a4e27a8d3ce508ad4bfab32ef4d94e292) | True if this pin is supported for capacitive touch sensing. |
| [`ts_num`](#struct_cap_touch_pin_mapping_1a64ecb7d254b91a6d02229c4b26b94f26) | Touch sensor channel number. |
| [`chac_idx`](#struct_cap_touch_pin_mapping_1aff722690ee3b00afb64221eb1cde5a75) | Channel control register index. |
| [`chac_val`](#struct_cap_touch_pin_mapping_1ad1a206d6af3ddf996713d18190cd2f2c) | Bit mask used to enable the sensor channel. |

## Members

### `arduinoPin` <a id="struct_cap_touch_pin_mapping_1a8f558f88fa5298ded8cbbdd4830b4cbe" class="anchor"></a>

```cpp
uint8_t arduinoPin
```

Arduino pin number (e.g., D4, D3, A4)

<hr />

### `supported` <a id="struct_cap_touch_pin_mapping_1a4e27a8d3ce508ad4bfab32ef4d94e292" class="anchor"></a>

```cpp
bool supported
```

True if this pin is supported for capacitive touch sensing.

<hr />

### `ts_num` <a id="struct_cap_touch_pin_mapping_1a64ecb7d254b91a6d02229c4b26b94f26" class="anchor"></a>

```cpp
uint8_t ts_num
```

Touch sensor channel number.

<hr />

### `chac_idx` <a id="struct_cap_touch_pin_mapping_1aff722690ee3b00afb64221eb1cde5a75" class="anchor"></a>

```cpp
uint8_t chac_idx
```

Channel control register index.

<hr />

### `chac_val` <a id="struct_cap_touch_pin_mapping_1ad1a206d6af3ddf996713d18190cd2f2c" class="anchor"></a>

```cpp
uint8_t chac_val
```

Bit mask used to enable the sensor channel.

<hr />

