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
#include <utility>


class ChaseTower;

class TowerMap
{

  public:
    //constructors
    TowerMap(std::vector<ChaseTower> init_towers, ChaseTower MaxTower); 
    void Reset(std::vector<ChaseTower> init_towers, ChaseTower MaxTower); 
    inline float getTowerEnergy(std::pair<int,int> position){
      if (_map.find(position)!=_map.end())
      {
        return _map[position];
      }
      else return -999;
    }
    ~TowerMap();

  private:
    void init(std::vector<ChaseTower> init_towers, ChaseTower MaxTower);
    std::map<std::pair<int,int>, float> _map;
    int angle2Int(float eta);
    ChaseTower* _maxTower=NULL;
    const unsigned _kNTOWERS=49;
    bool has49;
};



#endif // __TowerMap_H__
