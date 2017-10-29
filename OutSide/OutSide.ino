#include <ArduinoJson.h>
#include "DHT.h"
#include <VirtualWire.h>

#define transmiter_pin 6
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  vw_set_tx_pin(transmiter_pin);
  vw_setup(2000);
}

void loop() {

  readSensor();

}

void sendInfo(int temp, int humidity)
{
  String toSend;
  StaticJsonBuffer<50> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["temp"] = temp;
  root["humidity"] = humidity;
  root.printTo(toSend);
  //Serial.println(toSend);
  char message[30]; 
  toSend.toCharArray(message, toSend.length() + 1);
  vw_send((uint8_t *)message, strlen(message));

}

void readSensor()
{
  Serial.print("Wilgotnosc: ");
  Serial.print(dht.readHumidity());
  Serial.println(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(dht.readTemperature());
  Serial.println(" *C ");
  sendInfo(dht.readTemperature(), dht.readHumidity());
  delay(2000);
}
