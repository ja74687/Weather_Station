#include <VirtualWire.h>

#define receive_pin 6


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
    Serial.println(wiadomosc);
  }
}
