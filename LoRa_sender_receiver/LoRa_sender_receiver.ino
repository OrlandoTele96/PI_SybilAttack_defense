#include "heltec.h"
#include "Node.hpp"
#define BAND 433E6

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
    sendMessage(n);
    LoRa.receive();
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
  int rssi = LoRa.packetRssi();
  if (n.SearchID(id)==true)// Revisa si esta en la lista historial
  {
    /*Si esta, revisa que el tam de la cola sea indicada si no lo
    es, saca el primer elemento*/
    if (n.QueueFull(id)==true)
    {
      n.RemoveRSSI(id);
    }
  }
  else
  {
    /*Si no esta en el historial, se crea la cola para este id*/
    n.addIDQueue(id);
  }
  n.addRSSI(id,rssi);
  n.generateGrayList();
}
