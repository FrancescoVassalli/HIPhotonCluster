#include "GammaLocal.h"
#include "IDBurner.h"

#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4VtxPoint.h>
#include <jetbackground/TowerBackground.h>
#include <phool/getClass.h>

#include <TLorentzVector.h>

#include <iostream>
#include <map>
using namespace std;

const float GammaLocal::_kCLUSTERDR = 0.2f;
const float GammaLocal::_kMAXETA=1.1f;


GammaLocal::GammaLocal(const std::string &name, unsigned runnumber=0,bool isHI=false) : SubsysReco("GammaLocal"),
  _kRunNumber(runnumber), _kISHI(isHI)
{
  _foutname = name;
  _towerBurner= new IDBurner(isHI);
}

GammaLocal::~GammaLocal(){
  if (_f) delete _f;
  if (_towerBurner) delete _towerBurner; 
}

int GammaLocal::InitRun(PHCompositeNode *topNode)
{

  _f = new TFile( _foutname.c_str(), "RECREATE");
  _ttree = new TTree("subtractedTree","super stylish sapling");
  _ttree->Branch("sub_clus_n",&_b_clustersub_n);
  _ttree->Branch("tphoton_e",&_b_truthphoton_E,"tphoton_e[sub_clus_n]/F");
  _ttree->Branch("tphoton_pT",&_b_truthphoton_pT,"tphoton_pT[sub_clus_n]/F");
  _ttree->Branch("tphoton_eta",&_b_truthphoton_eta,"tphoton_eta[sub_clus_n]/F");
  _ttree->Branch("tphoton_phi",&_b_truthphoton_phi,"tphoton_phi[sub_clus_n]/F");
  _ttree->Branch("sub_clus_e",&_b_clustersub_E,"sub_clus_e[sub_clus_n]/F");
  _ttree->Branch("sub_clus_ecore",&_b_clustersub_ecore,"sub_clus_ecore[sub_clus_n]/F");
  _ttree->Branch("sub_clus_eta",&_b_clustersub_eta,"sub_clus_eta[sub_clus_n]/F");
  _ttree->Branch("sub_clus_phi",&_b_clustersub_phi,"sub_clus_phi[sub_clus_n]/F");
  _ttree->Branch("sub_clus_prob",&_b_clustersub_prob,"sub_clus_prob[sub_clus_n]/F");
  _ttree->Branch("matchDR",&_b_matchDR,"matchDR[sub_clus_n]/F");
  _ttree->Branch("matchEta",&_b_matchEta,"matchEta[sub_clus_n]/F");
  _ttree->Branch("matchPhi",&_b_matchPhi,"matchPhi[sub_clus_n]/F");

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
bool GammaLocal::doNodePointers(PHCompositeNode* topNode){
  bool goodPointers=true;
  //use subtracted clusters for HI events
  if(_kISHI)_subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC_SUB");
  else _subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC");
  _truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode,"G4TruthInfo");
  TowerBackground *towerBack = findNode::getClass<TowerBackground>(topNode,"TowerBackground_Sub2");
  
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
  else{
    cout<<Name()<<" found event with "<<_subClusterContainer->size()<<" clusters\n";
  }

  return goodPointers && _towerBurner->doNodePointers(topNode);
}

int GammaLocal::process_event(PHCompositeNode *topNode)
{
  if(!doNodePointers(topNode)) return 2;
  if(_kRunNumber%10==0){
    cout<<"GamaLocal Processesing Event "<<_kRunNumber<<endl;
  }

  _b_clustersub_n=0; 
  //std::map<unsigned int,unsigned int> keys_map; //make a list of which clusters I've used so I don't do merged clusters
  //std::vector<PHG4Particle*> convertedPhotons; //make a list of the photon conversion so their photons are not added to the analysis 
                                               //will not work if tracking was turned off 
  PHG4TruthInfoContainer::Range range = _truthinfo->GetPrimaryParticleRange(); //look at all truth particles
  for ( PHG4TruthInfoContainer::ConstIterator iter = range.first; iter != range.second; ++iter ) {
    PHG4Particle* g4particle = iter->second;
    if(g4particle->get_pid()==22){
      //if it is a photon make the tlv
      TLorentzVector gamma_tlv;
      gamma_tlv.SetPxPyPzE(g4particle->get_px(),g4particle->get_py(),g4particle->get_pz(),g4particle->get_e());
      //if the photon is not in acceptance eta or energy skip it
      if(gamma_tlv.Pt()<_kMINCLUSTERENERGY||TMath::Abs(gamma_tlv.Eta())>_kMAXETA) continue;
      //find the towers
      std::vector<ChaseTower> towers = getTowers(gamma_tlv);
      cout<<"Got "<<towers.size()<< "towers\n";
        _b_truthphoton_E[_b_clustersub_n ] =gamma_tlv.E();
        _b_truthphoton_eta[_b_clustersub_n ] =gamma_tlv.Eta();
        _b_truthphoton_phi[_b_clustersub_n ] =gamma_tlv.Phi();
        for (unsigned i = 0; i < _kNTOWERS; ++i)
        {
          _b_tower_Eray[i][_b_clustersub_n] = _towerBurner->getTowerEnergy(i);
          //_b_tower_Eray[i][_b_clustersub_n] = -999;
        }
        keys_map[cluster->get_id()]=_b_clustersub_n;
        _b_clustersub_n++; 
      }
    } //particle is photon
  }
  cout<<"tree filled"<<endl;
  _ttree->Fill();
  return 0;
}

std::vector<ChaseTower> GammaLocal::getTowers(TLorentzVector* tlv){
  RawTowerContainer::ConstRange towerrange = _towerContainer->getTowers();
  for (RawTowerContainer::ConstIterator rtiter = towerrange.first; rtiter != towerrange.second; ++rtiter) 
  {
    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = _geomEM->get_tower_geometry(tower->get_key());
    double this_phi = tower_geom->get_phi();
    double this_eta = tower_geom->get_eta();
    double this_energy = tower->get_energy();
    double dif_eta = this_eta - tlv->getEta();
    double dif_phi = this_phi - tlv->getPhi();

    if(dif_phi > TMath::Pi()){dif_phi -= 2*TMath::Pi();} //make sure dif_phi is between -pi and pi
    else if(dif_phi < -1*TMath::Pi()){dif_phi += 2*TMath::Pi();}
    const float kMAXDIFF = .8;
    if(fabs(dif_eta) < kMAXDIFF and fabs(dif_phi) < kMAXDIFF )
    {
      Sasha49Towers.push_back(ChaseTower(dif_eta, dif_phi, this_energy, tower->get_key()));
    }
  }
  return Sasha49Towers;
}

int GammaLocal::End(PHCompositeNode *topNode)
{
  cout<<"closing"<<endl;
  _f->cd();
  _ttree->Write();
  _f->Write();
  _f->Close();
  return 0;
}

double GammaLocal::DeltaR (TLorentzVector *tlv, RawCluster* cluster){
  return DeltaR(tlv->Eta(),get_eta(cluster),tlv->Phi(),cluster->get_phi());
}
