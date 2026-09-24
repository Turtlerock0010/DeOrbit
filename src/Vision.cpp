#include "Vision.h"


void beginVision() {
  distanceSensor.setTimeout(500);
  
  // Checks if distance sensor initializes
  if (!distanceSensor.init()) {
    Serial.println("VL53L0X initialization failed");
    return;
  }

  distanceSensor.startContinuous();
}

float readDistanceCm() {
  uint16_t millimeters = distanceSensor.readRangeContinuousMillimeters();

  if (distanceSensor.timeoutOccurred()) {
    Serial.println("VL53L0X timeout");
    return NAN;
  }

  return millimeters / 10.0f;
}
