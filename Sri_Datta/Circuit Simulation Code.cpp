#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0); // I2C LCD

// Pins for LEDs
const int heaterPins[2] = {6, 8};
const int fanPins[2] = {7, 9};

void setup() {
  for (int i = 0; i < 2; i++) {
    pinMode(heaterPins[i], OUTPUT);
    pinMode(fanPins[i], OUTPUT);
  }

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  Serial.begin(9600);

  randomSeed(analogRead(0)); // Seed randomness
}

void loop() {
  float temperature[2];
  float humidity[2];
  bool heaterState[2];
  bool fanState[2];

  for (int i = 0; i < 2; i++) {
    // --- Simulate values ---
    temperature[i] = random(150, 351) / 10.0; // 15.0–35.0 °C
    humidity[i] = random(0, 101);             // 0–100 %

    // --- Heater Logic ---
    heaterState[i] = false;
    if (temperature[i] < 20) heaterState[i] = true;
    if (temperature[i] > 25) heaterState[i] = false;
    digitalWrite(heaterPins[i], heaterState[i] ? HIGH : LOW);

    // --- Fan Logic ---
    fanState[i] = false;
    if (temperature[i] > 30) fanState[i] = true;
    if (temperature[i] > 25 && humidity[i] > 70) fanState[i] = true;
    if (humidity[i] > 90) fanState[i] = true;
    if (temperature[i] < 25 && humidity[i] < 40) fanState[i] = false;
    digitalWrite(fanPins[i], fanState[i] ? HIGH : LOW);

    // --- Serial Output ---
    Serial.print("Room ");
    Serial.print(i + 1);
    Serial.print(" | Temp: ");
    Serial.print(temperature[i]);
    Serial.print(" C | Hum: ");
    Serial.print(humidity[i]);
    Serial.print(" % | Heater: ");
    Serial.print(heaterState[i] ? "ON" : "OFF");
    Serial.print(" | Fan: ");
    Serial.println(fanState[i] ? "ON" : "OFF");
  }

  // --- LCD Display for Both Rooms ---
  lcd.setCursor(0, 0);
  lcd.print("RM1 T:");
  lcd.print((int)temperature[0]);
  lcd.print(" H:");
  lcd.print((int)humidity[0]);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("RM2 T:");
  lcd.print((int)temperature[1]);
  lcd.print(" H:");
  lcd.print((int)humidity[1]);
  lcd.print("%   ");

  delay(5000); // Refresh every 5 second
}
