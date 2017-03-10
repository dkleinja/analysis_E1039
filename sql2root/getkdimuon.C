void getkdimuon(const char *schema)
{
  //connect to server
  TSQLServer *con = TSQLServer::Connect("mysql://e906-db3.fnal.gov:3306", "seaguest", "qqbar2mu+mu-");
  //TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306", "root", "");
  
  //TTreeSQL * treeql_run = new TTreeSQL(con, schema, "mRun");
  Int_t nEvGen; 
  Int_t nevgen = 1;
  Float_t sigwt =1;

  cout << nevgen << endl;
  
  TTreeSQL * treeql = new TTreeSQL(con, schema, "kDimuon");
  Int_t dimuonID;
  Int_t runID;
  Int_t spillID, eventID;
  Int_t negTrackID, posTrackID;
  Int_t negHits, posHits;
  Float_t xF, xT, xB, mass;
  Float_t costh, phi;
  Float_t dx, dy, dz;
  Float_t dpx, dpy, dpz;
  Float_t px1, py1, pz1;
  Float_t px2, py2, pz2;
  Float_t trackSeparation, chisq_dimuon;
  UInt_t target, dump;
  Int_t targetPos;
  Float_t m3hm, m3hs, m3vm, m3vs;
  Float_t m2hm, m2hs, m2vm, m2vs;
  Float_t posx1, posy1, posx3, posy3;
  Float_t negx1, negy1, negx3, negy3;
  Float_t Intensity_p;


  TH1F *Hmass = new TH1F("Hmass", "HodoAll Mass of mu+mu- pair", 100, 0, 10);
  TH1F *Hpz = new TH1F("Hpz", "HodoAll pz of mu+mu- pair", 120,0,120);
  TH1F *HxF = new TH1F("HxF", "HodoAll Feynman-x of pair", 100, -1, 1);
  TH1F *HxB = new TH1F("HxB", "HodoAll Bjorken-x of Beam, x_{B}", 100, 0, 1);
  TH1F *HxT = new TH1F("HxT", "HodoAll Bjorken-x of Target, x_{T}", 100, 0, 1);

  TH2F *HxTxB = new TH2F("HxTxB", "HodoAllx_{T} vs. x_{B}", 100, 0, 1, 100, 0, 1);
  TH2F *Hpzmass = new TH2F("Hpzmass", "HodoAll p_{z} vs. Mass", 100, 0, 10, 120, 0, 120);

  TH1F *Zvtx = new TH1F("Zvtx"," Smeared Zvtx",160,-400,240);
  Zvtx -> Sumw2();
  Zvtx -> SetXTitle("Zvtx [cm]");
  Zvtx -> SetYTitle("Rate [Hz]");

  Hmass -> SetXTitle("mass [GeV/c^{2}]");
  HxF -> SetXTitle("Feynman-x");
  HxB -> SetXTitle("x_{B}");
  HxT -> SetXTitle("x_{T}");
  Hpz -> SetXTitle("p_{z}");
  HxF -> SetYTitle("Rate [Hz]");
  HxB -> SetYTitle("Rate [Hz]");
  HxT -> SetYTitle("Rate [Hz]");
  Hpz -> SetYTitle("Rate [Hz]");
  HxTxB -> SetXTitle("x_{T}");
  HxTxB -> SetYTitle("x_{B}");
  Hpzmass -> SetXTitle("mass [GeV/c^{2}]");
  Hpzmass -> SetYTitle("p_{z}");

  Hmass -> Sumw2();
  HxF -> Sumw2();
  HxB -> Sumw2();
  HxT ->  Sumw2();
  Hpz ->  Sumw2();
  HxTxB ->  Sumw2();
  Hpzmass ->  Sumw2();

  //Make NTuple for Andi, Kun
  TTree *kdimuon = new TTree("kdimuon","kdimuon");
  kdimuon -> SetDirectory(0);
  kdimuon -> Branch ("dimuonID",        &dimuonID,        "dimuonID/I");
  kdimuon -> Branch ("runID",           &runID,           "runID/I");
  kdimuon -> Branch ("eventID",         &eventID,         "eventID/I");
  kdimuon -> Branch ("spillID",         &spillID,         "spillID/I");
  kdimuon -> Branch ("targetPos",       &targetPos,       "targetPos/I");
  kdimuon -> Branch ("posTrackID",    &posTrackID,       "posTrackID/I");
  kdimuon -> Branch ("negTrackID",    &negTrackID,       "negTrackID/I");
  kdimuon -> Branch ("posHits",       &posHits,          "posHits/I");
  kdimuon -> Branch ("negHits",       &negHits,          "negHits/I");
  kdimuon -> Branch ("dx",              &dx,              "dx/F");
  kdimuon -> Branch ("dy",              &dy,              "dy/F");
  kdimuon -> Branch ("dz",              &dz,              "dz/F");
  kdimuon -> Branch ("dpx",             &dpx,             "dpx/F");
  kdimuon -> Branch ("dpy",             &dpy,             "dpy/F");
  kdimuon -> Branch ("dpz",             &dpz,             "dpz/F");
  kdimuon -> Branch ("mass",            &mass,            "mass/F");
  kdimuon -> Branch ("xF",              &xF,              "xF/F");  
  kdimuon -> Branch ("xB",              &xB,              "xB/F");
  kdimuon -> Branch ("xT",              &xT,              "xT/F");
  kdimuon -> Branch ("costh",           &costh,           "costh/F");
  kdimuon -> Branch ("phi",             &phi,             "phi/F");
  kdimuon -> Branch ("trackSeparation", &trackSeparation, "trackSeparation/F");
  kdimuon -> Branch ("chisq_dimuon",    &chisq_dimuon,    "chisq_dimuon/F");
  kdimuon -> Branch ("px1",           &px1,             "px1/F");
  kdimuon -> Branch ("py1",           &py1,             "py1/F");
  kdimuon -> Branch ("pz1",           &pz1,             "pz1/F");
  kdimuon -> Branch ("px2",           &px2,             "px2/F");
  kdimuon -> Branch ("py2",           &py2,             "py2/F");
  kdimuon -> Branch ("pz2",           &pz2,             "pz2/F");
  kdimuon -> Branch ("target",          &target,          "target/I");
  kdimuon -> Branch ("dump",            &dump,            "dump/I");
  kdimuon -> Branch ("Intensity_p",     &Intensity_p,      "Intensity_p/F");
  //kdimuon -> Branch ("posx1",           &posx1,           "posx1/F");
  //kdimuon -> Branch ("posy1",           &posy1,           "posy1/F");
  //kdimuon -> Branch ("posx3",           &posx3,           "posx3/F");
  //kdimuon -> Branch ("posy3",           &posy3,           "posy3/F");
  //kdimuon -> Branch ("negx1",           &negx1,           "negx1/F");
  //kdimuon -> Branch ("negy1",           &negy1,           "negy1/F");
  //kdimuon -> Branch ("negx3",           &negx3,           "negx3/F");
  //kdimuon -> Branch ("negy3",           &negy3,           "negy3/F");
  //kdimuon -> Branch ("m3hm",            &m3hm,            "m3hm/F");
  //kdimuon -> Branch ("m3hs",            &m3hs,            "m3hs/F");
  //kdimuon -> Branch ("m3vm",            &m3vm,            "m3vm/F");
  //kdimuon -> Branch ("m3vs",            &m3vs,            "m3vs/F");
  //kdimuon -> Branch ("m2hm",            &m2hm,            "m2hm/F");
  //kdimuon -> Branch ("m2hs",            &m2hs,            "m2hs/F");
  //kdimuon -> Branch ("m2vm",            &m2vm,            "m2vm/F");
  //kdimuon -> Branch ("m2vs",            &m2vs,            "m2vs/F");

  treeql -> SetBranchAddress ("dimuonID",        &dimuonID);
  treeql -> SetBranchAddress ("runID",           &runID);
  treeql -> SetBranchAddress ("eventID",         &eventID);
  treeql -> SetBranchAddress ("spillID",         &spillID);
  treeql -> SetBranchAddress ("targetPos",       &targetPos);
  treeql -> SetBranchAddress ("posTrackID",      &posTrackID);
  treeql -> SetBranchAddress ("negTrackID",      &negTrackID);
  treeql -> SetBranchAddress ("negHits",         &negHits);
  treeql -> SetBranchAddress ("posHits",         &posHits);
  treeql -> SetBranchAddress ("dx",              &dx);
  treeql -> SetBranchAddress ("dy",              &dy);
  treeql -> SetBranchAddress ("dz",              &dz);
  treeql -> SetBranchAddress ("dpx",             &dpx);
  treeql -> SetBranchAddress ("dpy",             &dpy);
  treeql -> SetBranchAddress ("dpz",             &dpz);
  treeql -> SetBranchAddress ("mass",            &mass);
  treeql -> SetBranchAddress ("xF",              &xF);
  treeql -> SetBranchAddress ("xB",              &xB);
  treeql -> SetBranchAddress ("xT",              &xT);
  treeql -> SetBranchAddress ("costh",    	 &costh);
  treeql -> SetBranchAddress ("phi",    	 &phi);
  treeql -> SetBranchAddress ("trackSeparation", &trackSeparation);
  treeql -> SetBranchAddress ("chisq_dimuon",    &chisq_dimuon);
  treeql -> SetBranchAddress ("px1",             &px1);
  treeql -> SetBranchAddress ("py1",             &py1);
  treeql -> SetBranchAddress ("pz1",             &pz1);
  treeql -> SetBranchAddress ("px2",             &px2);
  treeql -> SetBranchAddress ("py2",             &py2);
  treeql -> SetBranchAddress ("pz2",             &pz2);
  treeql -> SetBranchAddress ("target",          &target);
  treeql -> SetBranchAddress ("dump",            &dump);
  treeql -> SetBranchAddress ("Intensity_p",     &Intensity_p);
  //treeql -> SetBranchAddress ("posx1",           &posx1);
  //treeql -> SetBranchAddress ("posy1",           &posy1);
  //treeql -> SetBranchAddress ("posx3",           &posx3);
  //treeql -> SetBranchAddress ("posy3",           &posy3);
  //treeql -> SetBranchAddress ("negx1",           &negx1);
  //treeql -> SetBranchAddress ("negy1",           &negy1);
  //treeql -> SetBranchAddress ("negx3",           &negx3);
  //treeql -> SetBranchAddress ("negy3",           &negy3);
  //treeql -> SetBranchAddress ("m3hm",            &m3hm);
  //treeql -> SetBranchAddress ("m3hs",            &m3hs);
  //treeql -> SetBranchAddress ("m3vm",            &m3vm);
  //treeql -> SetBranchAddress ("m3vs",            &m3vs);
  //treeql -> SetBranchAddress ("m2hm",            &m2hm);
  //treeql -> SetBranchAddress ("m2hs",            &m2hs);
  //treeql -> SetBranchAddress ("m2vm",            &m2vm);
  //treeql -> SetBranchAddress ("m2vs",            &m2vs);

  int nentries = treeql -> GetEntries();
  cout << "The number of Entries is " << nentries << endl;
  for(int i = 0; i < nentries; i++){
    if(i%100000 == 0) cout << i << endl;
    treeql -> GetEntry(i);

    //if(target == 0 && dump ==0) cout << runID << " " << spillID << " " << eventID << " " << target << " " << dump << " " << targetPos << endl;
    //if(mass < 4.2)continue;
    kdimuon -> Fill();

    //if(eventID%1 == 0) cout << "EventID " << eventID << endl;
    Hmass -> Fill(mass, sigwt);
    Hpz -> Fill(dpz, sigwt);
    HxF -> Fill(xF, sigwt);
    HxT -> Fill(xT, sigwt);
    HxB -> Fill(xB, sigwt);
    HxTxB -> Fill(xT, xB, sigwt);
    Hpzmass -> Fill(mass, dpz, sigwt);

    dz = gRandom -> Gaus(dz,35);
    Zvtx -> Fill(dz, sigwt);
     
    //TCanvas *c1 = new TCanvas("c1","c1",

  }

  Int_t getbin;
  char Hname[64];
  TH1F *HxTbins[20];
  
  TGraphErrors *GxT = new TGraphErrors();
  GxT -> SetMarkerStyle(21);
  GxT -> SetTitle("Target-x");
  GxT -> SetName("GxT");
  Int_t bval = 0;

  Double_t x[20], y[20],ex[20],ey[20];

  //get mean xT for 4-6 points
  for(int b = 0; b < 20; b++){
    sprintf(Hname, "HxTbins_xt%d", b);
    HxTbins[b] = new TH1F(Hname, Hname, 100, 0, 1);
     
    for(int i = 1; i < 6; i++){     
      getbin = 5*b + i;
      HxTbins[b] -> SetBinContent(getbin, HxT -> GetBinContent(getbin));
      HxTbins[b] -> SetBinError  (getbin, HxT -> GetBinError(getbin));       
    }

    x[b] = HxTbins[b] -> GetMean();
    y[b] = HxTbins[b] -> IntegralAndError(1,100,ey[b]);

    if(y[b] > 1.0e-9){
      GxT -> SetPoint(bval, x[b], y[b]);
      GxT -> SetPointError(bval, 0, ey[b]);
      bval++;
    }
  }

  GxT -> GetXaxis() -> SetTitle("x_{T}");
  GxT -> GetYaxis() -> SetTitle("Rate [Hz]");

  char outname[128];
  sprintf(outname, "./nDST/%s.root", schema);
  TFile *outfile = new TFile(outname,"RECREATE");
  outfile -> cd();
  kdimuon -> Write();
  Hmass -> Write();
  Hpz -> Write();
  HxF -> Write();
  HxB -> Write();
  HxT -> Write();
  HxTxB -> Write();
  Hpzmass -> Write();
 
  for(int b = 0; b < 20; b++){
    HxTbins[b] -> Write();
  }

  GxT -> Write("GxT");
  Zvtx -> Write();
  outfile -> Close();

}
