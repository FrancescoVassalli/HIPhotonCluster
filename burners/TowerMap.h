/**
 * @TowerMap.h
 * @author Francesco Vassalli <Francesco.Vassalli@colorado.edu>
 * @version 1.0
 *
 * @sort towers and extract topological info 
 */
#ifndef TowerMap_H__
#define TowerMap_H__

#include <vector>
#include <map>
#include <pair>


class ChaseTower;

class TowerMap
{

  public:
    //constructors
  TowerMap(std::vector<ChaseTower> init_towers, ChaseTower MaxTower); 
  inline float getTowerEnergy(std::pair<int,int> position){
  	if (Map.find(position)!=Map.end())
  	{
  		return map[position];
  	}
  	else return -999;
  }

  private:
    std::map<std::pair<int,int>, float> Map;
    int angle2Int(float eta);
    ChaseTower _maxTower;
};



#endif // __TowerMap_H__