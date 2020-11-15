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
#include<iostream>
#include <time.h>
//#include "sha256.hpp"
#include "SHA256.hpp"
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
class Node:public SHA256 {
private:
  unsigned char id = 0x00;  //id by default
  unsigned char type = 0x00; // message type by default
  unsigned char id_dst;
  vector<char> payload;
  vector<queue> Hist;
  vector<vector<char>> graylist;
  int r;
  vector<int> pow_t;
  vector<string> pow_sol;
public:
    /*Constructor*/
    Node();
    Node (unsigned char Id,unsigned char tm);
    Node (const Node &n);
    /*Getters & Setters*/
    unsigned char getID()const;
    unsigned char getTm()const;
    vector<char> getPayload()const;
    vector<vector<char>> getGrayList()const;
    unsigned char getIDdst()const;
    void setID(unsigned char id);
    void setTm(unsigned char tm);
    void setPayload(vector<char> p);
    void clearhist();
    void setGrayList(vector<vector <char>> gl);
    void setIDdst(unsigned char dst);
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
    int Discard();
    void removesubset();
    int inGraylist(vector<char> subset);
    /*Phase 2 : PoW*/
    vector<vector <char>> genPoW(vector<char> subset,vector<char> rand_n);
    string ProofOfWork(string input,int dif);
    string toHash(string input,string lhash);
    string GenerateTarget(int difficulty);
    string randNumAdapter(vector<char> randnum);
    vector<char> solvePoW(vector<char> rand_n);
};
#endif /* Node_hpp */
