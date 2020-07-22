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
void Node::CreateQueue()
{
  this->queue_received.start=-1;
  this->queue_received.end=0;
}
int Node::IsFull()
{
  if(this->queue_received.end==10)
    return 1;
  else
    return 0;
}
int Node::IsVoid()
{
  if(this->queue_received.end==0)
    return 1;
  else
    return 0;
}
void Node::Pull()
{
  int rssi,i;
  char id;
  rssi = this->queue_received.RSSI[0];
  id = this->queue_received.ID[0];
  for (i=0;i<this->queue_received.end-1;i++)
  {
    this->queue_received.RSSI[i]=this->queue_received.RSSI[i+1];
    this->queue_received.ID[i]=this->queue_received.ID[i+1];

  }
  this->queue_received.end--;
}
void Node::Push(char id,int rssi)
{
  if (IsFull()!=1)
  {
    this->queue_received.RSSI[this->queue_received.end]=rssi;
    this->queue_received.ID[this->queue_received.end]=id;
    this->queue_received.end++;
  }
}
void Node::PrintQueue()
{
  int i;
  cout<<"Size"<<this->queue_received.end<<endl;
  for (i=0;i<this->queue_received.end;i++)
  {
    cout<<"ID : "<<  this->queue_received.ID[i]<<endl;
    cout<<"RSSI : "<<  this->queue_received.RSSI[i]<<endl;
    cout<<"---------------------------"<<endl;
  }
}
