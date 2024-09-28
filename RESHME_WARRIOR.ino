/**
This Arduino sketch is the code we executed on our offline live demonstration using Arduino UNO.
  **/

#include "DHT.h"

int DHT11_PIN =A2;  //input pin for DHT sensor
DHT dht11(A2, DHT11);

int LED1=2; //output pin 2
int LED2=3; //output pin 3
int sensorPin = A0; //input pin for LDR sensor
int sensorValue = 0; // variable to store the value coming from the sensor

void setup() {
  dht11.begin();
  Serial.begin(9600); //sets serial port for communication
  pinMode(2, OUTPUT); //set LED for light
  pinMode(3,OUTPUT); //set LED for temperature
 
}

void loop() {
sensorValue = analogRead(sensorPin); //read the value from the sensor
Serial.println(sensorValue); //prints the values coming from the sensor on the screen

if (sensorValue<300) {
    digitalWrite(LED1, HIGH); //Dark conditions
 
  } else {
    digitalWrite(LED1, LOW); //Bright conditions
   
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
       digitalWrite(LED2, HIGH); //Unsuitable conditions
  } else {
    digitalWrite(LED2,LOW); //Suitable conditions
  }

  }
  delay(1000); //1 second delay
}
