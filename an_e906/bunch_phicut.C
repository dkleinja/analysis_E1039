#include <asym_funcs.C>
#include <fit_funcs.C>

void bunch_phicut(const int Trig = 0, const int Roadset = 67, const int NBins = 1)
{

  //general business
  gROOT -> ProcessLine (".L ./my_root_functions.C");
  gROOT -> ProcessLine (".L ./asym_funcs.C");
  //gROOT -> ProcessLine (".L ./fit_funcs.C");
  gROOT -> SetStyle("Plain");
  gStyle -> SetPalette (1);
  gStyle -> SetOptStat(1);
  gStyle -> SetOptFit(1);

   //general constants
  Float_t PI = 3.141592654;
  char Hname[64];
  char Tname[128];
  char Fname[64];
  char inName[128];
  char outName[64];
  char inArm[6];
  char inType[4];
  char inSign[20];
  char inTrig[5];
  if(Trig == 0) sprintf (inTrig, "Real");
  else if(Trig == 2) sprintf (inTrig, "Sim");
  const Int_t nbins = NBins;
  Int_t binjump = 3.5;
  if(NBins == 1) binjump = 4.5;

  //Load the asym Plots
  TH1F *an1[2]; //[arm]
  TH1F *an5[2];
  TH1F *Han1[2];
  TH1F *Han5[2];
  TH1F *Hfit_rndf1[2];
  TH1F *Hfit_rndf5[2];


  //Plots to be made
  TH1F *bunch1[2][nbins]; //[arm][xf]
  TH1F *bunch5[2][nbins];  TF1 *gausdist1[2][nbins];
  TF1 *gausdist5[2][nbins];
  TH1F *anval1[2][nbins];
  TH1F *anval5[2][nbins];
  TH1F *anval1err[2][nbins];
  TH1F *anval5err[2][nbins];

  TH1F *fit_rndf1[2];
  TH1F *fit_rndf5[2];

  for(int a = 0; a < 2; a++){
    if(a == 0) sprintf (inArm, "Dump");
    else if(a == 1) sprintf (inArm, "Target");
    if(a == 0) sprintf (inType, "L-R");
    else if(a == 1) sprintf (inType, "Azi");

    sprintf(Hname, "Hfit_rndf1_target%d", a);
    Hfit_rndf1[a] = new TH1F (Hname, Hname, 100, 0., 10.);
    sprintf(Hname, "Hfit_rndf5_target%d", a);
    Hfit_rndf5[a] = new TH1F (Hname, Hname, 100, 0., 10.);
    Hfit_rndf1[a] -> GetXaxis() -> SetTitle ("#chi^{2} / (n.d.f. - 1)");
    Hfit_rndf5[a] -> GetXaxis() -> SetTitle ("#chi^{2} / (n.d.f. - 1)");

    for (int k = 0; k < nbins; k++){ // These are the slices in xf !!!

      //labels!
      float xfTitleMin, xfTitleMax;
      if(Trig == 0 || Trig == 2){
	xfTitleMin = 0.1 * float (k) + 0.2;
	xfTitleMax = 0.1 * float (k) + 0.3;
	if(NBins == 1){xfTitleMin = 0.2, xfTitleMax = 0.7;}
      }

      sprintf(Hname, "bunch1_target%d_xf%d", a, k);
      sprintf(Tname, "Sqrt A_{N}/#sigma_{A_{N}} 1 Day Shuff. for Roadset %d, %1.1f < x_{F} < %1.1f for %s", Roadset, xfTitleMin, xfTitleMax, inArm);
      bunch1[a][k] = new TH1F(Hname, Tname, 200, -5., 5);
      bunch1[a][k] -> GetYaxis() -> SetTitle("Counts");
      bunch1[a][k] -> GetXaxis() -> SetTitle("A_{N}^{b.s.}/#sigma_{A_{N}}");
      bunch1[a][k] -> Sumw2();

      sprintf(Hname, "bunch5_target%d_xf%d", a, k);
      sprintf(Tname, "Pol A_{N} 1 Day Shuff. for Roadset%d, %1.1f < x_FT} < %1.1f for %s", Roadset, xfTitleMin, xfTitleMax, inArm);
      bunch5[a][k] = new TH1F(Hname, Tname, 200, -5., 5);
      bunch5[a][k] -> GetYaxis() -> SetTitle("Counts");
      bunch5[a][k] -> GetXaxis() -> SetTitle("A_{N}^{b.s.}/#sigma_{A_{N}}");
      bunch5[a][k] -> Sumw2();

      sprintf(Hname, "gausdist_target%d_xf%d", a, k);
      gausdist1[a][k] = new TF1(Hname, "gaus", -2, 2);

      sprintf(Hname, "gausdist_target%d_xf%d", a, k);
      gausdist5[a][k] = new TF1(Hname, "gaus", -2, 2);

      sprintf(Hname, "anval5_target%d_xf%d", a, k);
      sprintf(Tname, "Sqrt A_{N} #chi^{2}/NDF for Roadset %d, %1.1f < x_{F} < %1.1f for %s", Roadset, xfTitleMin, xfTitleMax, inArm);
      anval5[a][k] = new TH1F(Hname, Hname, 200, -.1, .1);
      anval5[a][k] -> GetXaxis() -> SetTitle ("Counts");
      anval5[a][k] -> GetYaxis() -> SetTitle ("A_{N}");

      sprintf(Hname, "anval1_target%d_xf%d", a, k);
      sprintf(Tname, "Sqrt A_{N} 1 Day Shuff. for Roadset %d, %1.1f < x_{F} < %1.1f for %s", Roadset, xfTitleMin, xfTitleMax, inArm);
      anval1[a][k] = new TH1F(Hname, Hname, 200, -.1, .1);
      anval1[a][k] -> GetYaxis() -> SetTitle ("Counts");
      anval1[a][k] -> GetXaxis() -> SetTitle ("A_{N}");


      sprintf(Hname, "anval1err_target%d_xf%d", a, k);
      sprintf(Tname, "Sqrt #sigma_{A_{N}} 1 Day Shuff. for Roadset %d, %1.1f < x_{F} < %1.1f for %s", Roadset, xfTitleMin, xfTitleMax, inArm);
      anval1err[a][k] = new TH1F(Hname, Hname, 200, -.1, .1);
      anval1err[a][k] -> GetYaxis() -> SetTitle ("Counts");
      anval1err[a][k] -> GetXaxis() -> SetTitle ("#sigma_{A_{N}}");

      sprintf(Hname, "anval5err_target%d_xf%d", a, k);
      sprintf(Tname, "Sqrt #sigma_{A_{N}} 1 Day Shuff. for Roadset %d, %1.1f < x_{F} < %1.1f for %s", Roadset, xfTitleMin, xfTitleMax, inArm);
      anval5err[a][k] = new TH1F(Hname, Hname, 200, -.1, .1);
      anval5err[a][k] -> GetYaxis() -> SetTitle ("Counts");
      anval5err[a][k] -> GetXaxis() -> SetTitle ("#sigma_{A_{N}}");

    }
  }

  //inFiles
  const int NFILES = 5000;
  int ifile;
  TFile *inFile[NFILES];
  
  sprintf(inName, "./results/phicut_asym2_roadset%d_nbins%d_byday0.root", Roadset, nbins);
  TFile *inFile1 = new TFile(inName, "READ");

  Float_t an1_val, an1_err;  
  Float_t an5_val, an5_err;  

  for(int a = 0; a < 2; a++){
      sprintf(Hname, "xf_an1_target%d", a);
      //sprintf(Hname, "xf_an6_target%d", a);
      Han1[a] = (TH1F*) inFile1 -> Get(Hname);
      sprintf(Hname, "xf_an5_target%d", a);
      sprintf(Hname, "xf_an6_target%d", a);
      Han5[a] = (TH1F*) inFile1 -> Get(Hname);
  }
  //Let's load each BS file!!
  //for (int i = 20001; i < 25001; i++){
  for (int i = 0; i < NFILES; i++){
    if(i%1000 == 0)cout << "doing BS file seed" << i << "." << endl;
    //load the inFile   
    if(Trig == 0) sprintf(inName, "./results/phicut_asym2_roadset%d_nbins%d_byday%d.root", Roadset, nbins, i);
    //printf(inName);
    inFile[i] = new TFile(inName);

    if(!inFile[i]){
      cout << "Unable to open file for Bunch iteration " << i << ".  Skipping this Bunch shuffling." << endl;
      continue;
    }
    //arm loop
    for(int a = 0; a < 2; a++){
      //xf bin loop
      sprintf(Hname,"fit_rndf1_target%d", a);
      fit_rndf1[a] = (TH1F*) inFile[i] -> Get(Hname);
      sprintf(Hname,"fit_rndf5_target%d", a);
      fit_rndf5[a] = (TH1F*) inFile[i] -> Get(Hname);

      Hfit_rndf1[a] -> Add(fit_rndf1[a]);
      Hfit_rndf5[a] -> Add(fit_rndf5[a]);

      for (int k = 0; k < nbins; k++){ // These are the slices in xf !!!
	//Get the bin value
	sprintf(Hname,"xf_an1_target%d", a);
	//sprintf(Hname,"xf_an6_target%d", a);
	an1[a] = (TH1F*) inFile[i] -> Get(Hname);
	sprintf(Hname,"xf_an5_target%d", a);
	sprintf(Hname,"xf_an7_target%d", a);
	an5[a] = (TH1F*) inFile[i] -> Get(Hname);
	
	an1_val = an1[a]-> GetBinContent(k + binjump);
	an5_val = an5[a]-> GetBinContent(k + binjump);
	//an1_err = Han1[a] -> GetBinError(k + binjump);
	//an5_err = Han5[a] -> GetBinError(k + binjump);
	an1_err = an1[a] -> GetBinError(k + binjump);
	an5_err = an5[a] -> GetBinError(k + binjump);
	//fill the bunch shuffling histograms 
	bunch1[a][k] -> Fill(an1_val / an1_err);
	bunch5[a][k] -> Fill(an5_val / an5_err);
	anval1[a][k] -> Fill(an1_val );
	anval1err[a][k] -> Fill(an1_err );
	anval5[a][k] -> Fill(an5_val );
	anval5err[a][k] -> Fill(an5_err );
      }
    }
    //delete the file
    inFile[i] -> Close();
    inFile[i] -> Delete();
  }//end of BS files
  
  //Draw the BS values!
  TCanvas *ca1[2];
  TCanvas *ca5[2];
  for(int a = 1; a < 2; a++){
    sprintf(Hname, "ca1%d", a);
    ca1[a] = new TCanvas(Hname, Hname, 1800, 400*nbins);
    ca1[a] -> Divide(3, nbins);
    sprintf(Hname, "ca5%d", a);
    ca5[a] = new TCanvas(Hname, Hname, 1800, 400*nbins);
    ca5[a] -> Divide(3, nbins);

    for (int k = 0; k < nbins; k++){ // These are the slices in xf !!!
      ca1[a] -> cd (2*k + 1);
      bunch1[a][k] -> Fit(gausdist1[a][k],"R");
      bunch1[a][k] -> Draw();
      anval1err[a][k] -> Draw();
      ca1[a] -> cd (2*k + 2);
      anval1[a][k] -> Draw();
      if(NBins == 1){
	ca1[a] -> cd (2*k + 3);
	Hfit_rndf1[a] -> Draw();
      }

      ca5[a] -> cd (2*k + 1);
      bunch5[a][k] -> Fit(gausdist5[a][k],"R");
      bunch5[a][k] -> Draw();
      anval5err[a][k] -> Draw();
      ca5[a] -> cd (2*k + 2);
      anval5[a][k] -> Draw();
      if(NBins == 1){
	ca5[a] -> cd (2*k + 3);
	Hfit_rndf5[a] -> Draw();
      }
    }
    ca1[a] -> cd();
    sprintf(Hname, "./bunch_an1_byday_nbins%d_roadset%d_target%d.gif", nbins, Roadset, a);
    ca1[a] -> SaveAs(Hname);
    ca5[a] -> cd();
    sprintf(Hname, "./bunch_an5_byday_nbins%d_roadset%d_target%d.gif", nbins, Roadset, a);
    ca5[a] -> SaveAs(Hname);
  }

}
