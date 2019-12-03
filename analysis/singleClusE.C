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
        change=std::abs(error.getError()-lastError);
        tolerance=error.getError()*tolAdjust;
      }
      lastError=error.getError();
      // if the error changed less than the tolerance do this again 
    }while(change<0||change>tolerance);
    delete toy;
    return error.getError();
  }

  private:
  double *s=NULL;

};

void makeEspec(TTree* tree,string ext=""){
  int clusn;
  float gammaE[600];
  float clusE[600];
  float clusEcore[600];
  float matchDR[600];
  float matchDPhi[600];
  float matchDEta[600];

  tree->SetBranchAddress("tphoton_e",gammaE);
  tree->SetBranchAddress("sub_clus_n",&clusn);
  tree->SetBranchAddress("sub_clus_e",clusE);
  tree->SetBranchAddress("sub_clus_ecore",clusEcore);
  tree->SetBranchAddress("matchDR",matchDR);
  tree->SetBranchAddress("matchPhi",matchDPhi);
  tree->SetBranchAddress("matchEta",matchDEta);

  const int kBINS=40;
  string name = "eSpec";
  name+=ext;
  //the avergae cluster energy at each photon e
  TH1F* eSpec = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);
  name = "eCoreSpec";
  name+=ext;
  TH1F* eCoreSpec = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);
  name = "eRes";
  name+=ext;
  TH1F *eRes = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);
  name = "responseRes";
  name+=ext;
  TH1F *responseRes = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);
  name = "eResCore";
  name+=ext;
  TH1F *eCoreRes = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);

  eSpec->Sumw2();
  eCoreSpec->Sumw2();
  eRes->Sumw2();
  eCoreRes->Sumw2();
  responseRes->Sumw2();             

  //used to calculate the average
  vector<Average> v_average(kBINS);
  vector<Average> v_average_core(kBINS);

  //the distribution of cluster energy for a photon E range
  TH1F* eDist[kBINS];
  for(unsigned int count=0; count<kBINS; count++){
    name="dist";
    name+=std::to_string(count);
    name+=ext;
    //string bound1=to_string(5+10*count);
    //string bound2=to_string(5+10*(count+1));
    //name+=bound1;name+="_";name+=bound2;
    eDist[count] = new TH1F(name.c_str(),"",kBINS,0,kBINS+1);
    eDist[count]->Sumw2();
  }

  //show the response distribution of cluster energy for a photon E range
  TH1F* response[kBINS];
  for(unsigned int count=0; count<kBINS; count++){
    name="response";
    name+=ext;
    name+=to_string(count);
    response[count] = new TH1F(name.c_str(),"",kBINS,0,2);
    response[count]->Sumw2();
  }

  name="DR"; name+=ext;
  TH1F* dRDist = new TH1F(name.c_str(),"",kBINS,0,.1);
  name="DEtaPhi"; name+=ext;
  TH2F* dEtaPhi = new TH2F(name.c_str(),"",kBINS,0,.1,kBINS,0,.1);

  name="DRlowpt"; name+=ext;
  TH1F* dRLowPt = new TH1F(name.c_str(),"",kBINS,0,.1);

  name="DRhighpt"; name+=ext;
  TH1F* dRHighPt = new TH1F(name.c_str(),"",kBINS,0,.1);

  dRDist->Sumw2();
  dRLowPt->Sumw2();

  //calculate average response at each truth e
  vector<Average> v_response(kBINS);
  name="Response_E";
  name+=ext;
  TH1F *response_e = new TH1F(name.c_str(),"",kBINS,-.5,kBINS+.5);
  response_e->Sumw2();

  for(unsigned event=0; event<tree->GetEntries();event++){
    tree->GetEntry(event);
    for(unsigned i=0; i<clusn; i++){
      if(gammaE[i]<0||(int)gammaE[i]>=kBINS) continue;

      v_average[(int)(gammaE[i])]+=clusE[i];
      v_average_core[(int)(gammaE[i])]+=clusEcore[i];
      response[(int)(gammaE[i])]->Fill( clusE[i]/gammaE[i]);
      v_response[(int)(gammaE[i])]+=clusE[i]/gammaE[i];
      eDist[(int)(gammaE[i])]->Fill(clusE[i]);
      /*if(gammaE[i]<15){
        eDist[0]->Fill(clusE[i]);
        }
        else if(gammaE[i]<25){
        eDist[1]->Fill(clusE[i]);
        }
        else{
        eDist[2]->Fill(clusE[i]);
        }*/
      dRDist->Fill(matchDR[i]);
      dEtaPhi->Fill(matchDEta[i],matchDPhi[i]);
      if(gammaE[i]<15) dRLowPt->Fill(matchDR[i]);
      else dRHighPt->Fill(matchDR[i]);
    }
  }

  for(unsigned bin=1;bin<kBINS+1;bin++){
    eSpec->SetBinContent(bin,v_average[bin-1].value);
    eSpec->SetBinError(bin,v_average[bin-1].getError());
    eCoreSpec->SetBinContent(bin,v_average_core[bin-1].value);
    eCoreSpec->SetBinError(bin,  v_average_core[bin-1].getError());
    response_e->SetBinContent(bin,v_response[bin-1].value);
    response_e->SetBinError(bin, v_response[bin-1].getError());
    eRes->SetBinContent(bin,v_average[bin-1].getS());
    eRes->SetBinError(bin,v_average[bin-1].getSError());
    responseRes->SetBinContent(bin,v_response[bin-1].getS());
    responseRes->SetBinError(bin,v_response[bin-1].getSError());
    eCoreRes->SetBinContent(bin,v_average[bin-1].getS());
    eCoreRes->SetBinError(bin,v_average[bin-1].getSError());
    cout<<"Energy bin with mu = "<<v_average[bin-1].value<<" and sigma = "<<v_average[bin-1].getS()
      <<"\n\tResponse bin with mu = "<<v_response[bin-1].value<<" and sigma = "<<v_response[bin-1].getS()<<'\n';
  }
  eSpec->Write();
  eCoreSpec->Write();
  responseRes->Write();
  response_e->Write();
  eRes->Write();
  eCoreRes->Write();
  for(TH1F* plot : eDist){
    plot->Write();
  }
  for (TH1F* plot : response)
  {
    plot->Write();
  }
  dRDist->Write();
  dRLowPt->Write();
  dRHighPt->Write();
  dEtaPhi->Write();
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
  subTree->Add(inName.c_str());
  makeEspec(subTree,"sub");

  return 0;
}
