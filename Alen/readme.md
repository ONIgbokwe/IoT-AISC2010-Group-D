#  2. Environmental Monitoring Simulation – Multi-Room (2 Rooms)

![tinker_sim_short_gify](https://github.com/user-attachments/assets/546f9e6e-1da1-4d50-9e3e-f0ffecabbb29)

Live Simulation : https://www.tinkercad.com/things/62OxKchC6fj-environmental-monitoring-hvac-control?sharecode=Y49i-6hM7RdGHMtvykeewaLG6v7k1MxPvvfT0AiaxKA  <br><br> This project simulates a **multi-room environmental monitoring system** using an Arduino Uno. It demonstrates how to monitor and control the temperature and humidity of **two rooms** using:

- **TMP36 temperature sensors** (for each room)
- **Potentiometers** to simulate humidity (for each room)
- **LEDs** to represent fan and heater control
- **I2C LCD display** to show real-time readings for both rooms

---

##  Features

- Real-time monitoring of **temperature and humidity** for 2 rooms
- **Automatic control** of fans and heaters based on thresholds:

  - Heater **ON** if temperature < 20°C  
  - Heater **OFF** if temperature > 25°C  
  - Fan **ON** if temperature > 30°C  
  - Fan **ON** if temperature > 25°C **and** humidity > 70%  
  - Fan **OFF** if temperature < 25°C **and** humidity < 40%  
  - Fan **ON** if humidity > 90%

- LCD display shows **both rooms simultaneously** (`T:` for temperature, `H:` for humidity)
- Serial Monitor logs **real-time fan/heater status** for each room

---

##  Hardware Components

- 1 × Arduino Uno  
- 2 × TMP36 temperature sensors  
- 2 × 10kΩ potentiometers (simulate humidity)  
- 2 × Heater LEDs (e.g., red)  
- 2 × Fan LEDs (e.g., green)  
- 6 × 220Ω resistors  
- 1 × I2C LCD (16x2)  
- Breadboard + jumper wires

---

##  How It Works

1. The TMP36 sensors provide real-time temperature data.
2. Potentiometers simulate humidity values (0–100%).
3. Fan and heater LEDs are controlled based on the logic conditions.
4. A 16x2 I2C LCD displays current `T:` (Temperature) and `H:` (Humidity) for **Room 1** and **Room 2**.
5. Serial Monitor logs include exact temperature, humidity, and device statuses.

---

