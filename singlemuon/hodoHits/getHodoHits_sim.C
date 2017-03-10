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

  //gStyle -> SetOptFit(1);
  const Float_t PI = 3.14159;
  
  char Fname[128];
  char Hname[128];
  char Tname[128];
  
  const int nHodos = 8;
  int nElements[8] = {23, 23, 16, 16, 16, 16, 16, 16};
  int hodoIDs[8] = {25, 26, 31, 32, 33, 34, 39, 40};
  std::string hodoNames[8] = {"H1B", "H1T", "H2B", "H2T", "H3B", "H3T", "H4B", "H4T"};

  TH1I *hodoHits[4][8];
  for(int i = 0; i < 4; ++i)
    for(int k = 0; k < nHodos; ++k)
    {
      //cout<<i << " " << k << " blah"<<endl;
      sprintf(Hname, "nHits_matrix%d_hodo%d", i+1, k);
      sprintf(Tname, "rawMATRIX%d hits on %s", i+1, hodoNames[k].c_str() );
      hodoHits[i][k] = new TH1I(Hname, Tname, nElements[k], 1, nElements[k]+1);
    }
    cout<<"blah"<<endl;
  TFile* dataFile = new TFile("MATRIXcheck_20K_988x50M.root", "READ");
  cout<<"blah"<<endl;
  TTree* dataTree = (TTree*)dataFile->Get("save");
  cout<<"blah"<<endl;
  DPMCRawEvent* rawEvent = new DPMCRawEvent;
  dataTree->SetBranchAddress("rawEvent", &rawEvent);

  int matrix1, matrix2, matrix3, matrix4;
  int elementID, detectorID;
  
  int nentries = dataTree -> GetEntries();
  cout << "The number of Entries is: " << nentries << endl;
  for(Int_t i = 0; i < nentries; ++i){
  //for(Int_t i = 0; i < 10000; ++i){
    matrix1=0; matrix2=0; matrix3=0; matrix4=0;
    dataTree->GetEntry(i);
    if(i%10000 == 0) cout << i << " Events. " << endl;
    //get trigger conditions
    //if(trigon == 1){
    DPMCHeader evt = rawEvent->eventHeader();
    if((evt.fTriggerBit>>0 & 1) != 0) matrix1 = 1;
    if((evt.fTriggerBit>>1 & 1) != 0) matrix2 = 1;
    if((evt.fTriggerBit>>2 & 1) != 0) matrix3 = 1;
    if((evt.fTriggerBit>>3 & 1) != 0) matrix4 = 1;

    if(i%100000 == 0)cout << i << "  Number of hits this event is " << rawEvent->getNHits() << endl;
    for(Int_t j = 0; j < rawEvent->getNHits(); ++j){
      DPMCHit hit = rawEvent->getHit(j);
      elementID = hit.fElementID;
      detectorID = hit.fDetectorID;
      for(int k = 0; k < 8; ++k){
	if(detectorID == hodoIDs[k]){
	  if(matrix1==1)hodoHits[0][k] -> Fill(elementID);
	  if(matrix2==1)hodoHits[1][k] -> Fill(elementID);
	  if(matrix3==1)hodoHits[2][k] -> Fill(elementID);
	  if(matrix4==1)hodoHits[3][k] -> Fill(elementID);
	}
      }
      //hit.clear();
    }
    //evt.clear();
    rawEvent->Clear();
  }

  TCanvas *c4m[4];
  for(int i = 0; i < 4; ++i){
    sprintf(Hname, "c4matrix%d", i+1);
    c4m[i] = new TCanvas(Hname, Hname, 2000, 1000);
    c4m[i] -> Divide(4, 2);
    for(int k = 0; k < nHodos; ++k){
      cout << i << " " << k << endl;
      c4m[i] -> cd(k+1);
      cout << i << " " << k << endl;
      hodoHits[i][k] -> Draw();
      
    }
  }

}
