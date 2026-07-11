// Dependencies
#include "Vision.h"


HUSKYLENS huskylens;

// Variables
int targetIndex = 0;
int xCenter = 0;
int yCenter = 0;
int width = 0;
int height = 0;
bool isTagVisable = false;

// Lists
float availableTags[3][7] = { // On the format of tagID, xCenter, yCenter, width, height, distance, angle
  {0, 0, 0, 0, 0, 0, 0}, // Hub Tag
  {0, 0, 0, 0, 0, 0, 0}, // Right Trench Tag
  {0, 0, 0, 0, 0, 0, 0}, // Left Trench Tag
};

// Return Data Functions
float* get_TagRow(int rowIndex) { return availableTags[rowIndex];}
bool get_isTagVisable(){ return isTagVisable;}


void beginVision(){
  // Start I2C communication
  Wire.begin(8,9); // SDA, SCL
  // [NOTE]: These are the pins I used for I2C on a ESP32-S3 Dev Board.
  // The pins may be different on the NoU3!!!

  if (!huskylens.begin(Wire)) {
    Serial.println(F("HuskyLens begin failed!"));
  }

  // Sets the Husky Lens to automatically go to tag detection
  huskylens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION);
}


void updateVision() {
  if (huskylens.requestBlocks() && huskylens.countBlocks() >= 1){
    isTagVisable = true;

    // Adjusts the tag data matrix to match sensor readings
    for (int i = 0; i <= 2; i++) {
      HUSKYLENSResult result = huskylens.getBlock(i); // get tag with ID zero
      targetIndex = result.ID - 1;

      availableTags[targetIndex][0] = i;
      availableTags[targetIndex][1] = result.xCenter;
      availableTags[targetIndex][2] = result.yCenter;
      availableTags[targetIndex][3] = result.width;
      availableTags[targetIndex][4] = result.height;
      availableTags[targetIndex][5] = ((FOCAL_CONSTANT * FIELD_APRIL_TAG_WIDTH) / result.width);

      // NOTE: There is an issue with yaw outputting incorrect angles.
      //       The issue is fixable & tolerable but not solvable.
      float ratio = (float)result.width / (float)result.height;
      ratio = constrain(ratio, -1.0, 1.0);
      availableTags[i][6] = acos(ratio) * (180.0 / PI);
      // NOTE: the constant 16.5 is subject to change when tags are measured
    }
  } else {
    isTagVisable = false;
  }
}

void printVision() {
  Serial.print(F("tagID= "));  Serial.print(availableTags[0][0]);
  Serial.print(F(" ,xCenter= ")); Serial.print(availableTags[0][1]);
  Serial.print(F(" ,yCenter= ")); Serial.print(availableTags[0][2]);
  Serial.print(F(" ,width= "));   Serial.print(availableTags[0][3]);
  Serial.print(F(" ,height= "));   Serial.print(availableTags[0][4]);
  Serial.print(F(" ,distance= "));   Serial.print(availableTags[0][5]);
  Serial.print(F(" ,angle= "));  Serial.println(availableTags[0][6]); // println here to end the line
}
