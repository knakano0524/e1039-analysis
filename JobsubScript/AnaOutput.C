
void AnaOutputInTime()
{
  const int N_RUN = 6;
  const int LIST_RUN[N_RUN] = { 24500, 25000, 26000, 27000, 28000, 28700 };

  const int N_PL = 8;
  const char* LIST_PL[N_PL] = { "H1T", "H1B", "H2T", "H2B", "H3T", "H3B", "H4T", "H4B" };

  ostringstream oss;

  gSystem->mkdir("result", true);
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
 
  TFile* file[N_RUN];
  for (int ir = 0; ir < N_RUN; ir++) {
    int run = LIST_RUN[ir];
    cout << "Run " << run << endl;
    oss.str("");
    oss << "dir_job_20210208/" << run << "/out/output.root";
    file[ir] = new TFile(oss.str().c_str());
    if (! file[ir]->IsOpen()) {
      cerr << "! file[ir]->IsOpen()\n";
      continue;
    }

    for (int ipl = 0; ipl < N_PL; ipl++) {
      string plane = LIST_PL[ipl];
      oss.str("");
      oss << "h1_time_all_" << plane;
      TH1* h1_time_all = (TH1*)file[ir]->Get(oss.str().c_str());
      if (! h1_time_all) { cerr << "! h1_time_all\n"; exit(1); }

      oss.str("");
      oss << "h1_time_" << plane;
      TH1* h1_time = (TH1*)file[ir]->Get(oss.str().c_str());
      if (! h1_time) { cerr << "! h1_time\n"; exit(1); }

      h1_time_all->SetLineWidth(2);
      h1_time    ->SetLineColor(kRed);
      h1_time_all->Draw();
      h1_time    ->Draw("same");
      oss.str("");
      oss << "result/h1_time_" << plane << "_" << run << ".png";
      c1->SaveAs(oss.str().c_str());
    }
  }

  exit(0);
}
