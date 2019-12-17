#include "TowerMap.h"
#include "ChaseTower.h"

using namespace std;

TowerMap::TowerMap(std::vector<ChaseTower> towers, ChaseTower MaxTower){
  for (std::vector<ChaseTower>::iterator iTower = towers.begin(); iTower != towers.end(); ++iTower)
  {
    Map[pair<angle2Int(iTower->getEta()),angle2Int(iTower->getPhi())>] = iTower->getEnergy();
  }
  _maxTower = MaxTower;
}


int TowerMap::angle2Int(float eta){
  if (eta<=-.075)
  {
    return 0;
  }
  else if (eta<=-.05)
    {
      return 1;
    }
  else if (eta<=-.025)
      {
        return 2;
      }
  else if (eta<=0)
      {
        return 3;
      }
  else if(eta<=.025)
  {
    return 4;
  }
  else if(eta<=.05){
    return 5;
  }
  else if(eta<=.075){
    return 6;
  }
  else return 7;
}