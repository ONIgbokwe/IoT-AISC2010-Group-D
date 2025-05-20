# Energy Efficiency Management System

This project demonstrates the use of motion sensors and relay modules to manage energy efficiency in an environment. The system detects motion across multiple rooms and controls the power state of non-essential appliances (simulated by LEDs) to save energy when no motion is detected for an extended period.

## Features

- **Motion Detection**: Each room is equipped with a PIR (Passive Infrared) motion sensor to detect occupancy.
- **Power-Saving Mode**: If no motion is detected in any room for **10 minutes**, the system will turn off non-essential appliances such as lights, fans, and heaters (simulated using LEDs).
- **Reactivation Upon Motion**: When motion is detected, the system restores power to appliances immediately, turning them back on.
- **Real-time Feedback**: The system provides real-time feedback on the occupancy status and the power state of appliances (ON/OFF) via an I2C-enabled LCD or serial monitor.
  
## Components

- **Arduino MCU (e.g., Arduino Uno)**
- **PIR Motion Sensors**: To detect motion in each room.
- **Relay Module**: To control the power to appliances (simulated by LEDs).
- **LEDs**: Representing appliances such as lights, fans, and heaters.
- **I2C LCD** (optional): To display real-time status.
- **Wires and Breadboard**: For connecting components.

## System Behavior

### Motion Detection and Power-Saving Logic:
- The system checks for motion every second.
- If no motion is detected for **10 minutes**, the system assumes all rooms are vacant and activates power-saving mode by turning off the LEDs (simulating turning off appliances).
- If motion is detected again, the system resets the timer and restores power to the LEDs (simulating turning on appliances).

Find the link for simulation below:
https://www.tinkercad.com/things/2O8J5WONK5p-case-study-1-q3/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits&sharecode=8Yrco2wnO2yWhgbT-BrrzI8vbwjitc5o4nDL2WVqWI0
