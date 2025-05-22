# 🔆 Smart Lighting Automation – Multi-Room (4 Rooms)

This Arduino-based project simulates an intelligent lighting automation system for 4 different rooms. Each room is equipped with sensors that detect both **motion** and **ambient light**, allowing smart control of lighting to **save energy** and enhance convenience.

---

## 🎯 Features

- 🧠 **Motion & Light-Aware Lighting Control** for 4 separate rooms
- 💡 **Turn ON** light only when motion is detected and the room is dark
- 🔄 **Dim** the light after 5 minutes of no activity
- 📴 **Turn OFF** the light after 15 minutes of no motion or when ambient light increases
- 🧪 Configurable for **quick simulation** (10s/20s version)
- 📟 Real-time **Serial Monitor logging** for debugging

---

## 🧰 Components Used

| Component              | Quantity |
|------------------------|----------|
| Arduino UNO            | 1        |
| PIR Motion Sensors     | 4        |
| LDR (Photoresistors)   | 4        |
| 10kΩ Resistors (for LDRs) | 4     |
| LEDs (Room Lights)     | 4        |
| 220Ω Resistors (for LEDs) | 4     |
| Breadboard + Jumper Wires | As needed |

---

## 🔌 Circuit Overview

Each room includes:
- 1 × PIR sensor for motion detection
- 1 × LDR for ambient light sensing (via voltage divider with 10kΩ)
- 1 × LED to simulate a room light
- Independent control logic using `millis()` for non-blocking timing

---

## 🚦 Control Logic

| Condition                                           | Action         |
|----------------------------------------------------|----------------|
| Ambient light < threshold AND motion detected      | Turn **ON** LED (255) |
| No motion for **5 minutes**                        | **Dim** LED (100)     |
| No motion for **15 minutes** OR light is sufficient | Turn **OFF** LED (0)  |

---

## 🧪 Quick Test Mode

For simulation purposes:
- You can test the system by replacing timing values to:
  - **10 seconds** (dim)
  - **20 seconds** (off)

```cpp
// Replace these for testing
300000UL → 10000UL  // 5 mins → 10 sec
900000UL → 20000UL  // 15 mins → 20 sec

