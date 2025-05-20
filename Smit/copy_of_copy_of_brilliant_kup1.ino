#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 7, 6, 3, 2);

// Pin definitions
const int gasSensorPin = 5;       // Digital gas sensor output (HIGH means gas detected)
const int pirSensorPin = 4;       // PIR motion sensor output (HIGH means motion detected)
const int greenLEDPin = 8;        // Green LED pin for safe
const int redLEDPin = 9;          // Red LED pin for fire alert
const int buzzerPin = 10;         // Buzzer pin
const int tempSensorPin = A0;     // TMP36 analog input for temperature
const int ldrPin = A1;            // LDR analog input for light level

// Threshold values — Adjust these after testing your hardware
const int LDR_THRESHOLD = 300;        // LDR value below this means low light (possible smoke)
const float TEMP_THRESHOLD = 50.0;    // Temperature threshold in Celsius for fire

void setup() {
  // Setup pins
  pinMode(gasSensorPin, INPUT);
  pinMode(pirSensorPin, INPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize Serial for debugging
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read sensors
  int gasDetected = digitalRead(gasSensorPin);      // HIGH if gas detected
  int motionDetected = digitalRead(pirSensorPin);   // HIGH if motion detected
  int ldrValue = analogRead(ldrPin);                 // Light level (0-1023)
  int tempRaw = analogRead(tempSensorPin);           // Raw TMP36 analog value

  // Convert TMP36 raw analog to Celsius
  float voltage = tempRaw * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100.0;

  // Debug print for calibration and testing
  Serial.print("Gas: ");
  Serial.print(gasDetected);
  Serial.print(" | Motion: ");
  Serial.print(motionDetected);
  Serial.print(" | LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Fire detection logic:
  // Fire suspected if any condition:
  // 1. Gas sensor detects gas (smoke)
  // 2. Temperature above threshold AND low light detected (smoke blocks light)
  // 3. Motion detected (optional: may indicate emergency presence)
  bool fireDetected = (gasDetected == HIGH) ||
                      (temperature > TEMP_THRESHOLD && ldrValue < LDR_THRESHOLD) ||
                      (motionDetected == HIGH);

  if (fireDetected) {
    // Fire alert: turn red LED and buzzer ON, green LED OFF
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    tone(buzzerPin, 1000);  // 1 kHz tone for buzzer

    // Display alert on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! FIRE ALERT !!");
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print((int)temperature);
    lcd.print("C LDR:");
    lcd.print(ldrValue);
  } else {
    // Safe state: green LED ON, red LED and buzzer OFF
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
    noTone(buzzerPin);

    // Display safe status on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAFE - Temp:");
    lcd.print((int)temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("LDR:");
    lcd.print(ldrValue);
  }

  delay(1000); // 1-second delay between readings
}
