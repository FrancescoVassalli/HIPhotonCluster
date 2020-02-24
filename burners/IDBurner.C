#include "IDBurner.h"
#include "TowerMap.h"
#include "ChaseTower.h"

#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawCluster.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTowerContainer.h>
#include <jetbackground/TowerBackground.h>
#include <phool/getClass.h>

#include <TLorentzVector.h>

#include <iostream>
#include <map>
#include <utility>

using namespace std;
//This class is a Functor to asses if the current tower should be in the 49 tower group
class EvaluateBinIn49{
  public:
    EvaluateBinIn49(pair<int,int> maxBins){
      this->maxBins = maxBins;
      //is it trivial?
      firstWrap = maxBins.first <3 || maxBins.first>253;
      secondWrap = maxBins.second <3|| maxBins.first>93;
    }
    bool operator()(pair<int,int> thisBins){
      return firstWrapCheck(thisBins) and secondWrapCheck(thisBins);
    }
    pair<int,int> getMapPosition(pair<int,int> thisBins){
      pair<int,int> rpair (maxBins.first-thisBins.first,maxBins.second-thisBins.second);
      if(firstWrap&&abs(rpair.first)>3){
        if(maxBins.first>93)
          thisBins.first +=96; 
        else
          thisBins.first-=96;
        rpair.first=maxBins.first-thisBins.first;
      }
      if(secondWrap&&abs(rpair.second)>3){
        if(maxBins.second>253)
          thisBins.second +=256; 
        else
          thisBins.second-=256;
        rpair.second=maxBins.second-thisBins.second;
      }
      rpair.first+=3;
      rpair.second+=3;
      return rpair;
    }
  private:
    pair<int,int> maxBins;
    bool firstWrap;
    bool secondWrap;
    bool output;
    bool firstWrapCheck(pair<int,int> thisBins){
      //if it is the trivial case handle it
      if(!firstWrap) return abs(thisBins.first-maxBins.first)<=3 ;
      //otherwise check if it is in the domain
      if(abs(thisBins.first-maxBins.first)<=3) return true;
      //if not check if it is in the wrapped domain 
      else if(maxBins.first>93)return abs(thisBins.first-maxBins.first+96)<=3 ;
      else return abs(thisBins.first-maxBins.first-96)<=3 ;
    }
    bool secondWrapCheck(pair<int,int> thisBins){
      if(!secondWrap) return abs(thisBins.second-maxBins.second)<=3 ;
      if(abs(thisBins.first-maxBins.first)<=3) return true;
      if(maxBins.second>253)return abs(thisBins.second-maxBins.second+256)<=3 ;
      else return abs(thisBins.second-maxBins.second-256)<=3 ;
    }
};


const float IDBurner::_kSEGMENTATION = 0.025f;


IDBurner::IDBurner(bool isHI=false) : _kISHI(isHI)
{
}

IDBurner::~IDBurner(){
  if(_towerMap) delete _towerMap;
}

bool IDBurner::doNodePointers(PHCompositeNode* topNode){
  cout<<"Doing IDBurner nodes"<<endl;
  bool goodPointers=true;
  //use subtracted clusters for HI events
  TowerBackground *towerBack;
  if(_kISHI){
    _subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC_SUB");
    towerBack = findNode::getClass<TowerBackground>(topNode,"TowerBackground_Sub2");
    _towerContainer = findNode::getClass<RawTowerContainer>(topNode,"TOWER_CALIB_CEMC_SUB1");
  }
  else{
    _subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC");
    towerBack = NULL;
    _towerContainer = findNode::getClass<RawTowerContainer>(topNode,"TOWER_CALIB_CEMC");
  }
  _truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode,"G4TruthInfo");
  _geomEM = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_CEMC");
  if(!towerBack&&_kISHI){
    cerr<<Name()<<": TowerBackground not in node tree\n";
  }
  else if(_kISHI){
    cout<<"TowerBackground is valid = "<<towerBack->isValid();
    towerBack->identify();
  }
  if(!_subClusterContainer){
    cerr<<Name()<<": critical error-bad nodes \n";
    if(!_subClusterContainer){
      cerr<<"\t RawClusterContainer is bad";
    }
    cerr<<endl;
    goodPointers=false;
  }
  if (!_towerContainer)
  {
    cerr<<Name()<<": Missing Tower Container\n";
  }
  else{
    cout<<Name()<<" found event with "<<_subClusterContainer->size()<<" clusters\n";
  }
  return goodPointers;
}

void IDBurner::process_cluster(RawCluster *cluster)
{
  //https://github.com/ChaseSmith/PhotonIso/blob/master/treesource/TreeMaker.C
  //get the towers from the cluster
  std::vector <ChaseTower> clusterTowers;
  RawCluster::TowerConstRange clusterrange = cluster->get_towers();
  for (RawCluster::TowerConstIterator rtiter = clusterrange.first; rtiter != clusterrange.second; ++rtiter) 
  {
    RawTower *tower = _towerContainer->getTower(rtiter->first);
    if (tower)
    {
      RawTowerGeom *tower_geom = _geomEM->get_tower_geometry(tower->get_key());
      ChaseTower temp;
      temp.setEta(tower_geom->get_eta());
      temp.setPhi(tower_geom->get_phi());
      temp.setEnergy(tower->get_energy());
      temp.setKey(tower->get_key());
      clusterTowers.push_back(temp);
    }
    else{
      cerr<<"Tower not found"<<endl;
    }
  }

  //now that we have all towers from cluster, find max tower

  ChaseTower MaxTower;
  MaxTower = MaxTower.findMaxTower(clusterTowers);

  //Find 49 towers around max tower, Sasha style

  std::vector<ChaseTower> Sasha49Towers;
  
  pair<int,int> maxBins = pair<int,int>(_geomEM->get_etabin(MaxTower.getEta()),_geomEM->get_phibin(MaxTower.getPhi()));
  EvaluateBinIn49 binEvaluator = EvaluateBinIn49(maxBins);
  RawTowerContainer::ConstRange towerrange = _towerContainer->getTowers();

  for (RawTowerContainer::ConstIterator rtiter = towerrange.first; rtiter != towerrange.second; ++rtiter) 
  {
    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = _geomEM->get_tower_geometry(tower->get_key());
    double this_energy = tower->get_energy();
    /*double this_phi = tower_geom->get_phi();
    double this_eta = tower_geom->get_eta();
    double dif_eta = this_eta - MaxTower.getEta();
    double dif_phi = this_phi - MaxTower.getPhi();

    if(dif_phi > TMath::Pi()){dif_phi -= 2*TMath::Pi();} //make sure dif_phi is between -pi and pi
    else if(dif_phi < -1*TMath::Pi()){dif_phi += 2*TMath::Pi();}
    //const float kMAXDIFF = .084; //this sets the size of the image cluster
    //if(fabs(dif_eta) < kMAXDIFF and fabs(dif_phi) < kMAXDIFF )
    ///{
      //Sasha49Towers.push_back(ChaseTower(dif_eta, dif_phi, this_energy, tower->get_key()));
      //tower_geom->identify();*/
    //}
    //check if the eta phi bins of the current tower are close to the max tower
    pair<int,int> thisBins (_geomEM->get_etabin(tower_geom->get_eta()),_geomEM->get_phibin(tower_geom->get_phi()));
    if(binEvaluator(thisBins)){
        thisBins = binEvaluator.getMapPosition(thisBins);
        cout<<"Max E = "<<MaxTower.getEnergy()<<": Tower at ("<<thisBins.first<<','<<thisBins.second<<") with E = "<<this_energy<<'\n';
        Sasha49Towers.push_back(ChaseTower(thisBins.first, thisBins.second, this_energy, tower->get_key()));
      }
  }
  if(Sasha49Towers.size()!=49){
    cout<<"got "<<Sasha49Towers.size()<<"/49 towers\n";
    cout<<"Max at ("<<MaxTower.getEta()<<','<<MaxTower.getPhi()<<")\n";
  }
  if(!_towerMap) {
    _towerMap = new TowerMap(Sasha49Towers,MaxTower);
  }
  else{
    _towerMap->Reset(Sasha49Towers,MaxTower);
  }
}

float IDBurner::getTowerEnergy(unsigned mapPosition){
  if(!_towerMap) return -999;
  unsigned xPos = (unsigned) mapPosition / 7;
  unsigned yPos = mapPosition % 7;
  return _towerMap->getTowerEnergy(pair<int,int>(xPos,yPos));
}



