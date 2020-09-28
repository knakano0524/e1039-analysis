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
  /// Parameters: 2020-09-24.
  const int N_PT = 3;
  const double VAL_X[N_PT] = { 0.05, 0.07, 0.09 };
  const double VAL_Y[N_PT] = { 0, 0, 0 };
  const double ERR_X[N_PT] = { 0.01, 0.01, 0.01 };
  double ERR_Y[N_PT] = { 0.0154607, 0.00630896, 0.0112202 };
  const double L_1s  = 1.643; // integrated luminosity of one simDST [/pb]
  const double N_sim = 3997; // N of simDSTs

  const double L_sim = L_1s * N_sim; // integrated luminosity of simulation [/pb]
  const double L_1w  = 1.75e4; // integrated luminosity in 1 week [/pb]
  const double df    = 0.176; // dilution_factor
  const double pol   = 0.8;   // polarization
  const double X = sqrt(L_sim / L_1w) / df / pol;
  cout << "Integrated luminosity of all simDSTs = " << L_sim << " /pb\n"
       << "Error scaling factor = " << X << endl;
  for (int ii = 0; ii < N_PT; ii++) ERR_Y[ii] *= X;

  TGraphErrors* gr = new TGraphErrors(N_PT, VAL_X, VAL_Y, ERR_X, ERR_Y);
  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(kRed);
  gr->SetLineColor(kRed);

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  c1->DrawFrame(0.0, -0.3, 0.2, 0.3, "J/#psi | One Week;x_{2};Anticipated accuracy of A_{N}: #delta_{AN}");
  gr->Draw("PE1same");

  c1->SaveAs("gr_delta_an.png");
}
