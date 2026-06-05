#ifndef MOTOR_PID_H
#define MOTOR_PID_H

// Dependencies
#include <Arduino.h>
#include <Alfredo_NoU3.h>


class MotorPID {
  public:
    // Constructor
    MotorPID(NoU_Motor& givenMotor);

    // PID constants
    float Kp;
    float Ki;
    float Kd;

    // Public methods
    void setZieglerNicholsConstants(float Ku, float Tu);
    void updateMotor();
    void setAngle(float givenAngle);

  private:
    // Initialize Given Motor
    NoU_Motor& myMotor;

    // Target Angle Adjustment
    float targetAngle;

    // PID variables  
    float lastError;
    float integralSum;

    // PID timing
    unsigned long lastPIDTime;
    const unsigned long PIDInterval; 
};

#endif
