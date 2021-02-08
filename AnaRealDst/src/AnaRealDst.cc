#include <iomanip>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQHitVector.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <geom_svc/GeomSvc.h>
#include "UtilSQHit.h"
#include "AnaRealDst.h"
using namespace std;

//const vector<string> AnaRealDst::list_det_name = { "H1T", "H1B", "H1L", "H1R" };
const vector<string> AnaRealDst::list_det_name = { "H1T", "H1B", "H2T", "H2B", "H3T", "H3B", "H4T", "H4B" };

AnaRealDst::AnaRealDst()
  : use_trig_hit(false)
{
  ;
}


int AnaRealDst::Init(PHCompositeNode* topNode)
{
  f_out = new TFile("output.root", "RECREATE");
  tree  = new TTree("tree", "Created by AnaRealDst");
  tree->Branch("run"     , &b_run     ,      "run/I");
  tree->Branch("evt"     , &b_evt     ,      "evt/I");
  tree->Branch("inte_max", &b_inte_max, "inte_max/I");
  tree->Branch("fpga1_0" , &b_fpga1_0 ,  "fpga1_0/I");
  tree->Branch("fpga1_1" , &b_fpga1_1 ,  "fpga1_1/I");
  tree->Branch("fpga1_2" , &b_fpga1_2 ,  "fpga1_2/I");

  h1_evt_cnt = new TH1D("h1_evt_cnt", "", 20, 0.5, 20.5);

  ostringstream oss;
  GeomSvc* geom = GeomSvc::instance();
  for (unsigned int i_det = 0; i_det < list_det_name.size(); i_det++) {
    string name = list_det_name[i_det];
    int id = geom->getDetectorID(name);
    if (id <= 0) {
      cerr << "!ERROR!  AnaRealDst::Init():  Invalid ID (" << id << ").  Probably the detector name that you specified in 'list_det_name' (" << name << ") is not valid.  Abort." << endl;
      exit(1);
    }
    list_det_id.push_back(id);
    int n_ele = geom->getPlaneNElements(id);
    cout << "  " << setw(6) << name << " = " << id << endl;

    oss.str("");
    oss << "h1_ele_" << name;
    h1_ele[i_det] = new TH1D(oss.str().c_str(), "", n_ele, 0.5, n_ele+0.5);
    oss.str("");
    oss << name << ";Element ID;Hit count";
    h1_ele[i_det]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_ele_all_" << name;
    h1_ele_all[i_det] = new TH1D(oss.str().c_str(), "", n_ele, 0.5, n_ele+0.5);
    oss.str("");
    oss << name << ";Element ID;Hit count";
    h1_ele_all[i_det]->SetTitle(oss.str().c_str());

    int    TN = use_trig_hit ? 200 : 450 ;
    double T0 = use_trig_hit ? 690 : 2150.5*4/9 ;
    double T1 = use_trig_hit ? 890 : 2600.5*4/9 ;

    oss.str("");
    oss << "h1_time_" << name;
    h1_time[i_det] = new TH1D(oss.str().c_str(), "", TN, T0, T1);
    oss.str("");
    oss << name << ";TDC time (nsec);Hit count";
    h1_time[i_det]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_time_all_" << name;
    h1_time_all[i_det] = new TH1D(oss.str().c_str(), "", TN, T0, T1);
    oss.str("");
    oss << name << ";TDC time (nsec);Hit count";
    h1_time_all[i_det]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_nhit_" << name;
    h1_nhit[i_det] = new TH1D(oss.str().c_str(), "", 20, -0.5, 19.5);
    oss.str("");
    oss << name << ";N of hits/plane/event;Hit count";
    h1_nhit[i_det]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_nhit_all_" << name;
    h1_nhit_all[i_det] = new TH1D(oss.str().c_str(), "", 20, -0.5, 19.5);
    oss.str("");
    oss << name << ";N of hits/plane/event;Hit count";
    h1_nhit_all[i_det]->SetTitle(oss.str().c_str());
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::InitRun(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::process_event(PHCompositeNode* topNode)
{
  h1_evt_cnt->Fill(1);

  SQEvent* event       = findNode::getClass<SQEvent    >(topNode, "SQEvent");
  SQEvent* event1      = findNode::getClass<SQEvent    >(topNode, "SQEventDPTA1");
  SQEvent* event2      = findNode::getClass<SQEvent    >(topNode, "SQEventDPTA2");

  SQHitVector* hit_vec;
  if (use_trig_hit) hit_vec = findNode::getClass<SQHitVector>(topNode, "SQTriggerHitVector");
  else              hit_vec = findNode::getClass<SQHitVector>(topNode, "SQHitVector");

  if (!event || !hit_vec) return Fun4AllReturnCodes::ABORTEVENT;
  //int spill_id = event->get_spill_id();
  //int event_id = event->get_event_id();

  h1_evt_cnt->Fill(2);

  static vector<int> list_spill_ok;
  if (list_spill_ok.size() == 0) {
    cout << "Read the good-spill list." << endl;
    ifstream ifs("list_spill_good.txt");
    int sp;
    while (ifs >> sp) list_spill_ok.push_back(sp);
    ifs.close();
    if (list_spill_ok.size() == 0) {
      cout << "  No good spill was found.  Abort." << endl;
      exit(1);
    }
  }
  int spill_id = event->get_spill_id();
  if (find(list_spill_ok.begin(), list_spill_ok.end(), spill_id) == list_spill_ok.end()) return Fun4AllReturnCodes::EVENT_OK;

  h1_evt_cnt->Fill(3);

  ///
  /// Event selection
  ///
  if (! event->get_trigger(SQEvent::NIM3)) { // SQEvent::MATRIX1
    return Fun4AllReturnCodes::EVENT_OK;
  }

  h1_evt_cnt->Fill(4);

  int inte_max = 0;
  for (int ii = -8; ii <= 8; ii++) {
    int inte = event->get_qie_rf_intensity(ii);
    //cout << " " << inte;
    if (inte > inte_max) inte_max = inte;
  }
  //cout << " " << inte_max << endl;


  if (event->get_trigger(SQEvent::MATRIX1)) h1_evt_cnt->Fill(5);
  if (event->get_trigger(SQEvent::MATRIX2)) h1_evt_cnt->Fill(6);
  if (event1) {
    if (event1->get_trigger(SQEvent::MATRIX1)) h1_evt_cnt->Fill(7);
    if (event1->get_trigger(SQEvent::MATRIX2)) h1_evt_cnt->Fill(8);
  }
  if (event2) {
    if (event2->get_trigger(SQEvent::MATRIX1)) h1_evt_cnt->Fill( 9);
    if (event2->get_trigger(SQEvent::MATRIX2)) h1_evt_cnt->Fill(10);
  }

  b_run      = event->get_run_id();
  b_evt      = event->get_event_id();
  b_inte_max = inte_max;
  b_fpga1_0  = event ->get_trigger(SQEvent::MATRIX1);
  b_fpga1_1  = event1->get_trigger(SQEvent::MATRIX1);
  b_fpga1_2  = event2->get_trigger(SQEvent::MATRIX1);
  tree->Fill();

  ///
  /// Get & fill the hit info
  ///
  for (unsigned int i_det = 0; i_det < list_det_name.size(); i_det++) {
    int det_id = list_det_id[i_det];
    shared_ptr<SQHitVector> hv(UtilSQHit::FindHits(hit_vec, det_id));
    int n_intime = 0;
    for (SQHitVector::ConstIter it = hv->begin(); it != hv->end(); it++) {
      int  ele_id = (*it)->get_element_id();
      double time = (*it)->get_tdc_time  ();
      bool is_intime = (*it)->is_in_time();
      if (is_intime) {
        n_intime++;
        h1_ele [i_det]->Fill(ele_id);
        h1_time[i_det]->Fill(time  );
      }

      h1_ele_all [i_det]->Fill(ele_id);
      h1_time_all[i_det]->Fill(time  );
    }
    h1_nhit    [i_det]->Fill(n_intime);
    h1_nhit_all[i_det]->Fill(hv->size());
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::End(PHCompositeNode* topNode)
{
  f_out->cd();
  f_out->Write();
  f_out->Close();
  
  return Fun4AllReturnCodes::EVENT_OK;
}
