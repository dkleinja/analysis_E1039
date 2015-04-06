void getming(const char *schema)
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

   // Assign which seed to use (this chooses a database on the server)
   sprintf(seedstmt, "USE %s", schema);

   // Drop the table if it's already there for some reason
   // 	(DROP means 'delete')
   sprintf(delstmt, "DROP TABLE IF EXISTS data_ming");

   // Assemble the query based on the parameters given
   
   sprintf(stmt, // This first part tells it where to put the data you want
	   "CREATE TABLE data_ming\n" 
	   // The SELECT part lets you select what you want from which tables
	   //"SELECT hits.x, hits.y, dimuons.x1, dimuons.x2, hits.hitsID\n"
	   "SELECT mDimuon.eventID, mDimuon.mass, mDimuon.xB, mDimuon.xT, mDimuon.sigWeight, mTrack.particleID, mTrack.x0, mTrack.y0, mTrack.z0, mTrack.xf, mTrack.yf, mTrack.zf, mTrack.px0, mTrack.py0, mTrack.pz0, mTrack.pxf, mTrack.pyf, mTrack.pzf, mTrack.hitHodo1, mTrack.hitHodo2 "
	   // The FROM part puts together the source and how the tables
	   //   are related to one another
	   "FROM mDimuon, mTrack "

	   //"FROM mDimuon INNER JOIN mTrack ON "
	   //"(mDimuon.eventID=mTrack.eventID)) "
	   "WHERE (mDimuon.mtrackID1=mTrack.mtrackID OR mDimuon.mtrackID2=mTrack.mtrackID); ");
	   // The WHERE part lets you cut the data any way you want it
	   //"WHERE mDimuon.eventID = mTrack.eventID  AND mDimuon.eventID < 100;");


   

   //"SELECT mDimuon.eventID, mDimuon.xB, mDimuon.xT, mTrack.x0, mTrack.y0, mTrack.z0 FROM mDimuon, mTrack WHERE mDimuon.eventID = mTrack.eventID  AND mDimuon.eventID < 100;");
   // --- Connect to the database and execute the above assembled statements
   

   cout << stmt << endl;
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
   cout<<"blah"<<endl;
   TTreeSQL * treeql = new TTreeSQL(con, schema, "data_ming");
   cout<<"blah"<<endl;
   // Declare a TTreeSQL object corresponding to the table created
   // Since TTreeSQL is a child of TTree, you can do whatever you can
   // 	do with a TTree to these objects, including Scan() and Draw(),
   // 	but with added functionality.

   // Display the number of data points used (by counting enries
   // 	in the table created).
   cout << "Number of data points: " << treeql->GetEntries() << endl;
   /*
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
   */
}

// Please consult ROOT's class definitions of TTreeSQL and TSQLServer
//   or the MySQL website for any further informarion
//
// End of file

