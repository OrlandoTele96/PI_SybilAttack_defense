//
//  Node.cpp
//  PI_SybilAttack_defense
//
//  Created by <author> on 05/09/2020.
//
//

#include "Node.hpp"

Node::Node(unsigned char Id,unsigned char tm):id(Id),type(tm)
{
  //
}
Node::Node(const Node &n)
{
  this->id = n.id;
  this->type = n.type;
}

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
