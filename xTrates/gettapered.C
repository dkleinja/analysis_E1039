void gettapered(const char *schema, const float maglength = 100, const float magpos = 150, const float magval = 0)
{
   // EXAMPLE USERCODE FOR ANALYZING SQL DATA

   gROOT->Reset();
   gBenchmark->Reset();
   gStyle->SetOptStat(0);
   gStyle->SetPalette(1);
   //gStyle->SetCanvasColor(33);
   //gStyle->SetFrameFillColor(18);

   char Hname[128];
   char outname[128];

   Double_t hxtbins[5] = {0.1,0.14,0.17,0.21,0.50};

   TH1F *Hmass = new TH1F("Hmass", "Excl0 Mass of mu+mu- pair", 100, 0, 10);
   TH1F *Hpz = new TH1F("Hpz", "Excl0 pz of mu+mu- pair", 120,0,120);
   TH1F *HxF = new TH1F("HxF", "Excl0 Feynman-x of pair", 100, -1, 1);
   TH1F *HxB = new TH1F("HxB", "Excl0 Bjorken-x of Beam, x_{B}", 100, 0, 1);
   TH1F *HxT = new TH1F("HxT", "Excl0 Bjorken-x of Target, x_{T}", 100, 0, 1);
   TH1F *HxTprop = new TH1F("HxTprop", "Excl0 Bjorken-x of Target, x_{T}", 4, hxtbins);

   TH2F *HxTxB = new TH2F("HxTxB", "Excl0 x_{T} vs. x_{B}", 100, 0, 1, 100, 0, 1);
   TH2F *Hpzmass = new TH2F("Hpzmass", "Excl0 p_{z} vs. Mass", 100, 0, 10, 120, 0, 120);

   TH1F *Hdr1 = new TH1F("Hdr1","Dimuon #DeltaR from #theta and #Deltaz",50,0,50);
   TH1F *Hdr2 = new TH1F("Hdr2","Dimuon #DeltaR from calculation",50,0,50);
   Hdr2 -> SetLineColor(2);

   sprintf(Hname, "End of magnet [z_{End}= %d cm], z_{Length}=%d cm, z_{pos}=%1.0f cm, B=%1.0f Tesla ", (-1*magpos + maglength/2), maglength, -1*magpos, magval);
   //cout << magval << endl;exit(1);
   TH2F *Hdxdy = new TH2F("Hdxdy", Hname, 50, -50, 50, 50, -50, 50);
   //TH2F *Hdxdy = new TH2F("Hdxdy", "#Delta x vs #Delta y at -50 cm", 40, -20, 20, 25, -20, 20);

   TH1F *Zvtx = new TH1F("Zvtx"," Smeared Zvtx",160,-400,240);
   Zvtx -> Sumw2();
   Zvtx -> SetXTitle("Zvtx [cm]");
   Zvtx -> SetYTitle("Rate [Hz]");

   TH1I *Hexcl = new TH1I("Hexcl", "exclusion cut applied",3,0,2);

   Hmass -> SetXTitle("mass [GeV/c^{2}]");
   HxF -> SetXTitle("Feynman-x");
   HxB -> SetXTitle("x_{B}");
   HxT -> SetXTitle("x_{T}");
   HxTprop -> SetXTitle("x_{T}");
   Hpz -> SetXTitle("p_{z}");
   HxF -> SetYTitle("Rate [Hz]");
   HxB -> SetYTitle("Rate [Hz]");
   HxT -> SetYTitle("Rate [Hz]");
   HxTprop -> SetYTitle("Rate [Hz]");
   Hpz -> SetYTitle("Rate [Hz]");
   HxTxB -> SetXTitle("x_{T}");
   HxTxB -> SetYTitle("x_{B}");
   Hpzmass -> SetXTitle("mass [GeV/c^{2}]");
   Hpzmass -> SetYTitle("p_{z}");
   Hdr1 -> SetXTitle("#Deltar [cm]");
   Hdr2 -> SetXTitle("#Deltar [cm]");
   Hdxdy -> SetXTitle("#Deltax [cm]");
   Hdxdy -> SetYTitle("#Deltay [cm]");

   Hmass -> Sumw2();
   HxF -> Sumw2();
   HxB -> Sumw2();
   HxT ->  Sumw2();
   HxTprop ->  Sumw2();
   Hpz ->  Sumw2();
   //Hdr1 ->  Sumw2();
   //Hdr2 ->  Sumw2();
   HxTxB ->  Sumw2();
   Hpzmass ->  Sumw2();
 
   char stmt [1024];
   char delstmt [64];
   char seedstmt [64];

  /*
   // Assign which seed to use (this chooses a database on the server)
   sprintf(seedstmt, "USE %s", schema);

   // Drop the table if it's already there for some reason
   //   (DROP means 'delete')
   sprintf(delstmt, "DROP TABLE IF EXISTS mTrackTable");
   
   // Assemble the query based on the parameters given
   sprintf(stmt, // This first part tells it where to put the data you want
	   "CREATE TABLE mTrackTable\n"
	   "SELECT mTrackID,particleID,px0,py0,pz0 "
	   // The FROM part puts together the source and how the tables
	   //   are related to one another
	   "FROM mTrack");
	   //"WHERE mTrack.mTrackID= mDimuon.mTrackID1 OR mTrack.mTrackID = mDimuon.mTrackID2;");
   cout << stmt << endl;
*/
   // This it used to time the SQL action of the macro
   gBenchmark->Start("SQL");

   // Connect to server
   TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306",
                "root", "");
/*
   // Select which database to use on the server
   con->Exec(seedstmt);

   // Delete the table if it already exitst
   con->Exec(delstmt);

   // Create a table based on the exact cut of data you want
   con->Exec(stmt);

   // Display the time it took
   gBenchmark->Show("SQL");
*/
   TTreeSQL * treeql_mTrack = new TTreeSQL(con, schema, "mTrackTable2");

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

   TTreeSQL * treeql_mDimuon = new TTreeSQL(con, schema, "mDimuon");

   UInt_t runID;
   Int_t spillID, eventID;
   Int_t mTrackID1, mTrackID2;
   Float_t xF, xT, xB, mass;
   Float_t dx, dy, dz;
   Float_t dpx, dpy, dpz;
   Float_t phi_gam, phi_mu, theta_mu;
   UInt_t acceptHodoAll, acceptDriftAll;
   Float_t sigWeight;

   Int_t excl, zvtxcut;
   Int_t mTrackID, particleID; 
   Float_t px0, py0, pz0;
   Float_t deltax0, deltax1;
   Float_t deltax, deltay, deltar;
   Float_t dr; 
   Float_t ptkickx;
   Float_t charge, tesla;
   tesla = 0.0;
   zvtxcut = 0;

   if(schema == "dimuon_target_tarpos130_magpos0_magval0") zvtxcut = 1;
   else if(schema == "dimuon_target_tarpos130_kmagm1") zvtxcut = 1;
   if(magval == -1) tesla = -1;
   if(magval == 1) tesla = 1;
   //cout << "Tesla, zxtxcut " << tesla << " " << zvtxcut << endl;exit(0);
   
   treeql_mDimuon -> SetBranchAddress ("runID",      &runID);
   treeql_mDimuon -> SetBranchAddress ("eventID",    &eventID);
   treeql_mDimuon -> SetBranchAddress ("spillID",    &spillID);
   treeql_mDimuon -> SetBranchAddress ("mTrackID1",    &mTrackID1);
   treeql_mDimuon -> SetBranchAddress ("mTrackID2",    &mTrackID2);
   //reeql_mDimuon -> SetBranchAddress ("parentID",   &particleID);
   //treeql_mDimuon -> SetBranchAddress ("particleID", &particleID);
   treeql_mDimuon -> SetBranchAddress ("mass",         &mass);
   treeql_mDimuon -> SetBranchAddress ("xF",         &xF);
   treeql_mDimuon -> SetBranchAddress ("xT",         &xT);
   treeql_mDimuon -> SetBranchAddress ("xB",         &xB);
   treeql_mDimuon -> SetBranchAddress ("dx",         &dx);
   treeql_mDimuon -> SetBranchAddress ("dy",         &dy);
   treeql_mDimuon -> SetBranchAddress ("dz",         &dz);
   treeql_mDimuon -> SetBranchAddress ("dpx",        &dpx);
   treeql_mDimuon -> SetBranchAddress ("dpy",        &dpy);
   treeql_mDimuon -> SetBranchAddress ("dpz",        &dpz);
   treeql_mDimuon -> SetBranchAddress ("phi_gam",    &phi_gam);
   treeql_mDimuon -> SetBranchAddress ("phi_mu",         &phi_mu);
   treeql_mDimuon -> SetBranchAddress ("theta_mu",       &theta_mu);
   treeql_mDimuon -> SetBranchAddress ("acceptHodoAll",  &acceptHodoAll);
   treeql_mDimuon -> SetBranchAddress ("acceptDriftAll", &acceptDriftAll);
   treeql_mDimuon -> SetBranchAddress ("sigWeight",      &sigWeight);

   treeql_mTrack -> SetBranchAddress ("mTrackID",    &mTrackID);
   treeql_mTrack -> SetBranchAddress ("particleID",    &particleID);
   treeql_mTrack -> SetBranchAddress ("px0",    &px0);
   treeql_mTrack -> SetBranchAddress ("py0",    &py0);
   treeql_mTrack -> SetBranchAddress ("pz0",    &pz0);

   int nentries = treeql_mDimuon -> GetEntries();
   int nentries2 = treeql_mTrack -> GetEntries();
   cout << "The number of mDimuon Entries is " << nentries << endl;
   cout << "The number of mTrack Entries is " << nentries2 << endl;
   for(int i = 0; i < nentries; i++){
     treeql_mDimuon -> GetEntry(i);
     //if(eventID%100 == 0) cout << "EventID " << eventID << endl;
     
     //check the mTrack positions       
     excl = 0;
     treeql_mTrack -> GetEntry(2*i);
     if(mTrackID1 == mTrackID){
       if(particleID == 13){charge = -1.0;}
       if(particleID == -13){charge = 1.0;}
       //cout << i << " mTrackID1 " << mTrackID1 << " " << mTrackID << endl;
       //dr = TMath::Tan(theta_mu*3.14159/180) * (fabs(dz) - 50);
       deltay = (py0 / pz0) * ( -1*fabs(magpos - maglength/2.0) -  dz);
     
       ptkickx = -0.3 * charge * tesla;
       //cout << i << " charge, tesla, particleID, ptkick " << charge << " " << tesla <<  " " << ptkickx << endl;
       deltax0 = (px0 / pz0) * ( -1*magpos - dz);
       deltax1 = ((px0 + ptkickx) / pz0) * fabs(maglength/2.0);
       deltax = deltax0 + deltax1;
       //cout << "total distance " <<  -1*fabs(magpos - maglength/2.0) -  dz << " first distance " << -1*magpos -dz << " .  Last distance " << fabs(maglength/2) << endl;
       //cout << "Delta x after " << deltax << endl;
       
       deltar = sqrt ( pow(deltax,2) + pow(deltay,2));
       if(deltar < 5.71)excl = 1;
     }
     Hdxdy -> Fill(deltax,deltay);
     Hdr1 -> Fill(dr);
     Hdr2 -> Fill(deltar);
   
     treeql_mTrack -> GetEntry(2*i + 1);
     if(mTrackID2 == mTrackID){
       if(particleID == 13){charge = -1.0;}
       if(particleID == -13){charge = 1.0;}
       //cout << i << " mTrackID1 " << mTrackID1 << " " << mTrackID << endl;
       dr = TMath::Tan(theta_mu*3.14159/180) * (fabs(dz) - 50);
       deltay = (py0 / pz0) * ( -1*fabs(magpos - maglength/2.0) -  dz);
     
       ptkickx = -0.3 * charge * tesla;
       //cout << i << " charge, tesla, particleID, ptkick " << charge << " " << tesla <<  " " << ptkickx << endl;
       deltax0 = (px0 / pz0) * ( -1*magpos - dz);
       deltax1 = ((px0 + ptkickx) / pz0) * fabs(maglength/2.0);
       deltax = deltax0 + deltax1;
       deltar = sqrt ( pow(deltax,2) + pow(deltay,2));
       if(deltar < 5.71)excl = 1;
     }
     //cout << "deltax, deltay, deltar, excl " << deltax << " " << deltay << " " << deltar << " " << excl << endl;
     //draw the deltax,deltay
     Hdxdy -> Fill(deltax,deltay);
     Hdr1 -> Fill(dr);
     Hdr2 -> Fill(deltar);

     //zvtx exclusion cut for e906 part
     dz = gRandom -> Gaus(dz,35);
     if(zvtxcut == 1 && dz > -80.0){
       excl = 1;
       //cout << "should be here for tarpos130 " << dz << " " << zvtxcut << " " << excl << endl;
     }
     
     //if(dz < -80) cout << i << " " << zvtxcut << " " << dz << endl;
     Hexcl -> Fill(excl);
     sigwt = sigWeight / (nevgen* 1.0);
     if(excl == 0){
       Hmass -> Fill(mass, sigwt);
       Hpz -> Fill(dpz, sigwt);
       HxF -> Fill(xF, sigwt);
       HxT -> Fill(xT, sigwt);
       HxTprop -> Fill(xT, sigwt);
       HxB -> Fill(xB, sigwt);
       HxTxB -> Fill(xT, xB, sigwt);
       Hpzmass -> Fill(mass, dpz, sigwt);
       dz = gRandom -> Gaus(dz,35);        
       Zvtx -> Fill(dz, sigwt);
     }
   }

   Int_t getbin;
   char Hname[64];
   TH1F *HxTbins[20];
   TH1F *HxTbinsprop[4];

   TGraphErrors *GxT = new TGraphErrors();
   GxT -> SetMarkerStyle(21);
   GxT -> SetTitle("Target-x");
   GxT -> SetName("GxT");
   Double_t x[20], y[20],ex[20],ey[20];

   //get mean xT for 20 points
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

     GxT -> SetPoint(b, x[b], y[b]);
     GxT -> SetPointError(b, 0, ey[b]);
   }

   GxT -> GetXaxis() -> SetTitle("x_{T}");
   GxT -> GetYaxis() -> SetTitle("Rate [Hz]");
   
   TGraphErrors *GxTprop = new TGraphErrors();
   GxTprop -> SetMarkerStyle(21);
   GxTprop -> SetTitle("Target-x");
   GxTprop -> SetName("GxTprop");
   Double_t xprop[4], yprop[4],exprop[4],eyprop[4];

   //get the yield for 4 points from proposal
   for(int b = 0; b < 4; b++){
     sprintf(Hname, "HxTbinsprop_xt%d", b);
     HxTbinsprop[b] = new TH1F(Hname, Hname, 100, 0, 1);
   }

   //1st bin
   for(int i = 11; i < 14; i++){
   //for(int i = 11; i < 16; i++){
     HxTbinsprop[0] -> SetBinContent(i, HxT -> GetBinContent(i));
     HxTbinsprop[0] -> SetBinError  (i, HxT -> GetBinError(i));
   }

   //2nd bin
   for(int i = 14; i < 17; i++){
   //for(int i = 16; i < 21; i++){
     HxTbinsprop[1] -> SetBinContent(i, HxT -> GetBinContent(i));
     HxTbinsprop[1] -> SetBinError  (i, HxT -> GetBinError(i));
   }

   //3rd bin
   for(int i = 17; i < 22; i++){
   //for(int i = 21; i < 26; i++){
     HxTbinsprop[2] -> SetBinContent(i, HxT -> GetBinContent(i));
     HxTbinsprop[2] -> SetBinError  (i, HxT -> GetBinError(i));
   }

   //4th bin
   for(int i = 21; i < 51; i++){
   //for(int i = 26; i < 31; i++){
     HxTbinsprop[3] -> SetBinContent(i, HxT -> GetBinContent(i));
     HxTbinsprop[3] -> SetBinError  (i, HxT -> GetBinError(i));
   }

   for(int b = 0; b < 4; b++){
     exprop[b] = 0.0;
     xprop[b] = HxTbinsprop[b] -> GetMean();
     yprop[b] = HxTbinsprop[b] -> IntegralAndError(1,100,eyprop[b]);
     cout << "x,y,ex,ey " << xprop[b] << " " << yprop[b] << " " << exprop[b] << " " << eyprop[b] << endl;
     GxTprop -> SetPoint(b, xprop[b], yprop[b]);
     GxTprop -> SetPointError(b, exprop[b], eyprop[b]);
   }

   GxTprop -> GetXaxis() -> SetTitle("x_{T}");
   GxTprop -> GetYaxis() -> SetTitle("Rate [Hz]");   
   /*
   TCanvas *c2 = new TCanvas("c2","c2",1200,400);
   c2 -> Divide(2,1);
   c2 -> cd(1);
   Hdxdy -> Draw("colz");
   c2 -> cd(2);
   Hdr1 -> Draw();
   Hdr2 -> Draw("same");

   c2 -> cd();*/
   Hdxdy -> Draw("colz");
   sprintf(Hname, "./images/dxdy_maglength%d_magpos%d_magval%d.gif",maglength, magpos, magval);   
   c1 -> SaveAs(Hname);

   char outname[128];
   //sprintf(outname, "./root_files/tapered_%s.root", schema);
   sprintf(outname, "./root_files/dxdy_maglength%d_magpos%d_magval%d.root",maglength, magpos, magval);   
   TFile *outfile = new TFile(outname,"RECREATE");
   outfile -> cd();
   Hmass -> Write();
   Hpz -> Write();
   HxF -> Write();
   HxB -> Write();
   HxT -> Write();
   HxTprop -> Write();
   HxTxB -> Write();
   Hpzmass -> Write();
   Hdxdy -> Write();

   Hexcl -> Write();
   
   for(int b = 0; b < 20; b++){
     HxTbins[b] -> Write();
     if(b<4) HxTbinsprop[b] -> Write();
   }
   
   GxT -> Write("GxT");
   GxTprop -> Write("GxTprop");
   Zvtx -> Write();
   outfile -> Close();
   
}
