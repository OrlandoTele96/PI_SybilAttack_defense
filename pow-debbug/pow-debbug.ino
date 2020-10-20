
#include "heltec.h"
#include "Node.hpp"
#include <vector>

#define BAND    433E6  
long lastSendTime = 0;        // last send time
int interval = 2000; 
Node n;
unsigned char id = 0x00;
std::vector<std::vector<char>> gl{{'1','2'},{'2'},{'3','4','5'}};
std::vector<char> subset;


void setup() {
  // put your setup code here, to run once:
    Heltec.begin(true, true, true, true , BAND);
    LoRa.onReceive(onReceive);
    LoRa.receive();
    Serial.println("Heltec.LoRa init succeeded.");
    n.setID(id);
    n.clearhist();
    n.setGrayList(gl);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char type = 0x00;
  int i=0;
  n.setTm(type);
  //vector<vector<char>> gray;
  if (millis() - lastSendTime > interval)
  {
    //gray = n.getGrayList();
     // send a message
    n.setID(id);
    GL_pow();
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
  //Serial.println("Message was sent!");
}


void onReceive(int packetSize)
{
  if (packetSize == 0) return;
  Serial.print("Message Received");
  vector<vector<char>> gl;
  unsigned char IDE = LoRa.read();
  unsigned char type = LoRa.read();
  String incoming="";
  int rssi = LoRa.packetRssi();
  Serial.println("Received from : "+String(IDE));
  Serial.println("RSSI : "+String(rssi));
  while(LoRa.available())
  {
    incoming += (char) LoRa.read();
  }
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

void GL_pow()
{
  /*
  Este codigo genera PoW para cada subconjunto de la lista gris y al final
  los borra.
  */
    vector<vector<char>> gl;
    int i=0;
    int tam;
    gl = n.getGrayList();
    //PrintGrayList(gl); // Solamente imprime la lista gris
    tam =gl.size();
    vector<char> rand_n = {'1','2','3','4'};
    if(tam>0)//primero nos aseguramos que exista la lista gris o que haya almenos un subconjunto
    {
      for(i=0;i<tam;i++) // Recorremos la lista gris y generamos una Pow para cada subconjunto
      {
          //i++;
          //genPoW
          n.genPoW(gl[i],rand_n);
          //packtomsg
          //send
          //remove subset
          //n.removesubset();
          Serial.println("PoW generated : "+String(i));
          //gl = n.getGrayList();
          //Serial.println("Size : "+String(gl.size()));
      }

      //n.removesubset(); // Elimina los subconjuntos que ya fueron generados para Pow
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
