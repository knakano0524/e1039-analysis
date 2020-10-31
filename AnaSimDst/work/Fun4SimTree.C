/// Fun4SimTree.C:  Macro to analyze the simulated tree created by Fun4SimMicroDst.C.
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
R__LOAD_LIBRARY(libana_sim_dst)
#endif

using namespace std;
TFile* file;
TTree* tr;
TCanvas* c1;
void DrawOneVar(const char* name, const char* var, const int N, const double X0, const double X1);
void DrawAcc();

void DrawDimTrueKin(TTree* tr);
void DrawDimRecoKin(TTree* tr);
void DrawTrkTrueKin(TTree* tr);
void DrawTrueVar(TTree* tr, const string varname, const string title_x, const int n_x, const double x_lo, const double x_hi);

void Fun4SimTree(const char* fname="sim_tree.root", const char* tname="tree")
{
  file = new TFile(fname);
  tr = (TTree*)file->Get(tname);

  gSystem->mkdir("result", true);
  c1 = new TCanvas("c1", "");
  c1->SetGrid();
  c1->SetLogy(true);

  gStyle->SetOptStat(0000);

  tr->Draw("weight", "!std::isnan(weight)");
  c1->SaveAs("result/h1_weight.png");

  DrawOneVar("mass", "dim_true.mom.M()", 44, 1, 5.4);
  DrawOneVar("xF", "dim_true.x1 - dim_true.x2", 60, -0.2, 1.0);

  DrawAcc();

//  DrawDimTrueKin(tr);
//  DrawDimRecoKin(tr);
//  DrawTrkTrueKin(tr);

  exit(0);
}

///
/// Functions below
///
void DrawOneVar(const char* name, const char* var, const int N, const double X0, const double X1)
{
  TH1* h1_all = new TH1D("h1_all", "", N, X0, X1);
  h1_all->GetSumw2();
  tr->Project("h1_all", var, "std::isnan(weight) ? 0 : weight");
  //h1_all->Draw("E1");
  //c1->SaveAs("result/h1_m_all.png");

  TH1* h1_rec = new TH1D("h1_rec", "", N, X0, X1);
  h1_rec->GetSumw2();
  tr->Project("h1_rec", var, "(std::isnan(weight) || rec_stat!=0) ? 0 : weight");

  TH1* h1_nim2 = new TH1D("h1_nim2", "", N, X0, X1);
  h1_nim2->GetSumw2();
  tr->Project("h1_nim2", var, "(std::isnan(weight) || rec_stat!=0 || !nim2) ? 0 : weight");

  TH1* h1_fpga1 = new TH1D("h1_fpga1", "", N, X0, X1);
  h1_fpga1->GetSumw2();
  tr->Project("h1_fpga1", var, "(std::isnan(weight) || rec_stat!=0 || !fpga1) ? 0 : weight");

  h1_all  ->SetLineColor(kBlack);
  h1_rec  ->SetLineColor(kRed);
  h1_nim2 ->SetLineColor(kGreen);
  h1_fpga1->SetLineColor(kBlue);
  
  //h1_rec  ->SetLineWidth(3);

  ostringstream oss;
  oss << "GMC;" << name << ";Weighted yield";

  THStack hs("hs", oss.str().c_str());
  hs.Add(h1_all  , "E1");
  hs.Add(h1_rec  , "E1");
  hs.Add(h1_nim2 , "E1");
  hs.Add(h1_fpga1, "E1");
  hs.Draw("nostack");

  TLegend leg (0.75, 0.75, 0.99, 0.99);
  leg.AddEntry(h1_all  , "All in acceptance", "l");
  leg.AddEntry(h1_rec  , "Reconstructed", "l");
  leg.AddEntry(h1_nim2 , "NIM2 fired", "l");
  leg.AddEntry(h1_fpga1, "FPGA1 fired", "l");
  leg.SetTextFont(22);
  leg.SetBorderSize(1);
  leg.SetFillColor(0);
  leg.Draw();

  oss.str("");
  oss << "result/h1_" << name << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h1_all  ;
  delete h1_rec  ;
  delete h1_nim2 ;
  delete h1_fpga1;
}

void DrawAcc()
{
  c1->SetLogy(false);

  TH1* h2_rec = new TH2D("h2_rec", "Weighted yield: Reconstructed;mass;xF", 22, 1, 5.4,  30, -0.2, 1.0);
  h2_rec->GetSumw2();
  tr->Project("h2_rec", "(dim_true.x1 - dim_true.x2):(dim_true.mom.M())", "(std::isnan(weight) || rec_stat!=0) ? 0 : weight");
  h2_rec->Draw("colz");
  c1->SaveAs("result/h2_rec.png");

  TH1* h2_nim2 = new TH2D("h2_nim2", "Weighted yield: NIM2 fired;mass;xF", 22, 1, 5.4,  30, -0.2, 1.0);
  h2_nim2->GetSumw2();
  tr->Project("h2_nim2", "(dim_true.x1 - dim_true.x2):(dim_true.mom.M())", "(std::isnan(weight) || rec_stat!=0 || !nim2) ? 0 : weight");
  h2_nim2->Draw("colz");
  c1->SaveAs("result/h2_nim2.png");

  TH1* h2_fpga1 = new TH2D("h2_fpga1", "Weighted yield: FPGA1 fired;mass;xF", 22, 1, 5.4,  30, -0.2, 1.0);
  h2_fpga1->GetSumw2();
  tr->Project("h2_fpga1", "(dim_true.x1 - dim_true.x2):(dim_true.mom.M())", "(std::isnan(weight) || rec_stat!=0 || !fpga1) ? 0 : weight");
  h2_fpga1->Draw("colz");
  c1->SaveAs("result/h2_fpga1.png");

  c1->SetLogz(true);

  TH1* h2_acc_nim2 = (TH1*)h2_nim2->Clone("h2_acc_nim2");
  h2_acc_nim2->SetTitle("Trigger acceptance: NIM2");
  h2_acc_nim2->Divide(h2_rec);
  h2_acc_nim2->Draw("colz");
  c1->SaveAs("result/h2_acc_nim2.png");

  TH1* h2_acc_fpga1 = (TH1*)h2_fpga1->Clone("h2_acc_fpga1");
  h2_acc_fpga1->SetTitle("Trigger acceptance: FPGA1");
  h2_acc_fpga1->Divide(h2_rec);
  h2_acc_fpga1->Draw("colz");
  c1->SaveAs("result/h2_acc_fpga1.png");
}

void DrawDimTrueKin(TTree* tr)
{
  tr->Draw("n_dim_true");
  c1->SaveAs("result/h1_true_n_dim.png");
  tr->Draw("n_dim_reco");
  c1->SaveAs("result/h1_reco_n_dim.png");

  const double PI = TMath::Pi();
  DrawTrueVar(tr, "dim_true.pdg_id"    , "True dimuon PDG ID", 1000, 0, 0);
  DrawTrueVar(tr, "dim_true.mom.X()"   , "True dimuon px (GeV)", 100, -5,   5);
  DrawTrueVar(tr, "dim_true.mom.Y()"   , "True dimuon py (GeV)", 100, -5,   5);
  DrawTrueVar(tr, "dim_true.mom.Z()"   , "True dimuon pz (GeV)", 100,  0, 100);
  DrawTrueVar(tr, "dim_true.mom.M()"   , "True dimuon mass (GeV)", 100, 0, 5);
  DrawTrueVar(tr, "dim_true.mom.Eta()" , "True dimuon #eta", 110, 0, 11);
  DrawTrueVar(tr, "dim_true.mom.Phi()" , "True dimuon #phi", 100, -PI, PI);
  DrawTrueVar(tr, "dim_true.x1"        , "True x1", 50, 0, 1);
  DrawTrueVar(tr, "dim_true.x2"        , "True x2", 50, 0, 1);
}

void DrawDimRecoKin(TTree* tr)
{
  tr->Draw("rec_stat"); // cf. GlobalConsts.h.
  c1->SaveAs("result/h1_rec_stat.png");
  
  tr->Draw("trig_bits", "rec_stat==0");
  c1->SaveAs("result/h1_trig_bits.png");

  tr->Draw("dim_reco.mom.M()", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_mass.png");

  tr->Draw("dim_reco.x1", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_x1.png");

  tr->Draw("dim_reco.x2", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_x2.png");
}


void DrawTrkTrueKin(TTree* tr)
{
  DrawTrueVar(tr, "dim_true.mom_pos.X()", "True px (GeV) of mu+", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.mom_pos.Y()", "True py (GeV) of mu+", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.mom_pos.Z()", "True pz (GeV) of mu+", 100,  0, 100);
  DrawTrueVar(tr, "dim_true.mom_neg.X()", "True px (GeV) of mu-", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.mom_neg.Y()", "True py (GeV) of mu-", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.mom_neg.Z()", "True pz (GeV) of mu-", 100,  0, 100);

  THStack* hs;
  TH1* h1_all = new TH1D("h1_all", "", 100, -1, 1);
  TH1* h1_rec = new TH1D("h1_rec", "", 100, -1, 1);
  tr->Project("h1_all", "(dim_true.mom_pos.Z() - dim_true.mom_neg.Z())/(dim_true.mom_pos.Z() + dim_true.mom_neg.Z())");
  tr->Project("h1_rec", "(dim_true.mom_pos.Z() - dim_true.mom_neg.Z())/(dim_true.mom_pos.Z() + dim_true.mom_neg.Z())", "rec_stat==0");
  hs = new THStack("hs", "J/#psi GMC;gpz+gpz (GeV) of tracks;N of tracks");
  hs->Add(h1_all);
  hs->Add(h1_rec);
  h1_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_trk_true_pz_asym.png");
}

void DrawTrueVar(TTree* tr, const string varname, const string title_x, const int n_x, const double x_lo, const double x_hi)
{
  TH1* h1_all = new TH1D("h1_all", "", n_x, x_lo, x_hi);
  TH1* h1_rec = new TH1D("h1_rec", "", n_x, x_lo, x_hi);
  tr->Project("h1_all", varname.c_str());
  tr->Project("h1_rec", varname.c_str(), "rec_stat==0");

  ostringstream oss;
  oss << "J/#psi GMC;" << title_x << ";Yield";
  THStack hs("hs", oss.str().c_str());
  hs.Add(h1_all);
  hs.Add(h1_rec);
  h1_rec->SetLineColor(kRed);
  hs.Draw("nostack");

  oss.str("");
  oss << "result/h1_";
  for (string::const_iterator it = varname.begin(); it != varname.end(); it++) {
    switch (*it) { // modify bad chars for file name
    case '.': case '*': case '/': oss << '_'; break;
    case '(': case ')': case ' ': /* omit */ break;
    default: oss << *it;
    }
  }
  oss << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h1_all;
  delete h1_rec;
}
