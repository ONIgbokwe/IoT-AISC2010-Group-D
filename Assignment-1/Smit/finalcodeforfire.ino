#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 7, 6, 3, 2);

// Pin definitions
const int gasSensorPin = 5;       // Digital gas sensor output (HIGH = gas detected)
const int pirSensorPin = 4;       // PIR motion sensor output (HIGH = motion detected)
const int greenLEDPin = 8;        // Green LED pin (safe)
const int redLEDPin = 9;          // Red LED pin (fire alert)
const int buzzerPin = 10;         // Buzzer pin
const int tempSensorPin = A0;     // TMP36 analog input for temperature
const int ldrPin = A1;            // LDR analog input for light level

// Threshold values
const int LDR_THRESHOLD = 300;        // Low light threshold
const float TEMP_THRESHOLD = 50.0;    // High temperature threshold in Celsius

void setup() {
  pinMode(gasSensorPin, INPUT);
  pinMode(pirSensorPin, INPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Sensor readings
  int gasDetected = digitalRead(gasSensorPin);
  int motionDetected = digitalRead(pirSensorPin);
  int ldrValue = analogRead(ldrPin);
  int tempRaw = analogRead(tempSensorPin);

  // Convert TMP36 to Celsius
  float voltage = tempRaw * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100.0;

  // Debug output
  Serial.print("Gas: ");
  Serial.print(gasDetected);
  Serial.print(" | Motion: ");
  Serial.print(motionDetected);
  Serial.print(" | LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Fire detection logic (any of the following):
  bool fireDetectedByGas = (gasDetected == HIGH);
  bool fireDetectedByHeatAndDark = (temperature > TEMP_THRESHOLD && ldrValue < LDR_THRESHOLD);
  bool fireDetected = fireDetectedByGas || fireDetectedByHeatAndDark;

  if (fireDetected) {
    // FIRE ALERT
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    tone(buzzerPin, 1000);  // Buzzer ON

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! FIRE ALERT !!");
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print((int)temperature);
    lcd.print("C L:");
    lcd.print(ldrValue);
  } else if (motionDetected == HIGH) {
    // Only motion detected
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
    noTone(buzzerPin);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motion Detected");
    lcd.setCursor(0, 1);
    lcd.print("Monitoring...");
  } else {
    // Safe condition
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
    noTone(buzzerPin);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAFE - Temp:");
    lcd.print((int)temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("LDR:");
    lcd.print(ldrValue);
  }

  delay(1000); // 1-second interval
}
