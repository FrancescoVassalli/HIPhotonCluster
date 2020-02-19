int PromptRatio(){
  TFile* data = new TFile("pythadata.root","READ");
  TFile* output = new TFile("promptana.root","WRITE");
  TTree* tree = data->Get("tree");
  TH1F *pHist = new TH1F("pHist","",2,-.5,1.5);
  tree->Draw("prompt >> pHist","pid==22");
  pHist->Write();
  output->Close();
}
