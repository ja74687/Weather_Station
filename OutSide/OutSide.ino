#include <ArduinoJson.h>
#include "DHT.h"
#include <LowPower.h>
#include <VirtualWire.h>


#define transmiter_pin 6
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int analogInput = A1;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;
int pinPower = 4;
StaticJsonBuffer<100> jsonBuffer;
void setup() {
 // Serial.begin(9600);
  dht.begin();
  vw_set_tx_pin(transmiter_pin);
  vw_setup(2000);
  pinMode(analogInput, INPUT);
  delay(2000);
  pinMode(pinPower, OUTPUT);
  digitalWrite(pinPower, HIGH);
}

void loop() {

  sendInfo(dht.readTemperature(), dht.readHumidity());
  delay(2000);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  
}

void sendInfo(int temp, int humidity)
{
  String toSend;
  jsonBuffer.clear();
  JsonObject& root = jsonBuffer.createObject();
  root["temp"] = temp;
  root["humidity"] = humidity;
  root["battery"] = getBattery();
  root.printTo(toSend);
  //Serial.println(toSend);
  char message[70];
  toSend.toCharArray(message, toSend.length() + 1);
  vw_send((uint8_t *)message, strlen(message));

}

double getBattery() {
  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  return (double) vin;
}

