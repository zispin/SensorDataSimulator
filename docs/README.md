# Sensor Data Simulation

This project simulates reading data from a sensor in real time. It generates random sensor readings for three axes (X, Y, Z) and performs several operations, including checking for threshold exceedance, logging data to a CSV file, and calculating statistics.

## Features

- Real-Time Simulation: Generates random sensor data continuously.
- Threshold Alerts: Notifies when sensor readings exceed a specified threshold.
- Data Logging: Logs the sensor readings to a CSV file for later analysis.
- Statistics Calculation: Calculates and displays mean and standard deviation of the sensor readings upon exit.

## Requirements

- C compiler (GCC or compatible)
- Windows environment (for `Sleep()` function)

## Getting Started

Step One: Installation

Clone the repository and navigate into the project directory:

git clone https://github.com/zispin/sensor-simulation.git cd sensor-simulation

Step Two: Compile the Project

Compile the project using the following command:

gcc -o sensor_simulation src/simulation.c src/data_processing.c -Iinclude -lm

Step Three: Run the Simulation

Run the compiled executable with optional command-line arguments for sleep time and threshold:

./sensor_simulation <sleep_time_in_ms> <threshold_value>

**Example:**

./sensor_simulation 500 0.8

- `<sleep_time_in_ms>`: Time in milliseconds to simulate the delay between readings (default is 1000 ms).
- `<threshold_value>`: Threshold for sensor alerts (default is 0.8).

### Output

The program outputs sensor data readings to the console and logs them to a file named `sensor_data_log.csv`. Upon termination (e.g., pressing `Ctrl+C`), the program prints statistics, including the mean and standard deviation of the readings.

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request for improvements or bug fixes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by sensor data processing applications.
- Uses C standard libraries for random number generation, file I/O, and signal handling.

