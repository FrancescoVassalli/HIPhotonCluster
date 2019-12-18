#include "GammaClusBurner.h"
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

const float GammaClusBurner::_kCLUSTERDR = 0.2f;
const float GammaClusBurner::_kMAXETA=1.1f;


GammaClusBurner::GammaClusBurner(const std::string &name, unsigned runnumber=0,bool isHI=false) : SubsysReco("GammaClusBurner"),
  _kRunNumber(runnumber), _kISHI(isHI)
{
  _foutname = name;
  _towerBurner= new IDBurner(isHI);
}

GammaClusBurner::~GammaClusBurner(){
  if (_f) delete _f;
}

int GammaClusBurner::InitRun(PHCompositeNode *topNode)
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

bool GammaClusBurner::doNodePointers(PHCompositeNode* topNode){
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
  return goodPointers;
}

int GammaClusBurner::process_event(PHCompositeNode *topNode)
{
  if(!doNodePointers(topNode)) return 2;
  if(_kRunNumber%10==0){
    cout<<"GamaClusBurner Processesing Event "<<_kRunNumber<<endl;
  }
  _b_clustersub_n=0; 
  std::map<unsigned int,unsigned int> keys_map; //make a list of which clusters I've used so I don't do merged clusters
  std::vector<PHG4Particle*> convertedPhotons;
  PHG4TruthInfoContainer::Range range = _truthinfo->GetPrimaryParticleRange(); //look at all truth particles
  for ( PHG4TruthInfoContainer::ConstIterator iter = range.first; iter != range.second; ++iter ) {
    PHG4Particle* g4particle = iter->second;
    //maybe I am double counting due to kinematic updates
    if(g4particle->get_pid()==22){
      //if it is a photon make the tlv
      TLorentzVector gamma_tlv;
      gamma_tlv.SetPxPyPzE(g4particle->get_px(),g4particle->get_py(),g4particle->get_pz(),g4particle->get_e());
      //if the photon is not in acceptance eta or energy skip it
      if(gamma_tlv.Pt()<_kMINCLUSTERENERGY||TMath::Abs(gamma_tlv.Eta())>_kMAXETA) continue;
      //find the matching cluster
      RawCluster* cluster=getCluster(&gamma_tlv);
      float energy = cluster->get_energy(); 
      if ( energy < _kMINCLUSTERENERGY ) continue; 
      //prevent double counting 
      if (keys_map.find(cluster->get_id())==keys_map.end()){
        cout<<"\t photon cluster with cluster e= "<<energy<<" and photon e= "<<g4particle->get_e()<<" dPhi= "<< DeltaPhi(gamma_tlv.Phi(),cluster->get_phi()) <<'\n';
        cout<<"\t \t cluster at (eta,phi) = ("<<get_eta(cluster)<<','<<cluster->get_phi()<<")and photon at (eta,phi) = ("<<gamma_tlv.Eta()<<','<<gamma_tlv.Phi()<<")\n";
        float phi = cluster->get_phi(); 
        float eta = get_eta(cluster); 
        _b_truthphoton_E[_b_clustersub_n ] =gamma_tlv.E();
        _b_truthphoton_pT[_b_clustersub_n ] =gamma_tlv.Pt();
        _b_truthphoton_eta[_b_clustersub_n ] =gamma_tlv.Eta();
        _b_truthphoton_phi[_b_clustersub_n ] =gamma_tlv.Phi();
        _b_clustersub_E[ _b_clustersub_n ] = energy ; 
        _b_clustersub_ecore[ _b_clustersub_n ] = cluster->get_ecore() ; 
        _b_clustersub_prob[ _b_clustersub_n ] = cluster->get_prob() ; 
        _b_clustersub_eta[ _b_clustersub_n ] = eta ; 
        _b_clustersub_phi[ _b_clustersub_n ] = phi ; 
        _b_matchDR[ _b_clustersub_n ] = DeltaR(&gamma_tlv,cluster) ; 
        _b_matchPhi[ _b_clustersub_n ] = (float) DeltaPhi(gamma_tlv.Phi(),cluster->get_phi()) ; 
        _b_matchEta[ _b_clustersub_n ] = TMath::Abs(gamma_tlv.Eta()-get_eta(cluster)); 
        _towerBurner->process_cluster(cluster);
        for (unsigned i = 0; i < _kNTOWERS; ++i)
        {
          _b_tower_Eray[i][_b_clustersub_n] = _towerBurner->getTowerEnergy(i);
        }
        keys_map[cluster->get_id()]=_b_clustersub_n;
        _b_clustersub_n++; 
      }
      else{
        cout<<"Cluster Rejected!\n";
        unsigned int clustersub_n = keys_map[cluster->get_id()];
        _b_truthphoton_E[clustersub_n ] =-999;
        _b_truthphoton_pT[clustersub_n ] =-999;
        _b_truthphoton_eta[clustersub_n ] =-999;
        _b_truthphoton_phi[clustersub_n ] =-999;
        _b_clustersub_E[ clustersub_n ] = -999 ; 
        _b_clustersub_ecore[ clustersub_n ] = -999 ; 
        _b_clustersub_prob[ clustersub_n ] = -999 ; 
        _b_clustersub_eta[ clustersub_n ] = -999 ; 
        _b_clustersub_phi[ clustersub_n ] = -999 ; 
        _b_matchDR[ clustersub_n ] = -999 ; 
        _b_matchPhi[ _b_clustersub_n ] = -999 ; 
        _b_matchEta[ _b_clustersub_n ] = -999; 
      }
    } //particle is photon
    else if (abs(g4particle->get_pid())==11) //check if conversions need to be removed
    {
      PHG4Particle* parent =_truthinfo->GetParticle(g4particle->get_parent_id());
      PHG4VtxPoint* vtx=_truthinfo->GetVtx(g4particle->get_vtx_id()); 
      if (parent->get_pid()==22&&vtx)
      {
        if (sqrt(vtx->get_x()*vtx->get_x()+vtx->get_y()*vtx->get_y())<s_kTPCRADIUS)
        {
          convertedPhotons.push_back(parent);
        }
      }
    }
  }
  //remove the converted photons
  for(PHG4Particle* g4particle : convertedPhotons){
    TLorentzVector gamma_tlv;
    gamma_tlv.SetPxPyPzE(g4particle->get_px(),g4particle->get_py(),g4particle->get_pz(),g4particle->get_e());
    if(gamma_tlv.Pt()<_kMINCLUSTERENERGY||TMath::Abs(gamma_tlv.Eta())>_kMAXETA) continue;
    RawCluster* cluster=getCluster(&gamma_tlv);
    if ( cluster->get_energy()  < _kMINCLUSTERENERGY ) continue; 
    unsigned int clustersub_n = keys_map[cluster->get_id()];
    _b_truthphoton_E[clustersub_n ] =-999;
        _b_truthphoton_pT[clustersub_n ] =-999;
        _b_clustersub_E[ clustersub_n ] = -999 ; 
        _b_clustersub_ecore[ clustersub_n ] = -999 ; 
        _b_clustersub_prob[ clustersub_n ] = -999 ; 
        _b_clustersub_eta[ clustersub_n ] = -999 ; 
        _b_clustersub_phi[ clustersub_n ] = -999 ; 
        _b_matchDR[ clustersub_n ] = -999 ; 
        _b_matchPhi[ _b_clustersub_n ] = -999 ; 
        _b_matchEta[ _b_clustersub_n ] = -999; 
  }
  _ttree->Fill();
  return 0;
}

RawCluster* GammaClusBurner::getCluster(TLorentzVector* tlv){
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

int GammaClusBurner::End(PHCompositeNode *topNode)
{
  cout<<"closing"<<endl;
  _f->cd();
  _ttree->Write();
  _f->Write();
  _f->Close();
  return 0;
}

double GammaClusBurner::DeltaR (TLorentzVector *tlv, RawCluster* cluster){
  return DeltaR(tlv->Eta(),get_eta(cluster),tlv->Phi(),cluster->get_phi());
}
