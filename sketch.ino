#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int pirPin = A0;
const int trigPin = A1;
const int echoPin = A2;
const int redLED = 13;
const int yellowLED = 12;    
const int greenLED = 11;     

void setup() {
  Serial.begin(9600);
  
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);  
  pinMode(greenLED, OUTPUT);   

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("OLED failed!"));
    while(1);
  }
  display.clearDisplay();
  display.display();
  delay(1000);
}

void loop() {
  // Read PIR status
  int pirStatus = digitalRead(pirPin);
  String pirText = (pirStatus == HIGH) ? "Detected" : "Not Detected";

  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  String condition;
  // Turn off all LEDs
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);   
  digitalWrite(greenLED, LOW);    

  // Determine condition based on distance
  if (distance < 50) {
    condition = "Near";
    digitalWrite(redLED, HIGH);
  } 
  else if (distance >= 50 && distance <= 125) {
    condition = "Medium";
    digitalWrite(yellowLED, HIGH);
  } 
  else {
    condition = "Far";
    digitalWrite(greenLED, HIGH);  
  }

  // Display data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.print("PIR: ");
  display.println(pirText);
  
  display.setCursor(0, 20);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");
  
  display.setCursor(0, 40);
  display.print("Status: ");
  display.println(condition); // Corrected typo: condtion -> condition
  
  display.display();

  delay(500);
}