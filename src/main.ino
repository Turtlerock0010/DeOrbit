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
#include "Vision.h"

// Motor Functions Init
NoU_Motor intakeMotor(intakeMotorTerminal);
NoU_Motor kickerMotor(kickerMotorTerminal);
NoU_Motor shooterMotor(shooterMotorTerminal);
NoU_Motor spindexerMotor(spindexerMotorTerminal);

// Servo Functions Init
NoU_Servo turretServo(turretServoHeader);

// PID Init
// N/A; Nothing here for now

// --- Variables ---
float measured_angle = 27.451;
float angular_scale = (5.0*2.0*PI) / measured_angle;
float movementSpeed = 1;
float turret_servo_angle;


// Functions
// N/A; Nothing here for now


void setup() {
  PestoLink.begin("ADD VI");
  Serial.begin(115200);

  NoU3.begin();
  NoU3.calibrateIMUs(); // this takes exactly one second. Do not move the robot during calibration.

  // Per Dependency Starts
  beginDrivetrain(); // Starts the drivetrain
  beginVision(); // Starts the vision system

  kickerMotor.setInverted(true);
  shooterMotor.setInverted(false);
  intakeMotor.setInverted(false);
  spindexerMotor.setInverted(true);
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

    // Spindexer Motor Buttons
    if (PestoLink.buttonHeld(1)) {
      spindexerMotor.set(1);
    } else {
      spindexerMotor.set(0);
    }

    // Shooter Motor Buttons
    if (PestoLink.buttonHeld(7)) {
      shooterMotor.set(1);
    } else {
      shooterMotor.set(0);
    }

    // Intake Motor Buttons
    if (PestoLink.buttonHeld(6)) {
      intakeMotor.set(1);
    } else {
      intakeMotor.set(0);
    }

    // Kicker Motor Buttons
    if (PestoLink.buttonHeld(3)) {
      kickerMotor.set(1);
    }
    else {
      kickerMotor.set(0);
    }

    // Turret Rotation Buttons
    if (PestoLink.buttonHeld(4)) {
      turret_servo_angle++;
      turretServo.write(turret_servo_angle);
    }

    if (PestoLink.buttonHeld(5)) {
      turret_servo_angle--;
      turretServo.write(turret_servo_angle);
    }

    // Per Dependency Updates
    updateVision(); 
    updateDrivetrain(PestoLink.getAxis(0), PestoLink.getAxis(1), PestoLink.getAxis(2), movementSpeed);
    NoU3.setServiceLight(LIGHT_ENABLED);
  } else {
    updateDrivetrain(0, 0, 0, 0);
    NoU3.setServiceLight(LIGHT_DISABLED);
  }
}
