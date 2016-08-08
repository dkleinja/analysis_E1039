
void drawbeam2(const float xdrift = 0., const float ydrift = 0.)
{
  gStyle->SetStatW(0.4); // Set width of stat-box (fraction of pad size)
  gStyle->SetStatH(0.2); // Set height of stat-box (fraction of pad size)
  
  gStyle -> SetOptStat(10);
  int nentries = 1000000;
  float xbeam = 6.8;
  float ybeam = 7.6;
  float xtarget = 9.5;
  float ytarget = 10.5;
  float sigmaswic = 1.25;
  float xswic = sigmaswic*xbeam;
  float yswic = sigmaswic*ybeam;

  //center 
  TH2F *Hbeam = new TH2F("Hbeam","Hbeam", 150, -15, 15, 150, -15, 15);
  TH2F *Hpos = new TH2F("Hpos","Hpos", 150, -15, 15, 150, -15, 15);
  TH2F *Hneg = new TH2F("Hneg","Hneg", 150, -15, 15, 150, -15, 15);
  Hbeam -> SetTitle("Beam Middle");
  Hbeam -> SetXTitle("x (mm)");
  Hbeam -> SetYTitle("y (mm)");
  Hpos -> SetTitle("Beam Drift Gained");
  Hpos -> SetXTitle("x (mm)");
  Hpos -> SetYTitle("y (mm)");
  Hneg -> SetTitle("Beam Drift Lost");
  Hneg -> SetXTitle("x (mm)");
  Hneg -> SetYTitle("y (mm)");
  //TF2 *ell = new TF2("ell","pow(y/sqrt(a) + x/sqrt(b) ,2)+pow(y/sqrt(c) + x/sqrt(d) ,2)<e",-6.,6.,-10.0,10.0);

  float xvtx, yvtx;
  float xvtx_pos, yvtx_pos;
  float xvtx_neg, yvtx_neg;
  float xvtx_shift, yvtx_shift;

  for(int i = 0; i < nentries; i++){
    if(i%1000000 == 0) cout << "Event " << i << endl;
    xvtx = gRandom -> Gaus(0., xbeam); 
    yvtx = gRandom -> Gaus(0., ybeam);
    xvtx_pos = gRandom -> Gaus(xdrift, xbeam);
    yvtx_pos = gRandom -> Gaus(ydrift, ybeam);
    xvtx_neg = gRandom -> Gaus(-1*xdrift, xbeam);
    yvtx_neg = gRandom -> Gaus(-1*ydrift, ybeam);
    xvtx_pos = xvtx + xdrift;
    yvtx_pos = yvtx + ydrift;

    if(xvtx*xvtx/xswic/xswic + yvtx*yvtx/yswic/yswic < 1){
    //if(xvtx*xvtx/xswic/xswic < 1 && yvtx*yvtx/yswic/yswic < 1){
      Hbeam -> Fill(xvtx, yvtx);
      xvtx_shift = xvtx + xdrift;
      yvtx_shift = yvtx + ydrift;
      //if(xvtx_shift*xvtx_shift/xswic/xswic + yvtx_shift*yvtx_shift/yswic/yswic > 1){
      //Hpos -> Fill(xvtx_pos, yvtx_pos);
      //if(xvtx_shift*xvtx_shift/xtarget/xtarget + yvtx_shift*yvtx_shift/ytarget/ytarget > 1){
      //if(xvtx_neg*xvtx_neg/xswic/xswic + yvtx_neg*yvtx_neg/yswic/yswic > 1){
	Hneg -> Fill(xvtx_shift, yvtx_shift);
	//}
    }
    if(xvtx_pos*xvtx_pos/(xswic)/(xswic) + yvtx_pos*yvtx_pos/(yswic)/(yswic) < 1){
      if((xvtx_pos-xdrift)*(xvtx_pos-xdrift)/(xswic)/(xswic) + (yvtx_pos-ydrift)*(yvtx_pos-ydrift)/(yswic)/(yswic) > 1){
	//if((xvtx_pos+xdrift)*(xvtx_pos+xdrift)/(xswic)/(xswic) + (yvtx_pos+ydrift)*(yvtx_pos+ydrift)/(yswic)/(yswic) < 1){
	Hpos -> Fill(xvtx_pos, yvtx_pos);
      }
    }
  }
  
  TLine *h0 = new TLine(-15,0,15,0);
  TLine *v0 = new TLine(0,-15,0,15);
  h0 ->SetLineStyle(2);
  v0 ->SetLineStyle(2);
 
  TEllipse *E2p5sigma = new TEllipse(0., 0., xbeam*sigmaswic,ybeam*sigmaswic);
  E2p5sigma -> SetLineColor(1);
  E2p5sigma -> SetLineWidth(4);
  E2p5sigma -> SetLineStyle(2);
  E2p5sigma -> SetFillStyle(0);
  
  TEllipse *Elost = new TEllipse(xdrift, ydrift, xbeam*sigmaswic,ybeam*sigmaswic);
  Elost -> SetLineColor(2);
  Elost -> SetLineWidth(4);
  Elost -> SetLineStyle(2);
  Elost -> SetFillStyle(0);
  TEllipse *Egain = new TEllipse(xdrift, ydrift, xbeam*sigmaswic,ybeam*sigmaswic);
  Egain -> SetLineColor(3);
  Egain -> SetLineWidth(4);
  Egain -> SetLineStyle(2);
  Egain -> SetFillStyle(0);

  TEllipse *Ebeam = new TEllipse(0., 0., xtarget,ytarget);
  Ebeam -> SetLineColor(6);
  Ebeam -> SetLineWidth(4);
  Ebeam -> SetFillStyle(0);
  
  TCanvas *c3 = new TCanvas("c3","c3", 1800, 500);
  c3 -> Divide(3, 1);
  c3 -> cd(1);
  Hbeam -> Draw("colz");
  Ebeam->Draw(); //E2p5sigma -> Draw();
  //Egain -> Draw();Elost -> Draw();
  h0 -> Draw(); v0 -> Draw();
  c3 -> cd(2);
  Hpos -> Draw("colz");
  Ebeam->Draw();// E2p5sigma -> Draw();
  //Egain -> Draw();
  h0 -> Draw(); v0 -> Draw();
  c3 -> cd(3);
  Hneg -> Draw("colz");
  Ebeam->Draw(); //E2p5sigma -> Draw();
  //Elost -> Draw();
  h0 -> Draw(); v0 -> Draw();

  TCanvas *c2 = new TCanvas("c2","c2", 1200, 500);
  c2 -> Divide(2, 1);
  c2 -> cd(1);
  Hbeam -> Draw("colz");
  Ebeam->Draw(); //E2p5sigma -> Draw();
  //Egain -> Draw();Elost -> Draw();
  h0 -> Draw(); v0 -> Draw();
  c2 -> cd(2);
  Hneg -> Draw("colz");
  Ebeam->Draw();// E2p5sigma -> Draw();
  //Egain -> Draw();
  h0 -> Draw(); v0 -> Draw();


  cout << "Number of protons in target after swic:  " << Hbeam -> GetEntries() << endl;
  //cout << "Percent of protons in target:  " << Hpos -> GetEntries()/nentries << endl;
  cout << "Lost due to beam shift, #DELTAN_{L} " << Hneg -> GetEntries()  << endl;
  cout << "Gained due to beam shift, #DELTAN_{G} " << Hpos -> GetEntries() << endl;
  
  //cout << "The error due to beam shift is:  " << sqrt ( (pow(Hpos -> GetEntries(), 2) + pow(Hneg -> GetEntries(), 2) ) / pow(Hbeam -> GetEntries(), 2) ) << endl;
  cout << "The error due to beam shift is:  " << fabs(Hneg -> GetEntries() -  Hpos -> GetEntries()) /  Hbeam -> GetEntries() << endl;

  c2 -> SaveAs("./beamprofile.gif");
  
}
  
