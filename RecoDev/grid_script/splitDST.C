#include <TSystem.h>
R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libonlmonserver)
R__LOAD_LIBRARY(libdecoder_maindaq)

int splitDST(const int nEvents = 1){
 Fun4AllServer* se = Fun4AllServer::instance();
 se->Verbosity(0);
 Fun4AllInputManager* in = new Fun4AllDstInputManager("DSTIN");
 in->Verbosity(0); 
 in->fileopen("/data2/e1039/dst/run_001838_spin.root");
 se->registerInputManager(in);

  Fun4AllSpillDstOutputManager* out_sp = new Fun4AllSpillDstOutputManager("spill");
  out_sp->SetSpillStep(750);
  se->registerOutputManager(out_sp);

  se->run(nEvents);

  // finish job - close and save output files
  se->End();
  se->PrintTimer();
  std::cout << "All done" << std::endl;

  delete se;
  gSystem->Exit(0);


return 0;
}
