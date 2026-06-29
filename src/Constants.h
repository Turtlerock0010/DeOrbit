// Self Definition
#ifndef CONSTANTS_H
#define CONSTANTS_H


// --- Drivetrain Terminals ---
const int frontLeftTerminal = 1;
const int frontRightTerminal = 8;
const int rearLeftTerminal = 4;
const int rearRightTerminal = 5;

// --- Motor Terminals ---
const int indexerMotorTerminal = 1; // PLACEHOLDER VALUE
const int shooterMotorTerminal = 2; // PLACEHOLDER VALUE
const int intakeMotorTerminal = 3; // PLACEHOLDER VALUE

// --- Servo Terminals ---
const int turretServoTerminal = 1; // PLACEHOLDER VALUE

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
