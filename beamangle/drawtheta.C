void drawtheta(const int roadset = 57, const int spot = 5)
{
  gStyle -> SetOptFit(1);

  char Fname[128];
  char Hname[128];
  char Tname[128];

  sprintf(Fname, "./nDST/BeamAngle2_kdimuon.root");
  sprintf(Fname, "./nDST/BeamAngle_kdimuon.root");
  sprintf(Fname, "./nDST/MagFlip_kdimuon.root");
  sprintf(Fname, "./nDST/Analysis_roadset%d_R005_V001.root", roadset);
  //sprintf(Fname, "./nDST/Analysis_mc_drellyan_LD2_M014_S019.root");
  TFile *inFile = new TFile(Fname);
  TTree *dmtree = (TTree*) inFile -> Get("kdimuon");

  TH2F *Hm3m = new TH2F("Hm3m","M3 Vert Mean vs. Hori Mean", 40, -5, 5, 40,  -5, 5);
  Hm3m -> SetXTitle("hm (mm)");
  Hm3m -> SetYTitle("vm (mm)");

  TH2F *Hpxpy = new TH2F("Hpxpy","M3 Vert Mean vs. Hori Mean", 40, -4, 4, 40,  -4, 4);
  Hpxpy -> SetXTitle("p_{x} (GeV)");
  Hpxpy -> SetYTitle("p_{y} (GeV)");

  TH1F *Hthetax0 = new TH1F("Hthetax0","beam angle of Dimuons target", 200, -0.1, 0.1);
  TH1F *Hthetay0 = new TH1F("Hthetay0","beam angle of Dimuons target", 200, -0.1, 0.1);

  Hthetax0 -> SetXTitle("#theta (radians)");
  Hthetay0 -> SetXTitle("#theta (radians)");

  Hthetax0 -> Sumw2();
  Hthetay0 -> Sumw2();

  TF1 *Fpx0 = new TF1("Fpx0","gaus",-1,1);
  TF1 *Fpy0 = new TF1("Fpy0","gaus",-1,1);

  const int nbins = 4;
  TH1F *Hthetax1[nbins];
  TH1F *Hthetay1[nbins];
  int rangelow[nbins], rangehigh[nbins];
  TF1 *Fpx0[nbins];
  TF1 *Fpy0[nbins];

  for(int i = 0; i < nbins; i++){

    rangelow[i] = i*50 - 50;
    rangehigh[i] = i*50;

    sprintf(Hname, "Hthetax1_%d", i);
    sprintf(Tname, "beam angle of Dimuons dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hthetax1[i] = new TH1F(Hname, Tname, 200, -0.1, 0.1);
    sprintf(Hname, "Hthetay1_%d", i);
    sprintf(Tname, "beam angle of Dimuons dump [%d < z < %d cm]", rangelow[i], rangehigh[i]);
    Hthetay1[i] = new TH1F(Hname, Tname, 200, -0.1, 0.1.);
   
    Hthetax1[i] -> SetXTitle("#theta (rad)");
    Hthetay1[i] -> SetXTitle("#theta (rad)");
	
    Hthetax1[i] -> Sumw2();
    Hthetay1[i] -> Sumw2();

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
  Float_t p1, p2;
  Float_t trackSeparation, chisq_dimuon;
  Int_t target, dump;
  Int_t targetPos;
  Float_t m3hm, m3hs, m3vm, m3vs;
  Float_t m2hm, m2hs, m2vm, m2vs;
  Float_t dhm, dvm;
  Float_t thetax, thetay;
  Float_t q, qx, qy, qz;

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

    if(runID > 10091 && runID < 10185)continue;
    if(runID > 12439 && runID < 12522)continue;
    //if((m3hm < -2.5 || m3hm > 1.2) || (m3vm < 1. || m3vm > 4.6))continue;

    //if(spot == 1 && (runID < 10091 || runID > 10185))continue;

    if(spot == 0 && m3vm < 3.10) continue;
    if(spot == 1 && m3vm > 3.10) continue;
    if(spot == 2 && m3hm > -0.71) continue;
    if(spot == 3 && m3hm < -0.71) continue;
    if(spot == 4 && (m3hm < -0.71 || m3vm > 3.10)) continue;
    Hm3m -> Fill(m3hm,m3vm);
    Hpxpy -> Fill(dpx,dpy);
    p1 = sqrt(pow(px1, 2) + pow(py1, 2) + pow(pz1, 2));
    p2 = sqrt(pow(px2, 2) + pow(py2, 2) + pow(pz2, 2));
    qx = px1 + px2; qy = py1 + py2; qz = pz1 + pz2;
    q = sqrt(pow(qx, 2) + pow(qy, 2) + pow(qz, 2));
    //if(i%1000 == 0) cout << i << " " << p1 + p2 << " " << q << " " << q/(p1+p2)*100 << endl;
    thetax = (px1 + px2)/(p1 + p2);
    thetay = (py1 + py2)/(p1 + p2);
    if(i%100000 == 0) cout << i << " " << sqrt(pow(px1 + px2, 2)) << " " << px1 + px2 << " " << sqrt(pow(px1+px2, 2))/(px1 + px2)*100 << endl;
    //if(i%1000 == 0) cout << i << " " << sqrt(pow(p1 + p2, 2)) << " " << thetax << " " << sqrt(pow(p1+p2, 2))/thetax*100 << endl;
	
    if(target == 1){
      Hthetax0 -> Fill(thetax);
      Hthetay0 -> Fill(thetay);
    }

    if(dump == 1 && dz > -50. && dz < 150.){
      zbin = dz*0.02 + 1;
      //cout << i << " " << zbin << " " << dz << endl;
      Hthetax1[zbin] -> Fill(thetax);
      Hthetay1[zbin] -> Fill(thetay);
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
      gpx_y[i] = Hthetax0 -> GetMean();
      gpy_y[i] = Hthetay0 -> GetMean();
    }
    else{
      gpx_y[i] = Hthetax1[i-1] -> GetMean();
      //if(fmag == 1 && i > 2){gpx_y[i] = gpx_y[i] + 0.075*(i-2);}
      gpy_y[i] = Hthetay1[i-1] -> GetMean();
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
  //legend -> AddEntry(Gpz, "dp_{z} / 1000");

  TLine *m3h = new TLine(-5,0,5,0);
  TLine *m3v = new TLine(0,-5,0,5);
  m3h -> SetLineStyle(2);
  m3v -> SetLineStyle(2);

  if(spot == 1)TBox *box = new TBox(-5,3.04,5,5);
  if(spot == 0)TBox *box = new TBox(-5,3.04,5,-5);
  if(spot == 3)TBox *box = new TBox(-5,-5,-0.68,5);
  if(spot == 2)TBox *box = new TBox(-0.68,-5,5,5);

  TCanvas *c2 = new TCanvas("c2","c2",1200,400);
  c2 -> Divide(2,1);
  c2 -> cd(1);

  Hpxpy -> Draw("colz");
  m3h -> Draw();
  m3v -> Draw();
  //if(spot != 4) box -> Draw();
  c2 -> cd(2);
  Hm3m -> Draw("colz");
  m3h -> Draw();
  m3v -> Draw();
  //Gpx -> Draw("ap");
  //Gpy -> Draw("p");
  //legend -> Draw();

  //Gpz -> Draw("p");
  sprintf(Hname,"./images/dp_all_spot%d.gif", spot);
  c2 -> SaveAs(Hname);
  
  TCanvas *c3 = new TCanvas("c3","c3",1800,400);
  c3 -> Divide(3,1);
  c3 -> cd(1);
  //Hthetax0 -> Fit(Fpx0,"R","",-1,1);
  Hthetax0 -> Draw();
  c3 -> cd(2);
  //Hthetay0 -> Fit(Fpy0,"R","",-1,1);
  Hthetay0 -> Draw();
  c3 -> cd(3);
  c3 -> SaveAs("./images/dp_target.gif");

  TCanvas *cx8 = new TCanvas("cx8","cx8",1200,800);
  cx8 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cx8 -> cd(i+1);
    Hthetax1[i] -> Draw();

  }
  cx8 -> cd();
  cx8 -> SaveAs("./images/dpx_dump.gif");
  
  TCanvas *cy8 = new TCanvas("cy8","cy8",1200,800);
  cy8 -> Divide(2,2);
  for(int i = 0; i < nbins; i++){
    cy8 -> cd(i+1);
    Hthetay1[i] -> Draw();

  }
  cy8 -> cd();
  cy8 -> SaveAs("./images/dpy_dump.gif");

Gpx -> Print();
Gpy -> Print(); 
}
