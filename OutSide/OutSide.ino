#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>
//#include <LowPower.h>
#include <VirtualWire.h>

#define DHTPIN 2        
#define DHTTYPE DHT11 
#define transmiter_pin 6

DHT_Unified dht(DHTPIN, DHTTYPE);
StaticJsonBuffer<100> jsonBuffer;

int analogInput = 1;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;
int pinPower = 4;

void setup() {
  // Serial.begin(9600);
  dht.begin();
  vw_set_tx_pin(transmiter_pin);
  vw_setup(2000);
  pinMode(analogInput, INPUT);
  pinMode(pinPower, OUTPUT);
  digitalWrite(pinPower, HIGH);
}

void loop() {
  int temp = 0;
  int humi = 0;
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  temp = event.temperature;
  dht.humidity().getEvent(&event);
  humi = event.relative_humidity;
  sendInfo(temp, humi);
  delay(30000);
  //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

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
  // Serial.println(vin);
  return (double) vin;

}

