#include "heltec.h"
#include "Node.hpp"
#include "Sybil.hpp"

#define BAND 433E6

Node n;
Sybil sybil;
vector<Node> fake_identities;
char id;

void setup()
{
  Heltec.begin(true,true,true,true,BAND);
  LoRa.onReceive(onReceive);
  LoRa.receive();
  Serial.println("LoRa device init succeeded");
  id = "1";
  n.setID(id);
  fake_identities.push_back(n);
  id = "2";
  n.setID(id);
  fake_identities.push_back(n);
  id = "3";
  n.setID(id);
  fake_identities.push_back(n);
  id = "4";
  n.setID(id);
  fake_identities.push_back(n);
  sybil.setSybilNodes(s);
  int s = fake_identities.size();
  sybil.setN(s);
}

void loop()
{
  Node sender;
  int i=0;
  char type ='0';
  for(i=0;i<sybil_nodes.getN();i++)
  {
    sender=sybil.getNode(i);
    sender.Pack(type);
    sendMessage(sender);
  }
  Serial.print("Sybil attack is running");

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
  /*while(LoRa.available())
  {
    payload = LoRa.readString();
    Serial.print(payload);
  }
  n.Unpack(type,payload);
  Serial.print("Unpacked successful")*/
}
