void drawParent(int target = 0)
{
  
  char Fname[128];
  sprintf(Fname, "./beamProfile/e906beamProf_100M_0.root");
  //sprintf(Fname, "./nDST/longerkaondecay_50M_0.root");
  TFile *inFile = new TFile(Fname);
  /*
  TH1I *hparent_all = new TH1I("hparent_all", "all parents",8000, -3500, 4500);
  TH1I *hparent_pions = new TH1I("hparent_pions", "pions",8000, -3500, 4500);
  TH1I *hparent_kaons = new TH1I("hparent_kaons", "kaons",8000, -3500, 4500);
  TH1I *hparent_short = new TH1I("hparent_short", "other",8000, -3500, 4500);
  */
  TH1I *hparent_all = new TH1I("hparent_all", "all parents",1000, -500, 500);
  TH1I *hparent_pions = new TH1I("hparent_pions", "pions",1000, -500, 500);
  TH1I *hparent_kaons = new TH1I("hparent_kaons", "kaons",1000, -500, 500);
  TH1I *hparent_short = new TH1I("hparent_short", "other",1000, -500, 500);

  
  Int_t all, pions, kaons, others;
  Int_t n;
  Int_t parentID[1000];
  Float_t zProd;
  TVector3 *getpos;

  //TClonesArray* p_pos = new TClonesArray("TVector3");  p_pos->BypassStreamer(); TClonesArray& pos = *p_pos;
  TClonesArray* posArr = new TClonesArray("TVector3");
  TClonesArray* momArr = new TClonesArray("TVector3");
  //TClonesArray *externalPositions = new TClonesArray("TVector3");
  //TClonesArray *externalPositions = 0;
  
  TTree *tracktree = (TTree*) inFile -> Get("save");
  tracktree -> SetBranchAddress ("n",        &n);
  tracktree -> SetBranchAddress ("zProd",        &zProd);
  tracktree -> SetBranchAddress ("parentID", parentID);
  tracktree -> SetBranchAddress ("pos",   &posArr);
  //TBranch *bfZ = tree->GetBranch("fZ");

  int nentries = tracktree -> GetEntries();
  cout << "The number of track Entries is " << nentries << endl;
  for(int i = 0; i < nentries; i++){
    tracktree -> GetEntry(i);
    //cout << externalPositions.fZ[0] << endl;
    //target math
    for(int j = 0; j < n; j++){
      TVector3* p_pos = (TVector3*)posArr->At(j);
      //cout << i << " "<< j << " " << parentID[j] << endl;
      if(target == 0 && zProd < 0.) continue;
      else if(target == 1 && zProd > 0.) continue;
      hparent_all -> Fill(parentID[j]);
      all++;
      //get the particles
      if(abs(parentID[j]) == 211){
	pions++;
	hparent_pions -> Fill(parentID[j]);
      }
      if(abs(parentID[j]) == 321 || parentID[j] == 130){
	kaons++;
	hparent_kaons -> Fill(parentID[j]);
      }
      if(abs(parentID[j]) != 211 && abs(parentID[j])!=321 && parentID[j] != 130){
	others++;
	hparent_short -> Fill(parentID[j]);
      }
    }
  }

  
  TCanvas *c4 = new TCanvas("c4", "c4", 1200, 1000);
  c4 -> Divide(2, 2);
  c4 -> cd(1);
  hparent_all -> Draw();
  c4 -> cd(2);
  hparent_pions -> Draw();
  c4 -> cd(3);
  hparent_kaons -> Draw();
  c4 -> cd(4);
  hparent_short -> Draw();

  cout << "The total number of particles is " << all << " " << 1.*all/all*100 << "%" << endl;
  cout << "The total number of pions is " << pions << " " << 1.*pions/all*100. << "%" << endl;
  cout << "The total number of kaons is " << kaons << " " << 1.*kaons/all*100. << "%" << endl;
  cout << "The total number of ohters is " << others << " " << 1.*others/all*100. << "%" << endl;
}
