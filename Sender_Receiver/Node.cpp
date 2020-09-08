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
  queue q;
  q.start = -1;
  q.end = 0;
  q.ID = id;
  return q;
}
/*RSSI Storage*/
void Node::AddIDtoHist(unsigned char id)
{
  queue q;
  q= create(id);
  this->Hist.push_back(q);
}
int Node::IsinHist(unsigned char id)
{
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
  return this->Hist.size();
}
void Node::clearGL()
{
  this->graylist.clear();
}
/*Phase 1 : RSSI*/
int Node::getP()
{
  return 0;
}
int Node::getV()
{
  return 0;
}
void Node::computeProm(queue *q)
{
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
  int i=0;
  int aux;
  //float n = 10;
  for (i=0;i<q->end;i++)
  {
    aux += (q->RSSI[i]-q->prom)*(q->RSSI[i]-q->prom);
  }
  q->var=aux/10;
}
int Node::Discard()
{
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
        //Add to list
        id_test.push_back(this->Hist.at(i));
      }
    }
    if(id_test.size()>1)
    {
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
            inf = id_test.at(i).prom-id_test.at(i).var;
            sup =  id_test.at(i).prom+id_test.at(i).var;
            if(id_test.at(j).prom>inf && id_test.at(j).prom<sup)
            {
              suspected.push_back(id_test.at(i).ID);
              suspected.push_back(id_test.at(j).ID);
              //ans=1;
            }
          }
        }
        this->graylist.push_back(suspected);
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
int Node::inGraylist(char id)
{
  int i=0;
  int j=0;
  int ans=0;
  for (i=0;i<this->graylist.size();i++)
  {
    for(j=0;j<this->graylist.at(i).size();j++)
    {
      if(this->graylist.at(i).at(j)==id)
        ans=1;
    }
  }
  return ans;
}
