#include <glob.h>

void AnaOutput()
{
  TChain* tree = new TChain("tree");
  glob_t gl;
  glob("dir_job_20201231/*/out/output.root", GLOB_TILDE, 0, &gl);
  for (int ii = 0; ii < gl.gl_pathc; ii++) tree->Add(gl.gl_pathv[ii]);
  globfree(&gl);
  cout << "N of chained trees = " << tree->GetNtrees() << endl;
  if (tree->GetNtrees() == 0) {
    cout << "  No tree strangely.  Abort." << endl;
    exit(1);
  }

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();

  TH1* h2_inte_run = new TH2D("h2_inte_run", "N of NIM3 events;Run ID;Max(RF-08...RF+08);", 100, 28050, 28750,  40, 0, 8000);
  tree->Project("h2_inte_run", "inte_max:run");
  h2_inte_run->Draw("colz");
  c1->SaveAs("h2_inte_run.png");

  c1->SetLogy(true);

  TH1* h1_inte = new TH1D("h1_inte", ";Max(RF-08...RF+08);N of NIM3 events", 20, 0, 20000);
  tree->Project("h1_inte", "inte_max");
  h1_inte->Draw();
  c1->SaveAs("h1_inte.png");

  int I_N = 50;
  double I_MIN =    0;
  double I_MAX = 5000;
  TH1* h1_inte_0 = new TH1D("h1_inte_0", "", I_N, I_MIN, I_MAX);
  TH1* h1_inte_1 = new TH1D("h1_inte_1", "", I_N, I_MIN, I_MAX);
  TH1* h1_inte_2 = new TH1D("h1_inte_2", "", I_N, I_MIN, I_MAX);
  tree->Project("h1_inte_0", "inte_max", "fpga1_0>0");
  tree->Project("h1_inte_1", "inte_max", "fpga1_1>0");
  tree->Project("h1_inte_2", "inte_max", "fpga1_2>0");
  h1_inte_0->SetLineColor(kBlack);
  h1_inte_1->SetLineColor(kBlue);
  h1_inte_2->SetLineColor(kRed);

  THStack hs("hs", ";Max(RF-08...RF+08);N of NIM3 events");
  hs.Add(h1_inte_0);
  hs.Add(h1_inte_1);
  hs.Add(h1_inte_2);
  hs.Draw("nostack");
  c1->SaveAs("h1_inte_fpga.png");  

  int bin0 = 2; // Exclude the 1st bin (inte = 0)
  int bin1 = h1_inte_0->FindBin(1000);
  ofstream ofs("result.txt");
  ofs << "N of chained trees = " << tree->GetNtrees() << "\n"
      << "N of NIM3 events = " << (int)h1_inte->Integral() << "\n\n"
      << "n_0 = " << (int)h1_inte_0->Integral(bin0, bin1) << "\n"
      << "n_1 = " << (int)h1_inte_1->Integral(bin0, bin1) << "\n"
      << "n_2 = " << (int)h1_inte_2->Integral(bin0, bin1) << "\n"
      << "n_0 = " << (int)h1_inte_0->Integral(bin0, I_N+1) << "\n"
      << "n_1 = " << (int)h1_inte_1->Integral(bin0, I_N+1) << "\n"
      << "n_2 = " << (int)h1_inte_2->Integral(bin0, I_N+1) << "\n"
      << endl;
  ofs.close();

  exit(0);
}
