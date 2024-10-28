#include "sensor_simulation.h"
#include <windows.h> // For Sleep()
#include <signal.h>  // For handling signals
#include <math.h>    // For mathematical functions
#include <stdio.h>

// Global variables for controlling simulation
volatile int running = 1; // Flag for running the simulation
int sleepTime = 10; // Set to 100 milliseconds for faster printing
FILE *logFile; // File pointer for logging data
float sumX = 0.0, sumY = 0.0, sumZ = 0.0; // Sums for average calculation
int count = 0; // Count of readings
#define MAX_READINGS 1000 // Define max readings to store
SensorData readings[MAX_READINGS]; // Array to store readings

// Function to simulate reading data from a sensor
SensorData readSimulatedSensor() {
    SensorData data;
    data.x = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0; // Range: -1.0 to 1.0
    data.y = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0;
    data.z = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0;
    return data;
}

// Signal handler for graceful shutdown
void handleSignal(int signal) {
    printf("\nExiting gracefully...\n");
    running = 0; // Set the running flag to false
    if (logFile) {
        fclose(logFile); // Close log file
    }
}

// Threshold value for alerting
float threshold = 0.8;

// Main function with improved initialization
int main(int argc, char *argv[]) {
    // Seed the random number generator
    srand(time(0));

    // Open log file
    logFile = fopen("sensor_data_log.csv", "w");
    if (!logFile) {
        fprintf(stderr, "Error: Could not open log file for writing.\n");
        return EXIT_FAILURE; // Exit with failure
    }
    fprintf(logFile, "Timestamp,X,Y,Z\n"); // Write header

    // Configure sleep time and threshold via command-line arguments
    if (argc > 1) {
        sleepTime = atoi(argv[1]); // Set sleep time from command-line argument
        if (sleepTime <= 0) {
            fprintf(stderr, "Invalid sleep time. Using default: 100ms\n");
            sleepTime = 10; // Default to 100ms
        }
    }

    if (argc > 2) {
        threshold = atof(argv[2]); // Set threshold from command-line argument
        if (threshold < 0) {
            fprintf(stderr, "Invalid threshold. Using default: 0.8\n");
            threshold = 0.8;
        }
    }

    // Register signal handler for graceful shutdown
    signal(SIGINT, handleSignal);

    // Loop to continuously read sensor data
    while (running) {
        // Get simulated data
        SensorData data = readSimulatedSensor();
        
        // Print the data
        printf("Sensor Data - X: %.2f, Y: %.2f, Z: %.2f\n", data.x, data.y, data.z);
        
        // Log the data to the file
        logData(data, logFile);
        
        // Check for threshold exceedance
        checkThreshold(data, threshold); // Pass threshold to the function

        // Update sums for statistics calculation
        readings[count] = data; // Store the reading
        sumX += data.x;
        sumY += data.y;
        sumZ += data.z;
        count++;

        // Sleep for specified milliseconds to simulate real-time reading
        Sleep(sleepTime);
    }

    // Calculate and print statistics before exiting
    calculateAndPrintStatistics(readings, count);
    
    // Close the log file if open
    if (logFile) {
        fclose(logFile);
    }

    return 0;
}
