 #define BLYNK_TEMPLATE_ID "xxxxxxx"
#define BLYNK_TEMPLATE_NAME "xxxxx"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <AccelStepper.h>


//#define BLYNK_AUTH_TOKEN "_x-t9GG48karKxI_NzrH8QVVUwJOu4c0"
// WiFi credentials
char auth[] = "xxxx";
char ssid[] = "xxxx";
char pass[] = "xxxx";

// Blink authentication token


// Motor connections
#define STEP_PIN  12
#define DIR_PIN   14
#define ENABLE_PIN 15
#define SLEEP_PIN 32
// Define the number of steps per revolution
#define STEPS_PER_REVOLUTION 96

// Initialize the stepper motor object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

bool motorRunning = false;

void setup() {
  // Set up the serial monitor
  Serial.begin(9600);

  // Set up the motor pins
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode( 2, OUTPUT);
  pinMode( SLEEP_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Enable the motor driver

  // Set up WiFi connection
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite( 2, HIGH);
    delay(400);
    Serial.println("Connecting to WiFi...");
    digitalWrite( 2, LOW);
    delay(200);
  }
  Serial.println("Connected to WiFi");
  digitalWrite( 2, HIGH);
  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Set motor speed and acceleration
  stepper.setMaxSpeed(1000);      // Set the maximum speed in steps per second
  stepper.setAcceleration(500);   // Set the acceleration in steps per second^2
  stepper.setPinsInverted(false, false, true);
}

void loop() {
  Blynk.run();
  stepper.runSpeed();
  if (motorRunning && stepper.distanceToGo() == 0) {
    motorRunning = false;
    Serial.println("Motor stopped");
    digitalWrite(SLEEP_PIN, LOW);
  }
}

// Blynk virtual pin handler
BLYNK_WRITE(V0) {
  int motorControl = param.asInt();
  if (motorControl == 1 && !motorRunning) {
    digitalWrite(SLEEP_PIN, HIGH);
    delay(100);
    stepper.move(STEPS_PER_REVOLUTION); // Move one revolution
    stepper.runToPosition();
    motorRunning = true;
    Serial.println("Motor started");
  }
}