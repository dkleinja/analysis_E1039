#include <roadeff_calc.h>

void roadeff_calc_sim(const int momcut = 30)
{
  //initialize
  gStyle->SetOptStat(0);
  
  //TFile* dataFile = new TFile("hodoEff_012125.root", "READ");
  //TTree* dataTree = (TTree*)dataFile->Get("save");
  char Fname[128];
  TChain *dataTree = new TChain("save");
  int chainfirst = 12525;
  //int chainlast = 12526;
  int chainlast = 15789;

  //for(int i = chainfirst; i <= chainlast; i++){
  for(int i = 0; i <= 10; i++){
    sprintf(Fname, "./roadDSTs/roadeff_mc_track1_centcut95_%d.root", i);
    //cout << "Getting File " << Fname << endl;
    dataTree -> Add(Fname);
  }

  dataTree->SetBranchAddress("charge", &charge);
  dataTree->SetBranchAddress("detHalf", &detHalf);
  dataTree->SetBranchAddress("roadflag", &roadflag);
  dataTree->SetBranchAddress("n", &n);
  dataTree->SetBranchAddress("roadElements", &roadElements);
  dataTree->SetBranchAddress("mom_exp", &mom_exp);
  
  //read in the roadIDs
  ReadRoadID();
  
  cout << "The number of entries is " << dataTree->GetEntries() << endl;
  for(int i = 0; i < dataTree->GetEntries(); ++i)
    {
      dataTree->GetEntry(i);
      //if(mom_exp < momcut) cout << i << " " << mom_exp << endl;
      if(mom_exp < momcut)continue;

      //cout << i << " " << roadElements[0] << " " << roadElements[1] << " " << roadElements[2] << " " << roadElements[3] << endl;
      
      GetRoadID(charge, detHalf, roadflag, roadElements);
      
    }

  //let's Draw the results
  DrawRoadID();
  
  //let's print the results
  WriteRoadID();

}

void GetRoadID(int chg, int detH, int flag, int hodoElement[])
{
  //cout << " " << chg << " " << detH << endl;

  if(chg == -1 && detH == 0){
      for(int line = 0; line < nroadID_chg0_det0; line++){
	if(hodoElement[0] == h1_chg0_det0[line] && hodoElement[1] == h2_chg0_det0[line] && hodoElement[2] == h3_chg0_det0[line] && hodoElement[3] == h4_chg0_det0[line]){
	  //cout << "We have a hit! Negative, Bottom!  ";
	  //cout << roadID_chg0_det0[line] << " " <<  h1_chg0_det0[line]  << " " <<  h2_chg0_det0[line]  << " " <<  h3_chg0_det0[line]  << " " <<  h4_chg0_det0[line] << endl;
	  //cout << flag << " " << hodoElement[0] << " " << hodoElement[1] << " " << hodoElement[2] << " " << hodoElement[3] << endl;
	  total_chg0_det0[line]++;
	  if(flag == 1) accept_chg0_det0[line]++;
	}
      }
  }

  if(chg == -1 && detH == 1){
      for(int line = 0; line < nroadID_chg0_det1; line++){
	if(hodoElement[0] == h1_chg0_det1[line] && hodoElement[1] == h2_chg0_det1[line] && hodoElement[2] == h3_chg0_det1[line] && hodoElement[3] == h4_chg0_det1[line]){
	  //cout << "We have a hit! Negative, Top! ";
	  //cout << roadID_chg0_det1[line] << " " <<  h1_chg0_det1[line]  << " " <<  h2_chg0_det1[line]  << " " <<  h3_chg0_det1[line]  << " " <<  h4_chg0_det1[line] << endl;
	  //cout << flag << " " << hodoElement[0] << " " << hodoElement[1] << " " << hodoElement[2] << " " << hodoElement[3] << endl;
	  total_chg0_det1[line]++;
	  if(flag == 1) accept_chg0_det1[line]++;
	}
      }
  }

    if(chg == 1 && detH == 0){
      for(int line = 0; line < nroadID_chg1_det0; line++){
	if(hodoElement[0] == h1_chg1_det0[line] && hodoElement[1] == h2_chg1_det0[line] && hodoElement[2] == h3_chg1_det0[line] && hodoElement[3] == h4_chg1_det0[line]){
	  //cout << "We have a hit!  Positive, Bottom!";
	  //cout << roadID_chg1_det0[line] << " " <<  h1_chg1_det0[line]  << " " <<  h2_chg1_det0[line]  << " " <<  h3_chg1_det0[line]  << " " <<  h4_chg1_det0[line] << endl;
	  //cout << flag << " " << hodoElement[0] << " " << hodoElement[1] << " " << hodoElement[2] << " " << hodoElement[3] << endl;
	  total_chg1_det0[line]++;
	  if(flag == 1) accept_chg1_det0[line]++;
	}
      }
  }

  if(chg == 1 && detH == 1){
      for(int line = 0; line < nroadID_chg1_det1; line++){
	if(hodoElement[0] == h1_chg1_det1[line] && hodoElement[1] == h2_chg1_det1[line] && hodoElement[2] == h3_chg1_det1[line] && hodoElement[3] == h4_chg1_det1[line]){
	  //cout << "We have a hit! Positive, Top! ";
	  //cout << roadID_chg1_det1[line] << " " <<  h1_chg1_det1[line]  << " " <<  h2_chg1_det1[line]  << " " <<  h3_chg1_det1[line]  << " " <<  h4_chg1_det1[line] << endl;
	  //cout << flag << " " << hodoElement[0] << " " << hodoElement[1] << " " << hodoElement[2] << " " << hodoElement[3] << endl;
	  total_chg1_det1[line]++;
	  if(flag == 1) accept_chg1_det1[line]++;
	}
      }
  }
}

void ReadRoadID()
{

  sprintf(inName, "./roads/67/roads_minus_bottom.txt");
  inFile_chg0_det0.open(inName);
  for(int line = 0; line < nroadID_chg0_det0; line++){
    inFile_chg0_det0 >> roadID_chg0_det0[line] >> h1_chg0_det0[line]  >> h2_chg0_det0[line]  >> h3_chg0_det0[line]  >> h4_chg0_det0[line];
    printf("%5i : %2i : %2i : %2i : %2i : %2i \n",line, roadID_chg0_det0[line], h1_chg0_det0[line], h2_chg0_det0[line], h3_chg0_det0[line], h4_chg0_det0[line]);
  }
  inFile_chg0_det0.close();

  sprintf(inName, "./roads/67/roads_minus_top.txt");
  inFile_chg0_det1.open(inName);
  for(int line = 0; line < nroadID_chg0_det1; line++){
    inFile_chg0_det1 >> roadID_chg0_det1[line] >> h1_chg0_det1[line]  >> h2_chg0_det1[line]  >> h3_chg0_det1[line]  >> h4_chg0_det1[line];
    printf("%5i : %2i : %2i : %2i : %2i : %2i \n",line, roadID_chg0_det1[line], h1_chg0_det1[line], h2_chg0_det1[line], h3_chg0_det1[line], h4_chg0_det1[line]);
  }
  inFile_chg0_det1.close();

  sprintf(inName, "./roads/67/roads_plus_bottom.txt");
  inFile_chg1_det0.open(inName);
  for(int line = 0; line < nroadID_chg1_det0; line++){
    inFile_chg1_det0 >> roadID_chg1_det0[line] >> h1_chg1_det0[line]  >> h2_chg1_det0[line]  >> h3_chg1_det0[line]  >> h4_chg1_det0[line];
    printf("%5i : %2i : %2i : %2i : %2i : %2i \n",line, roadID_chg1_det0[line], h1_chg1_det0[line], h2_chg1_det0[line], h3_chg1_det0[line], h4_chg1_det0[line]);
  }
  inFile_chg1_det0.close();

  sprintf(inName, "./roads/67/roads_plus_top.txt");
  inFile_chg1_det1.open(inName);
  for(int line = 0; line < nroadID_chg1_det1; line++){
    inFile_chg1_det1 >> roadID_chg1_det1[line] >> h1_chg1_det1[line]  >> h2_chg1_det1[line]  >> h3_chg1_det1[line]  >> h4_chg1_det1[line];
    printf("%5i : %2i : %2i : %2i : %2i : %2i \n",line, roadID_chg1_det1[line], h1_chg1_det1[line], h2_chg1_det1[line], h3_chg1_det1[line], h4_chg1_det1[line]);
  }
  inFile_chg1_det1.close();

}

void WriteRoadID()
{
  
  sprintf(outName, "./roads/67/simeff_minus_bottom.txt");
  outFile_chg0_det0.open(outName);
  for(int line = 0; line < nroadID_chg0_det0; line++){
    eff_chg0_det0[line] = -999.;
    if(total_chg0_det0[line] > 0) eff_chg0_det0[line] = accept_chg0_det0[line] / (double)total_chg0_det0[line];
    outFile_chg0_det0 << roadID_chg0_det0[line] << "\t" << h1_chg0_det0[line]  << "\t" << h2_chg0_det0[line]  << "\t" << h3_chg0_det0[line]  << "\t" << h4_chg0_det0[line] << "\t" << accept_chg0_det0[line] << "\t" << total_chg0_det0[line] << "\t" << eff_chg0_det0[line] << "\n";
    printf("%5i : %2i : %2i : %2i : %2i : %2i : %2i : %2i : %2i \n",line, roadID_chg0_det0[line], h1_chg0_det0[line], h2_chg0_det0[line], h3_chg0_det0[line], h4_chg0_det0[line], accept_chg0_det0[line], total_chg0_det0[line], eff_chg0_det0[line]);
  }
  outFile_chg0_det0.close();

  sprintf(outName, "./roads/67/simeff_minus_top.txt");
  outFile_chg0_det1.open(outName);
  for(int line = 0; line < nroadID_chg0_det1; line++){
    eff_chg0_det1[line] = -999.;
    if(total_chg0_det1[line] > 0) eff_chg0_det1[line] = accept_chg0_det1[line] / (double)total_chg0_det1[line];
    outFile_chg0_det1 << roadID_chg0_det1[line] << "\t" << h1_chg0_det1[line]  << "\t" << h2_chg0_det1[line]  << "\t" << h3_chg0_det1[line]  << "\t" << h4_chg0_det1[line] << "\t" << accept_chg0_det1[line] << "\t" << total_chg0_det1[line] << "\t" << eff_chg0_det1[line] << "\n";
    printf("%5i : %2i : %2i : %2i : %2i : %2i : %2i : %2i : %2i \n",line, roadID_chg0_det1[line], h1_chg0_det1[line], h2_chg0_det1[line], h3_chg0_det1[line], h4_chg0_det1[line], accept_chg0_det1[line], total_chg0_det1[line], eff_chg0_det1[line]);
  }
  outFile_chg0_det1.close();

  sprintf(outName, "./roads/67/simeff_plus_bottom.txt");
  outFile_chg1_det0.open(outName);
  for(int line = 0; line < nroadID_chg1_det0; line++){
    eff_chg1_det0[line] = -999.;
    if(total_chg1_det0[line] > 0) eff_chg1_det0[line] = accept_chg1_det0[line] / (double)total_chg1_det0[line];
    outFile_chg1_det0 << roadID_chg1_det0[line] << "\t" << h1_chg1_det0[line]  << "\t" << h2_chg1_det0[line]  << "\t" << h3_chg1_det0[line]  << "\t" << h4_chg1_det0[line] << "\t" << accept_chg1_det0[line] << "\t" << total_chg1_det0[line] << "\t" << eff_chg1_det0[line] << "\n";
    printf("%5i : %2i : %2i : %2i : %2i : %2i : %2i : %2i : %2i \n",line, roadID_chg1_det0[line], h1_chg1_det0[line], h2_chg1_det0[line], h3_chg1_det0[line], h4_chg1_det0[line], accept_chg1_det0[line], total_chg1_det0[line], eff_chg1_det0[line]);
  }
  outFile_chg1_det0.close();

  sprintf(outName, "./roads/67/simeff_plus_top.txt");
  outFile_chg1_det1.open(outName);
  for(int line = 0; line < nroadID_chg1_det1; line++){
    eff_chg1_det1[line] = -999.;
    if(total_chg1_det1[line] > 0) eff_chg1_det1[line] = accept_chg1_det1[line] / (double)total_chg1_det1[line];
    //if(total_chg1_det1[line] > 0) cout << line << " "  << accept_chg1_det1[line] << " " << total_chg1_det1[line] << " " << accept_chg1_det1[line] / (double)total_chg1_det1[line] << endl;
    outFile_chg1_det1 << roadID_chg1_det1[line] << "\t" << h1_chg1_det1[line]  << "\t" << h2_chg1_det1[line]  << "\t" << h3_chg1_det1[line]  << "\t" << h4_chg1_det1[line] << "\t" << accept_chg1_det1[line] << "\t" << total_chg1_det1[line] << "\t" << eff_chg1_det1[line] << "\n";
    printf("%4i : %4i : %4i : %4i : %4i : %4i : %4i : %4i : %2.3f \n",line, roadID_chg1_det1[line], h1_chg1_det1[line], h2_chg1_det1[line], h3_chg1_det1[line], h4_chg1_det1[line], accept_chg1_det1[line], total_chg1_det1[line], eff_chg1_det1[line]);
  }
  outFile_chg1_det1.close();
}

void DrawRoadID()
{
  hist_all_chg0_det0 = new TH1I("all_minus_bottom", "all_minus_bottom", nroadID_chg0_det0, 0, nroadID_chg0_det0);
  hist_acc_chg0_det0 = new TH1I("acc_minus_bottom", "acc_minus_bottom", nroadID_chg0_det0, 0, nroadID_chg0_det0);
  hist_all_chg0_det1 = new TH1I("all_minus_top", "all_minus_top", nroadID_chg0_det1, 0, nroadID_chg0_det1);
  hist_acc_chg0_det1 = new TH1I("acc_minus_top", "acc_minus_top", nroadID_chg0_det1, 0, nroadID_chg0_det1);
  hist_all_chg1_det0 = new TH1I("all_plus_bottom", "all_plus_bottom", nroadID_chg1_det0, 0, nroadID_chg1_det0);
  hist_acc_chg1_det0 = new TH1I("acc_plus_bottom", "acc_plus_bottom", nroadID_chg1_det0, 0, nroadID_chg1_det0);
  hist_all_chg1_det1 = new TH1I("all_plus_top", "all_plus_top", nroadID_chg1_det1, 0, nroadID_chg1_det1);
  hist_acc_chg1_det1 = new TH1I("acc_plus_top", "acc_plus_top", nroadID_chg1_det1, 0, nroadID_chg1_det1);

  for(int i = 0; i < nroadID_chg0_det0; i++){
    hist_all_chg0_det0 -> SetBinContent(i+1, total_chg0_det0[i]);
    hist_acc_chg0_det0 -> SetBinContent(i+1, accept_chg0_det0[i]);
  }
  for(int i = 0; i < nroadID_chg0_det1; i++){
    hist_all_chg0_det1 -> SetBinContent(i+1, total_chg0_det1[i]);
    hist_acc_chg0_det1 -> SetBinContent(i+1, accept_chg0_det1[i]);
  }
  for(int i = 0; i < nroadID_chg1_det0; i++){
    hist_all_chg1_det0 -> SetBinContent(i+1, total_chg1_det0[i]);
    hist_acc_chg1_det0 -> SetBinContent(i+1, accept_chg1_det0[i]);
  }
  for(int i = 0; i < nroadID_chg1_det1; i++){
    hist_all_chg1_det1 -> SetBinContent(i+1, total_chg1_det1[i]);
    hist_acc_chg1_det1 -> SetBinContent(i+1, accept_chg1_det1[i]);
  }

 //set up graphs
  graph_eff_chg0_det0 = new TGraphAsymmErrors();
  graph_eff_chg0_det0 -> SetName("eff_minus_bottom");
  graph_eff_chg0_det0 -> SetTitle("eff_minus_bottom");
  graph_eff_chg0_det0->GetXaxis()->SetTitle("roadID");
  graph_eff_chg0_det0->GetXaxis()->CenterTitle();
  graph_eff_chg0_det0->SetMarkerStyle(8);
  graph_eff_chg0_det0->SetMarkerSize(0.4);
  graph_eff_chg0_det1 = new TGraphAsymmErrors();
  graph_eff_chg0_det1 -> SetName("eff_minus_top");
  graph_eff_chg0_det1 -> SetTitle("eff_minus_top");
  graph_eff_chg0_det1->GetXaxis()->SetTitle("roadID");
  graph_eff_chg0_det1->GetXaxis()->CenterTitle();
  graph_eff_chg0_det1->SetMarkerStyle(8);
  graph_eff_chg0_det1->SetMarkerSize(0.4);
  graph_eff_chg1_det0 = new TGraphAsymmErrors();
  graph_eff_chg1_det0 -> SetName("eff_plus_bottom");
  graph_eff_chg1_det0 -> SetTitle("eff_plus_bottom");
  graph_eff_chg1_det0->GetXaxis()->SetTitle("roadID");
  graph_eff_chg1_det0->GetXaxis()->CenterTitle();
  graph_eff_chg1_det0->SetMarkerStyle(8);
  graph_eff_chg1_det0->SetMarkerSize(0.4);
  graph_eff_chg1_det1 = new TGraphAsymmErrors();
  graph_eff_chg1_det1 -> SetName("eff_plus_top");
  graph_eff_chg1_det1 -> SetTitle("eff_plus_top");
  graph_eff_chg1_det1->GetXaxis()->SetTitle("roadID");
  graph_eff_chg1_det1->GetXaxis()->CenterTitle();
  graph_eff_chg1_det1->SetMarkerStyle(8);
  graph_eff_chg1_det1->SetMarkerSize(0.4);

  graph_eff_chg0_det0->Divide(hist_acc_chg0_det0, hist_all_chg0_det0, "cl=0.683 b(1,1) mode");
  graph_eff_chg0_det0->GetXaxis()->SetTitle("roadID");
  graph_eff_chg0_det0->GetXaxis()->CenterTitle();
  graph_eff_chg0_det1->Divide(hist_acc_chg0_det1, hist_all_chg0_det1, "cl=0.683 b(1,1) mode");
  graph_eff_chg0_det1->GetXaxis()->SetTitle("roadID");
  graph_eff_chg0_det1->GetXaxis()->CenterTitle();
  graph_eff_chg1_det0->Divide(hist_acc_chg1_det0, hist_all_chg1_det0, "cl=0.683 b(1,1) mode");
  graph_eff_chg1_det0->GetXaxis()->SetTitle("roadID");
  graph_eff_chg1_det0->GetXaxis()->CenterTitle();
  graph_eff_chg1_det1->Divide(hist_acc_chg1_det1, hist_all_chg1_det1, "cl=0.683 b(1,1) mode");
  graph_eff_chg1_det1->GetXaxis()->SetTitle("roadID");
  graph_eff_chg1_det1->GetXaxis()->CenterTitle();
  
  TCanvas* c1 = new TCanvas("c1","c1", 1000,1000);
  c1->Divide(2, 2);  
  for(int i = 1; i <= 4; ++i){
      c1->cd(i)->SetGridx();
      c1->cd(i)->SetGridy();
    }

  c1 -> cd(1);
  graph_eff_chg0_det0->GetYaxis()->SetRangeUser(0.9, 1.02);
  graph_eff_chg0_det0->GetXaxis()->SetLimits(0, nroadID_chg0_det0);
  graph_eff_chg0_det0->Draw("ap");
  c1 -> cd(2);
  graph_eff_chg0_det1->GetYaxis()->SetRangeUser(0.9, 1.02);
  graph_eff_chg0_det1->GetXaxis()->SetLimits(1, nroadID_chg0_det1);
  graph_eff_chg0_det1->Draw("ap");
  c1 -> cd(3);
  graph_eff_chg1_det0->GetYaxis()->SetRangeUser(0.9, 1.02);
  graph_eff_chg1_det0->GetXaxis()->SetLimits(1, nroadID_chg1_det0);
  graph_eff_chg1_det0->Draw("ap");
  c1 -> cd(4);
  graph_eff_chg1_det1->GetYaxis()->SetRangeUser(0.9, 1.02);
  graph_eff_chg1_det1->GetXaxis()->SetLimits(1, nroadID_chg1_det1);
  graph_eff_chg1_det1->Draw("ap");

  c1 -> cd();
  //sprintf(Fname, "./images/roadset_67_momcut%d_R006.gif", momcut);
  //c1 -> SaveAs(Fname);
}
