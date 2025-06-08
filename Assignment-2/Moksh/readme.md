
## 📊 Tasks Performed

* Computed relative X/Y coordinates from GPS data (used for movement plotting).
* Generated the following visualizations:

  1. **2D Movement Path** with orientation arrows (Gyroscope X/Y).
  2. **3D Orientation Path** using Gyroscope X/Y/Z axes.
  3. **Interactive Folium Map** showing the GPS route.
  4. **Saved all visualizations** to: `your_path/visualizations`.

---

## 🔍 Key Findings

### 1. Orientation Vectors Over Time — *Gyro X vs Y*

* **X-axis**: Time index
* **Y-axis**: Fixed at 0 (for display clarity)
* **Arrows**: Represent Gyroscope X and Y values at each timestamp

**Observation**:

* Arrows appear very small or almost invisible, indicating **minimal rotation**.
* Gyroscope readings for X and Y are extremely close to zero, suggesting the device remained **mostly stationary**, causing the vector plot to collapse into near-zero directions (dots or flat line).

---

### 2. Acceleration vs. Speed Overlay — *`acceleration_vs_speed.png`*

* **Green line**: Acceleration magnitude
* **Blue line**: Speed derived from GPS
* **Twin Y-axes**: Left = Acceleration (m/s²), Right = Speed (m/s)

**Observation**:

* Acceleration remains close to **9.81 m/s²**, implying:

  * The object (e.g., phone) was likely **stationary**.
  * Sensor is primarily capturing **gravitational acceleration**.
* Minor fluctuations (\~±0.01 m/s²) could be due to:

  * Sensor noise
  * Minor vibrations or environmental tremors

---

### 3. 3D Orientation of the Phone — *`gyro_3d_orientation.png`*

* **Axes**: Gyroscope X, Y, Z
* **Purpose**: Visualize phone’s orientation drift in 3D space

**Observation**:

* Values are small (\~0.001–0.003), but plot shows **slight jittery motion**, consistent with a mostly stationary phone that may have experienced **small rotations or tilts**.

---

### 4. Interactive GPS Route Map — *`route_map.html`*

* Built using **Folium** to visualize the GPS path taken.
* Route constructed using recorded **latitude and longitude** data.
* Interactive HTML map file for dynamic viewing of movement trace.
