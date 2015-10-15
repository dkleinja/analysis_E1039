void drawmuon(const int roadset= 62, const int spot = 5)
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

  TH2F *Hm3m = new TH2F("Hm3m","M3 Vert Mean vs. Hori Mean", 40, -5, 5, 40,  -5, 5);
  Hm3m -> SetXTitle("hm (mm)");
  Hm3m -> SetYTitle("vm (mm)");

  TH2F *Hpxpy1 = new TH2F("Hpxpy1","py vs px #mu^{+}", 40, -5, 5, 40,  -5, 5);
  Hpxpy1 -> SetXTitle("p_{x} [GeV]");
  Hpxpy1 -> SetYTitle("p_{y} [GeV]");

  TH2F *Hpxpy2 = new TH2F("Hpxpy2","py vs px #mu^{-}", 40, -5, 5, 40,  -5, 5);
  Hpxpy2 -> SetXTitle("p_{x} [GeV]");
  Hpxpy2 -> SetYTitle("p_{y} [GeV]");
 
  TH2F *Hpxpz1 = new TH2F("Hpxpz1","dimuon dpz vs #mu^{+} px", 40, -5, 5, 100,  20, 120);
  Hpxpz1 -> SetXTitle("p_{x} [GeV]");
  Hpxpz1 -> SetYTitle("p_{z} [GeV]");

  TH2F *Hpxpz2 = new TH2F("Hpxpz2","dimuon dpz vs #mu^{-} px", 40, -5, 5, 100, 20, 120);
  Hpxpz2 -> SetXTitle("p_{x} [GeV]");
  Hpxpz2 -> SetYTitle("p_{z} [GeV]");

  TH2F *Hpypz1 = new TH2F("Hpypz1","dimuon dpz vs #mu^{+} py", 40, -5, 5, 100,  20, 120);
  Hpypz1 -> SetXTitle("p_{y} [GeV]");
  Hpypz1 -> SetYTitle("p_{z} [GeV]");

  TH2F *Hpypz2 = new TH2F("Hpypz2","dimuon dpz vs #mu^{-} py", 40, -5, 5, 100, 20, 120);
  Hpypz2 -> SetXTitle("p_{y} [GeV]");
  Hpypz2 -> SetYTitle("p_{z} [GeV]");

  TH1F *Hpx01 = new TH1F("Hpx01","dpx of  #mu^{+}, #mu^{-}  target", 40, -4, 4);
  TH1F *Hpy01 = new TH1F("Hpy01","dpy of  #mu^{+}, #mu^{-} target", 40, -4, 4);
  TH1F *Hpz01 = new TH1F("Hpz01","dpz of  #mu^{+}, #mu^{-} target", 100, 20, 120);
  TH1F *Hpx02 = new TH1F("Hpx02","dpx of  #mu^{-}, #mu^{-} target", 40, -4, 4);
  TH1F *Hpy02 = new TH1F("Hpy02","dpy of  #mu^{-}, #mu^{-} target", 40, -4, 4);
  TH1F *Hpz02 = new TH1F("Hpz02","dpz of  #mu^{-}, #mu^{-} target", 100, 20, 120);

  Hpx01 -> SetXTitle("px [GeV]");
  Hpy01 -> SetXTitle("py [GeV]");
  Hpz01 -> SetXTitle("pz [GeV]");
  Hpx01 -> Sumw2();
  Hpy01 -> Sumw2();
  Hpz01 -> Sumw2();
  Hpx02 -> SetXTitle("px [GeV]");
  Hpy02 -> SetXTitle("py [GeV]");
  Hpz02 -> SetXTitle("pz [GeV]");
  Hpx02 -> Sumw2();
  Hpy02 -> Sumw2();
  Hpz02 -> Sumw2();
  Hpx02 -> SetLineColor(2);
  Hpy02 -> SetLineColor(2);
  Hpz02 -> SetLineColor(2);

  TF1 *Fpx0 = new TF1("Fpx0","gaus",-1,1);
  TF1 *Fpy0 = new TF1("Fpy0","gaus",-1,1);

  const int nbins = 4;
  TH1F *Hpx11[nbins];
  TH1F *Hpy11[nbins];
  TH1F *Hpz11[nbins];
  TH1F *Hpx12[nbins];
  TH1F *Hpy12[nbins];
  TH1F *Hpz12[nbins];
  int rangelow[nbins], rangehigh[nbins];
  TF1 *Fpx0[nbins];
  TF1 *Fpy0[nbins];

  for(int i = 0; i < nbins; i++){

    rangelow[i] = i*50 - 50;
    rangehigh[i] = i*50;

    sprintf(Hname, "Hpx11_%d", i);
    sprintf(Tname, "dpx of #mu^{+}, #mu^{-} dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpx11[i] = new TH1F(Hname, Tname, 40, -4, 4);
    sprintf(Hname, "Hpy11_%d", i);
    sprintf(Tname, "dpy of  #mu^{+}, #mu^{-} dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpy11[i] = new TH1F(Hname, Tname, 40, -4, 4);
    sprintf(Hname, "Hpz11_%d", i);
    sprintf(Tname, "dpz of  #mu^{+}, #mu^{-} dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpz11[i] = new TH1F(Hname, Tname, 100, 20, 120);

    Hpx11[i] -> SetXTitle("px [GeV]");
    Hpy11[i] -> SetXTitle("py [GeV]");
    Hpz11[i] -> SetXTitle("pz [GeV]");

    Hpx11[i] -> Sumw2();
    Hpy11[i] -> Sumw2();
    Hpz11[i] -> Sumw2();

    sprintf(Hname, "Hpx12_%d", i);
    sprintf(Tname, "dpx of  #mu^{-} dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpx12[i] = new TH1F(Hname, Tname, 40, -4, 4);
    sprintf(Hname, "Hpy12_%d", i);
    sprintf(Tname, "dpy of  #mu^{-} dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpy12[i] = new TH1F(Hname, Tname, 40, -4, 4);
    sprintf(Hname, "Hpz12_%d", i);
    sprintf(Tname, "dpz of  #mu^{-} dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hpz12[i] = new TH1F(Hname, Tname, 100, 20, 120);

    Hpx12[i] -> SetXTitle("px [GeV]");
    Hpy12[i] -> SetXTitle("py [GeV]");
    Hpz12[i] -> SetXTitle("pz [GeV]");

    Hpx12[i] -> Sumw2();
    Hpy12[i] -> Sumw2();
    Hpz12[i] -> Sumw2();

    Hpx12[i] -> SetLineColor(2);
    Hpy12[i] -> SetLineColor(2);
    Hpz12[i] -> SetLineColor(2);
  }

  //initialize kdimuon tree
  Int_t dimuonID;
  Int_t runID;
  Int_t spillID, eventID;
  Int_t negTrackID, posTrackID;
  Float_t xF, xT, xB, mass;
  Float_t dx, dy, dz;
  Float_t px, py, pz;
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

  int nentries = dmtree -> GetEntries();
  cout << "The number of Entries is " << nentries << endl;
  int zbin;
  for(int i = 0; i < nentries; i++){
    dmtree -> GetEntry(i);

    //if(runID > 10091 && runID < 10185)continue;
    //if((m3hm < -2.5 || m3hm > 1.2) || (m3vm < 1. || m3vm > 4.6))continue;

    //if(spot == 1 && (runID < 10091 || runID > 10185))continue;

    if(spot == 0 && m3vm < 3.10) continue;
    if(spot == 1 && m3vm > 3.10) continue;
    if(spot == 2 && m3hm > -0.71) continue;
    if(spot == 3 && m3hm < -0.71) continue;
    if(spot == 4 && (m3hm < -0.71 || m3vm > 3.10)) continue;
    Hm3m -> Fill(m3hm,m3vm);
  
    if(target == 1){
      Hpx01 -> Fill(px1);
      Hpy01 -> Fill(py1);
      Hpz01 -> Fill(pz1);
      Hpx02 -> Fill(px2);
      Hpy02 -> Fill(py2);
      Hpz02 -> Fill(pz2);
      Hpxpy1 -> Fill(px1, py1);
      Hpxpz1 -> Fill(px1, dpz);
      Hpypz1 -> Fill(py1, dpz);
      Hpxpy2 -> Fill(px2, py2);
      Hpxpz2 -> Fill(px2, dpz);
      Hpypz2 -> Fill(py2, dpz);
    }
  
    if(dump == 1 && dz > -50. && dz < 150.){
      zbin = dz*0.02 + 1;
      Hpx11[zbin] -> Fill(px1);
      Hpy11[zbin] -> Fill(py1);
      Hpz11[zbin] -> Fill(pz1);
      Hpx12[zbin] -> Fill(px2);
      Hpy12[zbin] -> Fill(py2);
      Hpz12[zbin] -> Fill(pz2);
      Hpxpy1 -> Fill(px1, py1);
      Hpxpz1 -> Fill(px1, dpz);
      Hpypz1 -> Fill(py1, dpz);
      Hpxpy2 -> Fill(px2, py2);
      Hpxpz2 -> Fill(px2, dpz);
      Hpypz2 -> Fill(py2, dpz);

    }
  }

  //get mean px,py for pz points mu+
  TH1D *Hpxpz1bins[20];
  TH1D *Hpypz1bins[20];
  TGraph *Gpxpz1 = new TGraph();
  Gpxpz1 -> SetMarkerStyle(21);
  Gpxpz1 -> SetTitle("#mu^{+,-} p_{x} vs Dimuon p_{z}");
  TGraph *Gpypz1 = new TGraph();
  Gpypz1 -> SetMarkerStyle(21);
  Gpypz1 -> SetMarkerColor(2);
  Gpypz1 -> SetTitle("#mu^{+,-} p_{y} vs Dimuon p_{z}");
  Int_t blow, bhigh;
  Double_t gpxpz_x1[9] = {35, 45, 55, 65, 75, 85, 95, 105, 115};
  Double_t gpxpz_y1[9];
  Double_t gpypz_x1[9] = {35, 45, 55, 65, 75, 85, 95, 105, 115};
  Double_t gpypz_y1[9];
  for(int b = 0; b < 9; b++){
    blow = b*10+11;
    bhigh = b*10+21;
    sprintf(Hname, "Hpxpz1bins_dpz%d", b);
    Hpxpz1bins[b] = new TH1D(Hname, Hname, 100, 20, 120);
    Hpxpz1bins[b] = Hpxpz1 -> ProjectionX("Hname", blow, bhigh);
    gpxpz_y1[b] = Hpxpz1bins[b] -> GetMean();
    sprintf(Hname, "Hpypz1bins_dpz%d", b);
    Hpypz1bins[b] = new TH1D(Hname, Hname, 100, 20, 120);
    Hpypz1bins[b] = Hpypz1 -> ProjectionX("Hname", blow, bhigh);
    gpypz_y1[b] = Hpypz1bins[b] -> GetMean();

    //cout << b << " " << gpxpz_x1[b] << " " << gpxpz_y1[b] << endl;
    //cout << b << " " << gpypz_x1[b] << " " << gpypz_y1[b] << endl;
    Gpxpz1 -> SetPoint(b, gpxpz_x1[b], gpxpz_y1[b]);
    Gpypz1 -> SetPoint(b, gpypz_x1[b], gpypz_y1[b]);
    
  }

  Gpxpz1 -> GetXaxis() -> SetTitle("dimuon pz [GeV]");
  Gpxpz1 -> GetYaxis() -> SetTitle("#mu^{+,-} px [GeV]");
  Gpypz1 -> GetXaxis() -> SetTitle("dimuon pz [GeV]");
  Gpypz1 -> GetYaxis() -> SetTitle("#mu^{+,-} py [GeV]");


  //get mean px,py for pz points mu-
  TH1D *Hpxpz2bins[20];
  TH1D *Hpypz2bins[20];
  TGraph *Gpxpz2 = new TGraph();
  Gpxpz2 -> SetMarkerStyle(25);
  Gpxpz2 -> SetTitle("#mu^{+,-} p_{x} vs Dimuon p_{z}");
  TGraph *Gpypz2 = new TGraph();
  Gpypz2 -> SetMarkerStyle(25);
  Gpypz2 -> SetMarkerColor(2);
  Gpypz2 -> SetTitle("#mu^{+,-} p_{x} vs Dimuon p_{z}");
  Double_t gpxpz_x2[9] = {35, 45, 55, 65, 75, 85, 95, 105, 115};
  Double_t gpxpz_y2[9];
  Double_t gpypz_x2[9] = {35, 45, 55, 65, 75, 85, 95, 105, 115};
  Double_t gpypz_y2[9];
  for(int b = 0; b < 9; b++){
    blow = b*10+11;
    bhigh = b*10+21;
    sprintf(Hname, "Hpxpz2bins_dpz%d", b);
    Hpxpz2bins[b] = new TH1D(Hname, Hname, 100, 20, 120);
    Hpxpz2bins[b] = Hpxpz2 -> ProjectionX("Hname", blow, bhigh);
    gpxpz_y2[b] = Hpxpz2bins[b] -> GetMean();
    sprintf(Hname, "Hpypz2bins_dpz%d", b);
    Hpypz2bins[b] = new TH1D(Hname, Hname, 100, 20, 120);
    Hpypz2bins[b] = Hpypz2 -> ProjectionX("Hname", blow, bhigh);
    gpypz_y2[b] = Hpypz1bins[b] -> GetMean();

    //cout << b << " " << gpxpz_x1[b] << " " << gpxpz_y1[b] << endl;
    //cout << b << " " << gpxpz_x2[b] << " " << gpxpz_y2[b] << endl;
    Gpxpz2 -> SetPoint(b, gpxpz_x2[b], fabs(gpxpz_y2[b]));
    Gpypz2 -> SetPoint(b, gpypz_x2[b], gpypz_y2[b]);
    
  }

  Gpxpz1 -> SetMaximum(2.51);
  Gpxpz1 -> SetMinimum(1.89);
  Gpypz1 -> SetMaximum(0.23);
  Gpypz1 -> SetMinimum(-0.23);
  Gpxpz2 -> GetXaxis() -> SetTitle("dimuon pz [GeV]");
  Gpxpz2 -> GetYaxis() -> SetTitle("#mu^{+,-} p_{x} [GeV]");
  Gpypz2 -> GetXaxis() -> SetTitle("dimuon pz [GeV]");
  Gpypz2 -> GetYaxis() -> SetTitle("#mu^{+,-} p_{y} [GeV]");
  //exit(1);

  //get the average value of px, py and make graphs
  Double_t gpx_x1[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpx_y1[nbins+1];
  TGraph *Gpx1 = new TGraph();
  Gpx1 -> SetMarkerStyle(21);
  Gpx1 -> SetFillColor(0);
  Double_t gpy_x1[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpy_y1[nbins+1];
  TGraph *Gpy1 = new TGraph();
  Gpy1 -> SetMarkerStyle(21);
  Gpy1 -> SetMarkerColor(2);
  Gpy1 -> SetFillColor(0);
  Double_t gpz_x1[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpz_y1[nbins+1];
  TGraph *Gpz1 = new TGraph();
  Gpz1 -> SetMarkerStyle(21);
  Gpz1 -> SetMarkerColor(4);
  Gpz1 -> SetFillColor(0);
  for(int i = 0; i < nbins+1; i++){
    if(i == 0){
      gpx_y1[i] = Hpx01 -> GetMean();
      gpy_y1[i] = Hpy01 -> GetMean();
      gpz_y1[i] = Hpz01-> GetMean() / 1000.;
    }
    else{
      gpx_y1[i] = Hpx11[i-1] -> GetMean();
      //if(fmag == 1 && i > 2){gpx_y[i] = gpx_y[i] + 0.075*(i-2);}
      gpy_y1[i] = Hpy11[i-1] -> GetMean();
      gpz_y1[i] = Hpz11[i-1] -> GetMean() / 1000.;
    }
    Gpx1 -> SetPoint(i, gpx_x1[i], gpx_y1[i]);
    Gpy1 -> SetPoint(i, gpy_x1[i], gpy_y1[i]);
    Gpz1 -> SetPoint(i, gpz_x1[i], gpz_y1[i]);
  }
  Gpx1 -> SetMaximum(2.51);
  Gpx1 -> SetMinimum(1.89);
  Gpy1 -> SetMaximum(0.23);
  Gpy1 -> SetMinimum(-0.23);
  Gpx1 -> SetTitle("#mu^{+,-} p_{x} vs Dimuon zvtx");
  Gpx1 -> GetXaxis() -> SetTitle("zvtx [cm]");
  Gpx1 -> GetYaxis() -> SetTitle("#mu^{+,-} p_{x} [GeV]");
  Gpx1 -> SetTitle("#mu^{+,-} p_{y} vs Dimuon zvtx");
  Gpy1 -> GetXaxis() -> SetTitle("zvtx [cm]");
  Gpy1 -> GetYaxis() -> SetTitle("#mu^{+,-} p_{y} [GeV]");
  
 Double_t gpx_x2[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpx_y2[nbins+1];
  TGraph *Gpx2 = new TGraph();
  Gpx2 -> SetMarkerStyle(25  );
  Gpx2 -> SetFillColor(0);
  Double_t gpy_x2[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpy_y2[nbins+1];
  TGraph *Gpy2 = new TGraph();
  Gpy2 -> SetMarkerStyle(25);
  Gpy2 -> SetMarkerColor(2);
  Gpy2 -> SetFillColor(0);
  Double_t gpz_x2[nbins+1] = {-130, -25., 25., 75., 125.};
  Double_t gpz_y2[nbins+1];
  TGraph *Gpz2 = new TGraph();
  Gpz2 -> SetMarkerStyle(25);
  Gpz2 -> SetMarkerColor(4);
  Gpz2 -> SetFillColor(0);
  for(int i = 0; i < nbins+1; i++){
    if(i == 0){
      gpx_y2[i] = Hpx02 -> GetMean();
      gpy_y2[i] = Hpy02 -> GetMean();
      gpz_y2[i] = Hpz02 -> GetMean() / 1000.;
    }
    else{
      gpx_y2[i] = Hpx12[i-1] -> GetMean();
      //if(fmag == 1 && i > 2){gpx_y[i] = gpx_y[i] + 0.075*(i-2);}
      gpy_y2[i] = Hpy12[i-1] -> GetMean();
      gpz_y2[i] = Hpz12[i-1] -> GetMean() / 1000.;
    }
    Gpx2 -> SetPoint(i, gpx_x2[i], fabs(gpx_y2[i]));
    Gpy2 -> SetPoint(i, gpy_x2[i], gpy_y2[i]);
    Gpz2 -> SetPoint(i, gpz_x2[i], gpz_y2[i]);
  }
  //Gpx2 -> SetTitle("Mean p_{x,y,z} of dimuons vs zvtx");
  //Gpx2 -> GetXaxis() -> SetTitle("dz [cm]");
  //Gpx2 -> GetYaxis() -> SetTitle("dp_{x,y,z} [GeV]");


  TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
  TLegend *legend2 = new TLegend(0.7,0.7,0.9,0.9);
  legend1 -> AddEntry(Gpx1, "dp_{x} of #mu^{+}");
  legend2 -> AddEntry(Gpy1, "dp_{y} of #mu^{+}");
  legend1 -> AddEntry(Gpx2, "|dp_{x}| of #mu^{-}");
  legend2 -> AddEntry(Gpy2, "dp_{y} of #mu^{-}");

  TLine *m3h = new TLine(-5,0,5,0);
  TLine *m3v = new TLine(0,-5,0,5);
  m3h -> SetLineStyle(2);
  m3v -> SetLineStyle(2);

  TLine *ldpz = new TLine(30, 0,  120, 0);
  TLine *ldz = new TLine(-150,0,150,0);
  ldpz -> SetLineStyle(2);
  ldz -> SetLineStyle(2);


  TCanvas *c2 = new TCanvas("c2","c2",2000,800);
  c2 -> Divide(4,2);
  c2 -> cd(1);
  Hpxpz1 -> Draw("colz");
  m3h -> Draw();
  c2 -> cd(2);
  Hpxpz2 -> Draw("colz");
  m3h -> Draw();
  c2 -> cd(3);
  Gpxpz1 -> Draw("ap");
  Gpxpz2 -> Draw("p");
  ldpz -> Draw();
  c2 -> cd(4);
  Gpx1 -> Draw("ap");
  Gpx2 -> Draw("p");
  legend1 -> Draw();
  ldz -> Draw();
  c2 -> cd(5);
  Hpypz1 -> Draw("colz");
  m3h -> Draw();
  c2 -> cd(6);
  Hpypz2 -> Draw("colz");
  m3h -> Draw(); 
  c2 -> cd(7);
  Gpypz1 -> Draw("ap");
  Gpypz2 -> Draw("p");
  ldpz -> Draw();
  c2 -> cd(8);
  Gpy1 -> Draw("ap");
  Gpy2 -> Draw("p");
  legend2 -> Draw();
  ldz -> Draw();
  sprintf(Hname,"./images/dp_all_spot%d.gif", spot);
  c2 -> SaveAs(Hname);
  /*
  TCanvas *c3 = new TCanvas("c3","c3",1800,400);
  c3 -> Divide(3,1);
  c3 -> cd(1);
  //Hpx0 -> Fit(Fpx0,"R","",-1,1);
  Hpx01 -> Draw();
  Hpx02 -> Draw("same");
  c3 -> cd(2);
  //Hpy0 -> Fit(Fpy0,"R","",-1,1);
  Hpy01 -> Draw();
  Hpy02 -> Draw("same");
  c3 -> cd(3);
  Hpz01 -> Draw();
  Hpz02 -> Draw("same");
  c3 -> SaveAs("./images/dp_target.gif");

  TCanvas *cx8 = new TCanvas("cx8","cx8",1200,800);
  cx8 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cx8 -> cd(i+1);
    Hpx11[i] -> Draw();
    Hpx12[i] -> Draw("same");

  }
  cx8 -> cd();
  cx8 -> SaveAs("./images/dpx_dump.gif");
  
  TCanvas *cy8 = new TCanvas("cy8","cy8",1200,800);
  cy8 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cy8 -> cd(i+1);
    Hpy11[i] -> Draw();
    Hpy12[i] -> Draw("same");
  }
  cy8 -> cd();
  cy8 -> SaveAs("./images/dpy_dump.gif");
 
  TCanvas *cz8 = new TCanvas("cz8","cz8",1200,800);
  cz8 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cz8 -> cd(i+1);
    Hpz11[i] -> Draw();
    Hpz12[i] -> Draw("same");

  }
  cz8 -> cd();
  cz8 -> SaveAs("./images/dpz_dump.gif");
 
  */
  Gpy1 -> Print();
  Gpy2 -> Print();
  
}
