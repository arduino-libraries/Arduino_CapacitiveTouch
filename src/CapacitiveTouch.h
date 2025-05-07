#ifndef CAPACITIVE_TOUCH_H
#define CAPACITIVE_TOUCH_H

#include <Arduino.h>
#include "r_dtc.h"
#include "IRQManager.h"



// Ensure we compile only for the supported boards.
#if !defined(ARDUINO_UNOR4_MINIMA) && !defined(ARDUINO_UNOR4_WIFI)
  #error "This library only supports the Arduino UNO-R4 Minima and UNO-R4 WiFi boards."
#endif

// Define a symbolic constant for the LOVE button (special pin).
#define LOVE_BUTTON   20  
/**
 * @struct CapTouchPinMapping
 * @brief Defines the mapping between an Arduino pin and its capacitive touch hardware settings.
 *
 * Each entry includes the Arduino pin number, a flag indicating support,
 * and additional hardware parameters needed for the CTSU.
 */
struct CapTouchPinMapping {
    uint8_t arduinoPin;   ///< Arduino pin number (e.g., D4, D3, A4)
    bool supported;       ///< True if this pin is supported for capacitive touch sensing.
    uint8_t ts_num;       ///< Touch sensor channel number.
    uint8_t chac_idx;     ///< Channel control register index.
    uint8_t chac_val;     ///< Bit mask used to enable the sensor channel.
};

#if defined(ARDUINO_UNOR4_MINIMA)
static const CapTouchPinMapping capTouchMappings[] = {
  {0,  true,  9,  1, (1 << 1)},
  {1,  true,  8,  1, (1 << 0)},
  {2,  true, 34,  4, (1 << 2)},
  {3,  true, 13,  1, (1 << 5)},
  {4,  false, 0,  0,  0},
  {5,  false, 0,  0,  0},
  {6,  false, 0,  0,  0},
  {7,  false, 0,  0,  0},
  {8,  true, 11,  1, (1 << 3)},
  {9,  true,  2,  0, (1 << 2)},
  {10, false, 0,  0,  0},
  {11, true, 10,  1, (1 << 2)},
  {12, false, 0,  0,  0},
  {13, true, 12,  1, (1 << 4)},
  {14, false, 0,  0,  0}, // A0 unsupported.
  {15, true, 21,  2, (1 << 5)}, // A1.
  {16, true, 22,  2, (1 << 6)}, // A2.
  {17, false, 0,  0,  0}, // A3 unsupported.
  {18, false, 0,  0,  0}, // A4 unsupported.
  {19, false, 0,  0,  0}, // A5 unsupported.
  {20, true,  0,  0, (1 << 0)}  // LOVE pin.
};
#elif defined(ARDUINO_UNOR4_WIFI)
static const CapTouchPinMapping capTouchMappings[] = {
  {0,  true,  9,  1, (1 << 1)},
  {1,  true,  8,  1, (1 << 0)},
  {2,  true, 13,  1, (1 << 5)},
  {3,  true, 34,  4, (1 << 2)},
  {4,  false, 0,  0,  0},
  {5,  false, 0,  0,  0},
  {6,  true, 12,  1, (1 << 4)},
  {7,  false, 0,  0,  0},
  {8,  true, 11,  1, (1 << 3)},
  {9,  true,  2,  0, (1 << 2)},
  {10, false, 0,  0,  0},
  {11, true,  7,  0, (1 << 7)},
  {12, true,  6,  0, (1 << 6)},
  {13, false, 0,  0,  0},
  {14, false, 0,  0,  0}, // A0 unsupported.
  {15, true, 21,  2, (1 << 5)},
  {16, true, 22,  2, (1 << 6)},
  {17, false, 0,  0,  0}, // A3 unsupported.
  {18, false, 0,  0,  0}, // A4 unsupported.
  {19, false, 0,  0,  0}, // A5 unsupported.
  {20, true, 27, 3, (1<<3)}  
};

#define CTSUMCH0_LOVE  0x1B //TS27
#define LOVE_PORT 1
#define LOVE_PIN 13 //Capacitive button connected to pin P113

#endif

/**
 * @brief A user-friendly class for capacitive touch sensing.
 *
 * Create an instance with:  
 *   CapacitiveTouch ct = CapacitiveTouch(LOVE_BUTTON);
 */
class CapacitiveTouch {
public:
    /**
     * @brief Constructs a capacitive touch sensor for the given pin.
     * @param pin The Arduino pin number (such as LOVE_BUTTON).
     */
    CapacitiveTouch(uint8_t pin);

    /**
     * @brief Initializes the sensor.
     *
     * Configures the pin and initializes the CTSU/DTC hardware.
     */
    void begin();

    /**
     * @brief Reads the raw sensor value.
     *
     * Initiates a measurement cycle and returns the raw value.
     * @return int The raw sensor value.
     */
    int read();

    /**
     * @brief Checks if the sensor is touched.
     *
     * Compares the raw value to the set threshold.
     * @return true if the sensor is touched, false otherwise.
     */
    bool isTouched();

    /**
     * @brief Sets the detection threshold.
     * @param threshold The new threshold value.
     */
    void setThreshold(int threshold);

    /**
     * @brief Retrieves the current detection threshold.
     * @return int The current threshold value.
     */
    int getThreshold();

private:
    uint8_t _pin;       ///< The sensor pin.
    int _threshold;     ///< The detection threshold.
    uint8_t _ts_num;    ///< Sensor channel number (from mapping).
    uint8_t _chac_idx;  ///< Channel control index (from mapping).
    uint8_t _chac_val;  ///< Bit mask (from mapping).
    uint8_t _sensorIndex; ///< Index assigned after enabling the channel.
    /**
     * @brief Looks up the mapping for a given pin.
     * @param pin The Arduino pin.
     * @param mapping Reference to a mapping structure to fill.
     * @return true if mapping found and supported, false otherwise.
     */
    bool lookupMapping(uint8_t pin, CapTouchPinMapping &mapping);

       // Low-level helper functions as static members
    static bool setTouchMode(uint8_t pin);
    static void startTouchMeasurement(bool fr);
    static bool touchMeasurementReady();
};

#endif // CAPACITIVE_TOUCH_H
