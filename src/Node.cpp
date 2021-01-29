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
int Node::getTime_interval()const
{
  return this->time_interval;
}
int Node::getDifficulty()const
{
  return this->difficulty;
}
double Node::getFactor()const
{
  return this->fact;
}
void Node::setTime_interval(int interval)
{
  this->time_interval = interval;
}
void Node::setDifficulty(int dif)
{
  this->difficulty = dif;
}
void Node::setFactor(double factor)
{
  this->fact = factor;
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
            inf = id_test.at(i).prom-(this->fact*(id_test.at(i).desv));
            sup =  id_test.at(i).prom+(this->fact*(id_test.at(i).desv));
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
    solution=ProofOfWork(input,this->difficulty);
    f_t = clock();
    t_pow = f_t-i_t;
    solution=solution.substr(0,32);
    cout<<subset.at(i)<<endl;
    cout<<solution<<endl;
    pow_time.push_back(t_pow);
    pow_solutions.push_back(solution);
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
  target = GenerateTarget(dif);
  do
  {
    hash = sha256(to_hash);
    to_hash = toHash(to_hash,hash);
  }while(hash.substr(0,dif)!=target);
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
  sol = ProofOfWork(input,this->difficulty);
  sol =sol.substr(0,32);
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
  int i,j;
  int tam,tamsol;
  int issybil;
  vector<string> solutions;
  vector<char> id;
  vector<int> pow_time;
  vector<int> tol;
  int sup,inf;
  int T;
  solutions = this->pow.back();
  pow_time = this->pow_ti.back();
  id = this->id_tested.back();
  tam = solutions.size();
  tamsol = this->pow_ans.size();
  int ans=0;
  if(tam>0)
  {
    cout<<"**--subset--**"<<endl;
    T = this->tol.back();
    for (i=0;i<tam;i++)
    {

      issybil=1;
      cout<<id.at(i);
      sup = pow_time.at(i) + T;
      for(j=0;j<tamsol;j++)
      {
        if(solutions.at(i)==this->pow_ans.at(j))
        {
          cout<<"timed"<<this->pow_tf.at(j)<<","<<sup<<endl;
          if(this->pow_tf.at(j) <= sup)
          {
            issybil = 0;
          }
        }
      }
      if(issybil==1)
      {
        this->blacklist.push_back(id.at(i));
        ans =1;
      }

    }
    cout<<"--"<<endl;
  }
  this->pow.pop_back();
  this->pow_ti.pop_back();
  this->id_tested.pop_back();
  this->tol.pop_back();
  this->pow_ans.clear();
  this->pow_tf.clear();
  return ans;
}

void Node::clearBlackList()
{
  this->blacklist.clear();
}

vector<int> Node::calcThreshold()
{
  int i,j;
  int act,last,men;
  int threshold;
  vector<int>thresholds;
  for(i=0;i<this->pow_ti.size();i++)
  {
    last=0;
    for(j=0;j<this->pow_ti.at(i).size();j++)
    {
      act =this->pow_ti.at(i).at(j);
      if(act>last)
      {
        last = act;
      }
    }
    threshold = last +10;
    thresholds.push_back(threshold);
  }
  return thresholds;
}

void Node::calcTmin()
{
  int i,j;
  int tmin,act,tole;
  for(i=0;i<this->pow_ti.size();i++)
  {
    tmin =0;
    for(j=0;j<this->pow_ti.at(i).size();j++)
    {
      if(j==0)
      {
        tmin = this->pow_ti.at(i).at(j);
      }
      else{
        act = this->pow_ti.at(i).at(j);
        if(tmin>act)
        {
          tmin = act;
        }
      }
    }

    tole =(this->pow_t+40+tmin)*1;//300 -> dif 2, 20000 -> dif 3
    this->tol.push_back(tole);
  }
}

void Node::setPoW_t()
{
  int powtime;
  int factor;
  if(this->difficulty==2)
  {
    powtime = 100;
  }
  else{
    powtime = 2000;
  }
  if(this->fact==0.25 || this->fact ==0.5)
  {
    factor = 3;
  }
  else{
    factor = 4;
  }
  this->pow_t = powtime*factor;
  cout<<this->pow_t<<endl;
}
/*---------------------------Phase 3 : PoW------------------------------------*/
void Node::HonestList()
{
  int i,j,inbl;
  for(i=0;i<this->Hist.size();i++)
  {
    cout<<Hist.at(i).ID<<endl;
    inbl=0;
    for (j=0;j<this->blacklist.size();j++)
    {
      if(this->Hist.at(i).ID == this->blacklist.at(j))
      {
        inbl =1;
      }
    }
    if(inbl==0)
    {
      cout<<"This ID was honest"<<endl;
      this->honest.push_back(this->Hist.at(i).ID);
    }
  }
}
void Node::Consensus(vector<char> bl)
{
  //
}
