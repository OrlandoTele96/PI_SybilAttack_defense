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
  int time_interval;
  int difficulty;
  double fact;
  vector<vector<string>> pow;
  vector<vector<int>> pow_ti;
  vector<vector<char>> id_tested;
  vector<string> pow_ans;
  vector<int> pow_tf;
  vector<char> blacklist;
  vector<char> bl_hashes;
  vector<int> tol;
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
    vector<char> getBlackList()const;
    void setID(unsigned char id);
    void setTm(unsigned char tm);
    void setPayload(vector<char> p);
    void clearhist();
    void setGrayList(vector<vector <char>> gl);
    void setIDdst(unsigned char dst);
    int getTime_interval()const;
    int getDifficulty()const;
    double getFactor()const;
    void setTime_interval(int interval);
    void setDifficulty(int dif);
    void setFactor(double factor);
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
    void genPoW(vector<char> subset,vector<char> rand_n);
    string ProofOfWork(string input,int dif);
    string toHash(string input,string lhash);
    string GenerateTarget(int difficulty);
    string randNumAdapter(vector<char> randnum);
    vector<char> solvePoW(vector<char> rand_n);
    void AddAnswer(vector<char> ans);
    void AddPowTime(int pow_t);
    int SybilDetection();
    void clearBlackList();
    vector<int> calcThreshold();
    void calcTmin();
};
#endif /* Node_hpp */
