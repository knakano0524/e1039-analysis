/// Fun4SimTree.C:  Macro to analyze the simulated tree created by Fun4SimMicroDst.C.
R__LOAD_LIBRARY(libana_sim_dst)
using namespace std;
TFile* fi_4pi;
TTree* tr_4pi;
TFile* fi_acc;
TTree* tr_acc;
TCanvas* c1;
void Draw1D(const string varname);
void Draw2DSlice(const string varname);
void Draw2D(const string varname);
void Draw3DSlice(const string varname);

void draw_geom_acc()
{
  fi_4pi = new TFile("4pi/hist.root");
  fi_acc = new TFile("acc/hist.root");

  gSystem->mkdir("result_geom_acc", true);
  c1 = new TCanvas("c1", "");
  c1->SetGrid();
  gStyle->SetOptStat(0000);

  Draw1D("mass");
  //Draw1D("px");
  //Draw1D("py");
  Draw1D("pz");
  Draw1D("pT");
  //Draw1D("x1");
  //Draw1D("x2");
  Draw1D("xF");

  Draw2DSlice("pz_mass");
  Draw2DSlice("xF_mass");
  Draw2DSlice("pT_mass");
  Draw2DSlice("x1_x2");

  Draw3DSlice("pz_pT_mass");
  Draw3DSlice("phi_costh_mass");

  exit(0);
}

///
/// Functions below
///
void Draw1D(const string varname)
{
  ostringstream oss;
  oss << "h1_" << varname;
  TH1* h1_4pi = (TH1*)fi_4pi->Get(oss.str().c_str());
  TH1* h1_acc = (TH1*)fi_acc->Get(oss.str().c_str());
  if (!h1_4pi || !h1_acc) {
    cout << "Cannot get h1_4pi/acc for " << varname << ".  Abort." << endl;
    exit(1);
  }
  h1_4pi->UseCurrentStyle();
  h1_acc->UseCurrentStyle();
  h1_4pi->SetLineWidth(2);
  h1_4pi->SetLineColor(kBlack);
  h1_acc->SetLineColor(kRed);
  h1_4pi->Draw("E1");
  h1_acc->Draw("E1same");
  c1->SetLogy(true);

  oss.str("");
  oss << "result_geom_acc/h1_" << varname << ".png";
  c1->SaveAs(oss.str().c_str());

  TH1* h1_ratio = (TH1*)h1_acc->Clone("h1_ratio");
  h1_ratio->Divide(h1_4pi);
  h1_ratio->Scale(100); // %
  h1_ratio->Draw("E1");
  h1_ratio->SetMinimum(0);
  //h1_ratio->GetYaxis()->SetRangeUser(0, 1);
  h1_ratio->GetYaxis()->SetTitle("Geometric acceptance (%)");
  c1->SetLogy(false);

  oss.str("");
  oss << "result_geom_acc/geom_acc_" << varname << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h1_ratio;
}

void Draw2DSlice(const string varname)
{
  ostringstream oss;
  oss << "h2_" << varname;
  TH2* h2_4pi = (TH2*)fi_4pi->Get(oss.str().c_str());
  TH2* h2_acc = (TH2*)fi_acc->Get(oss.str().c_str());
  if (!h2_4pi || !h2_acc) {
    cout << "Cannot get h2_4pi/acc for " << varname << ".  Abort." << endl;
    exit(1);
  }
  TH2* h2_ratio = (TH2*)h2_acc->Clone("h2_ratio");
  h2_ratio->Divide(h2_4pi);
  h2_ratio->Scale(100); // %

  int n_y = h2_ratio->GetNbinsY();

  TLegend leg(0.9, 0.95-0.05*n_y, 0.99, 0.99);
  leg.SetHeader(h2_ratio->GetYaxis()->GetTitle());

  oss.str("");
  oss << h2_ratio->GetTitle() << ";" << h2_ratio->GetXaxis()->GetTitle() << ";Geometric acceptance (%)";
  THStack hs("hs", oss.str().c_str());
  TH1* h1_ratio[99];
  for (int iy = 1; iy <= n_y; iy++) {
    oss.str("");
    oss << "h1_" << iy;
    h1_ratio[iy] = h2_ratio->ProjectionX(oss.str().c_str(), iy, iy);
    h1_ratio[iy]->SetMarkerStyle(7);
    h1_ratio[iy]->SetMarkerColor(iy);
    h1_ratio[iy]->SetLineColor  (iy);
    hs.Add(h1_ratio[iy], "E1");

    oss.str("");
    oss << h2_ratio->GetYaxis()->GetBinLowEdge(iy) << "-" << h2_ratio->GetYaxis()->GetBinLowEdge(iy+1);
    leg.AddEntry(h1_ratio[iy], oss.str().c_str(), "lp");
  }
  hs.Draw("nostack");
  leg.Draw();

  c1->SetLogy(true);
  oss.str("");
  oss << "result_geom_acc/geom_acc_" << varname << ".png";
  c1->SaveAs(oss.str().c_str());

  for (int iy = 1; iy <= h2_ratio->GetNbinsY(); iy++) delete h1_ratio[iy];
  delete h2_ratio;
}

void Draw2D(const string varname)
{
  ostringstream oss;
  oss << "h2_" << varname;
  TH2* h2_4pi = (TH2*)fi_4pi->Get(oss.str().c_str());
  TH2* h2_acc = (TH2*)fi_acc->Get(oss.str().c_str());
  if (!h2_4pi || !h2_acc) {
    cout << "Cannot get h2_4pi/acc for " << varname << ".  Abort." << endl;
    exit(1);
  }

  TH2* h2_ratio = (TH2*)h2_acc->Clone("h2_ratio");
  h2_ratio->Divide(h2_4pi);
  h2_ratio->Scale(100); // %
  h2_ratio->Draw("colz");
  //h2_ratio->SetMinimum(0);
  h2_ratio->GetZaxis()->SetTitle("");
  h2_ratio->SetTitle("Geometric acceptance (%)");

  c1->SetLogz(true);
  oss.str("");
  oss << "result_geom_acc/geom_acc_" << varname << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h2_ratio;
}

void Draw3DSlice(const string varname)
{
  ostringstream oss;
  oss << "h3_" << varname;
  TH3* h3_4pi = (TH3*)fi_4pi->Get(oss.str().c_str());
  TH3* h3_acc = (TH3*)fi_acc->Get(oss.str().c_str());
  if (!h3_4pi || !h3_acc) {
    cout << "Cannot get h3_4pi/acc for " << varname << ".  Abort." << endl;
    exit(1);
  }
  TH3* h3_ratio = (TH3*)h3_acc->Clone("h3_ratio");
  h3_ratio->Divide(h3_4pi);
  h3_ratio->Scale(100); // %

  c1->SetLogx(false);
  c1->SetLogy(false);
  c1->SetLogz(true);

  for (int iz = 1; iz <= h3_ratio->GetNbinsZ(); iz++) {
    h3_ratio->GetZaxis()->SetRange(iz, iz);
    TH1* h2 = h3_ratio->Project3D("yx");
    h2->Draw("colz");

    oss.str("");
    oss << "Geometric acceptance (%) @ " << h3_ratio->GetZaxis()->GetTitle() << " = "
        << h3_ratio->GetZaxis()->GetBinLowEdge(iz) << "-" << h3_ratio->GetZaxis()->GetBinLowEdge(iz+1);
    h2->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "result_geom_acc/geom_acc_" << varname << "_" << iz << ".png";
    c1->SaveAs(oss.str().c_str());
  }

  delete h3_ratio;
}

