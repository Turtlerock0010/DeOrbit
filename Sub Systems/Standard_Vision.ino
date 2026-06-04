// [NOTE]
// This code is not meant to be used on the robot.
// Instead, this code is purely meant as reference for the actual code to be put in.
// For fab: You must get the wires right as the module must operate within the 3.3V and 5V range. So servo pins or Qwiic port will do fine.
// These modules cost 35$ each so be careful 😭.

#include "Wire.h"
#include "HUSKYLENS.h"

HUSKYLENS huskylens; // Defines the sensor as an object

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);


  // All of the stuff below this is really important VVVVV
  // Start the I2C communication protocol
  Wire.begin(8,9);
  
  // Attempt to connect to the HuskyLens over I2C
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Initialization failed!"));
    Serial.println(F("1. Check that HuskyLens 'Protocol Type' is set to I2C."));
    Serial.println(F("2. Recheck your SDA/SCL wiring connections."));
    delay(1000);
  }
  
  Serial.println(F("HuskyLens successfully connected!"));
}

void loop() {
  // Checks if there is no data being sent to the microcontroller
  if (!huskylens.request()) {
    Serial.println(F("Fail to request data from HUSKYLENS, check connection!"));
    delay(500);
    return;
  }

  // Check if there are any tags detected on screen
  if (!huskylens.available()) {
    Serial.println(F("No tags detected on screen."));
  } else {
    Serial.println(F("====== Tag(s) Detected! ======"));
    
    // Loop through all currently detected targets
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();


      // VVV The stuff below here is formatting stuff, but still somethings are important to look out for VVV

      // Important Variables:
      /*
      result.ID: Checks what tag it sees, important for tag differentiation
      result.width: Checks the width of the tag on the screen; can be used for distance measurement if there isn't any built in function
      result.heigh: Checks the height of the tag on the screen; can be used for distance measurement if there isn't any built in function
      result.xCenter: Checks the X POS of the tag relative to the screen
      result.yCenter: Checks the Y POS of the tag relative to the screen
      */
        
      // Tag recognition data comes back as a "Block"
      if (result.command == COMMAND_RETURN_BLOCK) {
        Serial.print(F("Tag ID: "));
        Serial.println(result.ID); // ID = 0 means unlearned/generic tag, ID >= 1 is a learned tag
        
        Serial.print(F("Center X: "));
        Serial.print(result.xCenter);
        Serial.print(F(" | Center Y: "));
        Serial.println(result.yCenter);
        
        Serial.print(F("Width: "));
        Serial.print(result.width);
        Serial.print(F(" | Height: "));
        Serial.println(result.height);
        
        Serial.println(F("-----------------------------"));
      }
    }
  }

  // Making sure that the thing is readable but this is absolutely removable
  delay(200);
