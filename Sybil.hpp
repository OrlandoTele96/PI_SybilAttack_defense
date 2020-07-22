//
//  Sybil.hpp
//  PI_SybilAttack_defense
//
//  Created by <author> on 21/07/2020.
//
//

#ifndef Sybil_hpp
#define Sybil_hpp

#include "Node.hpp"
#include <vector>
using namespace std;

class Sybil {
private:
  vector<Node> sybil_id;
  int N=0;
public:
    Sybil()=default;
    Sybil(vector<Node> sn,int n);
    vector<Node> getSybil_Nodes()const;
    int getN()const;
    void setSybilNodes(vector<Node> sn);
    void setN(int n);
};


#endif /* Sybil_hpp */
