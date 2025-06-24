#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pins for Ultrasonic Sensor
#define TRIG_PIN 9
#define ECHO_PIN 10

// Pins for LED, Buzzer, Button
#define RED_LED 7
#define GREEN_LED 6
#define BUZZER 8
#define BUTTON 2

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool systemActive = false;

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP); // Use internal pull-up resistor

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (true); // Stay here if display fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Drone Obstacle Detector");
  display.println("Press Button to Start");
  display.display();
}

void loop() {
  // Toggle system ON/OFF with button press
  if (digitalRead(BUTTON) == LOW) {
    systemActive = !systemActive;
    delay(300); // Debounce delay
  }

  if (!systemActive) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, LOW);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("System OFF");
    display.println("Press Button to Start");
    display.display();

    delay(500);
    return;
  }

  // Measure Distance
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Print to Serial
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Display on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");

  // Obstacle logic
  if (distance < 20.0) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    display.println("⚠️ Obstacle Ahead!");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    display.println("✅ Path Clear");
  }

  display.display();
  delay(500);
}
