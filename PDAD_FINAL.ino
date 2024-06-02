#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if necessary

// Define pins for ultrasonic sensor
const int trigPin = 8;
const int echoPin = 10;

// Define distance threshold for detecting a vehicle
const int distanceThreshold = 20;  // Adjust based on your setup

// Variables for timing
unsigned long startTime = 0;
unsigned long endTime = 0;
bool vehiclePresent = false;

void setup() {
  // Initialize LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Parking System");

  // Set up pins for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initial message
  lcd.setCursor(0, 1);
  lcd.print("Parking Available");
}

void loop() {
  long duration, distance;

  // Send a 10us pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distance = (duration / 2) / 29.1;

  if (distance < distanceThreshold) {
    if (!vehiclePresent) {
      vehiclePresent = true;
      startTime = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Occupied");
      lcd.setCursor(0, 1);
      lcd.print("Timer Started");
    }
  } else {
    if (vehiclePresent) {
      vehiclePresent = false;
      endTime = millis();
      unsigned long parkedTime = (endTime - startTime) / 1000;  // Convert to seconds
      float cost = (parkedTime * 2);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Exited    Rs");
      lcd.print(cost);
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(parkedTime);
      lcd.print(" sec");
      
      delay(3000);
      
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Parking Available");
      lcd.setCursor(0, 1);
      lcd.print("               ");  // Clear second line
    }
  }
  delay(500);  // Check every 500ms
}
