/*
--------------------------------------------------------------------------------
                          Dispositivo honesto
--------------------------------------------------------------------------------
*/
#include "heltec.h"
#include "Node.hpp"
#define BAND    433E6
long lastSendTime = 0;        // last send time
int interval = 1000;
Node n;
vector<char> payload{'1','2'};
unsigned char id = '0'; //cambiar por cualquier ID
unsigned char sybil[4]={'2','6','7','8'};
unsigned char dst='d';//default
unsigned char type = 0x00;//Default message
int isgl=0;
int isPoW=0;
int lastpow=0;
vector<char> rnum;
vector<vector<char>> proofs;
vector<vector<char>> pows;
vector<char> tested;
vector<int> thresholds;
int T=0;
int lastbl;
int isbl=0;
int c;
int inpow=0;
int nmsj;

void setup() {
  // Inicializamos LoRa
    Heltec.begin(true, true, true, true , BAND);
    LoRa.setSpreadingFactor(7);
    LoRa.setCodingRate4(6);    
    LoRa.onReceive(onReceive);//Interrupcion para recepcion
    LoRa.receive();
    Serial.println("Heltec.LoRa init succeeded.");
    n.setID(id);//Configuramos la clase nodo
    n.setFactor(2);
    n.setDifficulty(2);
    n.setTime_interval(1000);
    n.setPoW_t();
}

void loop() {
  //n.setTm(type);
  /*
  Solo ejecutamos el main cada intervalo de tiempo aleatorio para evitar
  colisiones en la transmision de paquetes
  */
  //unsigned char dst;
  vector<char> solution;
  vector<char> bl;
  int j,i;
  int ti,tf,tt,f;
  int lastgl=0;
  if (isPoW==1)
  {
    type=0x02;
    Serial.println("Dst"+String(dst));
    int tampows = pows.size();
    for(i=0;i<tampows;i++)
    {
      inpow = 1;
      Serial.println("ID  "+String(tested.at(i))+" Proof");
      n.setID(tested.at(i));
      solution=n.solvePoW(pows.at(i));
      Pack(type,dst,solution);
      payload = solution;
      sendMessage(n);
      payload.clear();
      solution.clear();
    }
    Serial.println("All proofs were solved for dst : "+String(dst));
    tested.clear();
    pows.clear();
    isPoW=0;
    isgl=0;
    inpow=0;
  }
  if (millis() - lastSendTime > interval)
  {
    if(isPoW==0){
      if((millis()-lastbl)>T && isbl ==1)
      {
        int isblist = n.SybilDetection();
        //Serial.println("Generated!!");
        bl = n.getBlackList();
        if (isblist==1)
        {
          //Serial.println("Generated");
        //PrintBlackList(bl);
        //n.clearBlackList();
        }
        else{
          //Serial.println("**--BlackList--**");
          //Serial.println("No blacklist");
        }
        lastbl=millis();
        thresholds.pop_back();
        //Serial.println("size : "+String(thresholds.size()));
          //Serial.println("cleaning");
        thresholds.clear();
        isbl=0;
      }
      if(proofs.size()>0)
      {
        dst = 'd';
        type =0x01;
        int proof_tam = proofs.size();
        if(millis()-lastpow>thresholds.back())
        {
          //payload = proofs.back();
          //Pack(type,dst,payload);
          //sendMessage(n);
          lastpow=millis();
          //Serial.println(String(lastpow-lastbl));
          proofs.pop_back();
          lastbl=millis();
          T = thresholds.back();
          isbl=1;
        }
      }
      if(isgl==1 && thresholds.size()==0) // Si se genero la lista gris entonces se genera PoW
      {
        proofs.clear();
        //thresholds.clear();
        Serial.println("proofs of work");
        GL_pow(); // Genera PoW
        thresholds = n.calcThreshold();
        n.calcTmin();
        lastpow=0;
        lastbl=millis();
        //Serial.println(lastbl);
        //Serial.println("Pow time : "+String(T)+" , time"+String(millis()-lastpow));
        Serial.println("proofs of work generated!");
        isgl=0;
      }
      if( isgl==0 && isPoW==0)
      {
        //Serial.println("Sending temperatue");
        //Send a temperature
        for(c=0;c<4;c++)
        {
          n.setID(sybil[c]);
          type=0x00;
          dst = 'd';
          Pack(type,dst,payload);
          sendMessage(n);  
        }
      }
      lastSendTime = millis();
      interval = 2000;
      LoRa.receive(); 
    }
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
  int j=0;
  int i=0;
  //for(j=0;j<5;j++)
  //{
  LoRa.beginPacket();
  /*ID src*/
  LoRa.write(n.getID());
  /*Type message*/
  LoRa.write(n.getTm());
  /*ID dst*/
  LoRa.write(dst);
  /*Payload*/
  for (i=0;i<payload.size();i++)
  {
    LoRa.print(payload.at(i));
  }
  LoRa.endPacket(); 
  //delay(100);
  //}
  payload.clear();
  //Serial.println("Message : "+String(n.getTm())+String(dst));
}


void onReceive(int packetSize)
{
  if (packetSize == 0) return;
  /*ID src*/
  char IDE = (char)LoRa.read(); // Recibe ID
  //Serial.println("Received from : "+String(IDE)+":"+String(type));
  /*Type message*/
  unsigned char type = LoRa.read(); // Recibe tipo de mensaje
  /*ID dst*/
  char dst = (char)LoRa.read();
  //Serial.println("Destination ID : "+String(dst));
  /*Payload*/
  String incoming="";
  while(LoRa.available())
  {
    /*
    Recibe todo el payload del mensaje
    */
    incoming += (char) LoRa.read();
  }
  /*Reading RSSI*/
  int rssi = (int )LoRa.packetRssi(); // Obtiene rssi del mensaje
  //Serial.println("RSSI: "+String(rssi));
  /*Storage RSSI*/
  storageRSSI(IDE,type,rssi); // Almacenamos el ID y rssi recibido
  /*Phase 1*/
  if(nmsj==100)
    isgl= n.Discard(); // Algoritmo de descarte de nodos maliciosos
  /*Unpack content*/
  Unpack(type,dst,incoming,IDE);
}

void Unpack(unsigned char type,char i_dst,String pay,char src)
{
  /*Desempaqueta mensajes*/
  
  vector<char> id_dst;
  int n_id_dst,i,j;
  int pay_len;
  vector<char> solution;
  vector<char> rnum;
  int pow_t;
  int pow_f;
  if(type ==0x00)
  {
    //Serial.println("Message 0 received");
  }
  if(type ==0x01 && inpow ==0)
  {
    pay_len = pay.length();
    n_id_dst = pay_len-4;
    for(i=0;i<n_id_dst;i++)
    {
      if(pay.charAt(i)==sybil[0])
      {
        
        Serial.println("Message 1 received : "+sybil[0]);
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        pows.push_back(rnum);
        rnum.clear();
        tested.push_back(sybil[0]);
        dst=src;
      }
      if(pay.charAt(i)==sybil[1])
      {
        
        Serial.println("Message 1 received: "+sybil[1]);
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        pows.push_back(rnum);
        tested.push_back(sybil[1]);
        rnum.clear();
        dst=src;
      }
      if(pay.charAt(i)==sybil[2])
      {
        
        Serial.println("Message 1 received"+sybil[2]);
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        pows.push_back(rnum);
        tested.push_back(sybil[2]);
        rnum.clear();
        dst=src;
      }
      if(pay.charAt(i)==sybil[3])
      {
        
        Serial.println("Message 1 received"+sybil[3]);
        rnum.push_back(pay.charAt(pay_len-4));
        rnum.push_back(pay.charAt(pay_len-3));
        rnum.push_back(pay.charAt(pay_len-2));
        rnum.push_back(pay.charAt(pay_len-1));
        pows.push_back(rnum);
        tested.push_back(sybil[3]);
        rnum.clear();
        dst=src;
      }
    }
    isPoW = 1;
  }
  if(type==0x02)
  {
    //Serial.println("Message 2 received from : "+String(i_dst)+": "+String(src));
    /*if(i_dst == n.getID())
    {
      Serial.println("Node "+String(src)+"\thas replied a Pow with : "+pay);
      pow_f = millis();
      pow_t = pow_f-lastpow;
      //Serial.println(pow_t);
      n.AddPowTime(pow_t);
      Serial.println("Timed at"+String(pow_t));
      String pow_s=pay;
      for (i=0;i<(pow_s.length());i++)
      {
        //Serial.print(pow_s.charAt(i));
        solution.push_back(pow_s.charAt(i));
      }
      n.AddAnswer(solution);
      solution.clear();
    }*/  
  }
}

void Pack(unsigned char type_m,unsigned char id_dest,vector<char> pa)
{
  if(type_m ==0x00)
  {
    //Serial.println("Packing message for temperature");
    payload.push_back('1');
  }
  if(type_m == 0x01)
  {
    //Serial.println("Packing message for PoW solicitude");
    isgl=0;
  }
  if (type_m == 0x02)
  {
    //Serial.println("Packing message for PoW solicitude confirmation");
    //set dst
    //set payload
    payload = pa;
    //Serial.println(id_dest);
    n.setIDdst(id_dest);
    //Serial.println(n.getIDdst());
  }
  if (type_m == 0x03)
  {
    //Serial.println("Packing message for PoW solution");
  }
  if (type_m == 0x04)
  {
    //Serial.println("Packing message for consensus");
  }
  n.setTm(type_m);
  n.setIDdst(id_dest);
  n.setPayload(payload);
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
  nmsj++;
}
vector<char> MakePayload(vector<char> ID,vector<char> randnum)
{
  vector<char> pay;
  int i;
  /*Add payload :
  [+]   4 bytes : rand_num
  [+]   n bytes : IDs*/
  for(i=0;i<ID.size();i++)
  {
    pay.push_back(ID.at(i));
  }
  pay.push_back(randnum.at(0));
  pay.push_back(randnum.at(1));
  pay.push_back(randnum.at(2));
  pay.push_back(randnum.at(3));
  //n.setPayload(pay);
  return pay;
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
    //gl = n.getGrayList();
    vector<char> d = {'6','7','8','2','3'};
    vector<char> f = {'3','7','8','6','2'};
    gl.push_back(d);
    gl.push_back(f);
    //PrintGrayList(gl); // Solamente imprime la lista gris
    tam =gl.size();
    int rnd;
    String rn;
    vector<char> rand_n,proof;
    vector<vector <char>> sol;
    if(tam>0)//primero nos aseguramos que exista la lista gris o que haya almenos un subconjunto
    {
      for(i=0;i<tam;i++) // Recorremos la lista gris y generamos una Pow para cada subconjunto
      {
        /*Generating random number*/
          rnd = random(1000,2000);
          rn = String(rnd);
          //Serial.println("Random number : "+rn);
          rand_n.push_back(rn.charAt(0));//insert(0,)
          rand_n.push_back(rn.charAt(1));
          rand_n.push_back(rn.charAt(2));
          rand_n.push_back(rn.charAt(3));
          n.genPoW(gl[i],rand_n);
          proof = MakePayload(gl[i],rand_n);
          proofs.push_back(proof);
          rand_n.clear();
      }

      n.removesubset(); // Elimina los subconjuntos que ya fueron generados para Pow
    }
}


void PrintGrayList(vector<vector<char>> gl)
{
  int i,j;
  //int prom,desv;
  Serial.println("**");
  Serial.println("**--Gray List--**");
  for(i=0;i<gl.size();i++)
  {
    Serial.println("\nSubconjunto : "+String(i));
    for(j=0;j<gl.at(i).size();j++)
    {
      Serial.print(String(gl.at(i).at(j)));
    }
  }
  Serial.println("\n**");
}

void PrintBlackList(vector<char> bl)
{
  int i;
  Serial.println("**---BlackList--**");
  for(i=0;i<bl.size();i++)
  {
    Serial.print(String(bl.at(i)));
  }
  Serial.println("\n**");
}
