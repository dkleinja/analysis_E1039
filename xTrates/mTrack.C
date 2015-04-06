void quick(const char *schema, const char *table)
{
   //connect to server
   TSQLServer *con = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283", 
   		"seaguest", "qqbar2mu+mu-");

   //con -> GetTables("test_dkleinja_Le906");

   //TTreeSQL * treeql = new TTreeSQL(con, "test_dkleinja_Fe906", "mDimuon");
   //TTreeSQL * treeql = new TTreeSQL(con, "test_dkleinja_Fe906", "mHit");
   //TTreeSQL * treeql = new TTreeSQL(con, "test_dkleinja_Fe906", "mTrack");
   TTreeSQL * treeql = new TTreeSQL(con, schema, table);
   /*
  treeql -> SetBranchAddress ("runID",      &runID);
  treeql -> SetBranchAddress ("eventID",    &eventID);
  treeql -> SetBranchAddress ("spillID",    &spillID);
  treeql -> SetBranchAddress ("parentID", &particleID);
  treeql -> SetBranchAddress ("particleID", &particleID);
  treeql -> SetBranchAddress ("x0",         &x0);
  treeql -> SetBranchAddress ("y0",       &y0);
  treeql -> SetBranchAddress ("z0",       &z0);
  treeql -> SetBranchAddress ("xf",       &xf);
  treeql -> SetBranchAddress ("yf",       &yf);
  treeql -> SetBranchAddress ("zf",       &zf);
  treeql -> SetBranchAddress ("px0",       &px0);
  treeql -> SetBranchAddress ("py0",       &py0);
  treeql -> SetBranchAddress ("pz0",       &pz0);
  treeql -> SetBranchAddress ("pxf",       &pxf);
  treeql -> SetBranchAddress ("pyf",       &pyf);
  treeql -> SetBranchAddress ("pzf",       &pzf);
   */




for(int i = 0; i < treeql -> GetEntries(); i++){



}

/*
   TFile *outfile = new TFile("rootfile.root","RECREATE");
	outfile -> cd();
	treeql -> Write();
	outfile -> Close();
*/
}
