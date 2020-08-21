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
        n.generatePoWs(n.getGrayList().at(i)); //Generate a PoW for each subset in the history
        type='1';
        n.setTm(type);
        n.Pack(type);
        sendMessage(n);
        //Erase this ID from gray list
      }
    }
    else
    {
      //Send other message
      n.setTm(type);
      n.Pack(type);
      sendMessage(n);
    }
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
  String payload;
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
  n.Unpack(type,payload);
}
