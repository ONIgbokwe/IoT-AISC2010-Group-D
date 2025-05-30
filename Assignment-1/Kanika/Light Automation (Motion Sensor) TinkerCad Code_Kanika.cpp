// Pin configuration for 4 rooms
const int pirPins[4] = {2, 3, 4, 5};     // PIR motion sensors
const int ldrPins[4] = {A0, A1, A2, A3}; // LDR analog inputs
const int ledPins[4] = {6, 7, 8, 9};     // LEDs for each room

// To track when motion was last detected
unsigned long lastMotionTime[4] = {0, 0, 0, 0};

// Light threshold to determine low vs. high ambient light
int lightThreshold = 500;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(pirPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  unsigned long currentTime = millis();

  for (int i = 0; i < 4; i++) {
    int lightValue = analogRead(ldrPins[i]);      // Read LDR
    bool motion = digitalRead(pirPins[i]);        // Read motion sensor

    // Always update last motion time if motion is detected
    if (motion) {
      lastMotionTime[i] = currentTime;
    }

    // Default LED value (OFF)
    int ledValue = 0;

    // Check if room is dark and there was motion in last 5 min
    if (lightValue < lightThreshold && (currentTime - lastMotionTime[i]) <= 10000UL) 
    //if (lightValue < lightThreshold && (currentTime - lastMotionTime[i]) <= 300000UL)
    {
      ledValue = 255; // Full brightness
    }
    // If no motion for more than 5 min but less than 15, dim LED
    else if ((currentTime - lastMotionTime[i]) > 10000UL && (currentTime - lastMotionTime[i]) <= 20000UL)
    //else if ((currentTime - lastMotionTime[i]) > 300000UL && (currentTime - lastMotionTime[i]) <= 900000UL)
    {
      ledValue = 150; // Dim
    }
    // Else (either light is bright or no motion for >15 min), turn off LED
    else {
      ledValue = 0;
    }

    analogWrite(ledPins[i], ledValue);

    // Debugging
    Serial.print("Room ");
    Serial.print(i + 1);
    Serial.print(" | Light: ");
    Serial.print(lightValue);
    Serial.print(" | Motion: ");
    Serial.print(motion);
    Serial.print(" | LED: ");
    Serial.println(ledValue);
  }

  delay(1000); // Update every second
}
