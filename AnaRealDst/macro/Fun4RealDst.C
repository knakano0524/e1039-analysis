/// Fun4MainDaq.C:  Fun4all macro to decode the MainDAQ data.
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
R__LOAD_LIBRARY(libana_real_dst)
R__LOAD_LIBRARY(libdptrigger)
R__LOAD_LIBRARY(libktracker)
#endif

/// Main function.
int Fun4RealDst(const int run, const string fname, const int nevent=0)
{
  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);

  const bool use_trigger_hit = false;

  Fun4AllSRawEventInputManager* in = new Fun4AllSRawEventInputManager("SRawEventIM");
  in->Verbosity(0);
  in->enable_E1039_translation();
  se->registerInputManager(in);

  DPTriggerAnalyzer* ta1 = new DPTriggerAnalyzer("DPTA1");
  ta1->set_road_set_file_name("$E1039_RESOURCE/trigger/trigger_67.txt");
  ta1->set_output_node_name("SQEventDPTA1");
  if (use_trigger_hit) ta1->use_trigger_hit();
  ta1->require_in_time(true);
  se->registerSubsystem(ta1);

  DPTriggerAnalyzer* ta2 = new DPTriggerAnalyzer("DPTA2");
  ta2->set_road_set_file_name("$E1039_RESOURCE/trigger/trigger_78.txt");
  ta2->set_output_node_name("SQEventDPTA2");
  if (use_trigger_hit) ta2->use_trigger_hit();
  ta2->require_in_time(true);
  se->registerSubsystem(ta2);

  AnaRealDst* ana = new AnaRealDst();
  if (use_trigger_hit) ana->use_trigger_hit();
  se->registerSubsystem(ana);

  in->fileopen(fname);
  se->run(nevent);
  se->End();
  delete se;
  return 0;
}
