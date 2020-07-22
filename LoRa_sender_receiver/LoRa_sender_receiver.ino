#include "heltec.h"

#define BAND 433E6

long lastSendTime = 0;
int interval = 4000;

void setup()
{
  Heltec.begin(true,true,true,true,BAND);
  LoRa.onReceive(onReceive);
  LoRa.receive();
  Serial.println("LoRa device init succeeded");
}

void loop()
{
  if (millis() - lastSendTime > interval)
  {
    //n.setMessage_Type(type_message);
    //n.setPacket_Number(msgCount);
    sendMessage(n);

    lastSendTime = millis();
    interval = random(2000) + 1000;
    if (n.getSizeListedNodes()==10)
    {
      //Detect RSSI
      //Clear
    }
    else
    {
    LoRa.receive();
    }
  }
}

void SendMessage(Node n)
{
  LoRa.beginPacket();
  LoRa.write(n.getID());
  LoRa.write(n.getTm());
  LoRa.write(n.getPayload());
  LoRa.endPacket();
}

void OnReceive(int packetSize)
{
  if (packetSize == 0) return;  //if there's not packet, return
  char id = LoRa.read();
  char type = LoRa.read();
  if (type =='0')
  {
    //Receive string
    int rssi = LoRa.packetRssi();
    n.storeRSSI(id,rssi);
  }
  if (type =='1')
  {

  }
  if (type =='2')
  {

  }
  if (type =='3')
  {

  }
}
