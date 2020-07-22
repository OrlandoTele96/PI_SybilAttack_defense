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
vector<char>  Node::getReceived_id()const
{
  return this->received_id;
}
vector<char>  Node::getFake()const
{
  return this->Fake;
}
vector<int>  Node::getRSSI()const
{
  return this->RSSI;
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
void storeRSSI(char src_id,int rssi)
{
  this->received_id(src_id);
  this->RSSI.append(rssi);
}
