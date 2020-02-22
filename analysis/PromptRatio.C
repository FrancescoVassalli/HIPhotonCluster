int PromptRatio(){
  TFile* output = new TFile("promptana.root","CREATE");
  TChain *tree = new TChain("tree");
  tree->Add("pythdata.root");
  TH1F *all = new TH1F("all","",8,5,20);
  tree->Draw("energy >> all","pid==22");
  TH1F *prompt = new TH1F("prompt","",8,5,20);
  tree->Draw("energy >> prompt","prompt==1");
  TEfficiency* promptRate = new TEfficiency(prompt,all);
  promptRate->Draw();
  promptRate->Write();
  //output->Close();
  return 0;
}
