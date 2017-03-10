#include "drawhodo_dpsim2.h"

void drawhodo_dpsim2(const int dmrebin = 2, const int quad = 0)
{
  //get files
  sprintf(Fname, "./root_files/devel/phidists_dpsim_batch0_hodoEff0.root");
  //sprintf(Fname, "./root_files/phidists_dpsim_hodoEff0.root");
  TFile *inFile0 = new TFile(Fname);
  sprintf(Fname, "./root_files/devel/phidists_dpsim_batch0_hodoEff1.root");
  //sprintf(Fname, "./root_files/phidists_dpsim_hodoEff1.root");
  TFile *inFile1 = new TFile(Fname);
  
  gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(1);
  int nbins = 4;
  //initialize fit functions  
  //TF1 *cos1x = new TF1("fa1","[0]*cos(x)",-3.14, 3.14);
  TF1 *cos1x = new TF1("fa1","[0] + [1]*cos(x)",-3.14, 3.14); 
  cos1x -> SetParameters(1,1);
  TF1 *cos2x = new TF1("fa1","[0] + [1]*cos(2*x)",-3.14, 3.14); 
  cos2x -> SetParameters(1,1,0);
  TF1 *sinxsinx = new TF1("fa1","[0] + [1]*sin(x)*sin(x)",-3.14, 3.14); 
  sinxsinx -> SetParameters(0,0);

  //initialize histos
  TH1D *Hdmphirat = new TH1D("Hdmphirat", "Laboratory #phi hodoeff on/off ratio", 64, -3.15, 3.15);
  Hdmphirat -> SetXTitle("Laboratory #phi");
  Hdmphirat -> SetYTitle("Ratio");
  TH1D *Hpmuonphirat = new TH1D("Hpmuonphirat", "#mu^{+} Laboratory #phi hodoeff on/off ratio", 64, -3.15, 3.15);
  Hpmuonphirat -> SetXTitle("Laboratory #phi");
  Hpmuonphirat -> SetYTitle("Ratio");
  
  TH1D *Hmmuonphirat = new TH1D("Hmmuonphirat", "#mu^{-} Laboratory #phi hodoeff on/off ratio", 64, -3.15, 3.15);
  Hmmuonphirat -> SetXTitle("Laboratory #phi");
  Hmmuonphirat -> SetYTitle("Ratio");
  Hmmuonphirat -> SetLineColor(2);
  TH1D *Hdmcsphirat = new TH1D("Hdmcsphirat", "C-S #phi hodoeff on/off ratio", 64, -3.15, 3.15);
  Hdmcsphirat -> SetXTitle("C-S #phi");
  Hdmcsphirat -> SetYTitle("Ratio");
  //TH1D *Hdmcsthetarat = new TH1D("Hdmcsthetarat", "C-S cos(#theta) hodoeff on/off ratio", 100, -1.0, 1.0);
  TH1D *Hdmcsthetarat = new TH1D("Hdmcsthetarat", "C-S cos(#theta) hodoeff on/off ratio", 64, 0., 3.15);
  Hdmcsthetarat -> SetXTitle("C-S cos(#theta)");
  Hdmcsthetarat -> SetXTitle("C-S #theta");
  Hdmcsthetarat -> SetYTitle("Ratio");
  for(int a = 0; a < 2; a++){//[onoff][pt]
    if(a == 0)sprintf(onoff, "off");
    if(a == 1)sprintf(onoff, "on");
    
    sprintf(Hname, "Hallphi_hodo%d", a);
    sprintf(Tname, "Laboratory #phi, hodoeff=%s,  p_{T} > 0.5 GeV/c", onoff);
    Hallphi[a] = new TH3D(Hname, Tname, 64, -3.15, 3.15, 64, -3.15, 3.15, 64, -3.15, 3.15);
    Hallphi[a] -> SetXTitle("#mu^{+} Laboratory #phi");
    Hallphi[a] -> SetYTitle("#mu^{-} Laboratory #phi");
    Hallphi[a] -> SetZTitle("D-Y Laboratory #phi");
    Hallphi[a] -> SetMinimum(0.1);
    Hallphi[a] -> Sumw2();
    if(a==1)Hallphi[a] -> SetLineColor(2);
    
    sprintf(Hname, "Hphi_hodo%d", a);
    sprintf(Tname, "Laboratory #phi, hodoeff=%s,  p_{T} > 0.5 GeV/c", onoff);
    Hdmphi[a] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
    Hdmphi[a] -> SetXTitle("Laboratory #phi");
    Hdmphi[a] -> SetYTitle("Counts");
    Hdmphi[a] -> SetMinimum(0.1);
    Hdmphi[a] -> Sumw2();
    if(a==1)Hdmphi[a] -> SetLineColor(2);

    sprintf(Hname, "Hpmuonphi_hodo%d", a);
    sprintf(Tname, " #mu^{+} Laboratory #phi, p_{T} > 0.5 GeV/c", onoff);
    Hpmuonphi[a] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
    Hpmuonphi[a] -> SetXTitle("Laboratory #phi");
    Hpmuonphi[a] -> SetYTitle("Counts");
    Hpmuonphi[a] -> Sumw2();
    if(a==1)Hpmuonphi[a] -> SetLineColor(2);
    
    sprintf(Hname, "Hmmuonphi_hodo%d", a);
    sprintf(Tname, " #mu^{-} Laboratory #phi, p_{T} > 0.5 GeV/c", onoff);
    Hmmuonphi[a] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
    Hmmuonphi[a] -> SetXTitle("Laboratory #phi");
    Hmmuonphi[a] -> SetYTitle("Counts");
    Hmmuonphi[a] -> Sumw2();
    if(a==1)Hmmuonphi[a] -> SetLineColor(2);
    
    sprintf(Hname, "Hcsphi_hodo%d", a);
    sprintf(Tname, "C-S #phi, hodoeff=%s,  p_{T} > 0.5 GeV/c", onoff);
    Hdmcsphi[a] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
    Hdmcsphi[a] -> SetXTitle("C-S #phi");
    Hdmcsphi[a] -> SetYTitle("Counts");
    Hdmcsphi[a] -> SetMinimum(0.1);
    Hdmcsphi[a] -> Sumw2();
    if(a==1)Hdmcsphi[a] -> SetLineColor(2);
    
    sprintf(Hname, "Hcstheta_hodo%d", a);
    sprintf(Tname, "C-S cos(#theta), hodoeff=%s,  p_{T} > 0.5 GeV/c", onoff);
    //Hdmcstheta[a] = new TH1D(Hname, Tname, 100, -1., 1.);
    Hdmcstheta[a] = new TH1D(Hname, Tname, 64, 0., 3.15);
    Hdmcstheta[a] -> SetXTitle("C-S cos(#theta)");
    Hdmcstheta[a] -> SetXTitle("C-S #theta");
    Hdmcstheta[a] -> SetYTitle("Counts");
    Hdmcstheta[a] -> SetMinimum(0.1);
    Hdmcstheta[a] -> Sumw2();
    if(a==1)Hdmcstheta[a] -> SetLineColor(2);

    for(int k = 0; k < nbins; k++){      
      ptTitleMin = 0.5 * (float)k + 0.5;
      ptTitleMax = 0.5 * (float)k + 1.0;
      if(k == 3)ptTitleMax = 5.0;

      sprintf(Hname, "Hphi_hodo%d_pt%d", a, k);
      sprintf(Tname, "Laboratory #phi, hodoeff=%s, %1.1f < p_{T} < %1.1f GeV/c", onoff, ptTitleMin, ptTitleMax);
      Hphi[a][k] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
      Hphi[a][k] -> SetXTitle("Laboratory #phi");
      Hphi[a][k] -> SetYTitle("Counts");
      Hphi[a][k] -> Sumw2();
      if(a==1)Hphi[a][k] -> SetLineColor(2);

      sprintf(Hname, "Hcsphi_hodo%d_pt%d", a, k);
      sprintf(Tname, "C-S #phi, hodoeff=%s, %1.1f < p_{T} < %1.1f GeV/c", onoff, ptTitleMin, ptTitleMax);
      Hcsphi[a][k] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
      Hcsphi[a][k] -> SetXTitle("C-S #phi");
      Hcsphi[a][k] -> SetYTitle("Counts");
      Hcsphi[a][k] -> Sumw2();
      if(a==1)Hcsphi[a][k] -> SetLineColor(2);

      sprintf(Hname, "Hcstheta_hodo%d_pt%d", a, k);
      sprintf(Tname, "C-S cos(#theta), hodoeff=%s, %1.1f < p_{T} < %1.1f GeV/c", onoff, ptTitleMin, ptTitleMax);
      Hcstheta[a][k] = new TH1D(Hname, Tname, 64, 0., 3.15);
      Hcstheta[a][k] -> SetXTitle("C-S cos(#theta)");
      Hcstheta[a][k] -> SetXTitle("C-S #theta");
      Hcstheta[a][k] -> SetYTitle("Counts");
      Hcstheta[a][k] -> Sumw2();
      if(a==1)Hcstheta[a][k] -> SetLineColor(2);

      sprintf(Hname, "Hmass_hodo%d_pt%d", a, k);
      sprintf(Tname, "Mass, hodoeff=%s, %1.1f < p_{T} < %1.1f GeV/c", onoff, ptTitleMin, ptTitleMax);
      Hmass[a][k] = new TH1D(Hname, Tname, 60, 4., 10.);
      Hmass[a][k] -> SetXTitle("mass [GeV/c^{2}]");
      Hmass[a][k] -> SetYTitle("Counts");
      Hmass[a][k] -> Sumw2();
      if(a==1)Hmass[a][k] -> SetLineColor(2);
      
      sprintf(Hname, "HxF_hodo%d_pt%d", a, k);
      sprintf(Tname, "x_{F}, hodoeff=%s, %1.1f < p_{T} < %1.1f GeV/c", onoff, ptTitleMin, ptTitleMax);
      HxF[a][k] = new TH1D(Hname, Tname, 200, -1., 1.);
      HxF[a][k] -> SetXTitle("x_{F}");
      HxF[a][k] -> SetYTitle("Counts");
      HxF[a][k] -> Sumw2();
      if(a==1)HxF[a][k] -> SetLineColor(2);
      
      sprintf(Hname, "Hdpz_hodo%d_pt%d", a, k);
      sprintf(Tname, "p_{z}, hodoeff=%s, %1.1f < p_{T} < %1.1f GeV/c", onoff, ptTitleMin, ptTitleMax);
      Hdpz[a][k] = new TH1D(Hname, Tname, 100, 20., 120.);
      Hdpz[a][k] -> SetXTitle("p_{z} [GeV/c]");
      Hdpz[a][k] -> SetYTitle("Counts");
      Hdpz[a][k] -> Sumw2();
      if(a==1)Hdpz[a][k] -> SetLineColor(2);
    }
  }

  for(int k = 0; k < nbins; k++){
    ptTitleMin = 0.5 * (float)k + 0.5;
    ptTitleMax = 0.5 * (float)k + 1.0;
    if(k == 3)ptTitleMax = 5.0;
    sprintf(Hname, "Hphi_rat_pt%d", k);
    sprintf(Tname, "Laboratory #phi, hodoeff on/off ratio, %1.1f < p_{T} < %1.1f GeV/c", ptTitleMin, ptTitleMax);
    Hphirat[k] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
    Hphirat[k] -> SetXTitle("Laboratory #phi");
    Hphirat[k] -> SetYTitle("Counts");
    Hphirat[k] -> Sumw2();

    sprintf(Hname, "Hcsphi_rat_pt%d", k);
    sprintf(Tname, "C-S #phi, hodoeff on/off ratio, %1.1f < p_{T} < %1.1f GeV/c", ptTitleMin, ptTitleMax);
    Hcsphirat[k] = new TH1D(Hname, Tname, 64, -3.15, 3.15);
    Hcsphirat[k] -> SetXTitle("C-S #phi");
    Hcsphirat[k] -> SetYTitle("Counts");
    Hcsphirat[k] -> Sumw2();

    sprintf(Hname, "Hcstheta_rat_pt%d", k);
    sprintf(Tname, "C-S cos(#theta), hodoeff on/off ratio, %1.1f < p_{T} < %1.1f GeV/c", ptTitleMin, ptTitleMax);
    Hcsthetarat[k] = new TH1D(Hname, Tname, 64, 0., 3.15);
    Hcsthetarat[k] -> SetXTitle("C-S cos(#theta)");
    Hcsthetarat[k] -> SetXTitle("C-S #theta");
    Hcsthetarat[k] -> SetYTitle("Counts");
    Hcsthetarat[k] -> Sumw2();
    
    sprintf(Hname, "Hmass_rat_pt%d", k);
    sprintf(Tname, "Mass, hodoeff on/off ratio, %1.1f < p_{T} < %1.1f GeV/c", ptTitleMin, ptTitleMax);
    Hmassrat[k] = new TH1D(Hname, Tname, 60, 4., 10.);
    Hmassrat[k] -> SetXTitle("mass [GeV/c^{2}]");
    Hmassrat[k] -> SetYTitle("Counts");
    Hmassrat[k] -> Sumw2();
	  
    sprintf(Hname, "HxF_rat_pt%d", k);
    sprintf(Tname, "x_{F}, hodoeff on/off ratio, %1.1f < p_{T} < %1.1f GeV/c", ptTitleMin, ptTitleMax);
    HxFrat[k] = new TH1D(Hname, Tname, 200, -1., 1.);
    HxFrat[k] -> SetXTitle("x_{F}");
    HxFrat[k] -> SetYTitle("Counts");
    HxFrat[k] -> Sumw2();
      
    sprintf(Hname, "Hdpz_rat_pt%d", k);
    sprintf(Tname, "p_{z}, hodoeff on/off ratio, %1.1f < p_{T} < %1.1f GeV/c", ptTitleMin, ptTitleMax);
    Hdpzrat[k] = new TH1D(Hname, Tname, 100, 20., 120.);
    Hdpzrat[k] -> SetXTitle("p_{z} [GeV/c]");
    Hdpzrat[k] -> SetYTitle("Counts");
    Hdpzrat[k] -> Sumw2();

    sprintf(Hname, "Hcsphiphi_pt%d", k);
    sprintf(Tname, "C-S #phi vs Laboratory #phi, %1.1f < p_{T} < %1.1f GeV/c", ptTitleMin, ptTitleMax);
    Hcsphiphi[k] = new TH2D(Hname, Tname, 64, -3.15, 3.15, 64, -3.15, 3.15);
    Hcsphiphi[k] -> SetXTitle("Laboratory #phi");
    Hcsphiphi[k] -> SetYTitle("C-S #phi");
    Hcsphiphi[k] -> Sumw2();
  }

  TH1D *Hmmuonphi0 = (TH1D*) inFile0 -> Get("Hmmuonphi");
  TH1D *Hmmuonphi1 = (TH1D*) inFile1 -> Get("Hmmuonphi");
  TH1D *Hpmuonphi0 = (TH1D*) inFile0 -> Get("Hpmuonphi");
  TH1D *Hpmuonphi1 = (TH1D*) inFile1 -> Get("Hpmuonphi");
  Hmmuonphi0 -> SetTitle("#mu^{-} Laboratory #phi, hodoeff=0");
  Hmmuonphi1 -> SetTitle("#mu^{-} Laboratory #phi, hodoeff=1");
  Hpmuonphi0 -> SetTitle("#mu^{+} Laboratory #phi, hodoeff=0");
  Hpmuonphi1 -> SetTitle("#mu^{+} Laboratory #phi, hodoeff=1");
  Hmmuonphi0 -> SetLineColor(2);
  Hmmuonphi1 -> SetLineColor(2);
  //Hpmuonphi0 -> SetLineColor(2);

  TH1D *Hmmmuonphirat = new TH1D("Hmmmuonphirat","#mu^{-} Laboratory #phi hodoeff on/off ratio", 64, -3.15, 3.15);
  TH1D *Hppmuonphirat = new TH1D("Hppmuonphirat","#mu^{+} Laboratory #phi hodoeff on/off ratio", 64, -3.15, 3.15);
  Hmmmuonphirat -> SetLineColor(2);
    
  Hmmuonphi0 -> SetXTitle("#phi");
  Hmmuonphi1 -> SetXTitle("#phi");
  Hmmuonphi0 -> SetYTitle("Counts");
  Hmmuonphi1 -> SetYTitle("Counts");
  Hpmuonphi0 -> SetXTitle("#phi");
  Hpmuonphi1 -> SetXTitle("#phi");
  Hpmuonphi0 -> SetYTitle("Counts");
  Hpmuonphi1 -> SetYTitle("Counts");
  Hmmmuonphirat -> SetXTitle("#phi");
  Hmmmuonphirat -> SetYTitle("Ratio");
  Hppmuonphirat -> SetXTitle("#phi");
  Hppmuonphirat -> SetYTitle("Ratio");
  
  //OK, let's get the TTree to get binned histos
  TTree *dataTree0 = (TTree*) inFile0 -> Get("dmHodos");
  TTree *dataTree1 = (TTree*) inFile1 -> Get("dmHodos");
  
  dataTree0 -> SetBranchAddress("sigWeight", &sigWeight);
  dataTree0 -> SetBranchAddress("dpx",       &dpx);
  dataTree0 -> SetBranchAddress("dpy",       &dpy);
  dataTree0 -> SetBranchAddress("dpz",       &dpz);
  dataTree0 -> SetBranchAddress("dpt",       &dpt);
  dataTree0 -> SetBranchAddress ("mass",       &mass);
  dataTree0 -> SetBranchAddress ("xF",         &xF);
  dataTree0 -> SetBranchAddress ("xB",         &xB);
  dataTree0 -> SetBranchAddress ("xT",         &xT);
  dataTree0 -> SetBranchAddress ("cs_costh",   &cs_costh);
  dataTree0 -> SetBranchAddress ("cs_phi",     &cs_phi);
  dataTree0 -> SetBranchAddress ("theta",      &theta);
  dataTree0 -> SetBranchAddress ("phi",        &phi);
  dataTree0 -> SetBranchAddress ("px1",        &px1);
  dataTree0 -> SetBranchAddress ("py1",        &py1);
  dataTree0 -> SetBranchAddress ("pz1",        &pz1);
  dataTree0 -> SetBranchAddress ("e1",         &e1);
  dataTree0 -> SetBranchAddress ("px2",        &px2);
  dataTree0 -> SetBranchAddress ("py2",        &py2);
  dataTree0 -> SetBranchAddress ("pz2",        &pz2);
  dataTree0 -> SetBranchAddress ("e2",         &e2);

  dataTree1 -> SetBranchAddress("sigWeight", &sigWeight);
  dataTree1 -> SetBranchAddress("dpx",       &dpx);
  dataTree1 -> SetBranchAddress("dpy",       &dpy);
  dataTree1 -> SetBranchAddress("dpz",       &dpz);
  dataTree1 -> SetBranchAddress("dpt",       &dpt);
  dataTree1 -> SetBranchAddress ("mass",       &mass);
  dataTree1 -> SetBranchAddress ("xF",         &xF);
  dataTree1 -> SetBranchAddress ("xB",         &xB);
  dataTree1 -> SetBranchAddress ("xT",         &xT);
  dataTree1 -> SetBranchAddress ("cs_costh",   &cs_costh);
  dataTree1 -> SetBranchAddress ("cs_phi",     &cs_phi);
  dataTree1 -> SetBranchAddress ("theta",      &theta);
  dataTree1 -> SetBranchAddress ("phi",        &phi);
  dataTree1 -> SetBranchAddress ("px1",        &px1);
  dataTree1 -> SetBranchAddress ("py1",        &py1);
  dataTree1 -> SetBranchAddress ("pz1",        &pz1);
  dataTree1 -> SetBranchAddress ("e1",         &e1);
  dataTree1 -> SetBranchAddress ("px2",        &px2);
  dataTree1 -> SetBranchAddress ("py2",        &py2);
  dataTree1 -> SetBranchAddress ("pz2",        &pz2);
  dataTree1 -> SetBranchAddress ("e2",         &e2);

  cout << "The number of Entries in dataTree0 is " << dataTree0 -> GetEntries() << endl;
  for(int i = 0; i < dataTree0 -> GetEntries(); i++){
  //for(int i = 0; i < 10; i++){
    dataTree0 -> GetEntry(i);

    if(i % 1000000 == 0) cout << "Entry " << i << endl;
    if(dpt < PTMIN) continue;
    if(dpt > PTMAX) continue;
    if(mass < MASS) continue;
    if(dpz < PZ) continue;

    track1.SetPxPyPzE(px1, py1, pz1, e1);
    track2.SetPxPyPzE(px2, py2, pz2, e2);
    //look at one quad at a time
    if(quad == 1){if(track1.Phi() < 0)continue;}
    if(quad == 2){if(track1.Phi() > 0)continue;}

    int kbin = dpt*2 - 1;
    if(kbin < 0) continue;
    if(kbin > 3) kbin = 3;


    
    //get C-S variables
    cs_costh = 2.0 * (e2 * pz1 - e1 * pz2) / (mass * TMath::Sqrt(mass*mass + dpt*dpt));
    cs_costh2 = 2.0 * (e1 * pz2 - e2 * pz1) / (mass * TMath::Sqrt(mass*mass + dpt*dpt));
    cs_th2 = TMath::ACos(cs_costh2);
    //cout << i << " " << cs_th2 << endl;
    //cs_phi2 = TMath::ATan(2.*TMath::Sqrt(mass*mass + dpt*dpt)/mass*(px1*py2 - px2*py1)/(px2*px2 - px1*px1 + py2*py2 -py1*py1));
    cs_phi = TMath::ATan2(2.*TMath::Sqrt(mass*mass + dpt*dpt)*(px2*py1 - px1*py2), mass*(px1*px1 - px2*px2 + py1*py1 -py2*py2));
    cs_phi2 = TMath::ATan2(2.*TMath::Sqrt(mass*mass + dpt*dpt)*(px1*py2 - px2*py1), mass*(px2*px2 - px1*px1 + py2*py2 -py1*py1));
    //cout << i << " " << cs_costh << "\t" << cs_costh2 << endl;
    //cout << i << " " << phi << "\t" << cs_phi << "\t" << cs_phi2 << "\t" << cs_phi2 - cs_phi << endl;
    
    Hphi[0][kbin] -> Fill(phi, sigWeight);
    Hmass[0][kbin] -> Fill(mass, sigWeight);
    HxF[0][kbin] -> Fill(xF, sigWeight);
    Hdpz[0][kbin] -> Fill(dpz, sigWeight);
    Hcsphi[0][kbin] -> Fill(cs_phi2, sigWeight);
    //Hcstheta[0][kbin] -> Fill(cs_costh2, sigWeight);
    Hcstheta[0][kbin] -> Fill(cs_th2, sigWeight);
    Hdmcsphi[0] -> Fill(cs_phi2, sigWeight);
    //Hdmcstheta[0] -> Fill(cs_costh2, sigWeight);
    Hdmcstheta[0] -> Fill(cs_th2, sigWeight);
    Hdmphi[0] -> Fill(phi, sigWeight);  
    Hpmuonphi[0] -> Fill(track1.Phi(), sigWeight);
    Hmmuonphi[0] -> Fill(track2.Phi(), sigWeight);
    Hallphi[0] -> Fill(track1.Phi(), track2.Phi(), phi, sigWeight);
    Hcsphiphi[kbin] -> Fill(phi, cs_phi2);
  }

  cout << "The number of Entries in dataTree1 is " << dataTree1 -> GetEntries() << endl;
  for(int i = 0; i < dataTree1 -> GetEntries(); i++){
    dataTree1 -> GetEntry(i);
    if(i % 1000000 == 0) cout << "Entry " << i << endl;

    if(dpt < PTMIN) continue;
    if(dpt > PTMAX) continue;
    if(mass < MASS) continue;
    if(dpz < PZ) continue;

    track1.SetPxPyPzE(px1, py1, pz1, e1);
    track2.SetPxPyPzE(px2, py2, pz2, e2);
    //look at one quad at a time
    if(quad == 1){if(track1.Phi() < 0)continue;}
    if(quad == 2){if(track1.Phi() > 0)continue;}
    int kbin = dpt*2 - 1;
    
    if(kbin < 0) continue;
    if(kbin > 3) kbin = 3;

    //get C-S variables
    //cs_costh2 = 2.0 * (e2 * pz1 - e1 * pz2) / (mass * TMath::Sqrt(mass*mass + dpt*dpt));
    cs_costh2 = 2.0 * (e1 * pz2 - e2 * pz1) / (mass * TMath::Sqrt(mass*mass + dpt*dpt));
    cs_th2 = TMath::ACos(cs_costh2);
    //cs_phi2 = TMath::ATan(2.*TMath::Sqrt(mass*mass + dpt*dpt)/mass*(px1*py2 - px2*py1)/(px2*px2 - px1*px1 + py2*py2 -py1*py1));
    cs_phi2 = TMath::ATan2(2.*TMath::Sqrt(mass*mass + dpt*dpt)*(px1*py2 - px2*py1),mass*(px2*px2 - px1*px1 + py2*py2 -py1*py1));
    //cout << i << " " << cs_costh << "\t" << cs_costh2 << endl;
    //cout << i << " " << cs_phi << "\t" << cs_phi2 << "\t" << cs_phi2 - cs_phi << endl;
    
    Hphi[1][kbin] -> Fill(phi, sigWeight);
    Hmass[1][kbin] -> Fill(mass, sigWeight);
    HxF[1][kbin] -> Fill(xF, sigWeight);
    Hdpz[1][kbin] -> Fill(dpz, sigWeight);
    Hcsphi[1][kbin] -> Fill(cs_phi2, sigWeight);
    //Hcstheta[1][kbin] -> Fill(cs_costh2, sigWeight);
    Hcstheta[1][kbin] -> Fill(cs_th2, sigWeight);
    Hdmcsphi[1] -> Fill(cs_phi2, sigWeight);
    //Hdmcstheta[1] -> Fill(cs_costh2, sigWeight);
    Hdmcstheta[1] -> Fill(cs_th2, sigWeight);
    Hdmphi[1] -> Fill(phi, sigWeight);
    Hpmuonphi[1] -> Fill(track1.Phi(), sigWeight);
    Hmmuonphi[1] -> Fill(track2.Phi(), sigWeight);
    Hallphi[1] -> Fill(track1.Phi(), track2.Phi(), phi, sigWeight);
  }

  //rebin if needed
  for(int a = 0; a < 2; a++){
    rebinHisto(Hdmphi[a], dmrebin);
    rebinHisto(Hpmuonphi[a], dmrebin);
    rebinHisto(Hmmuonphi[a], dmrebin);
    rebinHisto(Hdmcsphi[a], dmrebin);
    rebinHisto(Hdmcstheta[a], dmrebin);
    for(int k = 0; k < nbins; k++){
      rebinHisto(Hphi[a][k], dmrebin);
      rebinHisto(Hcsphi[a][k], dmrebin);
      rebinHisto(Hcstheta[a][k], dmrebin);
      rebinHisto(Hmass[a][k], dmrebin);
      rebinHisto(Hdpz[a][k], dmrebin);
      rebinHisto(HxF[a][k], dmrebin);
    }
  }
  rebinHisto(Hdmphirat, dmrebin);
  rebinHisto(Hpmuonphirat, dmrebin);
  rebinHisto(Hmmuonphirat, dmrebin);
  rebinHisto(Hdmcsphirat, dmrebin);
  rebinHisto(Hdmcsthetarat, dmrebin);
  for(int k = 0; k < nbins; k++){
    rebinHisto(Hphirat[k], dmrebin);
    Hphirat[k] -> Divide(Hphi[1][k], Hphi[0][k], 1, 1);
    Hphirat[k] -> SetMinimum(0.7);
    Hphirat[k] -> SetMaximum(1.01);
    rebinHisto(Hcsphirat[k], dmrebin);
    Hcsphirat[k] -> Divide(Hcsphi[1][k], Hcsphi[0][k], 1, 1);
    Hcsphirat[k] -> SetMinimum(0.7);
    Hcsphirat[k] -> SetMaximum(1.01);
    rebinHisto(Hcsthetarat[k], dmrebin);
    Hcsthetarat[k] -> Divide(Hcstheta[1][k], Hcstheta[0][k], 1, 1);
    Hcsthetarat[k] -> SetMinimum(0.7);
    Hcsthetarat[k] -> SetMaximum(1.01);
    rebinHisto(Hmassrat[k], dmrebin);
    Hmassrat[k] -> Divide(Hmass[1][k], Hmass[0][k], 1, 1);
    Hmassrat[k] -> SetMinimum(0.7);
    Hmassrat[k] -> SetMaximum(1.01);
    rebinHisto(HxFrat[k], dmrebin);
    HxFrat[k] -> Divide(HxF[1][k], HxF[0][k], 1, 1);
    HxFrat[k] -> SetMinimum(0.7);
    HxFrat[k] -> SetMaximum(1.01);
    rebinHisto(Hdpzrat[k], dmrebin);
    Hdpzrat[k] -> Divide(Hdpz[1][k], Hdpz[0][k], 1, 1);
    Hdpzrat[k] -> SetMinimum(0.7);
    Hdpzrat[k] -> SetMaximum(1.01);
  }

  Hdmphirat -> Divide(Hdmphi[1], Hdmphi[0], 1, 1);
  Hdmphirat -> SetMinimum(0.7);
  Hdmphirat -> SetMaximum(1.01);
  Hpmuonphirat -> Divide(Hpmuonphi[1], Hpmuonphi[0], 1, 1);
  Hpmuonphirat -> SetMinimum(0.7);
  Hpmuonphirat -> SetMaximum(1.01);
  Hmmuonphirat -> Divide(Hmmuonphi[1], Hmmuonphi[0], 1, 1);
  Hmmuonphirat -> SetMinimum(0.7);
  Hmmuonphirat -> SetMaximum(1.01);
  Hdmcsphirat -> Divide(Hdmcsphi[1], Hdmcsphi[0], 1, 1);
  Hdmcsphirat -> SetMinimum(0.7);
  Hdmcsphirat -> SetMaximum(1.01);
  cout<<"balh"<<endl;
  Hdmcsthetarat -> Divide(Hdmcstheta[1], Hdmcstheta[0], 1, 1);
  Hdmcsthetarat -> SetMinimum(0.7);
  Hdmcsthetarat -> SetMaximum(1.01);
  
  Hmmmuonphirat -> Divide(Hmmuonphi1, Hmmuonphi0, 1, 1);
  Hppmuonphirat -> Divide(Hpmuonphi1, Hpmuonphi0, 1, 1);
  Hmmmuonphirat -> SetMinimum(0.7);
  Hmmmuonphirat -> SetMaximum(1.01);
  Hppmuonphirat -> SetMinimum(0.7);
  Hppmuonphirat -> SetMaximum(1.01);

  TLine *l0 = new TLine(-3.15, 1., 3.15, 1.);
  l0 -> SetLineStyle(2);
 
  TCanvas *c3 = new TCanvas("c3", "c3", 1800, 400);
  c3 -> Divide(3, 1);
  c3 -> cd(1);
  Hdmcsphi[0] -> Draw();
  c3 -> cd(2);
  Hdmcsphi[1] -> Draw();
  c3 -> cd(3);
  Hdmcsphirat -> Draw();
  Hdmcsphirat -> Fit("pol0");
  Hdmcsphirat -> Fit(cos2x);
  l0 -> Draw();
  sprintf(Hname, "./images/0csphi_quad%d_rebin%d.gif", quad, dmrebin);
  c3 -> SaveAs(Hname);
  c3 -> cd(1);
  Hdmphi[0] -> Draw();
  c3 -> cd(2);
  Hdmphi[1] -> Draw();
  c3 -> cd(3);
  Hdmphirat -> Draw();
  Hdmphirat -> Fit("pol0");
  Hdmphirat -> Fit(cos2x);
  l0 -> Draw();
  sprintf(Hname, "./images/0phi_quad%d_rebin%d.gif", quad, dmrebin);
  c3 -> SaveAs(Hname);
  c3 -> cd(1);
  Hdmcstheta[0] -> Draw();
  c3 -> cd(2);
  Hdmcstheta[1] -> Draw();
  c3 -> cd(3);
  Hdmcsthetarat -> Draw();
  Hdmcsthetarat -> Fit("pol2");
  //Hdmcsthetarat -> Fit(sinxsinx);
  l0 -> Draw();
  sprintf(Hname, "./images/0cstheta_quad%d_rebin%d.gif", quad, dmrebin);
  c3 -> SaveAs(Hname);
  
  TCanvas *c6b = new TCanvas("c6b", "c6b", 1800, 800);
  c6b -> Divide(3, 2);
  c6b -> cd(1);
  Hmmuonphi0 -> Draw();
  c6b -> cd(2);
  Hmmuonphi1 -> Draw();
  c6b -> cd(3);
  Hmmmuonphirat -> Draw();
  //Hmmmuonphirat -> Fit("pol0");
  //Hmmmuonphirat -> Fit(cos2x);
  l0 -> Draw();
  
  c6b -> cd(4);
  Hpmuonphi0 -> Draw();
  c6b -> cd(5);
  Hpmuonphi1 -> Draw();
  c6b -> cd(6);
  Hppmuonphirat -> Draw();
  //Hppmuonphirat -> Fit("pol0");
  //Hmmmuonphirat -> Fit(cos2x);
  l0 -> Draw();
  sprintf(Hname, "./images/0tracklike_quad%d.gif", quad);
  c6b -> SaveAs(Hname);

  //Draw Plots
  TCanvas *c1 = new TCanvas("c1","c1",1200,400);
  c1 -> Divide(2,1);
  c1 -> cd(1);
  Hmmuonphirat -> Draw();
  Hpmuonphirat -> Draw("same");
  c1 -> cd(2);
  Hdmphirat -> Draw();
  sprintf(Hname, "./images/0tracks_quad%d.gif", quad);
  c1 -> SaveAs(Hname);

  TCanvas *cphi = new TCanvas("cphi", "cphi", 1200, 800);
  cphi -> Divide(2, 2);
  
  //draw binned Histos
  for(int c = 0; c < 6; c++){
    sprintf(Fname, "c4%d", c);
    c4[c] = new TCanvas(Fname, Fname, 2000, 1500);
    c4[c] -> Divide(4, 3);
  }

  for(int k = 0; k < nbins; k++){
    cphi -> cd(k+1);
    Hcsphiphi[k] -> Draw("colz");
    
    c4[0] -> cd(k+1); Hphi[1][k] -> Draw();
    c4[0] -> cd(k+5); Hphi[0][k] -> Draw();
    c4[0] -> cd(k+9); Hphirat[k] -> Draw();
    Hphirat[k] -> Fit(cos2x);
    l0 -> Draw();

    c4[1] -> cd(k+1); Hcsphi[1][k] -> Draw();
    c4[1] -> cd(k+5); Hcsphi[0][k] -> Draw();
    c4[1] -> cd(k+9); Hcsphirat[k] -> Draw();
    Hcsphirat[k] -> Fit(cos2x);
    l0 -> Draw();

    c4[2] -> cd(k+1); Hcstheta[1][k] -> Draw();
    c4[2] -> cd(k+5); Hcstheta[0][k] -> Draw();
    c4[2] -> cd(k+9); Hcsthetarat[k] -> Draw();
    Hcsthetarat[k] -> Fit("pol2");
    //Hcsthetarat[k] -> Fit(sinxsinx);
    l0 -> Draw();
 
    c4[3] -> cd(k+1); Hmass[1][k] -> Draw();
    c4[3] -> cd(k+5); Hmass[0][k] -> Draw();
    c4[3] -> cd(k+9); Hmassrat[k] -> Draw();
    //Hmassrat[k] -> Fit(cos2x);
    l0 -> Draw();

    c4[4] -> cd(k+1); HxF[1][k] -> Draw();
    c4[4] -> cd(k+5); HxF[0][k] -> Draw();
    c4[4] -> cd(k+9); HxFrat[k] -> Draw();
    //HxFrat[k] -> Fit(cos2x);
    l0 -> Draw();
    
    c4[5] -> cd(k+1); Hdpz[1][k] -> Draw();
    c4[5] -> cd(k+5); Hdpz[0][k] -> Draw();
    c4[5] -> cd(k+9); Hdpzrat[k] -> Draw();
    //Hdpzrat[k] -> Fit(cos2x);
    l0 -> Draw();
  }
  
  sprintf(Hname, "./images/labphi_quad%d_rebin%d_nbins%d.gif", quad, dmrebin, nbins);
  c4[0] -> cd();
  c4[0] -> SaveAs(Hname);
  sprintf(Hname, "./images/csphi_quad%d_rebin%d_nbins%d.gif", quad, dmrebin, nbins);
  c4[1] -> cd();
  c4[1] -> SaveAs(Hname);
  sprintf(Hname, "./images/cstheta_quad%d_rebin%d_nbins%d.gif", quad, dmrebin, nbins);
  c4[2] -> cd();
  c4[2] -> SaveAs(Hname);
  sprintf(Hname, "./images/mass_quad%d_rebin%d_nbins%d.gif", quad, dmrebin, nbins);
  c4[3] -> cd();
  c4[3] -> SaveAs(Hname);
  sprintf(Hname, "./images/xF_quad%d_rebin%d_nbins%d.gif", quad, dmrebin, nbins);
  c4[4] -> cd();
  c4[4] -> SaveAs(Hname);
  sprintf(Hname, "./images/dpz_quad%d_rebin%d_nbins%d.gif", quad, dmrebin, nbins);
  c4[5] -> cd();
  c4[5] -> SaveAs(Hname);
}

void rebinHisto(TH1D *histo, int rebin)
{
  histo -> Rebin(rebin);
}
