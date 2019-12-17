#include "IDBurner.h"
#include "ChaseTower.h"

#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <jetbackground/TowerBackground.h>
#include <phool/getClass.h>

#include <TLorentzVector.h>

#include <iostream>
#include <map>
#include <pair>
using namespace std;

const float IDBurner::_kSEGMENTATION = 0.025f;


IDBurner::IDBurner(bool isHI=false) : _kISHI(isHI)
{
}

IDBurner::~IDBurner(){
  if(_towerMap) delete _towerMap;
}

bool IDBurner::doNodePointers(PHCompositeNode* topNode){
  bool goodPointers=true;
  //use subtracted clusters for HI events
  if(_kISHI)_subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC_SUB");
  else _subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC");
  _truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode,"G4TruthInfo");
  TowerBackground *towerBack = findNode::getClass<TowerBackground>(topNode,"TowerBackground_Sub2");
  _towerContainer = findNode::getClass<RawTowerContainer>(topNode,"TOWER_CALIB_CEMC_SUB1");
  _geomEM = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_CEMC");
  if(!towerBack){
    cerr<<Name()<<": TowerBackground not in node tree\n";
  }
  else{
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
      RawTowerGeom *tower_geom = geomEM->get_tower_geometry(tower->get_key());
      ChaseTower temp;
      temp.setEta(tower_geom->get_eta());
      _b_clusterTower_eta[_b_clusterTower_towers] = tower_geom->get_eta();
      temp.setPhi(tower_geom->get_phi());
      _b_clusterTower_phi[_b_clusterTower_towers] = tower_geom->get_phi();
      temp.setEnergy(tower->get_energy());
      _b_clusterTower_energy[_b_clusterTower_towers] = tower->get_energy();
      temp.setKey(tower->get_key());
      clusterTowers.push_back(temp);
      _b_clusterTower_towers++;
    }

    //now that we have all towers from cluster, find max tower

    ChaseTower MaxTower = ChaseTower.findMaxTower(clusterTowers);

    //Find 49 towers around max tower, Sasha style

    std::vector<ChaseTower> Sasha49Towers;

    RawTowerContainer::ConstRange towerrange = _towerContainer->getTowers();
    for (RawTowerContainer::ConstIterator rtiter = towerrange.first; rtiter != towerrange.second; ++rtiter) 
    {
      RawTower *tower = rtiter->second;
      RawTowerGeom *tower_geom = geomEM->get_tower_geometry(tower->get_key());
      double this_phi = tower_geom->get_phi();
      double this_eta = tower_geom->get_eta();
      double this_energy = tower->get_energy();
      double dif_eta = this_eta - MaxTower.getEta();
      double dif_phi = this_phi - MaxTower.getPhi();

      if(dif_phi > TMath::Pi()){dif_phi -= 2*TMath::Pi();} //make sure dif_phi is between -pi and pi
      else if(dif_phi < -1*TMath::Pi()){dif_phi += 2*TMath::Pi();}
      const float kMAXDIFF = (_kTOPOSIZE+.4)*_kSEGMENTATION ;
      if(fabs(dif_eta) < kMAXDIFF and fabs(dif_phi) < kMAXDIFF )
      {
        Sasha49Towers.push_back(ChaseTower(dif_eta, dif_phi, this_energy, tower->get_key()));
      }
    }
    if(_towerMap) delete _towerMap;
    _towerMap = new TowerMap(Sasha49Towers,MaxTower);
}

float IDBurner::getTowerEnergy(unsigned mapPosition){
  unsigned xPos = mapPosition / 7;
  unsigned yPos = mapPosition % 7;
  return _towerMap->getTowerEnergy(pair<int,int>(xPos,yPos));
}
