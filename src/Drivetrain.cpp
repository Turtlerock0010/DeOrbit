// Dependenices
#include "Drivetrain.h"


void beginDrivetrain() {
  // Inversion Parts
  frontLeftMotor.setInverted(frontLeftInversion);
  frontRightMotor.setInverted(frontRightInversion);
  rearLeftMotor.setInverted(rearLeftInversion);
  rearRightMotor.setInverted(rearRightInversion);
}


void updateDrivetrain(float gamepadX, float gamepadY, float movement_speed) {
  // Sets Axes
  float fieldPowerX = xAxesInversion * movement_speed * gamepadX;
  float fieldPowerY = yAxesInversion * movement_speed * gamepadY;

  // Send to helper method
  drivetrain.arcadeDrive(fieldPowerX, fieldPowerY);
}