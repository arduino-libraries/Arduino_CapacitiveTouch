#include "CapacitiveTouch.h"
#include <string.h>

// Global definitions (mirroring legacy code)
#define CTSU_BASE_FREQ 24000.0

#ifndef NUM_ARDUINO_PINS
  #define NUM_ARDUINO_PINS 21
#endif

#if defined(ARDUINO_UNOR4_MINIMA)
  #define NUM_CTSU_PINS 11
#elif defined(ARDUINO_UNOR4_WIFI)
  #define NUM_CTSU_PINS 12
#endif

#define NOT_A_TOUCH_PIN 255

#if defined(ARDUINO_UNOR4_MINIMA)
  // On Minima, LOVE lives at P204
  #define LOVE_GPIO_PORT  2
  #define LOVE_GPIO_PIN   4
#elif defined(ARDUINO_UNOR4_WIFI)
  // On WiFi, LOVE lives at P113
  #define LOVE_GPIO_PORT  1
  #define LOVE_GPIO_PIN   13
#endif
// Forward declarations for internal functions used in ISRs:

typedef void (*fn_callback_ptr_t)();

static void startCTSUmeasure();  // This function resets the hardware & restarts the measurement cycle

// Global variables should be defined only once (ensure these aren't duplicated):
volatile bool ctsu_done = true;
bool free_running = true;
fn_callback_ptr_t ctsu_fn_callback = nullptr;

// Forward declarations for missing functions.
void initDTC();
bool setTouchMode(uint8_t pin);
void startTouchMeasurement(bool fr);
bool touchMeasurementReady();

// Global arrays (initialized in initGlobalArrays)
uint8_t dataIndexToTS[NUM_CTSU_PINS];
uint8_t pinToDataIndex[NUM_ARDUINO_PINS];
uint16_t results[NUM_CTSU_PINS][2];
uint16_t regSettings[NUM_CTSU_PINS][3];

int num_configured_sensors = 0;

// Dummy instances for DTC (assuming these types are defined by your system).
dtc_instance_ctrl_t wr_ctrl;
transfer_info_t wr_info;
dtc_extended_cfg_t wr_ext;
transfer_cfg_t wr_cfg = { &wr_info, &wr_ext };

dtc_instance_ctrl_t rd_ctrl;
transfer_info_t rd_info;
dtc_extended_cfg_t rd_ext;
transfer_cfg_t rd_cfg = { &rd_info, &rd_ext };

// Helper: Initialize global arrays.
static void initGlobalArrays() {
  for (uint8_t i = 0; i < NUM_ARDUINO_PINS; i++) {
    pinToDataIndex[i] = NOT_A_TOUCH_PIN;
  }
}


static void startCTSUmeasure() {
    // This helper function should reset the DTC channels and trigger a new CTSU measurement.
    ctsu_done = false;
    R_DTC_Reset(&wr_ctrl, &(regSettings[0][0]), (void *)&(R_CTSU->CTSUSSC), num_configured_sensors);
    R_DTC_Reset(&rd_ctrl, (void *)&(R_CTSU->CTSUSC), &(results[0][0]), num_configured_sensors);
    R_CTSU->CTSUCR0 = 1;
  }

// ------------------------------
// ISR Handlers
// ------------------------------
void CTSUWR_handler() {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);
    // Write ISR: typically triggers a state change.
  }
  
void CTSURD_handler() {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);
    // Read ISR: retrieve measurement counters into the results array.
}
  
void CTSUFN_handler() {
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);
    ctsu_done = true;
    if (ctsu_fn_callback) {
      ctsu_fn_callback();
    }
    if (free_running) {
      // Restart measurement cycle if in free-running mode.
      startCTSUmeasure();  
    }
}

// ------------------------------
// Low-level Hardware Initialization
// ------------------------------
void initCTSU() {
  static bool inited = false;
  if (!inited) {
    inited = true;
    // Example initialization sequence (using legacy register calls)
    R_PFS->PORT[1].PIN[12].PmnPFS = (1 << R_PFS_PORT_PIN_PmnPFS_PDR_Pos);
    delay(100);
    R_PFS->PORT[1].PIN[12].PmnPFS = (1 << R_PFS_PORT_PIN_PmnPFS_PMR_Pos) | 
                                    (12 << R_PFS_PORT_PIN_PmnPFS_PSEL_Pos);
    R_MSTP->MSTPCRC &= ~(1 << R_MSTP_MSTPCRC_MSTPC3_Pos);
    R_CTSU->CTSUCR0 = 0;
    R_CTSU->CTSUCR0 = 0x10;
    R_CTSU->CTSUCR1 = 0;
    R_CTSU->CTSUSO1 = 0x0F00;
    R_CTSU->CTSUCR1 = 3;
    R_CTSU->CTSUCR1 |= 0x40;
    delay(100);
    R_CTSU->CTSUSDPRS = 0x23;
    R_CTSU->CTSUSST   = 0x10;
    R_CTSU->CTSUDCLKC = 0x30;
    GenericIrqCfg_t rd_int_cfg = {FSP_INVALID_VECTOR, 12, ELC_EVENT_CTSU_READ};
    GenericIrqCfg_t wr_int_cfg = {FSP_INVALID_VECTOR, 12, ELC_EVENT_CTSU_WRITE};
    GenericIrqCfg_t fn_int_cfg = {FSP_INVALID_VECTOR, 12, ELC_EVENT_CTSU_END};
    IRQManager::getInstance().addGenericInterrupt(rd_int_cfg, CTSURD_handler);
    rd_ext.activation_source = rd_int_cfg.irq;
    IRQManager::getInstance().addGenericInterrupt(wr_int_cfg, CTSUWR_handler);
    wr_ext.activation_source = wr_int_cfg.irq;
    IRQManager::getInstance().addGenericInterrupt(fn_int_cfg, CTSUFN_handler);
    initDTC();
    initGlobalArrays();
  }
}

void initDTC() {
  wr_info.transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
  wr_info.transfer_settings_word_b.repeat_area       = TRANSFER_REPEAT_AREA_DESTINATION;
  wr_info.transfer_settings_word_b.irq               = TRANSFER_IRQ_END;
  wr_info.transfer_settings_word_b.chain_mode        = TRANSFER_CHAIN_MODE_DISABLED;
  wr_info.transfer_settings_word_b.src_addr_mode      = TRANSFER_ADDR_MODE_INCREMENTED;
  wr_info.transfer_settings_word_b.size              = TRANSFER_SIZE_2_BYTE;
  wr_info.transfer_settings_word_b.mode              = TRANSFER_MODE_BLOCK;
  wr_info.p_dest = (void *)&(R_CTSU->CTSUSSC);
  wr_info.p_src  = &(regSettings[0][0]);
  wr_info.num_blocks = 1;
  wr_info.length     = 3;
  
  rd_info.transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
  rd_info.transfer_settings_word_b.repeat_area       = TRANSFER_REPEAT_AREA_SOURCE;
  rd_info.transfer_settings_word_b.irq               = TRANSFER_IRQ_END;
  rd_info.transfer_settings_word_b.chain_mode        = TRANSFER_CHAIN_MODE_DISABLED;
  rd_info.transfer_settings_word_b.src_addr_mode      = TRANSFER_ADDR_MODE_INCREMENTED;
  rd_info.transfer_settings_word_b.size              = TRANSFER_SIZE_2_BYTE;
  rd_info.transfer_settings_word_b.mode              = TRANSFER_MODE_BLOCK;
  rd_info.p_dest = &(results[0][0]);
  rd_info.p_src  = (void *)&(R_CTSU->CTSUSC);
  rd_info.num_blocks = 1;
  rd_info.length     = 2;
  
  R_DTC_Open(&wr_ctrl, &wr_cfg);
  R_DTC_Enable(&wr_ctrl);
  R_DTC_Open(&rd_ctrl, &rd_cfg);
  R_DTC_Enable(&rd_ctrl);
}


// ------------------------------
// Non-Template Class Member Functions
// ------------------------------

/**
 * @brief Construct with the Arduino-side pin number (e.g. D4, A1, LOVE_BUTTON).
 * @param pin Arduino pin (0..20 on Minima, 0..27 on WiFi).
 */
CapacitiveTouch::CapacitiveTouch(uint8_t pin)
  : _pin(pin), _mappingIndex(0xFF), _threshold(500), _baseline(0), _sensorIndex(0)
{
  _mappingIndex = findMappingIndex(pin);
  if (_mappingIndex != 0xFF) {
    auto &m = capTouchMappings[_mappingIndex];
    // stash ts_num/chac_idx/chac_val if you need them—but you can also defer that until begin()
  }
}

bool CapacitiveTouch::lookupMapping(uint8_t pin, CapTouchPinMapping &mapping) {
  const uint8_t n = sizeof(capTouchMappings) / sizeof(capTouchMappings[0]);
  for (uint8_t i = 0; i < n; i++) {
    if (capTouchMappings[i].arduinoPin == pin) {
      mapping = capTouchMappings[i];
      return mapping.supported;
    }
  }
  return false;
}

bool CapacitiveTouch::begin() {
  if (_mappingIndex == 0xFF) {
    return false;
  }
  pinMode(_pin, INPUT);
  initCTSU();
  if (!setTouchMode(_pin)) {
    return false;
  }
  _sensorIndex = pinToDataIndex[_pin];

  long sum = 0;
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    sum += readRaw();
    delay(5);
  }
  _baseline = sum / CALIBRATION_SAMPLES;
  return true;
}

int CapacitiveTouch::readRaw() {
    startTouchMeasurement(false);
    while (!touchMeasurementReady()) {
      delay(1);
    }
    return results[_sensorIndex][0];
}

int CapacitiveTouch::read() {
    int raw   = readRaw();
    int delta = raw - _baseline;
    return (delta > 0) ? delta : 0;
}

bool CapacitiveTouch::isTouched() {
    int delta = read();
    return (delta >  _threshold) ||
           (delta < -_threshold);
}

void CapacitiveTouch::setThreshold(int threshold) {
  _threshold = threshold;
}

int CapacitiveTouch::getThreshold() {
  return _threshold;
}

// Explicit instantiation is no longer needed because this is a regular (non-template) class.
bool CapacitiveTouch::setTouchMode(uint8_t pin) {
    const CapTouchPinMapping *info = nullptr;
    const uint8_t n = sizeof(capTouchMappings) / sizeof(capTouchMappings[0]);
    for (uint8_t i = 0; i < n; i++) {
      if (capTouchMappings[i].arduinoPin == pin) {
        info = &capTouchMappings[i];
        break;
      }
    }
    if (!info || !info->supported || (info->ts_num == NOT_A_TOUCH_PIN)) {
      return false;
    }
    if (pinToDataIndex[pin] != NOT_A_TOUCH_PIN) {
      // Already configured.
      return false;
    }
    

    if (pin == LOVE_BUTTON) {
      // configure the physical PFS for LOVE_GPIO_[PORT|PIN]
      R_PFS->PORT[LOVE_GPIO_PORT]
            .PIN[LOVE_GPIO_PIN]
            .PmnPFS = (1 << R_PFS_PORT_PIN_PmnPFS_PMR_Pos)
                     | (12 << R_PFS_PORT_PIN_PmnPFS_PSEL_Pos);
    }
    else {
      // all the other CTSU-capable pins
      R_IOPORT_PinCfg(&g_ioport_ctrl,
                      g_pin_cfg[pin].pin,
                      IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_CTSU);
    }

    
    // Reinitialize CTSU hardware.
    initCTSU();
    
    // Enable the sensor channel.
    R_CTSU->CTSUCHAC[info->chac_idx] |= info->chac_val;
    
    // Insert sensor into global tracking arrays (legacy approach).
    int di = 0;
    for (int i = num_configured_sensors; i > 0; i--) {
      if (dataIndexToTS[i - 1] < info->ts_num) {
        di = i;
        break;
      } else {
        dataIndexToTS[i] = dataIndexToTS[i - 1];
        memcpy(&(regSettings[0][0]) + (3 * i),
               &(regSettings[0][0]) + (3 * (i - 1)), 6);
      }
    }
    if (di < num_configured_sensors) {
      for (int i = 0; i < NUM_ARDUINO_PINS; i++) {
        if ((pinToDataIndex[i] != NOT_A_TOUCH_PIN) && (pinToDataIndex[i] >= di)) {
          pinToDataIndex[i]++;
        }
      }
    }
    dataIndexToTS[di] = info->ts_num;
    regSettings[di][0] = 0x0200;
    regSettings[di][1] = 0;
    regSettings[di][2] = 0x0F00;
    pinToDataIndex[pin] = di;
    num_configured_sensors++;
    
    return true;
  }
  
void CapacitiveTouch::startTouchMeasurement(bool fr) {
    free_running = fr;
    if (ctsu_done || ((R_CTSU->CTSUST & 7) == 0)) {
      ctsu_done = false;
      R_DTC_Reset(&wr_ctrl, &(regSettings[0][0]),
                  (void *)&(R_CTSU->CTSUSSC), num_configured_sensors);
      R_DTC_Reset(&rd_ctrl, (void *)&(R_CTSU->CTSUSC),
                  &(results[0][0]), num_configured_sensors);
      R_CTSU->CTSUCR0 = 1;
    }
  }
  
bool CapacitiveTouch::touchMeasurementReady() {
    return (free_running || ctsu_done);
}