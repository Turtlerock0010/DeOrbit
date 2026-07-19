/*
Program: DeOrbit Code
Creation: June 1st, 2026
Contributors: Daniel Principe, Owen King, Yadhu Barath
Team: The Pink Fluffy Unicorns [83]
Use: The code that goes into DeOrbit
*/

// Library Dependencies
#include <PestoLink-Receive.h>
#include <Alfredo_NoU3.h>
#include <stdio.h>

// In-File Dependencies
#include "Constants.h"
#include "Drivetrain.h"
#include "Vision.h"

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
float hood_servo_angle = 0;
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

// Functions
void turretServoHandler(float turretServoAngle) {
  turretServo.write(turretServoAngle * 210/180 * 1/2); 
  // Weird math, but it sets the range back to 0-360 degrees :)
}

void hoodDistanceAdjustment(float recordedDistance) {
  hood_servo_angle = recordedDistance * linearHoodAngleMultiplier; // Change this based on field data!!!
  hood_servo_angle = constrain(hood_servo_angle, 0, 85);
  hoodServo.write(hood_servo_angle);
}

void setup() {
  PestoLink.begin("DeOrbit");
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
  intakeMotor.setInverted(false);
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
    PestoLink.printfTerminal("Distance: %.3f\n", readDistanceCm());


    // --------------- STATE MACHINES ---------------

    // State Machine Control Logic
    if (PestoLink.buttonHeld(RIGHT_TRIGGER_BUTTON) || PestoLink.keyHeld(Key::E)) {
      // SHOOT
      robot_state = RobotState::SHOOT;
    } else if (PestoLink.buttonHeld(LEFT_TRIGGER_BUTTON) || PestoLink.keyHeld(Key::Q)) {
      // INTAKE
      robot_state = RobotState::INTAKE;
    } else if (PestoLink.buttonHeld(LEFT_BUMPER_BUTTON) || PestoLink.keyHeld(Key::I)) {
      // OUTTAKE
      robot_state = RobotState::OUTTAKE;
    } else if (PestoLink.buttonHeld(RIGHT_BUMPER_BUTTON) || PestoLink.keyHeld(Key::Y)) {
      // JUGGLE
      robot_state = RobotState::JUGGLE;
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
        if (millis() - time_at_flywheel_start >= 1500) {
          // When turret flywheel is at max RPM
          kickerMotor.set(1);
          spindexerMotor.set(.7);
          if (millis() - time_at_flywheel_start >= 3000) {
            // Agitates the hopper
            static unsigned long lastAgitateTime = 0;
            if (lastAgitateTime + 100 < millis()){
              if (oppositeAgitatorDir) {
                leftAgitatorServo.write(0);
                rightAgitatorServo.write(30);
                oppositeAgitatorDir = false;
              } else {
                leftAgitatorServo.write(30);
                rightAgitatorServo.write(0);
                oppositeAgitatorDir = true;
              }
              lastAgitateTime = millis();
            }
          }
        } else {
          // When turret flywheel is warming up
          kickerMotor.set(0);
          spindexerMotor.set(0);
        }

        shooterMotor.set(1);
        intakeMotor.set(0);
      
        break;

      case RobotState::INTAKE:
        shooterMotor.set(0);
        spindexerMotor.set(0);
        kickerMotor.set(0);
        intakeMotor.set(1);
        break;

      case RobotState::OUTTAKE:
        shooterMotor.set(0);
        spindexerMotor.set(0);
        kickerMotor.set(0);
        intakeMotor.set(-1);
        break;
      
      case RobotState::JUGGLE:
        shooterMotor.set(0);
        spindexerMotor.set(-1);
        kickerMotor.set(-1);
        intakeMotor.set(0);
        break;

      case RobotState::STIR:
        shooterMotor.set(0);
        spindexerMotor.set(-1);
        kickerMotor.set(0);
        intakeMotor.set(1);

        // Agitates the hopper
        if (oppositeAgitatorDir) {
          leftAgitatorServo.write(0);
          rightAgitatorServo.write(30);
          oppositeAgitatorDir = false;
        } else {
          leftAgitatorServo.write(30);
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

          // Incriments the angle by 4, but is adjustable.
          turret_servo_angle += 8;
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
        turret_servo_angle = shortestRecordedDistanceAngle - turretBacklashOffset;
        turretServoHandler(turret_servo_angle);
        hoodDistanceAdjustment(shortestRecordedDistance);
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

    

    // --------------- MICELANEOUS KEYBINDS ---------------

    // --- Turret Servo Adjust ---
    // Set Zero
    if (PestoLink.keyHeld(Key::T)) {
      turret_servo_angle = 0;
      turretServoHandler(turret_servo_angle);
    }

    // Set to 360
    if ( PestoLink.keyHeld(Key::U)) {
      turret_servo_angle = 360;
      turretServoHandler(turret_servo_angle);
    }

    // Set 90
    if (PestoLink.buttonHeld(OPTION_LEFT)) {
      turret_servo_angle = 90;
      turretServoHandler(turret_servo_angle);
    }

    // Set to 270
    if (PestoLink.buttonHeld(OPTION_RIGHT)) {
      turret_servo_angle = 270;
      turretServoHandler(turret_servo_angle);
    }

    // Turn Clockwise
    if (PestoLink.buttonHeld(LEFT_DPAD_BUTTON) || PestoLink.keyHeld(Key::Digit1)) {
      turret_servo_angle += 8;

      turretServoHandler(turret_servo_angle);
    }

    // Turn Counterclockwise
    if (PestoLink.buttonHeld(RIGHT_DPAD_BUTTON) || PestoLink.keyHeld(Key::Digit2)) {
      turret_servo_angle -= 8;
      turretServoHandler(turret_servo_angle);
    }


    // --- Hood Servo Adjust ---
    // Set Zero
    if (PestoLink.keyHeld(Key::F)) {
      hood_servo_angle = 0;
      hoodServo.write(hood_servo_angle);
    }

    // Turn Clockwise
    if (PestoLink.keyHeld(Key::G)) {
      hood_servo_angle = 85;
      hoodServo.write(hood_servo_angle);
    }
    

    // Turn Clockwise
    if (PestoLink.keyHeld(Key::Digit1) || PestoLink.buttonHeld(SQUARE_BUTTON)) {
      beginVision();
      turretServoHandler(turret_servo_angle);
    }


    updateDrivetrain(PestoLink.getAxis(1), PestoLink.getAxis(2), movementSpeed);
    NoU3.setServiceLight(LIGHT_ENABLED);
  } else {
    updateDrivetrain(0, 0, 0);
    NoU3.setServiceLight(LIGHT_DISABLED);
  }
}
