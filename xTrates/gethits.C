void gethits(const char *schema, const int Section, const int mcut = 0)
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
   gStyle->SetCanvasColor(33);
   gStyle->SetFrameFillColor(18);

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
	     "SELECT mDimuon.xB, mDimuon.xT, mDimuon.mass, mHit.hx, mHit.hy, mHit.hz "
	     // The FROM part puts together the source and how the tables
	     //   are related to one another
	     "FROM mDimuon, mHit "

	     "WHERE ((mDimuon.mtrackID1=mHit.mtrackID OR mDimuon.mtrackID2=mHit.mtrackID) "

	     //"AND ((mDimuon.xB)>%f) AND ((mDimuon.xB)<%f) AND ((mDimuon.xT)>%f) AND ((mDimuon.xT)<%f) "
	     "AND (((mHit.particleID)=13) OR ((mHit.particleID)=-13)) "
	     "AND (((mHit.detectorName) ='D%iX') "
	     "OR   ((mHit.detectorName) ='D%iXp') " 
	     "OR   ((mHit.detectorName) ='D%iU') "
	     "OR   ((mHit.detectorName) ='D%iUp') "
	     "OR   ((mHit.detectorName) ='D%iV') "
	     "OR   ((mHit.detectorName) ='D%iVp')); "
	     "AND mHit.eventID < 20);"

	     , //section, xBmin, xBmax, xTmin, xTmax, 
	     section, section, section, section, section, section, 
	     section, section, section, section, section, section, 
	     section, section, section, section, section, section);
   } 
   
   else if(section == 3){
     sprintf(stmt, // This first part tells it where to put the data you want
	     "CREATE TABLE data_section%i\n" 
	     // The SELECT part lets you select what you want from which tables
	     //"SELECT hits.x, hits.y, dimuons.x1, dimuons.x2, hits.hitsID\n"
	     "SELECT mDimuon.mass, mDimuon.xB, mDimuon.xT, mHit.hx, mHit.hy, mHit.hz "
	     // The FROM part puts together the source and how the tables
	     //   are related to one another
	     "FROM mDimuon, mHit "

	     "WHERE ((mDimuon.mtrackID1=mHit.mtrackID OR mDimuon.mtrackID2=mHit.mtrackID) "
	     //"AND ((mDimuon.mass)>4) AND ((mDimuon.mass)<8)"
	     //"AND ((mDimuon.xB)>%f) AND ((mDimuon.xB)<%f) AND ((mDimuon.xT)>%f) AND ((mDimuon.xT)<%f) "
	     "AND (((mHit.particleID)=13) OR ((mHit.particleID)=-13)) "
	     "AND  (((mHit.detectorName) ='D%imX') "
	     "OR   ((mHit.detectorName) ='D%imXp') " 
	     "OR   ((mHit.detectorName) ='D%imU') "
	     "OR   ((mHit.detectorName) ='D%imUp') "
	     "OR   ((mHit.detectorName) ='D%imV') "
	     "OR   ((mHit.detectorName) ='D%imVp') "
	     "OR   ((mHit.detectorName) ='D%ipX') "
	     "OR   ((mHit.detectorName) ='D%ipXp') " 
	     "OR   ((mHit.detectorName) ='D%ipU') "
	     "OR   ((mHit.detectorName) ='D%ipUp') "
	     "OR   ((mHit.detectorName) ='D%ipV') "
	     "OR   ((mHit.detectorName) ='D%ipVp'))); "
	     //"AND mHit.eventID < 20);"

	     , //section, xBmin, xBmax, xTmin, xTmax, 
	     section, section, section, section, section, section, 
	     section, section, section, section, section, section, 
	     section, section, section, section, section, section);
   }
   
   // The WHERE part lets you cut the data any way you want it
   //"WHERE mDimuon.eventID = mTrack.eventID  AND mDimuon.eventID < 100;");


   

   //"SELECT mDimuon.eventID, mDimuon.xB, mDimuon.xT, mTrack.x0, mTrack.y0, mTrack.z0 FROM mDimuon, mTrack WHERE mDimuon.eventID = mTrack.eventID  AND mDimuon.eventID < 100;");
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
   
   Float_t xB, xT, hx, hy;

   TH1F *hxB = new TH1F("hxB","Beam momentum fraction, xB",100,0.,1.);
   TH1F *hxT = new TH1F("hxT","Target momentum fraction, xB",100,0.,1.);

   Int_t dbinsx, dbinsy;
   Float_t driftx, drifty;
   if(section == 1)driftx = 60, drifty = 70, dbinsx = 120, dbinsy = 140;
   else if(section == 2)driftx = 120, drifty = 160, dbinsx = 240, dbinsy = 320;
   else if(section == 3)driftx = 160, drifty = 120, dbinsx = 320, dbinsy = 240;

   sprintf(Hname,"DC%i Hits", section);
   TH2F *hdrift = new TH2F("hdrift",Hname, dbinsx, (-1)*driftx, driftx, dbinsy, (-1)*drifty, drifty);
   //,  -60,  60, 140,  -70,  70);
   //TH2F *hdrift2 = new TH2F("hdrift2","DC2 hits", 240, -120, 120, 320, -160, 160);
   //TH2F *hdrift3 = new TH2F("hdrift3","DC3 hits", 320, -160, 160, 240, -120, 120);

   hxB -> SetXTitle("x");
   hxB -> SetXTitle("x");
   hdrift -> SetXTitle ("x (cm)");
   hdrift -> SetYTitle ("y (cm)");

   /*
   hxB -> Fill(xB);
   hxT -> Fill(xF);
   hdrift1 -> Fill(hx, hy);
   hdrift2 -> Fill(hx, hy);
   hdrift3 -> Fill(hx, hy);
*/

   Int_t cancolor = 17;

   TCanvas *c1 = new TCanvas("cdrift","drift_chamber",600,400);
   c1 ->cd();
   treeql -> Draw("hy:hx","mass>4.5 && mass<10","CONTZ");
   sprintf(Hname,"./images/drift%i_masscut%i_%s.gif", section, mcut, schema);
   c1 -> SaveAs(Hname);


   // Create the histograms of x1 and x2
   TCanvas *xhist = new TCanvas("xhist","x1 and x2 Histograms",10,10,800,525); 
   xhist->Divide(1,2);
   xhist->SetFillColor(cancolor);
   xhist->cd(1);
   treeql->Draw("xB>>hxB","mass>4.5 && mass<10");
   xhist->cd(2);
   treeql->Draw("xT>>hxT","mass>4.5 && mass<10");
   xhist->Update();


   // Display various scatterplots of (x,y) for the selected criteria
   TCanvas *scatter = new TCanvas("scatter","x:y Scatterplot",10,10,800,525); 
   scatter->Divide(2,2);
   scatter->SetFillColor(cancolor);
   scatter->cd(1);
   treeql->Draw("hx:hy","","SCAT");
   scatter->cd(2);
   treeql->Draw("hx:hy","","BOX");
   scatter->cd(3);
   treeql->Draw("hx:hy","","ARR");
   scatter->cd(4);
   treeql->Draw("hx:hy>>hdrift","","COLZ");
   scatter->Update();
 
   // Display contour plots for (x,y)
   TCanvas *contour = new TCanvas("contour","x:y Contour Plots",100,100,800,525);
   contour->Divide(2,2);
   gPad->SetGrid();
   contour->SetFillColor(cancolor);
   contour->cd(1);
   treeql->Draw("hx:hy","","CONTZ");
   contour->cd(2);
   gPad->SetGrid();
   treeql->Draw("hx:hy","","CONT1");
   contour->cd(3);
   gPad->SetGrid();
   treeql->Draw("hx:hy","","CONT2");
   contour->cd(4);
   gPad->SetGrid();
   treeql->Draw("hx:hy","","CONT3");
   contour->Update();

         
   // Display the surface plots of (x,y)
   TCanvas *surface = new TCanvas("surface","x:y Surface Plots",200,200,800,525);
   surface->Divide(2,2);
   surface->SetFillColor(cancolor);
   surface->cd(1);
   treeql->Draw("hx:hy","","SURF1");
   surface->cd(2);
   treeql->Draw("hx:hy","","SURF2Z");
   surface->cd(3);
   treeql->Draw("hx:hy","","SURF3");
   surface->cd(4);
   treeql->Draw("hx:hy","","SURF4");
   surface->Update();
   
   sprintf(outname, "./root_files/drift%i_masscut%i_%s.root", section, mcut, schema);
   TFile *outfile = new TFile(outname, "RECREATE");
   outfile -> cd();
   hxT -> Write();
   hxB -> Write();
   hdrift -> Write();
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

