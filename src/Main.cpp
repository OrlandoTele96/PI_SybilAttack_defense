

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
  int tam;
  int isgl;
  //[-44, -43, -29, -33, -33, -19, -20, -21, -32, -20] 2
  //[-44, -57, -47, -41, -21, -58, -60, -35, -58, -32] 6

  n.AddIDtoHist('2');
  n.AddIDtoHist('4');
  n.AddIDtoHist('6');
  n.AddRSSI('2',-44);
  n.AddRSSI('2',-43);
  n.AddRSSI('2',-29);
  n.AddRSSI('2',-33);
  n.AddRSSI('2',-33);
  n.AddRSSI('2',-19);
  n.AddRSSI('2',-20);
  n.AddRSSI('2',-21);
  n.AddRSSI('2',-32);
  n.AddRSSI('2',-20);
  n.AddRSSI('6',-44);
  n.AddRSSI('6',-57);
  n.AddRSSI('6',-47);
  n.AddRSSI('6',-41);
  n.AddRSSI('6',-21);
  n.AddRSSI('6',-58);
  n.AddRSSI('6',-60);
  n.AddRSSI('6',-35);
  n.AddRSSI('6',-58);
  n.AddRSSI('6',-32);
  // [-31, -45, -52, -33, -24, -41, -59, -34, -30, -24] 4
  n.AddRSSI('4',-31);
  n.AddRSSI('4',-45);
  n.AddRSSI('4',-52);
  n.AddRSSI('4',-33);
  n.AddRSSI('4',-24);
  n.AddRSSI('4',-41);
  n.AddRSSI('4',-59);
  n.AddRSSI('4',-34);
  n.AddRSSI('4',-30);
  n.AddRSSI('4',-24);
  isgl= n.Discard();
  cout<<isgl<<endl;
  if (isgl ==1)
  {

    gl = n.getGrayList();
    tam =gl.size();
    for(j=0;j<tam;j++) // Recorremos la lista gris y generamos una Pow para cada subconjunto
    {
        //i++;
        //genPoW
        //packtomsg
        //send
        //remove subset
        //n.removesubset();
        cout<<"subconjunto"<<endl;
        for (k=0;k<gl.at(j).size();k++)
        {

          cout<<"ID"<<gl.at(j).at(k)<<endl;
        }
        //gl = n.getGrayList();
        //Serial.println("Size : "+String(gl.size()));
    }
  }
  /*for (i=0;i<100;i++)
  {
    cout<<"ID"<<endl;
    cin>>id;
    cout<<"RSSI"<<endl;
    cin>>rssi;
    int a =n.IsinHist(id);
    if (a==1)
    {
      if(n.isQueueFull(id)==true)
      {
        int r = n.RemoveRSSI(id);
      }
    }
    else
    {
      n.AddIDtoHist(id);
    }
    n.AddRSSI(id,rssi);
    isgl= n.Discard();
    if (isgl ==1)
    {
      gl = n.getGrayList();
      tam =gl.size();
      for(j=0;j<tam;j++) // Recorremos la lista gris y generamos una Pow para cada subconjunto
      {
          //i++;
          //genPoW
          //packtomsg
          //send
          //remove subset
          //n.removesubset();
          for (k=0;k<gl.at(j).size();k++)
          {
            cout<<"ID"<<gl.at(j).at(k)<<endl;
          }
          //gl = n.getGrayList();
          //Serial.println("Size : "+String(gl.size()));
      }
    }

  }*/
  return 0;
}
