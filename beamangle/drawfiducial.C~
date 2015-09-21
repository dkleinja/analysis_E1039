void drawfiducial()
{
  gStyle -> SetOptFit(1);

  char Fname[128];
  char Hname[128];
  char Tname[128];

  sprintf(Fname, "./nDST/BeamAngle_kdimuon.root");
  TFile *inFile = new TFile(Fname);
  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");

  sprintf(Fname, "./nDST/newMerge_ktrack.root");
  TFile *inFile2 = new TFile(Fname);
  TTree *tracktree = (TTree*) inFile2 -> Get("ktrack");

  TH2F *Hpos0st1 = new TH2F("Hpos0st1","Target #mu^{+} track at Station 1", 120, -60, 60, 120, -60,60);
  TH2F *Hneg0st1 = new TH2F("Hneg0st1","Target #mu^{-} track at Station 1", 120, -60, 60, 120, -60,60);
  TH2F *Hpos0st3 = new TH2F("Hpos0st3","Target #mu^{+} track at Station 3", 240, -120, 120, 300, -150,150);
  TH2F *Hpos0st3 = new TH2F("Hneg0st3","Target #mu^{-} track at Station 3", 240, -120, 120, 300, -150,150);

  Hpos0st1 -> SetXTitle("x [cm]");
  Hneg0st1 -> SetXTitle("x [cm]");
  Hpos0st3 -> SetYTitle("y [cm]");
  Hneg0st3 -> SetYTitle("y [cm]");

  const int nbins = 4;
  TH2F *Hpos1st1[nbins];
  TH2F *Hneg1st1[nbins];
  TH2F *Hpos1st3[nbins];
  TH2F *Hneg1st3[nbins];
  int rangelow[nbins], rangehigh[nbins];

  for(int i = 0; i < nbins; i++){
    rangelow[i] = i*50 - 50;
    rangehigh[i] = i*50;

    sprintf(Hname, "Hpos1st1_%d", i);
    sprintf(Tname, "Dump #mu^{+} track at Station 1 [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpos1st1[i] = new TH2F(Hname, Tname, 120, -60, 60, 120, -60, 60);
    sprintf(Hname, "Hneg1st1_%d", i);
    sprintf(Tname, "Dump #mu^{-} track at Station 1 [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hneg1st1[i] = new TH2F(Hname, Tname, 120, -60, 60, 120, -60, 60);
    sprintf(Hname, "Hpos1st3_%d", i);
    sprintf(Tname, "Dump #mu^{+} track at Station 3 [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpos1st3[i] = new TH2F(Hname, Tname, 240, -120, 120, 300, -150,150);
    sprintf(Hname, "Hneg1st3_%d", i);
    sprintf(Tname, "Dump #mu^{-} track at Station 3 [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hneg1st3[i] = new TH2F(Hname, Tname, 240, -120, 120, 300, -150,150);

    Hpos1st1[i] -> SetXTitle("x [cm]");
    Hpos1st3[i] -> SetXTitle("x [cm]");
    Hneg1st1[i] -> SetXTitle("x [cm]");
    Hneg1st3[i] -> SetXTitle("x [cm]");

    Hpos1st1[i] -> SetYTitle("y [cm]");
    Hpos1st3[i] -> SetYTitle("y [cm]");
    Hneg1st1[i] -> SetYTitle("y [cm]");
    Hneg1st3[i] -> SetYTitle("y [cm]");
   

  }
 
  //initialize kdimuon tree
  Int_t dimuonID;
  Int_t runID;
  Int_t spillID, eventID;
  Int_t negTrackID, posTrackID;
  Float_t xF, xT, xB, mass;
  Float_t dx, dy, dz;
  Float_t dpx, dpy, dpz;
  Float_t px1, py1, pz1;
  Float_t px2, py2, pz2;
  Float_t trackSeparation, chisq_dimuon;
  Int_t target, dump;
  Int_t targetPos;
  Float_t m3hm, m3hs, m3vm, m3vs;
  Float_t m2hm, m2hs, m2vm, m2vs;
  Float_t dhm, dvm;

  dmtree -> SetBranchAddress ("dimuonID",        &dimuonID);
  dmtree -> SetBranchAddress ("runID",           &runID);
  dmtree -> SetBranchAddress ("eventID",         &eventID);
  dmtree -> SetBranchAddress ("spillID",         &spillID);
  dmtree -> SetBranchAddress ("posTrackID",    &posTrackID);
  dmtree -> SetBranchAddress ("negTrackID",    &negTrackID);
  dmtree -> SetBranchAddress ("dx",              &dx);
  dmtree -> SetBranchAddress ("dy",              &dy);
  dmtree -> SetBranchAddress ("dz",              &dz);
  dmtree -> SetBranchAddress ("dpx",             &dpx);
  dmtree -> SetBranchAddress ("dpy",             &dpy);
  dmtree -> SetBranchAddress ("dpz",             &dpz);
  dmtree -> SetBranchAddress ("mass",            &mass);
  dmtree -> SetBranchAddress ("xF",              &xF);
  dmtree -> SetBranchAddress ("xB",              &xB);
  dmtree -> SetBranchAddress ("xT",              &xT);
  dmtree -> SetBranchAddress ("trackSeparation", &trackSeparation);
  dmtree -> SetBranchAddress ("chisq_dimuon",    &chisq_dimuon);
  //dmtree -> SetBranchAddress ("px1",             &px1);
  //dmtree -> SetBranchAddress ("py1",             &py1);
  //dmtree -> SetBranchAddress ("pz1",             &pz1);
  //dmtree -> SetBranchAddress ("px2",             &px2);
  //dmtree -> SetBranchAddress ("py2",             &py2);
  //dmtree -> SetBranchAddress ("pz2",             &pz2);
  dmtree -> SetBranchAddress ("target",          &target);
  dmtree -> SetBranchAddress ("dump",            &dump);
  dmtree -> SetBranchAddress ("targetPos",       &targetPos);
  dmtree -> SetBranchAddress ("m3hm",            &m3hm);
  dmtree -> SetBranchAddress ("m3hs",            &m3hs);
  dmtree -> SetBranchAddress ("m3vm",            &m3vm);
  dmtree -> SetBranchAddress ("m3vs",            &m3vs);
  dmtree -> SetBranchAddress ("m2hm",            &m2hm);
  dmtree -> SetBranchAddress ("m2hs",            &m2hs);
  dmtree -> SetBranchAddress ("m2vm",            &m2vm);
  dmtree -> SetBranchAddress ("m2vs",            &m2vs);

  //initialize ktrack tree
  Int_t trackID, runID2, spillID2, eventID2, charge, roadID;
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
  Int_t target2, dump2, targetPos2;

  tracktree -> SetBranchAddress ("runID",     &runID2);
  tracktree -> SetBranchAddress ("spillID",   &spillID2);
  tracktree -> SetBranchAddress ("eventID",   &eventID2);
  tracktree -> SetBranchAddress ("trackID",   &trackID);
  tracktree -> SetBranchAddress ("charge",    &charge);
  tracktree -> SetBranchAddress ("roadID",    &roadID);
  tracktree -> SetBranchAddress ("chisq",     &chisq);
  tracktree -> SetBranchAddress ("x0",        &x0);
  tracktree -> SetBranchAddress ("y0",        &y0);
  tracktree -> SetBranchAddress ("z0",        &z0);
  tracktree -> SetBranchAddress ("x_target",  &x_target);
  tracktree -> SetBranchAddress ("y_target",  &y_target);
  tracktree -> SetBranchAddress ("z_target",  &z_target);
  tracktree -> SetBranchAddress ("x_dump",    &x_dump);
  tracktree -> SetBranchAddress ("y_dump",    &y_dump);
  tracktree -> SetBranchAddress ("z_dump",    &z_dump);
  tracktree -> SetBranchAddress ("x1",        &x1);
  tracktree -> SetBranchAddress ("y1",        &y1);
  tracktree -> SetBranchAddress ("z1",        &z1);
  tracktree -> SetBranchAddress ("px1",       &px1);
  tracktree -> SetBranchAddress ("py1",       &py1);
  tracktree -> SetBranchAddress ("pz1",       &pz1);
  tracktree -> SetBranchAddress ("x3",        &x3);
  tracktree -> SetBranchAddress ("y3",        &y3);
  tracktree -> SetBranchAddress ("z3",        &z3);
  tracktree -> SetBranchAddress ("px3",       &px3);
  tracktree -> SetBranchAddress ("py3",       &py3);
  tracktree -> SetBranchAddress ("pz3",       &pz3);
  tracktree -> SetBranchAddress ("tx_PT",     &tx_PT);
  tracktree -> SetBranchAddress ("ty_PT",     &ty_PT);
  tracktree -> SetBranchAddress ("target",          &target2);
  tracktree -> SetBranchAddress ("dump",            &dump2);
  tracktree -> SetBranchAddress ("targetPos",       &targetPos2);


  int nentries = dmtree -> GetEntries();
  int nentries2 = tracktree -> GetEntries();
  cout << "The number of Entries in kdimuon is " << nentries << endl;
  cout << "The number of Entries in ktrack is " << nentries2 << endl;
  int zbin;
  for(int i = 0; i < nentries2; i++){
    tracktree -> GetEntry(i);
    cout << i << "blah " << endl;
    if(runID > 10091 && runID < 10185)continue;

    //ok, let's loop the kdimuon tree
    for(int j = 0; j < nentries; j++){
      dmtree -> GetEntry(j);
      //if(j == 261739) cout << "blah"<< endl;
      if(spillID != spillID2)continue;
	if(posTrackID == trackID){
	  cout << i << " " << j << " " << spillID << " " << spillID2 <<  " " << posTrackID << " " << trackID << endl;
	  if(target == 1){
	    Hpos0st1 -> Fill(x1, y1);
	    Hpos0st3 -> Fill(x3, y3);
	  }
	  if(dump == 1 && dz > -50. && dz < 150.){
	    zbin = dz*0.02 + 1;
	    Hpos1st1[zbin] -> Fill(x1, y1);
	    Hpos1st3[zbin] -> Fill(x3, y3);
	  } 
	  break;
	}
	if(negTrackID == trackID){
	  if(target == 1){
	    Hpos0st1 -> Fill(x1, y1);
	    Hpos0st3 -> Fill(x3, y3);
	  }
	  if(dump == 1 && dz > -50. && dz < 150.){
	    zbin = dz*0.02 + 1;
	    Hpos1st1[zbin] -> Fill(x1, y1);
	    Hpos1st3[zbin] -> Fill(x3, y3);
	  }
	}
	
    }

    }
  /*
  
  TCanvas *c3 = new TCanvas("c3","c3",1800,800);
  c3 -> Divide(2,2);
  c3 -> cd(1);
  Hpos0st1 -> Draw();
  c3 -> cd(2);
  Hneg0st1 -> Draw();
  c3 -> cd(3);
  Hpos0st3 -> Draw();
  c3 -> cd(4);
  Hneg0st3 -> Draw();
  c3 -> SaveAs("./images/fid_target.gif");

  TCanvas *cpos1 = new TCanvas("cpos1","cpos1",1200,800);
  cpos1 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cpos1 -> cd(i+1);
    Hpos1st1[i] -> Draw();

  }
  cpos1 -> cd();
  cpos1 -> SaveAs("./images/fid_pos1st1_dump.gif");

  TCanvas *cneg1 = new TCanvas("cneg1","cneg1",1200,800);
  cneg1 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cneg1 -> cd(i+1);
    Hneg1st1[i] -> Draw();

  }
  cneg1 -> cd();
  cneg1 -> SaveAs("./images/fid_neg1st1_dump.gif");
  



  TCanvas *cpos3 = new TCanvas("cpos3","cpos3",1200,800);
  cpos3 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cpos3 -> cd(i+1);
    Hpos3st3[i] -> Draw();

  }
  cpos3 -> cd();
  cpos3 -> SaveAs("./images/fid_pos1st3_dump.gif");

  TCanvas *cneg3 = new TCanvas("cneg3","cneg3",1200,800);
  cneg3 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cneg3 -> cd(i+1);
    Hneg1st3[i] -> Draw();

  }
  cneg3 -> cd();
  cneg3 -> SaveAs("./images/fid_neg1st3_dump.gif");
  */
}
