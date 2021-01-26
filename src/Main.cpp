

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
  id = 0x02;
  int rssi = -40;
  int i=0;
  int j,k;
  int ans;
  int siz;
  vector<vector<char>> gl,solutions;
  vector<char> subset;
  subset.push_back('1');
  subset.push_back('2');
  subset.push_back('3');
  gl.push_back(subset);
  subset.clear();
  subset.push_back('1');
  subset.push_back('2');
  gl.push_back(subset);
  subset.clear();
  n.setGrayList(gl);
  subset.push_back('2');
  subset.push_back('1');
  subset.push_back('3');
  subset.push_back('4');
  if(n.inGraylist(subset)==1)
  {
    cout<<"Ya existe en graylist"<<endl;
  }
  n.setDifficulty(6);
  vector<char> rand = {'1','2','2','3'};
  n.genPoW(subset,rand);
  subset.pop_back();
  rand.pop_back();
  rand.push_back('6');
  n.genPoW(subset,rand);
  n.setID('1');
  n.solvePoW(rand);
  /*n.AddAnswer();
  n.AddPowTime(0);
  n.AddAnswer();
  n.AddPowTime(0);
  n.AddAnswer();
  n.AddPowTime(0);
  n.SybilDetection();*/
  //n.calcThreshold();
  return 0;
}
