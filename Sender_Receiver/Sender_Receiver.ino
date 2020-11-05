/*
--------------------------------------------------------------------------------
                          Dispositivo honesto
--------------------------------------------------------------------------------
*/
#include "heltec.h"
#include "Node.hpp"

#define BAND    433E6
long lastSendTime = 0;        // last send time
int interval = 2000;
Node n;
unsigned char id = '1'; //cambiar por cualquier ID
unsigned char type = 0x00;
int isgl=0;
int counter=0;
void setup() {
  // Inicializamos LoRa
    Heltec.begin(true, true, true, true , BAND);
    LoRa.onReceive(onReceive);//Interrupcion para recepcion
    LoRa.receive();
    Serial.println("Heltec.LoRa init succeeded.");
    //Configuramos la clase nodo
    n.setID(id);
}

void loop() {
  //n.setTm(type);
  /*
  Solo ejecutamos el main cada intervalo de tiempo aleatorio para evitar
  colisiones en la transmision de paquetes
  */
  if (millis() - lastSendTime > interval)
  {
    //if(counter<=100000)
    //{
     // send a message
     n.setTm(type);
     sendMessage(n);
    //}
    lastSendTime = millis();
    interval = random(2000);
    LoRa.receive();

  }
}

void sendMessage(Node n)
{
  /*
  Este codigo envia mensajes de diferentes tipos
  Tipo 0 : mensaje generico o de cualquier clase
  Tipo 1 : mensaje de solicitud de PoW
  Tipo 2 : Confirmacion de PoW
  Tipo 3 : mensaje de respuesta de PoW
  Tipo 4 : mensaje de consenso
  */
  int i=0;
  LoRa.beginPacket();
  LoRa.write(n.getID());
  LoRa.write(n.getTm());
  for (i=0;i<payload.size();i++)
  {
    LoRa.print(payload.at(i));
  }
  LoRa.endPacket();
  counter ++;
  //Serial.println("Message was sent!");
}


void onReceive(int packetSize)
{
  if (packetSize == 0) return;
  //Serial.println("**");
  //Serial.println("-----------Receiving-----");
   char IDE = (char)LoRa.read(); // Recibe ID
  unsigned char type = LoRa.read(); // Recibe tipo de mensaje
  String incoming="";
  int rssi = (int )LoRa.packetRssi(); // Obtiene rssi del mensaje
  Serial.println("Received from : "+String(IDE));
  Serial.println("RSSI: "+String(rssi));
  while(LoRa.available())
  {
    /*
    Recibe todo el payload del mensaje
    */
    incoming += (char) LoRa.read();
  }
<<<<<<< HEAD
  storageRSSI(IDE,type,rssi); // Almacenamos el ID y rssi recibido
  isgl= n.Discard(); // Algoritmo de descarte de nodos maliciosos
  if(isgl==1) // Si se genero la lista gris entonces se genera PoW
  {
    GL_pow(); // Genera PoW
  }
  else
  {
    type = 0x00;
  }
=======
  //storageRSSI(IDE,type,rssi); // Almacenamos el ID y rssi recibido
  //isgl= n.Discard(); // Algoritmo de descarte de nodos maliciosos
  //if(isgl==1) // Si se genero la lista gris entonces se genera PoW
    //{
      //GL_pow(); // Genera PoW
    //}
>>>>>>> master
  //Unpack()
  //Serial.println("-----------------------");
  //Serial.println("**");
}

void Unpack()
{
  /*Desempaqueta mensajes*/
}

void Pack(unsigned char type_m,unsigned char id_dest,vector<char> payload)
{
  if(type_m ==0x00)
  {
    Serial.println("Packing message for temperature");
    n.setTm(type_m);
    //ID dest.

  }
  if(type_m == 0x01)
  {
    Serial.println("Packing message for PoW solicitude");
    n.setTm(type_m);
    //ID dest.
  }
  if (type_m == 0x02)
  {
    Serial.println("Packing message for PoW solicitude confirmation");
  }
  if (type_m == 0x03)
  {
    Serial.println("Packing message for PoW solution");
  }
  if (type_m == 0x04)
  {
    Serial.println("Packing message for consensus");
  }
}

void storageRSSI(char IDE, char type, int rssi)
{
  /*
  Almacena un ID en una estructura de datos y su correspondiente rssi
  en una cola de rssi
  */
  int a =n.IsinHist(IDE);
  if (a==1)
  {
    /*Si el ID existe en el historial de IDs revisa que la cola de rssi no este
    llena*/
    if(n.isQueueFull(IDE)==true)
    {
      /*
      Si esta llena la cola de rssi, elimina el rssi mas viejo.
      */
      int r = n.RemoveRSSI(IDE);
    }
  }
  else
  {
    /*
    Si no existe el ID en el historial de IDs, se agrega el nuevo ID en el
    historial y se crea su cola de rssi.
    */
    n.AddIDtoHist(IDE);
  }
  n.AddRSSI(IDE,rssi); //Agregamos el rssi del mensaje a sus correspondiente ID
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
    PrintGrayList(gl); // Solamente imprime la lista gris
    tam =gl.size();
    vector<char> rand_n = {'1','2','3','4'};
    vector<vector <char>> sol;
    if(tam>0)//primero nos aseguramos que exista la lista gris o que haya almenos un subconjunto
    {
      for(i=0;i<tam;i++) // Recorremos la lista gris y generamos una Pow para cada subconjunto
      {
          //genPoW
          sol=n.genPoW(gl[i],rand_n);
          //packtomsg
          //Pack(0x01);
          //send
          sendMessage(n);
          Serial.println("PoW generated : "+String(i));
      }

      n.removesubset(); // Elimina los subconjuntos que ya fueron generados para Pow
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
      Serial.println("ID : "+String(gl.at(i).at(j)));
    }
  }
  Serial.println("**");
}
