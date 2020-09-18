//
//  Node.cpp
//  PI_SybilAttack_defense
//
//  Created by <author> on 05/09/2020.
//
//

#include "Node.hpp"
/*Constructors*/
Node::Node(unsigned char Id,unsigned char tm):id(Id),type(tm)
{
  //
}
Node::Node(const Node &n)
{
  this->id = n.id;
  this->type = n.type;
}
/*Getter & Setter*/
unsigned char Node::getID()const
{
  return this->id;
}
unsigned char Node::getTm()const
{
  return this->type;
}
vector<char> Node::getPayload()const
{
  return this->payload;
}
vector<vector<char>> Node::getGrayList()const
{
  return this->graylist;
}
void Node::setID(unsigned char id)
{
  this->id = id;
}
void Node::setTm(unsigned char tm)
{
  this->type = tm;
}
void Node::setPayload(vector<char> p)
{
  this->payload = p;
}
void Node::clearhist(){
  this->Hist.clear();
}
/*Queue function*/
queue Node::create(unsigned char id)
{
  /*
  Esta función se manda a llamar cuando se acaba de conocer un nuevo ID, por lo que
  se crea un espacio en el historial para ese mismo ID mediante la creación de su
  estructura de datos queue (que almacena los ultimos 10 RSSI y otros datos más
  que son útiles basados en estos valores de RSSI, como promedio, varianza y
  desviacíon estandar).
  Recibe como parámetro el ID que recién llegó y se regresa la estructura de datos
  que se generó para este ID.
  */
  queue q;
  q.start = -1;
  q.end = 0;
  q.ID = id;
  return q;
}
/*RSSI Storage*/
void Node::AddIDtoHist(unsigned char id)
{
  /*
  Esta función agrega estructuras de datos relacionadas al ID al historial de IDs
  que se conoce en el dispositivo.
  Recibe el ID que se quiere agregar.
  */
  queue q;
  q= create(id);
  this->Hist.push_back(q); //Agrega la estructura de datos creada al historial
}
int Node::IsinHist(unsigned char id)
{
  /*
  Función encargada de buscar si un ID está contenido dentro del historial de
  ID.
  Recibe el ID que se desa buscar, regresa 1 si se encontró  y 0 en caso
  contrario.
  */
  int i=0;
  int ans=0;
  if(this->Hist.size()>0)
  {

    for(i=0;i<this->Hist.size();i++)
    {
      if(this->Hist.at(i).ID==id)
      {
        ans = 1;
      }
    }
  }
  return ans;
}

bool Node::isQueueFull(unsigned char id)
{
  /*
  Esta función revisa si la cola de RSSI de un ID dado está llena, es decir,
  si contiene 10 elementos se considera como lleno.
  Recibe el ID que se desea revisar y devuelve true si está lleno y false en caso
  contrario.
  */
  bool ans;
  int i;
  for (i=0;i<this->Hist.size();i++)
  {
    if (id == this->Hist.at(i).ID)
    {
      if (this->Hist.at(i).end ==10)
      {
        ans = true;
      }
      else
      {
        ans = false;
      }
    }
  }
  return ans;
}
int Node::RemoveRSSI(unsigned char id)
{
  /*
  Elimina el último elemento de la cola de RSSI (el RSSI más viejo).
  Recibe el ID al cual se refiera esta cola y regresa el RSSI eliminado.
  */
  int i;
  int j,rssi;
  for (i=0;i<this->Hist.size();i++)
  {
    if (id == this->Hist.at(i).ID)
    {
      rssi = this->Hist.at(i).RSSI[0];
      if(this->Hist.at(i).end>0)
      {
        for (j=0;j<this->Hist.at(i).end-1;j++)
        {
          this->Hist.at(i).RSSI[j]= this->Hist.at(i).RSSI[j+1];
        }
        this->Hist.at(i).end--;
      }
      //PrintQueue(&this->received_mjs.at(i));
    }
  }
  return rssi;
}
void Node::AddRSSI(unsigned char id,int rssi)
{
  /*
  Agrega un nuevo RSSI en la cola.
  Recibe como parámetros el ID al que se refiera la cola y el RSSI que se
  desea agregar.
  */
  int i;
  for (i=0;i<this->Hist.size();i++)
  {
    if (id == this->Hist.at(i).ID)
    {
      if(this->Hist.at(i).end<=10)
      {
        this->Hist.at(i).RSSI[this->Hist.at(i).end]=rssi;
        this->Hist.at(i).end++;
      }
      //PrintQueue(&this->received_mjs.at(i));
    }
  }
}
int Node::getHistSize()
{
  /*
  Regresa el tamaño del historial
  */
  return this->Hist.size();
}
/*Phase 1 : RSSI*/
void Node::computeProm(queue *q)
{
  /*
  Calcula el promedio de los 10 ultimos RSSI recibidos.
  Recibe como parámetro el apuntado del elemento en el historial al que se refiere.
  */
  int i=0;
  int aux;
  //float n=10;
  for(i=0;i<q->end;i++)
  {
    aux += q->RSSI[i];
  }
  q->prom = aux/10;
}
void Node::computeVar(queue *q)
{
  /*
  Calcula la varianza de los 10 ultimos RSSI recibidos.
  Recibe como parámetro el apuntado del elemento en el historial al que se refiere.
  */
  int i=0;
  int aux;
  //float n = 10;
  for (i=0;i<q->end;i++)
  {
    aux += (q->RSSI[i]-q->prom)*(q->RSSI[i]-q->prom);
  }
  q->var=aux/10;
}
void Node::computeDesv(queue *q)
{
  /*
  Calcula la desviación estandar de los 10 ultimos RSSI recibidos.
  Recibe como parámetro el apuntado del elemento en el historial al que se refiere.
  */
  q->desv=sqrt(q->var);
}
int Node::Discard()
{
  /*
   Esta función descarta ID dentro del historial de los que si pudieran ser
   sybil. Si un ID tiene más de elementos en su lista de RSSI es considerado para
   ser comparado con otros ID, de otra forma no es considerado en esta prueba.
  */
    int i,j;
    vector<queue> id_test;
    vector<char> suspected;
    int sup,inf;
    int ans;
    for (i=0;i<this->Hist.size();i++)
    {
      if (this->Hist.at(i).end>9)
      {
        //compute average
        computeProm(&this->Hist.at(i));
        //calcVar
        computeVar(&this->Hist.at(i));
        //calcDesv
        computeDesv(&this->Hist.at(i));
        //Add to list
        id_test.push_back(this->Hist.at(i));
      }
    }
    if(id_test.size()>1)
    {
      /*Si existe más de un ID que pueda ser probado entonces se recorre el arreglo
      de IDs que se pueden probar y se comparan uno con otro
      Por lo que si el promedio de ID_j está dentro del intervalo del promedio
      de ID_i + ó - su desviacion estandar, entonces se agrega a la lista gris y
      pasa a ser posible sospechoso.

      */
      //If size >= discard algorithm
      for(i=0;i<id_test.size();i++)
      {
        suspected.clear();
        for(j=0; j<id_test.size();j++)
        {
          inf = 0;
          sup = 0;
          if(id_test.at(i).ID!=id_test.at(j).ID)
          {
            inf = id_test.at(i).prom-(2*(id_test.at(i).desv));
            sup =  id_test.at(i).prom+(2*(id_test.at(i).desv));
            if(id_test.at(j).prom>inf && id_test.at(j).prom<sup)
            {
              if(inGraylist(id_test.at(i).ID,suspected)!=1){
                suspected.push_back(id_test.at(i).ID);
              }
              if(inGraylist(id_test.at(j).ID,suspected)!=1){
                suspected.push_back(id_test.at(j).ID);
              }
            }
          }
        }
        if(suspected.size()>0){
          /*Si hay al menos un ID sospechoso se agrega a la lista gris*/
          this->graylist.push_back(suspected);
        }

      }
    }
    if(this->graylist.size()>0)
    {
      ans=1;
    }
    else{
      ans=0;
    }
    return ans;
}
int Node::inGraylist(char id,vector<char> subset)
{
  /*Si existe el id en el subconjunto de sospechosos, regresa 1*/
  int i=0;
  int j=0;
  int ans=0;
  for (i=0;i<subset.size();i++)
  {
    if(subset.at(i)==id)
      {
        ans=1;
      }
  }
  return ans;
}

/*Phase 2: PoW*/

void Node::removesubset()
{
  this->graylist.erase(this->graylist.begin());
}
