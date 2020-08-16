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
  char ID;
  int RSSI[10];
  int start,end;
};
typedef struct Received Queue;
class Node {
private:
  /* message type 0*/
  char ID = '0';
  char tm = '0';
  string payload= "24.5"; // Temperature of a sensor.
  vector <Queue> received_mjs;
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
    Queue createQueue(char id);
    int isFull(Queue *q);
    int isVoid(Queue *q);
    void Pull(Queue *q);
    void Push(Queue *q,int rssi);
    void PrintQueue(Queue *q);
    /*Storage data*/
    bool SearchID(char id);
    void addIDQueue(char id);
    void addRSSI(char id,int rssi);
    bool QueueFull(char id);
    void RemoveRSSI(char id);
    void PrintHisto();
    /*RSSI phase 1*/
};
#endif /* Node_hpp */
