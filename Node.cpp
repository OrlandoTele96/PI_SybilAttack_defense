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
int Node::getSizeListedNodes()
{
  return 0;
}
void Node::Clear()
{

}
