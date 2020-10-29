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

const vector<string> AnaRealDst::list_det_name = { "H1T", "H1B", "H1L", "H1R" };

int AnaRealDst::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::InitRun(PHCompositeNode* topNode)
{
  f_out = new TFile("output.root", "RECREATE");
  tree  = new TTree("tree", "Created by AnaRealDst");
  tree->Branch("det_name", &b_det_name, "det_name/C");
  tree->Branch("det"     , &b_det     ,      "det/I");
  tree->Branch("ele"     , &b_ele     ,      "ele/I");
  tree->Branch("time"    , &b_time    ,     "time/D");

  h1_evt_cnt = new TH1D("h1_evt_cnt", "", 20, 0.5, 20.5);

  ostringstream oss;
  GeomSvc* geom = GeomSvc::instance();
  for (unsigned int i_det = 0; i_det < list_det_name.size(); i_det++) {
    string name = list_det_name[i_det];
    int id = geom->getDetectorID(name);
    if (id <= 0) {
      cerr << "!ERROR!  AnaRealDst::InitRun():  Invalid ID (" << id << ").  Probably the detector name that you specified in 'list_det_name' (" << name << ") is not valid.  Abort." << endl;
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

    oss.str("");
    oss << "h1_time_" << name;
    h1_time[i_det] = new TH1D(oss.str().c_str(), "", 200, 2190.5*4/9, 2390.5*4/9);
    oss.str("");
    oss << name << ";TDC time (nsec);Hit count";
    h1_time[i_det]->SetTitle(oss.str().c_str());

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

int AnaRealDst::process_event(PHCompositeNode* topNode)
{
  h1_evt_cnt->Fill(1);

  SQEvent* event       = findNode::getClass<SQEvent    >(topNode, "SQEvent");
  SQHitVector* hit_vec = findNode::getClass<SQHitVector>(topNode, "SQHitVector");
  if (!event || !hit_vec) return Fun4AllReturnCodes::ABORTEVENT;
  //int spill_id = event->get_spill_id();
  //int event_id = event->get_event_id();

  h1_evt_cnt->Fill(2);

  ///
  /// Event selection
  ///
  //if (! event->get_trigger(SQEvent::MATRIX1)) {
  if (! event->get_trigger(SQEvent::NIM2)) {
    return Fun4AllReturnCodes::EVENT_OK;
  }

  h1_evt_cnt->Fill(3);

  static int did_h1t = 0;
  static int did_h1b = 0;
  static int did_h1l = 0;
  static int did_h1r = 0;
  if (did_h1t == 0) {
    GeomSvc* geom = GeomSvc::instance();
    did_h1t = geom->getDetectorID("H1T");
    did_h1b = geom->getDetectorID("H1B");
    did_h1l = geom->getDetectorID("H1L");
    did_h1r = geom->getDetectorID("H1R");
    cout << "H1T = " << did_h1t << ", H1B = " << did_h1b << ", H1L = " << did_h1l << ", H1R = " << did_h1r << endl;
  }
  shared_ptr<SQHitVector> hv_h1t(UtilSQHit::FindHits(hit_vec, did_h1t));
  shared_ptr<SQHitVector> hv_h1b(UtilSQHit::FindHits(hit_vec, did_h1b));
  shared_ptr<SQHitVector> hv_h1l(UtilSQHit::FindHits(hit_vec, did_h1l));
  shared_ptr<SQHitVector> hv_h1r(UtilSQHit::FindHits(hit_vec, did_h1r));
  if (hv_h1t->size() + hv_h1b->size() == 1) return Fun4AllReturnCodes::EVENT_OK;

  h1_evt_cnt->Fill(4);

  ///
  /// Get & fill the hit info
  ///
  for (unsigned int i_det = 0; i_det < list_det_name.size(); i_det++) {
    strncpy(b_det_name, list_det_name[i_det].c_str(), sizeof(b_det_name));
    b_det = list_det_id[i_det];
    shared_ptr<SQHitVector> hv(UtilSQHit::FindHits(hit_vec, b_det));
    int n_intime = 0;
    for (SQHitVector::ConstIter it = hv->begin(); it != hv->end(); it++) {
      b_ele  = (*it)->get_element_id();
      b_time = (*it)->get_tdc_time  ();
      tree->Fill();

      bool is_intime = (*it)->is_in_time();
      if (is_intime) {
        n_intime++;
        h1_ele [i_det]->Fill(b_ele);
      }

      h1_ele_all[i_det]->Fill(b_ele );
      h1_time   [i_det]->Fill(b_time);
    }
    h1_nhit    [i_det]->Fill(n_intime);
    h1_nhit_all[i_det]->Fill(hv->size());
  }

  bool hit_h1l_top1 = false;
  bool hit_h1l_top2 = false;
  bool hit_h1l_bot1 = false;
  bool hit_h1l_bot2 = false;
  for (SQHitVector::ConstIter it = hv_h1l->begin(); it != hv_h1l->end(); it++) {
    if (!(*it)->is_in_time()) continue;
    int ele = (*it)->get_element_id();
    if (ele >= 12) hit_h1l_top1 = true;
    if (ele >= 13) hit_h1l_top2 = true;
    if (ele <=  9) hit_h1l_bot1 = true;
    if (ele <=  8) hit_h1l_bot2 = true;
  }
  bool hit_h1r_top1 = false;
  bool hit_h1r_top2 = false;
  bool hit_h1r_bot1 = false;
  bool hit_h1r_bot2 = false;
  for (SQHitVector::ConstIter it = hv_h1r->begin(); it != hv_h1r->end(); it++) {
    if (!(*it)->is_in_time()) continue;
    int ele = (*it)->get_element_id();
    if (ele >= 12) hit_h1r_top1 = true;
    if (ele >= 13) hit_h1r_top2 = true;
    if (ele <=  9) hit_h1r_bot1 = true;
    if (ele <=  8) hit_h1r_bot2 = true;
  }

  bool hit_h1y_top1 = hit_h1l_top1 || hit_h1r_top1;
  bool hit_h1y_top2 = hit_h1l_top2 || hit_h1r_top2;
  bool hit_h1y_bot1 = hit_h1l_bot1 || hit_h1r_bot1;
  bool hit_h1y_bot2 = hit_h1l_bot2 || hit_h1r_bot2;

  if (hv_h1l->size() + hv_h1r->size() > 0) h1_evt_cnt->Fill(5);
  if (hit_h1y_top1 && hit_h1y_bot1)        h1_evt_cnt->Fill(6);
  if (hit_h1y_top2 && hit_h1y_bot2)        h1_evt_cnt->Fill(7);

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::End(PHCompositeNode* topNode)
{
  ofstream ofs("result.txt");
  ofs << "Event counts:\n";
  for (int ii = 1; ii <= 10; ii++) {
    ofs << "  " << ii << "\t" << (int)(h1_evt_cnt->GetBinContent(ii)) << "\n";
  }
  ofs << endl;
  ofs.close();

  ostringstream oss;
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  for (unsigned int i_det = 0; i_det < list_det_id.size(); i_det++) {
    h1_ele    [i_det]->SetLineColor(kRed);
    h1_ele_all[i_det]->Draw();
    h1_ele    [i_det]->Draw("same");
    oss.str("");
    oss << h1_ele[i_det]->GetName() << ".png";
    c1->SaveAs(oss.str().c_str());

    h1_time[i_det]->Draw();
    oss.str("");
    oss << h1_time[i_det]->GetName() << ".png";
    c1->SaveAs(oss.str().c_str());

    h1_nhit[i_det]->SetLineColor(kRed);
    h1_nhit[i_det]->Draw();
    h1_nhit_all[i_det]->Draw("same");
    oss.str("");
    oss << h1_nhit[i_det]->GetName() << ".png";
    c1->SaveAs(oss.str().c_str());
  }
  delete c1;

  f_out->cd();
  f_out->Write();
  f_out->Close();
  
  return Fun4AllReturnCodes::EVENT_OK;
}
