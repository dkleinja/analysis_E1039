void drawVtx(int target = 2)
{
  
  char Fname[128];
  sprintf(Fname, "./beamProfile/output.root");
  sprintf(Fname, "./beamProfile/e906beamProf_100M_1.root");
  //sprintf(Fname, "./nDST/longerkaondecay_50M_0.root");
  TFile *inFile = new TFile(Fname);

  TH1I *hx_all = new TH1I("hx_all", "all parents",100,-10,10);
  TH1I *hx_pions = new TH1I("hx_pions", "pions",100,-10,10);
  TH1I *hx_kaons = new TH1I("hx_kaons", "kaons",100,-10,10);
  TH1I *hx_short = new TH1I("hx_short", "other",100,-10,10);
  TH1I *hy_all = new TH1I("hy_all", "all parents",100,-10,10);
  TH1I *hy_pions = new TH1I("hy_pions", "pions",100,-10,10);
  TH1I *hy_kaons = new TH1I("hy_kaons", "kaons",100,-10,10);
  TH1I *hy_short = new TH1I("hy_short", "other",100,-10,10);
  TH1I *hz_all = new TH1I("hz_all", "all parents",600,-200,400);
  TH1I *hz_pions = new TH1I("hz_pions", "pions",600,-200,400);
  TH1I *hz_kaons = new TH1I("hz_kaons", "kaons",600,-200,400);
  TH1I *hz_short = new TH1I("hz_short", "other",600,-200,400);

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
      //cout << i << " "<< j << " " << p_pos->Z() << endl;
      if(target == 0 && zProd < 0.) continue;
      else if(target == 1 && zProd > 0.) continue;
      hx_all -> Fill(p_pos->X());
      hy_all -> Fill(p_pos->Y());
      hz_all -> Fill(p_pos->Z());
      all++;
      //get the particles
      if(abs(parentID[j]) == 211){
	pions++;
	hx_pions -> Fill(p_pos->X());
	hy_pions -> Fill(p_pos->Y());
	hz_pions -> Fill(p_pos->Z());
      }
      if(abs(parentID[j]) == 321 || parentID[j] == 130){
	kaons++;
	hx_kaons -> Fill(p_pos->X());
     	hy_kaons -> Fill(p_pos->Y());
	hz_kaons -> Fill(p_pos->Z());
      }
      if(abs(parentID[j]) != 211 && abs(parentID[j])!=321 && parentID[j] != 130){
	others++;
	hx_short -> Fill(p_pos->X());
	hy_short -> Fill(p_pos->Y());
	hz_short -> Fill(p_pos->Z());
      }
    }
  }

  TCanvas *c4x = new TCanvas("c4x", "c4x", 1200, 1000);
  c4x -> Divide(2, 2);
  c4x -> cd(1);
  hx_all -> Draw();
  c4x -> cd(2);
  hx_pions -> Draw();
  c4x -> cd(3);
  hx_kaons -> Draw();
  c4x -> cd(4);
  hx_short -> Draw();
  
  TCanvas *c4z = new TCanvas("c4z", "c4z", 1200, 1000);
  c4z -> Divide(2, 2);
  c4z -> cd(1);
  hz_all -> Draw();
  c4z -> cd(2);
  hz_pions -> Draw();
  c4z -> cd(3);
  hz_kaons -> Draw();
  c4z -> cd(4);
  hz_short -> Draw();

  cout << "The total number of particles is " << all << " " << 1.*all/all*100 << "%" << endl;
  cout << "The total number of pions is " << pions << " " << 1.*pions/all*100. << "%" << endl;
  cout << "The total number of kaons is " << kaons << " " << 1.*kaons/all*100. << "%" << endl;
  cout << "The total number of ohters is " << others << " " << 1.*others/all*100. << "%" << endl;
}
