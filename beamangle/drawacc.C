void drawacc(const int tgtdmp = 0)
{

  char Fname[128];
  sprintf(Fname, "./nDST/BeamAngle_tgt%d.root",tgtdmp);
  TFile *inFile = new TFile(Fname);

  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");

  TH1F *hphi = new TH1F("hphi","hphi",64,-3.2,3.2);
  TH1F *htheta = new TH1F("hphi","htheta",45,0,45);

  Int_t dimuonID;
  Int_t runID;
  Int_t spillID, eventID;
  UInt_t negTrack, posTrackID;
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

  TLorentzVector *lorentz = new TLorentzVector();
  Float_t phi;
  Float_t theta;


  dmtree -> SetBranchAddress ("dimuonID",        &dimuonID);
  dmtree -> SetBranchAddress ("runID",           &runID);
  dmtree -> SetBranchAddress ("eventID",         &eventID);
  dmtree -> SetBranchAddress ("spillID",         &spillID);
  //dmtree -> SetBranchAddress ("posTrackID",    &posTrackID);
  //dmtree -> SetBranchAddress ("negTrackID",    &negTrackID);
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
  //dmtree -> SetBranchAddress ("px1",             &px1);
  //dmtree -> SetBranchAddress ("py1",             &py1);
  //dmtree -> SetBranchAddress ("pz1",             &pz1);
  //dmtree -> SetBranchAddress ("px2",             &px2);
  //dmtree -> SetBranchAddress ("py2",             &py2);
  //dmtree -> SetBranchAddress ("pz2",             &pz2);
  dmtree -> SetBranchAddress ("target",          &target);
  dmtree -> SetBranchAddress ("dump",            &dump);
  dmtree -> SetBranchAddress ("targetPos",       &targetPos);
  dmtree -> SetBranchAddress ("m3hm",            &m3hm);
  dmtree -> SetBranchAddress ("m3hs",            &m3hs);
  dmtree -> SetBranchAddress ("m3vm",            &m3vm);
  dmtree -> SetBranchAddress ("m3vs",            &m3vs);
  dmtree -> SetBranchAddress ("m2hm",            &m2hm);
  dmtree -> SetBranchAddress ("m2hs",            &m2hs);
  dmtree -> SetBranchAddress ("m2vm",            &m2vm);
  dmtree -> SetBranchAddress ("m2vs",            &m2vs);

  int nentries = dmtree -> GetEntries();
  cout << "The number of Entries is " << nentries << endl;
  for(int i = 0; i < nentries; i++){
    dmtree -> GetEntry(i);
    //if(tgtdmp == 1 && dz < 50)continue;

    lorentz -> SetXYZM(dpx,dpy,dpz,mass);
    phi = lorentz -> Phi();
    theta = lorentz -> Theta();
    hphi -> Fill(phi);
    htheta -> Fill(theta*180/3.14159);
    
  }

  TCanvas *c2 = new TCanvas("c2","c2",1200,400);
  c2 -> Divide(2,1);
  c2 -> cd(1);
  hphi -> Draw();
  c2 -> cd(2);
  htheta -> Draw();

}
