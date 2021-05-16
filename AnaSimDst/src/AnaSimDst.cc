#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <interface_main/SQMCEvent.h>
#include <interface_main/SQTrackVector.h>
#include <interface_main/SQDimuonVector.h>
#include <interface_main/SQHitVector.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include <UtilAna/UtilTrack.h>
#include <UtilAna/UtilDimuon.h>
#include <UtilAna/UtilTrigger.h>
#include <geom_svc/GeomSvc.h>
#include "AnaSimDst.h"
using namespace std;

int AnaSimDst::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::InitRun(PHCompositeNode* topNode)
{
  int ret = GetNodes(topNode);
  if (ret != Fun4AllReturnCodes::EVENT_OK) return ret;
  MakeTree();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::process_event(PHCompositeNode* topNode)
{
  static unsigned int n_evt = 0;
  if    (++n_evt % 100000 == 0) cout << n_evt << endl;
  else if (n_evt %  10000 == 0) cout << " . " << flush;

  for (unsigned int ii = 0; ii < mi_vec_dim->size(); ii++) {
    SQDimuon* dim = mi_vec_dim->at(ii);
    int id_trk_pos = dim->get_track_id_pos();
    int id_trk_neg = dim->get_track_id_neg();
    SQTrack* trk_pos = UtilTrack::FindTrackByID(mi_vec_trk, id_trk_pos, true);
    SQTrack* trk_neg = UtilTrack::FindTrackByID(mi_vec_trk, id_trk_neg, true);
    if (! trk_pos || ! trk_neg) {
      cerr << "Invalid true tracks!  Abort." << endl;
      exit(1);
    }
    
    shared_ptr<SQHitVector> hv_trk_pos(UtilTrack::FindHodoHitsOfTrack(mi_vec_hit, id_trk_pos));
    shared_ptr<SQHitVector> hv_trk_neg(UtilTrack::FindHodoHitsOfTrack(mi_vec_hit, id_trk_neg));
    int road_pos = UtilTrigger::ExtractRoadID(hv_trk_pos.get());
    int road_neg = UtilTrigger::ExtractRoadID(hv_trk_neg.get());
    //cout << "  pos: " << id_trk_pos << " " << hv_trk_pos->size() << " " << road_pos << "\n"
    //     << "  neg: " << id_trk_neg << " " << hv_trk_neg->size() << " " << road_neg << endl;
    //if (road_pos * road_neg >= 0) continue // Keep only T+B/B+T pairs

    mo_weight   = mi_evt_true->get_weight();
    mo_mass     = dim->get_mass();
    mo_xF       = dim->get_xf();
    mo_x1       = dim->get_x1();
    mo_x2       = dim->get_x2();
    mo_mom      = dim->get_mom().P();
    mo_phi      = dim->get_mom().Phi();
    mo_theta    = dim->get_mom().Theta();
    mo_road_pos = road_pos;
    mo_road_neg = road_neg;
    tree->Fill();
  }

//  tree->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::End(PHCompositeNode* topNode)
{
  file->cd();
  file->Write();
  file->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::GetNodes(PHCompositeNode *topNode)
{
  mi_evt_true = findNode::getClass<SQMCEvent     >(topNode, "SQMCEvent");
  mi_vec_trk  = findNode::getClass<SQTrackVector >(topNode, "SQTruthTrackVector");
  mi_vec_dim  = findNode::getClass<SQDimuonVector>(topNode, "SQTruthDimuonVector");
  mi_vec_hit  = findNode::getClass<SQHitVector   >(topNode, "SQHitVector");
  if (!mi_evt_true || !mi_vec_trk || !mi_vec_dim || !mi_vec_hit) {
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

void AnaSimDst::MakeTree()
{
  file = new TFile("sim_tree.root", "RECREATE");
  tree = new TTree("tree", "Created by AnaSimDst");

  tree->Branch("weight"  , &mo_weight  );
  tree->Branch("mass"    , &mo_mass    );
  tree->Branch("xF"      , &mo_xF      );
  tree->Branch("x1"      , &mo_x1      );
  tree->Branch("x2"      , &mo_x2      );
  tree->Branch("mom"     , &mo_mom     );
  tree->Branch("phi"     , &mo_phi     );
  tree->Branch("theta"   , &mo_theta   );
  tree->Branch("road_pos", &mo_road_pos);
  tree->Branch("road_neg", &mo_road_neg);
}
