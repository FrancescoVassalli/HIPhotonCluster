#include "TowerMap.h"
#include "ChaseTower.h"
#include <iostream>

using namespace std;

TowerMap::TowerMap(std::vector<ChaseTower> towers, ChaseTower MaxTower){
  for (std::vector<ChaseTower>::iterator iTower = towers.begin(); iTower != towers.end(); ++iTower)
  {
    _map[pair<int,int>(angle2Int(iTower->getEta()),angle2Int(iTower->getPhi()))] = iTower->getEnergy();
  }
  _maxTower = new ChaseTower(MaxTower);
  std::cout<<"made tower"<<std::endl;
}

void TowerMap::Reset(std::vector<ChaseTower> towers, ChaseTower MaxTower){
  _map.clear();
  if (towers.size()!=_kNTOWERS)
  {
    cerr<<"Bad tower count\n";
    cout<<"Bad tower count\n";
  }
  for (std::vector<ChaseTower>::iterator iTower = towers.begin(); iTower != towers.end(); ++iTower)
  {
    _map[pair<int,int>(angle2Int(iTower->getEta()),angle2Int(iTower->getPhi()))] = iTower->getEnergy();
    if (towers.size()!=_kNTOWERS)
    {
      cout<<iTower->getEta()<<", "<<iTower->getPhi()<<'\n';
    }
  }
  if(_maxTower) delete _maxTower;
  _maxTower = new ChaseTower(MaxTower);
  //std::cout<<"reset map"<<std::endl;
  //cout<<"Max energy check (3,3)= "<<_map[pair<int,int>(3,3)]<<" Max = "<<MaxTower.getEnergy()<< "(2,2) = "<<_map[pair<int,int>(2,2)]<<'\n';
}

TowerMap::~TowerMap(){if(_maxTower) delete _maxTower;}

int TowerMap::angle2Int(float eta){
  //cout<<"mapping angle="<<eta<<'\n';
  if (eta<=-.07)
  {
    return 0;
  }
  else if (eta<=-.04)
  {
    return 1;
  }
  else if (eta<=-.02)
  {
    return 2;
  }
  else if (eta==0)
  {
    return 3;
  }
  else if(eta<.04)
  {
    return 4;
  }
  else if(eta<=.05){
    return 5;
  }
  else if(eta<=.075){
    return 6;
  }
  else return -999;
}
