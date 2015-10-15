void drawfiducial(const int roadset = 62)
{
  gStyle -> SetOptFit(1);

  char Fname[128];
  char Hname[128];
  char Tname[128];

  sprintf(Fname, "./nDST/BeamAngle_kdimuon.root");
  sprintf(Fname, "./nDST/MagFlip_kdimuon.root");
  sprintf(Fname, "./nDST/Analysis_roadset%d_R005_V001.root", roadset);
  TFile *inFile = new TFile(Fname);
  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");

  TH2F *Hpos0st1 = new TH2F("Hpos0st1","Target #mu^{+} track at Station 1", 120, -60, 60, 120, -60,60);
  TH2F *Hneg0st1 = new TH2F("Hneg0st1","Target #mu^{-} track at Station 1", 120, -60, 60, 120, -60,60);
  TH2F *Hpos0st3 = new TH2F("Hpos0st3","Target #mu^{+} track at Station 3", 240, -120, 120, 300, -150,150);
  TH2F *Hneg0st3 = new TH2F("Hneg0st3","Target #mu^{-} track at Station 3", 240, -120, 120, 300, -150,150);

  Hpos0st1 -> SetXTitle("x [cm]");
  Hneg0st1 -> SetXTitle("x [cm]");
  Hpos0st3 -> SetYTitle("y [cm]");
  Hneg0st3 -> SetYTitle("y [cm]");

  TH1F *Hpx0 = new TH1F("Hpx0","dpx of Dimuons target", 40, -4, 4);
  TH1F *Hpy0 = new TH1F("Hpy0","dpy of Dimuons target", 40, -4, 4);
  TH1F *Hpz0 = new TH1F("Hpz0","dpz of Dimuons target", 100, 20, 120);

  Hpx0 -> SetXTitle("px (GeV)");
  Hpy0 -> SetXTitle("py (GeV)");
  Hpz0 -> SetXTitle("pz (GeV)");

  Hpx0 -> Sumw2();
  Hpy0 -> Sumw2();
  Hpz0 -> Sumw2();

  const int nbins = 4;
  TH2F *Hpos1st1[nbins];
  TH2F *Hneg1st1[nbins];
  TH2F *Hpos1st3[nbins];
  TH2F *Hneg1st3[nbins];
  TH1F *Hpx1[nbins];
  TH1F *Hpy1[nbins];
  TH1F *Hpz1[nbins];

  TH2F *Hpy1posy1[nbins];

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
   
    sprintf(Hname, "Hpx1_%d", i);
    sprintf(Tname, "dpx of Dimuons dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpx1[i] = new TH1F(Hname, Tname, 40, -4, 4);
    sprintf(Hname, "Hpy1_%d", i);
    sprintf(Tname, "dpy of Dimuons dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpy1[i] = new TH1F(Hname, Tname, 40, -4, 4);
    sprintf(Hname, "Hpz1_%d", i);
    sprintf(Tname, "dpz of Dimuons dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpz1[i] = new TH1F(Hname, Tname, 100, 20, 120);

    Hpx1[i] -> SetXTitle("px (GeV)");
    Hpy1[i] -> SetXTitle("py (GeV)");
    Hpz1[i] -> SetXTitle("pz (GeV)");

    Hpx1[i] -> Sumw2();
    Hpy1[i] -> Sumw2();
    Hpz1[i] -> Sumw2();

    sprintf(Hname, "Hpy1posy1_%d", i);
    sprintf(Tname, "Dump #mu^{+} dpy, posy1 at Station 1 [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpy1posy1[i] = new TH2F(Hname, Tname, 40, -4, 4, 120, -60, 60);

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
  Float_t posx1, posy1, posx3, posy3;
  Float_t negx1, negy1, negx3, negy3;

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
  dmtree -> SetBranchAddress ("posx1",           &posx1);
  dmtree -> SetBranchAddress ("posy1",           &posy1);
  dmtree -> SetBranchAddress ("posx3",           &posx3);
  dmtree -> SetBranchAddress ("posy3",           &posy3);
  dmtree -> SetBranchAddress ("negx1",           &negx1);
  dmtree -> SetBranchAddress ("negy1",           &negy1);
  dmtree -> SetBranchAddress ("negx3",           &negx3);
  dmtree -> SetBranchAddress ("negy3",           &negy3);


  int nentries = dmtree -> GetEntries();

  cout << "The number of Entries in kdimuon is " << nentries << endl;
  int zbin;
  for(int i = 0; i < nentries; i++){
    if(i%100000 == 0) cout << i << endl;
    dmtree -> GetEntry(i);


    if(mass < 4.2)continue;
    //if(runID > 10091 && runID < 10185)continue;
    //if((m3hm < -2.5 || m3hm > 1.2) || (m3vm < 1. || m3vm > 4.6))continue;

    //make fiducial cuts
    //if(posy1 > 0)continue;

    if(target == 1){
      Hpos0st1 -> Fill(posx1, posy1);
      Hpos0st3 -> Fill(posx3, posy3);
      Hneg0st1 -> Fill(negx1, negy1);
      Hneg0st3 -> Fill(negx3, negy3);
      Hpx0 -> Fill(dpx);
      Hpy0 -> Fill(dpy);
      Hpz0 -> Fill(dpz);

    }
    if(dump == 1 && dz > -50. && dz < 150.){
      zbin = dz*0.02 + 1;
      Hpos1st1[zbin] -> Fill(posx1, posy1);
      Hpos1st3[zbin] -> Fill(posx3, posy3);
      Hneg1st1[zbin] -> Fill(negx1, negy1);
      Hneg1st3[zbin] -> Fill(negx3, negy3);
      Hpx1[zbin] -> Fill(dpx);
      Hpy1[zbin] -> Fill(dpy);
      Hpz1[zbin] -> Fill(dpz);

      Hpy1posy1[zbin] -> Fill(dpy, negy1);

    } 
  }

  //get the average value of px, py and make graphs
  Double_t gpx_x[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpx_y[nbins+1];
  TGraph *Gpx = new TGraph();
  Gpx -> SetMarkerStyle(21);
  Gpx -> SetFillColor(0);
  Double_t gpy_x[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpy_y[nbins+1];
  TGraph *Gpy = new TGraph();
  Gpy -> SetMarkerStyle(21);
  Gpy -> SetMarkerColor(2);
  Gpy -> SetFillColor(0);
  Double_t gpz_x[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpz_y[nbins+1];
  TGraph *Gpz = new TGraph();
  Gpz -> SetMarkerStyle(21);
  Gpz -> SetMarkerColor(4);
  Gpz -> SetFillColor(0);
  for(int i = 0; i < nbins+1; i++){
    if(i == 0){
      gpx_y[i] = Hpx0 -> GetMean();
      gpy_y[i] = Hpy0 -> GetMean();
      gpz_y[i] = Hpz0 -> GetMean() / 1000.;
    }
    else{
      gpx_y[i] = Hpx1[i-1] -> GetMean();
      //if(fmag == 1 && i > 2){gpx_y[i] = gpx_y[i] + 0.075*(i-2);}
      gpy_y[i] = Hpy1[i-1] -> GetMean();
      gpz_y[i] = Hpz1[i-1] -> GetMean() / 1000.;
    }
    Gpx -> SetPoint(i, gpx_x[i], gpx_y[i]);
    Gpy -> SetPoint(i, gpy_x[i], gpy_y[i]);
    Gpz -> SetPoint(i, gpz_x[i], gpz_y[i]);
  }
  Gpx -> SetMaximum(0.23);
  Gpx -> SetMinimum(-0.01);
  Gpx -> SetTitle("Mean p_{x,y,z} of dimuons vs zvtx");
  Gpx -> GetXaxis() -> SetTitle("dz [cm]");
  Gpx -> GetYaxis() -> SetTitle("dp_{x,y,z} [GeV]");

  TLegend *legend = new TLegend(0.7,0.7,0.9,0.9);
  legend -> AddEntry(Gpx, "dp_{x}");
  legend -> AddEntry(Gpy, "dp_{y}");

  TLine *st1h = new TLine(-60,0,60,0);  
  TLine *st1v = new TLine(0,-60,0,60);  
  TLine *st3h = new TLine(-120,0,120,0);  
  TLine *st3v = new TLine(0,-150,0,150);  
  st1h -> SetLineStyle(2);
  st1v -> SetLineStyle(2);
  st3h -> SetLineStyle(2);
  st3v -> SetLineStyle(2);

  TCanvas *c2 = new TCanvas("c2","c2",1800,400);
  c2 -> Divide(3,1);
  c2 -> cd(1);
  //Hpos1st1[1]->Draw("colz");
  Hpy1posy1[1]->Draw("colz");
  c2 -> cd(2);
  Hpos1st1[1]->Draw("colz");
  //Hneg1st1[1]->Draw("colz");
  st1h -> Draw(); st1v -> Draw();
  //if(spot != 4) box -> Draw();
  c2 -> cd(3);
  Gpx -> Draw("ap");
  Gpy -> Draw("p");
  //Gpz -> Draw("p");
  legend -> Draw();

  
 
  
  //
  //st1h -> Draw(); st1v -> Draw();
  
  TCanvas *c3 = new TCanvas("c3","c3",1800,800);
  c3 -> Divide(2,2);
  c3 -> cd(1);
  Hpos0st1 -> Draw("colz");
  st1h -> Draw(); st1v -> Draw();
  c3 -> cd(2);
  Hneg0st1 -> Draw("colz");
  st1h -> Draw(); st1v -> Draw();
  c3 -> cd(3);
  Hpos0st3 -> Draw("colz");
  st3h -> Draw(); st3v -> Draw();
  c3 -> cd(4);
  Hneg0st3 -> Draw("colz");
  st3h -> Draw(); st3v -> Draw();
  c3 -> SaveAs("./images/fid_target.gif");
  
  TCanvas *cpos1 = new TCanvas("cpos1","cpos1",1200,800);
  cpos1 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cpos1 -> cd(i+1);
    Hpos1st1[i] -> Draw("colz");
    st1h -> Draw(); st1v -> Draw();
  }
  cpos1 -> cd();
  cpos1 -> SaveAs("./images/fid_pos1st1_dump.gif");
  
  TCanvas *cneg1 = new TCanvas("cneg1","cneg1",1200,800);
  cneg1 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cneg1 -> cd(i+1);
    Hneg1st1[i] -> Draw("colz");
    st1h -> Draw(); st1v -> Draw();
  }
  cneg1 -> cd();
  cneg1 -> SaveAs("./images/fid_neg1st1_dump.gif");
  

  TCanvas *cpos3 = new TCanvas("cpos3","cpos3",1200,800);
  cpos3 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cpos3 -> cd(i+1);
    Hpos1st3[i] -> Draw("colz");
    st3h -> Draw(); st3v -> Draw();
  }
  cpos3 -> cd();
  cpos3 -> SaveAs("./images/fid_pos1st3_dump.gif");

  TCanvas *cneg3 = new TCanvas("cneg3","cneg3",1200,800);
  cneg3 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cneg3 -> cd(i+1);
    Hneg1st3[i] -> Draw("colz");
    st3h -> Draw(); st3v -> Draw();

  }
  cneg3 -> cd();
  cneg3 -> SaveAs("./images/fid_neg1st3_dump.gif");
  
}
