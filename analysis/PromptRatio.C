int PromptRatio(){
  TFile* output = new TFile("promptana.root","CREATE");
  TChain *tree = new TChain("tree");
  tree->Add("pythdata.root");
  TH1F *all = new TH1F("all","",5,13,22);
  tree->Draw("energy >> all","pid==22");
  TH1F *prompt = new TH1F("prompt","",5,13,22);
  tree->Draw("energy >> prompt","prompt==1");
  TEfficiency* promptRate = new TEfficiency(*prompt,*all);
  promptRate->Write();
  prompt->Write();
  all->Write();
  promptRate->Draw();
  //output->Close();
  return 0;
}
