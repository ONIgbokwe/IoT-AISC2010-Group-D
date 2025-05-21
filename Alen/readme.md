Environmental Monitoring Simulation – Multi-Room (2 Rooms)
This project simulates a multi-room environmental monitoring system using an Arduino Uno. It demonstrates how to monitor and control the temperature and humidity of two rooms using:

TMP36 temperature sensors (for each room)

Potentiometers to simulate humidity

LEDs to represent fan and heater control

I2C LCD display to show real-time readings for both rooms

Features
Real-time monitoring of temperature and humidity for 2 rooms

Automatic control of fans and heaters based on thresholds:

Heater ON if temperature < 20°C

Heater OFF if temperature > 25°C

Fan ON if temperature > 30°C

Fan ON if temperature > 25°C and humidity > 70%

Fan OFF if temperature < 25°C and humidity < 40%

Fan ON if humidity > 90%

LCD display shows both rooms simultaneously (T: for temperature, H: for humidity)

Serial Monitor logs detailed fan/heater status for each room

Hardware Components
1 × Arduino Uno

2 × TMP36 temperature sensors

2 × 10kΩ potentiometers (simulate humidity)

2 × Heater LEDs (e.g., red)

2 × Fan LEDs (e.g., green)

6 × 220Ω resistors

1 × I2C LCD (16x2)

Breadboard + jumper wires

How It Works
Each loop:

Reads real temperature from TMP36 sensors

Reads simulated humidity from potentiometers

Applies control logic

Updates LEDs and LCD

Prints detailed output to Serial Monitor

