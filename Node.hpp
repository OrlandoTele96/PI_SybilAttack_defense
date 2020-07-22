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

class Node {
private:
  /* message type 0*/
  char ID = '0';
  char tm = '0';
  string payload= "24.5"; // Temperature of a sensor.
  vector<int> RSSI;
  vector<char> received_id;
  vector<char> Fake;
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
    vector<char> getReceived_id()const;
    vector<char> getFake()const;
    vector<int> getRSSI()const;
    void setID(char id);
    void setTm(char type);
    void setPayload(string Payload);
    /*Functions*/
    void storeRSSI(char src_id,int rssi);
    int getSizeListedNodes();
};


#endif /* Node_hpp */
