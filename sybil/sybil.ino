#include "heltec.h"
#include "Node.hpp"

#define BAND    433E6
long lastSendTime = 0;        // last send time
int interval = 3000;
Node n;
unsigned char id = 0x00;
unsigned char sybil[4]={'2','6','7','8'};
int c=0;
int counter[4]={0,0,0,0};
int isPow=0;
unsigned char type=0x00;
unsigned char dst;
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
  unsigned char type = 0x00;
  n.setTm(type);
  for(c=0;c<4;c++)
  {
    if (millis() - lastSendTime > interval)
    {
       // send a message
      //if(counter[c]<=100000)
      //{
        n.setID(sybil[c]);
        sendMessage(n);
      //}
      lastSendTime = millis();
      interval = random(3000);
      LoRa.receive();
    }
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
  LoRa.write('s');
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
  Serial.print("Message Received");
  vector<vector<char>> gl;
  char IDE = LoRa.read();
  unsigned char type = LoRa.read();
  String incoming="";
  int rssi = LoRa.packetRssi();
  Serial.println("Received from : "+String(IDE));
  Serial.println("RSSI : "+String(rssi));
  while(LoRa.available())
  {
    incoming += (char) LoRa.read();
  }
  Serial.println(incoming);
  /*
  int a =n.IsinHist(IDE);
  if (a==1)
  {
    //Serial.println("This ID is in the history"+String(a));
    if(n.isQueueFull(IDE)==true)
    {
      Serial.println("Queue is full, remove last rssi");
      int r = n.RemoveRSSI(IDE);
      Serial.println("Removed : "+String(r));
    }
  }
  else
  {

    //Serial.println("This ID is not in the history, will be added"+String(a));
    n.AddIDtoHist(IDE);
  }
  n.AddRSSI(IDE,rssi);
  //Serial.println("RSSI added succesfuly");
  int x = n.getHistSize();
  Serial.println("Historial size = "+String(x));
  int ans= n.Discard();
  if(ans==1)
  {
    Serial.println("Gray list was made"+String(ans));
    gl = n.getGrayList();
    Serial.println("Graylist size()"+String(gl.size()));
    PrintGrayList(gl);
  }
  else
  {
    Serial.println("Gray list was not made"+String(ans));
  }*/
}

void Unpack(unsigned char type,char i_dst,String pay,char src)
{
  /*Desempaqueta mensajes*/
  
  vector<char> id_dst;
  int n_id_dst,i,j;
  int pay_len;
  vector<char> solution;
  int pow_t;
  if(type ==0x00)
  {
    //Serial.println("Message 0 received");
  }
}

void PrintGrayList(vector<vector<char>> gl)
{
  int i,j;
  Serial.println("Gray List : ");
  for(i=0;i<gl.size();i++)
  {
    Serial.println("Subconjunto : "+String(i));
    for(j=0;j<gl.at(i).size();j++)
    {
      Serial.println("ID : "+String(gl.at(i).at(j)));
    }
  }
}
