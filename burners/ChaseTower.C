#include "ChaseTower.h"

using namespace std;

ChaseTower ChaseTower::findMaxTower(std::vector<ChaseTower> towers){
  ChaseTower MaxTower;
  MaxTower.setKey(0);
  MaxTower.setEnergy(0);
  MaxTower.setPhi(0);
  MaxTower.setEta(0);

  for(unsigned int i = 0; i < towers.size(); i++)
  {
    if(towers.at(i).getEnergy() > MaxTower.getEnergy())
    {
      MaxTower.setKey(towers.at(i).getKey());
      MaxTower.setEnergy(towers.at(i).getEnergy());
      MaxTower.setPhi(towers.at(i).getPhi());
      MaxTower.setEta(towers.at(i).getEta());
    }
  }
  return MaxTower;
}