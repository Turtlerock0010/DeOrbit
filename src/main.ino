/*
Program: ADD VI Code
Creation: June 1st, 2026
Contributors: Daniel Principe, Owen King, Yadhu Barath
Team: The Pink Fluffy Unicorns [83]
Use: The code that goes into ADD VI
*/

// Library Dependencies
#include <PestoLink-Receive.h>
#include <Alfredo_NoU3.h>
#include <stdio.h>

// In-File Dependencies
#include "MotorPID.h"
#include "Drivetrain.h"

// Motor Functions Init
// N/A; Nothing here for now

// PID Init
// N/A; Nothing here for now

// --- Variables ---
float measured_angle = 27.451;
float angular_scale = (5.0*2.0*PI) / measured_angle;
float movementSpeed = 1;



void setup() {
  PestoLink.begin("ADD VI");
  Serial.begin(115200);

  NoU3.begin();
  NoU3.calibrateIMUs(); // this takes exactly one second. Do not move the robot during calibration.

  beginDrivetrain(); // Starts the drivetrain
}

void loop() {
  // Prints robot angle to serial
  static unsigned long lastPrintTime = 0;
  if (lastPrintTime + 100 < millis()){
    Serial.printf("gyro yaw (radians): %.3f\r\n",  NoU3.yaw * angular_scale );
    lastPrintTime = millis();
  }

  // This measures your batteries voltage and sends it to PestoLink
  float batteryVoltage = NoU3.getBatteryVoltage();
  PestoLink.printBatteryVoltage(batteryVoltage);

  if (PestoLink.isConnected()) {
    // --- Robot Functions ---
    // N/A; Nothing here for now

    updateDrivetrain(PestoLink.getAxis(0), PestoLink.getAxis(1), PestoLink.getAxis(2), movementSpeed);
    NoU3.setServiceLight(LIGHT_ENABLED);
  } else {
    updateDrivetrain(0, 0, 0, 0);
    NoU3.setServiceLight(LIGHT_DISABLED);
  }
}
