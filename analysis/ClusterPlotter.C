#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <utility>

pair<int,int> getTowerEnergy(unsigned mapPosition){
  unsigned xPos = (unsigned) mapPosition / 7;
  unsigned yPos = mapPosition % 7;
  return pair<int,int>(xPos,yPos);
}

void makeMaps(TChain *tree,string ext="",unsigned nclusters=0){
  const unsigned kNTOWERS = 49;
	int clusn;
  float clusE[600];
  float prob[600];
  float towers[kNTOWERS][600];

  tree->SetBranchAddress("clus_n",&clusn);
  tree->SetBranchAddress("clus_e",clusE);
  tree->SetBranchAddress("clus_prob",prob);

  string bTitle="tower";
  string bStruct = "[clus_n]/F";
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
    if(clusn==0){
      treeEvent++;
      continue;
    }
    string name = strcluster + std::to_string(nclusters);
    std::pair<int,int> loc;
    TH2F *map= new TH2F(name.c_str(),"",7,-.5,6.5,7,-.5,6.5);
    for (unsigned i = 0; i < kNTOWERS; ++i)
    {
      loc = getTowerEnergy(i);
      map->SetBinContent(loc.first+1,loc.second+1,towers[i][iarray]);
    }
    map->Write();
    if(iarray<clusn-1) iarray++;
    else{
      iarray=0;
      treeEvent++;
    }
    nclusters--;
  }
  /*TH2F *test = new TH2F("test","",7,-.5,6.5,7,-.5,6.5);
  test->SetBinContent(4,4,1);
  test->Write();*/
}



void ClusterPlotter(){
	string inName="/sphenix/user/vassalli/idTest/HIsample/intana.root";
	TFile *f_data = new TFile("stowerData.root","CREATE");
	TChain *tree = new TChain("subtractedTree");
	tree->Add(inName.c_str());
	makeMaps(tree,"",5);
}
