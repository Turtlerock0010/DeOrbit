//Self Definition
#ifndef VISION_H
#define VISION_H

// Data Request Functions
int* get_TagRow(int rowIndex);
int get_isTagVisable();

// Setup & Loop Functions
void beginVision();
void updateVision();
void printVision();


#endif
