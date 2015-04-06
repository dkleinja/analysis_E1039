void dummy(const char *schema)
{
   // EXAMPLE USERCODE FOR ANALYZING SQL DATA


   // This macro displays histograms of x1 and x2, and then scatterplots, 
   // 	contour plots, and surface plots of (x,y) for different sections 
   // 	given different ranges of x1 and x2.
   //
   // Directions: Type in .x SQLCanvas.C while in ROOT on an NPL machine
   // 	to run this macro.
   //
   // Author: Bryan Dannowitz
   //

   gROOT->Reset();
   gBenchmark->Reset();
   gStyle->SetOptStat(0);
   gStyle->SetPalette(1);
   //gStyle->SetCanvasColor(33);
   //gStyle->SetFrameFillColor(18);

   char Hname[64];
   char outname[64];
    char stmt [1024];
   char delstmt [64];
   char seedstmt [64];

 // Assign which seed to use (this chooses a database on the server)
   sprintf(seedstmt, "USE %s", schema);

   // Drop the table if it's already there for some reason
   //   (DROP means 'delete')
   sprintf(delstmt, "DROP TABLE IF EXISTS mTrackTable2");
   
   // Assemble the query based on the parameters given
   sprintf(stmt, // This first part tells it where to put the data you want
	   "CREATE TABLE mTrackTable2\n"
	   "SELECT mTrack.eventID,mTrack.mTrackID,mTrack.particleID,mTrack.px0,mTrack.py0,mTrack.pz0 "
	   // The FROM part puts together the source and how the tables
	   //   are related to one another
	   "FROM mTrack,mDimuon "
	   "WHERE ((mTrack.mTrackID = mDimuon.mTrackID1 OR mTrack.mTrackID = mDimuon.mTrackID2))");
	   //"AND mTrack.eventID < 1000); ");
/*	   
   // Assemble the query based on the parameters given
   sprintf(stmt, // This first part tells it where to put the data you want
           "CREATE TABLE mTrackTable2\n"
           "SELECT mHit.elementID "
           "FROM mHit "
           "WHERE mHit.detectorName RLIKE 'H1[TB]'");

 */ 
   cout << stmt << endl;
   // This it used to time the SQL action of the macro
   gBenchmark->Start("SQL");

   // Connect to server
   TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306",
                "root", "");

   // Select which database to use on the server
   con->Exec(seedstmt);

   // Delete the table if it already exitst
   con->Exec(delstmt);

   // Create a table based on the exact cut of data you want
   con->Exec(stmt);

   // Display the time it took
   gBenchmark->Show("SQL");

   TTreeSQL * treeql = new TTreeSQL(con, schema,
				    "mTrackTable2");
   cout << "Number of data points: " << treeql->GetEntries() << endl;
/*
   treeql->Draw("mTrackID");

   TTreeSQL * treeql_mTrack = new TTreeSQL(con, schema, "mTrack");
   
   UInt_t mTrackID; 
   Int_t particleID; 
   Float_t px0, py0, pz0;
   //treeql_mTrack -> SetBranchAddress ("mTrackID",    &mTrackID);
   //treeql_mTrack -> SetBranchAddress ("particleID",    &particleID);
   treeql_mTrack -> SetBranchAddress ("px0",    &px0);
   treeql_mTrack -> SetBranchAddress ("py0",    &py0);
   treeql_mTrack -> SetBranchAddress ("pz0",    &pz0);

   int nentries2 = treeql_mTrack -> GetEntries();
   cout << "The number of mTrack Entries is " << nentries2 << endl;
   for(int j = 0; j < nentries2; j++){
     //cout << j << "blah"<< endl;
     treeql -> GetEntry(j);
     //cout << j << "blah"<< endl;
     //cout << j << " " << mTrackID << endl;
   }
  */ 
}
