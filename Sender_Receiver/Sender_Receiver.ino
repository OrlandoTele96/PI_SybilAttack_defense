#include "heltec.h"
#include "Node.hpp"

#define BAND    433E6  
long lastSendTime = 0;        // last send time
int interval = 3000; 
Node n;
unsigned char id = '2';
unsigned char type = 0x00;
int isgl=0;

void setup() {
  // put your setup code here, to run once:
    Heltec.begin(true, true, true, true , BAND);
    LoRa.onReceive(onReceive);
    LoRa.receive();
    Serial.println("Heltec.LoRa init succeeded.");
    n.setID(id);
    n.clearhist();
}

void loop() {
  // put your main code here, to run repeatedly:
  n.setTm(type);
  if (millis() - lastSendTime > interval)
  {
     // send a message
    //n.setID(id);
    n.setTm(type);
    sendMessage(n);
    lastSendTime = millis();            
    interval = random(3000);    
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
  //Serial.println("Message was sent!");
}


void onReceive(int packetSize)
{
  if (packetSize == 0) return;
  Serial.println("**");
  Serial.println("-----------Receiving-----");
  unsigned char IDE = LoRa.read();
  unsigned char type = LoRa.read();
  String incoming="";
  int rssi = (int )LoRa.packetRssi();
  Serial.println("Received from : "+String(IDE));
  Serial.println("RSSI: "+String(rssi));
  while(LoRa.available())
  {
    incoming += (char) LoRa.read();
  }
  storageRSSI(IDE,type,rssi);
  isgl= n.Discard();
  //n.setr(rssi);
  //Serial.println("rssi"+String(n.getr()));
  if(isgl==1)
    {
      GL_pow();
    }
  //Unpack()
  Serial.println("-----------------------");
  Serial.println("**");
}

void Unpack()
{
  
}

void storageRSSI(char IDE, char type, int rssi)
{
  int a =n.IsinHist(IDE);
  if (a==1)
  {
    if(n.isQueueFull(IDE)==true)
    {
      int r = n.RemoveRSSI(IDE);
    }
  }
  else
  {
    n.AddIDtoHist(IDE);
  }
  n.AddRSSI(IDE,rssi);
  //Serial.println("Storage!!");
}

void GL_pow()
{
    vector<vector<char>> gl;
    int i=0;
    int tam;
    gl = n.getGrayList();
    PrintGrayList(gl);
    tam =gl.size();
    if(tam>0)
    {
      //Serial.println("Size : "+String(tam));
      for(i=0;i<tam;i++)
      {
          //i++;
          //genPoW
          //packtomsg
          //send
          //remove subset
          //n.removesubset();
          Serial.println("PoW generated : "+String(i));
          //gl = n.getGrayList();
          //Serial.println("Size : "+String(gl.size()));
      }
    
      n.removesubset();
    }
}

void PrintGrayList(vector<vector<char>> gl)
{
  int i,j;
  //int prom,desv;
  Serial.println("**");
  Serial.println("----------Gray List : ");
  for(i=0;i<gl.size();i++)
  {
    Serial.println("Subconjunto : "+String(i));
    for(j=0;j<gl.at(i).size();j++)
    {
      //prom = (int)n.getP(gl.at(i).at(j));
      //desv = (int)n.getV(gl.at(i).at(j));
      Serial.println("ID : "+String(gl.at(i).at(j)));
      //Serial.println("prom : "+String(prom));
      //Serial.println("desv : "+String(desv));
    }
  }
  Serial.println("**");
}
