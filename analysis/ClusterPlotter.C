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
    name+=ext;
    string title = to_string(clusE[iarray]);
    title+=" GeV p=";
    title+=to_string(prob[iarray]);
    std::pair<int,int> loc;
    TH2F *map= new TH2F(name.c_str(),title.c_str(),7,-.5,6.5,7,-.5,6.5);
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

int DrawClusters(TFile* input){
  input->ReOpen("READ");
  const unsigned ncluster=3;
  string sclus= "cluster";
  gStyle->SetPaintTextFormat("1.3f");
  gStyle->SetOptStat(0);
  for(unsigned iclus=0;iclus<ncluster;iclus++){
    string name = sclus;
    name+=to_string(iclus+1);
    name+="s";
    cout<<"getting "<<name<<'\n';
    TH2F *thisClus = (TH2F*) input->Get(name.c_str());
    TCanvas *tc = new TCanvas();
    thisClus->Draw("colz text");
  }
/*  for(unsigned iclus=0;iclus<ncluster;iclus++){
    string name = sclus;
    name+=to_string(iclus+1);
    name+="b";
    TH2F *thisClus = (TH2F*) input->Get(name.c_str());
    TCanvas *tc = new TCanvas();
    thisClus->Draw("colz text");
  }*/
  return 0;
}



void ClusterPlotter(){
	//string inName="/sphenix/user/vassalli/idTest/singlesample/backana.root";
	//TFile *f_data = new TFile("btowerData.root","CREATE");
	string inName="../algoTesting/intana.root";
	TFile *f_data = new TFile("towerData.root","CREATE");
	TChain *tree = new TChain("subtractedTree");
	TChain *treeb = new TChain("subtractedTree");
	tree->Add(inName.c_str());
	makeMaps(tree,"s",3);
	inName="../algoTesting/backana.root";
	treeb->Add(inName.c_str());
  makeMaps(treeb,"b",3);
  f_data->Close();
  DrawClusters(f_data);
}
