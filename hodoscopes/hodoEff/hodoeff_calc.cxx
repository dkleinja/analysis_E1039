void hodoeff_calc(const int reco = 5, const int roadset = 57, const int ntracks = 1, const int momcut = 20, const int ycut = 3, const int centcut = 90)
{
  gStyle->SetOptStat(0);
  int hodoID;
  int elementID;
  int flag;
  int matrix1flag;
  double mom_exp;
  double y_exp;

  //get the values
  Int_t paddleNumber;
  Double_t x;
  Double_t Efficiency, Efficiency_low, Efficiency_high;
  Double_t EffError_low, EffError_high;
  
  /* 
  int nElements[8] = {20, 20, 19, 19, 16, 16, 16, 16};
  int hodoIDs[8] = {27, 28, 29, 30, 35, 36, 37, 38}; 
  std::string hodoNames[8] = {"H1L", "H1R", "H2L", "H2R", "H4Y1L", "H4Y1R", "H4Y2L", "H4Y2R"};
  TFile* dataFile = new TFile("hodoeff_Y.root", "READ");
  */
   
  int nElements[8] = {23, 23, 16, 16, 16, 16, 16, 16};
  int hodoIDs[8] = {25, 26, 31, 32, 33, 34, 39, 40}; 
  std::string hodoNames[8] = {"H1B", "H1T", "H2B", "H2T", "H3B", "H3T", "H4B", "H4T"};
  //TFile* dataFile = new TFile("hodoEff_012525.root", "READ");

  //TFile* dataFile = new TFile("hodoEff_012125.root", "READ");
  //TTree* dataTree = (TTree*)dataFile->Get("save");
  char Fname[128];
  TChain *dataTree = new TChain("save");
  int chainfirst = 12525;
  //int chainlast = 12526;
  int chainlast = 15789;
  if(roadset==57){
    chainfirst = 8412;
    chainlast = 10415;
  }
  //for(int i = chainfirst; i <= chainlast; i++){
  for(int i = chainfirst; i <= chainlast; i++){
    //if(i > 13799 && i < 14800)continue;
    sprintf(Fname, "./hodoDSTs/hodoEff_0%d.root", i);
    //sprintf(Fname, "./hodoDSTs/geotest_0%d.root", i);
    sprintf(Fname, "./hodoDSTs/kunadvise_0%d.root", i);
    sprintf(Fname, "./hodoDSTs/momcut%d_ycut3_centcut%d_trighits1_0%d.root", momcut, centcut, i);
    //sprintf(Fname, "./hodoDSTs/trighits_momcut%d_ycut3_centcut%d_0%d.root", momcut, centcut, i);
    //sprintf(Fname, "./hodoDSTs/momcut%d_ycut3_centcut%d_0%d.root", momcut, centcut, i);
    sprintf(Fname, "./hodoDSTs/R00%dfiles_track%d_0%d.root", reco, ntracks, i);
    //sprintf(Fname, "./hodoDSTs/R00%dfiles_OnlEnable%d_0%d.root", reco, ntracks, i);
    //cout << "Getting File " << Fname << endl;
    dataTree -> Add(Fname);
  }


  dataTree->SetBranchAddress("hodoID", &hodoID);
  dataTree->SetBranchAddress("elementID", &elementID);
  dataTree->SetBranchAddress("flag", &flag);
  dataTree->SetBranchAddress("matrix1flag", &matrix1flag);
  dataTree->SetBranchAddress("mom_exp", &mom_exp);
  dataTree->SetBranchAddress("y_exp", &y_exp);

  TH1I* hist_all[8];
  TH1I* hist_acc[8];
  TH1D* hist_eff[8];
  TGraphAsymmErrors* graph_eff[8];
  
  char buffer[20];
  for(int i = 0; i < 8; ++i)
    {
      sprintf(buffer, "%s_all", hodoNames[i].c_str());
      hist_all[i] = new TH1I(buffer, buffer, nElements[i], 1, nElements[i]+1);
      hist_all[i]->Sumw2();

      sprintf(buffer, "%s_acc", hodoNames[i].c_str());
      hist_acc[i] = new TH1I(buffer, buffer, nElements[i], 1, nElements[i]+1);
      hist_acc[i]->Sumw2();

      sprintf(buffer, "hist_%s_eff", hodoNames[i].c_str());
      hist_eff[i] = new TH1D(buffer, buffer, nElements[i], 1, nElements[i]+1);
      hist_eff[i]->Sumw2();
  
      hist_eff[i]->GetXaxis()->SetTitle("elementID");
      hist_eff[i]->GetXaxis()->CenterTitle();
      hist_eff[i]->SetMarkerStyle(8);
      hist_eff[i]->SetMarkerSize(0.4);

      sprintf(buffer, "%s_eff", hodoNames[i].c_str());
      graph_eff[i] = new TGraphAsymmErrors();
      graph_eff[i] -> SetName(buffer);
      graph_eff[i] -> SetTitle(buffer);
      graph_eff[i]->GetXaxis()->SetTitle("elementID");
      graph_eff[i]->GetXaxis()->CenterTitle();
      graph_eff[i]->SetMarkerStyle(8);
      graph_eff[i]->SetMarkerSize(0.4);
    }

  cout << "The number of entries is " << dataTree->GetEntries() << endl;
  for(int i = 0; i < dataTree->GetEntries(); ++i)
    {
      dataTree->GetEntry(i);
      //if(mom_exp < momcut) cout << i << " " << mom_exp << endl;
      if(mom_exp < momcut)continue;
      if(fabs(y_exp) < ycut)continue;
      //if(matrix1flag!=1)continue;
      int idx = -1;
      for(int j = 0; j < 8; ++j)
	{
	  if(hodoIDs[j] == hodoID)
	    {
	      idx = j;
	      break;
	    }
	}
      
      if(idx >= 0 && idx < 8)
	{
	  hist_all[idx]->Fill(elementID);
	  if(flag == 1) hist_acc[idx]->Fill(elementID);
	}
    }
  //let's create ofstream file
  ofstream outFile;
  sprintf(Fname, "./eff/roadset67_R00%d_ntracks%d_momcut%d_ycut%d_centcut90.txt", reco, ntracks, momcut, ycut);
  //sprintf(Fname, "detectorEff_low.txt", reco, ntracks, momcut, ycut);
  outFile.open(Fname);
  outFile << "HodoName" << "\t" << "elementID" << "\t" << "Efficiency" << "\t" << "Error_low" << "\t" << "Error_high" << "\n";
  for(int i = 0; i < 8; ++i)
    {
      hist_eff[i]->Divide(hist_acc[i], hist_all[i], 1., 1., "B");
      //hist_eff[i]->Divide(hist_acc[i], hist_all[i], 1., 1., "cl=0.683 b(1,1) mode");
      graph_eff[i]->Divide(hist_acc[i], hist_all[i], "cl=0.683 b(1,1) mode");
      graph_eff[i]->GetXaxis()->SetTitle("elementID");
      graph_eff[i]->GetXaxis()->CenterTitle();

      //here is where we need to write out to file
      for(int j = 0; j < graph_eff[i]->GetN(); j++){
	graph_eff[i] -> GetPoint(j, x, Efficiency);
	paddleNumber = x;
	EffError_low = graph_eff[i] -> GetErrorYlow(j);
	EffError_high = graph_eff[i] -> GetErrorYhigh(j);
	Efficiency_low = Efficiency - EffError_low;
	Efficiency_high = Efficiency + EffError_high;
	outFile << hodoNames[i] << "\t" << paddleNumber << "\t" << Efficiency << "\t" << EffError_low << "\t" << EffError_high << "\n";
	//outFile << hodoNames[i] << "\t" << paddleNumber << "\t" << Efficiency << "\t" << 0 << "\n";
	//outFile << hodoNames[i] << "\t" << paddleNumber << "\t" << Efficiency_low << "\t" << 0 << "\n";
	//outFile << hodoNames[i] << "\t" << paddleNumber << "\t" << Efficiency_high << "\t" << 0 << "\n";
	printf("%s : %4i : %1.4f : %1.4f : %1.4f \n", hodoNames[i].c_str(), paddleNumber, Efficiency, EffError_low, EffError_high);
      }
    }
      outFile.close();

  TCanvas* c1 = new TCanvas("c1","c1", 2000,1000);
  c1->Divide(4, 2);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLogx(); 
  c1->SetLogy();
  
  TCanvas* c2 = new TCanvas("c2", "c2", 2000, 1000);
  c2->Divide(4, 2);
  c2->SetGridx();
  c2->SetGridy();
  c2->SetLogx(); 
  c2->SetLogy();
  
  for(int i = 1; i <= 8; ++i)
    {
      c1->cd(i)->SetGridx();
      c1->cd(i)->SetGridy();
      //c1->cd(i)->SetLogy();
      //hist_all[i-1]->Draw(); hist_acc[i-1]->Draw("same");
      //hist_eff[i-1]->GetYaxis()->SetRangeUser(0.5, 1.1);
      //hist_eff[i-1]->Draw();
      graph_eff[i-1]->GetYaxis()->SetRangeUser(0.5, 1.1);
      graph_eff[i-1]->GetXaxis()->SetLimits(1, nElements[i-1]+1);
      graph_eff[i-1]->Draw("ap");
      
      c2->cd(i)->SetGridx();
      c2->cd(i)->SetGridy();
      c2->cd(i)->SetLogy();
      hist_acc[i-1]->Draw();
    
    }

  c1 -> cd();
  sprintf(Fname, "./images/roadset%d_R00%d_ntracks%d_momcut%d_ycut%d_centcut90.gif", roadset, reco, ntracks, momcut, ycut);
  c1 -> SaveAs(Fname);
  c2 -> cd();
  sprintf(Fname, "./images/roadset%d_yields_R00%d_ntracks%d_momcut%d_ycut%d_centcut90.gif", roadset, reco, ntracks, momcut, ycut);
  c2 -> SaveAs(Fname);
}
