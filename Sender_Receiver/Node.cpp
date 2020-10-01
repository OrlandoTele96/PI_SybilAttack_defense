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
/*Phase 1 : RSSI*/
int Node::getP(unsigned char id)
{
  int i;
  for (i=0;i<this->Hist.size();i++)
  {
    if(this->Hist.at(i).ID==id)
    {
      return this->Hist.at(i).prom;
    }
  }
}
int Node::getV(unsigned char id)
{
    int i;
  for (i=0;i<this->Hist.size();i++)
  {
    if(this->Hist.at(i).ID==id)
    {
      return this->Hist.at(i).desv;
    }
  }
}
void Node::computeProm(queue *q)
{
  int i=0;
  int aux;
  float n=0;
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
void Node::computeDesv(queue *q)
{
  q->desv=sqrt(q->var);
}
int Node::Discard()
{
    int i,j;
    vector<queue> id_test;
    vector<char> suspected;
    int sup,inf;
    int ans;
    int resp;
    int sump,sumdesv,var;
    for (i=0;i<this->Hist.size();i++)
    {
      if (this->Hist.at(i).end>9)
      {
        sump =0;
        sumdesv=0;
        var=0;
        //compute average
        //computeProm(&this->Hist.at(i));
        //calcVar
        //computeVar(&this->Hist.at(i));
        //calcDesv
        //computeDesv(&this->Hist.at(i));
        //Add to list
        for (j=0;j<Hist.at(i).end;j++)
        {
          sump = sump + this->Hist.at(i).RSSI[j];
        }
        this->Hist.at(i).prom = sump/10;
        for (j=0;j<Hist.at(i).end;j++)
        {
          sumdesv= (this->Hist.at(i).RSSI[j]-this->Hist.at(i).prom)*(this->Hist.at(i).RSSI[j]-this->Hist.at(i).prom);
        }
        var = sumdesv/10;
        this->Hist.at(i).desv = sqrt(var);;
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
          //inf = 0;
          //sup = 0;
          if(id_test.at(i).ID!=id_test.at(j).ID)
          {
            inf = id_test.at(i).prom-(2*(id_test.at(i).desv));
            sup =  id_test.at(i).prom+(2*(id_test.at(i).desv));
            if(id_test.at(j).prom>inf && id_test.at(j).prom<sup)
            {
              //resp = inGraylist(id_test.at(i).ID,suspected);
              //if(resp!=1)
              //{
                //suspected.push_back(id_test.at(i).ID);
              //}
              suspected.push_back(id_test.at(j).ID);
            }
          }
        }
        if(suspected.size()>0){
          suspected.push_back(id_test.at(i).ID);
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
int Node::inGraylist(char id,vector<char> suspected)
{
  int i=0;
  int ans=0;
  /*for(i=0;i<suspected.size();i++)
  {
    if(suspected.at(i)==id)
    {
      ans=1;
    }
  }*/
  return ans;
}

void Node::removesubset()
{
  this->graylist.clear();
}
