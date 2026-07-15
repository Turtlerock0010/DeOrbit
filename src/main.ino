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
#include <VL53L0X.h>

// In-File Dependencies
#include "Constants.h"
#include "MotorPID.h"
#include "Drivetrain.h"

// Motor Functions Init
NoU_Motor intakeMotor(intakeMotorTerminal);
NoU_Motor kickerMotor(kickerMotorTerminal);
NoU_Motor shooterMotor(shooterMotorTerminal);
NoU_Motor spindexerMotor(spindexerMotorTerminal);

// Servo Functions Init
NoU_Servo turretServo(turretServoHeader);
NoU_Servo hoodServo(hoodServoHeader);
NoU_Servo leftAgitatorServo(leftAgitatorServoHeader);
NoU_Servo rightAgitatorServo(rightAgitatorServoHeader);


// PID Init
// N/A; Nothing here for now

// --- Variables ---
float measured_angle = 27.451;
float angular_scale = (5.0*2.0*PI) / measured_angle;
float movementSpeed = 1;
float turret_servo_angle = 0;
float* hub_tag_data;
float shortestRecordedDistanceAngle = 0;
float shortestRecordedDistance = 0;
unsigned long time_at_flywheel_start = 0;

bool flyWheelTimeLatch = true;
bool is_flyWheelReady = false;
float testTurretReserveLatch = true;
bool respinTurret = false;
bool freeTurret = true;
bool oppositeAgitatorDir = false;
bool is_Scanning = false;

AutoAimState auto_aim_state = AutoAimState::STANDBY;
RobotState robot_state = RobotState::STANDBY;
VL53L0X distanceSensor;

// Functions
void turretServoHandler(float turretServoAngle) {
  turretServo.write(turretServoAngle * 210/180 * 1/2); 
  // Weird math, but it sets the range back to 0-360 degrees :)
}

float readDistanceCm() {
  return distanceSensor.readRangeContinuousMillimeters() / 10.0;
}


void setup() {
  PestoLink.begin("ADD VI");
  Serial.begin(115200);

  NoU3.begin();
  NoU3.calibrateIMUs(); // this takes exactly one second. Do not move the robot during calibration.

  // Per Dependency Starts
  beginDrivetrain(); // Starts the drivetrain

  // Distance Sensor Initialization
  distanceSensor.setTimeout(500);
  distanceSensor.init();
  distanceSensor.startContinuous();

  kickerMotor.setInverted(false);
  shooterMotor.setInverted(true);
  intakeMotor.setInverted(true);
  spindexerMotor.setInverted(true);


  // Set Turret Motor Direction

}

void loop() {
  // Prints robot angle to serial
  //static unsigned long lastPrintTime = 0;
  //if (lastPrintTime + 100 < millis()){
  //  Serial.printf("gyro yaw (radians): %.3f\r\n", readDistanceCm());
  //  lastPrintTime = millis();
  //}

  // This measures your batteries voltage and sends it to PestoLink
  float batteryVoltage = NoU3.getBatteryVoltage();
  PestoLink.printBatteryVoltage(batteryVoltage);

  if (PestoLink.isConnected()) {
    // --- Robot Functions ---

    // Get robot heading (in radians) from the gyro
    float heading = NoU3.roll * ANGULAR_SCALE;

    //PestoLink.printfTerminal("Distance: %.3f\n", readDistanceCm());


    // State Machine Control Logic
    if (PestoLink.buttonHeld(RIGHT_TRIGGER_BUTTON) || PestoLink.keyHeld(Key::E)) {
      // SHOOT
      robot_state = RobotState::SHOOT;
    } else if (PestoLink.buttonHeld(LEFT_TRIGGER_BUTTON) || PestoLink.keyHeld(Key::Q)) {
      // INTAKE
      robot_state = RobotState::INTAKE;
    } else if (PestoLink.buttonHeld(CIRCLE_BUTTON) || PestoLink.keyHeld(Key::R)) {
      // STIR
      robot_state = RobotState::STIR;
    } else {
      // STANDBY
      robot_state = RobotState::STANDBY;
    }


    // State Machine Execution Logic
    switch (robot_state) {
      case RobotState::SHOOT:

        if (flyWheelTimeLatch) { // Gathers millis at first press
          flyWheelTimeLatch = false;
          time_at_flywheel_start = millis();
        }


        // Times the kicker after 1 second of button held to warm up the flywheel.
        if (millis() - time_at_flywheel_start >= 1000) {
          // When turret flywheel is at max RPM
          kickerMotor.set(1);
        } else {
          // When turret flywheel is warming up
          kickerMotor.set(0);
        }

        shooterMotor.set(1);
        spindexerMotor.set(1);
        intakeMotor.set(1);

        // Agitates the hopper
        if (oppositeAgitatorDir) {
          leftAgitatorServo.write(0);
          rightAgitatorServo.write(15);
          oppositeAgitatorDir = false;
        } else {
          leftAgitatorServo.write(15);
          rightAgitatorServo.write(0);
          oppositeAgitatorDir = true;
        }
      
        break;

      case RobotState::INTAKE:
        shooterMotor.set(0);
        spindexerMotor.set(0);
        kickerMotor.set(0);
        intakeMotor.set(1);
        break;

      case RobotState::STIR:
        shooterMotor.set(0);
        spindexerMotor.set(1);
        kickerMotor.set(0);
        intakeMotor.set(1);

        // Agitates the hopper
        if (oppositeAgitatorDir) {
          leftAgitatorServo.write(0);
          rightAgitatorServo.write(15);
          oppositeAgitatorDir = false;
        } else {
          leftAgitatorServo.write(15);
          rightAgitatorServo.write(0);
          oppositeAgitatorDir = true;
        }
        
        break;

      case RobotState::STANDBY:
      default:
        // General Motor Standby
        shooterMotor.set(0);
        spindexerMotor.set(0);
        kickerMotor.set(0);
        intakeMotor.set(0);

        // Flywheel Standby
        flyWheelTimeLatch = true;
        time_at_flywheel_start = 0;

        // Agitator Standby
        leftAgitatorServo.write(0);
        rightAgitatorServo.write(15);
        break;
    }

    if (PestoLink.keyHeld(Key::P) || PestoLink.buttonHeld(UP_DPAD_BUTTON)) {
      auto_aim_state = AutoAimState::PREPARE;
    }


    switch (auto_aim_state) {
      case AutoAimState::PREPARE:
        // Checks to see if the turret servo zeroed.
        if (turretServo.getDegrees() != 0) {
          turret_servo_angle = 0;
          turretServoHandler(turret_servo_angle);
        } else {
          // Swaps to the next state.
          auto_aim_state = AutoAimState::SCAN;
        }
      break;

      case AutoAimState::SCAN:
        if (PestoLink.keyHeld(Key::H) || PestoLink.buttonHeld(DOWN_DPAD_BUTTON)) {
          // So that when the button is released, it goes to lock.
          is_Scanning = true;

          // Incriments the angle by 2, but is adjustable.
          turret_servo_angle += 2;
          turretServoHandler(turret_servo_angle);

          // Checks for most shortest distance
          float currentDistanceReading = readDistanceCm();
          if (currentDistanceReading < shortestRecordedDistance) {
            // If shorter distance found, then records data.
            shortestRecordedDistance = currentDistanceReading;
            shortestRecordedDistanceAngle = turret_servo_angle;
          }

        } else if (is_Scanning){
          // Swaps to the next state.
          auto_aim_state = AutoAimState::LOCK;
        }
      break;

      case AutoAimState::LOCK:
        // Locks on via custom servo handler and turret servo angle.
        turret_servo_angle = shortestRecordedDistanceAngle;
        turretServoHandler(turret_servo_angle);
        auto_aim_state = AutoAimState::STANDBY;

      break;

      case AutoAimState::STANDBY:
      default:
        // Resets all values for future auto aims.
        is_Scanning = false;
        shortestRecordedDistanceAngle = 0;
        shortestRecordedDistance = 9999; 
        // This is a big number that definitely won't happen :)
      break;
    }

    

    // Turret Rotation Buttons

    // Set Zero
    if (PestoLink.keyHeld(Key::T)) {
      turret_servo_angle = 0;
      turretServoHandler(turret_servo_angle);
    }

    if ( PestoLink.keyHeld(Key::U)) {
      turret_servo_angle = 360;
      turretServoHandler(turret_servo_angle);
    }

    //turret_servo_angle = turretReserveAngle - (boundedHeading * (180/PI) - 180) / 2) - 90
    // turretReserveAngle - (boundedHeading * (180/PI) - 180) / 2) - 90;
    //turretServo.write(turret_servo_angle);
    //PestoLink.printfTerminal("Off Ang: %.3f\r\n || Res Ang: %.3f\r\n || Tur Res: %.3f\r\n", 360 - relativeAngle, turretWorldReserveAngle, turret_servo_angle);

    // Turn Clockwise
    if (PestoLink.buttonHeld(LEFT_DPAD_BUTTON)) {
      turret_servo_angle += 1;

      turretServoHandler(turret_servo_angle);
    }

    // Turn Counterclockwise
    if (PestoLink.buttonHeld(RIGHT_DPAD_BUTTON)) {
      turret_servo_angle -= 1;
      turretServoHandler(turret_servo_angle);
    }

    updateDrivetrain(PestoLink.getAxis(2), PestoLink.getAxis(1), movementSpeed);
    NoU3.setServiceLight(LIGHT_ENABLED);
  } else {
    updateDrivetrain(0, 0, 0);
    NoU3.setServiceLight(LIGHT_DISABLED);
  }
}
