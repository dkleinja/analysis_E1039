void getspill(const char *schema)
{
  //connect to server
  //TSQLServer *con = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283", "seaguest", "qqbar2mu+mu-");
  TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306", "root", "");
  
  //TTreeSQL * treeql_run = new TTreeSQL(con, schema, "mRun");
  Int_t nEvGen; 
  Int_t nevgen = 1;
  Float_t sigwt =1;

  cout << nevgen << endl;
  
  TTreeSQL * treeql = new TTreeSQL(con, schema, "Spill");
  UInt_t runID;
  Int_t spillID;
  Float_t liveProton;
  Float_t liveProton2;

  TTree *spill = new TTree("spill","spill");
  spill -> SetDirectory(0);
  spill -> Branch ("runID",           &runID,           "runID/I");
  spill -> Branch ("spillID",         &spillID,         "spillID/I");
  spill -> Branch ("liveProton",      &liveProton,      "liveProton/F");
  spill -> Branch ("liveProton2",      &liveProton2,      "liveProton2/F");

  treeql -> SetBranchAddress ("runID",           &runID);
  treeql -> SetBranchAddress ("spillID",         &spillID);
  treeql -> SetBranchAddress ("liveProton",      &liveProton);
  treeql -> SetBranchAddress ("liveProton2",      &liveProton2);

  
  int nentries = treeql -> GetEntries();
  cout << "The number of Entries is " << nentries << endl;
  for(int i = 0; i < nentries; i++){
    if(i%10000 == 0) cout << i << endl;

    treeql -> GetEntry(i);
    cout<< i << " " << runID << " " << spillID << " " << liveProton << " " << liveProton2 << endl;
    spill -> Fill();

  }
 cout<<"blah"<<endl; 
  char outname[128];
  sprintf(outname, "./nDST/%s_spill.root", schema);
  TFile *outfile = new TFile(outname,"RECREATE");
  outfile -> cd();
  spill   -> Write();
  outfile -> Close();

}
