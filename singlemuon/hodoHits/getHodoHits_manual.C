#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <boost/array.hpp>

#include <TROOT.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TMatrixD.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TClonesArray.h>
#include <TArrow.h>
#include <TPaveText.h>
#include <TSystem.h>

#include <DPMCRawEvent.h>

using namespace std;

int main(int argc, char *argv[])
{
  //
  //gStyle -> SetOptFit(1);
  //gStyle -> SetStatX(0.3);
  //gStyle -> SetStatY(0.3);
  const Float_t PI = 3.14159;
  
  char Fname[128];
  char Hname[128];
  char Tname[128];
  
  const int nTrigs = 17;
  const int nHodos = 16;
   //trigger variables
  Int_t Trig[nTrigs];
  std::string hodoTrigs[nTrigs] = {"1H1Y", "1H2Y", "1H3Y", "1H4Y", "2H12Y", "2H13Y", "2H14Y", "2H23Y", "2H24Y", "2H34Y", "3H123Y", "3H124Y", "3H234Y", "NIM1", "NIM2", "NIM3", "MATRIX0"};
  //hodo variables;
  int nElements[nHodos] = {23, 23, 16, 16, 16, 16, 16, 16, 20, 20, 19, 19, 16, 16, 16, 16 };
  int hodoIDs[nHodos] = {25, 26, 31, 32, 33, 34, 39, 40, 27, 28, 29, 30, 35, 36, 37, 38 };
  std::string hodoNames[nHodos] = {"H1B", "H1T", "H2B", "H2T", "H3B", "H3T", "H4B", "H4T", "H1L", "H1R", "H2L", "H2R", "H4Y1L", "H4Y1R", "H4Y2L", "H4Y2R"};
  
  /* 
  int nElements[8] = {20, 20, 19, 19, 16, 16, 16, 16};
  int hodoIDs[8] = {27, 28, 29, 30, 35, 36, 37, 38}; 
  std::string hodoNames[8] = {"H1L", "H1R", "H2L", "H2R", "H4Y1L", "H4Y1R", "H4Y2L", "H4Y2R"};
  TFile* dataFile = new TFile("hodoeff_Y.root", "READ");
  */

  TH1I *hodoHits[nTrigs][nHodos];
  for(int i = 0; i < nTrigs; ++i){
    for(int k = 0; k < nHodos; ++k){
      sprintf(Hname, "nHits_%s_hodo%d",  hodoTrigs[i].c_str(), k);
      sprintf(Tname, "raw%s hits on %s", hodoTrigs[i].c_str(), hodoNames[k].c_str() );
      hodoHits[i][k] = new TH1I(Hname, Tname, nElements[k], 1, nElements[k]+1);
    }
  }
  //TFile* dataFile = new TFile("MATRIXcheck_20K_988x50M.root", "READ");
  //TFile* dataFile = new TFile("e906beamProf_20K_767x100M.root", "READ");
  //TFile* dataFile = new TFile("E906beamProf_998x100M.root", "READ");
  //TFile* dataFile = new TFile("redo_single_20K_992x50M.root", "READ");
  //TFile* dataFile = new TFile("TrigBug_993x100M.root", "READ");
  //TFile* dataFile = new TFile("TrigBug2_999x100M.root", "READ");
  //TFile* dataFile = new TFile("ftfp_bert_emx_999x100M.root", "READ");
  //TFile* dataFile = new TFile("NIMbits_20K_200x100M.root", "READ");
  //TFile* dataFile = new TFile("NIMbits2_993x100M.root", "READ");
  TFile* dataFile = new TFile("HYhits_200x100M.root", "READ");
  //TFile* dataFile = new TFile("batch0_dy_hodo0_10M_R005.root", "READ");
  TTree* dataTree = (TTree*)dataFile->Get("save");

  DPMCRawEvent* rawEvent = new DPMCRawEvent;
  dataTree->SetBranchAddress("rawEvent", &rawEvent);
  int elementID, detectorID;
  int YelementID, YdetectorID;
  int detx, dety;
  float sigWeight;
  const int Hits = 3000;
  Int_t nHits[nHodos];
  Int_t e_hodoPos[nHodos][Hits];

  int nentries = dataTree -> GetEntries();
  cout << "The number of Entries is: " << nentries << endl;
  for(Int_t ievent = 0; ievent < nentries; ++ievent){
    dataTree->GetEntry(ievent);
    if(ievent%100000 == 0) cout << ievent << " Events. " << endl;
    //get trigger conditions
    //if(trigon == 1){
    DPMCHeader evt = rawEvent->eventHeader();
    //sigWeight = evt.fSigWeight*100.;
    sigWeight = 1.; 
    
    if(ievent%100000 == 0)cout << ievent << "  Number of hits this event is " << rawEvent->getNHits() << endl;
    //set hodo hits to zero
    for(int k = 0; k < nHodos; ++k)nHits[k] = 0;//reset the hit counters every event;
    for(Int_t j = 0; j < rawEvent->getNHits(); ++j){
      DPMCHit hit = rawEvent->getHit(j);
      if(abs(hit.fPDGCode) != 13) continue;//only do muons for now
      elementID = hit.fElementID;
      detectorID = hit.fDetectorID;
      for(int k = 0; k < nHodos; ++k){
	e_hodoPos[k][j] = -1;//reset channel before assigning 
	if(detectorID == hodoIDs[k]){
	  hodoHits[16][k] -> Fill(elementID, sigWeight);
	  e_hodoPos[k][j] = elementID;
	  nHits[k]++;
	  //cout << i << " " << j << " " << k << " " << detectorID << " " << nHits[k] << endl;
	}
      }
    }
    //do trigger hits
    for(int trig = 0; trig < nTrigs; ++trig) Trig[trig] = 0;//reset trigger
    if(nHits[8]  > 0 || nHits[9]  > 0) Trig[0] = 1;//h1
    if(nHits[10] > 0 || nHits[11] > 0) Trig[1] = 1;//h2
    if(nHits[12] > 0 || nHits[13] > 0) Trig[2] = 1;//h3
    if(nHits[14] > 0 || nHits[15] > 0) Trig[3] = 1;//h4
    
    if( (nHits[8]  > 0 && nHits[10] > 0) || (nHits[9]  > 0 && nHits[11] > 0) ) Trig[4] = 1;//h12
    if( (nHits[8]  > 0 && nHits[12] > 0) || (nHits[9]  > 0 && nHits[13] > 0) ) Trig[5] = 1;//h13
    if( (nHits[8]  > 0 && nHits[14] > 0) || (nHits[9]  > 0 && nHits[15] > 0) ) Trig[6] = 1;//h14
    if( (nHits[10] > 0 && nHits[12] > 0) || (nHits[11] > 0 && nHits[13] > 0) ) Trig[7] = 1;//h23
    if( (nHits[10] > 0 && nHits[14] > 0) || (nHits[11] > 0 && nHits[15] > 0) ) Trig[8] = 1;//h24
    if( (nHits[12] > 0 && nHits[14] > 0) || (nHits[13] > 0 && nHits[15] > 0) ) Trig[9] = 1;//h34
    
    if( (nHits[8]  > 0 && nHits[10] > 0 && nHits[12] > 0) || (nHits[9]  > 0 && nHits[11] > 0 && nHits[13] > 0) ) Trig[10] = 1;//h123
    if( (nHits[8]  > 0 && nHits[10] > 0 && nHits[14] > 0) || (nHits[9]  > 0 && nHits[11] > 0 && nHits[15] > 0) ) Trig[11] = 1;//h124
    if( (nHits[10] > 0 && nHits[12] > 0 && nHits[14] > 0) || (nHits[11] > 0 && nHits[13] > 0 && nHits[15] > 0) ) Trig[12] = 1;//h234
    
    if( (nHits[8]  > 0 && nHits[10] > 0 && nHits[12] > 0 && nHits[14] > 0) || (nHits[9]  > 0 && nHits[11] > 0 && nHits[13] > 0 && nHits[15] > 0) ) Trig[13] = 1;//h1234, nim1
    if( (nHits[0]  > 0 && nHits[2] > 0 && nHits[4] > 0 && nHits[6] > 0) || (nHits[1]  > 0 && nHits[3] > 0 && nHits[5] > 0 && nHits[7] > 0) ) Trig[14] = 1;//h1234, nim2
    Trig[15] = 1;//all hits
    if(ievent%10000 == 0){
      cout<< ievent << " ";
      for(int trig = 0; trig < nTrigs; ++trig) cout << Trig[trig];
      cout << endl;
    }
      //for<< Trig[0] << Trig[1] << Trig[2] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] << Trig[0] <<
    
    //fill the histos
    for(int itrig = 0; itrig < nTrigs-1; ++itrig)
      for(int k = 0; k < nHodos; ++k)
	for(Int_t j = 0; j < rawEvent->getNHits(); ++j){
	  if(e_hodoPos[k][j] == -1)continue;
          if(Trig[itrig] == 1){
            hodoHits[itrig][k] -> Fill(e_hodoPos[k][j]);
          }
        }    
    //evt.clear();
    rawEvent->Clear();
  }

  //draw plots
  TCanvas *c4m[nTrigs];
  for(int i = 0; i < nTrigs; ++i){
    sprintf(Hname, "c4matrix%d", i+1);
    c4m[i] = new TCanvas(Hname, Hname, 2000, 1000);
    c4m[i] -> Divide(4, 2);
    //xhodos
    for(int k = 0; k < 8; ++k){
      c4m[i] -> cd(k+1);
      hodoHits[i][k] -> Draw();
    }
    sprintf(Hname, "./images/XhodoHits_%s.gif", hodoTrigs[i].c_str());
    c4m[i] -> SaveAs(Hname);
    //yhodos
    for(int k = 0; k < 8; ++k){
      c4m[i] -> cd(k+1);
      hodoHits[i][k+8] -> Draw();

    }
    sprintf(Hname, "./images/YhodoHits_%s.gif", hodoTrigs[i].c_str());
    c4m[i] -> SaveAs(Hname);
  }

  /*


  TCanvas *c2 = new TCanvas("c2", "c2", 500, 500);
  mumhodoHits2D[6][0] -> Draw("colz");
  c2 -> SaveAs("./images/2DhodoHits_NIM1_Station1.gif");
  */
}
