/// Fun4MainDaq.C:  Fun4all macro to decode the MainDAQ data.
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
R__LOAD_LIBRARY(libana_real_dst)
R__LOAD_LIBRARY(libktracker)
#endif

/// Main function.
int Fun4RealDst(const int run=1666, const int nevent=0)
{
  gSystem->Load("libana_real_dst.so");

  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);

//  string fn_in = UtilOnline::GetDstFilePath(run);
//  cout << "DST file = " << fn_in << endl;
//  Fun4AllInputManager *in = new Fun4AllDstInputManager("RealDst");
//  in->fileopen(fn_in);
//  se->registerInputManager(in);

  /// Run 28707: The last E906 run taken with KMag on.
  /// https://e906-gat1.fnal.gov/data-summary/run.php?run_begin=28680&run_end=28750
  /// 61194 events (where the FPGA2 events were mostly excluded)
  /// NIM2 prescale = 1 (!?)

  Fun4AllSRawEventInputManager* in = new Fun4AllSRawEventInputManager("SRawEventIM");
  in->Verbosity(0);
  in->enable_E1039_translation();
  //in->set_tree_name("save");
  //in->set_branch_name("rawEvent");
  //in->fileopen("support/digit_028692_009.root");
  in->fileopen("/pnfs/e906/production/digit/R009/02/87/digit_028707_009.root");
  se->registerInputManager(in);

  se->registerSubsystem(new AnaRealDst());
  //se->registerSubsystem(new AnaEffHodo());
  //se->registerSubsystem(new AnaEffCham(AnaEffCham::D3p));

  se->run(nevent);
  se->End();
  delete se;
  return 0;
}

/// Function to analyze multiple DST files.
/**
 * You first list up runs in "list_run.txt".
 * Then type the following commands to execute this function:
 *   root -b
 *   .L Fun4RealDst.C
 *   Fun4MultiRealDst();
 */
int Fun4MultiRealDst(const char* fn_list_run="list_run.txt")
{
  gSystem->Load("libana_real_dst.so");
  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("RealDst");
  se->registerInputManager(in);

  se->registerSubsystem(new AnaEffHodo());
  //se->registerSubsystem(new AnaEffCham(AnaEffCham::D3p));

  ifstream ifs(fn_list_run);
  int run;
  while (ifs >> run) {
    string fn_in = UtilOnline::GetDstFileDir() + "/" + UtilOnline::RunNum2DstFile(run);
    cout << "Run " << run << ": " << fn_in << endl;
    in->fileopen(fn_in);
    se->run();
  }
  ifs.close();

  se->End();
  delete se;
  return 0;
}
