



int DrawDecay(){
  //TFile *input = new TFile("btowerData.root","READ");
  TFile *input = new TFile("decays.root","READ");
  TTree *tree  = (TTree*) input->Get("decays");
  TH1F *h = new TH1F("low","15 GeV Decay Distribution",20,0,20);
  tree->Draw("dist >> low");
  return 0;
}
