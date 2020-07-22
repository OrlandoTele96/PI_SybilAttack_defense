//
//  Sybil.cpp
//  PI_SybilAttack_defense
//
//  Created by <author> on 21/07/2020.
//
//

#include "Sybil.hpp"


/*Constructors*/
Sybil::Sybil(vector<Node> sn,int n):sybil_id(sn),N(n)
{
  //Exception
}
/*Getters*/
vector<Node> Sybil::getSybil_Nodes()const
{
  return this->sybil_id;
}
int Sybil::getN()const
{
  return this->N;
}
/*Setters*/
void Sybil::setSybilNodes(vector<Node> sn){
  this->sybil_id=sn;
}
void Sybil::setN(int n){
  this->N=n;
}
