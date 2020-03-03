

int Pi0Decay(){
  TRandom2 *random = new TRandom2();
  Double_t masses[2] = {0,0} ;
  TGenPhaseSpace event;
  TFile *output = new TFile("decays.root","CREATE");
  TTree *tree = new TTree("decays","");
  double angle;
  double dist;
  double weight;
  tree->Branch("dist",&dist);
  for(unsigned i=0; i<1000;i++){
    TLorentzVector ptlv;
    ptlv.SetPtEtaPhiM(15,random->Uniform(-1,1),random->Uniform(0,2*TMath::Pi()),.135);
    event.SetDecay(ptlv,2,masses);
    weight = event.Generate();
    TLorentzVector *tlv1 = event.GetDecay(0);
    TLorentzVector *tlv2 = event.GetDecay(1);
    angle = tlv1->Angle(tlv2->Vect());
    dist = 100*TMath::Tan(angle); // 100cm propogation 
    tree->Fill();
  }
  cout<<"did loop"<<endl;
  tree->Write();
  output->Close();

  return 0;
}
