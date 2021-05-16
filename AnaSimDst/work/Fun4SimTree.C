/// Fun4SimTree.C:  Macro to analyze the simulated tree created by Fun4SimMicroDst.C.
R__LOAD_LIBRARY(libana_sim_dst)

using namespace std;
TFile* file;
TTree* tr;
TCanvas* c1;

void Fun4SimTree(const char* fname="sim_tree.root", const char* tname="tree")
{
  file = new TFile(fname);
  tr = (TTree*)file->Get(tname);

  gSystem->mkdir("result", true);
  c1 = new TCanvas("c1", "");
  c1->SetGrid();
  c1->SetLogy(true);

  gStyle->SetOptStat(0000);

//  tr->Draw("weight");
//  c1->SaveAs("result/h1_weight.png");

  map<int, double> count_pos_top;
  map<int, double> count_pos_bot;
  map<int, double> count_neg_top;
  map<int, double> count_neg_bot;
  double weight;
  double mass;
  int road_pos;
  int road_neg;
  tr->SetBranchAddress("weight"  , &weight  );
  tr->SetBranchAddress("mass"    , &mass    );
  tr->SetBranchAddress("road_pos", &road_pos);
  tr->SetBranchAddress("road_neg", &road_neg);
  for (int i_ent = 0; i_ent < tr->GetEntries(); i_ent++) {
    tr->GetEntry(i_ent);
    if (mass < 4.0) continue;
    if (road_pos * road_neg >= 0) continue;

    if (road_pos > 0) count_pos_top[road_pos] += weight;
    else              count_pos_bot[road_pos] += weight;
    if (road_neg > 0) count_neg_top[road_neg] += weight;
    else              count_neg_bot[road_neg] += weight;
  }

  vector< pair<double, int> > list_pos_top;
  vector< pair<double, int> > list_pos_bot;
  vector< pair<double, int> > list_neg_top;
  vector< pair<double, int> > list_neg_bot;
  for (map<int, double>::iterator it = count_pos_top.begin(); it != count_pos_top.end(); it++) {
    list_pos_top.push_back( pair<double, int>(it->second, it->first) );
  }
  for (map<int, double>::iterator it = count_pos_bot.begin(); it != count_pos_bot.end(); it++) {
    list_pos_bot.push_back( pair<double, int>(it->second, it->first) );
  }
  for (map<int, double>::iterator it = count_neg_top.begin(); it != count_neg_top.end(); it++) {
    list_neg_top.push_back( pair<double, int>(it->second, it->first) );
  }
  for (map<int, double>::iterator it = count_neg_bot.begin(); it != count_neg_bot.end(); it++) {
    list_neg_bot.push_back( pair<double, int>(it->second, it->first) );
  }
  sort(list_pos_top.begin(), list_pos_top.end());
  sort(list_pos_bot.begin(), list_pos_bot.end());
  sort(list_neg_top.begin(), list_neg_top.end());
  sort(list_neg_bot.begin(), list_neg_bot.end());
  cout << "pos_top: " << list_pos_top.size() << " " << list_pos_top[0].first << "\n"
       << "pos_bot: " << list_pos_bot.size() << " " << list_pos_bot[0].first << "\n"
       << "neg_top: " << list_neg_top.size() << " " << list_neg_top[0].first << "\n"
       << "neg_bot: " << list_neg_bot.size() << " " << list_neg_bot[0].first << "\n"
       << endl;

  exit(0);
}
