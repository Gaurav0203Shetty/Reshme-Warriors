#include <Adafruit_LiquidCrystal.h>

// Constants
const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int buzz = 3;
const int MQ2pin = A4;
const int tempPin = A3;
const int humidityPin = A2;
const int lcdColumns = 16;
const int lcdRows = 2;
const int IN1 = 8;
const int IN2 = 7;
const int EN1 = 9;
const int EN2 = 10;
const int LDR = A1;

// Variables
int rawValue = 0;
double voltage = 0;
double tempC = 0;
int humiditySensorOutput = 0;
int seconds = 0;

// LCD instance
Adafruit_LiquidCrystal lcd_1(0);

void setup() {  
  Serial.begin(9600);
  pinMode(humidityPin, INPUT);
  lcd_1.begin(lcdColumns, lcdRows);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(MQ2pin, INPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(LED3, OUTPUT);
  pinMode(buzz, OUTPUT);
}

void loop() {
  // Read temperature sensor
  rawValue = analogRead(tempPin);
  voltage = (rawValue / 1023.0) * 5000; // Convert to millivolts
  tempC = (voltage - 500) * 0.1; // Convert millivolts to Celsius

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

  // Simulate humidity
  humiditySensorOutput = analogRead(humidityPin);
  int humidity = map(humiditySensorOutput, 0, 1023, 10, 70); // Map to percentage

  // Display humidity on LCD
  lcd_1.setCursor(0, 1);
  lcd_1.print("Humidity: ");
  lcd_1.print(humidity);
  lcd_1.print("%");
  
  delay(1000);

  // Fan control based on temperature
  if (tempC < 22 || tempC > 28) {
    digitalWrite(LED3, HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    digitalWrite(EN1, HIGH);
    digitalWrite(IN1, LOW);
  	digitalWrite(IN2, HIGH);
    lcd_1.print("Fan ON");
    digitalWrite(buzz, HIGH);
  } else {
    digitalWrite(LED3, LOW);
    digitalWrite(EN1, LOW);
    Serial.println("Fan OFF");
    digitalWrite(buzz, LOW);
  }
  
  delay(1000);

  float sensorValue = analogRead(MQ2pin);
  if(sensorValue <= 650){
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    digitalWrite(LED2, LOW);
    Serial.print(sensorValue);
    lcd_1.print("Smoke Not");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Detected");
    digitalWrite(buzz, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(EN2, LOW);
    Serial.println("Fan OFF");
  }else{
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    digitalWrite(LED2, HIGH);
    lcd_1.print("Smoke Detected!");
    Serial.println("Val=");
    Serial.print(sensorValue);
    digitalWrite(buzz, HIGH);
    digitalWrite(EN2, HIGH);
    digitalWrite(IN1, LOW);
  	digitalWrite(IN2, HIGH);
    lcd_1.print("Fan ON");
  }
  
  delay(1000);
  
  int luminosidade = analogRead(A0);
  Serial.println(luminosidade);
  if(luminosidade < 755) {
  	digitalWrite (LED1 ,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("DIM!");
    lcd_1.setCursor(0, 1);
    lcd_1.print("LED turn on");
  }
  else { 
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("BRIGHT!");
    lcd_1.setCursor(0, 1);
    lcd_1.print("LED turn off");
    digitalWrite(LED1 , LOW);
  }
  
  seconds += 5;
  delay(5000); // Iterate every 5 second
}