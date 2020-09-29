

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
  vector<vector<char>> gl;
  /*for(i=0;i<40;i++){
    cout<<"Id: "<<endl;
    cin>>id;
    cout<<"RSSI"<<endl;
    cin>>rssi;
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
    ans=n.Discard();
    if(ans==1)
    {
      gl = n.getGrayList();
      for(j=0;i<gl.size();j++)
      {
        cout<<"subconjunto :\t"<<j<<endl;
        for(k=0;k<gl.at(i).size();k++)
        {
          cout<<"ID : \t"<<gl.at(i).at(j)<<"\t"<<endl;
        }
      }
    }
  }*/
  gl = n.getGrayList();
  for(i=0;i<gl.size();i++)
  {
    cout<<"Subset : \t"<<i<<endl;
    for(j=0;j<gl.at(i).size();j++)
    {
      cout<<"ID:\t"<<gl.at(i).at(j)<<endl;
    }
  }
  n.removesubset();
  gl = n.getGrayList();
  cout<<"Gray List:"<<gl.size()<<endl;
  gl = n.getGrayList();
  for(i=0;i<gl.size();i++)
  {
    cout<<"Subset : \t"<<i<<endl;
    for(j=0;j<gl.at(i).size();j++)
    {
      cout<<"ID:\t"<<gl.at(i).at(j)<<endl;
    }
  }
  n.removesubset();
  gl = n.getGrayList();
  cout<<"Gray List:"<<gl.size()<<endl;
  gl = n.getGrayList();
  for(i=0;i<gl.size();i++)
  {
    cout<<"Subset : \t"<<i<<endl;
    for(j=0;j<gl.at(i).size();j++)
    {
      cout<<"ID:\t"<<gl.at(i).at(j)<<endl;
    }
  }
  n.removesubset();
  gl = n.getGrayList();
  cout<<"Gray List:"<<gl.size()<<endl;
  gl = n.getGrayList();
  for(i=0;i<gl.size();i++)
  {
    cout<<"Subset : \t"<<i<<endl;
    for(j=0;j<gl.at(i).size();j++)
    {
      cout<<"ID:\t"<<gl.at(i).at(j)<<endl;
    }
  }
  return 0;
}
