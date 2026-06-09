//Self Definition
#ifndef VISION_H
#define VISION_H

// Vision Constants
const float M_TAG_WIDTH = 16.5;
const float M_TAG_DISTANCE = 45.7;
const float M_TAG_PIXEL_WIDTH = 108; // Alternative values that work: 118, 122
const float FOCAL_CONSTANT = (M_TAG_PIXEL_WIDTH * M_TAG_DISTANCE) / M_TAG_WIDTH;
const float FIELD_APRIL_TAG_WIDTH = 16.5; // Changeable based on measured size!

// Data Request Functions
float* get_TagRow(int rowIndex);
bool get_isTagVisable();

// Setup & Loop Functions
void beginVision();
void updateVision();
void printVision();


#endif
