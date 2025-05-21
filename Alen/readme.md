# Environmental Monitoring Simulation – Multi-Room (4 Rooms)

Video URL : https://youtu.be/6MJSCUErtKM
This project simulates a **multi-room environmental monitoring system** using an **Arduino Uno**. It demonstrates how to monitor and automatically control the **temperature and humidity of four rooms** using:

- **TMP36 temperature sensors** (one for each room)
- **Simulated humidity** values (in code, no sensors used)
- **LEDs** to represent fan and heater operation
- **Two I2C LCD displays** to show real-time temperature and humidity data for all rooms

---

## ✅ Features

- Real-time monitoring of **temperature** (via TMP36) and **humidity** (randomized in code) for **4 rooms**
- **Automatic control** of heater and fan per room based on environmental conditions:

  - Heater **ON** if temperature < 20 °C  
  - Heater **OFF** if temperature > 25 °C  
  - Fan **ON** if temperature > 30 °C  
  - Fan **ON** if temperature > 25 °C **and** humidity > 70%  
  - Fan **OFF** if temperature < 25 °C **and** humidity < 40%  
  - Fan **ON** if humidity > 90%

- **Dual LCDs**:
  - LCD 1 displays Room 1 & Room 2 readings
  - LCD 2 displays Room 3 & Room 4 readings
- **Serial Monitor** logs exact temperature, humidity, and fan/heater status for all rooms

---

## 🧩 Hardware Components

| Component                    | Quantity | Purpose                      |
|-----------------------------|----------|------------------------------|
| Arduino Uno R3              | 1        | Main controller              |
| TMP36 temperature sensors   | 4        | Real temperature input       |
| Heater LEDs (e.g., red)     | 4        | Represent heater ON/OFF      |
| Fan LEDs (e.g., blue/green) | 4        | Represent fan ON/OFF         |
| 220Ω resistors              | 8        | For LEDs                     |
| I2C LCD (16x2)              | 2        | Display Room 1–4 data        |
| Breadboard + jumper wires   | –        | Connections                  |

> 🔧 **Note**: Humidity is not sensed using a physical component. It is **simulated using random values in the code** to test HVAC behavior without needing humidity sensors.

---

## ⚙️ How It Works

1. Each **TMP36 sensor** provides real-time temperature input for one room.
2. **Humidity values are randomly generated in software** between 30–100%.
3. Based on the values, **heaters and fans are turned ON/OFF** using defined logic.
4. **Two I2C LCDs** display:
   - `T:` → Temperature (°C)
   - `H:` → Humidity (%)
5. **Serial Monitor** logs all values and control actions for verification.

---

## 📺 LCD Display Format

Each LCD shows 2 rooms:

**LCD 1:**
