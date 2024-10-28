#include "data_processing.h"
#include "sensor_simulation.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

// function to add data to the circular buffer
void addToBuffer(SensorData* buffer, int* index, int bufferSize, SensorData data) {
    buffer[*index] = data;
    printf("Adding data to buffer at index %d: X: %.2f, Y: %.2f, Z: %.2f\n", *index, data.x, data.y, data.z);
    *index = (*index + 1) % bufferSize; // makes the buffer circular
}

// function to calculate the average of the sensor readings
void calculateAverage(SensorData* buffer, int bufferSize, float* avgX, float* avgY, float* avgZ) {
    float sumX = 0, sumY = 0, sumZ = 0;
    for (int i = 0; i < bufferSize; i++) {
        sumX += buffer[i].x;
        sumY += buffer[i].y;
        sumZ += buffer[i].z;
    }
    *avgX = sumX / bufferSize; // average for X
    *avgY = sumY / bufferSize; // average for Y
    *avgZ = sumZ / bufferSize; // average for Z
}

// function to check threshold and alert
void checkThreshold(SensorData data, float threshold) {
    if (data.x > threshold || data.y > threshold || data.z > threshold) {
        printf("ALERT! Sensor reading exceeded threshold: X: %.2f, Y: %.2f, Z: %.2f\n", data.x, data.y, data.z);
    }
}

// function to log data to a CSV file
void logData(SensorData data, FILE *logFile) {
    if (logFile) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char timestamp[26];
        strftime(timestamp, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(logFile, "%s, %.2f, %.2f, %.2f\n", timestamp, data.x, data.y, data.z);
    } else {
        fprintf(stderr, "Error: Log file not open.\n"); // error handling if log file is not accessible
    }
}

// function to calculate and print statistics
void calculateAndPrintStatistics(SensorData *readings, int count) {
    if (count > 0) {
        float meanX = 0.0, meanY = 0.0, meanZ = 0.0;

        // calculate mean
        for (int i = 0; i < count; i++) {
            meanX += readings[i].x;
            meanY += readings[i].y;
            meanZ += readings[i].z;
        }
        meanX /= count; // final mean for X
        meanY /= count; // final mean for Y
        meanZ /= count; // final mean for Z

        // calculate standard deviation
        float stdDevX = 0.0, stdDevY = 0.0, stdDevZ = 0.0;
        for (int i = 0; i < count; i++) {
            stdDevX += pow((readings[i].x - meanX), 2);
            stdDevY += pow((readings[i].y - meanY), 2);
            stdDevZ += pow((readings[i].z - meanZ), 2);
        }
        stdDevX = sqrt(stdDevX / count); // standard deviation for X
        stdDevY = sqrt(stdDevY / count); // standard deviation for Y
        stdDevZ = sqrt(stdDevZ / count); // standard deviation for Z

        printf("Statistics - Mean: X: %.2f, Y: %.2f, Z: %.2f | Std Dev: X: %.2f, Y: %.2f, Z: %.2f\n",
               meanX, meanY, meanZ, stdDevX, stdDevY, stdDevZ);
    } else {
        printf("No data collected for statistics.\n"); // message when no data is available
    }
}
