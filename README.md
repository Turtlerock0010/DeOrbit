# ADD-VI
<img width="1695" height="960" alt="image" src="https://github.com/user-attachments/assets/f84a916c-1976-49ff-936d-714fc5278448" />

> No robot yet... But here is our MiniFRC 11.5 robot: Hungry Hungry Butterfly!

> [!NOTE]
> There is no robot yet! This page has been premade as preseason work. Stay tuned for when the MiniFRC development process begins!

## About
Welcome to ADD VI code respository, A repository for the programmers of MiniFRC team [83] Pink Fluffy Unicorns! Code will be updated frequently and changes will be documented in the [discussion page](https://github.com/Turtlerock0010/ADD-VI/discussions) for changes. For spectators of the repository, please feel free to comb through the repository to see how we do things here or copy some code for use in your robot!

> [!IMPORTANT]
> For team members accessing the code to push into the NoU3, please use go to [the releases page](https://github.com/Turtlerock0010/ADD-VI/releases) for stable code.

> [!NOTE]
> As an alternative to [the releases page](https://github.com/Turtlerock0010/ADD-VI/releases), copy and pasting is still viable through looking in the `src` folder! Although, I wouldn't necessarily recommend accessing the code that way due to the multi-file nature of it.



### Robot Contributors
- Owen
- [@Turtlerock0010](https://github.com/Turtlerock0010) || Daniel
- [@Ruffle15cute](https://github.com/Ruffle15cute) || Yadhu
- Oliver
- Karan


## Extra Information

### Layouts
None at the moment... 👀
> [Controller Layout Page](https://www.padcrafter.com/?templates=Hungry+Hungry+Butterfly+Controller+Layout&plat=1&leftTrigger=Outtake&rightTrigger=Intake&dpadUp=L4&dpadRight=L3&dpadLeft=L2%2FL1&dpadDown=Ground&yButton=Stow&aButton=Flip+End+Effector&leftStick=Strafe+Robot&rightStick=Rotate+Robot) if anyone is wondering where I got it from.

### PID Controller

#### Defining A Motor
- `MotorPID myPIDMotor(myNOUMotor);`
- Defines a motor to the PID controller.
- Requires a NoU motor be already defined before defining the MotorPID.

#### Updating A Motor
- `myPIDMotor.updateMotor();`
- Updates the motor to the correct state.
- Requires the piece of code to be within the `loop()` function of the file.

#### Adjusting Kp Value
- `myPIDMotor.Kp = 0.0;`
- Sets the Kp value to the desired variable.
- Requires a MotorPID to already be created.

#### Adjusting Ki Value
- `myPIDMotor.Ki = 0.0;`
- Sets the Ki value to the desired variable.
- Requires a MotorPID to already be created.

#### Adjusting Kd Value
- `myPIDMotor.Kd = 0.0;`
- Sets the Kd value to the desired variable.
- Requires a MotorPID to already be created.

#### Setting the desired PID angle
- `myPIDMotor.setAngle(90);`
- Sets the motor to the desired angle.
- Requires a MotorPID to already be created.

### General Robot Adjustments

#### Setting Drivetrain Motor Inversion
- `frontLeftMotor.setInverted(true);`
- `frontRightMotor.setInverted(true);`
- `rearLeftMotor.setInverted(true);`
- `rearRightMotor.setInverted(true);`
- Sets inversion or no inversion for the drivetrain motors.
- Requires drivetrain motors to already be defined.
