#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <algorithm>

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
  ~Average(){
    if(s){
      delete s;
      s=NULL;
    }
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
    return getS()/sqrt(n);
  }
  double getS(){
    double cm2 =0;
    for(double x : *allVals){
      cm2+=(x-value)*(x-value);
    }
    if(!s) s= new double(sqrt(cm2/(n-1)));
    else *s = sqrt(cm2/(n-1));
    return *s;
  }
  double getSError(){ 
    if(n<=1) return 0;
    static const float tolAdjust =.0001; //this is a hyper paramater for how the tolerance is made 
    TH1D *toy = new TH1D("toy","",1,*std::min_element(allVals->begin(),allVals->end()),*std::max_element(allVals->begin(),allVals->end()));
    for(double x : *allVals){
      toy->Fill(x);
    }
    Average error;
    double tolerance=0;
    double lastError=-1.;
    double change=-1.;
    do{
      //draw a random sample
      Average sample;
      for (int i = 0; i < n; ++i)
      {
        sample+=toy->GetRandom();
      }
      //calculate the statistic
      double ierror = sample.getS();
      //add the statistic to an average
      error+=ierror;
      //get the error on the average
      //find out how much the error changed 
      if (lastError>0)
      {
        change=std::fabs(error.getError()-lastError);
        tolerance=error.getError()*tolAdjust;
      }
      lastError=error.getError();
      // if the error changed less than the tolerance do this again 
    }while(change<0||change>tolerance);
    delete toy;
    return error.getS();
  }

  private:
  double *s=NULL;

};

void makeID(TChain *tree,string ext=""){
	int clusn;
  float gammaE[600];
  float gammapT[600];
  float clusE[600];
  float clusEcore[600];
  float matchDR[600];
  float matchDPhi[600];
  float matchDEta[600];
  float prob[600];

  tree->SetBranchAddress("tphoton_e",gammaE);
  tree->SetBranchAddress("tphoton_pT",gammapT);
  tree->SetBranchAddress("sub_clus_n",&clusn);
  tree->SetBranchAddress("sub_clus_e",clusE);
  tree->SetBranchAddress("sub_clus_ecore",clusEcore);
  tree->SetBranchAddress("matchDR",matchDR);
  tree->SetBranchAddress("matchPhi",matchDPhi);
  tree->SetBranchAddress("matchEta",matchDEta);
  tree->SetBranchAddress("sub_clus_prob",prob);

  const int kBINS=40;

  string name = "probE";
  name+=ext;
  //the average chi2 at each truth E
  TH1F* probE = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);
  probE->Sumw2();
  vector<Average> v_probE(kBINS);

  name = "probpT";
  name+=ext;
  //the average chi2 at each truth pT
  TH1F* probpT = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);
  probpT->Sumw2();
  vector<Average> v_probpT(kBINS);

  name = "badEta";
  name+=ext;
  TH1F* badEta = new TH1F(name.c_str(),"",kBINS,0,1.1);
  badEta->Sumw2();

  std::vector<TH1F*> v_slice;
  for (int i = 0; i < kBINS; ++i)
  {
  	name = "slice";
  	name+=std::to_string(i);
  	name+=ext;
  	TH1F* thisSlice = new TH1F(name.c_str(),"",kBINS,0,1);
  	thisSlice->Sumw2();
  	v_slice.push_back(thisSlice);
  }

  for(unsigned event=0; event<tree->GetEntries();event++){
    tree->GetEntry(event);
    for(unsigned i=0; i<clusn; i++){
      if(gammaE[i]<0||(int)gammaE[i]>=kBINS) continue;
      v_probE[(int)(gammaE[i])]+=prob[i];
      v_probpT[(int)(gammapT[i])]+=prob[i];
      v_slice[(int)(gammapT[i])]->Fill(prob[i]);
      if (prob[i]<.1)
      {
      	badEta->Fill(clusE[i]/gammaE[i]);
      }
    }
  }
  for(unsigned bin=1;bin<kBINS+1;bin++){
    probE->SetBinContent(bin,v_probE[bin-1].value);
    probE->SetBinError(bin,v_probE[bin-1].getError());
    probpT->SetBinContent(bin,v_probpT[bin-1].value);
    probpT->SetBinError(bin,v_probpT[bin-1].getError());
  }
  probE->Write();
  probpT->Write();
  badEta->Write();
  for(TH1F* plot : v_slice){
  	plot->Write();
  }
}



void IDsingle(){
	string inName="/sphenix/user/vassalli/idTest/singlesample/gana.root";
	TFile *f_data = new TFile("iddata.root","UPDATE");
	TChain *singleTree = new TChain("subtractedTree");
	singleTree->Add(inName.c_str());
	makeID(singleTree);

  inName="/sphenix/user/vassalli/idTest/HIsample/HIana.root";
  TChain *hiTree = new TChain("subtractedTree");
  hiTree->Add(inName.c_str());
  makeID(hiTree,"HI");

  inName="/sphenix/user/vassalli/idTest/HIsample/subana.root";
  TChain *subTree = new TChain("subtractedTree");
  subTree->Add(inName.c_str());
  makeID(subTree,"sub");

  /*inName="/sphenix/user/vassalli/idTest/HIsample/flowana.root";
  TChain *flowTree = new TChain("subtractedTree");
  flowTree->Add(inName.c_str());
  makeEspec(flowTree,"flow");*/

}
