void drawmagflip_realdpsim(const int dmrebin = 4, const int roadset0 = 67, const int tarvar = 1)
{

  gStyle -> SetOptFit(1);
  const Float_t PI = 3.14159;

  char Fname[128];
  char Hname[128];
  char Tname[128];
  char tardump[5];

  if(tarvar == 0){
    sprintf(tardump, "dump");
  }
  else if(tarvar == 1){
    sprintf(tardump, "LD2");
  }
  else if(tarvar == 2){
    sprintf(tardump, "C");
  }

  //sprintf(Fname, "./root_files/phidists_sims_%s_magflip0.root", tardump);
  sprintf(Fname, "./root_files/phidists_target%d_roadset%d.root", tarvar, roadset0);
  TFile *inFile0 = new TFile(Fname);
  sprintf(Fname, "./root_files/phidists_dpsim_hodoeff1_trigon0.root");
  TFile *inFile1 = new TFile(Fname);

  TH1D *Hdmphi0 = (TH1D*) inFile0 -> Get("Hdmphi");
  TH1D *Hdmphi1 = (TH1D*) inFile1 -> Get("Hdmphi");
  sprintf(Tname, "phi of dimuon magflip0/[magflip1 * rellum] in %s", tardump);
  TH1D *Hdmphirat = new TH1D("Hdmphirat", Tname, 64, -3.15, 3.15);

  Hdmphi0 -> SetXTitle("#phi");
  Hdmphi1 -> SetXTitle("#phi");
  Hdmphirat -> SetXTitle("#phi");
  Hdmphi0 -> SetYTitle("Counts");
  Hdmphi1 -> SetYTitle("Counts");
  Hdmphirat -> SetYTitle("Ratio");

  //initialize fit functions  
  //TF1 *cos1x = new TF1("fa1","[0]*cos(x)",-3.14, 3.14);
  TF1 *cos1x = new TF1("fa1","[0] + [1]*cos(x)",-3.14, 3.14); 
  cos1x -> SetParameters(1,1);

  TF1 *cos2x = new TF1("fa1","[0] + [1]*cos([2]*x+[3])",-3.14, 3.14); 
  cos2x -> SetParameters(1,1,2,0);

  

  //rebin if necessary
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
  Hdmphirat -> Divide(Hdmphi0, Hdmphi1, 1, 1);

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
  //Hdmphirat -> Fit("pol0");
  //Hdmphirat -> Fit(cos2x);
  l0 -> Draw();
  sprintf(Hname, "./images/dimuon_roadset%d_sim_%s_magflip0_over_magflip1.gif", roadset0, tardump);
  c3 -> SaveAs(Hname);
  
  
}
