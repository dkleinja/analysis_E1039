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
#include <TVector3.h>
#include <TLorentzVector.h>

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
  
  const int nTrigs = 21;
  const int nHodos = 16;
  int nElements[nHodos] = {23, 23, 16, 16, 16, 16, 16, 16, 20, 20, 19, 19, 16, 16, 16, 16 };
  int hodoIDs[nHodos] = {25, 26, 31, 32, 33, 34, 39, 40, 27, 28, 29, 30, 35, 36, 37, 38 };
  std::string hodoNames[nHodos] = {"H1B", "H1T", "H2B", "H2T", "H3B", "H3T", "H4B", "H4T", "H1L", "H1R", "H2L", "H2R", "H4Y1L", "H4Y1R", "H4Y2L", "H4Y2R"};
  std::string hodoTrigs[13] = {"1H1Y", "1H2Y", "1H3Y", "1H4Y", "2H12Y", "2H13Y", "2H14Y", "2H23Y", "2H24Y", "3H123Y", "3H124Y", "3H234Y"};
  //std::string hodoTrigs[13] = {"1H1Y", "1H2Y", "1H3Y", "1H4Y", "2H12Y", "2H13Y", "2H14Y", "2H23Y", "2H24Y", "2H34Y", "3H123Y", "3H124Y", "3H234Y"};
  
  /* 
  int nElements[8] = {20, 20, 19, 19, 16, 16, 16, 16};
  int hodoIDs[8] = {27, 28, 29, 30, 35, 36, 37, 38}; 
  std::string hodoNames[8] = {"H1L", "H1R", "H2L", "H2R", "H4Y1L", "H4Y1R", "H4Y2L", "H4Y2R"};
  TFile* dataFile = new TFile("hodoeff_Y.root", "READ");
  */

  TH1I *hodoHits[nTrigs][nHodos];
  TH2I *mumhodoHits2D[nTrigs][2][2];
  TH2I *muphodoHits2D[nTrigs][2][2];
  TH2I *emhodoHits2D[nTrigs][2][2];
  TH2I *ephodoHits2D[nTrigs][2][2];
  for(int i = 0; i < nTrigs; ++i){
    for(int k = 0; k < nHodos; ++k){
      //cout<<i << " " << k << " blah"<<endl;
      sprintf(Hname, "nHits_matrix%d_hodo%d", i, k);
      if(i>5)sprintf(Hname, "nHits_nim%d_hodo%d", i-5, k);
      if(i>8)sprintf(Hname, "nHits_%s_hodo%d",  hodoTrigs[i-9].c_str(), k);
      sprintf(Tname, "rawMATRIX%d hits on %s", i, hodoNames[k].c_str() );
      if(i>5)sprintf(Tname, "rawNIM%d hits on %s", i-5, hodoNames[k].c_str() );
      if(i>8)sprintf(Tname, "raw%s hits on %s", hodoTrigs[i-9].c_str(), hodoNames[k].c_str() ); 
      hodoHits[i][k] = new TH1I(Hname, Tname, nElements[k], 1, nElements[k]+1);
    }

    //get 2D hodo plots
    for(int j = 0; j < 2; ++j)//two x-hodos
      for(int k = 0; k < 2; ++k){//two y-hodos
	sprintf(Hname, "mumHits_matrix%d_hodostation%d%d", i, j, k);
	printf(Hname);cout<<endl;
	sprintf(Tname, "#mu^{-} rawMATRIX%d hits on Station %s, %s", i, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	if(i>5)sprintf(Tname, "#mu^{-} rawNIM%d hits on Station %s, %s", i-5, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	mumhodoHits2D[i][j][k] = new TH2I(Hname, Tname, nElements[j], 1, nElements[j]+1, nElements[k+8], 1, nElements[k+8]+1);
	
	sprintf(Hname, "mupHits_matrix%d_hodostation%d%d", i, j, k);
	sprintf(Tname, "#mu^{+} rawMATRIX%d hits on Station %s, %s", i, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	if(i>5)sprintf(Tname, "#mu^{+} rawNIM%d hits on Station %s, %s", i-5, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	muphodoHits2D[i][j][k] = new TH2I(Hname, Tname, nElements[j], 1, nElements[j]+1, nElements[k+8], 1, nElements[k+8]+1);

	sprintf(Hname, "emHits_matrix%d_hodostation%d%d", i, j, k);
	sprintf(Tname, "e^{-} rawMATRIX%d hits on Station %s, %s", i, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	if(i>5)sprintf(Tname, "#e^{-}rawNIM%d hits on Station %s, %s", i-5, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	emhodoHits2D[i][j][k] = new TH2I(Hname, Tname, nElements[j], 1, nElements[j]+1, nElements[k+8], 1, nElements[k+8]+1);

	sprintf(Hname, "epHits_matrix%d_hodostation%d%d", i, j, k);
	sprintf(Tname, "#e^{+} rawMATRIX%d hits on Station %s, %s", i, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	if(i>5)sprintf(Tname, "e^{+} rawNIM%d hits on Station %s, %s", i-5, hodoNames[j].c_str(),  hodoNames[k+8].c_str());
	ephodoHits2D[i][j][k] = new TH2I(Hname, Tname, nElements[j], 1, nElements[j]+1, nElements[k+8], 1, nElements[k+8]+1);
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

  //trigger variables
  int matrix0, matrix1, matrix2, matrix3, matrix4, matrix5;
  int nim0, nim1, nim2, nim3;
  int h1, h2, h3, h4;
  int h12, h13, h14, h23, h24, h34;
  int h123, h124, h234;

  //hit variables
  int elementID, detectorID;
  int YelementID, YdetectorID;
  int detx, dety;
  float sigWeight;

  //track variables;
  float zProd;
  float pdg, parentpdg;
  TVector3 p_pos;
  //TLorentzVector *p_pos;
  //TClonesArray* posArr = new TClonesArray("TVector3");
  
  int nentries = dataTree -> GetEntries();
  cout << "The number of Entries is: " << nentries << endl;
  for(Int_t i = 0; i < nentries; ++i){
  //for(Int_t i = 0; i < 50000; ++i){
    matrix0=0; matrix1=0; matrix2=0; matrix3=0; matrix4=0;matrix5=0;
    nim0=0; nim1=0; nim2=0; nim3=0;
    h1=0; h2=0; h3=0; h4=0;
    h12=0; h13=0; h14=0; h23=0; h24=0;
    h123=0; h124=0; h234=0;
    dataTree->GetEntry(i);
    if(i%10000 == 0) cout << i << " Events. " << endl;
    //get trigger conditions
    //if(trigon == 1){
    DPMCHeader evt = rawEvent->eventHeader();
    //sigWeight = evt.fSigWeight*100.;
    sigWeight = 1.;
    if(evt.fTriggerBit > 0) matrix0 = 1;
    if((evt.fTriggerBit>>0 & 1) != 0) matrix1 = 1;
    if((evt.fTriggerBit>>1 & 1) != 0) matrix2 = 1;
    if((evt.fTriggerBit>>2 & 1) != 0) matrix3 = 1;
    if((evt.fTriggerBit>>3 & 1) != 0) matrix4 = 1;
    if((evt.fTriggerBit>>4 & 1) != 0) matrix5 = 1;
    if((evt.fTriggerBit>>5 & 1) != 0) nim1 = 1;
    if((evt.fTriggerBit>>6 & 1) != 0) nim2 = 1;
    if((evt.fTriggerBit>>7 & 1) != 0) nim3 = 1;
    if((evt.fTriggerBit>>8 & 1) != 0) h1 = 1;
    if((evt.fTriggerBit>>9 & 1) != 0) h2 = 1;
    if((evt.fTriggerBit>>10 & 1) != 0) h3 = 1;
    if((evt.fTriggerBit>>11 & 1) != 0) h4 = 1;
    if((evt.fTriggerBit>>12 & 1) != 0) h12 = 1;
    if((evt.fTriggerBit>>13 & 1) != 0) h13 = 1;
    if((evt.fTriggerBit>>14 & 1) != 0) h14 = 1;
    if((evt.fTriggerBit>>15 & 1) != 0) h23 = 1;
    if((evt.fTriggerBit>>16 & 1) != 0) h24 = 1;
    //if((evt.fTriggerBit>>17 & 1) != 0) h34 = 1; out for now need to fix
    if((evt.fTriggerBit>>17 & 1) != 0) h123 = 1;
    if((evt.fTriggerBit>>18 & 1) != 0) h124 = 1;
    if((evt.fTriggerBit>>19 & 1) != 0) h234 = 1;
	
    if(i%100000 == 0)cout << i << "  Number of hits this event is " << rawEvent->getNHits() << endl;
    for(Int_t j = 0; j < rawEvent->getNHits(); ++j){
      DPMCHit hit = rawEvent->getHit(j);
      elementID = hit.fElementID;
      detectorID = hit.fDetectorID;
      if(abs(hit.fPDGCode)!=13) continue;//only do muons for now
      for(Int_t j2 = 0; j2 < rawEvent->getNTracks(); ++j2){
	DPMCTrack track = rawEvent->getTrack(j2);
	p_pos = track.fInitialPos;
	if(p_pos.Z() < 0.) continue;

	//trackparentpdg = track.fParentPDGCode;
	if(hit.fTrackID == track.fTrackID){
	  if(abs(track.fPDGCode)!=13)cout << i << " " << j << " " << j2 << " " << p_pos.X() << " " << p_pos.Y() << " " << p_pos.Z() << " " << track.fPDGCode << endl;
	  for(int k = 0; k < nHodos; ++k){
	    if(detectorID == hodoIDs[k]){
	      hodoHits[0][k] -> Fill(elementID, sigWeight);
	      //if(matrix0==1)hodoHits[0][k] -> Fill(elementID, sigWeight);
	      if(matrix1==1)hodoHits[1][k] -> Fill(elementID, sigWeight);
	      if(matrix2==1)hodoHits[2][k] -> Fill(elementID, sigWeight);
	      if(matrix3==1)hodoHits[3][k] -> Fill(elementID, sigWeight);
	      if(matrix4==1)hodoHits[4][k] -> Fill(elementID, sigWeight);
	      if(matrix5==1)hodoHits[5][k] -> Fill(elementID, sigWeight);
	      if(nim1==1)hodoHits[6][k] -> Fill(elementID, sigWeight);
	      if(nim2==1)hodoHits[7][k] -> Fill(elementID, sigWeight);
	      if(nim3==1)hodoHits[8][k] -> Fill(elementID, sigWeight);
	      if(h1==1)hodoHits[9][k] -> Fill(elementID, sigWeight);
	      if(h2==1)hodoHits[10][k] -> Fill(elementID, sigWeight);
	      if(h3==1)hodoHits[11][k] -> Fill(elementID, sigWeight);
	      if(h4==1)hodoHits[12][k] -> Fill(elementID, sigWeight);
	      if(h12==1)hodoHits[13][k] -> Fill(elementID, sigWeight);
	      if(h13==1)hodoHits[14][k] -> Fill(elementID, sigWeight);
	      if(h14==1)hodoHits[15][k] -> Fill(elementID, sigWeight);
	      if(h23==1)hodoHits[16][k] -> Fill(elementID, sigWeight);
	      if(h24==1)hodoHits[17][k] -> Fill(elementID, sigWeight);
	      //if(h34==1)hodoHits[18][k] -> Fill(elementID, sigWeight);
	      if(h123==1)hodoHits[18][k] -> Fill(elementID, sigWeight);
	      if(h124==1)hodoHits[19][k] -> Fill(elementID, sigWeight);
	      if(h234==1)hodoHits[20][k] -> Fill(elementID, sigWeight);
	    }
	  }
	}
      }
      /*
      //fill 2D plots
      for(Int_t y = j+1; y < rawEvent->getNHits(); ++y){

	DPMCHit hit2 = rawEvent->getHit(y);
	YelementID = hit2.fElementID;
	YdetectorID = hit2.fDetectorID;
	detx = -1; dety = -1;
	if(detectorID==25)detx = 0;
	if(detectorID==26)detx = 1;
	if(detx == -1)break;
	if(YdetectorID==27)dety= 0;
	if(YdetectorID==28)dety= 1;
	if(dety == -1) continue;
	//cout<< "Blah " << y << " " << detectorID << " " << detx << " " << YdetectorID << " " << dety << " " << hit.fPDGCode << " " << hit2.fPDGCode << endl;

	if(hit.fPDGCode==13 && hit2.fPDGCode==13){
	  mumhodoHits2D[0][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix1==1)mumhodoHits2D[1][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix2==1)mumhodoHits2D[2][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix3==1)mumhodoHits2D[3][detx][dety] -> Fill(elementID, YelementID);	
	  if(matrix4==1)mumhodoHits2D[4][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix5==1)mumhodoHits2D[5][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==1)mumhodoHits2D[6][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==2)mumhodoHits2D[7][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==3)mumhodoHits2D[8][detx][dety] -> Fill(elementID, YelementID);

	}

	if(hit.fPDGCode==-13 && hit2.fPDGCode==-13){
	  muphodoHits2D[0][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix1==1)muphodoHits2D[1][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix2==1)muphodoHits2D[2][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix3==1)muphodoHits2D[3][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix4==1)muphodoHits2D[4][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix5==1)muphodoHits2D[5][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==1)muphodoHits2D[6][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==2)muphodoHits2D[7][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==3)muphodoHits2D[8][detx][dety] -> Fill(elementID, YelementID);
	}
	if(hit.fPDGCode==11 && hit2.fPDGCode==11){
	  emhodoHits2D[0][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix1==1)emhodoHits2D[1][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix2==1)emhodoHits2D[2][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix3==1)emhodoHits2D[3][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix4==1)emhodoHits2D[4][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix5==1)emhodoHits2D[5][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==1)emhodoHits2D[6][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==2)emhodoHits2D[7][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==3)emhodoHits2D[8][detx][dety] -> Fill(elementID, YelementID);
	}
	if(hit.fPDGCode==-11 && hit2.fPDGCode==-11){
	  ephodoHits2D[0][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix1==1)ephodoHits2D[1][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix2==1)ephodoHits2D[2][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix3==1)ephodoHits2D[3][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix4==1)ephodoHits2D[4][detx][dety] -> Fill(elementID, YelementID);
	  if(matrix5==1)ephodoHits2D[5][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==1)emhodoHits2D[6][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==2)ephodoHits2D[7][detx][dety] -> Fill(elementID, YelementID);
	  if(nim1==3)ephodoHits2D[8][detx][dety] -> Fill(elementID, YelementID);
	}
	}*/
      //hit.clear();
    }
    //evt.clear();
    rawEvent->Clear();
  }
  
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
    sprintf(Hname, "./images/XhodoHits_MATRIX%d.gif", i);
    if(i>5)sprintf(Hname, "./images/XhodoHits_NIM%d.gif", i-5);
    if(i>8)sprintf(Hname, "./images/XhodoHits_%s.gif", hodoTrigs[i-9].c_str());
    c4m[i] -> SaveAs(Hname);

    //yhodos
    for(int k = 0; k < 8; ++k){
      c4m[i] -> cd(k+1);
      hodoHits[i][k+8] -> Draw();
      
    }
    sprintf(Hname, "./images/YhodoHits_MATRIX%d.gif", i);
    if(i>5)sprintf(Hname, "./images/YhodoHits_NIM%d.gif", i-5);
    if(i>8)sprintf(Hname, "./images/YhodoHits_%s.gif", hodoTrigs[i-9].c_str());
    c4m[i] -> SaveAs(Hname);

    //2d plots
    for(int j = 0; j < 2; ++j)
      for(int k = 0; k < 2; ++k){
	c4m[i] -> cd(2*j+k+1);
	mumhodoHits2D[i][j][k] -> Draw("colz");
	c4m[i] -> cd(2*j+k+5);
	muphodoHits2D[i][j][k] -> Draw("colz");
      }	
    sprintf(Hname, "./images/muon_hodoHits_MATRIX%d.gif", i);
    if(i>5)sprintf(Hname, "./images/muon_hodoHits_NIM%d.gif", i-5);
    if(i>8)sprintf(Hname, "./images/muon_hodoHits_%s.gif", hodoTrigs[i-9].c_str());
    c4m[i] -> SaveAs(Hname);

    for(int j = 0; j < 2; ++j)
      for(int k = 0; k < 2; ++k){
	c4m[i] -> cd(2*j+k+1);
	emhodoHits2D[i][j][k] -> Draw("colz");
	c4m[i] -> cd(2*j+k+5);
	ephodoHits2D[i][j][k] -> Draw("colz");
      }	
    sprintf(Hname, "./images/electron_hodoHits_MATRIX%d.gif", i);
    if(i>5)sprintf(Hname, "./images/electron_hodoHits_NIM%d.gif", i-5);
    if(i>8)sprintf(Hname, "./images/electron_hodoHits_%s.gif", hodoTrigs[i-9].c_str());
    c4m[i] -> SaveAs(Hname);
    
  }
  /*


  TCanvas *c2 = new TCanvas("c2", "c2", 500, 500);
  mumhodoHits2D[6][0] -> Draw("colz");
  c2 -> SaveAs("./images/2DhodoHits_NIM1_Station1.gif");
  */
}
