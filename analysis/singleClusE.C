
struct Average{
  double value;
  vector<double> *allVals;
  int n;
  Average(){
    value=0;
    n=0;
  }
  //allv should include v
  Average(double v,int n=1,vector<double> *allv=NULL){
    value=v;
    this->n=n;
    allVals=allv;
  }
  Average(float v,int n=1){
    value=(double)v;
    this->n=n;
    allVals=NULL;
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
    allVals->push_back(v);
    value=(value*n)/(n+1)+v/(n+1);
    ++n;
  }
  void operator +=(float fv){
    double v = (double) fv;
    allVals->push_back(v);
    value=(value*n)/(n+1)+v/(n+1);
    ++n;
  }
  //calculate error

};




void makeEspec(TTree* tree){
  int clusn;
  float gammaE;
  float clusE;
  float clusEcore;

  tree->SetBranchAddress("tphoton_e",&gammaE);
  tree->SetBranchAddress("sub_clus_n",&clusn);
  tree->SetBranchAddress("sub_clus_e",&clusE);
  tree->SetBranchAddress("sub_clus_ecore",&clusEcore);

  TH1F* eSpec = new TH1F("eSpec","",35,0,36);

  for(unsigned event=0; event<tree->GetEntries();event++){
    tree->GetEntry(event);
    for(unsigned i=0; i<clusn; i++){

    }
  }
}


int singleClusE(){
  string inName="";
  TFile f_data = new TFile("anadata.root","UPDATE");
  TChain *singleTree = new TChain("subtractedTree");
  singleTree->AddEntry(inName.c_str());

  
  return 0;
}
