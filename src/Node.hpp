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

#include "sha256.hpp"

using namespace std;


struct data
{
  /*
  Estructura de datos para cada ID que se conoce.
  */
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
  unsigned char id = 0x00;  //ID por defecto
  unsigned char type = 0x00; // Tipo de mensaje por defecto (cualquier dato que envie un dispositivo IoT)
  vector<char> payload; // Payload de longitud variable
  vector<queue> Hist; // Historial de ID conocidos, en el que se colecciona los últimos 10 RSSI de mensajes recibidos.
  vector<vector<char>> graylist; // Lista gris (de ID que se sospecha, pudieran ser sybil).
public:
    /*Constructor*/
    Node()=default; // Constructor por defecto
    Node (unsigned char Id,unsigned char tm); //Constructor con dos argumentos (solo usado en pruebas)
    Node (const Node &n); // Constructor copia (solo usado en pruebas)
    /*Getters & Setters*/
    unsigned char getID()const;
    unsigned char getTm()const;
    vector<char> getPayload()const;
    vector<vector<char>> getGrayList()const;
    void setID(unsigned char id);
    void setTm(unsigned char tm);
    void setPayload(vector<char> p);
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
    void computeProm(queue *q);
    void computeVar(queue *q);
    void computeDesv(queue *q);
    int Discard();
    int inGraylist(char id);
    void clearhist();
    /*Phase 2 : PoW*/
    void genPoW(string num,vector<char> id_suspect);
    string makeTarget(int difficultu);
    string PoW(char id,string n_rand,int difficulty);
    string packtoHash(char id,string num_rand,string lasthash);
};
#endif /* Node_hpp */
