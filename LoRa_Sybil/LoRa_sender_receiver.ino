#include "heltec.h"
#include "Node.hpp"

#define BAND    433E6  
long lastSendTime = 0;        // last send time
int interval = 2000; 
Node n;
char id = '2';

void setup()
{
  
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  LoRa.onReceive(onReceive);
  LoRa.receive();
  Serial.println("Heltec.LoRa init succeeded.");
  n.setID(id);
}

void loop()
{
  char type = '0';
  n.setTm(type);
  if (millis() - lastSendTime > interval)
  {
     // send a message
    sendMessage(n);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;     // 2-3 seconds
    LoRa.receive();                     // go back into receive mode
  }                     // go back into receive mode
}

void sendMessage(Node n)
{
  LoRa.beginPacket();                   // start packet
  LoRa.write(n.getID());
  LoRa.write(n.getTm());
  LoRa.print("25.7");
  LoRa.endPacket();                     // finish packet and send it
}

void onReceive(int packetSize)
{
  if (packetSize == 0) return;          // if there's no packet, return
  // read packet header bytes:
  Serial.println("Rx");
  char id = LoRa.read();          // recipient address
  char type = LoRa.read();            // sender address
  String incoming = "";                 // payload of packet
  while (LoRa.available())             // can't use readString() in callback
  {
    incoming += (char)LoRa.read();      // add bytes one by one
  }

  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  //Serial.println("Snr: " + String(LoRa.packetSnr()));

  int rssi = LoRa.packetRssi();

  if (n.SearchID(id)==true)
  {
    if(n.QueueFull(id)==1)
    {
      Serial.println("ID queue is full");
      //n.RemoveRSSI(id);
    }
  }
  else
  {
    Serial.println("Adding new id");
    n.addIDQueue(id);
  }
  n.addRSSI(id,rssi);
  n.generateGrayList();
  Serial.println("Graylist was made");
}
