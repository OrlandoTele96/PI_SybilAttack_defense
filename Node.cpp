//
//  Node.cpp
//  PI_SybilAttack_defense
//
//  Created by <author> on 21/07/2020.
//
//

#include "Node.hpp"

/*Constructors*/
Node::Node(char id): ID(id)
{
  //Lauch exceptions
}
Node::Node (const Node &n)
{
  this->ID=n.ID;
  this->tm=n.tm;
  this->payload=n.payload;
}
/*Getters and setters*/
char Node::getID()const
{
  return this->ID;
}
char Node::getTm()const
{
  return this->tm;
}
string Node::getPayload()const
{
  return this->payload;
}
vector<vector<char>> Node::getGrayList()const
{
  return this->graylist;
}
void Node::setID(char id)
{
  this->ID=id;
}
void Node::setTm(char type)
{
  this->tm=type;
}
void Node::setPayload(string Payload)
{
  this->payload=Payload;
}
/*Functions*/
int Node::getSizeGrayList()const
{
  int n;
  n=this->graylist.size();
  return n;
}
void Node::Pack(char type)
{
  if (type=='0')
  {
    this->payload="21.6";
  }
  if(type=='1')
  {

  }
  if(type=='2')
  {

  }
  if(type=='3')
  {

  }
}

void Node::Unpack(char type,string payload)
{
  if (type=='0')
  {
      this->payload = payload;
  }
  if(type=='1')
  {

  }
  if(type=='2')
  {

  }
  if(type=='3')
  {

  }
}
/*Queue Functions*/
Queue Node::createQueue(char id)
{
  Queue new_queue;
  new_queue.start=-1;
  new_queue.end=0;
  new_queue.ID=id;
  return new_queue;
}
int Node::isFull(Queue *q)
{
  if(q->end==10)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
int Node::isVoid(Queue *q)
{
  if(q->end==0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
void Pull(Queue *q)
{
  int i,rssi;
  rssi = q->RSSI[0];
  if(q->end!=0)
  {
    for (i=0;i<q->end-1;i++)
    {
      q->RSSI[i]=q->RSSI[i+1];
    }
    q->end--;
  }
}
void Node::Push(Queue *q,int rssi)
{
  if(isFull(q)!=1)
  {
    q->RSSI[q->end]=rssi;
    q->end++;
  }
}
void Node::PrintQueue(Queue *q)
{
  int i;
  cout<<"Size"<<q->end<<endl;
  for (i=0;i<q->end;i++)
  {
    cout<<"RSSI : "<<  q->RSSI[i]<<endl;
    cout<<"---------------------------"<<endl;
  }
}
/*Storage Data*/
bool Node::SearchID(char id)
{
  int i;
  if (this->received_mjs.size()!=0)
  {
    for (i=0;i<this->received_mjs.size();i++)
    {
      if (this->received_mjs.at(i).ID==id)
      {
        return true;
      }
    }
  }
  else
  {
    return false;
  }
  return false;
}

void Node::addIDQueue(char id)
{
  Queue new_queue;
  new_queue=createQueue(id);
  this->received_mjs.push_back(new_queue);
}
void Node::addRSSI(char id,int rssi)
{
  int i;
  for (i=0;i<this->received_mjs.size();i++)
  {
    if (id == this->received_mjs.at(i).ID)
    {
      Push(&this->received_mjs.at(i),rssi);
      //PrintQueue(&this->received_mjs.at(i));
    }
  }
}
bool Node::QueueFull(char id)
{
  bool ans;
  int i;
  for (i=0;i<this->received_mjs.size();i++)
  {
    if (id == this->received_mjs.at(i).ID)
    {
      ans=isFull(&this->received_mjs.at(i));
    }
  }
  return ans;
}
void Node::RemoveRSSI(char id)
{
  int i;
  int j,rssi;
  for (i=0;i<this->received_mjs.size();i++)
  {
    if (id == this->received_mjs.at(i).ID)
    {
      rssi = this->received_mjs.at(i).RSSI[0];
      if(this->received_mjs.at(i).end!=0)
      {
        for (j=0;j<this->received_mjs.at(i).end-1;j++)
        {
          this->received_mjs.at(i).RSSI[j]=this->received_mjs.at(i).RSSI[j+1];
        }
        this->received_mjs.at(i).end--;
      }
      //PrintQueue(&this->received_mjs.at(i));
    }
  }
}
void Node::PrintHisto()
{
  cout<<"History..."<<endl;
  int i=0;
  for(i=0;i<this->received_mjs.size();i++)
  {
    cout<<"ID : "<< this->received_mjs.at(i).ID<<endl;
  }
}
/*RSSI phase 1*/
void Node::calcProm(Queue *q)
{
  int i=0;
  int aux;
  float n=10;
  for(i=0;i<q->end;i++)
  {
    aux+=q->RSSI[i];
  }
  q->prom=aux/n;
}
void Node::calcVar(Queue *q)
{
  int i=0;
  int aux;
  float n = 10;
  for (i=0;i<q->end;i++)
  {
    aux += (q->RSSI[i]-q->prom)*(q->RSSI[i]-q->prom);
  }
  q->var=aux/n;
}
void Node::generateGrayList()
{
  vector <Queue> Id;
  int i=0;
  int j=0;
  int k=0;
  vector<char> suspected;
  int inf,sup;
  for (i=0;i<this->received_mjs.size();i++)
  {
    if (isFull(&this->received_mjs.at(i))==true)
    {
      calcProm(&this->received_mjs.at(i));
      calcVar(&this->received_mjs.at(i));
      Id.push_back(this->received_mjs.at(i));
      cout<<"ID : "<<this->received_mjs.at(i).ID<<": was added"<<endl;
      //cout<<"RSSI var : "<<this->received_mjs.at(i).var<<endl;
    }
  }
  if(Id.size()>=2)
  {
    for (i=0;i<Id.size();i++)
    {
      suspected.clear();
      for (j=0;j<Id.size();j++)
      {
        sup=0;
        inf=0;
        if(Id.at(i).ID!=Id.at(j).ID)
        {
            inf=Id.at(i).prom-Id.at(i).var;
            sup=Id.at(i).prom+Id.at(i).var;
            if (Id.at(j).prom>=(Id.at(i).prom-Id.at(i).var) and Id.at(j).prom<=(Id.at(i).prom+Id.at(i).var))
            {
              if(inGrayList(Id.at(i).ID)!=true)
              {
                cout<<"ID : "<<Id.at(i).ID<<": added to graylist"<<endl;
                suspected.push_back(Id.at(i).ID);
              }
              if(inGrayList(Id.at(j).ID)!=true)
              {
                cout<<"ID : "<<Id.at(j).ID<<": added to graylist"<<endl;
                suspected.push_back(Id.at(j).ID);
              }
            }

        }
      }
      if(suspected.size()>0)
        this->graylist.push_back(suspected);
    }
  }
}
bool Node::inGrayList(char id)
{
  int i=0;
  int j=0;
  for (i=0;i<this->graylist.size();i++)
  {
    for(j=0;j<this->graylist.at(i).size();j++)
    {
      if(this->graylist.at(i).at(j)==id)
        return true;
    }
  }
  return false;
}
/*PoW phase 2*/
void Node::generatePoWs(string num_rand,vector<char> id_suspect)
{
  vector <string > pows;
  string ans;
  int i;
  int i_time,f_time,time;
  for (i=0;i<id_suspect.size();i++)
  {
    cout<<"ID tested : "<<id_suspect.at(i)<<endl;
    i_time = clock();
    ans=PoW(id_suspect.at(i),num_rand,3);
    f_time = clock();
    time = f_time-i_time;
    cout<<"PoW for \t"<<id_suspect.at(i)<<"took \t"<<time<<"\tseconds"<<endl;
    //if ID not in ID_tested
      this->TIMED.push_back(time);
      this->ID_tested.push_back(id_suspect.at(i));
  }
}
string Node::GenerateTarget(int difficulty)
{
  char ctos[difficulty+1];
  int i =0;
  for (i=0;i<difficulty;i++)
  {
    ctos[i]='0';
  }
  ctos[difficulty]='\0';
  string str(ctos);
  return str;
}
string Node::PoW(char id,string num_rand,int difficulty)
{
  /*This is an implementation of proof of work
  using a hash function as SHA256 standard*/
  int i=0;
  string hash,target,input;
  target= GenerateTarget(difficulty);
  input = packtohash(id,num_rand,"");
  do
  {
    i++;
    hash = sha256(input);
    input = packtohash(id,num_rand,hash);
  }while(hash.substr(0,difficulty)!=target);
  cout<<"Pow permformed in :\t "<<i<<": cycles"<<endl;
  return hash;
}
string Node::packtohash(char id,string num_rand,string lasthash)
{
  string tohash;
  tohash = id + num_rand+lasthash;
  return tohash;
}
