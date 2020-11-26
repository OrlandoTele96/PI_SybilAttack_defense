#include "heltec.h"
#include "Node.hpp"

#define BAND    433E6
long lastSendTime = 0;        // last send time
int interval = 500;
Node n;
unsigned char id = 0x00;
unsigned char sybil[4]={'2','6','7','8'};
int c=0;
int counter[4]={0,0,0,0};
int isPow=0;
unsigned char type=0x00;
unsigned char dst;
vector<vector<char>> proofs;
vector<char> tested;
void setup() {
  // put your setup code here, to run once:
    Heltec.begin(true, true, true, true , BAND);
    LoRa.onReceive(onReceive);
    LoRa.setCodingRate4(6);
    LoRa.setSpreadingFactor(7);
    LoRa.receive();
    Serial.println("Heltec.LoRa init succeeded.");
    n.setID(id);
    n.clearhist();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char type = 0x00;
  int i;
  n.setTm(type);
  for(c=0;c<4;c++)
  {
    if (millis() - lastSendTime > interval)
    {
      int tam = proofs.size();
      if(tam>0)
      {
        for(i=0;i<tam;i++)
        {
          Serial.println("ID + "+String(tested.at(i)+"Proof"));
        }
      }
        n.setID(sybil[c]);
        sendMessage(n);
      lastSendTime = millis();
      interval = random(500);
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
  //vector<vector<char>> gl;
  /*ID src*/
  char IDE = LoRa.read();
  /*Type*/
  unsigned char type = LoRa.read();
  /*ID dst*/
  char dst = LoRa.read();
  /*Payload*/
  String incoming="";
  while(LoRa.available())
  {
    incoming += (char) LoRa.read();
  }
  /*Read RSSI*/
  int rssi = LoRa.packetRssi();
  Unpack(type,dst,incoming,IDE);
}

void Unpack(unsigned char type,char i_dst,String pay,char src)
{
  /*Desempaqueta mensajes*/
  
  vector<char> id_dst;
  int n_id_dst,i,j;
  int pay_len;
  vector<char> solution;
  int pow_t;
  vector<char> rnum;
  if(type ==0x00)
  {
    //Serial.println("Message 0 received");
  }
  if(type ==0x01)
  {
    
    pay_len = pay.length();
    n_id_dst = pay_len-4;
    for(i=0;i<n_id_dst;i++)
    {
      if(pay.charAt(i)==sybil[0])
      {
        
        Serial.println("Message 1 received");
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        proofs.push_back(rnum);
        rnum.clear();
        tested.push_back(sybil[0]);
        dst=src;
      }
      if(pay.charAt(i)==sybil[1])
      {
        
        Serial.println("Message 1 received");
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        proofs.push_back(rnum);
        tested.push_back(sybil[1]);
        rnum.clear();
        dst=src;
      }
      if(pay.charAt(i)==sybil[2])
      {
        
        Serial.println("Message 1 received");
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        proofs.push_back(rnum);
        tested.push_back(sybil[2]);
        rnum.clear();
        dst=src;
      }
      if(pay.charAt(i)==sybil[3])
      {
        
        Serial.println("Message 1 received");
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        proofs.push_back(rnum);
        tested.push_back(sybil[3]);
        rnum.clear();
        dst=src;
      }
    }
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
