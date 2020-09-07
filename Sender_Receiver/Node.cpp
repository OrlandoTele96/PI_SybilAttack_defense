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
bool Node::IsinHist(unsigned char id)
{
  int i=0;
  if(this->Hist.size()>0)
  {
    for(i=0;i<this->Hist.size();i++)
    {
      if(Hist.at(i).ID==id)
      {
        return true;
      }
    }
  }
  else
  {
    return false;
  }
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
/*Phase 1 : RSSI*/
void Node::computeProm(queue *q)
{
  int i=0;
  int aux;
  float n=10;
  for(i=0;i<q->end;i++)
  {
    aux += q->RSSI[i];
  }
  q->prom = aux/n;
}
void Node::computeVar(queue *q)
{

}
void Node::Discard()
{
    int i;
    vector<queue> id_test;
    for (i=0;i<this->Hist.size();i++)
    {
      if (this->Hist.at(i).end>=10)
      {
        //compute average
        computeProm(&this->Hist.at(i));
        //calcVar
        id_test.push_back(this->Hist.at(i));
      }
    }
}
bool Node::isGraylist()
{

}
