
#include "heltec.h"
#include "Node.hpp"

#define BAND    433E6  
long lastSendTime = 0;        // last send time
int interval = 2000; 
Node n;
unsigned char id = 0x02;

void setup() {
  // put your setup code here, to run once:
    Heltec.begin(true, true, true, true , BAND);
    LoRa.onReceive(onReceive);
    LoRa.receive();
    Serial.println("Heltec.LoRa init succeeded.");
    n.setID(id);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char type = 0x00;
  n.setTm(type);
    if (millis() - lastSendTime > interval)
  {
     // send a message
    sendMessage(n);
    lastSendTime = millis();            
    interval = random(2000);    
    LoRa.receive();                     
  } 
}

void sendMessage(Node n)
{
  vector<char> payload;
  //payload = n.getPayload();
  payload.push_back('2');
  payload.push_back('6');
  payload.push_back('0');
  payload.push_back('2');
  int i=0;
  LoRa.beginPacket();
  LoRa.write(n.getID());
  LoRa.write(n.getTm());
  for (i=0;i<payload.size();i++)
  {
    LoRa.print(payload.at(i));
  }
  LoRa.endPacket();
  Serial.println("Message was sent!");
}


void onReceive(int packetSize)
{
  if (packetSize == 0) return;
  Serial.print("Message Received");
  unsigned char ID = LoRa.read();
  unsigned char type = LoRa.read();
  String incoming="";
  int rssi = LoRa.packetRssi();
  while(LoRa.available())
  {
    incoming += (char) LoRa.read();
  }

  if (n.IsinHist(ID)==true)
  {
    Serial.println("This ID is in the history"+ID);
    if(n.isQueueFull(ID)==true)
    {
      Serial.println("Queue is full, remove last rssi");
      int r = n.RemoveRSSI(ID);
      Serial.println("Removed : "+String(r));
    }
  }
  else
  {
    Serial.println("This ID is not in the history, will be added");
    n.AddIDtoHist(ID);
  }
  n.AddRSSI(ID,rssi);
  Serial.println("RSSI added succesfuly");
  n.Discard();
  Serial.println("Gray list was made");
}
