import pandas as pd
import numpy as np
import json
from math import radians, sin, cos, sqrt, atan2
import matplotlib.pyplot as plt
import folium

# === Step 1: Load JSON file ===
df = pd.read_json(r"C:\Users\moksh\OneDrive\Documents\OneDrive\Desktop\LCIT\sensor_data 1.json")
df['timestamp'] = pd.to_datetime(df['timestamp'])

# === Step 2: Compute acceleration magnitude ===
df['acc_mag'] = np.sqrt(df['accX']**2 + df['accY']**2 + df['accZ']**2)

# === Step 3: Compute jerk ===
df['jerk'] = df['acc_mag'].diff() / df['timestamp'].diff().dt.total_seconds()

# === Step 4: Compute GPS distance between points ===
def haversine(lat1, lon1, lat2, lon2):
    R = 6371000  # Earth's radius in meters
    phi1, phi2 = radians(lat1), radians(lat2)
    d_phi = radians(lat2 - lat1)
    d_lambda = radians(lon2 - lon1)
    a = sin(d_phi/2)**2 + cos(phi1)*cos(phi2)*sin(d_lambda/2)**2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))
    return R * c

distances = [0]
for i in range(1, len(df)):
    dist = haversine(df.loc[i-1, 'gpsLat'], df.loc[i-1, 'gpsLon'],
                     df.loc[i, 'gpsLat'], df.loc[i, 'gpsLon'])
    distances.append(dist)

df['distance'] = distances
df['total_distance'] = df['distance'].cumsum()

# === Step 5: Speed ===
df['delta_time'] = df['timestamp'].diff().dt.total_seconds()
df['speed'] = df['distance'] / df['delta_time']

# === Step 6: Direction inference ===
def infer_direction(row):
    accX, accY, accZ = row['accX'], row['accY'], row['accZ']
    max_axis = max(abs(accX), abs(accY), abs(accZ))
    if max_axis == abs(accX):
        return "Right" if accX > 0 else "Left"
    elif max_axis == abs(accY):
        return "Forward" if accY > 0 else "Backward"
    else:
        return "Up" if accZ > 0 else "Down"

df['direction'] = df.apply(infer_direction, axis=1)

# === Step 7: Enhanced Event Detection ===
def detect_event(row):
    if pd.isna(row['jerk']):
        return "Normal"
    
    # Enhanced event detection with multiple types
    direction = row['direction']
    
    if row['acc_mag'] < 1.5:
        return f"Hard Fall - {direction}"
    elif 1.5 <= row['acc_mag'] < 2:
        return f"Soft Fall - {direction}"
    elif row['acc_mag'] > 20 and abs(row['jerk']) > 10:
        return f"Severe Crash - {direction}"
    elif row['acc_mag'] > 15 and abs(row['jerk']) > 7:
        return f"Moderate Crash - {direction}"
    elif abs(row['jerk']) > 8:
        return f"Hard Push - {direction}"
    elif abs(row['jerk']) > 5:
        return f"Soft Push - {direction}"
    elif row['accZ'] > 7:
        return f"Lift Up - {direction}"
    elif row['accZ'] < -7:
        return f"Put Down - {direction}"
    return "Normal"

df['event'] = df.apply(detect_event, axis=1)

# === Step 8: Save processed data ===
df_cleaned = df.dropna().reset_index(drop=True)
df_cleaned['timestamp'] = df_cleaned['timestamp'].astype(str)
processed_data = df_cleaned.to_dict(orient='records')

with open('processed_data.json', 'w') as f:
    json.dump(processed_data, f, indent=2)

# === GPS to XY conversion ===
R = 6371000  # Earth's radius in meters
origin_lat = df['gpsLat'].iloc[0]  # First point as origin
origin_lon = df['gpsLon'].iloc[0]

df['rel_x'] = df['gpsLon'].apply(lambda lon: radians(lon - origin_lon) * R * cos(radians(origin_lat)))
df['rel_y'] = df['gpsLat'].apply(lambda lat: radians(lat - origin_lat) * R)

# === Step 9: Enhanced Plotting ===
plt.style.use('ggplot')  # Using available ggplot style
plt.rcParams.update({
    'figure.facecolor': 'white',
    'axes.grid': True,
    'grid.alpha': 0.3,
    'axes.titlesize': 14,
    'axes.labelsize': 12,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10
})

fig, axs = plt.subplots(7, 1, figsize=(16, 40))

# 1. Acceleration Plot
axs[0].plot(df['timestamp'], df['acc_mag'], color='blue', linewidth=1.5)
axs[0].set_ylabel('Acceleration (m/s²)')
axs[0].set_title('Acceleration Magnitude Over Time', pad=20)

# 2. Jerk Plot
axs[1].plot(df['timestamp'], df['jerk'], color='green', linewidth=1.5)
axs[1].axhline(0, color='gray', linestyle='--', alpha=0.5)
axs[1].set_ylabel('Jerk (m/s³)')
axs[1].set_title('Jerk Over Time', pad=20)

# 3. Speed Plot
axs[2].plot(df['timestamp'], df['speed'], color='red', linewidth=1.5)
axs[2].axhline(0, color='gray', linestyle='--', alpha=0.5)
axs[2].set_ylabel('Speed (m/s)')
axs[2].set_title('Speed from GPS Data', pad=20)

# 4. Enhanced Events Plot
unique_events = sorted(df['event'].unique())
colors = plt.cm.tab20(np.linspace(0, 1, len(unique_events)))
event_colors = {event: color for event, color in zip(unique_events, colors)}

# Create event levels for vertical spacing
event_levels = {event: i for i, event in enumerate(unique_events)}
df['event_level'] = df['event'].map(event_levels)

# Plot events at different levels
for event in unique_events:
    event_data = df[df['event'] == event]
    axs[3].scatter(event_data['timestamp'], event_data['event_level'], 
                  color=event_colors[event], label=event, s=100, alpha=0.8)

# Configure event plot
axs[3].set_yticks(list(event_levels.values()))
axs[3].set_yticklabels(list(event_levels.keys()))
axs[3].set_title('Detected Events with Direction and Type', pad=20)
axs[3].grid(True, axis='y', alpha=0.3)

# Add legend
axs[3].legend(bbox_to_anchor=(1.05, 1), loc='upper left')

# 5. XY Movement Plot
axs[4].plot(df['rel_x'], df['rel_y'], marker='o', linestyle='-', 
           color='purple', markersize=5, linewidth=1.5, alpha=0.8)
axs[4].scatter(0, 0, color='red', s=100, label='Origin')
axs[4].set_xlabel('Relative X (meters)')
axs[4].set_ylabel('Relative Y (meters)')
axs[4].set_title('Device Movement Path (Relative to Origin)', pad=20)
axs[4].legend()

# Set equal aspect ratio with padding
x_range = df['rel_x'].max() - df['rel_x'].min()
y_range = df['rel_y'].max() - df['rel_y'].min()
max_range = max(x_range, y_range) * 1.2

x_center = (df['rel_x'].max() + df['rel_x'].min()) / 2
y_center = (df['rel_y'].max() + df['rel_y'].min()) / 2

axs[4].set_xlim(x_center - max_range/2, x_center + max_range/2)
axs[4].set_ylim(y_center - max_range/2, y_center + max_range/2)
axs[4].set_aspect('equal', adjustable='box')

# Orientation Arrows Over Time
# Show how the phone’s orientation (tilt/rotation) evolves.

axs[5].quiver(range(len(df)), [0]*len(df), df['gyroX'], df['gyroY'], angles='xy', scale_units='xy', scale=1)
axs[5].set_title('Orientation Vectors Over Time (Gyro X vs Y)')
axs[5].set_xlabel('Time Index')
axs[5].set_ylabel('Orientation')
axs[5].grid(True)

# Acceleration vs Speed Overlay (Merged into one subplot with twin axes) ===

fig2, ax1 = plt.subplots(figsize=(12, 6))
ax2 = ax1.twinx()

ax1.plot(df['timestamp'], df['acc_mag'], 'g-', label='Acceleration (m/s²)')
ax2.plot(df['timestamp'], df['speed'], 'b-', label='Speed (m/s)')

ax1.set_xlabel('Time')
ax1.set_ylabel('Acceleration', color='g')
ax2.set_ylabel('Speed', color='b')
fig2.suptitle('Acceleration vs Speed Over Time')

# === Save both figures ===
plt.tight_layout(pad=4.0, h_pad=3.0)
plt.subplots_adjust(right=0.85)
fig.savefig("iot_combined_plots.png")   # main subplot figure
fig2.savefig("acceleration_vs_speed.png")  # twin axis plot

# === 3D Orientation Plot ===
fig3 = plt.figure(figsize=(10, 6))
ax3 = fig3.add_subplot(111, projection='3d')
ax3.plot(df['gyroX'], df['gyroY'], df['gyroZ'], color='darkblue', linewidth=2)
ax3.set_title("3D Orientation of the Phone (Gyroscope)")
ax3.set_xlabel("Gyro X")
ax3.set_ylabel("Gyro Y")
ax3.set_zlabel("Gyro Z")
fig3.savefig("gyro_3d_orientation.png")

# === Folium Route Map ===
m = folium.Map(location=[df['gpsLat'].mean(), df['gpsLon'].mean()], zoom_start=15)
points = list(zip(df['gpsLat'], df['gpsLon']))
folium.PolyLine(points, color="blue", weight=2.5).add_to(m)
m.save('C:/Users/moksh/OneDrive/Documents/OneDrive/Desktop/LCIT/route_map.html')


# --- Save all matplotlib figures individually ---
from pathlib import Path

# Set save directory
save_dir = Path("C:/Users/moksh/OneDrive/Documents/OneDrive/Desktop/LCIT/visualizations")
save_dir.mkdir(parents=True, exist_ok=True)

# Save individual plots
figures = [plt.figure(n) for n in plt.get_fignums()]
for i, fig in enumerate(figures):
    fig_path = save_dir / f"visualization_{i+1}.png"
    fig.savefig(fig_path, bbox_inches='tight', dpi=300)

# Optional: Clear after saving if running in notebook or script multiple times
plt.close('all')

print(f"Saved {len(figures)} visualizations to {save_dir}")

with open('processed_data.json', 'w') as f:
    json.dump(processed_data, f, indent=2)
