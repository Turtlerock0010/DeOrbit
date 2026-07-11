// Self Definition
#ifndef CONSTANTS_H
#define CONSTANTS_H

// --- Controller Button Definitions ---
// NOTE: These are definitions for the PS5 controller

// Trigger Buttons
#define LEFT_TRIGGER_BUTTON 6
#define RIGHT_TRIGGER_BUTTON 7

// Bumper Buttons
#define LEFT_BUMPER_BUTTON 4
#define RIGHT_BUMPER_BUTTON 5

// D-Pad Buttons
#define UP_DPAD_BUTTON 12
#define DOWN_DPAD_BUTTON 13
#define LEFT_DPAD_BUTTON 14
#define RIGHT_DPAD_BUTTON 15

// Action Buttons
#define TRIANGLE_BUTTON 3
#define CIRCLE_BUTTON 1
#define CROSS_BUTTON 0
#define SQUARE_BUTTON 2


// --- State Machine Preprocessor Directives ---
enum RobotState {
  STANDBY,
  SHOOT,
  INTAKE,
  STIR
};


// --- Drivetrain Terminals ---
const int frontLeftTerminal = 4;
const int frontRightTerminal = 6;
const int rearLeftTerminal = 3;
const int rearRightTerminal = 5;

// --- General Motor Terminals ---
const int intakeMotorTerminal = 8;
const int kickerMotorTerminal = 2;
const int shooterMotorTerminal = 7;
const int spindexerMotorTerminal = 1;

// --- General Servo Headers ---
const int turretServoHeader = 1;

// --- Motor Port Inversions ---
const bool frontLeftInversion = true;
const bool frontRightInversion = false;
const bool rearLeftInversion = true;
const bool rearRightInversion = false;

// --- Axes Inversions ---
const int xAxesInversion = 1;
const int yAxesInversion = -1;


#endif