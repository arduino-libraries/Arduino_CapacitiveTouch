#include "Arduino_CapacitiveTouch.h"

CapacitiveTouch touchButton = CapacitiveTouch(LOVE_BUTTON);

void setup() {
  Serial.begin(9600);
  
  if(touchButton.begin()){
    Serial.println("Capacitive touch sensor initialized.");
  } else {
    Serial.println("Failed to initialize capacitive touch sensor. Please use a supported pin.");
    while(true);
  }

  touchButton.setThreshold(2000);
}

void loop() {
  // Read the raw value from the capacitive touch sensor.
  int sensorValue = touchButton.read();
  Serial.print("Raw value: ");
  Serial.println(sensorValue);

  // Check if the sensor is touched (raw value exceeds the threshold).
  if (touchButton.isTouched()) {
    Serial.println("Button touched!");
  }
  
  delay(100);
}
