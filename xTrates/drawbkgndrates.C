void drawbkgndrates(const int tarpos = 350, const int magpos = 150, const int trig = 0)
{

  char Fname[64];
  char Hname[64];

   
  sprintf(Fname, "./root_files/bkgndH1_e906db1_gun40M_tarpos130_magpos0_magval0.root");
  TFile *inFile2_e906 = new TFile(Fname);
  sprintf(Fname, "./root_files/bkgndH1_gun10M_tarpos%d_magpos%d_magval0.root", tarpos, magpos);  
  TFile * inFile2_0 = new TFile(Fname);
  sprintf(Fname, "./root_files/bkgndH1_e906db1_gun40M_tarpos%d_magpos%d_magvalm1.root", tarpos, magpos);  
  TFile * inFile2_m1 = new TFile(Fname);
  sprintf(Fname, "./root_files/bkgndH1_gun_tarpos%d_magpos%d_magvalm2.root", tarpos, magpos);  
  sprintf(Fname, "./root_files/bkgndH1_gun10M_tarpos%d_magpos%d_magvalm1.root", tarpos, magpos);  
  TFile * inFile2_m2 = new TFile(Fname);
  sprintf(Fname, "./root_files/bkgndH1_gun_tarpos%d_magpos%d_magvalp1.root", tarpos, magpos);  
  sprintf(Fname, "./root_files/bkgndH1_gun10M_tarpos%d_magpos%d_magvalm1.root", tarpos, magpos);  
  TFile * inFile2_p1 = new TFile(Fname);
  sprintf(Fname, "./root_files/bkgndH1_gun_tarpos%d_magpos%d_magvalp2.root", tarpos, magpos);  
  sprintf(Fname, "./root_files/bkgndH1_gun10M_tarpos%d_magpos%d_magvalm1.root", tarpos, magpos);  
  TFile * inFile2_p2 = new TFile(Fname);
  
  if(trig == 0){
    TH1I *H1hits_e906 = (TH1I*) inFile2_e906 -> Get("H1hits");
    TH1I *H1hits_0 = (TH1I*) inFile2_0 -> Get("H1hits");
    TH1I *H1hits_m1 = (TH1I*) inFile2_m1 -> Get("H1hits");
    TH1I *H1hits_m2 = (TH1I*) inFile2_m2 -> Get("H1hits");
    TH1I *H1hits_p1 = (TH1I*) inFile2_p1 -> Get("H1hits");
    TH1I *H1hits_p2 = (TH1I*) inFile2_p2 -> Get("H1hits");

    TH1I *H2hits_e906 = (TH1I*) inFile2_e906 -> Get("H2hits");
    TH1I *H2hits_0 = (TH1I*) inFile2_0 -> Get("H2hits");
    TH1I *H2hits_m1 = (TH1I*) inFile2_m1 -> Get("H2hits");
    TH1I *H2hits_m2 = (TH1I*) inFile2_m2 -> Get("H2hits");
    TH1I *H2hits_p1 = (TH1I*) inFile2_p1 -> Get("H2hits");
    TH1I *H2hits_p2 = (TH1I*) inFile2_p2 -> Get("H2hits");
      
    TH1I *H3hits_e906 = (TH1I*) inFile2_e906 -> Get("H3hits");
    TH1I *H3hits_0 = (TH1I*) inFile2_0 -> Get("H3hits");
    TH1I *H3hits_m1 = (TH1I*) inFile2_m1 -> Get("H3hits");
    TH1I *H3hits_m2 = (TH1I*) inFile2_m2 -> Get("H3hits");
    TH1I *H3hits_p1 = (TH1I*) inFile2_p1 -> Get("H3hits");
    TH1I *H3hits_p2 = (TH1I*) inFile2_p2 -> Get("H3hits");
  }
  else if (trig == 1){
    TH1I *H1hits_e906 = (TH1I*) inFile2_e906 -> Get("H1hits_tr");
    TH1I *H1hits_0 = (TH1I*) inFile2_0 -> Get("H1hits_tr");
    TH1I *H1hits_m1 = (TH1I*) inFile2_m1 -> Get("H1hits_tr");
    TH1I *H1hits_m2 = (TH1I*) inFile2_m2 -> Get("H1hits");
    TH1I *H1hits_p1 = (TH1I*) inFile2_p1 -> Get("H1hits");
    TH1I *H1hits_p2 = (TH1I*) inFile2_p2 -> Get("H1hits");

    TH1I *H2hits_e906 = (TH1I*) inFile2_e906 -> Get("H2hits_tr");
    TH1I *H2hits_0 = (TH1I*) inFile2_0 -> Get("H2hits_tr");
    TH1I *H2hits_m1 = (TH1I*) inFile2_m1 -> Get("H2hits_tr");
    TH1I *H2hits_m2 = (TH1I*) inFile2_m2 -> Get("H2hits");
    TH1I *H2hits_p1 = (TH1I*) inFile2_p1 -> Get("H2hits");
    TH1I *H2hits_p2 = (TH1I*) inFile2_p2 -> Get("H2hits");

    TH1I *H3hits_e906 = (TH1I*) inFile2_e906 -> Get("H3hits_tr");
    TH1I *H3hits_0 = (TH1I*) inFile2_0 -> Get("H3hits_tr");
    TH1I *H3hits_m1 = (TH1I*) inFile2_m1 -> Get("H3hits_tr");
    TH1I *H3hits_m2 = (TH1I*) inFile2_m2 -> Get("H3hits");
    TH1I *H3hits_p1 = (TH1I*) inFile2_p1 -> Get("H3hits");
    TH1I *H3hits_p2 = (TH1I*) inFile2_p2 -> Get("H3hits");
  }

  H1hits_e906 -> SetMarkerStyle(34);
  H1hits_0 -> SetMarkerStyle(21);
  H1hits_m1 -> SetMarkerStyle(25);
  H1hits_p1 -> SetMarkerStyle(25);
  H1hits_m2 -> SetMarkerStyle(21);
  H1hits_p2 -> SetMarkerStyle(21);

  H1hits_e906 -> SetMarkerColor(1);
  H1hits_0 -> SetMarkerColor(1);
  H1hits_m1 -> SetMarkerColor(2);
  H1hits_m2 -> SetMarkerColor(2);
  H1hits_p1 -> SetMarkerColor(4);
  H1hits_p2 -> SetMarkerColor(4);

  H1hits_e906 -> SetFillColor(0);
  H1hits_0 -> SetFillColor(0);
  H1hits_m1 -> SetFillColor(0);
  H1hits_p1 -> SetFillColor(0);

  H2hits_e906 -> SetMarkerStyle(34);
  H2hits_0 -> SetMarkerStyle(21);
  H2hits_m1 -> SetMarkerStyle(25);

  H2hits_e906 -> SetMarkerColor(1);
  H2hits_0 -> SetMarkerColor(1);
  H2hits_m1 -> SetMarkerColor(2);

  H2hits_e906 -> SetFillColor(0);
  H2hits_0 -> SetFillColor(0);
  H2hits_m1 -> SetFillColor(0);

  H3hits_e906 -> SetMarkerStyle(34);
  H3hits_0 -> SetMarkerStyle(21);
  H3hits_m1 -> SetMarkerStyle(25);

  H3hits_e906 -> SetMarkerColor(1);
  H3hits_0 -> SetMarkerColor(1);
  H3hits_m1 -> SetMarkerColor(2);

  H3hits_e906 -> SetFillColor(0);
  H3hits_0 -> SetFillColor(0);
  H3hits_m1 -> SetFillColor(0);
    
  TH1F *H1hits0 = new TH1F("H1hits0","Ratio of  magval0/e906",24,1,24);
  H1hits0 -> SetLineColor(1);
  TH1F *H1hits1 = new TH1F("H1hits1","Ratio of magvalm1/e906 ",24,1,24);
  H1hits1 -> SetLineColor(2);
  
  TH1F *H2hits0 = new TH1F("H2hits0","Ratio of  magval0/e906",8,1,16);
  H2hits0 -> SetLineColor(1);
  TH1F *H2hits1 = new TH1F("H2hits1","Ratio of magvalm1/e906 ",8,1,16);
  H2hits1 -> SetLineColor(2);
  
  TH1F *H3hits0 = new TH1F("H3hits0","Ratio of  magval0/e906",8,1,16);
  H3hits0 -> SetLineColor(1);
  TH1F *H3hits1 = new TH1F("H3hits1","Ratio of magvalm1/e906 ",8,1,16);
  H3hits1 -> SetLineColor(2);
    
  H1hits0 -> Divide(H1hits_0, H1hits_e906);
  H1hits1 -> Divide(H1hits_m1, H1hits_e906);

  H2hits0 -> Divide(H2hits_0, H2hits_e906);
  H2hits1 -> Divide(H2hits_m1, H2hits_e906);

  H3hits0 -> Divide(H3hits_0, H3hits_e906);
  H3hits1 -> Divide(H3hits_m1, H3hits_e906);

  /*
    H2hits_e906 -> Rebin(2);
    H2hits_0 -> Rebin(2);
    H2hits_m1 -> Rebin(2);
    H3hits_e906 -> Rebin(2);
    H3hits_0 -> Rebin(2);
    H3hits_m1 -> Rebin(2);
  */
  TLegend *legend = new TLegend(0.4,0.5,0.7,0.8);
  legend -> AddEntry(H1hits_e906,"e906 [-130 cm]");
  //legend -> AddEntry(H1hits_0,"kmag = 1");
  //legend -> AddEntry(H1hits_m1,"kmag = -1");
  legend -> AddEntry(H1hits_0,"0 Tesla");
  legend -> AddEntry(H1hits_m1,"-1 Tesla");
  //legend -> AddEntry(GxT_m2,"-2 Tesla");
  //legend -> AddEntry(GxT_p1,"+1 Tesla");
  //legend -> AddEntry(GxT_p2,"+2 Tesla");

  TCanvas *c1 = new TCanvas("c1","c1",600,400);

  c1 -> cd();
  H1hits_e906 -> Draw();
  H1hits_0 -> Draw();
  H1hits_m1 -> Draw("same");
  H1hits_e906 -> Draw("same");
  //H1hits_m2 -> Draw("same");
  //H1hits_p1 -> Draw("same");
  //H1hits_p2 -> Draw("same");
  legend ->Draw();

  sprintf(Hname, "./images_xTrates/H1hits_gun_target_tarpos%d_magpos%d_trig%d.gif", tarpos, magpos,trig);
  c1 -> SaveAs(Hname);

  TCanvas *c2 = new TCanvas("c2","c2",1200,600);
  c2 -> Divide(2,1);

  c2 -> cd(1);
  H1hits0 -> Draw();
  c2 -> cd(2);
  H1hits1 -> Draw();

  c2 -> cd();
  sprintf(Hname, "./images_xTrates/ratioH1hits_gun_target_tarpos%d_magpos%d_trig%d.gif", tarpos, magpos,trig);
  c2 -> SaveAs(Hname);


  c1 -> cd();
  H2hits_m1 -> Draw();
  H2hits_0 -> Draw("same");
  H2hits_e906 -> Draw("same");
  legend ->Draw();

  sprintf(Hname, "./images_xTrates/H2hits_gun_target_tarpos%d_magpos%d_trig%d.gif", tarpos, magpos,trig);
  c1 -> SaveAs(Hname);

  c2 -> cd(1);
  H2hits0 -> Draw();
  c2 -> cd(2);
  H2hits1 -> Draw();

  c2 -> cd();
  sprintf(Hname, "./images_xTrates/ratioH2hits_gun_target_tarpos%d_magpos%d_trig%d.gif", tarpos, magpos,trig);
  c2 -> SaveAs(Hname);

  c1 -> cd();
  H3hits_m1 -> Draw();
  H3hits_0 -> Draw("same");
  H3hits_e906 -> Draw("same");
  legend ->Draw();

  sprintf(Hname, "./images_xTrates/H3hits_gun_target_tarpos%d_magpos%d_trig%d.gif", tarpos, magpos,trig);
  c1 -> SaveAs(Hname);

  c2 -> cd(1);
  H3hits0 -> Draw();
  c2 -> cd(2);
  H3hits1 -> Draw();

  c2 -> cd();
  sprintf(Hname, "./images_xTrates/ratioH3hits_gun_target_tarpos%d_magpos%d_trig%d.gif", tarpos, magpos,trig);
  c2 -> SaveAs(Hname);

}
