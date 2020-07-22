#include "heltec.h"
#include "Node.hpp"
#define BAND 433E6

long lastSendTime = 0;
int interval = 4000;

Node n;
char id = '2';

void setup()
{
  Heltec.begin(true,true,true,true,BAND);
  LoRa.onReceive(onReceive);
  LoRa.receive();
  Serial.println("LoRa device init succeeded");
  n.setID(id);
  n.CreateQueue();
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
    LoRa.receive();
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
    if (n.IsFull()==1)
    {
      //Pull
      //Push
    }
    else
    {
      //Push
    }
    // Proof-of-RSSI
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
