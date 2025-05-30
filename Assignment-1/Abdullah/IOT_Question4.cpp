#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define ULTRASONIC_SIG_PIN 11 // Combined trigger/echo pin
#define BUZZER_PIN 10
#define DOOR_LOCK_PIN 13
#define LED_UNLOCKED_PIN A0 // Green LED
#define LED_LOCKED_PIN A1   // Blue LED
#define INTRUDER_LED_PIN 12   // Red LED

// Keypad Configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD I2C Setup
LiquidCrystal_I2C lcd(0x20, 16, 2); // Adjust I2C address if needed

// Security Variables
String correctPIN = "1234";
String enteredPIN = "";
bool systemArmed = false;
bool doorLocked = false;

// Distance Sensor Variables
const int ALARM_DISTANCE = 30; // Distance in cm to trigger alarm
unsigned long lastDistanceCheck = 0;
const unsigned long DISTANCE_CHECK_INTERVAL = 200; // ms between checks

// Display Variables
unsigned long messageDisplayTime = 0;
const unsigned long MESSAGE_TIMEOUT = 3000; // 3 seconds

void setup() {
  pinMode(ULTRASONIC_SIG_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DOOR_LOCK_PIN, OUTPUT);
  pinMode(LED_UNLOCKED_PIN, OUTPUT);
  pinMode(LED_LOCKED_PIN, OUTPUT);
  
  // Initialize door state
  digitalWrite(DOOR_LOCK_PIN, HIGH); // Lock door initially
  digitalWrite(LED_LOCKED_PIN, HIGH); // Red LED on
  digitalWrite(LED_UNLOCKED_PIN, LOW); // Green LED off
  doorLocked = true;
  
  lcd.init();
  lcd.backlight();
  displayMessage("System Ready");
  delay(1500);
  displayMessage("Enter PIN:");

  Serial.begin(9600);
  Serial.println("System Ready");
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    handleKeyPress(key);
  }

  // Return to PIN entry after message timeout
  if(millis() - messageDisplayTime > MESSAGE_TIMEOUT && enteredPIN.length() == 0) {
    displayMessage("Enter PIN:");
  }

  // If system is armed, check distance periodically
  if (systemArmed && millis() - lastDistanceCheck >= DISTANCE_CHECK_INTERVAL) {
    lastDistanceCheck = millis();
    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    if (distance > 0 && distance < ALARM_DISTANCE) {
      displayMessage("INTRUDER ALERT!");
      triggerAlarm();
    }
  }
}

void handleKeyPress(char key) {
  Serial.println(key);
  
  if (key == '#') { // Confirm entry
    Serial.println("Entered PIN: " + enteredPIN);
    if (enteredPIN == correctPIN) {
      systemArmed = !systemArmed;
      
      if(systemArmed) {
        // When arming, automatically lock door
        setDoorLock(true);
        displayMessage("System Armed");
        Serial.println("System Armed - Door Locked");
      } else {
        displayMessage("System Unarmed");
        Serial.println("System Unarmed");
      }
      delay(1500);
      displayMessage("Enter PIN:");
    } else {
      displayMessage("Wrong PIN");
      Serial.println("Invalid PIN attempt");
      delay(1500);
      displayMessage("Enter PIN:");
    }
    enteredPIN = "";
  } 
  else if (key == '*') { // Clear entry
    enteredPIN = "";
    displayMessage("Enter PIN:");
  } 
  else if (key == 'A' && !systemArmed) { // Toggle door lock (only when unarmed)
    setDoorLock(!doorLocked);
    displayMessage(doorLocked ? "Door Locked" : "Door Unlocked");
    delay(1500);
    displayMessage("Enter PIN:");
  }
  else {
    enteredPIN += key;
    lcd.clear();
    lcd.print("Enter PIN:");
    lcd.setCursor(0, 1);
    lcd.print(enteredPIN);
  }
}

float getDistance() {
  // Set pin to output for trigger pulse
  pinMode(ULTRASONIC_SIG_PIN, OUTPUT);
  
  // Send trigger pulse
  digitalWrite(ULTRASONIC_SIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_SIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_SIG_PIN, LOW);
  
  // Switch pin to input to read echo
  pinMode(ULTRASONIC_SIG_PIN, INPUT);
  
  // Measure pulse duration
  long duration = pulseIn(ULTRASONIC_SIG_PIN, HIGH);
  
  // Calculate distance in cm
  float distance = duration * 0.0343 / 2;
  
  return distance;
}

void triggerAlarm() {
  digitalWrite(INTRUDER_LED_PIN,HIGH);
  tone(BUZZER_PIN, 1000);
  delay(2000); // Alarm duration
  noTone(BUZZER_PIN);
  digitalWrite(INTRUDER_LED_PIN, LOW);
  displayMessage("Enter PIN:");
}

void setDoorLock(bool locked) {
  doorLocked = locked;
  digitalWrite(DOOR_LOCK_PIN, locked ? HIGH : LOW);
  digitalWrite(LED_LOCKED_PIN, locked ? HIGH : LOW);
  digitalWrite(LED_UNLOCKED_PIN, locked ? LOW : HIGH);
  
  Serial.println(locked ? "Door Locked" : "Door Unlocked");
}

void displayMessage(String message) {
  lcd.clear();
  lcd.print(message);
  messageDisplayTime = millis();
}