#include "heltec.h"
#include "Node.hpp"
#define BAND 433E6

Node n;
char id = '1';

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
    char type ='0';
    int i;
    if(n.getSizeGrayList()>0)
    {
      for (i=0;i<n.getSizeGrayList();i++)
      {
        n.generatePoWs(n.getGrayList().at(i));
        type='1';
        n.setTm(type);
        n.Pack(type);
        sendMessage(n);
        //Erase this ID from gray list
      }
    }
    //Send other message
    type ='0';
    n.setTm(type);
    n.Pack(type);
    sendMessage(n);
    LoRa.receive();
}

void SendMessage(Node n)
{
  LoRa.beginPacket();
  LoRa.write(n.getID());
  LoRa.write(n.getTm());
  LoRa.print(n.getPayload());
  LoRa.endPacket();
}

void OnReceive(int packetSize)
{
  if (packetSize == 0) return;  //if there's not packet, return
  /*Read data*/
  char id = LoRa.read();
  char type = LoRa.read();
  int rssi = LoRa.packetRssi();
  String payload;
  /*Read payload*/
  while(LoRa.available())
  {
    payload = LoRa.readString();
    Serial.print(payload);
  }
  /*Adding to rssi list*/
  if (n.SearchID(id)==true)// Revisa si esta en la lista historial
  {
    /*if there's an id in the history then it will check if the queue is full*/
    if (n.QueueFull(id)==true)
    {
      n.RemoveRSSI(id);
    }
  }
  else
  {
    /*if there's not in the history will add this id in that queue*/
    n.addIDQueue(id);
  }
  n.addRSSI(id,rssi);
  /*Compute a gray list*/
  n.generateGrayList();
  /*Unpack any type of message...*/
  n.Unpack(type,payload);
  Serial.print("Unpacked successful")
}
