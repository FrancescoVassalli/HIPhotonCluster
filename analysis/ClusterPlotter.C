#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>


void makeMaps(TChain *tree,string ext="",unsigned nclusters=0){
  const unsigned kNTOWERS = 49;
	int clusn;
  float gammaE[600];
  float clusE[600];
  float matchDR[600];
  float matchDPhi[600];
  float matchDEta[600];
  float prob[600];
  float towers[kNTOWERS][600];

  tree->SetBranchAddress("tphoton_e",gammaE);
  tree->SetBranchAddress("sub_clus_n",&clusn);
  tree->SetBranchAddress("sub_clus_e",clusE);
  tree->SetBranchAddress("matchDR",matchDR);
  tree->SetBranchAddress("matchPhi",matchDPhi);
  tree->SetBranchAddress("matchEta",matchDEta);
  tree->SetBranchAddress("sub_clus_prob",prob);

  string bTitle="tower";
  string bStruct = "[sub_clus_n]/F";
  for (unsigned i = 0; i < kNTOWERS; ++i)
  {
    string str = bTitle + std::to_string(i);
    string str2 = str+bStruct;
    tree->SetBranchAddress(str.c_str(),towers[i]);
  }

  const int kBINS=40;
  string strcluster = "cluster";
  unsigned treeEvent = 0;
  unsigned iarray=0;
  unsigned lastTree=-1;
  while(nclusters>0){
    if(lastTree!=treeEvent){
     tree->GetEntry(treeEvent);
     lastTree=treeEvent;
    }
    string name = strcluster + std::to_string(nclusters);
    TH2F *map= new TH2F(name.c_str(),std::to_string(gammaE[iarray]).c_str(),7,0,7,7,0,7);
    for (unsigned i = 0; i < kNTOWERS; ++i)
    {
      map->SetBinContent(i/7,i%7,towers[i][iarray]);
    }
    map->Write();
    if(iarray<clusn-1) iarray++;
    else{
      iarray=0;
      treeEvent++;
    }
    nclusters--;
  }
}



void ClusterPlotter(){
	string inName="/sphenix/user/vassalli/idTest/HIsample/hipytha_ANA0.root";
	TFile *f_data = new TFile("towerData.root","UPDATE");
	TChain *tree = new TChain("subtractedTree");
	tree->Add(inName.c_str());
	makeMaps(tree,"",5);
}