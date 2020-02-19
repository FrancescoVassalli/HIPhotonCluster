#include "AllClusBurner.h"
#include "IDBurner.h"

#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <g4main/PHG4VtxPoint.h>
#include <jetbackground/TowerBackground.h>
#include <phool/getClass.h>

#include <TLorentzVector.h>

#include <iostream>
#include <map>
#include <unordered_set>
using namespace std;

const float AllClusBurner::_kCLUSTERDR = 0.2f;
const float AllClusBurner::_kMAXETA=1.f;
const std::unordered_set<int> AllClusBurner::interest_pids {22,311,313,421,423,511,513,111,113,130,221,331};


AllClusBurner::AllClusBurner(const std::string &name, unsigned runnumber=0,bool isHI=false) : SubsysReco("AllClusBurner"),
  _kRunNumber(runnumber), _kISHI(isHI)
{
  _foutname = name;
  _towerBurner= new IDBurner(isHI);
}

AllClusBurner::~AllClusBurner(){
  if (_f) delete _f;
  if (_towerBurner) delete _towerBurner; 
}

int AllClusBurner::InitRun(PHCompositeNode *topNode)
{

  _f = new TFile( _foutname.c_str(), "RECREATE");
  _ttree = new TTree("subtractedTree","super stylish sapling");
  _ttree->Branch("sub_clus_n",&_b_clustersub_n);
  _ttree->Branch("isPhoton",&_b_isPhoton,"isPhoton[sub_clus_n]/I");
  _ttree->Branch("parent_pid",&_b_parent_pid,"parent_pid[sub_clus_n]/I");
  /*_ttree->Branch("tphoton_e",&_b_truthphoton_E,"tphoton_e[sub_clus_n]/F");
    _ttree->Branch("tphoton_pT",&_b_truthphoton_pT,"tphoton_pT[sub_clus_n]/F");
    _ttree->Branch("tphoton_eta",&_b_truthphoton_eta,"tphoton_eta[sub_clus_n]/F");
    _ttree->Branch("tphoton_phi",&_b_truthphoton_phi,"tphoton_phi[sub_clus_n]/F");*/
  _ttree->Branch("sub_clus_e",&_b_clustersub_E,"sub_clus_e[sub_clus_n]/F");
  _ttree->Branch("sub_clus_calE",&_b_clustersub_calE,"sub_clus_calE[sub_clus_n]/F");
  _ttree->Branch("sub_clus_ecore",&_b_clustersub_ecore,"sub_clus_ecore[sub_clus_n]/F");
  /*_ttree->Branch("sub_clus_eta",&_b_clustersub_eta,"sub_clus_eta[sub_clus_n]/F");
    _ttree->Branch("sub_clus_phi",&_b_clustersub_phi,"sub_clus_phi[sub_clus_n]/F");*/
  _ttree->Branch("sub_clus_prob",&_b_clustersub_prob,"sub_clus_prob[sub_clus_n]/F");
  /*_ttree->Branch("matchDR",&_b_matchDR,"matchDR[sub_clus_n]/F");
    _ttree->Branch("matchEta",&_b_matchEta,"matchEta[sub_clus_n]/F");
    _ttree->Branch("matchPhi",&_b_matchPhi,"matchPhi[sub_clus_n]/F");*/

  //make a branch for each tower
  string bTitle="tower";
  string bStruct = "[sub_clus_n]/F";
  for (unsigned i = 0; i < _kNTOWERS; ++i)
  {
    string str = bTitle + std::to_string(i);
    string str2 = str+bStruct;
    _ttree->Branch(str.c_str(),_b_tower_Eray[i],str2.c_str());
  }

  return 0;
}


//get all the nodes off of the tree that I will need and print an error if they cannot be found
bool AllClusBurner::doNodePointers(PHCompositeNode* topNode){
  bool goodPointers=true;
  //use subtracted clusters for HI events
  TowerBackground *towerBack;
  if(_kISHI){
    _subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC_SUB");
    towerBack = findNode::getClass<TowerBackground>(topNode,"TowerBackground_Sub2");
  }
  else{
    _subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC");
    towerBack = NULL;
  }
  _towerContainerEM = findNode::getClass<RawTowerContainer>(topNode,"TOWER_CALIB_CEMC");
  _towerContainerIH = findNode::getClass<RawTowerContainer>(topNode,"TOWER_CALIB_HCALIN");
  _towerContainerOH = findNode::getClass<RawTowerContainer>(topNode,"TOWER_CALIB_HCALOUT");
  _truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode,"G4TruthInfo");
  if(!towerBack&&_kISHI){
    cerr<<Name()<<": TowerBackground not in node tree\n";
  }
  else if(_kISHI){
    cout<<"TowerBackground is valid = "<<towerBack->isValid()<<endl;
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
  else{
    cout<<Name()<<" found event with "<<_subClusterContainer->size()<<" clusters\n";
  }
  if(!_towerContainerEM){
    cerr<<Name()<<"EMcal towers not found"<<endl; 
  }
  if(!_towerContainerIH){
    cerr<<Name()<<"IHcal towers not found"<<endl; 
  }
  if(!_towerContainerOH){
    cerr<<Name()<<"OHcal towers not found"<<endl; 
  }
  return goodPointers && _towerBurner->doNodePointers(topNode);
}

int AllClusBurner::process_event(PHCompositeNode *topNode)
{
  if(!doNodePointers(topNode)) return 2;
  if(_kRunNumber%10==0){
    cout<<"AllClusBurner Processesing Event "<<_kRunNumber<<endl;
  }
  cout<<"getting CalEnergy"<<endl;
  const float calE =(float) getCalEnergy();
  _b_clustersub_n=0; 
  cout<<"tagging clusters"<<endl;
  map<int,int> photonClusterIds = getTaggedClusters(topNode);
  auto range  = _subClusterContainer->getClusters();
  for (auto i = range.first; i != range.second; ++i)
  {
    RawCluster* icluster = i->second;
    if(icluster->get_energy()<_kMINCLUSTERENERGY||get_eta(icluster)>_kMAXETA) continue;
    if(photonClusterIds.find(icluster->get_id()) != photonClusterIds.end()){
      _b_isPhoton[_b_clustersub_n] = 1;
      _b_parent_pid[_b_clustersub_n] = photonClusterIds.find(icluster->get_id())->second;
      cout<<icluster->get_id()<<": "<<_b_isPhoton[_b_clustersub_n]<< " with source "<<photonClusterIds.find(icluster->get_id())->second;
    }
    else{
      _b_isPhoton[_b_clustersub_n] = 0;
      _b_parent_pid[_b_clustersub_n] = -999;
    }
    _b_clustersub_E[ _b_clustersub_n ] = icluster->get_energy() ; 
    _b_clustersub_ecore[ _b_clustersub_n ] = icluster->get_ecore() ; 
    _b_clustersub_prob[ _b_clustersub_n ] = icluster->get_prob() ; 
    _b_clustersub_calE[ _b_clustersub_n ] = calE ; 
    _towerBurner->process_cluster(icluster);
    for (unsigned i = 0; i < _kNTOWERS; ++i)
    {
      _b_tower_Eray[i][_b_clustersub_n] = _towerBurner->getTowerEnergy(i);
    }
    _b_clustersub_n++; 
  }
  _ttree->Fill();
  return 0;
}

std::map<int,int> AllClusBurner::getTaggedClusters(PHCompositeNode *topNode){
  std::map<int,int> taggedClusters;
  PHG4TruthInfoContainer::Range range = _truthinfo->GetPrimaryParticleRange(); //look at all truth particles
  for ( PHG4TruthInfoContainer::ConstIterator iter = range.first; iter != range.second; ++iter ) {
    PHG4Particle* g4particle = iter->second;
    float pt = sqrt(g4particle->get_px()*g4particle->get_px()+g4particle->get_py()*g4particle->get_py());
    if(pt>5){
      TLorentzVector tlv;
      tlv.SetPxPyPzE(g4particle->get_px(),g4particle->get_py(),g4particle->get_pz(),g4particle->get_e());
      if(TMath::Abs(tlv.Eta())<1.1){
        cout<<"High energy particle "<<g4particle->get_pid()<<" with "<<pt<<" GeV and eta = "<<tlv.Eta()<<"\n";
      }
    }
    //check if a particle of interest
    if(interest_pids.find(TMath::Abs(g4particle->get_pid()))!=interest_pids.end()){
      //if it is make the tlv
      TLorentzVector All_tlv;
      All_tlv.SetPxPyPzE(g4particle->get_px(),g4particle->get_py(),g4particle->get_pz(),g4particle->get_e());
      //if the particle is not in acceptance eta or energy skip it
      if(All_tlv.Pt()<_kMINCLUSTERENERGY||TMath::Abs(All_tlv.Eta())>_kMAXETA) continue;
      //find the matching cluster
      RawCluster* cluster=getCluster(&All_tlv);
      //check if this cluster is already in the list
      if (cluster && taggedClusters.find(cluster->get_id())!=taggedClusters.end())
      {
        /*If it is already in the list tag it as multi particle cluster*/
        taggedClusters.find(cluster->get_id())->second = -998;
        cout<<"Collision cluster with E = "<<cluster->get_energy()<<'\n';
      }
      else if(cluster){
        taggedClusters[cluster->get_id()] = TMath::Abs(g4particle->get_pid());
      }
      /*get the parent info
        PHG4Particle *parent = _truthinfo->GetParticle(g4particle->get_parent_id());
        if(parent){
        info.second = parent->get_pid();
        cout<<"Found parent id "<<parent->get_pid()<<'\n';
        }
        else {
        info.second = -999;
        cout<<"Parent not found id = "<<g4particle->get_parent_id()<<'\n';
        }*/
    }
  }
  cout<<"Found "<<taggedClusters.size()<<" neutral meson/photon  clusters"<<endl;
  for(auto i = taggedClusters.begin(); i!=taggedClusters.end();i++){
    cout<<i->first<<"\n\t";
  }
  return taggedClusters;
}

RawCluster* AllClusBurner::getCluster(TLorentzVector* tlv){
  RawClusterContainer::ConstRange begin_end = _subClusterContainer->getClusters(); 
  RawClusterContainer::ConstIterator rtiter;
  double dr=-1;
  RawCluster *rcluster=NULL;
  //loop over all clusters to find match with lowest delta R
  for (rtiter = begin_end.first; rtiter != begin_end.second; ++rtiter) 
  { 
    RawCluster *cluster = rtiter->second; 
    if(cluster->get_energy()>_kMINCLUSTERENERGY&&(DeltaR(tlv,cluster)<dr||dr<0)){
      dr=DeltaR(tlv,cluster);
      rcluster=cluster;
    }
  }
  return rcluster;
}

double AllClusBurner::getCalEnergy(){
  //loop over all towers in EMcal
  double rsum = 0;
  auto range = _towerContainerEM->getTowers();
  for(auto itTower =range.first; itTower !=range.second; itTower++){
    rsum+=itTower->second->get_energy();
  }
  //then do the hadronic calorimeters
  range = _towerContainerIH->getTowers();
  for(auto itTower =range.first; itTower !=range.second; itTower++){
    rsum+=itTower->second->get_energy();
  }
  range = _towerContainerOH->getTowers();
  for(auto itTower =range.first; itTower !=range.second; itTower++){
    rsum+=itTower->second->get_energy();
  }
  return rsum;
}

int AllClusBurner::End(PHCompositeNode *topNode)
{
  cout<<"closing"<<endl;
  _f->cd();
  _ttree->Write();
  _f->Write();
  _f->Close();
  return 0;
}

double AllClusBurner::DeltaR (TLorentzVector *tlv, RawCluster* cluster){
  return DeltaR(tlv->Eta(),get_eta(cluster),tlv->Phi(),cluster->get_phi());
}
