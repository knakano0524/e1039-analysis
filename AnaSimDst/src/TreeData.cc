#include "TreeData.h"
using namespace std;

EventData::EventData()
  : weight(0), proc_id(0), fpga1(false), nim1(false), nim2(false), rec_stat(0), n_dim_true(0), n_dim_reco(0)
{
  for (int ii = 0; ii < 4; ii++) {
    par_id [ii] = 0;
    par_mom[ii].SetXYZT(0, 0, 0, 0);
  }
}

TrackData::TrackData() 
  : charge(0)
{
  ;
}
  
DimuonData::DimuonData() 
  : pdg_id(0), x1(0), x2(0)
{
  ;
}
