#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include "sensor_simulation.h"

//functions for data processing
void addToBuffer(SensorData* buffer, int* index, int bufferSize, SensorData data);
void calculateAverage(SensorData* buffer, int bufferSize, float* avgX, float* avgY, float* avgZ);

#endif //DATA_PROCESSING_H
