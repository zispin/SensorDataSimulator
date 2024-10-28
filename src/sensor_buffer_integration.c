#include "sensor_data.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <signal.h>

#define DEFAULT_BUFFER_SIZE 10
#define DEFAULT_THRESHOLD 0.5 // Set a default threshold for detection

// Circular buffer to store sensor data
SensorData* dataBuffer;
int bufferIndex = 0;
int bufferSize;
float threshold;
FILE *logFile; // File pointer for logging data

// Function to simulate reading data from a sensor
SensorData readSimulatedSensor() {
    SensorData data;
    data.x = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0;
    data.y = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0;
    data.z = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0;
    return data;
}

// Function to log data to a CSV file
void logData(SensorData data, FILE *logFile) {
    if (logFile) {
        fprintf(logFile, "%.2f,%.2f,%.2f\n", data.x, data.y, data.z);
    }
}

// Function to add data to the circular buffer
void addToBuffer(SensorData data) {
    dataBuffer[bufferIndex] = data;
    printf("Adding data to buffer at index %d: X: %.2f, Y: %.2f, Z: %.2f\n", bufferIndex, data.x, data.y, data.z);
    bufferIndex = (bufferIndex + 1) % bufferSize; // Circular behavior
}

// Function to calculate the average of the sensor readings
void calculateAverage(float* avgX, float* avgY, float* avgZ) {
    float sumX = 0, sumY = 0, sumZ = 0;
    
    for (int i = 0; i < bufferSize; i++) {
        sumX += dataBuffer[i].x;
        sumY += dataBuffer[i].y;
        sumZ += dataBuffer[i].z;
    }
    
    *avgX = sumX / bufferSize;
    *avgY = sumY / bufferSize;
    *avgZ = sumZ / bufferSize;
}

// Function to check if any sensor value exceeds the threshold
void checkThreshold(SensorData data) {
    if (data.x > threshold || data.y > threshold || data.z > threshold) {
        printf("Threshold exceeded: X: %.2f, Y: %.2f, Z: %.2f\n", data.x, data.y, data.z);
    }
}

// Function to print the current state of the buffer
void printBuffer() {
    printf("Current Buffer State:\n");
    for (int i = 0; i < bufferSize; i++) {
        printf("[%d] X: %.2f, Y: %.2f, Z: %.2f\n", i, dataBuffer[i].x, dataBuffer[i].y, dataBuffer[i].z);
    }
}

// Signal handler for graceful shutdown
void handleSignal(int signal) {
    printf("\nExiting gracefully...\n");
    if (logFile) {
        fclose(logFile); // Close log file if open
    }
    free(dataBuffer); // Free allocated memory
    exit(0);
}

// Main function with improved initialization
int main(int argc, char *argv[]) {
    srand(time(0));
    
    // Configure buffer size and threshold via command-line arguments
    bufferSize = (argc > 1) ? atoi(argv[1]) : DEFAULT_BUFFER_SIZE;
    threshold = (argc > 2) ? atof(argv[2]) : DEFAULT_THRESHOLD;

    // Allocate memory for the circular buffer
    dataBuffer = (SensorData*)malloc(bufferSize * sizeof(SensorData));
    if (!dataBuffer) {
        fprintf(stderr, "Memory allocation failed!\n");
        return EXIT_FAILURE;
    }

    // Open log file
    logFile = fopen("sensor_data_log.csv", "w");
    if (!logFile) {
        fprintf(stderr, "Could not open log file for writing.\n");
        free(dataBuffer);
        return EXIT_FAILURE;
    }
    fprintf(logFile, "X,Y,Z\n"); // Write header

    // Register signal handler for graceful shutdown
    signal(SIGINT, handleSignal);

    // Initialize the buffer with zeros
    for (int i = 0; i < bufferSize; i++) {
        dataBuffer[i].x = 0.0;
        dataBuffer[i].y = 0.0;
        dataBuffer[i].z = 0.0;
    }

    while (1) {
        SensorData data = readSimulatedSensor();
        addToBuffer(data);
        logData(data, logFile); // Log data to file

        // Calculate and print average values
        float avgX, avgY, avgZ;
        calculateAverage(&avgX, &avgY, &avgZ);
        printf("Average Values - X: %.2f, Y: %.2f, Z: %.2f\n", avgX, avgY, avgZ);

        // Check for threshold exceedance
        checkThreshold(data);

        // Print the buffer for debugging
        printBuffer();

        Sleep(1000); // Adjust sleep time as necessary
    }

    // Clean up memory
    fclose(logFile); // Close log file if open
    free(dataBuffer);
    return 0;
}
