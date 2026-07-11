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
#include "Constants.h"
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
float turret_servo_angle = 0;
float* hub_tag_data;

RobotState robot_state = STANDBY;


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

  kickerMotor.setInverted(false);
  shooterMotor.setInverted(true);
  intakeMotor.setInverted(true);
  spindexerMotor.setInverted(true);
}

void loop() {
  // Prints robot angle to serial
  //static unsigned long lastPrintTime = 0;
  //if (lastPrintTime + 100 < millis()){
  //  Serial.printf("gyro yaw (radians): %.3f\r\n",  NoU3.yaw * angular_scale );
  //  lastPrintTime = millis();
  //}

  hub_tag_data = get_TagRow(0);

  // This measures your batteries voltage and sends it to PestoLink
  float batteryVoltage = NoU3.getBatteryVoltage();
  PestoLink.printBatteryVoltage(batteryVoltage);

  if (PestoLink.isConnected()) {
    // --- Robot Functions ---

    // Get robot heading (in radians) from the gyro
    float heading = NoU3.roll * ANGULAR_SCALE;

    static unsigned long lastPrintTime = 0;
    if (lastPrintTime + 100 < millis()){
      Serial.printf("heading: %.3f\r\n", heading*180/PI);
      lastPrintTime = millis();
    }

    // State Machine Control Logic
    if (PestoLink.buttonHeld(RIGHT_TRIGGER_BUTTON) || PestoLink.keyHeld(Key::E)) {
      // SHOOT
      robot_state = SHOOT;
    } else if (PestoLink.buttonHeld(LEFT_TRIGGER_BUTTON) || PestoLink.keyHeld(Key::Q)) {
      // INTAKE
      robot_state = INTAKE;
    } else if (PestoLink.buttonHeld(CIRCLE_BUTTON) || PestoLink.keyHeld(Key::R)) {
      // STIR
      robot_state = STIR;
    } else {
      // STANDBY
      robot_state = STANDBY;
    }


    // State Machine Execution Logic
    switch (robot_state) {
      case SHOOT:
        shooterMotor.set(1);
        spindexerMotor.set(1);
        kickerMotor.set(1);
        intakeMotor.set(1);
        break;

      case INTAKE:
        shooterMotor.set(0);
        spindexerMotor.set(0);
        kickerMotor.set(0);
        intakeMotor.set(1);
        break;

      case STIR:
        shooterMotor.set(0);
        spindexerMotor.set(1);
        kickerMotor.set(0);
        intakeMotor.set(1);
        break;

      case STANDBY:
      default:
        shooterMotor.set(0);
        spindexerMotor.set(0);
        kickerMotor.set(0);
        intakeMotor.set(0);
        break;
    }
    

    // Turret Rotation Buttons

    // Set Zero
    if (PestoLink.keyHeld(Key::T)) {
      turret_servo_angle = 0;
      turretServo.write(turret_servo_angle);
    }

    if (PestoLink.buttonHeld(SQUARE_BUTTON) || PestoLink.keyHeld(Key::G)) {
      
      robot_state = SHOOT;
      turret_servo_angle = 117;
      turretServo.write(turret_servo_angle);
    }

    // Turn Clockwise
    if (PestoLink.buttonHeld(LEFT_BUMPER_BUTTON) || PestoLink.keyHeld(Key::F)) {
      if (turret_servo_angle < 220) {
        turret_servo_angle += 1;
      }
      turretServo.write(turret_servo_angle);
    }

    // Turn Counterclockwise
    if (PestoLink.buttonHeld(RIGHT_BUMPER_BUTTON) || PestoLink.keyHeld(Key::C)) {
      turret_servo_angle -= 1;
      turretServo.write(turret_servo_angle);
    }

    // Per Dependency Updates
    updateVision(); 
    updateDrivetrain(PestoLink.getAxis(0), PestoLink.getAxis(1), movementSpeed);
    NoU3.setServiceLight(LIGHT_ENABLED);
  } else {
    updateDrivetrain(0, 0, 0);
    NoU3.setServiceLight(LIGHT_DISABLED);
  }
}
