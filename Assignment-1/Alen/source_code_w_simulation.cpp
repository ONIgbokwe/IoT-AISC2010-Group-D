#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Two I2C LCDs at different addresses
Adafruit_LiquidCrystal lcd1(0x27); // Room 1 & 2
Adafruit_LiquidCrystal lcd2(0x26); // Room 3 & 4

const int NUM_ROOMS = 4;

// TMP36 analog input pins
const int tempPins[NUM_ROOMS] = {A0, A1, A2, A3};

// LEDs
const int heaterPins[NUM_ROOMS] = {6, 8, 10, 12};
const int fanPins[NUM_ROOMS] = {7, 9, 11, 13};

void setup() {
  for (int i = 0; i < NUM_ROOMS; i++) {
    pinMode(heaterPins[i], OUTPUT);
    pinMode(fanPins[i], OUTPUT);
  }

  lcd1.begin(16, 2);
  lcd2.begin(16, 2);
  lcd1.setBacklight(HIGH);
  lcd2.setBacklight(HIGH);

  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  float temperature[NUM_ROOMS];
  float humidity[NUM_ROOMS];
  bool heaterState[NUM_ROOMS];
  bool fanState[NUM_ROOMS];

  for (int i = 0; i < NUM_ROOMS; i++) {
    // Read real temperature from TMP36
    int raw = analogRead(tempPins[i]);
    temperature[i] = (raw * 5.0 / 1023.0 - 0.5) * 100; // TMP36 formula

    // Simulated humidity
    humidity[i] = random(30, 101); // 30% to 100%

    // HVAC control logic
    heaterState[i] = (temperature[i] < 20);
    if (temperature[i] > 25) heaterState[i] = false;
    digitalWrite(heaterPins[i], heaterState[i] ? HIGH : LOW);

    fanState[i] = false;
    if (temperature[i] > 30 || (temperature[i] > 25 && humidity[i] > 70) || humidity[i] > 90)
      fanState[i] = true;
    if (temperature[i] < 25 && humidity[i] < 40)
      fanState[i] = false;
    digitalWrite(fanPins[i], fanState[i] ? HIGH : LOW);

    // Serial output
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

  // LCD 1 (Room 1 & 2)
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("R1T:");
  lcd1.print((int)temperature[0]);
  lcd1.print(" H:");
  lcd1.print((int)humidity[0]);

  lcd1.setCursor(0, 1);
  lcd1.print("R2T:");
  lcd1.print((int)temperature[1]);
  lcd1.print(" H:");
  lcd1.print((int)humidity[1]);

  // LCD 2 (Room 3 & 4)
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("R3T:");
  lcd2.print((int)temperature[2]);
  lcd2.print(" H:");
  lcd2.print((int)humidity[2]);

  lcd2.setCursor(0, 1);
  lcd2.print("R4T:");
  lcd2.print((int)temperature[3]);
  lcd2.print(" H:");
  lcd2.print((int)humidity[3]);

  delay(3000); // Refresh rate
}
