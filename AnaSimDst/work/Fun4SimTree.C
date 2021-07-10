/// Fun4SimTree.C:  Macro to analyze the simulated tree created by Fun4SimMicroDst.C.
R__LOAD_LIBRARY(libana_sim_dst)

using namespace std;
TFile* file_out;
TFile* file;
TTree* tree;
TCanvas* c1;
double inte_lumi;
void DrawDimTrueKin();
void MakeHist1D(const string name, const string cut, const string var_x, const string title_x, const int n_x, const double x_lo, const double x_hi);
void MakeHist2D(const string name, const string cut, 
  const string var_x, const string title_x, const int n_x, const double x_lo, const double x_hi,
  const string var_y, const string title_y, const int n_y, const double y_lo, const double y_hi);
void MakeHist3D(const string name, const string cut, 
  const string var_x, const string title_x, const int n_x, const double x_lo, const double x_hi,
  const string var_y, const string title_y, const int n_y, const double y_lo, const double y_hi,
  const string var_z, const string title_z, const int n_z, const double z_lo, const double z_hi);

double GetInteLumi(const char* fn_lumi="lumi_tot.txt");

void Fun4SimTree(const char* fname="sim_tree.root", const char* tname="tree")
{
  file = new TFile(fname);
  tree = (TTree*)file->Get(tname);
  gSystem->mkdir("result", true);
  c1 = new TCanvas("c1", "");
  c1->SetGrid();
  //c1->SetLogy(true);

  file_out = new TFile("hist.root", "RECREATE");

  inte_lumi = GetInteLumi();
  cout << "Integrated luminosity = " << inte_lumi << endl;

  DrawDimTrueKin();

  file_out->Write();
  file_out->Close();
  exit(0);
}

///
/// Functions below
///
void DrawDimTrueKin()
{
  c1->SetLogy(true);
  tree->Draw("n_dim_true");
  c1->SaveAs("result/h1_true_n_dim.png");

  const double PI = TMath::Pi();
  //MakeHist1D("vtx_x", "", "dim_true.pos.X()"   , "X-vertex", 100, -2, 2);
  //MakeHist1D("vtx_y", "", "dim_true.pos.Y()"   , "Y-vertex", 100, -2, 2);
  //MakeHist1D("vtx_z", "", "dim_true.pos.Z()"   , "Z-vertex", 100, -310, -290);
  //MakeHist1D("px"      , "", "dim_true.mom.X()"   , "px"  , 50, -5, 5);
  //MakeHist1D("py"      , "", "dim_true.mom.Y()"   , "py"  , 50, -5, 5);
  MakeHist1D("pz"      , "dim_true.mom.M() > 4", "dim_true.mom.Z()"   , "pz @ M > 4"  , 30, 30, 120);
  MakeHist1D("pT"      , "dim_true.mom.M() > 4", "dim_true.mom.Perp()", "pT @ M > 4"  , 25, 0,   5);
  MakeHist1D("mass"    ,                     "", "dim_true.mom.M()"   , "Mass"        , 20, 0,  10);
  MakeHist1D("phi"     , "dim_true.mom.M() > 4", "dim_true.mom.Phi()" , "#phi @ M > 4", 30, -PI, PI);
  //MakeHist1D("x1"      , "", "dim_true.x1"        , "x1"        , 50,  0.0, 1.0);
  //MakeHist1D("x2"      , "", "dim_true.x2"        , "x2"        , 25,  0.0, 0.5);
  MakeHist1D("xF"      , "dim_true.mom.M() > 4", "dim_true.xF"        , "xF @ M > 4"  , 30, -0.5, 1.0);
  //MakeHist1D("cs_costh", "", "dim_true.costh"     , "cos#theta in C-S frame", 50, -1, 1);
  //MakeHist1D("cs_phi"  , "", "dim_true.phi"       , "#phi in C-S frame"     , 50, 0, 2*PI);

  gStyle->SetOptStat(0);

  MakeHist2D("pz_mass", "", "dim_true.mom.Z()"   , "pz", 18, 30, 120 , "dim_true.mom.M()", "Mass", 4, 2, 6);
  MakeHist2D("xF_mass", "", "dim_true.xF"        , "xF", 15, -0.5, 1 , "dim_true.mom.M()", "Mass", 4, 2, 6);
  MakeHist2D("pT_mass", "", "dim_true.mom.Perp()", "pT", 10, 0, 5    , "dim_true.mom.M()", "Mass", 4, 2, 6);
  MakeHist2D("x1_x2"  , "", "dim_true.x1"        , "x1", 14, 0.3, 1.0, "dim_true.x2"     , "x2"  , 4, 0.0, 0.4);

  MakeHist3D("pz_pT_mass", "", "dim_true.mom.Z()", "pz", 18, 30, 120, "dim_true.mom.Perp()", "pT", 5, 0, 5, "dim_true.mom.M()", "Mass", 4, 2, 6);
  MakeHist3D("phi_costh_mass", "", "dim_true.phi", "#phi in C-S frame", 30, 0, 2*PI, "dim_true.costh", "cos#theta in C-S frame", 30, -1, 1, "dim_true.mom.M()", "Mass", 4, 2, 6);
}

void MakeHist1D(const string name, const string cut, const string var_x, const string title_x, const int n_x, const double x_lo, const double x_hi)
{
  ostringstream oss;
  oss << "h1_" << name;
  string name_h1 = oss.str();
  TH1* h1 = new TH1D(name_h1.c_str(), "", n_x, x_lo, x_hi);

  oss.str("");
  oss << ";" << title_x << ";Rate (pb)";
  h1->SetTitle(oss.str().c_str());

  oss.str("");
  oss << "weight * (n_dim_true == 1";
  if (cut.length() > 0) oss << " && " << cut;
  oss << ")";
  string par_cut = oss.str();

  tree->Project(name_h1.c_str(), var_x.c_str(), par_cut.c_str());
  h1->Scale(1/inte_lumi);
  h1->SetLineColor(kRed);
  h1->Draw("E1");

  c1->SetLogy(false);

  oss.str("");
  oss << "result/" << name_h1 << ".png";
  c1->SaveAs(oss.str().c_str());
}

void MakeHist2D(const string name, const string cut, 
  const string var_x, const string title_x, const int n_x, const double x_lo, const double x_hi,
  const string var_y, const string title_y, const int n_y, const double y_lo, const double y_hi)
{
  ostringstream oss;
  oss << "h2_" << name;
  string name_h2 = oss.str();
  TH2* h2 = new TH2D(name_h2.c_str(), "", n_x, x_lo, x_hi,  n_y, y_lo, y_hi);

  oss.str("");
  oss << ";" << title_x << ";" << title_y << ";Rate (pb)";
  h2->SetTitle(oss.str().c_str());

  oss.str("");
  oss << "weight * (n_dim_true == 1";
  if (cut.length() > 0) oss << " && " << cut;
  oss << ")";
  string par_cut = oss.str();

  oss.str("");
  oss << var_y << ":" << var_x;
  tree->Project(name_h2.c_str(), oss.str().c_str(), par_cut.c_str());
  h2->Scale(1/inte_lumi);
  h2->Draw("colz");
  //h2->SetMarkerSize(2.0); // = text size (1.0 by default)
  gStyle->SetPaintTextFormat("3.2f");
  h2->Draw("TEXTsame");

  oss.str("");
  oss << "result/" << name_h2 << ".png";
  c1->SaveAs(oss.str().c_str());
}

void MakeHist3D(const string name, const string cut, 
  const string var_x, const string title_x, const int n_x, const double x_lo, const double x_hi,
  const string var_y, const string title_y, const int n_y, const double y_lo, const double y_hi,
  const string var_z, const string title_z, const int n_z, const double z_lo, const double z_hi)
{
  ostringstream oss;
  oss << "h3_" << name;
  string name_h3 = oss.str();
  TH3* h3 = new TH3D(name_h3.c_str(), "", n_x, x_lo, x_hi,  n_y, y_lo, y_hi,  n_z, z_lo, z_hi);

  oss.str("");
  oss << "Rate (pb);" << title_x << ";" << title_y << ";" << title_z;
  h3->SetTitle(oss.str().c_str());

  oss.str("");
  oss << "weight * (n_dim_true == 1";
  if (cut.length() > 0) oss << " && " << cut;
  oss << ")";
  string par_cut = oss.str();

  oss.str("");
  oss << var_z << ":" << var_y << ":" << var_x;
  tree->Project(name_h3.c_str(), oss.str().c_str(), par_cut.c_str());
  h3->Scale(1/inte_lumi);
  h3->Draw("BOX");

  oss.str("");
  oss << "result/" << name_h3 << ".png";
  c1->SaveAs(oss.str().c_str());
}

double GetInteLumi(const char* fn_lumi)
{
  ifstream ifs(fn_lumi);
  if (!ifs.is_open()) {
    cout << "GetInteLumi():  Cannot open '" << fn_lumi << "'.  Just return 1.0." << endl;
    return 1.0;
  }
  double val;
  ifs >> val;
  ifs.close();
  return val;
}
