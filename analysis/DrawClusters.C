



int DrawClusters(){
  TFile *input = new TFile("towerData.root","READ");
  const unsigned ncluster=5;
  string sclus= "cluster";
  gStyle->SetPaintTextFormat("1.3f");
  gStyle->SetOptStat(0);
  for(unsigned iclus=0;iclus<ncluster;iclus++){
    string name = sclus;
    name+=to_string(iclus+1);
    TH2F *thisClus = (TH2F*) input->Get(name.c_str());
    TCanvas *tc = new TCanvas();
    thisClus->Draw("colz text");
  }
  return 0;
}
