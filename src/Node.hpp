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

class Node {
private:
  unsigned char id = 0x00;  //id by default
  unsigned char type = 0x00; // message type by default
  vector<char> payload;
public:
    Node()=default;
    Node (unsigned char Id,unsigned char tm);
    Node (const Node &n);
};
#endif /* Node_hpp */
