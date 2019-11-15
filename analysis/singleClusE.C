#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>

struct Average{
  double value;
  vector<double> *allVals;
  int n;
  Average(){
    value=0;
    n=0;
    allVals=new vector<double>();
  }
  //allv should include v
  Average(double v,int n=1,vector<double> *allv=NULL){
    value=v;
    this->n=n;
    allVals=allv;
    if(!allVals) allVals=new vector<double>();
  }
  Average(float v,int n=1){
    value=(double)v;
    this->n=n;
    allVals=new vector<double>();
  }
  Average operator +(double v){
    allVals->push_back(v);
    return Average((value*n)/(n+1)+v/(n+1),n+1,allVals);
  }
  Average operator +(float fv){
    double v = (double) fv;
    allVals->push_back(v);
    return Average((value*n)/(n+1)+v/(n+1),n+1,allVals);
  }
  void operator +=(double v){
    if(!allVals) allVals=new vector<double>();
    allVals->push_back(v);
    value=(value*n)/(n+1)+v/(n+1);
    ++n;
  }
  void operator +=(float fv){
    if(!allVals) allVals=new vector<double>();
    double v = (double) fv;
    allVals->push_back(v);
    value=(value*n)/(n+1)+v/(n+1);
    ++n;
  }
  void operator =(Average a){
    value=a.value;
    allVals=a.allVals;
    n=a.n;
  }
  double getError(){
    if(n<=1) return 0;
    double cm2 =0;
    for(double x : *allVals){
      cm2+=(x-value)*(x-value);
    }
    double s = sqrt(cm2/(n-1));
    return s/sqrt(n);
  }
  //calculate error

};

void makeEspec(TTree* tree,string ext=""){
  int clusn;
  float gammaE[600];
  float clusE[600];
  float clusEcore[600];

  tree->SetBranchAddress("tphoton_e",gammaE);
  tree->SetBranchAddress("sub_clus_n",&clusn);
  tree->SetBranchAddress("sub_clus_e",clusE);
  tree->SetBranchAddress("sub_clus_ecore",clusEcore);

  const int kBINS=40;
  string name = "eSpec";
  name+=ext;
  //the avergae cluster energy at each photon e
  TH1F* eSpec = new TH1F(name.c_str(),"",kBINS,0,kBINS+1);
  name = "eCoreSpec";
  name+=ext;
  TH1F* eCoreSpec = new TH1F(name.c_str(),"",kBINS,0,kBINS+1);
  name = "eff";
  name+=ext;
  TH1F* eff = new TH1F(name.c_str(),"",kBINS,0,kBINS+1);

  eSpec->Sumw2();
  eCoreSpec->Sumw2();
  eff->Sumw2();

  //used to calculate the average
  vector<Average> v_average(kBINS);
  vector<Average> v_average_core(kBINS);

  //the distribution of cluster energy for a photon E range
  TH1F* eDist[3];
  for(unsigned int count=0; count<3; count++){
    name="dist";
    name+=ext;
    string bound1=to_string(5+10*count);
    string bound2=to_string(5+10*(count+1));
    name+=bound1;name+="_";name+=bound2;
    eDist[count] = new TH1F(name.c_str(),"",kBINS,0,kBINS+1);
    eDist[count]->Sumw2();
    cout<<"here"<<endl;
  }

  for(unsigned event=0; event<tree->GetEntries();event++){
    tree->GetEntry(event);
    for(unsigned i=0; i<clusn; i++){
      if((int)gammaE[i]>=kBINS) continue;
      v_average[(int)gammaE[i]]+=clusE[i];
      v_average_core[(int)gammaE[i]]+=clusEcore[i];
      if(gammaE[i]<15){
        eDist[0]->Fill(clusE[i]);
      }
      else if(gammaE[i]<25){
        eDist[1]->Fill(clusE[i]);
      }
      else{
        eDist[2]->Fill(clusE[i]);
      }
      eff->Fill(clusE[i]/gammaE[i]);
    }
  }
  for(unsigned bin=1;bin<kBINS+1;bin++){
    eSpec->SetBinContent(bin,v_average[bin-1].value);
    eSpec->SetBinError(bin,v_average[bin-1].getError());
    eCoreSpec->SetBinContent(bin,v_average_core[bin-1].value);
    eCoreSpec->SetBinError(bin,  v_average_core[bin-1].getError());
  }
  eSpec->Write();
  eCoreSpec->Write();
  eff->Write();
  for(TH1F* plot : eDist){
    plot->Write();
  }
}


int singleClusE(){
  string inName="/sphenix/user/vassalli/idTest/singlesample/gana.root";
  TFile *f_data = new TFile("anadata.root","UPDATE");
  TChain *singleTree = new TChain("subtractedTree");
  singleTree->Add(inName.c_str());
  makeEspec(singleTree);

  inName="/sphenix/user/vassalli/idTest/HIsample/HIana.root";
  TChain *hiTree = new TChain("subtractedTree");
  hiTree->Add(inName.c_str());
  makeEspec(hiTree,"HI");

  inName="/sphenix/user/vassalli/idTest/HIsample/subana.root";
  TChain *subTree = new TChain("subtractedTree");
  hiTree->Add(inName.c_str());
  makeEspec(subTree,"sub");
  return 0;
}
