#include "SubtractedClusterBurner.h"
#include <calobase/RawCluster.h>
#include <phool/getClass.h>
#include <TMath.h>
#include <iostream>
using namespace std;
SubtractedClusterBurner::SubtractedClusterBurner(const std::string &name, unsigned runnumber) : SubsysReco("SubtractedClusterBurner"),
	_kRunNumber(runnumber)
{
	_foutname = name;
}

SubtractedClusterBurner::~SubtractedClusterBurner(){
	if (_f) delete _f;
}

int SubtractedClusterBurner::InitRun(PHCompositeNode *topNode)
{

	_f = new TFile( _foutname.c_str(), "RECREATE");
	_ttree = new TTree("subtractedTree","super stylish sapling");
  _ttree->Branch("sub_clus_n",&_b_clustersub_n);
  _ttree->Branch("sub_clus_e",&_b_clustersub_E,"sub_clus_e[sub_clus_n]/F");
  _ttree->Branch("sub_clus_ecore",&_b_clustersub_ecore,"sub_clus_ecore[sub_clus_n]/F");
  _ttree->Branch("sub_clus_eta",&_b_clustersub_eta,"sub_clus_eta[sub_clus_n]/F");
  _ttree->Branch("sub_clus_phi",&_b_clustersub_phi,"sub_clus_phi[sub_clus_n]/F");

	return 0;
}

bool SubtractedClusterBurner::doNodePointers(PHCompositeNode* topNode){
	bool goodPointers=true;
	_subClusterContainer = findNode::getClass<RawClusterContainer>(topNode,"CLUSTER_CEMC_SUB");
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

int SubtractedClusterBurner::process_event(PHCompositeNode *topNode)
{
	doNodePointers(topNode);
	RawClusterContainer::ConstRange begin_end = _subClusterContainer->getClusters(); 
	RawClusterContainer::ConstIterator rtiter;
  _b_clustersub_n=0; 

	for (rtiter = begin_end.first; rtiter != begin_end.second; ++rtiter) 
	{ 
		RawCluster *cluster = rtiter->second; 
		float energy = cluster->get_energy(); 
		if ( energy < _kMINCLUSTERENERGY ) continue; 
		float phi = cluster->get_phi(); 
		float eta = -1 * log( tan( TMath::ATan2( cluster->get_r(), cluster->get_z()  ) / 2.0 ) ); 
		_b_clustersub_E[ _b_clustersub_n ] = energy ; 
		_b_clustersub_ecore[ _b_clustersub_n ] = cluster->get_ecore() ; 
		_b_clustersub_eta[ _b_clustersub_n ] = eta ; 
		_b_clustersub_phi[ _b_clustersub_n ] = phi ; 
		_b_clustersub_n++; 
	} 
  _ttree->Fill();  
  return 0;
}


int SubtractedClusterBurner::End(PHCompositeNode *topNode)
{
	cout<<"closing"<<endl;
  _ttree->Write();
	_f->Write();
	_f->Close();
	return 0;
}
