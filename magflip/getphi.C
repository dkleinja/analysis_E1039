void getphi(const int ktrackon = 0, const int roadset = 61, const int prod = 6)
{

  gStyle -> SetOptFit(1);
  const Float_t PI = 3.14159;

  char Fname[128];
  char Hname[128];
  char Tname[128];

  sprintf(Tname, "#phi of dimuon, roadset%d_R00%d", roadset, prod);
  TH1D *Hdmphi = new TH1D("Hdmphi", Tname, 64, -3.15, 3.15);
  sprintf(Tname, "CS #phi of dimuon, roadset%d_R00%d", roadset, prod);
  TH1D *Hdmcsphi = new TH1D("Hcsphi", Tname, 64, -3.15, 3.15);
  sprintf(Tname, "#phi of #mu^{-}, roadset%d_R00%d", roadset, prod);
  TH1D *Hmmuonphi = new TH1D("Hmmuonphi", Tname, 64, -3.15, 3.15);
  sprintf(Tname, "#phi of #mu^{+}, roadset%d_R00%d", roadset, prod);
  TH1D *Hpmuonphi = new TH1D("Hpmuonphi", Tname, 64, -3.15, 3.15);
  Hdmphi -> SetXTitle("#phi");
  Hdmphi -> SetYTitle("Counts");
  Hmmuonphi -> SetXTitle("#phi");
  Hmmuonphi -> SetYTitle("Counts");
  Hpmuonphi -> SetXTitle("#phi");
  Hpmuonphi -> SetYTitle("Counts");

  Hdmphi -> Sumw2();
  Hdmcsphi -> Sumw2();
  Hmmuonphi -> Sumw2();
  Hpmuonphi -> Sumw2();

  sprintf(Tname, "charge of ktracks, roadset %d", roadset);
  TH1D *Hcharge = new TH1D("Hcharge", Tname, 4, -2, 2);
  Hcharge -> SetXTitle("Charge");
  Hcharge -> SetYTitle("Counts");

  sprintf(Fname, "./nDST/Analysis_roadset%d_R00%d_V001.root", roadset, prod);
  TFile *inFile = new TFile(Fname);
  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");

  sprintf(Fname, "./nDST/Analysis_roadset%d_R005_V001_ktrack.root", roadset);
  TFile *inFile1 = new TFile(Fname);
  TTree *tracktree = (TTree*) inFile1 -> Get("ktrack");

  sprintf(Fname, "./nDST/cuts1489v1/Analysis_roadset%d_R005_V001_spill.root", roadset);
  TFile *inFile2 = new TFile(Fname);
  TTree *spilltree = (TTree*) inFile2 -> Get("spill");

  //initialize kdimuon tree
  Int_t dimuonID;
  Int_t runID;
  Int_t spillID, eventID;
  Int_t negTrackID, posTrackID;
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
  Float_t phi;
  Float_t csphi;

  dmtree -> SetBranchAddress ("dimuonID",        &dimuonID);
  dmtree -> SetBranchAddress ("runID",           &runID);
  dmtree -> SetBranchAddress ("eventID",         &eventID);
  dmtree -> SetBranchAddress ("spillID",         &spillID);
  dmtree -> SetBranchAddress ("posTrackID",    &posTrackID);
  dmtree -> SetBranchAddress ("negTrackID",    &negTrackID);
  dmtree -> SetBranchAddress ("dx",              &dx);
  dmtree -> SetBranchAddress ("dy",              &dy);
  dmtree -> SetBranchAddress ("dz",              &dz);
  dmtree -> SetBranchAddress ("dpx",             &dpx);
  dmtree -> SetBranchAddress ("dpy",             &dpy);
  dmtree -> SetBranchAddress ("dpz",             &dpz);
  dmtree -> SetBranchAddress ("mass",            &mass);
  dmtree -> SetBranchAddress ("xF",              &xF);
  dmtree -> SetBranchAddress ("xB",              &xB);
  dmtree -> SetBranchAddress ("xT",              &xT);
  dmtree -> SetBranchAddress ("trackSeparation", &trackSeparation);
  dmtree -> SetBranchAddress ("chisq_dimuon",    &chisq_dimuon);
  dmtree -> SetBranchAddress ("px1",             &px1);
  dmtree -> SetBranchAddress ("py1",             &py1);
  dmtree -> SetBranchAddress ("pz1",             &pz1);
  dmtree -> SetBranchAddress ("px2",             &px2);
  dmtree -> SetBranchAddress ("py2",             &py2);
  dmtree -> SetBranchAddress ("pz2",             &pz2);
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

  sprintf(Tname, "The number of live Proton in roadset %d is %1.3f \n", roadset, lum);
  printf(Tname);
  //get phi distributions dimuon
  TLorentzVector *lordm = new TLorentzVector;
  TLorentzVector *track1 = new TLorentzVector;
  TLorentzVector *track2 = new TLorentzVector;
  int nentries = dmtree -> GetEntries();
  cout << "The number of dimuon Entries is " << nentries << endl;
  int zbin;
  for(int i = 0; i < nentries; i++){
    if(i%100000 == 0) cout << i << endl;
    dmtree -> GetEntry(i);

    if(target == 0 || dump == 1) continue;
    if(targetPos != 3) continue;
    lordm -> SetXYZM(dpx, dpy, dpz, mass);
    track1 -> SetXYZM(px1, py1, pz1, 0.105658);
    track2 -> SetXYZM(px2, py2, pz2, 0.105658);
    dpt = lordm -> Pt();
    csphi = TMath::ATan2(2.*TMath::Sqrt(mass*mass + dpt*dpt)*(px1*py2 - px2*py1), mass*(px2*px2 - px1*px1 + py2*py2 -py1*py1));
    if(prod==6)csphi = TMath::ATan2(2.*TMath::Sqrt(mass*mass + dpt*dpt)*(px2*py1 - px1*py2), mass*(px1*px1 - px2*px2 + py1*py1 -py2*py2));
    phi = lordm -> Phi();
    Hdmphi -> Fill(phi);
    Hdmcsphi -> Fill(csphi);
  } 

  //get phi distributions ktrack
  if(ktrackon == 1){
    int nentries = tracktree -> GetEntries();
    cout << "The number of track Entries is " << nentries << endl;
    int zbin;
    for(int i = 0; i < nentries; i++){
      if(i%1000000 == 0) cout << i << endl;
      tracktree -> GetEntry(i);

      if(target == 0 || dump == 1) continue;
      if(targetPos != 3) continue;
      lordm -> SetXYZM(px1, py1, pz1, 0.105);
      phi = lordm -> Phi();

      //in roadset62 careful with charge variable when magnet flipped
      if(charge == -1){
	Hmmuonphi -> Fill(phi);
	Hcharge -> Fill(charge);
      } 
      else if(charge == 1){
	Hpmuonphi -> Fill(phi);
	Hcharge -> Fill(charge);
      }
    } 
  }

  TCanvas *c3 = new TCanvas("c3", "c3", 1800, 800);
  c3 -> Divide(2, 2);
  c3 -> cd(1);
  Hdmphi -> Draw();
  c3 -> cd(2);
  Hdmcsphi -> Draw();
  c3 -> cd(3);
  Hmmuonphi -> Draw();
  c3 -> cd(4);
  Hpmuonphi -> Draw();
  sprintf(Hname, "./images/phidists_roadset%d_prod%d.gif", roadset, prod);
  c3 -> SaveAs(Hname);

  sprintf(Fname, "./root_files/phidists_roadset%d_prod%d.root", roadset, prod);
  TFile *outfile = new TFile(Fname, "RECREATE");
  outfile -> cd();
  Hdmphi -> Write();
  Hdmcsphi -> Write();
  Hmmuonphi -> Write();
  Hpmuonphi -> Write();
  Hcharge -> Write();
  
}
