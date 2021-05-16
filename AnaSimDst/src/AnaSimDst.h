#ifndef _ANA_SIM_DST__H_
#define _ANA_SIM_DST__H_
#include <map>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <fun4all/SubsysReco.h>
#include "TreeData.h"
class TFile;
class TTree;
class SQMCEvent;
class SQTrackVector;
class SQDimuonVector;
class SQHitVector;

/// An example class to analyze the simulated uDST file.
class AnaSimDst: public SubsysReco {
  /// Input
  SQMCEvent     * mi_evt_true;
  SQTrackVector * mi_vec_trk;
  SQDimuonVector* mi_vec_dim;
  SQHitVector   * mi_vec_hit;

  /// Output
  TFile* file;
  TTree* tree;
  EventData  mo_evt;
  TrackList  mo_trk_true;
  TrackList  mo_trk_reco;
  DimuonList mo_dim_true;
  DimuonList mo_dim_reco;

  double mo_weight;
  double mo_mass;
  double mo_xF;
  double mo_x1;
  double mo_x2;
  double mo_mom;
  double mo_phi;
  double mo_theta;
  int    mo_road_pos;
  int    mo_road_neg;

 public:
  AnaSimDst() {;}
  virtual ~AnaSimDst() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 private:
  int GetNodes(PHCompositeNode *topNode);
  void MakeTree();
};

#endif /* _ANA_SIM_DST__H_ */
