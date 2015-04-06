void drawxTrates(const int tarpos, const int magpos)
{

  char Fname[64];
  char Hname[64];

  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos130_magpos0_magval0.root");
  TFile *inFile_e906 = new TFile(Fname);
  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos%d_magpos150_magval0.root", tarpos, magpos);  
  TFile * inFile_0 = new TFile(Fname);
  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos%d_magpos%d_magvalm1.root", tarpos, magpos);  
  TFile * inFile_m1 = new TFile(Fname);
  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos%d_magpos%d_magvalm2.root", tarpos, magpos);  
  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos130_magpos0_magval0.root");
  TFile * inFile_m2 = new TFile(Fname);
  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos%d_magpos%d_magvalp1.root", tarpos, magpos);  
  TFile * inFile_p1 = new TFile(Fname);
  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos%d_magpos%d_magvalp2.root", tarpos, magpos);  
  sprintf(Fname, "./root_files/hitrate_dimuon_target_tarpos130_magpos0_magval0.root");
  TFile * inFile_p2 = new TFile(Fname);

  TGraphErrors *GxT_e906 = (TGraphErrors*) inFile_e906 -> Get("GxT");
  TGraphErrors *GxT_0 = (TGraphErrors*) inFile_0 -> Get("GxT");
  TGraphErrors *GxT_m1 = (TGraphErrors*) inFile_m1 -> Get("GxT");
  TGraphErrors *GxT_m2 = (TGraphErrors*) inFile_m2 -> Get("GxT");
  TGraphErrors *GxT_p1 = (TGraphErrors*) inFile_p1 -> Get("GxT");
  TGraphErrors *GxT_p2 = (TGraphErrors*) inFile_p2 -> Get("GxT");

  GxT_e906 -> SetMarkerStyle(34);
  GxT_e906 -> SetMarkerColor(1);

  sprintf(Hname, "tarpos%d_magpos%d", tarpos, magpos);
  GxT_e906 -> SetTitle(Hname);
  GxT_e906 -> SetMaximum(1.11);
  GxT_e906 -> SetMinimum(-0.01);


  GxT_m1 -> SetMarkerStyle(21);
  GxT_p1 -> SetMarkerStyle(21);
 
  GxT_m1 -> SetMarkerColor(2);
  GxT_m2 -> SetMarkerColor(2);
  GxT_p1 -> SetMarkerColor(4);
  GxT_p2 -> SetMarkerColor(4);

  GxT_e906 -> SetFillColor(0);
  GxT_0 -> SetFillColor(0);
  GxT_m1 -> SetFillColor(0);
  GxT_m2 -> SetFillColor(0);
  GxT_p1 -> SetFillColor(0);
  GxT_p2 -> SetFillColor(0);
  GxT_e906 -> SetLineColor(0);
  GxT_0 -> SetLineColor(0);
  GxT_m1 -> SetLineColor(0);
  GxT_m2 -> SetLineColor(0);
  GxT_p1 -> SetLineColor(0);
  GxT_p2 -> SetLineColor(0);

  //TCanvas *c1 = new TCanvas("c1","c1",600,400);
  //TCanvas *c2 = new TCanvas("c2","c2",600,400);
  //c2 -> Divide(2,1);

  TAxis *axis = GxT_e906 -> GetXaxis();
  axis -> SetLimits(-0.01,0.71);

  //c1 -> cd(1);
GxT_e906 -> SetTitle("Accepted Drell-Yan #mu^{+}#mu^{-} pairs rates");
  GxT_e906 -> Draw("ap");
  GxT_0 -> Draw("p");
  GxT_m1 -> Draw("p");
 // GxT_m2 -> Draw("p");
  GxT_p1 -> Draw("p");
  //GxT_p2 -> Draw("p");

  TLegend *legend = new TLegend(0.5,0.5,0.8,0.8);
  legend -> AddEntry(GxT_e906,"e906 [-130 cm]");
  legend -> AddEntry(GxT_0,"0 Tesla");
  legend -> AddEntry(GxT_m1,"-1 Tesla");
  //legend -> AddEntry(GxT_m2,"-2 Tesla");
  legend -> AddEntry(GxT_p1,"+1 Tesla");
  //legend -> AddEntry(GxT_p2,"+2 Tesla");
  legend -> Draw();

  sprintf(Hname, "./images_xTrates/dimuon_target_tarpos%d_magpos%d.gif", tarpos, magpos);
  c1 -> SaveAs(Hname);


}
