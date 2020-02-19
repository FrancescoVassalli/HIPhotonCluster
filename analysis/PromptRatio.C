int PromptRatio(){
  TFile* output = new TFile("promptana.root","CREATE");
  TChain *tree = new TChain("tree");
  tree->Add("pythdata.root");
  TH1F *pHist = new TH1F("pHist","",2,-.5,1.5);
  tree->Draw("prompt >> pHist","pid==22");
  pHist->Scale(1/pHist->Integral());
  //pHist->Draw();
  pHist->Write();
  //output->Close();
  return 0;
}
