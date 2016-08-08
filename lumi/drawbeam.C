
void drawbeam(const float xdrift = 0., const float ydrift = 0.)
{
  int nentries = 1000000;
  float xbeam = 6.8;
  float ybeam = 7.6;
  float sigmaswic = 2.5;
  float xswic = sigmaswic*xbeam;
  float yswic = sigmaswic*ybeam;
  
  //center 
  TH2F *Hbeam = new TH2F("Hbeam","Hbeam", 80, -40, 40, 80, -40, 40);
  TH2F *Hpos = new TH2F("Hpos","Hpos", 80, -40, 40, 80, -40, 40);
  TH2F *Hneg = new TH2F("Hneg","Hneg", 80, -40, 40, 80, -40, 40);
  Hbeam -> SetTitle("Beam Middle");
  Hbeam -> SetXTitle("x (mm)");
  Hbeam -> SetYTitle("y (mm)");
  Hpos -> SetTitle("Beam Drift Positive");
  Hpos -> SetXTitle("x (mm)");
  Hpos -> SetYTitle("y (mm)");
  Hneg -> SetTitle("Beam Drift Negative");
  Hneg -> SetXTitle("x (mm)");
  Hneg -> SetYTitle("y (mm)");
  //TF2 *ell = new TF2("ell","pow(y/sqrt(a) + x/sqrt(b) ,2)+pow(y/sqrt(c) + x/sqrt(d) ,2)<e",-6.,6.,-10.0,10.0);

  float xvtx, yvtx;
  float xvtx_pos, yvtx_pos;
  float xvtx_neg, yvtx_neg;

  for(int i = 0; i < nentries; i++){
    if(i%1000000 == 0) cout << "Event " << i << endl;
    xvtx = gRandom -> Gaus(0., 6.8); 
    yvtx = gRandom -> Gaus(0., 7.6);
    xvtx_pos = gRandom -> Gaus(xdrift, xbeam);
    yvtx_pos = gRandom -> Gaus(ydrift, ybeam);
    xvtx_neg = gRandom -> Gaus(-1*xdrift, xbeam);
    yvtx_neg = gRandom -> Gaus(-1*ydrift, ybeam);
    //xvtx_pos = xvtx - xdrift;
    //yvtx_pos = yvtx - ydrift;

    if(xvtx*xvtx/xswic/xswic + yvtx*yvtx/yswic/yswic < 1){
    //if(xvtx*xvtx/17/17 + yvtx*yvtx/19/19 < 1){
      //if(pow(xvtx/2.5/xswic, 2) + pow(yvtx/2.5/yswic,2) < 1){
      Hbeam -> Fill(xvtx, yvtx);
      xvtx_pos = xvtx - xdrift;
      yvtx_pos = yvtx - ydrift;
      if(xvtx_pos*xvtx_pos/19/19 + yvtx_pos*yvtx_pos/21/21 < 1){
      Hpos -> Fill(xvtx-xdrift, yvtx-ydrift);
      }
    }
    if(xvtx_pos*xvtx_pos/xswic/xswic + yvtx_pos*yvtx_pos/yswic/yswic < 1){
      //Hpos -> Fill(xvtx_pos, yvtx_pos);
    }
    if(xvtx_neg*xvtx_neg/xswic/xswic + yvtx_neg*yvtx_neg/yswic/yswic < 1){
      Hneg -> Fill(xvtx_neg, yvtx_neg);
    }
  }
  
  TLine *h0 = new TLine(-40,0,40,0);
  TLine *v0 = new TLine(0,-40,0,40);
  h0 ->SetLineStyle(2);
  v0 ->SetLineStyle(2);
 
  
  TEllipse *E2p5sigma = new TEllipse(0., 0., 17.,19.);
  E2p5sigma -> SetLineColor(1);
  E2p5sigma -> SetLineWidth(4);
  E2p5sigma -> SetLineStyle(2);
  E2p5sigma -> SetFillStyle(0);

  TEllipse *Ebeam = new TEllipse(0., 0., 19.,21.);
  Ebeam -> SetLineColor(6);
  Ebeam -> SetLineWidth(4);
  Ebeam -> SetFillStyle(0);
  
  TCanvas *c1 = new TCanvas("c1","c1", 1500, 500);
  c1 -> Divide(3, 1);
  c1 -> cd(1);
  Hbeam -> Draw("colz");
  Ebeam->Draw(); E2p5sigma -> Draw();
  h0 -> Draw(); v0 -> Draw();
  c1 -> cd(2);
  Hpos -> Draw("colz");
  Ebeam->Draw(); E2p5sigma -> Draw();
  h0 -> Draw(); v0 -> Draw();
  c1 -> cd(3);
  Hneg -> Draw("colz");
  Ebeam->Draw(); E2p5sigma -> Draw();
  h0 -> Draw(); v0 -> Draw();

  cout << "Percent of protons in target:  " << Hbeam -> GetEntries()/nentries << endl;
  cout << "Percent of protons in target:  " << Hpos -> GetEntries()/nentries << endl;
  cout << "Relative Luminosity:  " << Hpos -> GetEntries() / Hbeam -> GetEntries() << endl;

  c1 -> SaveAs("./beamprofile.gif");
  
}
  
