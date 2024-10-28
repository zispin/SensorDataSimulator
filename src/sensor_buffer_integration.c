#include "sensor_data.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <signal.h>

#define DEFAULT_BUFFER_SIZE 10
#define DEFAULT_THRESHOLD 0.5 // set a default threshold for detection

// circular buffer to store sensor data
SensorData* dataBuffer;
int bufferIndex = 0;
int bufferSize;
float threshold;
FILE *logFile; // file pointer for logging data

// function to simulate reading data from a sensor
SensorData readSimulatedSensor() {
    SensorData data;
    data.x = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0; // generate random X
    data.y = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0; // generate random Y
    data.z = ((float)rand() / (float)(RAND_MAX)) * 2.0 - 1.0; // generate random Z
    return data; // return the simulated data
}

// function to log data to a CSV file
void logData(SensorData data, FILE *logFile) {
    if (logFile) {
        fprintf(logFile, "%.2f,%.2f,%.2f\n", data.x, data.y, data.z); // log the data
    }
}

// function to add data to the circular buffer
void addToBuffer(SensorData data) {
    dataBuffer[bufferIndex] = data; // store data in buffer
    printf("Adding data to buffer at index %d: X: %.2f, Y: %.2f, Z: %.2f\n", bufferIndex, data.x, data.y, data.z);
    bufferIndex = (bufferIndex + 1) % bufferSize; // wrap around the buffer
}

// function to calculate the average of the sensor readings
void calculateAverage(float* avgX, float* avgY, float* avgZ) {
    float sumX = 0, sumY = 0, sumZ = 0;

    for (int i = 0; i < bufferSize; i++) {
        sumX += dataBuffer[i].x; // sum up X values
        sumY += dataBuffer[i].y; // sum up Y values
        sumZ += dataBuffer[i].z; // sum up Z values
    }

    *avgX = sumX / bufferSize; // calculate average for X
    *avgY = sumY / bufferSize; // calculate average for Y
    *avgZ = sumZ / bufferSize; // calculate average for Z
}

// function to check if any sensor value exceeds the threshold
void checkThreshold(SensorData data) {
    if (data.x > threshold || data.y > threshold || data.z > threshold) {
        printf("Threshold exceeded: X: %.2f, Y: %.2f, Z: %.2f\n", data.x, data.y, data.z); // alert if threshold exceeded
    }
}

// function to print the current state of the buffer
void printBuffer() {
    printf("Current Buffer State:\n");
    for (int i = 0; i < bufferSize; i++) {
        printf("[%d] X: %.2f, Y: %.2f, Z: %.2f\n", i, dataBuffer[i].x, dataBuffer[i].y, dataBuffer[i].z);
    }
}

// signal handler for graceful shutdown
void handleSignal(int signal) {
    printf("\nExiting gracefully...\n");
    if (logFile) {
        fclose(logFile); // close log file if open
    }
    free(dataBuffer); // free allocated memory
    exit(0);
}

// main function with improved initialization
int main(int argc, char *argv[]) {
    srand(time(0)); // seed random number generator

    // configure buffer size and threshold via command-line arguments
    bufferSize = (argc > 1) ? atoi(argv[1]) : DEFAULT_BUFFER_SIZE; // set buffer size
    threshold = (argc > 2) ? atof(argv[2]) : DEFAULT_THRESHOLD; // set threshold

    // allocate memory for the circular buffer
    dataBuffer = (SensorData*)malloc(bufferSize * sizeof(SensorData));
    if (!dataBuffer) {
        fprintf(stderr, "Memory allocation failed!\n"); // handle memory allocation failure
        return EXIT_FAILURE;
    }

    // open log file
    logFile = fopen("sensor_data_log.csv", "w");
    if (!logFile) {
        fprintf(stderr, "Could not open log file for writing.\n"); // handle log file opening failure
        free(dataBuffer);
        return EXIT_FAILURE;
    }
    fprintf(logFile, "X,Y,Z\n"); // write header to log file

    // register signal handler for graceful shutdown
    signal(SIGINT, handleSignal);

    // initialize the buffer with zeros
    for (int i = 0; i < bufferSize; i++) {
        dataBuffer[i].x = 0.0; // initialize X
        dataBuffer[i].y = 0.0; // initialize Y
        dataBuffer[i].z = 0.0; // initialize Z
    }

    while (1) {
        SensorData data = readSimulatedSensor(); // read simulated data
        addToBuffer(data); // add data to buffer
        logData(data, logFile); // log data to file

        // calculate and print average values
        float avgX, avgY, avgZ;
        calculateAverage(&avgX, &avgY, &avgZ);
        printf("Average Values - X: %.2f, Y: %.2f, Z: %.2f\n", avgX, avgY, avgZ);

        // check for threshold exceedance
        checkThreshold(data);

        // print the buffer for debugging
        printBuffer();

        Sleep(1000); // adjust sleep time as necessary
    }

    // clean up memory
    fclose(logFile); // close log file if open
    free(dataBuffer); // free allocated memory
    return 0;
}
