void drawbeamangle(const int tgtdmp = 2)
{

  char Fname[128];
  sprintf(Fname, "./nDST/BeamAngle_tgt%d.root",tgtdmp);
  TFile *inFile = new TFile(Fname);

  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");

  TH2F *Hm2m = new TH2F("Hm2m","M2 Vert Mean vs. Hori Mean", 40, -2, 2, 40, -2, 2);
  TH2F *Hm3m = new TH2F("Hm3m","M3 Vert Mean vs. Hori Mean", 40, -5, 5, 40,  -5, 5);
  TH2F *Hm2s = new TH2F("Hm2s","M2 Vert Sigma vs. Hori Sigma",48, 0, 15, 48, 0, 15);
  TH2F *Hm3s = new TH2F("Hm3s","M3 Vert Sigma vs. Hori Sigma",48, 0, 6, 48, 0, 6);

  TH2F *Hdiff = new TH2F("Hdiff","(M2 -M3) Vert Mean vs. Hori Mean", 20, -5, 5, 20,  -5, 5);

  TH2F *Hpt = new TH2F("Hpt","dpy vs dpx of Dimuons", 40,-4,4,40,-4,4);
  TH2F *Hpt0 = new TH2F("Hpt0","dpy vs dpx of Dimuons", 40,-4,4,40,-4,4);
  TH2F *Hpt1 = new TH2F("Hpt1","dpy vs dpx of Dimuons", 40,-4,4,40,-4,4);

  TH1I *Hsp0 = new TH1I("Hsp0", "spillID",700, 300000, 370000);
  TH1I *Hsp1 = new TH1I("Hsp1", "spillID",700, 300000, 370000);
  TH1I *Hrun0 = new TH1I("Hrun0", "runID",700, 300000, 370000);
  TH1I *Hrun1 = new TH1I("Hrun1", "runID",700, 300000, 370000);
  Hsp1 -> SetLineColor(2);

  Hm2m -> SetXTitle("hm (mm)");
  Hm3m -> SetXTitle("hm (mm)");
  Hm2s -> SetXTitle("hs (mm)");
  Hm3s -> SetXTitle("hs (mm)");
  Hm2m -> SetYTitle("vm (mm)");
  Hm3m -> SetYTitle("vm (mm)");
  Hm2s -> SetYTitle("vs (mm)");
  Hm3s -> SetYTitle("vs (mm)");

  Hpt -> SetXTitle("px (GeV)");
  Hpt0 -> SetXTitle("px (GeV)");
  Hpt1 -> SetXTitle("px (GeV)");
  Hpt -> SetYTitle("py (GeV)");
  Hpt0 -> SetYTitle("py (GeV)");
  Hpt1 -> SetYTitle("py (GeV)");

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
    //if(tgtdmp == 1 && dz > 25)continue;
    if(runID > 10091 && runID < 10185)continue;
    Hm2m -> Fill(m2hm,m2vm);
    Hm3m -> Fill(m3hm,m3vm);
    Hm2s -> Fill(m2hs,m2vs);
    Hm3s -> Fill(m3hs,m3vs);

    dhm = m2hm - m3hm;
    dvm = m2vm - m3vm;

    Hdiff -> Fill(dhm,dvm);
    Hpt -> Fill(dpx,dpy);

    if((m3hm > -2 && m3hm < 1) && (m3vm > 2 && m3vm < 4.5)) {
      Hpt0 -> Fill(dpx,dpy);
      Hsp0 -> Fill(spillID);
    }
    if((m3hm > 1 && m3hm < 2.5) && (m3vm > 1 && m3vm < 2)){
      cout << runID << " " << spillID << " " << eventID << " " << target << " " << dump << " " << targetPos << endl;
      Hpt1 -> Fill(dpx,dpy);
      Hsp1 -> Fill(spillID);
    }
  }

  TLine *m2h = new TLine(-2,0,2,0);
  TLine *m2v = new TLine(0,-2,0,2);
  m2h -> SetLineStyle(2);
  m2v -> SetLineStyle(2);
  TLine *m3h = new TLine(-5,0,5,0);
  TLine *m3v = new TLine(0,-5,0,5);
  m3h -> SetLineStyle(2);
  m3v -> SetLineStyle(2);

  TLine *mpx = new TLine(-4,0,4,0);
  TLine *mpy = new TLine(0,-4,0,4);
  mpx -> SetLineStyle(2);
  mpy -> SetLineStyle(2);

  TCanvas *c2 = new TCanvas("c2","c2",1200,400);
  c2 -> Divide(2,1);
  c2 -> cd(1);
  Hm2m -> Draw("colz");
  m2h -> Draw();
  m2v -> Draw();
  c2 -> cd(2);
  Hm2s -> Draw("colz");

  TCanvas *c3 = new TCanvas("c3","c3",1200,400);
  c3 -> Divide(2,1);
  c3 -> cd(1);
  Hm3m -> Draw("colz");
  m3h -> Draw();
  m3v -> Draw();
  c3 -> cd(2);
  Hm3s -> Draw("colz");

  TCanvas *c1 = new TCanvas("c1","c1",600,400);
  Hpt -> Draw("colz");
  mpx -> Draw();
  mpy -> Draw();
  //Hsp1 -> Draw();
  //Hsp0 -> Draw("same");

  TCanvas *c4 = new TCanvas("c4","c4",1200,400);
  c4 -> Divide(2,1);
  c4 -> cd(1);
  Hm3m -> Draw("colz");
  m3h -> Draw();
  m3v -> Draw();
  c4 -> cd(2);
  Hpt0 -> Draw("colz");
  mpx -> Draw();
  mpy -> Draw();

  TCanvas *c5 = new TCanvas("c5","c5",1200,400);
  c5 -> Divide(2,1);
  c5 -> cd(1);
  Hm3m -> Draw("colz");
  m3h -> Draw();
  m3v -> Draw();
  c5 -> cd(2);
  Hpt1 -> Draw("colz");
  mpx -> Draw();
  mpy -> Draw();
  //Hsp0 -> Draw();
  //Hsp1 -> Draw("same");

}
