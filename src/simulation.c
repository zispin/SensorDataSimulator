#include "sensor_simulation.h"
#include <windows.h> // for Sleep()
#include <signal.h>  // for handling signals
#include <math.h>    // for mathematical functions
#include <stdio.h>

// global variables for controlling simulation
volatile int running = 1; // flag for running the simulation
int sleepTime = 10; // set to 10 milliseconds for faster printing
FILE *logFile; // file pointer for logging data
float sumX = 0.0, sumY = 0.0, sumZ = 0.0; // sums for average calculation
int count = 0; // count of readings
#define MAX_READINGS 1000 // define max readings to store
SensorData readings[MAX_READINGS]; // array to store readings

// function to simulate reading data from a sensor
SensorData readSimulatedSensor() {
    SensorData data;
    data.x = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0; // range: -1.0 to 1.0
    data.y = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0; // generate random Y
    data.z = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0; // generate random Z
    return data; // return the simulated data
}

// signal handler for graceful shutdown
void handleSignal(int signal) {
    printf("\nExiting gracefully...\n");
    running = 0; // set the running flag to false
    if (logFile) {
        fclose(logFile); // close log file
    }
}

// threshold value for alerting
float threshold = 0.8; // default threshold value

// main function with improved initialization
int main(int argc, char *argv[]) {
    // seed the random number generator
    srand(time(0));

    // open log file
    logFile = fopen("sensor_data_log.csv", "w");
    if (!logFile) {
        fprintf(stderr, "Error: Could not open log file for writing.\n"); // handle log file opening failure
        return EXIT_FAILURE; // exit with failure
    }
    fprintf(logFile, "Timestamp,X,Y,Z\n"); // write header

    // configure sleep time and threshold via command-line arguments
    if (argc > 1) {
        sleepTime = atoi(argv[1]); // set sleep time from command-line argument
        if (sleepTime <= 0) {
            fprintf(stderr, "Invalid sleep time. Using default: 10ms\n"); // warn for invalid input
            sleepTime = 10; // default to 10ms
        }
    }

    if (argc > 2) {
        threshold = atof(argv[2]); // set threshold from command-line argument
        if (threshold < 0) {
            fprintf(stderr, "Invalid threshold. Using default: 0.8\n"); // warn for invalid input
            threshold = 0.8; // reset to default
        }
    }

    // register signal handler for graceful shutdown
    signal(SIGINT, handleSignal);

    // loop to continuously read sensor data
    while (running) {
        // get simulated data
        SensorData data = readSimulatedSensor();
        
        // print the data
        printf("Sensor Data - X: %.2f, Y: %.2f, Z: %.2f\n", data.x, data.y, data.z);
        
        // log the data to the file
        logData(data, logFile);
        
        // check for threshold exceedance
        checkThreshold(data, threshold); // pass threshold to the function

        // update sums for statistics calculation
        readings[count] = data; // store the reading
        sumX += data.x; // accumulate sum for X
        sumY += data.y; // accumulate sum for Y
        sumZ += data.z; // accumulate sum for Z
        count++; // increment reading count

        // sleep for specified milliseconds to simulate real-time reading
        Sleep(sleepTime); // pause execution
    }

    // calculate and print statistics before exiting
    calculateAndPrintStatistics(readings, count);
    
    // close the log file if open
    if (logFile) {
        fclose(logFile); // clean up log file
    }

    return 0; // exit successfully
}
