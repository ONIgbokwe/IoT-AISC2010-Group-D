const int ledPins[4] = {6,9,10,11};
 
char mode = 'X';

int simulatedHour = -1;
 
void setup() {

  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {

    pinMode(ledPins[i], OUTPUT);

    analogWrite(ledPins[i], 255); // Initial state: ON

  }
 
  Serial.println("Welcome to Smart Lighting Automation");

  Serial.println("Testing 1d and 1e (Time based)");

  Serial.println("Enter simulated hour (0 to 23):");

}
 
void loop() {

  if (Serial.available() > 0) {

    simulatedHour = Serial.parseInt();

    while (Serial.available() > 0) Serial.read(); // clear buffer
 
    Serial.print("Simulated Hour Set To: ");

    Serial.println(simulatedHour);
 
    for (int i = 0; i < 4; i++) {

      if (simulatedHour >= 23) {

        digitalWrite(ledPins[i], LOW);

        Serial.print("Type 2: Room "); Serial.print(i + 1); Serial.println(" : OFF (Q1.e After 11PM)");

      } 

      else if (simulatedHour >= 21) {

        analogWrite(ledPins[i], 90); // Dim light

        Serial.print("Type 2: Room "); Serial.print(i + 1); Serial.println(" : DIM (Q1.d After 9PM)");

      } 

      else {

        analogWrite(ledPins[i], 255); // Full brightness

        Serial.print("Type 2: Room "); Serial.print(i + 1); Serial.println(": ON (Before 9 PM)");

      }

    }
 
    Serial.println("Put another time to check other scenarios");

  }
 
  delay(500); // Small delay for smooth serial output

}
 