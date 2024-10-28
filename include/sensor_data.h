#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdio.h>

//structure to hold data
typedef struct {
    float x;
    float y;
    float z;
} SensorData;

SensorData readSimulatedSensor();
void handleSignal(int signal);
void logData(SensorData data, FILE *logFile);
void checkThreshold(SensorData data, float threshold);
void calculateAverage(SensorData *dataBuffer, int bufferSize, float *avgX, float *avgY, float *avgZ);
void printBuffer(SensorData *dataBuffer, int bufferSize);

#endif //SENSOR_DATA_H
