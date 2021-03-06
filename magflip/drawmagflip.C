void drawmagflip(const int dmrebin = 4, const int roadset0 = 57, const int roadset1 = 62)
{

  gStyle -> SetOptFit(1);
  const Float_t PI = 3.14159;

  char Fname[128];
  char Hname[128];
  char Tname[128];

  sprintf(Fname, "./root_files/phidists_roadset%d.root", roadset0);
  TFile *inFile0 = new TFile(Fname);
  sprintf(Fname, "./root_files/phidists_roadset%d.root", roadset1);
  TFile *inFile1 = new TFile(Fname);

  TH1D *Hdmphi0 = (TH1D*) inFile0 -> Get("Hdmphi");
  TH1D *Hdmphi1 = (TH1D*) inFile1 -> Get("Hdmphi");
  sprintf(Tname, "phi of dimuon roadset%d/[roadset%d * rellum]", roadset0, roadset1);
  TH1D *Hdmphirat = new TH1D("Hdmphirat", Tname, 64, -3.15, 3.15);

  TH1D *Hmmuonphi0 = (TH1D*) inFile0 -> Get("Hmmuonphi");
  TH1D *Hmmuonphi1 = (TH1D*) inFile1 -> Get("Hmmuonphi");
  TH1D *Hpmuonphi0 = (TH1D*) inFile0 -> Get("Hpmuonphi");
  TH1D *Hpmuonphi1 = (TH1D*) inFile1 -> Get("Hpmuonphi");
  sprintf(Tname, "#phi of  roadset%d(#mu^{-}) / [roadset%d(#mu^{+}) * rellum]", roadset0, roadset1);
  TH1D *Hmpmuonphirat = new TH1D("Hmpmuonphirat", Tname, 64, -3.15, 3.15);
  sprintf(Tname, "#phi of  roadset%d(#mu^{+}) / [roadset%d(#mu^{-}) * rellum]", roadset0, roadset1);
  TH1D *Hpmmuonphirat = new TH1D("Hpmmuonphirat", Tname, 64, -3.15, 3.15);
  sprintf(Tname, "#phi of  roadset%d(#mu^{-}) / [roadset%d(#mu^{-}) * rellum]", roadset0, roadset1);
  TH1D *Hmmmuonphirat = new TH1D("Hmmmuonphirat",Tname, 64, -3.15, 3.15);
  sprintf(Tname, "#phi of  roadset%d(#mu^{+}) / [roadset%d(#mu^{+}) * rellum]", roadset0, roadset1);
  TH1D *Hppmuonphirat = new TH1D("Hppmuonphirat",Tname, 64, -3.15, 3.15);
  
  TH1D *Hcharge0 = (TH1D*) inFile0 -> Get("Hcharge");
  TH1D *Hcharge1 = (TH1D*) inFile1 -> Get("Hcharge");
  sprintf(Tname,  "ratio of charge of ktracks roadset%d/[roadset%d * rellum]", roadset0, roadset1);
  TH1D *Hchargerat = new TH1D("Hchargerat", Tname, 4, -2, 2);
  Hcharge0 -> SetXTitle("Charge");
  Hcharge1 -> SetXTitle("Charge");
  Hchargerat -> SetXTitle("Charge");
  Hcharge0 -> SetYTitle("Counts");
  Hcharge1 -> SetYTitle("Counts");
  Hcharge1 -> SetYTitle("Ratio");

  Hdmphi0 -> SetXTitle("#phi");
  Hdmphi1 -> SetXTitle("#phi");
  Hdmphirat -> SetXTitle("#phi");
  Hdmphi0 -> SetYTitle("Counts");
  Hdmphi1 -> SetYTitle("Counts");
  Hdmphirat -> SetYTitle("Ratio");
  Hmmuonphi0 -> SetXTitle("#phi");
  Hmmuonphi1 -> SetXTitle("#phi");
  Hmmuonphi0 -> SetYTitle("Counts");
  Hmmuonphi1 -> SetYTitle("Counts");
  Hmpmuonphirat -> SetXTitle("#phi");
  Hmpmuonphirat -> SetYTitle("Ratio");
  Hpmuonphi0 -> SetXTitle("#phi");
  Hpmuonphi1 -> SetXTitle("#phi");
  Hpmuonphi0 -> SetYTitle("Counts");
  Hpmuonphi1 -> SetYTitle("Counts");
  Hpmmuonphirat -> SetXTitle("#phi");
  Hpmmuonphirat -> SetYTitle("Ratio");
  Hmmmuonphirat -> SetXTitle("#phi");
  Hmmmuonphirat -> SetYTitle("Ratio");
  Hppmuonphirat -> SetXTitle("#phi");
  Hppmuonphirat -> SetYTitle("Ratio");

  //initialize fit functions  
  TF1 *cos1x = new TF1("fa1","[0] + [1]*cos(x)",-3.14, 3.14); 
  cos1x -> SetParameters(1,1);

  TF1 *cos2x = new TF1("fa1","[0] + [1]*cos([2]*x)",-3.14, 3.14); 
  cos2x -> SetParameters(1,1,2);

  
  //initialize spill trees
  sprintf(Fname, "./nDST/Analysis_roadset%d_R005_V001_spill.root", roadset0);
  printf(Fname);
  TFile *inFile0_spill = new TFile(Fname);
  TTree *spilltree0 = (TTree*) inFile0_spill -> Get("spill");
  sprintf(Fname, "./nDST/Analysis_roadset%d_R005_V001_spill.root", roadset1);
  TFile *inFile1_spill = new TFile(Fname);
  TTree *spilltree1 = (TTree*) inFile1_spill -> Get("spill");

  Int_t runID, spillID;
  Float_t liveProton;
  Float_t lum0 = 0.;
  Float_t lum1 = 0.;
  Float_t rellum;

  int nentries2 = spilltree0 -> GetEntries();

  spilltree0 -> SetBranchAddress ("runID",           &runID);
  spilltree0 -> SetBranchAddress ("spillID",         &spillID);
  spilltree0 -> SetBranchAddress ("liveProton",         &liveProton);
  //ok, lets get relative luminosity
  int nentries2 = spilltree0 -> GetEntries();
  cout << "The number of Spill0 Entries is " << nentries2 << endl;
  for(int i = 0; i < nentries2; i++){
    spilltree0 -> GetEntry(i);
      lum0 += liveProton;
  }
  spilltree1 -> SetBranchAddress ("runID",           &runID);
  spilltree1 -> SetBranchAddress ("spillID",         &spillID);
  spilltree1 -> SetBranchAddress ("liveProton",         &liveProton);
  nentries2 = spilltree1 -> GetEntries();
  cout << "The number of Spill1 Entries is " << nentries2 << endl;
  for(int i = 0; i < nentries2; i++){
    spilltree1 -> GetEntry(i);
      lum1 += liveProton;
  }
  sprintf(Tname, "The number of live Proton in roadset %d is %1.3f \n", roadset0, lum0);
  printf(Tname);
  sprintf(Tname, "The number of live Proton in roadset %d is %1.3f \n", roadset1, lum1);
  printf(Tname);  
  rellum = lum0/lum1;
  cout << "The relative Luminosity is " << lum0/lum1 << endl;

  //rebin if necessary
  if(dmrebin == 2){
    Hdmphi0 -> Rebin(2);
    Hdmphi1 -> Rebin(2);
    Hdmphirat -> Rebin(2);
  }
  if(dmrebin == 4){
    Hdmphi0 -> Rebin(4);
    Hdmphi1 -> Rebin(4);
    Hdmphirat -> Rebin(4);
  }
  if(dmrebin == 8){
    Hdmphi0 -> Rebin(8);
    Hdmphi1 -> Rebin(8);
    Hdmphirat -> Rebin(8);
  }
  if(dmrebin == 16){
    Hdmphi0 -> Rebin(16);
    Hdmphi1 -> Rebin(16);
    Hdmphirat -> Rebin(16);
  }
  Hdmphirat -> Divide(Hdmphi0, Hdmphi1, 1, rellum);
  Hmpmuonphirat -> Divide(Hmmuonphi0, Hpmuonphi1, 1, rellum);
  Hpmmuonphirat -> Divide(Hpmuonphi0, Hmmuonphi1, 1, rellum);
  Hmmmuonphirat -> Divide(Hmmuonphi0, Hmmuonphi1, 1, rellum);
  Hppmuonphirat -> Divide(Hpmuonphi0, Hpmuonphi1, 1, rellum);
  Hchargerat -> Divide(Hcharge0, Hcharge1, 1, rellum);

  TLine *l0 = new TLine(-3.15, 1., 3.15, 1.);
  l0 -> SetLineStyle(2);

  TCanvas *c3 = new TCanvas("c3", "c3", 1800, 400);
  c3 -> Divide(3, 1);
  c3 -> cd(1);
  Hdmphi0 -> Draw();
  c3 -> cd(2);
  Hdmphi1 -> Draw();
  c3 -> cd(3);
  Hdmphirat -> Draw();
  Hdmphirat -> Fit("pol0");
  Hdmphirat -> Fit(cos1x);
  l0 -> Draw();
  sprintf(Hname, "./images/dimuon_roadset%d_over_roadset%d.gif", roadset0, roadset1);
  c3 -> SaveAs(Hname);

  TCanvas *c2 = new TCanvas("c2", "c2", 1800, 400);
  c2 -> Divide(3, 1);
  c2 -> cd(1);
  Hcharge0 -> Draw();
  c2 -> cd(2);
  Hcharge1 -> Draw();
  c2 -> cd(3);
  Hchargerat -> Draw();
  sprintf(Hname, "./images/charge_roadset%d_over_roadset%d.gif", roadset0, roadset1);
  c2 -> SaveAs(Hname);
  
  TCanvas *c6a = new TCanvas("c6a", "c6a", 1800, 800);
  c6a -> Divide(3, 2);
  c6a -> cd(1);
  Hmmuonphi0 -> Draw();
  c6a -> cd(2);
  Hpmuonphi1 -> Draw();
  c6a -> cd(3);
  Hmpmuonphirat -> Draw();
  //Hmpmuonphirat -> Fit("pol0");
  Hmpmuonphirat -> Fit(cos2x);
  l0 -> Draw();
  
  c6a -> cd(4);
  Hpmuonphi0 -> Draw();
  c6a -> cd(5);
  Hmmuonphi1 -> Draw();
  c6a -> cd(6);
  Hpmmuonphirat -> Draw();
  //Hpmmuonphirat -> Fit("pol0");
  Hpmmuonphirat -> Fit(cos2x);
  l0 -> Draw();
  sprintf(Hname, "./images/trackopp_roadset%d_over_roadset%d.gif", roadset0, roadset1);
  c6a -> SaveAs(Hname);

  TCanvas *c6b = new TCanvas("c6b", "c6b", 1800, 800);
  c6b -> Divide(3, 2);
  c6b -> cd(1);
  Hmmuonphi0 -> Draw();
  c6b -> cd(2);
  Hmmuonphi1 -> Draw();
  c6b -> cd(3);
  Hmmmuonphirat -> Draw();
  Hmmmuonphirat -> Fit("pol0");
  //Hmmmuonphirat -> Fit(cos1x);
  l0 -> Draw();
  
  c6b -> cd(4);
  Hpmuonphi0 -> Draw();
  c6b -> cd(5);
  Hpmuonphi1 -> Draw();
  c6b -> cd(6);
  Hppmuonphirat -> Draw();
  Hppmuonphirat -> Fit("pol0");
  //Hmmmuonphirat -> Fit(cos1x);
  l0 -> Draw();
  sprintf(Hname, "./images/tracklike_roadset%d_over_roadset%d.gif", roadset0, roadset1);
  c6b -> SaveAs(Hname);
  
}
