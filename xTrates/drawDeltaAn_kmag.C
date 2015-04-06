void drawDeltaAn_kmag(const int tarpos =350, const int magpos = 150, const int type = 0)
{

  char Fname[128];
  char Hname[128];

  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos130_magpos0_magval0.root");
  TFile *inFile_e906 = new TFile(Fname);
  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos130_kmagm1.root");
  //sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos%d_kmagm1.root", tarpos, magpos);  
  TFile * inFile_0 = new TFile(Fname);
  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos%d_magpos%d_magvalm1_kmagm1.root", tarpos, magpos);  
  TFile * inFile_m1 = new TFile(Fname);
  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos%d_magpos%d_magvalm2.root", tarpos, magpos);  
  TFile * inFile_m2 = new TFile(Fname);
  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos%d_magpos%d_magvalp1_kmagm1.root", tarpos, magpos);  
  TFile * inFile_p1 = new TFile(Fname);
  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos%d_magpos%d_magvalp2.root", tarpos, magpos);  
  TFile * inFile_p2 = new TFile(Fname);


  if(type == 0){
    TH1F *HxT_e906 = (TH1F*) inFile_e906 -> Get("HxTprop");
    TH1F *HxT_0 = (TH1F*) inFile_0 -> Get("HxTprop");
    TH1F *HxT_m1 = (TH1F*) inFile_m1 -> Get("HxTprop");
    TH1F *HxT_m2 = (TH1F*) inFile_m2 -> Get("HxTprop");
    TH1F *HxT_p1 = (TH1F*) inFile_p1 -> Get("HxTprop");
    TH1F *HxT_p2 = (TH1F*) inFile_p2 -> Get("HxTprop");
  }
  if(type == 1){
    TH1F *HxT_e906 = (TH1F*) inFile_e906 -> Get("HxT");
    TH1F *HxT_0 = (TH1F*) inFile_0 -> Get("HxT");
    TH1F *HxT_m1 = (TH1F*) inFile_m1 -> Get("HxT");
    TH1F *HxT_m2 = (TH1F*) inFile_m2 -> Get("HxT");
    TH1F *HxT_p1 = (TH1F*) inFile_p1 -> Get("HxT");
    TH1F *HxT_p2 = (TH1F*) inFile_p2 -> Get("HxT");

    HxT_e906 -> Rebin(5);
    HxT_0 -> Rebin(5);
    HxT_m1 -> Rebin(5);
    HxT_m2 -> Rebin(5);
    HxT_p1 -> Rebin(5);
    HxT_p2 -> Rebin(5);
  }

  HxT_e906 -> SetMarkerStyle(34);

  sprintf(Hname, "tarpos%d_magpos%d", tarpos, magpos);
  HxT_e906 -> SetTitle(Hname);
  HxT_e906 -> SetMaximum(1.11);
  HxT_e906 -> SetMinimum(-0.01);


  HxT_0 -> SetMarkerStyle(21);
  HxT_m1 -> SetMarkerStyle(21);
  HxT_p1 -> SetMarkerStyle(21);
 
  HxT_m1 -> SetMarkerColor(2);
  HxT_m2 -> SetMarkerColor(2);
  HxT_p1 -> SetMarkerColor(4);
  HxT_p2 -> SetMarkerColor(4);
  /*
  HxT_e906 -> SetFillColor(0);
  HxT_0 -> SetFillColor(0);
  HxT_m1 -> SetFillColor(0);
  HxT_m2 -> SetFillColor(0);
  HxT_p1 -> SetFillColor(0);
  HxT_p2 -> SetFillColor(0);
  HxT_e906 -> SetLineColor(0);
  HxT_0 -> SetLineColor(0);
  HxT_m1 -> SetLineColor(0);
  HxT_m2 -> SetLineColor(0);
  HxT_p1 -> SetLineColor(0);
  HxT_p2 -> SetLineColor(0);
  */
  //TCanvas *c1 = new TCanvas("c1","c1",600,400);
  //TCanvas *c2 = new TCanvas("c2","c2",600,400);
  //c2 -> Divide(2,1);

  //c1 -> cd(1);
  HxT_0 -> Divide(HxT_e906);
  HxT_m1 -> Divide(HxT_e906);
  HxT_m2 -> Divide(HxT_e906);
  HxT_p1 -> Divide(HxT_e906);
  HxT_p2 -> Divide(HxT_e906);


  HxT_0 -> SetTitle("Ratio of Rates, E906/E1039");
  HxT_0 -> SetYTitle("Ratio");
  HxT_0 -> Draw();
  HxT_m1 -> Draw("same");
  HxT_p1 -> Draw("same");
  //HxT_p2 -> Draw("same");
  //HxT_m2 -> Draw("same");


  TLegend *legend = new TLegend(0.5,0.5,0.8,0.8);
  //legend -> AddEntry(HxT_e906,"e906 [-130 cm]");
  legend -> AddEntry(HxT_0,"0 Tesla");
  legend -> AddEntry(HxT_m1,"-1 Tesla");
  //legend -> AddEntry(HxT_m2,"-2 Tesla");
  legend -> AddEntry(HxT_p1,"+1 Tesla");
  //legend -> AddEntry(HxT_p2,"+2 Tesla");
  legend -> Draw();
  
  sprintf(Hname, "./images_xTrates/yield%d_dimuon_target_tarpos%d_magpos%d.gif", tarpos, magpos,type);
  c1 -> SaveAs(Hname);

  for(int i = 0; i < 5; i++){
    cout << "HxT_0 rate " << HxT_p1->GetBinContent(i)<< endl;
  }
  cout << endl;
  for(int i = 0; i < 5; i++){
    cout << "HxT_0 rate " << HxT_p1->GetBinContent(i)<< endl;
  }
  cout << endl;
  for(int i = 0; i < 5; i++){
    cout << "HxT_0 rate " << HxT_p1->GetBinContent(i)<< endl;
  }

}
