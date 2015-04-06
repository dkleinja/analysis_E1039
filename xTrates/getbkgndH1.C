void getbkgndH1(const char *schema)
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

   char stmt1 [1024];
   char delstmt1 [64];
   char stmt2[1024];
   char delstmt2 [64];
   char stmt3 [1024];
   char delstmt3 [64];
   char seedstmt [64];
   char Hname[64];
   char outname[64];
   float xBmin = 0.01;
   float xBmax = 0.99;
   float xTmin = 0.01;
   float xTmax = 0.99;
   float mmin = 0;
   float mmax = 10;
   /*
  // You can tailor this so that the user inputs any conditions you want...
   //    and they will be used in 'stmt's assembly below to query the server
   cout << "Please enter the detector section (1-4) you would like to analyze: ";
   cin >> section;
   cout << "Please enter the minimum x1 value: ";
   cin >> xBmin;
   cout << "Please enter the maximum x1 value: ";
   cin >> xBmax;
   cout << "Please enter the minimum x2 value: ";
   cin >> xTmin;
   cout << "Please enter the maximum x2 value: ";
   cin >> xTmax;
   */

   // Assign which seed to use (this chooses a database on the server)
   sprintf(seedstmt, "USE %s", schema);
   cout<<"blah"<<endl;
   // Drop the table if it's already there for some reason
   // 	(DROP means 'delete')
   sprintf(delstmt1, "DROP TABLE IF EXISTS data_section");
   sprintf(delstmt1, "DROP TABLE IF EXISTS data_section1");
   sprintf(delstmt2, "DROP TABLE IF EXISTS data_section2");
   sprintf(delstmt3, "DROP TABLE IF EXISTS data_section3");
   cout<<"blah"<<endl;
   sprintf(stmt1, 
	   "CREATE TABLE data_section1\n" 
	   "SELECT mHit.elementID,mHit.hpx,mHit.hpy,mHit.hpz,mHit.particleID "
	   "FROM mHit "
	   //"WHERE mHit.detectorName RLIKE 'H1[TB];");
	   "WHERE mHit.detectorName RLIKE 'H1[TB]' AND ABS(mHit.particleID)=13;");

   sprintf(delstmt2, "DROP TABLE IF EXISTS data_section2");
   
   sprintf(stmt2, 
	   "CREATE TABLE data_section2\n" 
	   "SELECT mHit.elementID,mHit.hpx,mHit.hpy,mHit.hpz,mHit.particleID "
	   "FROM mHit "
	   //"WHERE mHit.detectorName RLIKE 'H2[TB]';");
	   "WHERE mHit.detectorName RLIKE 'H2[TB]' AND ABS(mHit.particleID)=13;");
   
   sprintf(delstmt3, "DROP TABLE IF EXISTS data_section3");
 
   // Assemble the query based on the parameters given
   sprintf(stmt3, // This first part tells it where to put the data you want
	   "CREATE TABLE data_section3\n" 
	   "SELECT mHit.elementID,mHit.hpx,mHit.hpy,mHit.hpz,mHit.particleID "
	   "FROM mHit "
	   //"WHERE mHit.detectorName RLIKE 'H3[TB]';");
	   "WHERE mHit.detectorName RLIKE 'H3[TB]' AND ABS(mHit.particleID)=13;");

    // --- Connect to the database and execute the above assembled statements  

   // This it used to time the SQL action of the macro
   gBenchmark->Start("SQL");

   // Connect to server
   TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306",
                "root", "");

   TTreeSQL * treeql_run = new TTreeSQL(con, schema, "mRun");
   Int_t nEvGen;
   Int_t nevgen;
   Float_t sigwt;
   nevgen = treeql_run -> GetEntries();
   cout << "Number of buckets used " << nevgen << endl;


   // Select which database to use on the server
   con->Exec(seedstmt);

   // Delete the table if it already exitst
   con->Exec(delstmt1);
   con->Exec(delstmt2);
   con->Exec(delstmt3);

   // Create a table based on the exact cut of data you want
   con->Exec(stmt1);
   cout << stmt1 << endl;
   con->Exec(stmt2);
   cout << stmt2 << endl;
   con->Exec(stmt3);
   cout << stmt3 << endl;
   // Display the time it took
   gBenchmark->Show("SQL");
   // Declare a TTreeSQL object corresponding to the table created
   // Since TTreeSQL is a child of TTree, you can do whatever you can
   // 	do with a TTree to these objects, including Scan() and Draw(),
   // 	but with added functionality.
   TTreeSQL * treeql1 = new TTreeSQL(con, schema,"data_section1");
   TTreeSQL * treeql2 = new TTreeSQL(con, schema,"data_section2");
   TTreeSQL * treeql3 = new TTreeSQL(con, schema,"data_section3");

   // Display the number of data points used (by counting enries
   // 	in the table created).
   cout << "Number of data points: " << treeql1->GetEntries() << endl;

   TH1F *H1hits = new TH1F("H1hits","Bkgnd Hits in H1T, H1B",24,1,24);
   H1hits -> SetXTitle("ElementID");
   H1hits -> SetYTitle("Yield/Bucket");
   H1hits -> Sumw2();
   TH1F *H1hits_tr = new TH1F("H1hits_tr","Bkgnd Hits in H1T, H1B, p > 1",24,1,24);
   H1hits_tr -> SetXTitle("ElementID");
   H1hits_tr -> SetYTitle("Yield/Bucket");
   H1hits_tr -> Sumw2();
 
   TH1F *H2hits = new TH1F("H2hits","Bkgnd Hits in H2T, H2B",8,1,16);
   H2hits -> SetXTitle("ElementID");
   H2hits -> SetYTitle("Yield/Bucket");
   H2hits -> Sumw2();
   TH1F *H2hits_tr = new TH1F("H2hits_tr","Bkgnd Hits in H2T, H2B, p > 1",8,1,16);
   H2hits_tr -> SetXTitle("ElementID");
   H2hits_tr -> SetYTitle("Yield/Bucket");
   H2hits_tr -> Sumw2();
 
   TH1F *H3hits = new TH1F("H3hits","Bkgnd Hits in H3T, H3B",8,1,16);
   H3hits -> SetXTitle("ElementID");
   H3hits -> SetYTitle("Yield/Bucket");
   H3hits -> Sumw2();
   TH1F *H3hits_tr = new TH1F("H3hits_tr","Bkgnd Hits in H3T, H3B, p > 1",8,1,16);
   H3hits_tr -> SetXTitle("ElementID");
   H3hits_tr -> SetYTitle("Yield/Bucket");
   H3hits_tr -> Sumw2();
 
   treeql1 -> Draw("elementID>>H1hits");
   treeql1 -> Draw("elementID>>H1hits_tr","sqrt(pow(hpx,2)+pow(hpy,2)+pow(hpz,2))>1");
 
   treeql2 -> Draw("elementID>>H2hits");
   treeql2 -> Draw("elementID>>H2hits_tr","sqrt(pow(hpx,2)+pow(hpy,2)+pow(hpz,2))>1");
   
   treeql3 -> Draw("elementID>>H3hits");
   treeql3 -> Draw("elementID>>H3hits_tr","sqrt(pow(hpx,2)+pow(hpy,2)+pow(hpz,2))>1");
     

   H1hits -> Scale(1.0/nevgen);
   H1hits_tr -> Scale(1.0/nevgen);
   H2hits -> Scale(1.0/nevgen);
   H2hits_tr -> Scale(1.0/nevgen);
   H3hits -> Scale(1.0/nevgen);
   H3hits_tr -> Scale(1.0/nevgen);

   sprintf(outname, "./root_files/bkgndH1_%s.root", schema);
   TFile *outfile = new TFile(outname, "RECREATE");
   outfile -> cd();
   H1hits -> Write();
   H1hits_tr -> Write();
   H2hits -> Write();
   H2hits_tr -> Write();
   H3hits -> Write();
   H3hits_tr -> Write();
   outfile -> Write();
   outfile -> Close();
   
   /*
   // Take out the garbage on your way out, and delete the created table.
   delete treeql1;
   con->Exec("DROP TABLE IF EXISTS data_section1, data_section2, "
   		"data_section3, data_section4");

   // Disconnect from the server
   delete con;
   */
}

// Please consult ROOT's class definitions of TTreeSQL and TSQLServer
//   or the MySQL website for any further informarion
//
// End of file

