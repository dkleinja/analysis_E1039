void getphi_sim(const int ktrackon = 0, const int tarvar = 0, const int magflip = 0)
{

  gStyle -> SetOptFit(1);
  const Float_t PI = 3.14159;

  char Fname[128];
  char Hname[128];
  char Tname[128];
  char tardump[5];
  int mvar = 13;
  int svar = 1;
  
  if(tarvar == 0){
    sprintf(tardump, "dump");
  }
  else if(tarvar == 1){
    sprintf(tardump, "LD2");
  }
  else if(tarvar == 2){
    sprintf(tardump, "C");
  }
  if(magflip == 1){
    mvar = 14;
    svar = 19;
  }

  sprintf(Tname, "#phi of dimuon, %s origin, magflip %d", tardump, magflip);
  TH1D *Hdmphi = new TH1D("Hdmphi", Tname, 64, -3.15, 3.15);
  Hdmphi -> SetMinimum(0.1);
  sprintf(Tname, "#phi of #mu^{-}, %s origin, magflip %d", tardump, magflip);
  TH1D *Hmmuonphi = new TH1D("Hmmuonphi", Tname, 64, -3.15, 3.15);
  Hmmuonphi -> SetMinimum(0.1);
  sprintf(Tname, "#phi of #mu^{+}, %s origin, magflip %d", tardump, magflip);
  TH1D *Hpmuonphi = new TH1D("Hpmuonphi", Tname, 64, -3.15, 3.15);
  Hpmuonphi -> SetMinimum(0.1);
  sprintf(Tname, "#phi of dimuon, %s origin, magflip %d", tardump, magflip);
  TH1D *Hdmeta = new TH1D("Hdmeta", Tname, 60, -0., 6.);
  Hdmeta -> SetMinimum(0.1);
  sprintf(Tname, "#eta of #mu^{-}, %s origin, magflip %d", tardump, magflip);
  TH1D *Hmmuoneta = new TH1D("Hmmuoneta", Tname, 60, -0., 6.);
  Hmmuoneta -> SetMinimum(0.1);
  sprintf(Tname, "#eta of #mu^{+}, %s origin, magflip %d", tardump, magflip);
  TH1D *Hpmuoneta = new TH1D("Hpmuoneta", Tname, 60, -0., 6.);
  Hpmuoneta -> SetMinimum(0.1);

  Hdmphi -> SetXTitle("#phi");
  Hdmphi -> SetYTitle("Counts");
  Hmmuonphi -> SetXTitle("#phi");
  Hmmuonphi -> SetYTitle("Counts");
  Hpmuonphi -> SetXTitle("#phi");
  Hpmuonphi -> SetYTitle("Counts");

  Hdmeta -> SetXTitle("#eta");
  Hdmeta -> SetYTitle("Counts");
  Hmmuoneta -> SetXTitle("#eta");
  Hmmuoneta -> SetYTitle("Counts");
  Hpmuoneta -> SetXTitle("#eta");
  Hpmuoneta -> SetYTitle("Counts");

  Hdmphi -> Sumw2();
  Hmmuonphi -> Sumw2();
  Hpmuonphi -> Sumw2();
  Hdmeta -> Sumw2();
  Hmmuoneta -> Sumw2();
  Hpmuoneta -> Sumw2();
  
  sprintf(Tname, "charge of ktracks, %s origin, magflip %d", tardump, magflip);
  TH1D *Hcharge = new TH1D("Hcharge", Tname, 4, -2, 2);
  Hcharge -> SetXTitle("Charge");
  Hcharge -> SetYTitle("Counts");

  //sprintf(Fname, "./nDST/Analysis_mc_drellyan_%s_M0%d_S0%02d.root", tardump, mvar, svar);
  //printf(Fname, "./nDST/Analysis_mc_drellyan_%s_M0%d_S0%02d.root ", tardump, mvar, svar);

  sprintf(Fname, "./nDST/test_dkleinja_mc_drellyan_%s_M0%d_S0%02d.root", tardump, mvar, svar);
  printf(Fname, "./nDST/test_dkleinja_mc_drellyan_%s_M0%d_S0%02d.root ", tardump, mvar, svar);
  
  TFile *inFile = new TFile(Fname);
  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");

  sprintf(Fname, "./nDST/Analysis_mc_drellyan_%s_M0%d_S0%02d_ktrack.root", tardump, mvar, svar);
  TFile *inFile1 = new TFile(Fname);
  TTree *tracktree = (TTree*) inFile1 -> Get("ktrack");

  sprintf(Fname, "./nDST/Analysis_mc_drellyan_%s_M0%d_S0%02d_spill.root", tardump, mvar, svar);
  TFile *inFile2 = new TFile(Fname);
  TTree *spilltree = (TTree*) inFile2 -> Get("spill");

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
  Float_t sigWeight;
  Int_t target, dump;
  Int_t targetPos;
  Float_t m3hm, m3hs, m3vm, m3vs;
  Float_t m2hm, m2hs, m2vm, m2vs;
  Float_t dhm, dvm;
  Float_t costh, phi;
  Float_t eta;
  
  dmtree -> SetBranchAddress ("dimuonID",        &dimuonID);
  dmtree -> SetBranchAddress ("runID",           &runID);
  dmtree -> SetBranchAddress ("eventID",         &eventID);
  dmtree -> SetBranchAddress ("spillID",         &spillID);
  dmtree -> SetBranchAddress ("posTrackID",    &posTrackID);
  dmtree -> SetBranchAddress ("negTrackID",    &negTrackID);
  dmtree -> SetBranchAddress ("posHits",    &posHits);
  dmtree -> SetBranchAddress ("negHits",    &negHits);
  dmtree -> SetBranchAddress ("dx",              &dx);
  dmtree -> SetBranchAddress ("dy",              &dy);
  dmtree -> SetBranchAddress ("dz",              &dz);
  dmtree -> SetBranchAddress ("dpx",             &dpx);
  dmtree -> SetBranchAddress ("dpy",             &dpy);
  dmtree -> SetBranchAddress ("dpz",             &dpz);
  dmtree -> SetBranchAddress ("mass",            &mass);
  dmtree -> SetBranchAddress ("xF",              &xF);
  dmtree -> SetBranchAddress ("xB",              &xB);
  dmtree -> SetBranchAddress ("costh",              &costh);
  dmtree -> SetBranchAddress ("phi",              &phi);
  dmtree -> SetBranchAddress ("trackSeparation", &trackSeparation);
  dmtree -> SetBranchAddress ("chisq_dimuon",    &chisq_dimuon);
  dmtree -> SetBranchAddress ("px1",             &px1);
  dmtree -> SetBranchAddress ("py1",             &py1);
  dmtree -> SetBranchAddress ("pz1",             &pz1);
  dmtree -> SetBranchAddress ("px2",             &px2);
  dmtree -> SetBranchAddress ("py2",             &py2);
  dmtree -> SetBranchAddress ("pz2",             &pz2);
  dmtree -> SetBranchAddress ("sigWeight",             &sigWeight);
  dmtree -> SetBranchAddress ("target",          &target);
  dmtree -> SetBranchAddress ("dump",            &dump);
  dmtree -> SetBranchAddress ("targetPos",       &targetPos);

  //initialize ktrack tree
  Int_t trackID, runID, spillID, eventID, charge, roadID;
  Int_t numHits, numHitsSt1, numHitsSt2, numHitsSt3, numHitsSt4H, numHitsSt4V;
  Float_t chisq;
  Float_t x0, y0, z0;
  Float_t x_target, y_target, z_target;
  Float_t x_dump, y_dump, z_dump;
  Float_t px0, py0, pz0;
  Float_t x1, y1, z1;
  Float_t px1, py1, pz1;
  Float_t x3, y3, z3;
  Float_t px3, py3, pz3;
  Float_t tx_PT, ty_PT;
  //UInt_t target, dump, targetPos;
  tracktree -> SetBranchAddress ("trackID",   &trackID);
  tracktree -> SetBranchAddress ("runID",     &runID);
  tracktree -> SetBranchAddress ("spillID",   &spillID);
  tracktree -> SetBranchAddress ("eventID",   &eventID);
    tracktree -> SetBranchAddress ("numHits",   &numHits);
  tracktree -> SetBranchAddress ("charge",    &charge);
  tracktree -> SetBranchAddress ("roadID",    &roadID);
  tracktree -> SetBranchAddress ("chisq",     &chisq);
  tracktree -> SetBranchAddress ("x0",        &x0);
  tracktree -> SetBranchAddress ("y0",        &y0);
  tracktree -> SetBranchAddress ("z0",        &z0);
  tracktree -> SetBranchAddress ("x_target",  &x_target);
  tracktree -> SetBranchAddress ("y_target",  &y_target);
  tracktree -> SetBranchAddress ("z_target",  &z_target);
  tracktree -> SetBranchAddress ("x_dump",    &x_dump);
  tracktree -> SetBranchAddress ("y_dump",    &y_dump);
  tracktree -> SetBranchAddress ("z_dump",    &z_dump);
  tracktree -> SetBranchAddress ("x1",        &x1);
  tracktree -> SetBranchAddress ("y1",        &y1);
  tracktree -> SetBranchAddress ("z1",        &z1);
  tracktree -> SetBranchAddress ("px1",       &px1);
  tracktree -> SetBranchAddress ("py1",       &py1);
  tracktree -> SetBranchAddress ("pz1",       &pz1);
  tracktree -> SetBranchAddress ("x3",        &x3);
  tracktree -> SetBranchAddress ("y3",        &y3);
  tracktree -> SetBranchAddress ("z3",        &z3);
  tracktree -> SetBranchAddress ("px3",       &px3);
  tracktree -> SetBranchAddress ("py3",       &py3);
  tracktree -> SetBranchAddress ("pz3",       &pz3);
  tracktree -> SetBranchAddress ("tx_PT",     &tx_PT);
  tracktree -> SetBranchAddress ("ty_PT",     &ty_PT);
  tracktree -> SetBranchAddress ("targetPos", &targetPos);
  tracktree -> SetBranchAddress ("target",    &target);
  tracktree -> SetBranchAddress ("dump",    &dump);

  //initialize spill tree
  Float_t liveProton;
  spilltree -> SetBranchAddress ("runID",           &runID);
  spilltree -> SetBranchAddress ("spillID",         &spillID);
  spilltree -> SetBranchAddress ("liveProton",         &liveProton);

  Float_t lum = 0.;

  //ok, lets get luminosity
  int nentries2 = spilltree -> GetEntries();
  cout << "The number of Spill Entries is " << nentries2 << endl;
  for(int i = 0; i < nentries2; i++){
    spilltree -> GetEntry(i);
    
      lum += liveProton;
  } 

  sprintf(Tname, "The number of live Proton in %s origin, magflip %d is %1.3f \n", tardump, magflip, lum);
  printf(Tname);
  //get phi distributions dimuon
  TLorentzVector *lordm = new TLorentzVector;
  TLorentzVector *track1 = new TLorentzVector;
  TLorentzVector *track2 = new TLorentzVector;
  
  TVector2 *vec2 = new TVector2;
  TVector3 *vec3 = new TVector3;
  int nentries = dmtree -> GetEntries();
  cout << "The number of dimuon Entries is " << nentries << endl;
  int zbin;
  for(int i = 0; i < nentries; i++){
    if(i%100000 == 0) cout << i << endl;
    dmtree -> GetEntry(i);
    
    if(target == 0 && dump == 0) continue;
    if(tarvar == 0){
      if(target == 1) continue;
    }
    if(tarvar == 1){
      if(dump == 1) continue;
    }
    //if tarvar == 2, it includes both target and dump
    if(tarvar != 0 && tarvar != 1 && tarvar != 2){
      cout << "Set tarvar variable to 0,1, or 2! " << endl;
      exit(1);
    }
    //cout << target << " " << dump << endl;
    //if(dz > 50)continue;//just get event from early in dump
    if(pz1 < 27 || pz2 < 27) continue;
    if(posHits < 17 || negHits < 17) continue;

    track1 -> SetXYZM(px1, py1, pz1, 0.105);
    track2 -> SetXYZM(px2, py2, pz2, 0.105);
    vec3 -> SetXYZ(px1, py1, pz1);
    if(vec3 -> Pt() < 1.)continue;
    vec3 -> SetXYZ(px2, py2, pz2);
    if(vec3 -> Pt() < 1.)continue;

    lordm -> SetXYZM(dpx, dpy, dpz, mass);
    //cout << i << " " << lordm -> Pt() << endl;  
    //cout << i << dpx << " " << dpy << " " << " " << vec2 -> Phi() << " " << vec3 -> Phi() << " " << lordm -> Phi() << endl;
    if(lordm -> Pt() < 1.)continue;

    phi = lordm -> Phi();
    Hdmphi -> Fill(phi, sigWeight);
    phi = track1 -> Phi();
    Hpmuonphi -> Fill(phi, sigWeight);
    phi = track2 -> Phi();
    Hmmuonphi -> Fill(phi, sigWeight);
    eta = lordm -> Eta();
    Hdmeta -> Fill(eta, sigWeight);
    eta = track1 -> Eta();
    Hpmuoneta -> Fill(eta, sigWeight);
    eta = track2 -> Eta();
    Hmmuoneta -> Fill(eta, sigWeight);
  } 

  //get phi distributions ktrack
  if(ktrackon == 1){
    int nentries = tracktree -> GetEntries();
    cout << "The number of track Entries is " << nentries << endl;
    int zbin;
    for(int i = 0; i < nentries; i++){
      if(i%1000000 == 0) cout << i << endl;
      tracktree -> GetEntry(i);

      if(target == 0 && dump == 0) continue;
      if(tarvar == 0){
	if(target == 1) continue;
      }
      if(tarvar == 1){
	if(dump == 1) continue;
      }      
      //if(target == 1 || dump == 0) continue;
      if(numHits < 18) continue;
      if(pz1 < 27 || pz3 < 27) continue;
      if(chisq > 5) continue;
      vec2 -> Set(px1, py1);
      vec3 -> SetXYZ(px1, py1, pz1);
      //if(vec3 -> Pt() < 1.)continue;
      //phi = vec2 -> Phi();
      phi = vec3 -> Phi();

      //in roadset62 careful with charge variable when magnet flipped
      if(charge == -1){
	Hmmuonphi -> Fill(phi, sigWeight);
	Hcharge -> Fill(charge);
      } 
      else if(charge == 1){
	Hpmuonphi -> Fill(phi, sigWeight);
	Hcharge -> Fill(charge);
      }
    } 
  }

  TCanvas *c3 = new TCanvas("c3", "c3", 1800, 400);
  c3 -> Divide(3, 1);
  c3 -> cd(1);
  Hdmphi -> Draw();
  c3 -> cd(2);
  Hmmuonphi -> Draw();
  c3 -> cd(3);
  Hpmuonphi -> Draw();
  sprintf(Hname, "./images/phidists_sims_%s_magflip%d.gif", tardump, magflip);
  c3 -> SaveAs(Hname);

  TCanvas *c3a = new TCanvas("c3a", "c3a", 1800, 400);
  c3a -> Divide(3, 1);
  c3a -> cd(1);
  Hdmeta -> Draw();
  c3a -> cd(2);
  Hmmuoneta -> Draw();
  c3a -> cd(3);
  Hpmuoneta -> Draw();
  sprintf(Hname, "./images/etadists_sims_%s_magflip%d.gif", tardump, magflip);
  //c3a -> SaveAs(Hname);
  
  sprintf(Fname, "./root_files/phidists_sims_%s_magflip%d.root", tardump, magflip);
  TFile *outfile = new TFile(Fname, "RECREATE");
  outfile -> cd();
  Hdmphi -> Write();
  Hmmuonphi -> Write();
  Hpmuonphi -> Write();
  Hcharge -> Write();
  outfile -> Write();
}
