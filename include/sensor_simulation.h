#ifndef SENSOR_SIMULATION_H
#define SENSOR_SIMULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define a structure to hold sensor data
typedef struct {
    float x;
    float y;
    float z;
} SensorData;

// Function declarations
SensorData readSimulatedSensor();
void handleSignal(int signal);
void logData(SensorData data, FILE *logFile);
void checkThreshold(SensorData data, float threshold);
void calculateAndPrintStatistics(SensorData *readings, int count);

#endif // SENSOR_SIMULATION_H
