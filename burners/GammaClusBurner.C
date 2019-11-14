#include "GammaClusBurner.h"

#include <calobase/RawCluster.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <phool/getClass.h>

#include <TLorentzVector.h>

#include <iostream>
using namespace std;

const float GammaClusBurner::_kCLUSTERDR = 0.2f;
const float GammaClusBurner::_kMAXETA=1.1f;


GammaClusBurner::GammaClusBurner(const std::string &name, unsigned runnumber=0,bool isHI=false) : SubsysReco("GammaClusBurner"),
  _kRunNumber(runnumber), _kISHI(isHI)
{
  _foutname = name;
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
  _ttree->Branch("sub_clus_e",&_b_clustersub_E,"sub_clus_e[sub_clus_n]/F");
  _ttree->Branch("sub_clus_ecore",&_b_clustersub_ecore,"sub_clus_ecore[sub_clus_n]/F");
  _ttree->Branch("sub_clus_eta",&_b_clustersub_eta,"sub_clus_eta[sub_clus_n]/F");
  _ttree->Branch("sub_clus_phi",&_b_clustersub_phi,"sub_clus_phi[sub_clus_n]/F");
  _ttree->Branch("sub_clus_prob",&_b_clustersub_prob,"sub_clus_prob[sub_clus_n]/F");

  return 0;
}

bool GammaClusBurner::doNodePointers(PHCompositeNode* topNode){
  bool goodPointers=true;
  //use subtracted clusters for HI events
  if(_kISHI)_subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC_SUB");
  else _subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC");
  _truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode,"G4TruthInfo");
  if(!_subClusterContainer){
    cerr<<Name()<<": critical error-bad nodes \n";
    if(!_subClusterContainer){
      cerr<<"\t RawClusterContainer is bad";
    }
    cerr<<endl;
    goodPointers=false;
  }
  return goodPointers;
}

int GammaClusBurner::process_event(PHCompositeNode *topNode)
{
  doNodePointers(topNode);
  if(_kRunNumber%10==0){
    cout<<"GamaClusBurner Processesing Event "<<_kRunNumber<<endl;
  }
  _b_clustersub_n=0; 
  PHG4TruthInfoContainer::Range range = _truthinfo->GetPrimaryParticleRange(); //look at all truth particles
  for ( PHG4TruthInfoContainer::ConstIterator iter = range.first; iter != range.second; ++iter ) {
    PHG4Particle* g4particle = iter->second;
    //maybe I am double counting due to kinematic updates
    if(g4particle->get_pid()==22){
      TLorentzVector gamma_tlv;
      gamma_tlv.SetPxPyPzE(g4particle->get_px(),g4particle->get_py(),g4particle->get_pz(),g4particle->get_e());
      if(gamma_tlv.Pt()<_kMINCLUSTERENERGY||TMath::Abs(gamma_tlv.Eta())>_kMAXETA) continue;
      RawCluster* cluster=getCluster(&gamma_tlv);
      float energy = cluster->get_energy(); 
      cout<<"\t photon cluster with cluster e= "<<energy<<" and photon e= "<<g4particle->get_e()<<" dR= "<<DeltaR(&gamma_tlv,cluster)<<'\n';
      if ( energy < _kMINCLUSTERENERGY ) continue; 
      float phi = cluster->get_phi(); 
      float eta = -1 * log( tan( TMath::ATan2( cluster->get_r(), cluster->get_z()  ) / 2.0 ) ); 
      _b_truthphoton_E[_b_clustersub_n ] =gamma_tlv.E();
      _b_truthphoton_pT[_b_clustersub_n ] =gamma_tlv.Pt();
      _b_clustersub_E[ _b_clustersub_n ] = energy ; 
      _b_clustersub_ecore[ _b_clustersub_n ] = cluster->get_ecore() ; 
      _b_clustersub_prob[ _b_clustersub_n ] = cluster->get_prob() ; 
      _b_clustersub_eta[ _b_clustersub_n ] = eta ; 
      _b_clustersub_phi[ _b_clustersub_n ] = phi ; 
      _b_clustersub_n++; 
    }
  }
  _ttree->Fill();
  return 0;
}

RawCluster* GammaClusBurner::getCluster(TLorentzVector* tlv){
  RawClusterContainer::ConstRange begin_end = _subClusterContainer->getClusters(); 
  RawClusterContainer::ConstIterator rtiter;
  double dr=-1;
  RawCluster *rcluster=NULL;
  for (rtiter = begin_end.first; rtiter != begin_end.second; ++rtiter) 
  { 
    RawCluster *cluster = rtiter->second; 
    if(cluster->get_energy()>1&&(DeltaR(tlv,cluster)<dr||dr<0)){
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
  float cluseta=-1 * log( tan( TMath::ATan2( cluster->get_r(), cluster->get_z()  ) / 2.0 ) );
  return DeltaR(tlv->Eta(),cluseta,tlv->Phi(),cluster->get_phi());
}
