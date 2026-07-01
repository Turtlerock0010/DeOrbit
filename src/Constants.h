// Self Definition
#ifndef CONSTANTS_H
#define CONSTANTS_H


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
const int rotationAxesInversion = -1;


#endif
