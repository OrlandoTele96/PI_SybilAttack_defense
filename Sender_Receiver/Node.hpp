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
#include <math.h>

using namespace std;


struct data
{
  unsigned char ID;
  //vector<int> RSSI;
  int RSSI[10];
  int start,end;
  int prom=0;
  int var=0;
  int desv=0;
};
typedef struct data queue;
class Node {
private:
  unsigned char id = 0x00;  //id by default
  unsigned char type = 0x00; // message type by default
  vector<char> payload;
  vector<queue> Hist;
  vector<vector<char>> graylist;
public:
    /*Constructor*/
    Node()=default;
    Node (unsigned char Id,unsigned char tm);
    Node (const Node &n);
    /*Getters & Setters*/
    unsigned char getID()const;
    unsigned char getTm()const;
    vector<char> getPayload()const;
    vector<vector<char>> getGrayList()const;
    void setID(unsigned char id);
    void setTm(unsigned char tm);
    void setPayload(vector<char> p);
    void clearhist();
    /*Queue function*/
    queue create(unsigned char id);
    /*RSSI Storage*/
    void AddIDtoHist(unsigned char id);
    int IsinHist(unsigned char id);
    bool isQueueFull(unsigned char id);
    int RemoveRSSI(unsigned char id);
    void AddRSSI(unsigned char id,int rssi);
    int getHistSize();
    /*Phase 1: RSSI*/
    int getP();
    int getV();
    void computeProm(queue *q);
    void computeVar(queue *q);
    void computeDesv(queue *q);
    int Discard();
    int inGraylist(char id,vector<char> suspected);
    void removesubset();
};
#endif /* Node_hpp */
