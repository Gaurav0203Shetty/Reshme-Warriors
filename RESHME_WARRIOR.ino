#include "DHT.h"

int DHT11_PIN =A2; 
DHT dht11(A2, DHT11);

int LED1=2; //output pin 1
int LED2=3; //output pin 2
int sensorPin = A0; //input pin for LDR
int sensorValue = 0; // variable to store the value coming from the sensor

void setup() {

  Serial.begin(9600);
  dht11.begin();
  Serial.begin(9600); //sets serial port for communication
  pinMode(2, OUTPUT);
  pinMode(3,OUTPUT);
 
}

void loop() {
sensorValue = analogRead(sensorPin); // read the value from the sensor
Serial.println(sensorValue); //prints the values coming from the sensor on the screen

if (sensorValue<300) {
    digitalWrite(LED1, HIGH); 
 
  } else {
    digitalWrite(LED1, LOW);
   
  }


  delay(2000);
  float hum  = dht11.readHumidity();
// read temperature as Celsius
  float tempC = dht11.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht11.readTemperature(true);
//check if any reads fialed
if (isnan(hum) || isnan(tempC) || isnan(tempF)){
  Serial.println("Failed to read the values from the dht11 sensor");
}
  else{
    Serial.print("DHT11# Humidity:  ");
    Serial.print(hum);
    Serial.print("%");

    Serial.print("|");
    Serial.print("Temperature");
    Serial.print(tempC);
    Serial.print("°C ");
    Serial.print(tempF);
    Serial.println("°F");
    if (tempC < 22 || tempC > 28) {
       digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED2,LOW);
  }

  }
  delay(1000);
}
