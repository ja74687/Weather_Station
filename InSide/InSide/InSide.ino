#include <ArduinoJson.h>
#include <VirtualWire.h>

#define receive_pin 6

int temp = 0;
int humi = 0;
float battery = 0.0;

void setup()
{
  Serial.begin(9600);
  vw_set_rx_pin(receive_pin);
  vw_setup(2000);
  vw_rx_start();


}

void loop()
{
  receiveData();  
}

void receiveData() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen))
  {
    String wiadomosc;
    for (int i = 0; i < buflen; i++) 
    {
      wiadomosc += char(buf[i]);
    }
  //  char json[wiadomosc.length()];
   // wiadomosc.toCharArray(json, wiadomosc.length());
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(wiadomosc);
    battery = root["battery"];
    temp = root["temp"];
    humi = root["humidity"];
   // Serial.print(temp);
   // Serial.print(" ");
  //  Serial.print(humi);
   // Serial.print(" ");
    Serial.println(battery, 7);
    Serial.println(wiadomosc);
  }
}
