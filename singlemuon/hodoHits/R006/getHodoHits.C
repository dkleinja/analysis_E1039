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
#include <TChain.h>
#include <TRandom.h>
#include <TMatrixD.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TH1I.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TClonesArray.h>
#include <TArrow.h>
#include <TPaveText.h>
#include <TSystem.h>

#include "SRawEvent.h"
#include "SRecEvent.h"

using namespace std;

int main(int argc, char *argv[])
{

  //gStyle -> SetOptFit(1);
  const Float_t PI = 3.14159;
  
  char Fname[128];
  char Hname[128];
  char Tname[128];
  
  const int nTrigs = 9;
  const int nHodos = 16;
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
  for(int i = 0; i < nTrigs; ++i)
    for(int k = 0; k < nHodos; ++k)
    {
      //cout<<i << " " << k << " blah"<<endl;
      sprintf(Hname, "nHits_matrix%d_hodo%d", i, k);
      sprintf(Tname, "rawMATRIX%d hits on %s", i, hodoNames[k].c_str() );
      if(i>5)sprintf(Tname, "rawNIM%d hits on %s", i-5, hodoNames[k].c_str() );
      hodoHits[i][k] = new TH1I(Hname, Tname, nElements[k], 1, nElements[k]+1);
    }

  //python file method
  //TFile* dataFile = new TFile(argv[1], "READ");
  //TTree* dataTree = (TTree*)dataFile->Get("save");
  
  //tchain method
  TChain *dataTree = new TChain("save");
  int chainfirst = 12525;
  //int chainlast = 12600;
  int chainlast = 15789;
  
  for(int i = chainfirst; i <= chainlast; i++){
    //sprintf(Fname, "./digit_0%d_R006.root", i);
    sprintf(Fname, "./roadset67DSTs/track_0%d_r1.6.0.root", i);
    //cout << "Getting File " << Fname << endl;
    dataTree -> Add(Fname);
  }
  
  SRawEvent* rawEvent = new SRawEvent();
  SRecEvent* recEvent = new SRecEvent();
  dataTree->SetBranchAddress("rawEvent", &rawEvent);
  dataTree->SetBranchAddress("recEvent", &recEvent);

  Int_t  matrix1, matrix2, matrix3, matrix4, matrix5;
  Int_t  nim1, nim2, nim3;
  Int_t elementID, detectorID;
  
  int nentries = dataTree -> GetEntries();
  cout << "The number of Entries is: " << nentries << endl;
  for(Int_t i = 0; i < nentries; ++i){
    //for(Int_t i = 0; i < 10000; ++i){
    matrix1=0; matrix2=0; matrix3=0; matrix4=0, matrix5=0, nim1=0, nim2=0, nim3=0;
    dataTree->GetEntry(i);
    if(i%10000 == 0) cout << i << " Events. " << endl;
    //make sure hit & track form same event
    //if(rawEvent->getSpillID() != recEvent->getSpillID() && rawEvent->getEventID() != recEvent->getEventID()) continue;
    //cout << i << " " << rawEvent->getSpillID() << " " << recEvent->getSpillID() << " " << rawEvent->getEventID() << " " << recEvent->getEventID() << endl;

    //get trigger conditions
    //if(trigon == 1){
    if(rawEvent->isTriggeredBy(SRawEvent::MATRIX1))matrix1 = 1;
    if(rawEvent->isTriggeredBy(SRawEvent::MATRIX2))matrix2 = 1;
    if(rawEvent->isTriggeredBy(SRawEvent::MATRIX3))matrix3 = 1;
    if(rawEvent->isTriggeredBy(SRawEvent::MATRIX4))matrix4 = 1;
    if(rawEvent->isTriggeredBy(SRawEvent::MATRIX5))matrix5 = 1;
    if(rawEvent->isTriggeredBy(SRawEvent::NIM1))nim1 = 1;
    if(rawEvent->isTriggeredBy(SRawEvent::NIM2))nim2 = 1;
    if(rawEvent->isTriggeredBy(SRawEvent::NIM3))nim3 = 1;
    //if(rawEvent->isTriggeredBy(SRawEvent::NIM4))nim4 = 1;
    //if(rawEvent->isTriggeredBy(SRawEvent::NIM5))nim5 = 1;

    if(i%100000 == 0)cout << i << "  Number of hits this event is " << rawEvent->getNTriggerHits() << endl;
    for(Int_t j = 0; j < rawEvent->getNTriggerHits(); ++j){
      Hit hit = rawEvent->getTriggerHit(j);
      //if(i%100000 == 0)cout << i << "  Number of hits this event is " << rawEvent->getNHitsAll() << endl;
      //for(Int_t j = 0; j < rawEvent->getNHitsAll(); ++j){
      //Hit hit = rawEvent->getHit(j);
      elementID = hit.elementID;
      detectorID = hit.detectorID;
      for(Int_t j2 = 0; j2 < recEvent->getNTracks(); ++j2){
	
        SRecTrack track = recEvent->getTrack(j2);
	//do track cuts here

	if(!track.isValid())continue;
	//cout << track.getChisq() << " " << track.getChisqVertex() << " " << track.getChisqTarget() << " " << track.getChisqDump() << endl;
	//cout << i << " " << j << " There are not many valid tracks.  This one is Valid " << track.isValid() <<  " " << track.isTarget() << endl;//" " << track.isDump() << endl;
	//if(track.isTarget()){
	for(int k = 0; k < nHodos; ++k){
	  if(detectorID == hodoIDs[k]){
	    hodoHits[0][k] -> Fill(elementID);
	    if(matrix1==1)hodoHits[1][k] -> Fill(elementID);
	    if(matrix2==1)hodoHits[2][k] -> Fill(elementID);
	    if(matrix3==1)hodoHits[3][k] -> Fill(elementID);
	    if(matrix4==1)hodoHits[4][k] -> Fill(elementID);
	    if(matrix5==1)hodoHits[5][k] -> Fill(elementID);
	    if(nim1==1)hodoHits[6][k] -> Fill(elementID);
	    if(nim2==1)hodoHits[7][k] -> Fill(elementID);
	    if(nim3==1)hodoHits[8][k] -> Fill(elementID);
	    //}
	  }
	}
      }
      //hit.clear();
    }
    //evt.clear();
    rawEvent->Clear();
  }

  //save to file
  TFile* saveFile = new TFile(argv[2], "recreate");
  for(int i = 0; i < nTrigs; ++i){
    for(int k = 0; k < nHodos; ++k){
      hodoHits[i][k] -> Write();
    }
  }
  
  //draw files
  TCanvas *c4m[nTrigs];
  for(int i = 0; i < nTrigs; ++i){
    sprintf(Hname, "c4matrix%d", i+1);
    c4m[i] = new TCanvas(Hname, Hname, 2000, 1000);
    c4m[i] -> Divide(4, 2);
    for(int k = 0; k < 8; ++k){
      c4m[i] -> cd(k+1);
      hodoHits[i][k] -> Draw();
      
    }
    sprintf(Hname, "./images/real_XhodoHitsMATRIX%d.gif", i);
    if(i>5)sprintf(Hname, "./images/real_XhodoHits_NIM%d.gif", i-5);
    c4m[i] -> SaveAs(Hname);

    for(int k = 0; k < 8; ++k){
      c4m[i] -> cd(k+1);
      hodoHits[i][k+8] -> Draw();
      
    }
    sprintf(Hname, "./images/real_YhodoHits_MATRIX%d.gif", i);
    if(i>5)sprintf(Hname, "./images/real_YhodoHits_NIM%d.gif", i-5);
    c4m[i] -> SaveAs(Hname); 
    
  }
  
}
