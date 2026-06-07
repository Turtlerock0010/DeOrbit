//Self Definition
#ifndef VISION_H
#define VISION_H

// Vision Constants
const float M_TAG_WIDTH = 16.5;
const float M_TAG_DISTANCE = 45.7;
const float M_TAG_PIXEL_WIDTH = 122;
const float FOCAL_CONSTANT = (M_TAG_PIXEL_WIDTH * M_TAG_DISTANCE) / M_TAG_WIDTH;

// Data Request Functions
int* get_TagRow(int rowIndex);
bool get_isTagVisable();

// Setup & Loop Functions
void beginVision();
void updateVision();
void printVision();


#endif
