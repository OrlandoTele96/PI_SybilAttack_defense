

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
  int i=0;
  int rssi=-35;
  id =0x02;
  int siz;
  if (n.IsinHist(id)==true)
  {
    cout<<"Está"<<endl;
  }
  else
  {
    cout<<"no esta"<<endl;
    n.AddIDtoHist(id);
  }
  n.AddRSSI(id,rssi);
  siz = n.getHistSize();
  cout<<"tam"<<siz<<endl;
  id = 0x02;
  rssi = -40;
  if (n.IsinHist(id)==true)
  {
    cout<<"Está"<<endl;
  }
  else
  {
    cout<<"no esta"<<endl;
    n.AddIDtoHist(id);
  }
  n.AddRSSI(id,rssi);
  siz = n.getHistSize();
  cout<<"tam"<<siz<<endl;
  id = 0x03;
  if (n.IsinHist(id)==true)
  {
    cout<<"Está"<<endl;
  }
  else
  {
    cout<<"no esta"<<endl;
    n.AddIDtoHist(id);
  }
  n.AddRSSI(id,rssi);
  siz = n.getHistSize();
  cout<<"tam"<<siz<<endl;
  return 0;
}
