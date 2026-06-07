// Dependencies
#include "Wire.h"
#include "HUSKYLENS.h"
#include "Vision.h"

HUSKYLENS huskylens;

// Variables
int xCenter = 0;
int yCenter = 0;
int width = 0;
int height = 0;
bool isTagVisable = false;

// Lists
int availableTags[3][6] = { // On the format of tagID, xCenter, yCenter, width, height, distance
  {0, 0, 0, 0, 0, 0}, // Hub Tag
  {0, 0, 0, 0, 0, 0}, // Right Trench Tag
  {0, 0, 0, 0, 0, 0}, // Left Trench Tag
};

// Return Data Functions
int* get_TagRow(int rowIndex) { return availableTags[rowIndex];}
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
      availableTags[i][1] = i;
      availableTags[i][1] = result.xCenter;
      availableTags[i][2] = result.yCenter;
      availableTags[i][3] = result.width;
      availableTags[i][4] = result.height;
      availableTags[i][5] = ((FOCAL_CONSTANT * 16.5) / result.width);
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
  Serial.print(F(" ,height= "));  Serial.println(availableTags[0][4]); // println here to end the line
}
