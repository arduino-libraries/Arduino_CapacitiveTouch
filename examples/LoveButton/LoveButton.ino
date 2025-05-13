#include "CapacitiveTouch.h"

CapacitiveTouch touchButton = CapacitiveTouch(LOVE_BUTTON);

void setup() {
  Serial.begin(9600);
  touchButton.begin();
  touchButton.setThreshold(9000);
}

void loop() {
  // Read the raw value from the capacitive touch sensor.
  int sensorValue = touchButton.read();
  Serial.print("Raw value: ");
  Serial.println(sensorValue);

  // Check if the sensor is touched (raw value exceeds the threshold).
  if (touchButton.isTouched()) {
    Serial.println("D1 touched!");
  }
  
  delay(100);
}
