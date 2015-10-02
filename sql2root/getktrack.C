void getktrack(const char *schema, const int val)
{

  char stmt [1024];
  char delstmt [64];
  char seedstmt [64];
  char table[64];
  int range = 1000000;
  int limit = val*range;

  sprintf(table, "kTrack_%d", val);

  // Assign which seed to use (this chooses a database on the server)
  sprintf(seedstmt, "USE %s", schema);

   // Drop the table if it's already there for some reason
   sprintf(delstmt, "DROP TABLE IF EXISTS kTrack_%d", val);

   sprintf(stmt, // This first part tells it where to put the data you want
	   "CREATE TABLE kTrack_%d "
	   "SELECT kTrack.* FROM kTrack "
	   "LIMIT %d, %d;",val, limit, range);
   //exit(1);

   cout << stmt << endl;
   // This it used to time the SQL action of the macro
   gBenchmark->Start("SQL");

   //connect to server
   //TSQLServer *con = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283", "seaguest", "qqbar2mu+mu-");
   TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306", "root", "");

   // Select which database to use on the server
   con->Exec(seedstmt);

   // Delete the table if it already exitst
   con->Exec(delstmt);

   // Create a table based on the exact cut of data you want
   con->Exec(stmt);

   // Display the time it took
   gBenchmark->Show("SQL");

   TTreeSQL * treeql = new TTreeSQL(con, schema, table);


   Int_t trackID, runID, spillID, eventID, charge, roadID;
   Int_t numHits, numHitsSt1, numHitsSt2, numHitsSt3, numHitsSt4H, numHitsSt4V;
   Float_t chisq;
   Float_t x0, y0, z0;
   Float_t x_target, y_target, z_target;
   Float_t x_dump, y_dump, z_dump;
   Float_t px0, py0, pz0;
   Float_t x1, y1, z1;
   Float_t px1, py1, pz1;
   Float_t x3, y3, z3;
   Float_t px3, py3, pz3;
   Float_t tx_PT, ty_PT;
   UInt_t target, dump, targetPos;


  TTree *ktrack = new TTree("ktrack","ktrack");
  ktrack -> SetDirectory(0);
  ktrack -> Branch ("runID",     &runID,     "runID/I");
  ktrack -> Branch ("spillID",   &spillID,   "spillID/I");
  ktrack -> Branch ("eventID",   &eventID,   "eventID/I");
  ktrack -> Branch ("charge",    &charge,    "charge/I");
  ktrack -> Branch ("roadID",    &roadID,    "roadID/I");
  ktrack -> Branch ("chisq",     &chisq,     "chisq/F");
  ktrack -> Branch ("x0",        &x0,        "x0/F");
  ktrack -> Branch ("y0",        &y0,        "y0/F");
  ktrack -> Branch ("z0",        &z0,        "z0/F");
  //ktrack -> Branch ("x_target",  &x_target,  "x_target/F");
  //ktrack -> Branch ("y_target",  &y_target,  "y_target/F");
  //ktrack -> Branch ("z_target",  &z_target,  "z_target/F");
  //ktrack -> Branch ("x_dump",    &x_dump,    "x_dump/F");
  //ktrack -> Branch ("y_dump",    &y_dump,    "y_dump/F");
  //ktrack -> Branch ("z_dump",    &z_dump,    "z_dump/F");
  ktrack -> Branch ("x1",        &x1,        "x1/F");
  ktrack -> Branch ("y1",        &y1,        "y1/F");
  ktrack -> Branch ("z1",        &z1,        "z1/F");
  ktrack -> Branch ("px1",       &px1,       "px1/F");
  ktrack -> Branch ("py1",       &py1,       "py1/F");
  ktrack -> Branch ("pz1",       &pz1,       "pz1/F");
  ktrack -> Branch ("x3",        &x3,        "x3/F");
  ktrack -> Branch ("y3",        &y3,        "y3/F");
  ktrack -> Branch ("z3",        &z3,        "z3/F");
  ktrack -> Branch ("px3",       &px3,       "px3/F");
  ktrack -> Branch ("py3",       &py3,       "py3/F");
  ktrack -> Branch ("pz3",       &pz3,       "pz3/F");
  ktrack -> Branch ("tx_PT",     &tx_PT,     "tx_PT/F");
  ktrack -> Branch ("ty_PT",     &ty_PT,     "ty_PT/F");
  ktrack -> Branch ("targetPos", &targetPos, "targetPos/I");
  ktrack -> Branch ("target",    &target,    "target/I");
  ktrack -> Branch ("dump",      &dump,      "dump/I");
  ktrack -> Branch ("chisq",     &chisq,     "chisq/F");

  treeql -> SetBranchAddress ("runID",     &runID);
  treeql -> SetBranchAddress ("spillID",   &spillID);
  treeql -> SetBranchAddress ("eventID",   &eventID);
  treeql -> SetBranchAddress ("charge",    &charge);
  treeql -> SetBranchAddress ("roadID",    &roadID);
  treeql -> SetBranchAddress ("chisq",     &chisq);
  treeql -> SetBranchAddress ("x0",        &x0);
  treeql -> SetBranchAddress ("y0",        &y0);
  treeql -> SetBranchAddress ("z0",        &z0);
  //treeql -> SetBranchAddress ("x_target",  &x_target);
  //treeql -> SetBranchAddress ("y_target",  &y_target);
  //treeql -> SetBranchAddress ("z_target",  &z_target);
  //treeql -> SetBranchAddress ("x_dump",    &x_dump);
  //treeql -> SetBranchAddress ("y_dump",    &y_dump);
  //treeql -> SetBranchAddress ("z_dump",    &z_dump);
  treeql -> SetBranchAddress ("x1",        &x1);
  treeql -> SetBranchAddress ("y1",        &y1);
  treeql -> SetBranchAddress ("z1",        &z1);
  treeql -> SetBranchAddress ("px1",       &px1);
  treeql -> SetBranchAddress ("py1",       &py1);
  treeql -> SetBranchAddress ("pz1",       &pz1);
  treeql -> SetBranchAddress ("x3",        &x3);
  treeql -> SetBranchAddress ("y3",        &y3);
  treeql -> SetBranchAddress ("z3",        &z3);
  treeql -> SetBranchAddress ("px3",       &px3);
  treeql -> SetBranchAddress ("py3",       &py3);
  treeql -> SetBranchAddress ("pz3",       &pz3);
  treeql -> SetBranchAddress ("tx_PT",     &tx_PT);
  treeql -> SetBranchAddress ("ty_PT",     &ty_PT);
  //treeql -> SetBranchAddress ("targetPos", &targetPos);
  treeql -> SetBranchAddress ("target",    &target);
  treeql -> SetBranchAddress ("dump",      &dump);
  
  int nentries = treeql -> GetEntries();
  cout << "The number of Entries is " << nentries << endl;
  for(int i = 0; i < nentries; i++){
    treeql -> GetEntry(i);
    ktrack -> Fill();
  }


  char outname[128];
  sprintf(outname, "./nDST/%s_%d.root", schema, val);
  TFile *outfile = new TFile(outname,"RECREATE");
  outfile -> cd();
  ktrack -> Write();
  outfile -> Close();
  
  // Take out the garbage on your way out, and delete the created table.
  delete treeql;
  sprintf(stmt,"DROP TABLE IF EXISTS ktrack_%d", val); 
  con->Exec(stmt);
  
  // Disconnect from the server
  delete con;
  


}
