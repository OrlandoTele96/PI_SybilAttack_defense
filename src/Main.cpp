

#include <iostream>
#include<string>
#include "Node.hpp"

using namespace std;

int main()
{
  Node n;
  // in arduino...
  unsigned char id =0x01;
  unsigned char tm = 0x00;
  string temperature = "26";
  vector<char> payload;
  n.setID(id);
  n.setTm(tm);
  cout<<temperature[0]<<endl;
  payload.push_back(temperature[0]);
  cout<<payload.at(0)<<endl;
  n.AddIDtoHist(0x02);
  payload.push_back('1');
  payload.push_back('2');
  payload.push_back('3');
  cout<<payload.size()<<endl;
  cout<<payload.at(0)<<endl;
  payload.erase(payload.begin());
  cout<<payload.size()<<endl;
  cout<<payload.at(0)<<endl;
  return 0;
}
