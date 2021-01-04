#ifndef _ANA_REAL_DST__H_
#define _ANA_REAL_DST__H_
#include <fun4all/SubsysReco.h>
class TFile;
class TTree;
class TH1;
class TH2;

/// An example class to analyze the E1039 DST file.
class AnaRealDst: public SubsysReco {
  static const std::vector<std::string> list_det_name;
  std::vector<int> list_det_id;
  bool use_trig_hit;

  TFile* f_out;
  TTree* tree;
  int    b_run;
  int    b_evt;
  int    b_inte_max;
  int    b_fpga1_0;
  int    b_fpga1_1;
  int    b_fpga1_2;

  TH1*   h1_evt_cnt;
  TH1*   h1_ele[99];
  TH1*   h1_ele_all[99];
  TH1*   h1_time[99];
  TH1*   h1_time_all[99];
  TH1*   h1_nhit[99];
  TH1*   h1_nhit_all[99];

 public:
  AnaRealDst();
  virtual ~AnaRealDst() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

  void use_trigger_hit() { use_trig_hit = true; }
};

#endif /* _ANA_REAL_DST__H_ */
