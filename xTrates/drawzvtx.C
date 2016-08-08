void drawzvtx()
{

  gStyle -> SetOptStat(0);

  char Fname[64];
  char Hname[64];

  sprintf(Fname, "root_files/zvtxsmear_dimuon_dump.root");
  TFile *inFile_dump = new TFile(Fname);
  
  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos130_magpos0_magval0.root");
  TFile *inFile_e906 = new TFile(Fname);

  sprintf(Fname, "./root_files/exclusion_dimuon_target_tarpos350_magpos150_magval0.root");
  TFile * inFile_0 = new TFile(Fname);

  TH1F *hzvtx_dump = (TH1F*) inFile_dump -> Get("Zvtx");
  TH1F *hzvtx_e906 = (TH1F*) inFile_e906 -> Get("Zvtx");
  TH1F *hzvtx_0 = (TH1F*) inFile_0 -> Get("Zvtx");

  hzvtx_dump -> SetLineColor(8);
  hzvtx_e906 -> SetLineColor(1);
  hzvtx_0 -> SetLineColor(2);

  //hzvtx_dump -> SetLogY();

  TLegend *legend = new TLegend(0.8,0.8,0.95,0.95);
  legend -> AddEntry(hzvtx_dump,"Dump");
  legend -> AddEntry(hzvtx_e906,"E906 [-130 cm]");
  legend -> AddEntry(hzvtx_0,"E1039 [-350 cm]");

  hzvtx_dump -> Draw();
  legend -> Draw();
  hzvtx_e906 -> Draw("same");
  hzvtx_0 -> Draw("same");



}
