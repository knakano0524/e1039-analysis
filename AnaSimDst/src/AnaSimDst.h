#ifndef _ANA_SIM_DST__H_
#define _ANA_SIM_DST__H_
#include <map>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <fun4all/SubsysReco.h>
#include "TreeData.h"
class TFile;
class TTree;
class PHGenIntegral;
class SQEvent;
class SRecEvent;
class SQMCEvent;
class SQTrackVector;
class SQDimuonVector;

/// An example class to analyze the simulated uDST file.
class AnaSimDst: public SubsysReco {
  /// Input
  PHGenIntegral * mi_gen_inte;
  SQEvent       * mi_evt;
  SRecEvent     * mi_srec;
  SQMCEvent     * mi_evt_true;
  SQTrackVector * mi_vec_trk;
  SQDimuonVector* mi_vec_dim;

  /// Output
  TFile* file;
  TTree* tree;
  EventData  mo_evt;
  TrackList  mo_trk_true;
  TrackList  mo_trk_reco;
  DimuonList mo_dim_true;
  DimuonList mo_dim_reco;

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

  typedef std::map<int, int> IdMap_t; // For now the key is not ID but index.
  void FindTrackRelation (IdMap_t& id_map);
  void FindDimuonRelation(IdMap_t& id_map);

  void CalcAngle(const TLorentzVector& mu0, const TLorentzVector& mu1, const int pol_sign, double& xb, double& xt, double& phi_s_tf);
  void Lab2CollinsSoper(const double px1, const double py1, const double pz1, const double px2, const double py2, const double pz2, double& costh, double& phi);
  void Lab2CollinsSoper(const TVector3& p1, const TVector3& p2, double& costh, double& phi);
};

#endif /* _ANA_SIM_DST__H_ */
