#include "CapacitiveTouch.h"

// Create a CapacitiveTouch object for the LOVE button using the defined LOVE_BUTTON macro.
CapacitiveTouch loveButton = CapacitiveTouch(1);

void setup() {
  Serial.begin(9600);
  // Initialize the LOVE button sensor.
  loveButton.begin();
  // Optionally adjust the threshold if needed.
  loveButton.setThreshold(5000);
  Serial.println("LOVE button sensor test started.");
}

void loop() {
  // Retrieve the raw sensor reading.
  int sensorValue = loveButton.read();

  // Print the raw value.
  Serial.print("Raw value: ");
  Serial.println(sensorValue);

  // Check if the sensor is touched (raw value exceeds the threshold).
  if (loveButton.isTouched()) {
    Serial.println("D1 touched!");
  }
  
  delay(100);
}
