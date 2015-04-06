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
   int seed;
   int section;
   float x1min;
   float x1max;
   float x2min;
   float x2max;
   

   // You can tailor this so that the user inputs any conditions you want...
   //    and they will be used in 'stmt's assembly below to query the server
   cout << "Please enter the seed of data that you would like to load: ";
   cin >> seed;
   cout << "Please enter the detector section (1-4) you would like to analyze: ";
   cin >> section;
   cout << "Please enter the minimum x1 value: ";
   cin >> x1min;
   cout << "Please enter the maximum x1 value: ";
   cin >> x1max;
   cout << "Please enter the minimum x2 value: ";
   cin >> x2min;
   cout << "Please enter the maximum x2 value: ";
   cin >> x2max;


   // Assign which seed to use (this chooses a database on the server)
   sprintf(seedstmt, "USE test_dkleinja_Fe906", seed);

   // Drop the table if it's already there for some reason
   // 	(DROP means 'delete')
   sprintf(delstmt, "DROP TABLE IF EXISTS data_section%i", section);

   // Assemble the query based on the parameters given

   sprintf(stmt, // This first part tells it where to put the data you want
   		"CREATE TABLE data_section%i\n" 
		 // The SELECT part lets you select what you want from which tables
		"SELECT mHit.hx, mHit.hy, mDimuon.x1, mDimuon.x2, mHit.hitID\n"
		 // The FROM part puts together the source and how the tables
		 //   are related to one another
		"FROM hits INNER JOIN (tracks INNER JOIN dimuons ON "
		"(tracks.eventID=dimuons.dimuonID)) ON (hits.u_trackID"
		"=tracks.u_trackID)\n"
		 // The WHERE part lets you cut the data any way you want it
		"WHERE (((dimuons.x1)>%f) AND ((dimuons.x1)<%f) AND "
		"((dimuons.x2)>%f) AND ((dimuons.x2)<%f) AND "
		"(((tracks.particleID)=13) OR ((tracks.particleID)=-13)) "
		"AND (((hits.detectorName)='H%ix') OR ((hits.detectorName)"
		"='H%iy') OR ((hits.detectorName)='H%iy1') OR "
		"((hits.detectorName)='H%iy2')))", section, x1min, x1max, 
		x2min, x2max, section, section, section, section);
 
   // --- Connect to the database and execute the above assembled statements

   // This it used to time the SQL action of the macro
   gBenchmark->Start("SQL");

   // Connect to server
   TSQLServer *con = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283",
                "seaguest", "qqbar2mu+mu-");
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
      		TTreeSQL * treeql = new TTreeSQL(con, "test_dkleinja_Fe906", 
				"data_section1");
		break;
   	case 2: 
      		TTreeSQL * treeql = new TTreeSQL(con, "test_dkleinja_Fe906", 
				"data_section2");
		break;
   	case 3: 
   		TTreeSQL * treeql = new TTreeSQL(con, "test_dkleinja_Fe906", 
				"data_section3");
		break;
   	case 4: 
     		TTreeSQL * treeql = new TTreeSQL(con, "test_dkleinja_Fe906", 
				"data_section4");
		break;
   	default:
   }

   // Display the number of data points used (by counting enries
   // 	in the table created).
   cout << "Number of data points: " << treeql->GetEntries() << endl;
   
   Int_t cancolor = 17;

   // Create the histograms of x1 and x2
   TCanvas xhist("xhist","x1 and x2 Histograms",10,10,800,525); 
   xhist.Divide(1,2);
   xhist.SetFillColor(cancolor);
   xhist.cd(1);
   treeql->Draw("x1");
   xhist.cd(2);
   treeql->Draw("x2");
   xhist.Update();

   

   // Display various scatterplots of (x,y) for the selected criteria
   TCanvas scatter("scatter","x:y Scatterplot",10,10,800,525); 
   scatter.Divide(2,2);
   scatter.SetFillColor(cancolor);
   scatter.cd(1);
   treeql->Draw("x:y","","SCAT");
   scatter.cd(2);
   treeql->Draw("x:y","","BOX");
   scatter.cd(3);
   treeql->Draw("x:y","","ARR");
   scatter.cd(4);
   treeql->Draw("x:y","","COLZ");
   scatter.Update();
 
   // Display contour plots for (x,y)
   TCanvas contour("contour","x:y Contour Plots",100,100,800,525);
   contour.Divide(2,2);
   gPad->SetGrid();
   contour.SetFillColor(cancolor);
   contour.cd(1);
   treeql->Draw("x:y","","CONTZ");
   contour.cd(2);
   gPad->SetGrid();
   treeql->Draw("x:y","","CONT1");
   contour.cd(3);
   gPad->SetGrid();
   treeql->Draw("x:y","","CONT2");
   contour.cd(4);
   gPad->SetGrid();
   treeql->Draw("x:y","","CONT3");
   contour.Update();

         
   // Display the surface plots of (x,y)
   TCanvas surface("surface","x:y Surface Plots",200,200,800,525);
   surface.Divide(2,2);
   surface.SetFillColor(cancolor);
   surface.cd(1);
   treeql->Draw("x:y","","SURF1");
   surface.cd(2);
   treeql->Draw("x:y","","SURF2Z");
   surface.cd(3);
   treeql->Draw("x:y","","SURF3");
   surface.cd(4);
   treeql->Draw("x:y","","SURF4");
   surface.Update();


   // Take out the garbage on your way out, and delete the created table.
   delete treeql;
   con->Exec("DROP TABLE IF EXISTS data_section1, data_section2, "
   		"data_section3, data_section4");

   // Disconnect from the server
   delete con;
  
}

// Please consult ROOT's class definitions of TTreeSQL and TSQLServer
//   or the MySQL website for any further informarion
//
// End of file

