//
//  Node.hpp
//  PI_SybilAttack_defense
//
//  Created by <Jorge O. Gonzalez> on 21/07/2020.
//
//
/*------------------------------------------------------------------------------
                                    Node
--------------------------------------------------------------------------------
Atributes :
            * ID
            * Type of Msj [0,1,2,3]
            * Generic message
*/
#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Received
{
  int RSSI[10];
  char ID[10];
  int start,end;
};
typedef struct Received Queue;
class Node {
private:
  /* message type 0*/
  char ID = '0';
  char tm = '0';
  string payload= "24.5"; // Temperature of a sensor.
  Queue queue_received;
  vector<char> graylist;
  //vector<char>
public:
    /*Constructors*/
    Node()=default;
    //~Node();
    Node (char id);
    Node (const Node &n);
    /*Getters and setters*/
    char getID()const;
    char getTm()const;
    string getPayload()const;
    void setID(char id);
    void setTm(char type);
    void setPayload(string Payload);
    /*Functions*/
    void Pack(char type);
    void Unpack(char type,string payload);
    /*Queue functions*/
    void CreateQueue();
    int IsFull();
    int IsVoid();
    void Pull();
    void Push(char id,int rssi);
    void PrintQueue();
    /*Phase 1*/
    int CAT_RSSI(int rssi);
    void DiscardRssi();
};


#endif /* Node_hpp */
