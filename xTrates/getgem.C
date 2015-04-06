void getgem(const char *schema, const float maglength = 100, const float magpos = 150)
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

   TH1F *Hdr = new TH1F("Hdr","Dimuon #DeltaR from calculation",50,0,50);
   Hdr -> SetLineColor(2);

   int gempos = (-1*magpos + maglength/2) + 90;
   sprintf(Hname, "GemPos [z_{Gem}= %d cm], z_{magleng}=%d cm, z_{pos}=%1.0f cm, B=%1.0f Tesla ", gempos, maglength, -1*magpos);
   //cout << magval << endl;exit(1);
   TH2F *Hdxdy = new TH2F("Hdxdy", Hname, 100, -50, 50, 100, -50, 50);
   //TH2F *Hdxdy = new TH2F("Hdxdy", "#Delta x vs #Delta y at -50 cm", 40, -20, 20, 25, -20, 20);



   Hdr -> SetXTitle("#Deltar [cm]");
   Hdxdy -> SetXTitle("#Deltax [cm]");
   Hdxdy -> SetYTitle("#Deltay [cm]");
 
   char stmt [1024];
   char delstmt [64];
   char seedstmt [64];


   // Assign which seed to use (this chooses a database on the server)
   sprintf(seedstmt, "USE %s", schema);

   // Drop the table if it's already there for some reason
   //   (DROP means 'delete')
   sprintf(delstmt, "DROP TABLE IF EXISTS mTrackTable");

   // Assemble the query based on the parameters given
   sprintf(stmt, // This first part tells it where to put the data you want
	   "CREATE TABLE mTrackTable\n"
	   "SELECT mTrackID,particleID,px0,py0,pz0,x0,y0,z0,zf "
	   "FROM mTrack "
	   //"WHERE particleID!=2212 AND z0<%d;", gempos);
	   "WHERE particleID!=2212 AND z0<%d AND zf>%d;", gempos,gempos+10);
   cout << stmt << endl;
   //exit(1);
   // This it used to time the SQL action of the macro
   gBenchmark->Start("SQL");

   // Connect to server
   TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306","root", "");
   //TSQLServer *con = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283", "seaguest", "qqbar2mu+mu-");

   // Select which database to use on the server
   con->Exec(seedstmt);

   // Delete the table if it already exitst
   con->Exec(delstmt);

   // Create a table based on the exact cut of data you want
   con->Exec(stmt);

   // Display the time it took
   gBenchmark->Show("SQL");

   TTreeSQL * treeql_mTrack = new TTreeSQL(con, schema, "mTrackTable");

   TTreeSQL * treeql_run = new TTreeSQL(con, schema, "mRun");
   Int_t nEvGen;
   Int_t nevgen;
   Float_t sigwt;
   treeql_run -> SetBranchAddress("nEvGen", &nEvGen);
   for(int i = 0; i < 1; i++){
     treeql_run -> GetEntry(i);
     nevgen = nEvGen;
   }
   nevgen = treeql_run -> GetEntries();
   cout << nevgen << endl;

   Int_t mTrackID, particleID; 
   Float_t x0, y0, z0;
   Float_t px0, py0, pz0;
   Float_t deltax0, deltax1;
   Float_t deltax, deltay, deltar;
   Float_t ptkickx;
   Float_t charge, tesla;
   tesla = 0.0;

   if(schema == "e906db1_gun40M_tarpos350_magpos150_magvalm1"); tesla = -1;
   //if(schema == "e906db1_gun40M_tarpos350_magpos150_magvalm1"); tesla = -1;
   
   treeql_mTrack -> SetBranchAddress ("mTrackID",    &mTrackID);
   treeql_mTrack -> SetBranchAddress ("particleID",    &particleID);
   treeql_mTrack -> SetBranchAddress ("x0",    &x0);
   treeql_mTrack -> SetBranchAddress ("y0",    &y0);
   treeql_mTrack -> SetBranchAddress ("z0",    &z0);
   treeql_mTrack -> SetBranchAddress ("px0",    &px0);
   treeql_mTrack -> SetBranchAddress ("py0",    &py0);
   treeql_mTrack -> SetBranchAddress ("pz0",    &pz0);

   int nentries2 = treeql_mTrack -> GetEntries();
   cout << "The number of mTrack Entries is " << nentries2 << endl;
   for(int i = 0; i < nentries2; i++){
     treeql_mTrack -> GetEntry(i);
     if(i%1000 == 0) cout << "EventID " << i << endl;
     
     //check the mTrack positions       
     deltay = (py0 / pz0) * ( -1*fabs(magpos - maglength/2.0) -  z0);
     ptkickx = -0.3 * charge * tesla;
     //cout << i << " charge, tesla, particleID, ptkick " << charge << " " << tesla <<  " " << ptkickx << endl;
     deltax0 = (px0 / pz0) * ( -1*magpos - z0);
     deltax1 = ((px0 + ptkickx) / pz0) * fabs(maglength/2.0);
     deltax = deltax0 + deltax1;
     //cout << "total distance " <<  -1*fabs(magpos - maglength/2.0) -  dz << " first distance " << -1*magpos -dz << " .  Last distance " << fabs(maglength/2) << endl;
     //cout << "Delta x after " << deltax << endl;
     
     //if(schema == "gun10M_tarpos130_magpos0_magval0")deltax = (px0 / pz0) * ( fabs(dz));

     deltar = sqrt ( pow(deltax,2) + pow(deltay,2));
     
     Hdxdy -> Fill(deltax,deltay);
     Hdr -> Fill(deltar);
     
   }

   //scale by appropriate number of events
   Hdxdy -> Scale(2e12/(nevgen*40000));

   Hdxdy -> Draw("colz");
   sprintf(Hname, "./images/gundxdxy_%s_maglength%d_magpos%d.gif",schema,maglength, magpos);  
   c1 -> SaveAs(Hname);

   char outname[128];
   //sprintf(outname, "./root_files/tapered_%s.root", schema);
   sprintf(outname, "./root_files/gundxdy_%s_maglength%d_magpos%d.root",schema, maglength, magpos);   
   TFile *outfile = new TFile(outname,"RECREATE");
   outfile -> cd();
   Hdxdy -> Write();

   outfile -> Close();

}
