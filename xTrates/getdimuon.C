void getdimuon(const char *schema)
{
   //connect to server
   //TSQLServer *con = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283", "seaguest", "qqbar2mu+mu-");
   TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306", "root", "");
   
   TTreeSQL * treeql_run = new TTreeSQL(con, schema, "mRun");
   Int_t nEvGen; 
   Int_t nevgen;
   Float_t sigwt;
   treeql_run -> SetBranchAddress("nEvGen", &nEvGen);

   for(int i = 0; i < 1; i++){
     treeql_run -> GetEntry(i);
     nevgen = nEvGen;
   }
   cout << nevgen << endl;
   TTreeSQL * treeql = new TTreeSQL(con, schema, "mDimuon");

   
   UInt_t runID;
   Int_t spillID, eventID;
   UInt_t mTrackID1, mTrackID2;
   Float_t xF, xT, xB, mass;
   Float_t dx, dy, dz;
   Float_t dpx, dpy, dpz;
   Float_t phi_gam, phi_mu, theta_mu;
   UInt_t acceptHodoAll, acceptDriftAll;
   Float_t sigWeight;   
   
   TH1F *Hmass = new TH1F("Hmass", "HodoAll Mass of mu+mu- pair", 100, 0, 10);
   TH1F *Hpz = new TH1F("Hpz", "HodoAll pz of mu+mu- pair", 120,0,120);
   TH1F *HxF = new TH1F("HxF", "HodoAll Feynman-x of pair", 100, -1, 1);
   TH1F *HxB = new TH1F("HxB", "HodoAll Bjorken-x of Beam, x_{B}", 100, 0, 1);
   TH1F *HxT = new TH1F("HxT", "HodoAll Bjorken-x of Target, x_{T}", 100, 0, 1);
  
   TH2F *HxTxB = new TH2F("HxTxB", "HodoAllx_{T} vs. x_{B}", 100, 0, 1, 100, 0, 1);
   TH2F *Hpzmass = new TH2F("Hpzmass", "HodoAll p_{z} vs. Mass", 100, 0, 10, 120, 0, 120);

   TH1F *Dmass = new TH1F("Dmass", "DriftAll Mass of mu+mu- pair", 100, 0, 10);
   TH1F *Dpz = new TH1F("Dpz", "DriftAll pz of mu+mu- pair", 100,0,100);
   TH1F *DxF = new TH1F("DxF", "DriftAll Feynman-x of pair", 100, -1, 1);
   TH1F *DxB = new TH1F("DxB", "DriftAll Momentum fraction of Beam, x_{B}", 100, 0, 1);
   TH1F *DxT = new TH1F("DxT", "DriftAll Momentum fraction of Target, x_{T}", 100, 0, 1);
   TH2F *DxTxB = new TH2F("DxTxB", "DriftAll x_{T} vs. x_{B}", 100, 0, 1, 100, 0, 1);
   TH2F *Dpzmass = new TH2F("Dpzmass", "DriftAll p_{z} vs. Mass", 100, 0, 10, 120, 0, 120);

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

   Dmass -> SetXTitle("mass [GeV/c^{2}]");
   DxF -> SetXTitle("Feynman-x");
   DxB -> SetXTitle("x_{B}");
   DxT -> SetXTitle("x_{T}");
   Dpz -> SetXTitle("p_{z}");
   DxF -> SetYTitle("Rate [Hz]");
   DxB -> SetYTitle("Rate [Hz]");
   DxT -> SetYTitle("Rate [Hz]");
   Dpz -> SetYTitle("Rate [Hz]");
   DxTxB -> SetXTitle("x_{T}");
   DxTxB -> SetYTitle("x_{B}"); 
   Dpzmass -> SetXTitle("mass [GeV/c^{2}]");
   Dpzmass -> SetYTitle("p_{z}");  

   Hmass -> Sumw2();
   HxF -> Sumw2();
   HxB -> Sumw2();
   HxT ->  Sumw2();
   Hpz ->  Sumw2();
   HxTxB ->  Sumw2();
   Hpzmass ->  Sumw2();
   Dmass ->  Sumw2();
   DxF ->  Sumw2();
   DxB ->  Sumw2();
   DxT ->  Sumw2();
   Dpz ->  Sumw2();
   DxTxB ->  Sumw2();
   Dpzmass ->  Sumw2();   

   treeql -> SetBranchAddress ("runID",      &runID);
   treeql -> SetBranchAddress ("eventID",    &eventID);
   treeql -> SetBranchAddress ("spillID",    &spillID);
   //treeql -> SetBranchAddress ("parentID",   &particleID);
   //treeql -> SetBranchAddress ("particleID", &particleID);
   treeql -> SetBranchAddress ("mass",         &mass);
   treeql -> SetBranchAddress ("xF",         &xF);
   treeql -> SetBranchAddress ("xT",         &xT);
   treeql -> SetBranchAddress ("xB",         &xB);
   treeql -> SetBranchAddress ("dx",         &dx);
   treeql -> SetBranchAddress ("dy",         &dy);
   treeql -> SetBranchAddress ("dz",         &dz);
   treeql -> SetBranchAddress ("dpx",        &dpx);
   treeql -> SetBranchAddress ("dpy",        &dpy);
   treeql -> SetBranchAddress ("dpz",        &dpz);
   treeql -> SetBranchAddress ("phi_gam",    &phi_gam);
   treeql -> SetBranchAddress ("phi_mu",         &phi_mu);
   treeql -> SetBranchAddress ("theta_mu",       &theta_mu);
   treeql -> SetBranchAddress ("acceptHodoAll",  &acceptHodoAll);
   treeql -> SetBranchAddress ("acceptDriftAll", &acceptDriftAll);
   treeql -> SetBranchAddress ("sigWeight",      &sigWeight);
  

   int nentries = treeql -> GetEntries();
   cout << "The number of Entries is " << nentries << endl;
   for(int i = 0; i < nentries; i++){
     treeql -> GetEntry(i);
     //if(eventID%1 == 0) cout << "EventID " << eventID << endl;
     sigwt = sigWeight / (nevgen* 1.0);
     if(acceptHodoAll == 1){
       Hmass -> Fill(mass, sigwt);
       Hpz -> Fill(dpz, sigwt);
       HxF -> Fill(xF, sigwt);
       HxT -> Fill(xT, sigwt);
       HxB -> Fill(xB, sigwt);
       HxTxB -> Fill(xT, xB, sigwt);
       Hpzmass -> Fill(mass, dpz, sigwt);

     }
     
     if(acceptDriftAll == 1){
       Dmass -> Fill(mass, sigwt);
       Dpz -> Fill(dpz, sigwt);
       DxF -> Fill(xF, sigwt);
       DxT -> Fill(xT, sigwt);
       DxB -> Fill(xB, sigwt);
       DxTxB -> Fill(xT, xB, sigwt);
       Dpzmass -> Fill(mass, dpz, sigwt);
     }

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
   sprintf(outname, "./root_files/hitrate_%s.root", schema);
   TFile *outfile = new TFile(outname,"RECREATE");
   outfile -> cd();
   Hmass -> Write();
   Hpz -> Write();
   HxF -> Write();
   HxB -> Write();
   HxT -> Write();
   HxTxB -> Write();
   Hpzmass -> Write();
   Dmass -> Write();
   Dpz -> Write();
   DxF -> Write();
   DxB -> Write();
   DxT -> Write();
   DxTxB -> Write();
   Dpzmass -> Write();

	for(int b = 0; b < 20; b++){
	  HxTbins[b] -> Write();
	}

	GxT -> Write("GxT");
	Zvtx -> Write();
	outfile -> Close();

}
