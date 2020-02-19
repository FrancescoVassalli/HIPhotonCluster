#include "Pythia8/Pythia.h"
#include <TTree.h>
#include <TFile.h>
using namespace Pythia8;
using namespace std;


bool isPrompt(Event *e,unsigned index){
  if((*e)[index].id()!=22) return 0;
  if(index==5||index==6) return 1;
  return isPrompt(e,(*e)[index].mother1());
}

void generator(std::string filename, unsigned nEvents){

  /*pythia set up*/
  Pythia pythia;
  pythia.readString("Beams:eCM = 200.");
  pythia.readString("HardQCD:all = on");
  pythia.readString("PhaseSpace:pTHatMin = 5.");
  pythia.readString("Random::setSeed = on");
  pythia.readString("Random::seed =0");
  pythia.init();

  TFile *f = new TFile (filename.c_str() , "RECREATE");
  TTree *t = new TTree("tree","an isotropic pineapple tree");
  const int kMAXTRACKS = 1000;
  int ntracks;
  float energy[kMAXTRACKS];
  float eta[kMAXTRACKS];
  int pid[kMAXTRACKS];
  bool prompt[kMAXTRACKS];

  t->Branch ("ntracks", &ntracks);
  t->Branch ("energy", &energy,"energy[ntracks]/F");
  t->Branch ("eta", &eta,"eta[ntracks]/F");
  t->Branch ("pid", &pid,"pid[ntracks]/I");
  t->Branch ("prompt", &prompt,"prompt[ntracks]/I");

  for (unsigned iEvent = 0; iEvent < nEvents; ++iEvent)
  {
    if (!pythia.next()){
      cout<<"pythia.next() failed"<<"\n";
      iEvent--;
      continue;
    } 
    ntracks=0;
    for (int i = 0; i < pythia.event.size (); i++) {
      if (pythia.event[i].e() >= 5 && TMath::Abs(pythia.event[i].eta())<1&&pythia.event[i].isFinal()) {
        energy[ntracks]=pythia.event[i].pT ();
        eta[ntracks]=pythia.event[i].eta ();
        pid[ntracks]=pythia.event[i].id();
        prompt[ntracks++]=isPrompt(&pythia.event,i);
      }
    }
    if(ntracks==0) iEvent--;
    else t->Fill();
  }
  pythia.stat();
  t->Write();
  f->Close();
}

int main(int argc, char const *argv[] )
{
  string fileOut = string(argv[1]);
  long nEvents =strtol(argv[2],NULL,10);  // 5000000;
  generator(fileOut,nEvents);
  cout<<"All done"<<endl;
  return 0;
}
