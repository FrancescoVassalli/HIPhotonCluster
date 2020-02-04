#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include "GammaLocal.h"
#include "AllClusBurner.h"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libsubtractionburner.so)

int pythMacro(std::string infile = "XjPhi3_pT5_98_dst.root",std::string outfile="embedanalysis.root",int runNumber=0,bool HI=false)
{
  
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libphhepmc.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4eval.so");
  gSystem->Load("libcalotrigger.so");
  gSystem->Load("libsubtractionburner.so");

  int verbosity = 10;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(verbosity);
  // just if we set some flags somewhere in this macro
  //recoConsts *rc = recoConsts::instance();

  Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
  hitsin->fileopen( infile );
  se->registerInputManager(hitsin);
 
  AllClusBurner *rCE = new AllClusBurner(outfile,runNumber,HI);
  se->registerSubsystem( rCE );

  se->run();
  se->End();
  delete se;
  //gSystem->Exit(0);
  std::cout << "All done" << std::endl;
  return 0;
}
