#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

// Dependencies
#include <Arduino.h>
#include <Alfredo_NoU3.h>

// In-File Dependencies
#include "Constants.h"

// Variables
// N/A

// Drivetrain Motors Init
inline NoU_Motor frontLeftMotor(frontLeftTerminal); // [NOTE]: THIS IS CHANGEABLE!
inline NoU_Motor frontRightMotor(frontRightTerminal); // [NOTE]: THIS IS CHANGEABLE!
inline NoU_Motor rearLeftMotor(rearLeftTerminal); // [NOTE]: THIS IS CHANGEABLE!
inline NoU_Motor rearRightMotor(rearRightTerminal); // [NOTE]: THIS IS CHANGEABLE!

// Drivetrain Init
inline NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &rearLeftMotor, &rearRightMotor);

// Start Functions
void beginDrivetrain();

//Loop Functions
void updateDrivetrain(float gamepadX, float gamepadY, float movement_speed);

#define MEASURED_ANGLE 27.451;
#define ANGULAR_SCALE (5.0 * 2.0 * PI) / MEASURED_ANGLE;


#endif