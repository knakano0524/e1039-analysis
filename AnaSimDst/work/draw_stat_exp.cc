/// Macro to draw the expected statistical accuracy of J/psi TSSA.
/**
 * One week of data taking is assumed.
 * Run condition ... based on E1039 proposal for NH3 (E1039proposal.pdf)
 *  - Integrated luminosity: L^{1w} = 1.75e4 pb^{-1}  ->  1.82e42 cm^{-2} in two years (cf: 1 pb^-1 = 10^36 cm^-2)
 *  - Dilution factor: $f = 0.176$
 *  - Polarization: $P = 0.8$
 *  - 1/f * 1/P = 7.1
 *  - \delta_{AN}^{1w} = 1/f * 1/P * \delta_{AN}^{sim} * \sqrt{L^{sim} / L^{1w}} = 3.3 * \delta_{AN}^{sim}
 */

{
  const double lumi_1w  = 1.75e4; // integrated luminosity in 1 week [/pb]
  const double df       = 0.176; // dilution_factor
  const double pol      = 0.8;   // polarization

  double lumi_sim;
  ifstream ifs("lumi_tot.txt");
  if (! ifs.is_open()) {
    cout << "!ERROR!  Cannot open the luminosity file.  Abort." << endl;
    exit(1);
  }
  ifs >> lumi_sim;
  ifs.close();
  double XX = sqrt(lumi_sim / lumi_1w) / df / pol;
  cout << "Integrated luminosity of all simDSTs = " << lumi_sim << " /pb\n"
       << "Error scaling factor = " << XX << endl;

  int    n_pt = 0;
  double val_x[99];
  double val_y[99];
  double err_x[99];
  double err_y[99];
  ifs.open("result_asym/result.txt");
  if (! ifs.is_open()) {
    cout << "!ERROR!  Cannot open the simulation result file.  Abort." << endl;
    exit(1);
  }
  int idx;
  double val_x2, err_x2, cnt, val_an, err_an;
  while (ifs >> idx >> val_x2 >> err_x2 >> cnt >> val_an >> err_an) {
    if (cnt < 10) continue;
    double err_an_exp = err_an * XX;
    val_x[n_pt] = val_x2;
    val_y[n_pt] = 0;
    err_x[n_pt] = err_x2;
    err_y[n_pt] = err_an_exp;
    n_pt++;
  }
  ifs.close();

  TGraphErrors* gr = new TGraphErrors(n_pt, val_x, val_y, err_x, err_y);
  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(kRed);
  gr->SetLineColor(kRed);

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  c1->DrawFrame(0.0, -0.3, 0.2, 0.3, "J/#psi | One Week;x_{2};Anticipated accuracy of A_{N}: #delta_{AN}");
  gr->Draw("PE1same");

  c1->SaveAs("result_asym/gr_delta_an.png");
}
