//
//  Node.cpp
//  PI_SybilAttack_defense
//
//  Created by <Jorge O. Gonzalez> on 05/09/2020.
//
//
/*
--------------------------------------------------------------------------------
                            Class Node implementation
--------------------------------------------------------------------------------
*/
#include "Node.hpp"
/*------------------------------Constructors----------------------------------*/
Node::Node() : SHA256()
{

}
Node::Node(unsigned char Id,unsigned char tm):id(Id),type(tm),SHA256()
{
  //
}
Node::Node(const Node &n) : SHA256()
{
  this->id = n.id;
  this->type = n.type;
}
/*----------------------------Getter & Setter---------------------------------*/
unsigned char Node::getID()const
{
  return this->id;
}
unsigned char Node::getTm()const
{
  return this->type;
}
vector<char> Node::getPayload()const
{
  return this->payload;
}
vector<vector<char>> Node::getGrayList()const
{
  return this->graylist;
}
unsigned char Node::getIDdst()const
{
  return this->id_dst;
}
vector<char> Node::getBlackList()const
{
  return this->blacklist;
}
void Node::setID(unsigned char id)
{
  this->id = id;
}
void Node::setTm(unsigned char tm)
{
  this->type = tm;
}
void Node::setPayload(vector<char> p)
{
  this->payload = p;
}
void Node::clearhist(){
  this->Hist.clear();
}
void Node::setGrayList(vector<vector <char>> gl)
{
  this->graylist = gl;
}
void Node::setIDdst(unsigned char dst)
{
  this->id_dst = dst;
}
/*-----------------------------Queue function---------------------------------*/
queue Node::create(unsigned char id)
{
  queue q;
  q.start = -1;
  q.end = 0;
  q.ID = id;
  return q;
}
bool Node::isQueueFull(unsigned char id)
{
  bool ans;
  int i;
  for (i=0;i<this->Hist.size();i++)
  {
    if (id == this->Hist.at(i).ID)
    {
      if (this->Hist.at(i).end ==10)
      {
        ans = true;
      }
      else
      {
        ans = false;
      }
    }
  }
  return ans;
}
int Node::RemoveRSSI(unsigned char id)
{
  int i;
  int j,rssi;
  for (i=0;i<this->Hist.size();i++)
  {
    if (id == this->Hist.at(i).ID)
    {
      rssi = this->Hist.at(i).RSSI[0];
      if(this->Hist.at(i).end>0)
      {
        for (j=0;j<this->Hist.at(i).end-1;j++)
        {
          this->Hist.at(i).RSSI[j]= this->Hist.at(i).RSSI[j+1];
        }
        this->Hist.at(i).end--;
      }
    }
  }
  return rssi;
}
void Node::AddRSSI(unsigned char id,int rssi)
{
  int i;
  for (i=0;i<this->Hist.size();i++)
  {
    if (id == this->Hist.at(i).ID)
    {
      if(this->Hist.at(i).end<=10)
      {
        this->Hist.at(i).RSSI[this->Hist.at(i).end]=rssi;
        this->Hist.at(i).end++;
      }
    }
  }
}
/*-----------------------------RSSI Storage-----------------------------------*/
void Node::AddIDtoHist(unsigned char id)
{
  queue q;
  q= create(id);
  this->Hist.push_back(q);
}
int Node::IsinHist(unsigned char id)
{
  int i=0;
  int ans=0;
  if(this->Hist.size()>0)
  {

    for(i=0;i<this->Hist.size();i++)
    {
      if(this->Hist.at(i).ID==id)
      {
        ans = 1;
      }
    }
  }
  return ans;
}
int Node::getHistSize()
{
  return this->Hist.size();
}
/*-----------------------------Phase 1 : RSSI---------------------------------*/
int Node::Discard()
{
    int i,j;
    vector<queue> id_test;
    vector<char> suspected;
    int sup,inf;
    int ans;
    int resp;
    int sump,sumdesv,var;
    for (i=0;i<this->Hist.size();i++)
    {
      if (this->Hist.at(i).end>9)
      {
        sump =0;
        sumdesv=0;
        var=0;
        for (j=0;j<Hist.at(i).end;j++)
        {
          sump = sump + this->Hist.at(i).RSSI[j];
        }
        this->Hist.at(i).prom = sump/10;
        for (j=0;j<Hist.at(i).end;j++)
        {
          sumdesv= sumdesv + ((this->Hist.at(i).RSSI[j]-this->Hist.at(i).prom)*(this->Hist.at(i).RSSI[j]-this->Hist.at(i).prom));
        }
        var = sumdesv/10;
        this->Hist.at(i).desv = sqrt(var);
        id_test.push_back(this->Hist.at(i));
      }
    }
    if(id_test.size()>1)
    {
      for(i=0;i<id_test.size();i++)
      {
        suspected.clear();
        for(j=0; j<id_test.size();j++)
        {
          if(id_test.at(i).ID!=id_test.at(j).ID)
          {
            inf = id_test.at(i).prom-(3*(id_test.at(i).desv));
            sup =  id_test.at(i).prom+(3*(id_test.at(i).desv));
            if(id_test.at(j).prom>inf && id_test.at(j).prom<sup)
            {
              suspected.push_back(id_test.at(j).ID);
            }
          }
        }
        if(suspected.size()>0){
          suspected.push_back(id_test.at(i).ID);
          if(inGraylist(suspected)!=1)
          {
            this->graylist.push_back(suspected);
          }

        }

      }
    }
    if(this->graylist.size()>0)
    {
      ans=1;
    }
    else{
      ans=0;
    }
    return ans;
}
void Node::removesubset()
{
  this->graylist.clear();
}
int Node::inGraylist(vector<char> subset)
{
  /*This function return 1 if a subset is already in graylist,
  return 0 if it is not in graylist*/
  int ans =0;
  int i=0;
  int j=0;
  int k =0;
  int counter;
  for (i=0;i<this->graylist.size();i++)
  {
      if(this->graylist.at(i).size()==subset.size())
      {
        counter =0;
        for (j=0;j<subset.size();j++)
        {
          for (k=0;k<this->graylist.at(i).size();k++)
          {
            if(subset.at(j)==this->graylist.at(i).at(k))
            {
              counter ++;
            }
          }
        }
        if(counter==subset.size())
        {
          //cout<<counter<<endl;
          ans =1;
        }
      }
  }
  return ans;
}
/*---------------------------Phase 2 : PoW------------------------------------*/
void Node::genPoW(vector<char> subset,vector<char> rand_n)
{
  int i,j;
  string number ="";
  string input="";
  string solution;
  int i_t,f_t,t_pow;
  vector<vector <char>> solutions;
  vector<string> pow_solutions;
  vector<int> pow_time;
  number = randNumAdapter(rand_n);
  for (i=0;i<subset.size();i++)
  {
    input = number + subset.at(i);
    i_t = clock();
    solution=ProofOfWork(input,2);
    f_t = clock();
    t_pow = f_t-i_t;
    cout<<"mined time"<<t_pow<<endl;
    cout<<input<<endl;
    solution=solution.substr(0,32);
    cout<<"Mined hash : "<<solution<<endl;
    pow_time.push_back(t_pow);
    pow_solutions.push_back(solution);
    //this->id_tested.push_back(subset.at(i));
    //vector<char> s (solution.begin(),solution.end());
    //Convert solution in vector<char> format
    //solutions.push_back(s);
  }
  this->pow.push_back(pow_solutions);
  this->pow_ti.push_back(pow_time);
  this->id_tested.push_back(subset);
}
string Node::ProofOfWork(string input,int dif)
{
  string to_hash;
  string target;
  string hash="";
  //string solution;
  to_hash = toHash(input,hash);
  target = GenerateTarget(dif)+"b";
  do
  {
    hash = sha256(to_hash);
    to_hash = toHash(to_hash,hash);
  }while(hash.substr(0,dif+1)!=target);
  //cout<<"Mined"<<endl;
  return hash;
}
string Node::toHash(string input,string lhash)
{
  string to_hash;
  string key = "#Telecom";
  //input.substr(0,3)+
  to_hash = lhash.substr(0,3)+input;
  return to_hash;
}
string Node::GenerateTarget(int difficulty)
{
  char ctos[difficulty+1];
  int i =0;
  for (i=0;i<difficulty;i++)
  {
    ctos[i]='0';
  }
  ctos[difficulty]='\0';
  string str(ctos);
  return str;
}

string Node::randNumAdapter(vector<char> randnum)
{
  string number = "";
  int i;
  for(i=0;i<randnum.size();i++)
  {
    number = number + randnum.at(i);
  }
  return number;
}

vector<char> Node::solvePoW(vector<char> rand_n)
{
  string number,input,sol;

  char tested;
  tested = getID();
  number = randNumAdapter(rand_n);
  input = number + tested;
  cout<<"input"<<input<<endl;
  sol = ProofOfWork(input,2);
  sol =sol.substr(0,32);
  cout<<"solution"<<sol<<endl;
  vector<char> s (sol.begin(),sol.end());
  return s;
}
void Node::AddAnswer(vector<char> ans)
{
  /*String adapter*/
  int i;
  string solution="";
  for (i=0;i<ans.size();i++)
  {
    solution = solution+ans.at(i);
  }
  this->pow_ans.push_back(solution);
}
void Node::AddPowTime(int pow_t)
{
  this->pow_tf.push_back(pow_t);
}
int Node::SybilDetection()
{
  /*int thresh_t;
  int i,j,k;
  int inans;
  int sup,inf;
  thresh_t = 10000;
      int tam;
      tam = pow.size();
  if(tam>0)
  {
  for(i=0;i<this->pow.size();i++)
  {
    inans=0;
    sup=this->pow_ti.at(i)+thresh_t;
    inf=this->pow_ti.at(i)-thresh_t;


    for (j=0;j<this->pow_ans.size();j++)
    {
      if(pow.at(i)==pow_ans.at(j))
      {*/
        //check time

        //cout<<"ID :"<<id_tested.at(i)<<"answered"<<endl;
        /*if(pow_tf.at(j)>=inf && pow_tf.at(j)<=sup)
        {
          inans=1;
        }*/
        /*inans=1;
      }
    }
    if(inans==0)
    {
      //cout<<"ID : "<<id_tested.at(i)<<"never answered"<<endl;
      //Add to blacklist
      this->blacklist.push_back(id_tested.at(i));
    }
  }
    this->pow.clear();
  this->pow_ti.clear();
  this->pow_ans.clear();
  this->pow_tf.clear();
  this->id_tested.clear();
}*/
    return 0;
}

void Node::clearBlackList()
{
  this->blacklist.clear();
}

vector<int> Node::calcThreshold()
{
  int i,j;
  int act,last;
  int threshold;
  vector<int>thresholds;
  for(i=0;i<this->pow_ti.size();i++)
  {
    last=0;
    for(j=0;j<this->pow_ti.at(i).size();j++)
    {
      cout<<this->pow_ti.at(i).at(j)<<endl;
      act =this->pow_ti.at(i).at(j);
      if(act>last)
      {
        last = act;
      }
    }
    threshold = last + 1500+(2500);
    thresholds.push_back(threshold);
  }
  return thresholds;
}
