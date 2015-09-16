void drawmagflip(const int ktrackon = 1, const int dmrebin = 2)
{

  gStyle -> SetOptFit(1);
  const Float_t PI = 3.14159;

  char Fname[128];
  char Hname[128];
  char Tname[128];

  TH1D *Hdmphi0 = new TH1D("Hdmphi0","#phi of dimuon before magflip", 64, -3.15, 3.15);
  TH1D *Hdmphi1 = new TH1D("Hdmphi1","#phi of dimuon after magflip", 64, -3.15, 3.15);
  TH1D *Hdmphirat = new TH1D("Hdmphirat","#phi of dimuon before/after * rellum", 64, -3.15, 3.15);

  TH1D *Hmmuonphi0 = new TH1D("Hmmuonphi0","#phi of #mu^{-} before magflip", 64, -3.15, 3.15);
  TH1D *Hmmuonphi1 = new TH1D("Hmmuonphi1","#phi of #mu^{-} after magflip", 64, -3.15, 3.15);
  TH1D *Hpmuonphi0 = new TH1D("Hpmuonphi0","#phi of #mu^{+} before magflip", 64, -3.15, 3.15);
  TH1D *Hpmuonphi1 = new TH1D("Hpmuonphi1","#phi of #mu^{+} after magflip", 64, -3.15, 3.15);
  TH1D *Hmpmuonphirat = new TH1D("Hmpmuonphirat","#phi of  before(#mu^{-}) / [after(#mu^{+}) * rellum]", 64, -3.15, 3.15);
  TH1D *Hpmmuonphirat = new TH1D("Hpmmuonphirat","#phi of  before(#mu^{+}) / [after(#mu^{-})] * rellum]", 64, -3.15, 3.15);

  TH1D *Hcharge0 = new TH1D("Hcharge0", "charge of ktracks before magflip", 4, -2, 2);
  TH1D *Hcharge1 = new TH1D("Hcharge1", "charge of ktracks after magflip", 4, -2, 2);
  TH1D *Hchargerat = new TH1D("Hchargerat", "ratio of charge of ktracks before/after magflip", 4, -2, 2);
  Hcharge0 -> SetXTitle("Charge");
  Hcharge1 -> SetXTitle("Charge*(-1)");
  Hchargerat -> SetXTitle("Charge");
  Hcharge0 -> SetYTitle("Counts");
  Hcharge1 -> SetYTitle("Counts");
  Hcharge1 -> SetYTitle("Ratio");

  Hdmphi0 -> Sumw2();
  Hdmphi1 -> Sumw2();
  Hmmuonphi0 -> Sumw2();
  Hmmuonphi1 -> Sumw2();
  Hpmuonphi0 -> Sumw2();
  Hpmuonphi1 -> Sumw2();
  //Hcharge0 -> Sumw2();
  //Hcharge1 -> Sumw2();

  Hdmphi0 -> SetXTitle("#phi");
  Hdmphi1 -> SetXTitle("#phi");
  Hdmphirat -> SetXTitle("#phi");
  Hdmphi0 -> SetYTitle("Counts");
  Hdmphi1 -> SetYTitle("Counts");
  Hdmphirat -> SetYTitle("Ratio");
  Hmmuonphi0 -> SetXTitle("#phi");
  Hmmuonphi1 -> SetXTitle("#phi");
  Hmmuonphi0 -> SetYTitle("Counts");
  Hmmuonphi1 -> SetYTitle("Counts");
  Hmpmuonphirat -> SetXTitle("#phi");
  Hmpmuonphirat -> SetYTitle("Ratio");
  Hpmuonphi0 -> SetXTitle("#phi");
  Hpmuonphi1 -> SetXTitle("#phi");
  Hpmuonphi0 -> SetYTitle("Counts");
  Hpmuonphi1 -> SetYTitle("Counts");
  Hpmmuonphirat -> SetXTitle("#phi");
  Hpmmuonphirat -> SetYTitle("Ratio");

  sprintf(Fname, "./nDST/BeamAngle2_kdimuon.root");
  sprintf(Fname, "./nDST/BeamAngle_kdimuon.root");
  sprintf(Fname, "./nDST/MagFlip_kdimuon.root");
  TFile *inFile = new TFile(Fname);
  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");
  sprintf(Fname, "./nDST/cuts1314_merged_roadset62_R004_V005_0.root");
  sprintf(Fname, "./nDST/MagFlip_ktrack.root");
  TFile *inFile1 = new TFile(Fname);
  TTree *tracktree = (TTree*) inFile1 -> Get("ktrack");

  sprintf(Fname, "./nDST/MagFlip_spill.root");
  TFile *inFile2 = new TFile(Fname);
  TTree *spilltree = (TTree*) inFile2 -> Get("spill");

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
  Float_t phi;

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
  dmtree -> SetBranchAddress ("px1",             &px1);
  dmtree -> SetBranchAddress ("py1",             &py1);
  dmtree -> SetBranchAddress ("pz1",             &pz1);
  dmtree -> SetBranchAddress ("px2",             &px2);
  dmtree -> SetBranchAddress ("py2",             &py2);
  dmtree -> SetBranchAddress ("pz2",             &pz2);
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
  //UInt_t target, dump, targetPos;
  tracktree -> SetBranchAddress ("trackID",   &trackID);
  tracktree -> SetBranchAddress ("runID",     &runID);
  tracktree -> SetBranchAddress ("spillID",   &spillID);
  tracktree -> SetBranchAddress ("eventID",   &eventID);
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
  tracktree -> SetBranchAddress ("targetPos", &targetPos);
  tracktree -> SetBranchAddress ("target",    &target);
  tracktree -> SetBranchAddress ("dump",    &dump);

  //initialize spill tree
  Float_t liveProton;
  spilltree -> SetBranchAddress ("runID",           &runID);
  spilltree -> SetBranchAddress ("spillID",         &spillID);
  spilltree -> SetBranchAddress ("liveProton",         &liveProton);


  Float_t lum0 = 0.;
  Float_t lum1 = 0.;
  Float_t rellum;

  //ok, lets get relative luminosity
  int nentries2 = spilltree -> GetEntries();
  cout << "The number of Spill Entries is " << nentries2 << endl;
  for(int i = 0; i < nentries2; i++){
    spilltree -> GetEntry(i);
    
    if(runID > 12439 && runID < 12522){
      lum1 += liveProton;
    }
    else{
      lum0 += liveProton;
    }
  } 

  rellum = lum0/lum1;
  cout << "The number of live Proton from Before MagFlip is " << lum0 << endl;
  cout << "The number of live Proton from After MagFlip is " << lum1 << endl;
  cout << "The relative Luminosity is " << lum0/lum1 << endl;

  //get phi distributions dimuon
  TLorentzVector *lordm = new TLorentzVector;
  int nentries = dmtree -> GetEntries();
  cout << "The number of dimuon Entries is " << nentries << endl;
  int zbin;
  for(int i = 0; i < nentries; i++){
    if(i%100000 == 0) cout << i << endl;
    dmtree -> GetEntry(i);

    //if(target == 1) continue;

    lordm -> SetXYZM(dpx, dpy, dpz, mass);
    phi = lordm -> Phi();
    if(runID > 12439 && runID < 12522){
      Hdmphi1 -> Fill(phi);
    }
    else{
      Hdmphi0 -> Fill(phi);
    }  
  } 

  //get phi distributions ktrack
  if(ktrackon == 1){
    int nentries = tracktree -> GetEntries();
    cout << "The number of track Entries is " << nentries << endl;
    int zbin;
    for(int i = 0; i < nentries; i++){
      if(i%100000 == 0) cout << i << endl;
      tracktree -> GetEntry(i);

      if(dump == 1) continue;

      lordm -> SetXYZM(px1, py1, pz1, 0.105);
      phi = lordm -> Phi();

      //in roadset62 careful with charge variable when magnet flipped
      if(charge == -1){
	if(runID > 12439 && runID < 12522){
	  //if(phi > 0.){ phi = phi - PI;}
	  //else if(phi < 0.){ phi = phi + PI;}
	  Hpmuonphi1 -> Fill(phi);
	  Hcharge1 -> Fill(charge*-1);
	}
	else{
	  Hmmuonphi0 -> Fill(phi);
	  Hcharge0 -> Fill(charge);
	}  
      } 
      else if(charge == 1){
	if(runID > 12439 && runID < 12522){
	  //if(phi > 0.){ phi = phi - PI;}
	  //else if(phi < 0.){ phi = phi + PI;}
	  Hmmuonphi1 -> Fill(phi);
	  Hcharge1 -> Fill(charge*-1);
	}
	else{
	  Hpmuonphi0 -> Fill(phi);
	  Hcharge0 -> Fill(charge);
	} 
      }
    } 
  }

  if(dmrebin == 2){
    Hdmphi0 -> Rebin(2);
    Hdmphi1 -> Rebin(2);
    Hdmphirat -> Rebin(2);
  }
  if(dmrebin == 4){
    Hdmphi0 -> Rebin(4);
    Hdmphi1 -> Rebin(4);
    Hdmphirat -> Rebin(4);
  }
  if(dmrebin == 8){
    Hdmphi0 -> Rebin(8);
    Hdmphi1 -> Rebin(8);
    Hdmphirat -> Rebin(8);
  }
  if(dmrebin == 16){
    Hdmphi0 -> Rebin(16);
    Hdmphi1 -> Rebin(16);
    Hdmphirat -> Rebin(16);
  }
  Hdmphirat -> Divide(Hdmphi0, Hdmphi1, 1, rellum);
  Hmpmuonphirat -> Divide(Hmmuonphi0, Hpmuonphi1, 1, rellum);
  Hpmmuonphirat -> Divide(Hpmuonphi0, Hmmuonphi1, 1, rellum);
  Hchargerat -> Divide(Hcharge0, Hcharge1, 1, rellum);


  TLine *l0 = new TLine(-3.15, 1., 3.15, 1.);
  l0 -> SetLineStyle(2);


  TCanvas *c3 = new TCanvas("c3", "c3", 1800, 400);
  c3 -> Divide(3, 1);
  c3 -> cd(1);
  Hdmphi0 -> Draw();
  c3 -> cd(2);
  Hdmphi1 -> Draw();
  c3 -> cd(3);
  Hdmphirat -> Draw();
  Hdmphirat -> Fit("pol0");
  l0 -> Draw();
  c3 -> SaveAs("./images/dimuon.gif");

  if(ktrackon == 1){
      TCanvas *c2 = new TCanvas("c2", "c2", 1200, 400);
      c2 -> Divide(2, 1);
      c2 -> cd(1);
      Hcharge0 -> Draw();
      c2 -> cd(2);
      Hcharge1 -> Draw();
      //c2 -> cd(3);
      //Hchargerat -> Draw();
      c2 -> SaveAs("./images/charge.gif");

      TCanvas *c6 = new TCanvas("c6", "c6", 1800, 800);
      c6 -> Divide(3, 2);
      c6 -> cd(1);
      Hmmuonphi0 -> Draw();
      c6 -> cd(2);
      Hpmuonphi1 -> Draw();
      c6 -> cd(3);
      Hmpmuonphirat -> Draw();
      //Hmpmuonphirat -> Fit("pol0");
      l0 -> Draw();

      c6 -> cd(4);
      Hpmuonphi0 -> Draw();
      c6 -> cd(5);
      Hmmuonphi1 -> Draw();
      c6 -> cd(6);
      Hpmmuonphirat -> Draw();
      //Hpmmuonphirat -> Fit("pol0");
      l0 -> Draw();
      c6 -> SaveAs("./images/track.gif");
    }
}
