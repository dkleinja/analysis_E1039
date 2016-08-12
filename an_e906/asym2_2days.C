// Generate xf asymmetries
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TLegend.h>
#include <TLine.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <iostream>

#include <stdio.h>

using namespace std;

#include <my_root_functions.C>
#include <asym_funcs.C>

void asym2_2days(const int Trig = 0, const int NBins = 1, const int Roadset = 67, const int Seed = 0, const int SaveOn = 0)
{
  
  //gROOT -> ProcessLine ("~/root/init.C");
  gROOT -> ProcessLine (".L ./my_root_functions.C");
  gROOT -> ProcessLine (".L ./asym_funcs.C");
  gROOT -> ProcessLine (".L ./fit_funcs.C");
  gROOT -> SetStyle("Plain");
  gStyle -> SetPalette (1);
  //gStyle -> SetOptTitle(0);
  gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(1);
  gStyle -> SetCanvasColor(0);
  Float_t PI = 3.141592654;
  //File input/output names
  char inName[64];
  char outName[64];
  char inArm[5];
  char inTrig[5];
  char inBeam[7];
  char inPol[5];
  if(Trig == 0) sprintf (inTrig, "Real");
  //else if(Trig == 2) sprintf (inTrig, "Sim");

  //Beam polarization
  Double_t targetPol = 1.0;
  Double_t targetPolError = 0.02;
  char Beam[6];
  char Poldir[5];
  char LRdir[6];

  TRandom *TRand = new TRandom3(0);
  TRand -> SetSeed(Seed);
  
  //ok, lets make our histograms!
  const Int_t nbins = NBins;//2 for mb, 4 for trig
  Float_t binjump = 3.5;
  if(nbins == 1) binjump = 4.5;
  Int_t mbins = 140;
  Double_t mmin = 0.;
  Double_t mmax = 14;
  const Int_t phibins = 12;
  const Int_t cosbins = 32;
  Double_t phimin = -PI;
  Double_t phimax = +PI;
  char Hname[128];

  //Yield histograms
  TH2F *Yield[2][nbins][2]; // [target][xf][pol]
  TH2F *Yieldmix[2][nbins];
  TH2F *Yield2[2][nbins][2]; // [target][xf][pol]
  TH2F *Yieldmix2[2][nbins];
  TH2F *Yield3[2][nbins]; // [target][xf]
  TH2F *Yieldmix3[2][nbins];
  TH1D *Hmass[2][nbins];
  for (int a = 0; a < 2; a++)
    for (int k = 0; k < nbins; k++){
	for (int j = 0; j < 2; j++){
	  sprintf (Hname, "Yield_target%d_xf%d_p%d", a, k, j);
	  Yield[a][k][j] = new TH2F (Hname, Hname, mbins, mmin, mmax, phibins, phimin, phimax);
	  sprintf (Hname, "Yield2_target%d_xf%d_p%d", a, k, j);
	  Yield2[a][k][j] = new TH2F (Hname, Hname, mbins, mmin, mmax, 2, phimin, phimax);
	}
      sprintf (Hname, "Yield3_target%d_xf%d", a, k);
      Yield3[a][k] = new TH2F (Hname, Hname, mbins, mmin, mmax, cosbins, phimin, phimax);
      sprintf (Hname, "Hmass_target%d_xf%d", a, k);
      Hmass[a][k] = new TH1D (Hname, Hname, mbins, mmin, mmax);
    }

  //mixed event subtraction histograms
  TH1D *tmass;
  TH1D *tmass2;
  TH1D *tmass3;
  TH1D *hmass[phibins][2];
  TH1D *hmass2[2][2];
  TH1D *hmass3;

  //mean xf, pt, eta histos
  TH2F *yxfmass  = new TH2F ("yxfmass",  "yxfmass",  100, 0., 1., 100, 0., 10. );
  TH2F *yxfmass3  = new TH2F ("yxfmass3",  "yxfmass3",  100, 0., 1., 100, 0., 10. );
  TH1F *yeta  = new TH1F ("yeta",  "yeta", 300, 2.0, 5.0 );
  TH3F *yxfmasspt  = new TH3F ("yxfmasspt",  "yxfmasspt",  100, 0., 1., 100, 0., 10., 500, 0., 5. );
  TH3F *yxfmasspt3  = new TH3F ("yxfmasspt3",  "yxfmasspt3",  100, 0., 1., 100, 0., 10., 500, 0., 5. );
  TH3F *yxfmasseta  = new TH3F ("yxfmasseta",  "yxfmasseta",  100, 0., 1., 100, 0., 10., 300, 2., 5. );
  TH3F *yxfmasseta3  = new TH3F ("yxfmasseta3",  "yxfmasseta3",  100, 0., 1., 100, 0., 10., 300, 2., 5. );
  TH1F *xf_mean = new TH1F("xf_mean","xf_mean",10, 0., 1.);
  TH1F *pt_mean = new TH1F("pt_mean","pt_mean",10, 0., 1.);
  TH1F *eta_mean = new TH1F("eta_mean","eta_mean",10, 0., 1.);
  TH1D *txf3;
  TH1D *tpt3;
  TH1D *teta3;
  
  for (int p = 0; p < phibins; p++)
      for (int j = 0; j < 2; j++){
	sprintf (Hname, "mass_%d_%d", p, j);
	hmass[p][j] = new TH1D (Hname, Hname, mbins, mmin, mmax);
	hmass[p][j] -> GetXaxis() -> SetTitle ("m_{#gamma#gamma} (GeV/c^{2})");
	hmass[p][j] -> SetLineColor (1);
	hmass[p][j] -> Sumw2();

      }
  for (int p = 0; p < 2; p++)
      for (int j = 0; j < 2; j++){
	sprintf (Hname, "mass2_%d_%d", p, j);
	hmass2[p][j] = new TH1D (Hname, Hname, mbins, mmin, mmax);
	hmass2[p][j] -> GetXaxis() -> SetTitle ("m_{#gamma#gamma} (GeV/c^{2})");
	hmass2[p][j] -> SetLineColor (1);
	hmass2[p][j] -> Sumw2();

      }

  sprintf (Hname, "mass3");
  hmass3 = new TH1D (Hname, Hname, mbins, mmin, mmax);
  hmass3 -> GetXaxis() -> SetTitle ("m_{#gamma#gamma} (GeV/c^{2})");
  hmass3 -> SetLineColor (1);
  hmass3 -> Sumw2();
  
  //phi histograms to grab mass projections in eta range!
  TH1D *hphi_sub[2][nbins][2];   
  TH1D *hphi_sub2[2][nbins][2]; 
  TH1D *hphi_raw3[2][nbins];  
  TH1D *hphi_sub3[2][nbins];  

  // tmp histograms to get the y-projections of the Yield[k] [j] around the pi0 mass
  TH1D *tphi = new  TH1D ("tphi", "tphi", phibins, phimin, phimax); tphi -> Sumw2();
  TH1D *tphi2 = new  TH1D ("tphi2", "tphi2", 2, phimin, phimax); tphi2 -> Sumw2();
  TH1D *tphi3 = new  TH1D ("tphi3", "tphi3", cosbins, phimin, phimax); tphi3 -> Sumw2();
  TH1D *mphi3 = new  TH1D ("mphi3", "mphi3", cosbins, phimin, phimax); mphi3 -> Sumw2();

  for (int a = 0; a < 2; a++)
    for (int k = 0; k < nbins; k++){
      sprintf (Hname, "phi_raw3_%d_%d", a, k);
      hphi_raw3[a][k] = new TH1D (Hname, Hname, cosbins, phimin, phimax);
      hphi_raw3[a][k] -> SetLineColor (k + 1);
      hphi_raw3[a][k] -> Sumw2();
      sprintf (Hname, "phi_sub3_%d_%d", a, k);
      hphi_sub3[a][k] = new TH1D (Hname, Hname, cosbins, phimin, phimax);
      hphi_sub3[a][k] -> SetLineColor (k + 1);
      hphi_sub3[a][k] -> Sumw2();
      
	for (int j = 0; j < 2; j++){	  
	  sprintf (Hname, "phi_sub_%d_%d_%d", a, k, j);
	  hphi_sub[a][k][j] = new TH1D (Hname, Hname, phibins, phimin, phimax);
	  hphi_sub[a][k][j] -> SetLineColor (2 * j + 1);
	  hphi_sub[a][k][j] -> Sumw2();
	  sprintf (Hname, "phi_sub2_%d_%d_%d", a, k, j);
	  hphi_sub2[a][k][j] = new TH1D (Hname, Hname, 2, phimin, phimax);
	  hphi_sub2[a][k][j] -> SetLineColor (2 * j + 1);
	  hphi_sub2[a][k][j] -> Sumw2();
	}
    }

  // Phi distributions for different polarizations!!
  TH1D *yphi[2][nbins][2];      //[target][xf][pol]
  TH1D *yphi2[2][nbins][2];   
  TH1D *yphi3[2][nbins];   
  for (int a = 0; a < 2; a++)  
    for (int k = 0; k < nbins; k++){
      sprintf (Hname, "yphi3_target%d_xf%d", a, k);
      yphi3[a][k] = new TH1D (Hname, Hname, cosbins, phimin, phimax);
      yphi3[a][k] -> SetLineColor (k + 1);
      yphi3[a][k] -> Sumw2();
 
      for (int d = 0; d < 2; d++) { // down-up
	sprintf (Hname, "yphi_target%d_xf%d_%d", a, k, d);
	yphi[a][k][d] = new TH1D (Hname, Hname, phibins, phimin, phimax);
	yphi[a][k][d] -> SetLineColor (d + 1);
	yphi[a][k][d] -> Sumw2();
	sprintf (Hname, "yphi2_target%d_xf%d_%d", a, k, d);
	yphi2[a][k][d] = new TH1D (Hname, Hname, 2, phimin, phimax);
	yphi2[a][k][d] -> SetLineColor (d + 1);
	yphi2[a][k][d] -> Sumw2();
      }
    }
  
  // ---------------------------------------------------------------------
  // Polarization asymmetries [target][xf]
  TH1D *asymP[2][nbins];
  TH1D *asymP2[2][nbins];
  for (int a = 0; a < 2; a++) 
    for (int k = 0; k < nbins; k++) {
      sprintf (Hname, "asymPol_target%d_xf%d", a, k);
      asymP[a][k] = new TH1D (Hname, Hname, phibins, phimin, phimax); // full detector
      asymP[a][k] -> GetXaxis() -> SetTitle ("#varphi (rad)");
      asymP[a][k] -> GetYaxis() -> SetTitle ("#epsilon_{pol}");
      asymP[a][k] -> SetMarkerStyle (21);
      
      sprintf (Hname, "asymPol3_target%d_xf%d", a, k);
      asymP2[a][k] = new TH1D (Hname, Hname, 1, 0, 1); // only one bin
      asymP2[a][k] -> GetXaxis() -> SetTitle ("#varphi (rad)");
      asymP2[a][k] -> GetYaxis() -> SetTitle ("#epsilon_{pol}");
      asymP2[a][k] -> SetMarkerStyle (21);
    }
 
  TH1D *asymF[2][nbins];  // polarization asymmetries, cleaned from rel.lumi. (fit) !!!
  for (int a = 0; a < 2; a++)  
    for (int k = 0; k < nbins; k++) {
      sprintf (Hname, "asymPol2_target%d_xf%d", a, k);
      asymF[a][k] = new TH1D (Hname, Hname, phibins, phimin, phimax); // full detector
      asymF[a][k] -> GetXaxis() -> SetTitle ("#varphi (rad)");
      asymF[a][k] -> GetYaxis() -> SetTitle ("#epsilon_{pol}");
      asymF[a][k] -> SetMarkerStyle (21);
    }
  
  // ---------------------------------------------------------
  // left right asymmetries !!!
  TH1D *asymL[2][nbins];  // [target][xf]
  for (int a = 0; a < 2; a++)  
    for (int k = 0; k < nbins; k++) {  
      sprintf (Hname, "asymSide_target%d_xf%d", a, k);
      asymL[a][k] = new TH1D (Hname, Hname, phibins, phimin, phimax); // full detector
      asymL[a][k] -> GetXaxis() -> SetTitle ("#varphi (rad)");
      asymL[a][k] -> SetMarkerStyle (21);
    }
  
  // ---------------------------------------------------------
  // Square root asymmetries !!!
  TH1D *asymS[2][nbins];  // [target][xf]
  TH1D *asymS2[2][nbins];  // [target][xf]
  TH1F *anval1[2][nbins];
  TH1F *anval5[2][nbins];

  for (int a = 0; a < 2; a++)  
    for (int k = 0; k < nbins; k++) { 
      sprintf (Hname, "asymSqrt_target%d_xf%d", a, k);
      asymS[a][k] = new TH1D (Hname, Hname, phibins /2, phimin, 0); // only left side 
      asymS[a][k] -> GetXaxis() -> SetTitle ("#varphi (rad)");
      asymS[a][k] -> GetYaxis() -> SetTitle ("#epsilon_{sqrt}");
      asymS[a][k] -> SetMarkerStyle (21);
      sprintf (Hname, "asymSqrt2_target%d_xf%d", a, k);
      asymS2[a][k] = new TH1D (Hname, Hname, phibins / 2, phimin, 0); // only left side 
      asymS2[a][k] -> GetXaxis() -> SetTitle ("#varphi (rad)");
      asymS2[a][k] -> GetYaxis() -> SetTitle ("#epsilon_{sqrt}");
      asymS2[a][k] -> SetMarkerStyle (21);
      
      sprintf(Hname, "anval5_target%d_xf%d", a, k);
      anval5[a][k] = new TH1F(Hname, Hname, 200, -1, 1);
      anval5[a][k] -> GetXaxis() -> SetTitle ("Counts");
      anval5[a][k] -> GetYaxis() -> SetTitle ("A_{N}");

      sprintf(Hname, "anval1_target%d_xf%d", a, k);
      anval1[a][k] = new TH1F(Hname, Hname, 200, -1, 1);
      anval1[a][k] -> GetXaxis() -> SetTitle ("Counts");
      anval1[a][k] -> GetYaxis() -> SetTitle ("A_{N}");
    }

  //functions to fit azimuthal asymmetries!
  TF1 *fitcos  = new TF1 ("an_cos",  an_cos,  -PI, 0., 1); // cosine to sqrt-asymmetry
  TF1 *fitsin  = new TF1 ("an_sin",  an_sin,  -PI, 0., 2); // sine with phase to saqrt-asym
  TF1 *fitcosP = new TF1 ("an_cosP", an_cosP, -PI, PI, 2); // cosine with r.l. to P-asymmetry
  TF1 *fitsinP = new TF1 ("an_sinP", an_sinP, -PI, PI, 3); // sine with phase and r.l. to P-asym
  TF1 *fitcosF = new TF1 ("an_cosF", an_cos,  -PI, PI, 1); // cosine to asymF
  fitcos -> SetLineColor(2);
  fitcosF -> SetLineColor(4);

  double amp_val, amp_err;
  double phi_val, phi_err;
  double rel_val, rel_err;
  double ndf, csq;
  
  // Asymmetries as function of xf !!!
  TH1F *xf_asymfit1[2];
  TH1F *xf_asymfit2[2];
  TH1F *xf_phi0fit2[2];
  TH1F *xf_rellfit3[2];
  TH1F *xf_asymfit3[2];
  TH1F *xf_rellfit4[2];
  TH1F *xf_asymfit4[2];
  TH1F *xf_phi0fit4[2];
  TH1F *xf_asymfit5[2];
 
  TH1F *fit_rndf1[2];
  TH1F *fit_rndf2[2];
  TH1F *fit_rndf3[2];
  TH1F *fit_rndf4[2];
  TH1F *fit_rndf5[2];
  TH1F *xf_an5[2];
  TH1F *xf_an1[2];
  TH1F *xf_an3[2];
  TH1F *xf_an6[2];
  TH1F *xf_an7[2];

  for (int a = 0; a < 2; a++){
    sprintf(Hname, "xf_asymfit1_target%d", a);
    xf_asymfit1[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_asymfit1[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_asymfit1[a] -> GetYaxis() -> SetTitle ("raw asymmetry #epsilon");
    SetHistoStyle (xf_asymfit1[a], 20, 2, 1); 

    sprintf(Hname, "xf_asymfit2_target%d", a);
    xf_asymfit2[a] = new TH1F (Hname, Hname, 10, 0, 1);
    sprintf(Hname, "xf_phi0fit2_target%d", a);
    xf_phi0fit2[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_asymfit2[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_asymfit2[a] -> GetYaxis() -> SetTitle ("raw asymmetry #epsilon");
    xf_phi0fit2[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_phi0fit2[a] -> GetYaxis() -> SetTitle ("#varphi_{0}");
    SetHistoStyle (xf_asymfit2[a], 24, 2, 1);
    SetHistoStyle (xf_phi0fit2[a], 24, 2, 1);
    SetHistoAxes  (xf_phi0fit2[a], -1, 1, -.5 * PI, 1.5 * PI);


    sprintf(Hname, "xf_rellfit3_target%d", a);
    xf_rellfit3[a] = new TH1F (Hname, Hname, 10, 0, 1);
    sprintf(Hname, "xf_asymfit3_target%d", a);
    xf_asymfit3[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_rellfit3[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_rellfit3[a] -> GetYaxis() -> SetTitle ("rel.lumi.");
    xf_asymfit3[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_asymfit3[a] -> GetYaxis() -> SetTitle ("raw asymmetry #epsilon");
    SetHistoStyle (xf_rellfit3[a], 21, 8,  1); 
    SetHistoStyle (xf_asymfit3[a], 21, 8,  1);

    sprintf(Hname, "xf_rellfit4_target%d", a);  
    xf_rellfit4[a] = new TH1F (Hname, Hname, 10, 0, 1);
    sprintf(Hname, "xf_asymfit4_target%d", a);
    xf_asymfit4[a] = new TH1F (Hname, Hname, 10, 0, 1);
    sprintf(Hname, "xf_phi0fit4_target%d", a);
    xf_phi0fit4[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_rellfit4[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_rellfit4[a] -> GetYaxis() -> SetTitle ("rel.lumi.");
    xf_asymfit4[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_asymfit4[a] -> GetYaxis() -> SetTitle ("raw asymmetry #epsilon");
    xf_phi0fit4[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_phi0fit4[a] -> GetYaxis() -> SetTitle ("#varphi_{0}");
    SetHistoStyle (xf_rellfit4[a], 25, 8, 1);
    SetHistoStyle (xf_asymfit4[a], 25, 8, 1);
    SetHistoStyle (xf_phi0fit4[a], 24, 8, 1);
    SetHistoAxes  (xf_phi0fit4[a], -1, 1, -.5 * PI, 1.5 * PI);


    // Yes-yes, this is the final fit: subtract the global rel. luminosity and fit plain cosine !!!
    sprintf(Hname, "xf_asymfit5_target%d", a);  
    xf_asymfit5[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_asymfit5[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_asymfit5[a] -> GetYaxis() -> SetTitle ("raw asymmetry #epsilon");
    SetHistoStyle (xf_asymfit5[a], 21, 4, 1);
      
    sprintf(Hname, "fit_rndf1_target%d", a);  
    fit_rndf1[a] = new TH1F (Hname, Hname, 100, 0., 10.);
    sprintf(Hname, "fit_rndf2_target%d", a);  
    fit_rndf2[a] = new TH1F (Hname, Hname, 100, 0., 10.);
    sprintf(Hname, "fit_rndf3_target%d", a);  
    fit_rndf3[a] = new TH1F (Hname, Hname, 100, 0., 10.);
    sprintf(Hname, "fit_rndf4_target%d", a);  
    fit_rndf4[a] = new TH1F (Hname, Hname, 100, 0., 10.);
    sprintf(Hname, "fit_rndf5_target%d", a);  
    fit_rndf5[a] = new TH1F (Hname, Hname, 100, 0., 10.);
    fit_rndf1[a] -> GetXaxis() -> SetTitle ("#chi^{2} / (n.d.f. - 1)");
    fit_rndf2[a] -> GetXaxis() -> SetTitle ("#chi^{2} / (n.d.f. - 1)");
    fit_rndf3[a] -> GetXaxis() -> SetTitle ("#chi^{2} / (n.d.f. - 1)");
    fit_rndf4[a] -> GetXaxis() -> SetTitle ("#chi^{2} / (n.d.f. - 1)");
    fit_rndf5[a] -> GetXaxis() -> SetTitle ("#chi^{2} / (n.d.f. - 1)");

    // And look: then there is the analyzing power !!!
    sprintf(Hname, "xf_an5_target%d", a);  
    xf_an5[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_an5[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_an5[a] -> GetYaxis() -> SetTitle ("A_{N}");
    SetHistoStyle (xf_an5[a], 21, 4, 1);
    sprintf(Hname, "xf_an1_target%d", a);  
    xf_an1[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_an1[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_an1[a] -> GetYaxis() -> SetTitle ("A_{N}");
    SetHistoStyle (xf_an1[a], 21, 2, 1);
    sprintf(Hname, "xf_an3_target%d", a);  
    xf_an3[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_an3[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_an3[a] -> GetYaxis() -> SetTitle ("A_{N}");
    SetHistoStyle (xf_an3[a], 25, 8, 1);
    sprintf(Hname, "xf_an6_target%d", a);  
    xf_an6[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_an6[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_an6[a] -> GetYaxis() -> SetTitle ("A_{N}");
    SetHistoStyle (xf_an6[a], 25, 2, 1);
    sprintf(Hname, "xf_an7_target%d", a);  
    xf_an7[a] = new TH1F (Hname, Hname, 10, 0, 1);
    xf_an7[a] -> GetXaxis() -> SetTitle ("x_{F}");
    xf_an7[a] -> GetYaxis() -> SetTitle ("A_{N}");
    SetHistoStyle (xf_an7[a], 25, 4, 1);

    
  }

  // ============= Let's roll squared: get the data !!!
  int arm;
  int pol, polT;
  Float_t xf;
  Float_t phi, phi2, phi3;
  Float_t pt;

  //initialize kdimuon tree
  Int_t dimuonID;
  Int_t runID;
  Int_t spillID, eventID;
  Int_t negTrackID, posTrackID;
  Int_t negHits, posHits;
  Float_t xF, xT, xB, mass;
  Float_t dx, dy, dz;
  Float_t dpx, dpy, dpz;
  Float_t px1, py1, pz1;
  Float_t px2, py2, pz2;
  Float_t trackSeparation, chisq_dimuon;
  Int_t target, dump;
  Int_t targetPos;
  Float_t m3hm, m3hs, m3vm, m3vs;
  Float_t m2hm, m2hs, m2vm, m2vs;
  Float_t dhm, dvm;
  Float_t costh, phi;
  Float_t eta;
  TLorentzVector lordm;// = new TLorentzVector;

  if(Trig == 0)  sprintf (inName, "./nDST/Analysis_roadset%d_R005_V001.root", Roadset);
  //if(Trig == 2)  sprintf (inName, "./nDST/Analysis_mc_drellyan_LD2_M013_S001.root");

  TFile *inFile1 = new TFile (inName);
  gROOT -> cd();
  
  // ====================================================== Read the reconstructed cluster pairs !!!
  TTree *YieldTree = (TTree*) inFile1 -> Get ("kdimuon"); 
  //YieldTree -> SetBranchAddress ("dimuonID",        &dimuonID);
  YieldTree -> SetBranchAddress ("runID",           &runID);
  //YieldTree -> SetBranchAddress ("eventID",         &eventID);
  YieldTree -> SetBranchAddress ("spillID",         &spillID);
  //YieldTree -> SetBranchAddress ("posTrackID",    &posTrackID);
  //YieldTree -> SetBranchAddress ("negTrackID",    &negTrackID);
  //YieldTree -> SetBranchAddress ("posHits",    &posHits);
  //YieldTree -> SetBranchAddress ("negHits",    &negHits);
  //YieldTree -> SetBranchAddress ("dx",              &dx);
  //YieldTree -> SetBranchAddress ("dy",              &dy);
  //YieldTree -> SetBranchAddress ("dz",              &dz);
  YieldTree -> SetBranchAddress ("dpx",             &dpx);
  YieldTree -> SetBranchAddress ("dpy",             &dpy);
  YieldTree -> SetBranchAddress ("dpz",             &dpz);
  YieldTree -> SetBranchAddress ("mass",            &mass);
  YieldTree -> SetBranchAddress ("xF",              &xF);
  //YieldTree -> SetBranchAddress ("xB",              &xB);
  //YieldTree -> SetBranchAddress ("costh",              &costh);
  //YieldTree -> SetBranchAddress ("phi",              &phi);
  //YieldTree -> SetBranchAddress ("trackSeparation", &trackSeparation);
  //YieldTree -> SetBranchAddress ("chisq_dimuon",    &chisq_dimuon);
  //YieldTree -> SetBranchAddress ("px1",             &px1);
  //YieldTree -> SetBranchAddress ("py1",             &py1);
  //YieldTree -> SetBranchAddress ("pz1",             &pz1);
  //YieldTree -> SetBranchAddress ("px2",             &px2);
  //YieldTree -> SetBranchAddress ("py2",             &py2);
  //YieldTree -> SetBranchAddress ("pz2",             &pz2);
  YieldTree -> SetBranchAddress ("target",          &target);
  YieldTree -> SetBranchAddress ("dump",            &dump);
  //YieldTree -> SetBranchAddress ("targetPos",       &targetPos);

  //here's the for loop
  int nentries = YieldTree -> GetEntries();
  int spillID0;
  int daycount = 0;
  int DspillID;
  cout << "Number of entries: " << nentries << endl;
  for (int i = 0; i < nentries; i++) {
    //for (int i = 0; i < 10000; i++) {
    YieldTree -> GetEntry (i);
    if(i%100000 == 0) cout << "entry " << i << endl;
    if(i == 0 ) spillID0 = spillID;
    if(runID > 13800 && runID < 14799)continue;
    if(xT < 0.1 || xT > 0.4)continue;
    //Now do the kinematic cuts!
    xf = xF;
    lordm.SetXYZM(dpx, dpy, dpz, mass);
    pt = lordm.Pt();
    if(pt < 0.5) continue;
    //if(fabs(lordm.Py()/lordm.Px()) < 0.176 || fabs(lordm.Py()/lordm.Px()) > 5.67) continue;
    //if(fabs(lordm.Px()/lordm.Py()) < 0.176) continue;
    phi = lordm.Phi();

    //make a left right phi!
    phi2 = phi - PI/2;
    if(phi2 < -PI){phi2 = phi2 + 2*PI;}
    phi3 = phi - PI/2;
    if(phi3 < -PI){phi3 = phi3 + 2*PI;}

    if(nbins == 1){k = 0;}
    else{
      if(Trig == 0 || Trig == 2){int k = int (10 * xf);}
    }

    if(nbins == 1){
      if(xf < 0.2 || xf > 0.6)continue;}
    else if(nbins > 1){
      if(xf < 0.2 || xf > 0.7)continue;
    }
    if(nbins == 1){k = 0;}
    else{
      int k = int (10 * (xf - 0.2));
    }

    
    if (k < 0) continue;
    if (k > nbins-1) k = nbins - 1;
    //if (k > nbins-1) continue;

    //if(i == 0)cout << "entry, day, DspillID, polT " << i <<  " " << daycount << " " << DspillID << " " << polT <<endl;
    DspillID = float(spillID - spillID0) / 2600.;
    //if((spillID - spillID0) / 1300 > daycount){
    if(DspillID > daycount){
      if(Seed == 0){
      if(DspillID%2 == 0)polT = 0;      
      if(DspillID%2 == 1)polT = 1;      
      }
      else{
	polT = TRand -> Uniform(2);
      }
      //cout << "entry, day, Dspill, DspillID, polT " << i <<  " " << daycount << " " << spillID - spillID0 << " " << DspillID << " " << polT <<endl;
      daycount++;
    }

    //cout << "arm, k, phiy, phib, phi " << i << " " << arm << " " << k << " " << phiyellow << " " << phiblue << " " << phi << endl;
    Yield[target][k][polT] -> Fill (mass, phi);
    Yield2[target][k][polT] -> Fill (mass, phi2);
   
    Yield3[target][k] -> Fill (mass, phi);
    yxfmass  -> Fill (mass,xf);
    yeta  -> Fill (eta);
    yxfmasspt  -> Fill (mass,xf,pt);
    yxfmasseta  -> Fill (mass,xf,eta);
    
  }

  inFile1 -> Close();
  gROOT -> cd();
  //exit(1);
  // ================================================================== mass peak determination
  //                                                                    asymmetry calculation
  
  //Canvas to draw phi dists
  TCanvas *cpa[2];
  TCanvas *cpa2[2];
  TCanvas *c1;
  TCanvas *c2;

  c1 = new TCanvas();
    for (int j = 0; j < 2; j++){
      sprintf(Hname, "cpa_%d", j);
      cpa[j] = new TCanvas(Hname, Hname, 1800, 1200);
      cpa[j] -> Divide(3, 3);
      sprintf(Hname, "cpa2_%d", j);
      cpa2[j] = new TCanvas(Hname, Hname, 1200, 400);
      cpa2[j] -> Divide(2, 1);
    }

  for (int a = 0; a < 2; a++){ // These are the slices in dump/target !!
    if(a == 0) sprintf (inArm, "Dump");
    else if(a == 1) sprintf (inArm, "Target");
    if(a == 0){sprintf(Beam,"dump");}
    if(a == 1){sprintf(Beam,"target");}

    for (int k = 0; k < nbins; k++) { // These are the slices in xf !!!
       // Mass distribution for the mixed events !!!
      //labels!
      float xfTitleMin, xfTitleMax;
      float phiTitleMin, phiTitleMax;
      if(Trig == 0 || Trig == 2){
	xfTitleMin = 0.1 * float (k) + 0.2;
	xfTitleMax = 0.1 * float (k) + 0.3;
	if(NBins == 1){xfTitleMin = 0.2, xfTitleMax = 0.4;}
      }
      
      //fit range!
      printf("//////////////////////////////////");
      printf("GET PARAMETERS FOR TARGET %d, XF BIN %d \n", a, k);
      ///////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////

      c1->cd();
      printf("Doing mass distributions for target %d, xf bin%d \n", a, k);

      tmass3 = Yield3[a][k] -> ProjectionX();
      Hmass[a][k] -> Add (tmass3);
      Hmass[a][k] -> Draw();
      sprintf (Hname, "./images_xf/mass3_roadset%d_nbins%d_target%d_xf%d.gif", Roadset, nbins, a, k);
      if(SaveOn == 1) c1-> SaveAs(Hname);
      tmass3 -> Reset();

      //Set the phi distributions for cosavg!
      tphi3 = Yield3[a][k] -> ProjectionY ("tphi3", 1,140);
      hphi_raw3[a][k] -> Reset();
      hphi_raw3[a][k] -> Add (tphi3);
      hphi_sub3[a][k] -> Reset();
      hphi_sub3[a][k] -> Add (tphi3);
      
      //Do it for the bigger phi bins !!!
      printf("//////////////////////////////////");
      printf("BIG PHI BIN PART FOR XF BIN %d \n", k);
      ///////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////
      //one phi bin at a time, then subtract background!!

	for (int j = 0; j < 2; j++)
	  for(int p = 0; p < 2; p++){
	    if (j == 0) sprintf (inPol, "Down");
	    else if (j == 1) sprintf (inPol, "Up");
	    if (p == 0)sprintf (LRdir, "Left");
	    else if (p == 1)sprintf (LRdir, "Right");
	    c1->cd();
	    //Get invariant mass
	    printf("Doing mass distributions for %s xf bin%d, phi bin %s, %s direction \n", inArm, k, LRdir, inPol);

	    tmass2 = Yield2[a][k][j] -> ProjectionX("tmass2", p+1, p+1);
	    hmass2[p][j] -> Reset();
	    hmass2[p][j] -> Add (tmass2);

	    cpa2[j] -> cd(p+1);
	    int integral = hmass2[p][j] -> Integral(1, 140);
	    sprintf (Hname, "%d COUNTS! %s %s %s %s for %1.1f < x_{F} < %1.1f, %s", integral, inArm, inTrig, inBeam, inPol, xfTitleMin, xfTitleMax, LRdir);
	    hmass2[p][j] -> SetTitle(Hname);
	    hmass2[p][j] -> Draw();
	    tmass2 -> Reset();

	    // Set the phi distributions !!!
	    hphi_sub2[a][k][j] -> SetBinContent(p+1, hmass2[p][j] -> Integral(1, 140));
	  }//end of slices in big phi, pol direction!  
      //Save Canvas
	for (int j = 0; j < 2; j++){
	  cpa2[j] -> cd();
          sprintf (Hname, "./images_mass_xf/mass2_nbins%d_roadset%d_target%d_xf%d_D%d.gif", nbins, Roadset, a, k, j);
	  if(SaveOn == 1)cpa2[j] -> SaveAs(Hname);
	  }
	
      printf("//////////////////////////////////");
      printf("LITTLE PHI BIN PART FOR XF BIN %d \n", k);
      //one phi bin at a time, then subtract background!!
	for (int j = 0; j < 2; j++)
	  for(int p = 0; p < phibins; p++){
	    if (j == 0) sprintf (inPol, "Down");
	    else if (j == 1) sprintf (inPol, "Up");
	    phiTitleMin = PI/(phibins/2) * float(p) - PI;
	    phiTitleMax = PI/(phibins/2) * float(p) - PI + PI/(phibins/2);
	    c1->cd();

	    //Get the invariant mass
	    //tmass -> Reset();
	    tmass = Yield[a][k][j] -> ProjectionX("tmass", p+1, p+1);
	    hmass[p][j] -> Reset();
	    hmass[p][j] -> Add (tmass);

	    cpa[j] -> cd(p+1);
	    int integral = hmass[p][j] -> Integral(1, 140);
	    sprintf (Hname, "%d COUNTS! %s %s %s %s for %1.1f < x_{F} < %1.1f, %s", integral, inArm, inTrig, inBeam, inPol, xfTitleMin, xfTitleMax, LRdir);
	    hmass[p][j] -> SetTitle(Hname);
	    hmass[p][j] -> Draw();
	    tmass -> Reset();

	    hphi_sub[a][k][j] -> SetBinContent(p+1, hmass[p][j] -> Integral(1, 140));
	  }//end of slices in phi, pol direction!
    
	//Save Canvas
	for (int j = 0; j < 2; j++){
	  cpa[j] -> cd();
	  sprintf (Hname, "./images_mass_xf/masssub_nbins%d_Roadset%d_target%d_xf%d_B%d_D%d.gif", nbins, Roadset, a, k, j);
	  if(SaveOn == 1)cpa[j] -> SaveAs(Hname);
	}
	
      //Get the #eta meson Yields in phi!!
      printf("NOW GETTING THE SPIN, PHI DEPENDENT YIELDS FOR %s xf bin %d \n", inArm, k);
      yphi[a][k][0] -> Add (hphi_sub[a][k][0]); // target down
      yphi[a][k][1] -> Add (hphi_sub[a][k][1]); // target up
      yphi2[a][k][0] -> Add (hphi_sub2[a][k][0]); // target down
      yphi2[a][k][1] -> Add (hphi_sub2[a][k][1]); // target up
      yphi3[a][k] -> Add (hphi_sub3[a][k]); 
    
      c1 -> cd();
      // Calculate polarization asymmetries (asymP),left-right and square root asymmetries (asymS) !!!
      double n1, n2, n3, n4;
      double e1, e2, e3, e4;
      double asym_val, asym_err;
      double asym_val_up,asym_err_up,asym_val_down,asym_err_down;
      double asym_val_left,asym_err_left,asym_val_right,asym_err_right;
      //sqrt asymmetry
      for (int p = 0; p < phibins / 2; p++) {
	  n1 = yphi[a][k][1] -> GetBinContent (p + 1);
	  n2 = yphi[a][k][0] -> GetBinContent (p + 1);
	  n3 = yphi[a][k][1] -> GetBinContent (p + 1 + phibins / 2);
	  n4 = yphi[a][k][0] -> GetBinContent (p + 1 + phibins / 2);
	  e1 = sqrt(n1);
	  e2 = sqrt(n2);
	  e3 = sqrt(n3);
	  e4 = sqrt(n4);
	  if ((n1 > 9) && (n2 > 9) && (n3 > 9) && (n4 > 9)) {
	    asym_val = square_asym  (n1, n2, n3, n4);
	    asym_err = square_error (n1, n2, n3, n4, e1, e2, e3, e4);
	    asymS[a][k] -> SetBinContent (p + 1, asym_val);
	    asymS[a][k] -> SetBinError   (p + 1, asym_err);
	  }
	}
	//polarization asymmetry
	for (int p = 0; p < phibins; p++) {
	  n1 = yphi[a][k][1] -> GetBinContent (p + 1);
	  n2 = yphi[a][k][0] -> GetBinContent (p + 1);
	  e1 = sqrt(n1);
	  e2 = sqrt(n2);
	  if ((n1 > 9) && (n2 > 9)) {
	    asymP[a][k] -> SetBinContent (p + 1, simple_asym  (n1, n2));
	    asymP[a][k] -> SetBinError   (p + 1, simple_error (n1, n2, e1, e2));
	  }
	}
	//simple left-right asymmetry
	for (int p = 0; p < phibins / 2; p++) {
	  n1 = yphi[a][k][0] -> GetBinContent (p + 1 + phibins / 2);
	  n2 = yphi[a][k][0] -> GetBinContent (p + 1);
	  n3 = yphi[a][k][1] -> GetBinContent (p + 1);
	  n4 = yphi[a][k][1] -> GetBinContent (p + 1 + phibins / 2);
	  e1 = sqrt(n1);
	  e2 = sqrt(n2);
	  e3 = sqrt(n3);
	  e4 = sqrt(n4);
	  if ((n1 > 9) && (n2 > 9) && (n3 > 9) && (n4 > 9)) {
	    asym_val_up = simple_asym(n1, n2);
	    asym_err_up = simple_error(n1, n2, e1, e2);
	    asym_val_down = simple_asym(n3, n4);
	    asym_err_down = simple_error(n3, n4, e3, e4);
	    asym_val = weighted_mean(asym_val_up, asym_val_down, asym_err_up, asym_err_down);
	    asym_err = weighted_error(asym_err_up, asym_err_down);
	    asymL[a][k] -> SetBinContent (p + 1, asym_val_up);
	    asymL[a][k] -> SetBinError   (p + 1, asym_err_down);
	  }
	}
      
      ////////////////////////////////////////////////
      //this is new do the cos average first
      double cosval[cosbins]; //[cosbins]
      int cosavg_den = 0;
      double cosavg_num = 0;
      double cosavg = 0;
      printf ("The f correction factor variables for %s xf bin %d. \n", inArm, k);
      printf ("%5s : %9s : %5s : %11s : %11s \n", "phibin", "cosval[p]", "n1", "cosavg_num", "cosavg_den");
      for (int p = 0; p < cosbins; p++) {
	n1 = 0;
	n1 = yphi3[a][k] -> GetBinContent (p + 1);
	cosval[p] = PI/(cosbins/2) * double(p) - PI + (PI/(cosbins/2)/2);
	if (n1 > 0) {
	  cosavg_num += (n1 * fabs( cos( cosval[p] ) ) );
	  cosavg_den += n1;
	}
	printf ("%5d : %1.2f : %5d : %1.2f : %1.1f \n", p, cosval[p], n1, cosavg_num, cosavg_den);
      }
      cosavg = cosavg_num / double(cosavg_den);
      printf("The correction factor f for %s xf bin %d is %1.2f. \n", inArm, k, 1/cosavg);
      //end of cos avg function f
      
      /////////////////////////////////////////////////
      //this is new let's just do sqrt, left-right
      printf ("The sqrt-formula spin dependent yields for %s xf bin %d. \n", inArm, k);
      n1 = n2 = n3 = n4 = 0;
      n1 = yphi2[a][k][1] -> GetBinContent (1);
      n2 = yphi2[a][k][1] -> GetBinContent (2);
      n3 = yphi2[a][k][0] -> GetBinContent (1);
      n4 = yphi2[a][k][0] -> GetBinContent (2);
      e1 = sqrt(n1);
      e2 = sqrt(n2);
      e3 = sqrt(n3);
      e4 = sqrt(n4);
      if ((n1 > 9) && (n2 > 9) && (n3 > 9) && (n4 > 9)) {
	printf ("%5s : %5s : %5s : %5s : %5s : %5s : %5s : %5s \n", "n1", "n2", "n3", "n4", "e1", "e2", "e3", "e4");
	printf ("%5d : %5d : %5d : %5d : %5d : %5d : %5d : %5d \n", n1, n2, n3, n4, e1, e2, e3, e4);
	asym_val = (1/cosavg)*(square_asym  (n1, n2, n3, n4));
	asym_err = (1/cosavg)*(square_error (n1, n2, n3, n4, e1, e2, e3, e4));
	cout << "The sqrt-formula value of asym_val,asym_err after f correction is:  " << asym_val << ", " << asym_err << endl;
	asymS2[a][k] -> SetBinContent (1, asym_val);
	asymS2[a][k] -> SetBinError   (1, asym_err);
	}
      //this is the end of the new sqrt asym part
      
      /////////////////////////////////////////////////
      //this is new let's just do pol up-down
      printf ("The relative luminosity spin dependent yields for %s xf bin %d. \n", inArm, k);
      n1 = n2 = n3 = n4 = 0;
      n1 = yphi2[a][k][1] -> GetBinContent (1);
      n2 = yphi2[a][k][1] -> GetBinContent (2);
      n3 = yphi2[a][k][0] -> GetBinContent (1);
      n4 = yphi2[a][k][0] -> GetBinContent (2);
      e1 = sqrt(n1);
      e2 = sqrt(n2);
      e3 = sqrt(n3);
      e4 = sqrt(n4);
 
      if ((n1 > 9) && (n2 > 9) && (n3 > 9) && (n4 > 9)) {
	printf ("%5s : %5s : %5s : %5s : %5s : %5s : %5s : %5s : %5s : %11s : %11s \n", "phibin", "n1", "n2", "n3", "n4", "e1", "e2", "e3", "e4", "cosavg_num", "cosavg_den");
	printf ("%5d : %5d : %5d : %5d : %5d : %5d : %5d : %5d : %5d : %1.2f : %1.1f \n", p, n1, n2, n3, n4, e1, e2, e3, e4, cosavg_num, cosavg_den);
	asym_val_left = (1/cosavg)*(simple_asym  (n1, n3));
	asym_err_left = (1/cosavg)*(simple_error (n1, n3, e1, e3));
	asym_val_right = (1/cosavg)*(simple_asym  (n4, n2));
	asym_err_right = (1/cosavg)*(simple_error (n4, n2, e4, e2));
	asym_val = weighted_mean(asym_val_left,asym_val_right,asym_err_left,asym_err_right);
	asym_err = weighted_error(asym_err_left,asym_err_right);
	cout << "The rel-lum value of asym_val,asym_err after f correction for beam is:  " << asym_val << ", " << asym_err << endl;
	asymP2[a][k] -> SetBinContent (1, asym_val);
	asymP2[a][k] -> SetBinError   (1, asym_err);
      }

      //this is the end of the new pol asym part
      ////////////////////////////////////////////////
    
      // Determine the amplitude of the single spin asymmetries as function of xf !!!
      printf ("==============================================\n");
      printf (" fit results for %s xf-bin %d \n", inArm, k);
      printf ("----------------------------------------------\n");
      printf ("::: sqrt,pol-asymmetries, left-right ::: \n");
      amp_val = asymS2[a][k] -> GetBinContent(1);
      amp_err = asymS2[a][k] -> GetBinError(1);

      xf_an6[a] -> SetBinContent (binjump + k, amp_val / targetPol);
      xf_an6[a] -> SetBinError   (binjump + k, amp_err / targetPol);
      
      amp_val = asymP2[a][k] -> GetBinContent(1);
      amp_err = asymP2[a][k] -> GetBinError(1);

      xf_an7[a] -> SetBinContent (binjump + k, amp_val / targetPol);
      xf_an7[a] -> SetBinError   (binjump + k, amp_err / targetPol);
	
      printf ("----------------------------------------------\n");
      printf ("::: sqrt-asymmetries ::: \n");
      printf ("plain cosine fit: \n");
      fitcos -> SetParameters (0., 0.);
      asymS[a][k] -> Fit ("an_cos", "R");
      amp_val = fitcos -> GetParameter (0);
      amp_err = fitcos -> GetParError  (0);
      xf_asymfit1[a] -> SetBinContent (binjump + k, amp_val);
      xf_asymfit1[a] -> SetBinError   (binjump + k, amp_err);
      printf (" %5.3f +/- %5.3f \n", amp_val, amp_err);
      ndf     = fitcos -> GetNDF();
      csq     = fitcos -> GetChisquare();
      if (ndf > 1){
	fit_rndf1[a] -> Fill (csq / (ndf - 1));
      }

      xf_an1[a] -> SetBinContent (binjump + k, amp_val / targetPol);
      xf_an1[a] -> SetBinError   (binjump + k, amp_err / targetPol);
      anval1[a][k] -> Fill(amp_val);
      
      printf ("sine fit with free phase: \n");
      fitsin -> SetParameters (0., 1.5 * PI);
      asymS[a][k] -> Fit ("an_sin", "R");
      amp_val = fitsin -> GetParameter (0);
      amp_err = fitsin -> GetParError  (0);
      phi_val = fitsin -> GetParameter (1);
      phi_err = fitsin -> GetParError  (1);
      // ========>  correlation of sign of amplitude and phi0 !!! <========
      if (sin (phi_val) < 0.) {phi_val += PI; amp_val *= -1.;}
      phi_val = phase_modulo (phi_val);
      xf_asymfit2[a] -> SetBinContent (binjump + k, amp_val);
      xf_asymfit2[a] -> SetBinError   (binjump + k, amp_err);
      xf_phi0fit2[a] -> SetBinContent (binjump + k, phi_val - 0.5 * PI);
      xf_phi0fit2[a] -> SetBinError   (binjump + k, phi_err);
      printf (" %5.3f +/- %5.3f \n", amp_val, amp_err);
      printf (" %5.3f +/- %5.3f \n", phi_val, phi_err);
      ndf     = fitsin -> GetNDF();
      csq     = fitsin -> GetChisquare();
      if (ndf > 1){
	fit_rndf2[a] -> Fill (csq / (ndf - 1));
      }
      printf ("----------------------------------------------\n");
      printf ("::: pol-asymmetries :::");
      printf ("constant plus cosine fit: \n");
      fitcosP -> SetParameters (0., 0.);
      asymP[a][k] -> Fit ("an_cosP", "R");
      rel_val = fitcosP -> GetParameter (0);
      rel_err = fitcosP -> GetParError  (0);
      amp_val = fitcosP -> GetParameter (1);
      amp_err = fitcosP -> GetParError  (1);
      xf_rellfit3[a] -> SetBinContent (binjump + k, rel_val);
      xf_rellfit3[a] -> SetBinError   (binjump + k, rel_err);
      xf_asymfit3[a] -> SetBinContent (binjump + k, amp_val);
      xf_asymfit3[a] -> SetBinError   (binjump + k, amp_err);
      printf (" %5.3f +/- %5.3f \n", rel_val, rel_err);
      printf (" %5.3f +/- %5.3f \n", amp_val, amp_err);
      ndf     = fitcosP -> GetNDF();
      csq     = fitcosP -> GetChisquare();
      if (ndf > 1){
	fit_rndf3[a] -> Fill (csq / (ndf - 1));
      }
      printf ("constant plus sine fit with free phase: \n");
      fitsinP -> SetParameters (0., 1.5 * PI);
      asymP[a][k] -> Fit ("an_sinP", "R");
      rel_val = fitsinP -> GetParameter (0);
      rel_err = fitsinP -> GetParError  (0);
      amp_val = fitsinP -> GetParameter (1);
      amp_err = fitsinP -> GetParError  (1);
      phi_val = fitsinP -> GetParameter (2);
      phi_err = fitsinP -> GetParError  (2);
      // ========>  correlation of sign of amplitude and phi0 !!! <========
      if (sin (phi_val) < 0.) {phi_val += PI; amp_val *= -1.;}
      phi_val = phase_modulo (phi_val);
      xf_rellfit4[a] -> SetBinContent (binjump + k, rel_val);
      xf_rellfit4[a] -> SetBinError   (binjump + k, rel_err);
      xf_asymfit4[a] -> SetBinContent (binjump + k, amp_val);
      xf_asymfit4[a] -> SetBinError   (binjump + k, amp_err);
      xf_phi0fit4[a] -> SetBinContent (binjump + k, phi_val - 0.5 * PI);
      xf_phi0fit4[a] -> SetBinError   (binjump + k, phi_err);
      printf (" %5.3f +/- %5.3f \n", rel_val, rel_err);
      printf (" %5.3f +/- %5.3f \n", amp_val, amp_err);
      printf (" %5.3f +/- %5.3f \n", phi_val, phi_err);
      ndf     = fitsinP -> GetNDF();
      csq     = fitsinP -> GetChisquare();
      if (ndf > 1){
	fit_rndf4[a] -> Fill (csq / (ndf - 1));
      }
    }// End of the slices in k !!!
    cout << "END OF SLICES IN XF BINS!" << endl;
 
    // Determine the global relative luminosity !!!
    TLine *RLine;
    Double_t avRL;
    Double_t erRL;
    TF1 *relTarget   = new TF1 ("relTarget",   "pol0", -.7, -.1);
    xf_rellfit3[a] -> Fit ("relTarget",   "R");
    avRL = relTarget   -> GetParameter (0);
    erRL = relTarget   -> GetParError  (0);
    for (int k = 0; k < nbins; k++)
      for (int p = 0; p < phibins; p++)
	if (asymP[a][k] -> GetBinError   (p + 1)){ 
	  asymF[a][k] -> SetBinContent (p + 1, asymP[a][k] -> GetBinContent (p + 1) - avRL);
	  asymF[a][k] -> SetBinError   (p + 1, asymP[a][k] -> GetBinError   (p + 1));
	}

    RLine = new TLine (.1, avRL, .8, avRL); RLine -> SetLineWidth (3);
  
    for (int k = 0; k < nbins; k++) {
      printf ("==============================================\n");
      printf ("::: final pol-asymmetries :::");
      printf ("constant plus cosine fit (global relative luminosity): \n");
      fitcosF -> SetParameters (0., 0.);
      asymF[a][k] -> Fit ("an_cosF", "R");
      amp_val = fitcosF -> GetParameter (0);
      amp_err = fitcosF -> GetParError  (0);
      xf_asymfit5[a] -> SetBinContent (binjump + k, amp_val);
      xf_asymfit5[a] -> SetBinError   (binjump + k, amp_err);
      printf (" %5.3f +/- %5.3f \n", rel_val, rel_err);
      printf (" %5.3f +/- %5.3f \n", amp_val, amp_err);
      ndf     = fitcosF -> GetNDF();
      csq     = fitcosF -> GetChisquare();
      if (ndf > 1){
	fit_rndf5[a] -> Fill (csq / (ndf - 1));
      }
      
      xf_an5[a] -> SetBinContent (binjump + k, amp_val / targetPol);
      xf_an5[a] -> SetBinError   (binjump + k, amp_err / targetPol);
      anval5[a][k] -> Fill(amp_val);
    }
    // Images !!!
    /*   if(Peak == 0){
      xf_asymfit1[a] -> SetMinimum (-.08);
      xf_asymfit1[a] -> SetMaximum ( .13);
      xf_an1[a] -> SetMinimum (-.20);
      xf_an1[a] -> SetMaximum ( .25);
    }
    else if(Peak == 1 || Peak == 2){
      xf_asymfit1[a] -> SetMinimum (-.17);
      xf_asymfit1[a] -> SetMaximum ( .17);
      xf_an1[a] -> SetMinimum (-.35); 
      xf_an1[a] -> SetMaximum ( .25);
    }*/
    xf_an1[a] -> SetMinimum (-.1);
    xf_an1[a] -> SetMaximum ( .1);

    TLegend *Legend1 = new TLegend (.15, .7, .5, .88);
    Legend1 -> SetFillStyle (0);
    Legend1 -> SetLineColor (0);
    Legend1 -> AddEntry (xf_asymfit1[a], "#epsilon_{sqrt}:a*cos(#varphi)");
    Legend1 -> AddEntry (xf_asymfit3[a], "#epsilon_{pol}:b+a*cos(#varphi)");
    Legend1 -> AddEntry (xf_asymfit5[a], "#epsilon_{pol}:#bar{b}+a*cos(#varphi)");
    
    TLine *zxf = new TLine (0., 0., 1., 0.);
    char *asymxNameText = "./images_xf/asym2p_roadset%d_nbins%d_target%d.gif";
    sprintf (Hname, asymxNameText, Roadset, nbins, a);

    TCanvas *Casymx = new TCanvas ("asym", "asym", 500, 500);
    //Casymx -> Divide (1, 2);
    //Casymx -> cd(1);
    //xf_asymfit1[a] -> Draw();
    //xf_asymfit3[a] -> Draw("SAME");
    //xf_asymfit5[a] -> Draw("SAME");
    //Legend1 -> Draw ("SAME");
    //zxf -> Draw();
    //Casymx -> cd(2);
    xf_an1[a] -> Draw();
    xf_an5[a] -> Draw("same");
    zxf -> Draw();

    Casymx -> Update();
    if(SaveOn == 1) Casymx -> SaveAs (Hname);

    TLatex *Lxf[5];
    if(a == 1){neg = 1; pos = 0;}
    if(Trig == 0 || Trig == 2){
      if(nbins == 1){
	Lxf[0] = new TLatex (-2.8, .08, " 0.2 < x_{F} < 0.7");
      }
      else{
	Lxf[0] = new TLatex (-2.8, .08, "0.2 < x_{F} < 0.3");
	Lxf[1] = new TLatex (-2.8, .08, "0.3 < x_{F} < 0.4");
	Lxf[2] = new TLatex (-2.8, .08, "0.4 < x_{F} < 0.5");
	Lxf[3] = new TLatex (-2.8, .08, "0.5 < x_{F} < 0.6");
	Lxf[4] = new TLatex (-2.8, .16, "0.6 < x_{F} < 0.7");
      }
    }

    //    for (int k = 0; k < 3, k++){Lxf[k] -> SetTextSize (0.07);}
    TLine *phi_zero1 = new TLine (-PI, 0., 0., 0.);
    TLine *phi_zero2 = new TLine (-PI, 0., PI, 0.);

    TH1D *tasym1[2][nbins];
    TH1D *tasym2[2][nbins];
    TH1D *tasym3[2][nbins];
    TH1D *tasym4[2][nbins];
    TH1D *tasym5[2][nbins];

    char *asymsNameText = "./images_xf/asym2s_roadset%d_nbins%d_target%d.gif";
    sprintf (Hname, asymsNameText, Roadset, nbins, a);

    TCanvas *Casym4 = new TCanvas ("asyms", "asyms", 600, 400*(nbins));
    Casym4 -> Divide (1, nbins);
    for (int k = 0; k < nbins; k++){
      asymF[a][k] -> SetMinimum (-0.10);
      asymF[a][k] -> SetMaximum (0.10);
      Casym4 -> cd (k + 1);

      tasym5[a][k] = (TH1D*)asymF[a][k]->Clone();
      tasym5[a][k] -> Fit ("an_cosF", "R");
      tasym5[a][k] -> Draw();
      Lxf[k] -> Draw("same");
      phi_zero2 -> Draw("same");
    }
    if(SaveOn == 1)Casym4 -> SaveAs (Hname);

    char *asymqNameText = "./images_xf/asym2q_roadset%d_nbins%d_target%d.gif";
    sprintf (Hname, asymqNameText, Roadset, nbins, a);

    TCanvas *Casym5 = new TCanvas ("asymq", "asymq", 600, 400*(nbins));
    Casym5 -> Divide (1, nbins);
    for (int k = 0; k < nbins; k++){
      asymS[a][k] -> SetMinimum (-.10);
      asymS[a][k] -> SetMaximum ( .10);     
      
      Casym5 -> cd (k+1);
      
      tasym1[a][k] = (TH1D*)asymS[a][k]->Clone();
      tasym1[a][k] -> Fit ("an_cos", "R");
      tasym1[a][k] -> Draw();
      Lxf[k] -> Draw("same");
      phi_zero1 -> Draw("same");
    }
    if(SaveOn == 1)Casym5 -> SaveAs (Hname);
    TLine *defaultshiftblue   = new TLine (-1., PI, 1., PI);
    defaultshiftblue-> SetLineStyle (2);
    TLine *defaultshiftyellow = new TLine (-1., 0., 1., 0.);
    TLegend *Legend2 = new TLegend (.16, .65, .5, .88);
    //TLegend *Legend1 = new TLegend (.15, .7, .5, .88);
    Legend2 -> SetFillStyle (0);
    Legend2 -> SetLineColor (0);
    Legend2 -> AddEntry (xf_asymfit1[a], "#epsilon_{sqrt}:a*cos(#varphi)");
    Legend2 -> AddEntry (xf_asymfit2[a], "#epsilon_{sqrt}:a*cos(#varphi+#varphi_{0})");
    Legend2 -> AddEntry (xf_asymfit3[a], "#epsilon_{pol}:b+a*cos(#varphi)");
    Legend2 -> AddEntry (xf_asymfit4[a], "#epsilon_{pol}:b+a*cos(#varphi+#varphi_{0})");

    char *fitsNameText = "./images_xf/fitsummary_roadset%d_nbins%d_target%d.gif";
    sprintf (Hname, fitsNameText, Roadset, nbins, a);

    //printf("%s \n", Hname);
    TCanvas *Cfits = new TCanvas ("fitsummary", "fitsummary", 1000, 1000);
    Cfits -> Divide (2, 2);
    
   
    Cfits -> cd (1);
    xf_asymfit1[a] -> Draw();
    xf_asymfit2[a] -> Draw("same");
    xf_asymfit3[a] -> Draw("same");
    xf_asymfit4[a] -> Draw("same");
    Legend2 -> Draw ("same");
    zxf -> Draw();
   
    Cfits -> cd (3);
    xf_rellfit3[a] -> Draw();
    xf_rellfit4[a] -> Draw ("same");
    RLine -> Draw();

    Cfits -> cd (4);
    xf_phi0fit4[a] -> Draw();
    defaultshiftblue -> Draw("same");
    defaultshiftyellow -> Draw("same");

    Cfits -> cd (2);
    xf_phi0fit2[a] -> Draw();
    defaultshiftblue   -> Draw("same");
    defaultshiftyellow -> Draw("same");  

    //Cfits -> cd();
    if(SaveOn == 1)Cfits -> SaveAs (Hname);

    //End of Images!!!
    printf ("==================================================\n");
    printf ("Arm %s mean target relative luminosity:   %5.3f +/- %5.3f \n", inArm, avRL, erRL);
    printf ("==================================================\n\n");
    
  }  //End of Arms!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11

  //Let's get the average pt, xf
  /*
  for (int k = 0; k < nbins; k++) {
    //labels!
    float xfTitleMin, xfTitleMax;
    xfTitleMin = 0.1 * float (k) + 0.2;
    xfTitleMax = 0.1 * float (k) + 0.3;
    if(NBins == 1){xfTitleMin = 0.2, xfTitleMax = 0.7;}
      
    int kmin = xfTitleMin * 100 + 1;
    int kmax = xfTitleMax * 100 + 1;
    float meanxf;
    float rmsxf;
    float meanpt;
    float meaneta;
    for(int x = kmin; x < kmax; x++)
      for(int m = 1; m <= 140; m++){
	yxfmass3 -> SetBinContent(m, x, yxfmass -> GetBinContent(m, x));
	for(int p = 0; p < 500; p++){
	  yxfmasspt3 -> SetBinContent(m, x, p, yxfmasspt -> GetBinContent(m, x, p));
	}
	for(int p = 0; p < 300; p++){
	  yxfmasseta3 -> SetBinContent(m, x, p, yxfmasseta -> GetBinContent(m, x, p));
	}
      }     
    txf3 = yxfmass3 -> ProjectionY();
    meanxf = txf3 -> GetMean();
    txf3 -> Reset();
    yxfmass3 -> Reset();
    tpt3 = yxfmasspt3 -> ProjectionZ("tpt3");
    meanpt = tpt3 -> GetMean();
    tpt3 -> Reset();
    yxfmasspt3 -> Reset();
    c1 -> cd();
    yxfmasseta3 -> Draw();
    teta3 = yxfmasseta3 -> ProjectionZ("teta3");
    meaneta = teta3 -> GetMean();
    teta3 -> Reset();
    yxfmasseta3 -> Reset();
    cout << "k, meanxf, meanpt, meaneta " << k << " " << meanxf << " " << meanpt << " " << meaneta << endl;
    xf_mean -> SetBinContent(0.5 + (binjump + k) , meanxf);
    pt_mean -> SetBinContent(0.5 + (binjump + k) , meanpt);
    eta_mean -> SetBinContent(0.5 + (binjump + k) , meaneta);
    //exit(1);
  }
  
  //Let's get the average eta
  meaneta = yeta -> GetMean();
  cout << "The mean of pseudo-rapidity is " << meaneta << endl;
  */
  //Write out to OutFile!!
  char *outNameText = "./results/asym2_roadset%d_nbins%d_2days%d.root";
  sprintf (outName, outNameText, Roadset, nbins, Seed);

  TFile *outFile = new TFile (outName, "RECREATE");
    
    //yxfmass -> Write();
    //yxfmasspt -> Write();
    //yxfmass3  -> Write();
    //yxfmasspt3  -> Write();
    xf_mean -> Write();
    pt_mean -> Write();
    eta_mean -> Write();
    yeta -> Write();

    for (int a = 0; a < 2; a++) {
      
      xf_asymfit1[a] -> Write();
      
      xf_asymfit2[a] -> Write();
      xf_phi0fit2[a] -> Write();

      xf_rellfit3[a] -> Write();
      xf_asymfit3[a] -> Write();

      xf_rellfit4[a] -> Write();
      xf_asymfit4[a] -> Write();
      xf_phi0fit4[a] -> Write();

      xf_asymfit5[a] -> Write();
 
      fit_rndf1[a] -> Write();
      fit_rndf2[a] -> Write();
      fit_rndf3[a] -> Write();
      fit_rndf4[a] -> Write();
      fit_rndf5[a] -> Write();

      xf_an5[a] -> Write();
      xf_an1[a] -> Write();
      xf_an3[a] -> Write();
      xf_an6[a] -> Write();
      xf_an7[a] -> Write();
      for (int k = 0; k < nbins; k++) {
	Yield3[a][k] -> Write();
	Hmass[a][k] -> Write();
	yphi3[a][k] -> Write();
	hphi_sub3[a][k] -> Write();
	hphi_raw3[a][k] -> Write();
	for (int j = 0; j < 2; j++){
	  Yield[a][k][j] -> Write();
	  Yield2[a][k][j] -> Write();

	  
	  //hphi_raw[a][k][j] -> Write();
	  //hphi_raw2[a][k][j] -> Write();
	  hphi_sub[a][k][j] -> Write();
	  hphi_sub2[a][k][j] -> Write();

	}

	asymP[a][k] -> Write();
	asymP2[a][k] -> Write();
	asymS[a][k] -> Write();
	asymS2[a][k] -> Write();
	asymL[a][k] -> Write();
	asymF[a][k] -> Write();
	anval1[a][k] -> Write();
	anval5[a][k] -> Write();
	for (int d = 0; d < 2; d++) { // down-up
	  yphi[a][k][d] -> Write();
	  yphi2[a][k][d] -> Write();
	}
      }
    }
    
  /*
    for (int p = 0; p < phibins; p++)
    for (int j = 0; j < 2; j++){
    hmass[p][j] -> Write();
    }
    for (int p = 0; p < 2; p++)
    for (int j = 0; j < 2; j++){
    hmass2[p][j] -> Write();
    }
    hmass3 -> Write();
    }
  */
  outFile -> Write();
  outFile -> Close();
}
