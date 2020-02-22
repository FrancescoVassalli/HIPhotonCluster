#include "TowerMap.h"
#include "ChaseTower.h"
#include <iostream>

using namespace std;

TowerMap::TowerMap(std::vector<ChaseTower> towers, ChaseTower MaxTower){
  init(towers,MaxTower);
}

void TowerMap::Reset(std::vector<ChaseTower> towers, ChaseTower MaxTower){
  _map.clear();
  init(towers,MaxTower);
}

TowerMap::~TowerMap(){if(_maxTower) delete _maxTower;}

void TowerMap::init(std::vector<ChaseTower> init_towers, ChaseTower MaxTower){
  if (init_towers.size()!=_kNTOWERS)
  {
    cerr<<"Bad tower count\n";
    cout<<"Bad tower count\n";
    has49=false;
  }
  else has49=true;
  for (std::vector<ChaseTower>::iterator iTower = init_towers.begin(); iTower != init_towers.end(); ++iTower)
  {
    _map[pair<int,int>(angle2Int(iTower->getEta()),angle2Int(iTower->getPhi()))] = iTower->getEnergy();
  }
  if(_maxTower) delete _maxTower;
  _maxTower = new ChaseTower(MaxTower);

}

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
