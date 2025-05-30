#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pirPins[4] = {2, 3, 4, 5};
const int ledPins[4] = {6, 7, 8, 9};

const int tempPins[4] = {A0, A1, A2, A3};

const int fanLED = 10;
const int heaterLED = 11;

const int relayPin = 12;

const int trigPin = 13;
const int echoPin = A3; 

const int buzzerPin = A2;
const int alarmLED = A1;

const int smokePin = A0;
const int humidityPin = A1;

bool motionDetected = false;
unsigned long lastMotionTime;
bool armed = false;
float lastTemps[4];
unsigned long lastUpdate[4];
unsigned long faultCheckInterval = 30000;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(pirPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
    pinMode(tempPins[i], INPUT);
    lastTemps[i] = analogRead(tempPins[i]);
    lastUpdate[i] = millis();
  }

  pinMode(fanLED, OUTPUT);
  pinMode(heaterLED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(alarmLED, OUTPUT);
  pinMode(smokePin, INPUT);
  pinMode(humidityPin, INPUT);

  lastMotionTime = millis();
}

void loop() {
  handleLighting();
  handleHVAC();
  handleEnergySaver();
  handleSecurity();
  handleFireDetection();
  handleFaultMonitor();
  handleSerialCommands();
}

void handleLighting() {
  unsigned long currentTime = millis();
  int simulatedHour = (currentTime / 1000) % 24; // 1 second = 1 hour

  for (int i = 0; i < 4; i++) {
    int motion = digitalRead(pirPins[i]);
    if (motion) {
      motionDetected = true;
      lastMotionTime = millis();
    }

    if (motion) {
      if (simulatedHour >= 21 && simulatedHour < 23) {
        digitalWrite(ledPins[i], millis() % 1000 < 500 ? HIGH : LOW); // dim
      } else {
        digitalWrite(ledPins[i], HIGH); 
      }
    } else {
      if (simulatedHour >= 23) {
        digitalWrite(ledPins[i], LOW);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}

void handleHVAC() {
  float totalTemp = 0;
  for (int i = 0; i < 4; i++) {
    int reading = analogRead(tempPins[i]);
    float voltage = reading * 5.0 / 1023.0;
    float tempC = (voltage - 0.5) * 100;
    totalTemp += tempC;
  }

  float avgTemp = totalTemp / 4.0;
  int humidityRaw = analogRead(humidityPin);
  float humidity = map(humidityRaw, 0, 1023, 20, 90);

  lcd.setCursor(0, 0);
  lcd.print("Temp:"); lcd.print(avgTemp, 1);
  lcd.print("C ");
  lcd.print("H:"); lcd.print(humidity, 0);
  lcd.print("%   ");

  if (avgTemp < 20 || humidity < 40) {
    digitalWrite(heaterLED, HIGH);
    digitalWrite(fanLED, LOW);
  } else if (avgTemp > 30 || humidity > 70) {
    digitalWrite(fanLED, HIGH);
    digitalWrite(heaterLED, LOW);
  } else {
    digitalWrite(fanLED, LOW);
    digitalWrite(heaterLED, LOW);
  }
}

void handleEnergySaver() {
  if (millis() - lastMotionTime > 600000) { 
    digitalWrite(relayPin, LOW);
  } else {
    digitalWrite(relayPin, HIGH);
    motionDetected = false;
  }
} 

void handleSecurity() {
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  if (armed && distance < 20) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(alarmLED, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(alarmLED, LOW);
  }
}

void handleFireDetection() {
  float temp = analogRead(tempPins[2]) * 5.0 / 1023.0;
  temp = (temp - 0.5) * 100;
  int smoke = analogRead(smokePin);
  if (temp > 50 && smoke > 600) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(alarmLED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("FIRE ALERT!!!   ");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
}

void handleFaultMonitor() {
  for (int i = 0; i < 4; i++) {
    float current = analogRead(tempPins[i]);
    if (abs(current - lastTemps[i]) > 5) {
      lastUpdate[i] = millis();
      lastTemps[i] = current;
    } else if (millis() - lastUpdate[i] > faultCheckInterval) {
      Serial.print("Sensor "); Serial.print(i); Serial.println(" not responding!");
      digitalWrite(alarmLED, HIGH);
    }
  }
}

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "ARM") {
      armed = true;
      Serial.println("System Armed");
    } else if (cmd == "DISARM") {
      armed = false;
      Serial.println("System Disarmed");
    } else if (cmd == "MODE HOME") {
      armed = false;
      Serial.println("Mode: Home");
    } else if (cmd == "MODE AWAY") {
      armed = true;
      Serial.println("Mode: Away");
    }
  }
}