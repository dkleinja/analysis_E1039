void ptkick(const char *schema, const int Section, const int mcut = 0)
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

   char stmt [1024];
   char delstmt [64];
   char seedstmt [64];
   char Hname[64];
   char outname[64];
   int section = Section;
   float xBmin = 0.01;
   float xBmax = 0.99;
   float xTmin = 0.01;
   float xTmax = 0.99;
   float mmin = 0;
   float mmax = 10;
   if(mcut == 1){mmin = 4.5; mmax = 9;}
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

   // Drop the table if it's already there for some reason
   // 	(DROP means 'delete')
   sprintf(delstmt, "DROP TABLE IF EXISTS data_section%i", section);

   // Assemble the query based on the parameters given
   if(section !=3){
     sprintf(stmt, // This first part tells it where to put the data you want
	     "CREATE TABLE data_section%i\n" 
	     // The SELECT part lets you select what you want from which tables
	     //"SELECT hits.x, hits.y, dimuons.x1, dimuons.x2, hits.hitsID\n"
	     "SELECT mTrack.px0, mHit.hpx "
	     // The FROM part puts together the source and how the tables
	     //   are related to one another
	     "FROM mTrack, mHit "

	     "WHERE ((mTrack.mtrackID=mHit.mtrackID) "

	     //"AND ((mTrack.xB)>%f) AND ((mTrack.xB)<%f) AND ((mTrack.xT)>%f) AND ((mTrack.xT)<%f) "
	     "AND (((mHit.particleID)=13)) "
	     "AND (((mHit.detectorName) ='D%iX')) "
	     //"OR   ((mHit.detectorName) ='D%iXp') " 
	     //"OR   ((mHit.detectorName) ='D%iU') "
	     //"OR   ((mHit.detectorName) ='D%iUp') "
	     //"OR   ((mHit.detectorName) ='D%iV') "
	     //"OR   ((mHit.detectorName) ='D%iVp')) "
	     "AND mHit.eventID < 20000);"

	     , //section, xBmin, xBmax, xTmin, xTmax, 
	     section, section, section, section, section, section, 
	     section, section, section, section, section, section, 
	     section, section, section, section, section, section);
   } 
   
   // The WHERE part lets you cut the data any way you want it
   //"WHERE mTrack.eventID = mTrack.eventID  AND mTrack.eventID < 100;");


   

   //"SELECT mTrack.eventID, mTrack.xB, mTrack.xT, mTrack.x0, mTrack.y0, mTrack.z0 FROM mTrack, mTrack WHERE mTrack.eventID = mTrack.eventID  AND mTrack.eventID < 100;");
   // --- Connect to the database and execute the above assembled statements
   
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
   // Declare a TTreeSQL object corresponding to the table created
   // Since TTreeSQL is a child of TTree, you can do whatever you can
   // 	do with a TTree to these objects, including Scan() and Draw(),
   // 	but with added functionality.
   switch (section){
        case 1:
                TTreeSQL * treeql = new TTreeSQL(con, schema,
                                "data_section1");
                break;
        case 2:
                TTreeSQL * treeql = new TTreeSQL(con, schema,
                                "data_section2");
                break;
        case 3:
                TTreeSQL * treeql = new TTreeSQL(con, schema,
                                "data_section3");
                break;
        case 4:
                TTreeSQL * treeql = new TTreeSQL(con, schema,
                                "data_section4");
                break;
        default:
   }

   // Display the number of data points used (by counting enries
   // 	in the table created).
   cout << "Number of data points: " << treeql->GetEntries() << endl;

   Float_t px0, hpx,dpx;
   treeql -> SetBranchAddress("px0", &px0);
   treeql -> SetBranchAddress("hpx", &hpx);
     cout <<"blah " << endl;
   TH1F *Dpx = new TH1F("Dpx","#Delta p_{x}/p_{x,0} in Station 1",200,-5,5);
   Dpx -> SetXTitle("#Delta p_{x}/p_{x,0}");
   Dpx -> SetYTitle("Yield");
   /*  for(int i = 0; i < treeql->GetEntries(); i++){
     cout <<"blah " << i << endl;
     //if(i%100 == 0) cout < "Entry " << i << endl;
     dpx = (hpx - px0) / px0;
     cout <<"blah " << i << " " << hpx << " " << px0 << " " << dpx << endl;
     Dpx -> Fill(dpx);
   }*/
   cout <<"blah " << endl;
   treeql -> Draw("((hpx - px0))>>Dpx");
   
   sprintf(outname, "./root_files/ptkick%i_masscut%i_%s.root", section, mcut, schema);
   TFile *outfile = new TFile(outname, "RECREATE");
   outfile -> cd();
   Dpx -> Write();
   outfile -> Write();
   outfile -> Close();
   
   /*
   // Take out the garbage on your way out, and delete the created table.
   delete treeql;
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

