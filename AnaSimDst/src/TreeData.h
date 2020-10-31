#ifndef _TREE_DATA__H_
#define _TREE_DATA__H_
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct EventData {
  double weight;
  int proc_id;
  int par_id[4]; // 2 -> 2
  TLorentzVector par_mom[4];
  bool fpga1;
  bool nim1;
  bool nim2;
  int rec_stat;
  int n_dim_true;
  int n_dim_reco;

  EventData();
  virtual ~EventData() {;}

  ClassDef(EventData, 1);
};

struct TrackData {
  int            charge;
  TVector3       pos_vtx;
  TLorentzVector mom_vtx;

  TrackData();
  virtual ~TrackData() {;}

  ClassDef(TrackData, 1);
};

struct DimuonData {
  int            pdg_id;
  TVector3       pos;
  TLorentzVector mom;
  TLorentzVector mom_pos;
  TLorentzVector mom_neg;
  double         x1;
  double         x2;

  DimuonData();
  virtual ~DimuonData() {;}

  ClassDef(DimuonData, 1);
};

typedef std::vector<TrackData > TrackList;
typedef std::vector<DimuonData> DimuonList;

#endif /* _TREE_DATA__H_ */
