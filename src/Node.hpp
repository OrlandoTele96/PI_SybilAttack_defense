//
//  Node.hpp
//  PI_SybilAttack_defense
//
//  Created by <author> on 05/09/2020.
//
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <vector>

using namespace std;


struct data
{
  unsigned char ID;
  vector<int> RSSI;
  int start,end;
  //int prom=0;
  //int var=0;
};
typedef struct data queue;
class Node {
private:
  unsigned char id = 0x00;  //id by default
  unsigned char type = 0x00; // message type by default
  vector<char> payload;
  vector<queue> Hist;
public:
    /*Constructor*/
    Node()=default;
    Node (unsigned char Id,unsigned char tm);
    Node (const Node &n);
    /*Getters & Setters*/
    unsigned char getID()const;
    unsigned char getTm()const;
    vector<char> getPayload()const;
    void setID(unsigned char id);
    void setTm(unsigned char tm);
    void setPayload(vector<char> p);
    /*Queue function*/
    queue create(unsigned char id);
    /*RSSI Storage*/
    void AddIDtoHist(unsigned char id);
    bool IsinHist(unsigned char id);
    bool isQueueFull(unsigned char id);
};
#endif /* Node_hpp */
