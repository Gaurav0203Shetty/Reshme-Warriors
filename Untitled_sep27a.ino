#include "arduino_secrets.h"
#include <Adafruit_LiquidCrystal.h>
#include "thingProperties.h"

// Constants for ESP32 GPIO pins
const int LED1 = 5;              // LED1 pin (can be changed as needed)
const int LED2 = 4;              // LED2 pin (can be changed as needed)
const int MQ2pin = 35;           // MQ2 sensor on GPIO35 (ADC1)
const int analogIn = 34;         // Temperature sensor on GPIO34 (ADC1)
const int humidityPin = 32;      // Humidity sensor on GPIO32 (ADC1)
const int lcdColumns = 16;       // LCD columns
const int lcdRows = 2;           // LCD rows
const int IN1 = 18;              // Fan motor pin IN1
const int IN2 = 19;              // Fan motor pin IN2
const int EN3 = 21;              // Fan motor enable pin
const int LDR = 33;              // LDR on GPIO33 (ADC1)

// Variables
int rawValue = 0;
double voltage = 0;
double tempC = 0;
int humiditySensorOutput = 0;
int seconds = 0;

// LCD instance (I2C or Parallel Interface as per your setup)
Adafruit_LiquidCrystal lcd_1(0);

void setup() {  
  Serial.begin(115200);          // Start serial communication
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  pinMode(humidityPin, INPUT);   // Set humidity pin as input
  lcd_1.begin(lcdColumns, lcdRows); // Initialize the LCD
  pinMode(IN1, OUTPUT);          // Set fan motor control pins as output
  pinMode(IN2, OUTPUT);
  pinMode(EN3, OUTPUT);
  pinMode(LED1, OUTPUT);         // Set LED1 as output
  pinMode(MQ2pin, INPUT);        // Set MQ2 pin as input
  pinMode(LED2, OUTPUT);         // Set LED2 as output
  pinMode(LDR, INPUT);           // Set LDR pin as input
}

void loop() {
  // Read temperature sensor (ESP32 ADC reads 0 to 4095, 0 to 3.3V)
  rawValue = analogRead(analogIn);
  voltage = (rawValue / 4095.0) * 3300; // Convert to millivolts (for ESP32's 3.3V range)
  tempC = (voltage - 500) * 0.1;        // Convert millivolts to Celsius
  ArduinoCloud.update();
  gas = 0;
  // Output to serial monitor
  Serial.print("Raw Value = ");
  Serial.print(rawValue);
  Serial.print(" milli volts = ");
  Serial.print(voltage, 0);
  Serial.print(" tempC = ");
  Serial.print(tempC, 1);

  // Display temperature on LCD
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Temp C: ");
  lcd_1.print(tempC, 1);
  temperature = tempC;

  // Simulate humidity
  humiditySensorOutput = analogRead(humidityPin);
  int humid = map(humiditySensorOutput, 0, 4095, 10, 70); // Map to percentage for ESP32
  humidity = humid;

  // Display humidity on LCD
  lcd_1.setCursor(0, 1);
  lcd_1.print("Humidity: ");
  lcd_1.print(humidity);
  lcd_1.print("%");

  // Fan control based on temperature
  if (tempC < 22 || tempC > 28) {
    digitalWrite(EN3, HIGH);  // Enable fan motor
    digitalWrite(IN1, LOW);   // Control fan direction
    digitalWrite(IN2, HIGH);
    Serial.println("Fan ON");
  } else {
    digitalWrite(EN3, LOW);   // Disable fan motor
    Serial.println("Fan OFF");
  }

  // Smoke detection using MQ2
  float sensorValue = analogRead(MQ2pin);
  if (sensorValue <= 400) {
    gas = 0;
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    digitalWrite(LED1, LOW);  // Turn off LED1 if no smoke
    Serial.print(sensorValue);
    lcd_1.print("Smoke Not");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Detected");
  } else {
    gas = 1;
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    digitalWrite(LED1, HIGH); // Turn on LED1 if smoke detected
    lcd_1.print("Smoke Detected!");
    Serial.println("Val=");
    Serial.print(sensorValue);
  }

  // Light detection using LDR
  int luminosidade = analogRead(LDR);
  Serial.println(luminosidade);
  light = luminosidade;
  if (luminosidade < 2000) { // Adjust threshold based on ESP32 ADC range
    digitalWrite(LED2, HIGH);  // Turn on LED2 in low light
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("DIM!");
    lcd_1.setCursor(0, 1);
    lcd_1.print("LED turn on");
  } else { 
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("BRIGHT!");
    lcd_1.setCursor(0, 1);
    lcd_1.print("LED turn off");
    digitalWrite(LED2, LOW);   // Turn off LED2 in bright light
  }
  
  seconds += 1;
  delay(1000); // Iterate every 1 second
}
