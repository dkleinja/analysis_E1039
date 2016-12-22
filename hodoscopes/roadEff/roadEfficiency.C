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

#include "GeomSvc.h"
#include "SRawEvent.h"
#include "SRecEvent.h"
#include "FastTracklet.h"

using namespace std;

int main(int argc, char *argv[])
{
  //Initialization of geometry and tracked data
  GeomSvc* p_geomSvc = GeomSvc::instance();
  p_geomSvc->init();
  //p_geomSvc->init(GEOMETRY_VERSION);

  SRawEvent* rawEvent = new SRawEvent();
  TClonesArray* tracklets = new TClonesArray("Tracklet");

  TFile* dataFile = new TFile(argv[1], "READ");
  TTree* dataTree = (TTree*)dataFile->Get("save");

  dataTree->SetBranchAddress("rawEvent", &rawEvent);
  dataTree->SetBranchAddress("tracklets", &tracklets);

  //Hodoscope IDs
  boost::array<int, 8> hodoIDs = {25, 31, 33, 39, 26, 32, 34, 40};
  const int nHodos = 8;

  //Evaluation tree structure
  //road ID variables
  int eventID;
  int nTracklets;
  int charge;
  int detHalf;  int roadflag;
  int n = 4;//number of planes
  int hodoFlags[n];  //flag for elements IDs
  int roadElements[n]; //[elementIDs]
  
  //hodo, track, trigger variables
  double z_exp, x_exp, y_exp, pos_exp, pos, mom_exp;
  double tdcTime;
  int hodoID, elementID;
  int hodoflag; //flag = 1, should have fired, and did; flag = 0, should have fired, but not
  int matrix1two;
  int matrix1flag, matrix2flag, matrix3flag, matrix4flag, matrix5flag;
  int nim1flag, nim2flag, nim3flag, nim4flag, nim5flag;
  
  TFile* saveFile = new TFile(argv[2], "recreate");
  TTree* saveTree = new TTree("save", "save");

  saveTree->Branch("eventID", &eventID, "eventID/I"); //eventID for debugging
  saveTree->Branch("charge", &charge, "charge/I");
  saveTree->Branch("detHalf", &detHalf, "detHalf/I");
  saveTree->Branch("n", &n, "n/I");
  saveTree->Branch("roadflag", &roadflag, "roadflag/I");
  saveTree->Branch("hodoFlags", hodoFlags, "hodoFlags[n]/I");
  saveTree->Branch("roadElements", roadElements, "roadElements[n]/I");
  saveTree->Branch("mom_exp", &mom_exp, "mom_exp/D");
  saveTree->Branch("matrix1two", &matrix1two, "matrix1two/I");
  saveTree->Branch("matrix1flag", &matrix1flag, "matrix1flag/I");
  saveTree->Branch("matrix2flag", &matrix2flag, "matrix2flag/I");
  saveTree->Branch("matrix3flag", &matrix3flag, "matrix3flag/I");
  saveTree->Branch("matrix4flag", &matrix4flag, "matrix4flag/I");
  saveTree->Branch("matrix5flag", &matrix5flag, "matrix5flag/I");
  saveTree->Branch("nim1flag", &nim1flag, "nim1flag/I");
  saveTree->Branch("nim2flag", &nim2flag, "nim2flag/I");
  saveTree->Branch("nim3flag", &nim3flag, "nim3flag/I");
  saveTree->Branch("nim4flag", &nim4flag, "nim4flag/I");
  saveTree->Branch("nim5flag", &nim5flag, "nim5flag/I");
  
  //User tracks to fill residual distributions
  int nEvtMax = dataTree->GetEntries();
  for(int i = 0; i < nEvtMax; ++i)
    {
      eventID = i;
      if(i%1000 == 0) cout << "Event " << i << endl;
      dataTree->GetEntry(i);
      if(tracklets->GetEntries() < 1) continue;
      rawEvent->reIndex("oa");
      //if(rawEvent->isTriggeredBy(SRawEvent::NIM1))
      if(rawEvent->isTriggeredBy(SRawEvent::MATRIX1))matrix1flag = 1;
      else matrix1flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::MATRIX2))matrix2flag = 1;
      else matrix2flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::MATRIX3))matrix3flag = 1;
      else matrix3flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::MATRIX4))matrix4flag = 1;
      else matrix4flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::MATRIX5))matrix5flag = 1;
      else matrix5flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::NIM1))nim1flag = 1;
      else nim1flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::NIM2))nim2flag = 1;
      else nim2flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::NIM3))nim3flag = 1;
      else nim3flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::NIM4))nim4flag = 1;
      else nim4flag = 2;
      if(rawEvent->isTriggeredBy(SRawEvent::NIM5))nim5flag = 1;
      else nim5flag = 2;

      //Only the first track is used, for simplicity
      //Tracklet* _track = (Tracklet*)tracklets->At(0);
      //for(int track = 0; track < 1; track++){
      
      //use all tracks
      if( tracklets->GetEntries() > 1 && matrix1flag == 1){
	cout << i << " We have two tracks, MATRIX1." << endl;
	matrix1two = 1;
      }
      else matrix1two = 2;
      for(int track = 0; track < tracklets->GetEntries(); track++){
        Tracklet* _track = (Tracklet*)tracklets->At(track);
        if(track > 0)cout << i << "  More than one tracklet! " << track << endl;
	
	if(_track->getNHits() < 16) continue;
	if(_track->getProb() < 0.8) continue;
	mom_exp = _track->getMomentum();
	//if(fabs(_track->ty) < 0.01) continue;
	//if(_track->getMomentum() < 30.0) cout << i << " " << _track->getMomentum() << endl;
	if(mom_exp < 10.0) continue;//cout << i << " " << _track->getMomentum() << endl;
	charge = _track-> getCharge();
	
	for(int tb = 0; tb < 2; tb++){ //loop top and bottom
	  detHalf = tb;
	  roadflag = 1;  //default is 1
	  for(int j = 0; j < 4; ++j){ //loop 4 stations to reset
	    hodoFlags[j] = 0;
	    roadElements[j] = -999;
	  }
	  for(int j = 0; j < 4; ++j) //loop 4 stations, top and bottom
	    {
	      hodoflag = 0;  //reset flag
	      //Expected hit position on one hodo plane
	      hodoID = hodoIDs[4*tb+j];
	      //let's make sure we get correct detectors
	      //cout << i << " " << j << " " << hodoID << " " << p_geomSvc->getDetectorName(hodoID) <<  " " <<  p_geomSvc->getPlaneNElements(hodoID) << endl;
	      z_exp = p_geomSvc->getPlanePosition(hodoID);
	      x_exp = _track->getExpPositionX(z_exp);
	      y_exp = _track->getExpPositionY(z_exp);
	      pos_exp = p_geomSvc->getUinStereoPlane(hodoID, x_exp, y_exp);
	      if(fabs(y_exp) < 3.0) continue;
	      if(!p_geomSvc->isInPlane(hodoID, x_exp, y_exp)) continue;
	      
	      //Get the elementID_expected
	      elementID = p_geomSvc->getExpElementID(hodoID, pos_exp);
	      //Make sure it is not in overlap region
	      if(fabs(pos_exp - p_geomSvc->getMeasurement(hodoID,elementID)) > 0.90*(p_geomSvc->getPlaneSpacing(hodoID) - p_geomSvc->getCellWidth(hodoID)/2.)) continue;
	      
	      //Hit hit_exp = rawEvent->getHit(hodoID, elementID);
	      Hit hit_exp = rawEvent->getTriggerHit(hodoID, elementID);
	      if(hit_exp.detectorID < 0)cout << i << " " << pos_exp << " " << hit_exp.pos << " " << p_geomSvc->getCellWidth(hodoID) << "detectorIDs:  " << hodoID << " " << hit_exp.detectorID << endl;
	      
	      if(hit_exp.detectorID > 0)
		{
		  hodoflag = 1;
		  tdcTime = hit_exp.tdcTime;
		  pos = hit_exp.pos;
		}
	      else
		{
		  hodoflag = 2;
		  tdcTime = -999.;
		  pos = -999.;
		}
	      if(elementID > 0 && elementID <= p_geomSvc->getPlaneNElements(hodoID))
		{
		  hodoFlags[j] = hodoflag;
		  roadElements[j] = elementID;
		}
	    }// end 4-stations
	  for(int j = 0; j < 4; j++) //check if all 4 hodos are in and fired
	    if(hodoFlags[j] != 1) roadflag = 2;
	  if(roadElements[0] > 0 && roadElements[1] > 0 && roadElements[2] > 0 && roadElements[3] > 0)
	    saveTree -> Fill();
	}// end top-bottom
      }
      rawEvent->clear();
      tracklets->Clear();
    }

  //Save the temporary results into the ROOT file
  saveFile->cd();
  saveTree->Write();
  saveFile->Close();
  
  return 1;
}
