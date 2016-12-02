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

  //Get the geometry
  TString* geometry = new TString("Geometry");
  //std::string geom;
  //TString geometry;
  
  TTree* configTree = (TTree*)dataFile->Get("config"); 
  configTree->SetBranchAddress("Geometry", &geometry);
  configTree->GetEntry(0);
  cout << "The Geometry is " << geometry->Data() << endl;
  exit(1);
  //Hodoscope IDs
  boost::array<int, 16> hodoIDs = {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
  const int nHodos = 16;

  //Evaluation tree structure
  double z_exp, x_exp, y_exp, pos_exp, pos, mom_exp;
  double tdcTime;
  int hodoID, elementID;
  int flag; //flag = 1, should have fired, and did; flag = 0, should have fired, but not
  int matrix1flag, matrix2flag, matrix3flag, matrix4flag, matrix5flag;
  int nim1flag, nim2flag, nim3flag, nim4flag, nim5flag;
  int roadID;
  int charge;
  
  TFile* saveFile = new TFile(argv[2], "recreate");
  TTree* saveTree = new TTree("save", "save");

  saveTree->Branch("z_exp", &z_exp, "z_exp/D");
  saveTree->Branch("x_exp", &x_exp, "x_exp/D");
  saveTree->Branch("y_exp", &y_exp, "y_exp/D");
  saveTree->Branch("mom_exp", &mom_exp, "mom_exp/D");
  saveTree->Branch("tdcTime", &tdcTime, "tdcTime/D");
  saveTree->Branch("pos_exp", &pos_exp, "pos_exp/D");
  saveTree->Branch("y_exp", &pos_exp, "y_exp/D");
  saveTree->Branch("pos", &pos, "pos/D");
  saveTree->Branch("hodoID", &hodoID, "hodoID/I");
  saveTree->Branch("elementID", &elementID, "elementID/I");
  saveTree->Branch("flag", &flag, "flag/I");
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
      if(i%1000 == 0) cout << "Event " << i << endl;
      dataTree->GetEntry(i);
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
      if(tracklets->GetEntries() < 1) continue;
      rawEvent->reIndex("oa");

      //Only the first track is used, for simplicity
      //Tracklet* _track = (Tracklet*)tracklets->At(0);
      for(int track = 0; track < 1; track++){
      //use all tracks
      //for(int track = 0; track < tracklets->GetEntries(); track++){
	Tracklet* _track = (Tracklet*)tracklets->At(track);
	if(track > 0)cout << i << "More than one tracklet! " << track << endl;
	if(_track->getNHits() < 16) continue;
	if(_track->getProb() < 0.8) continue;
	mom_exp = _track->getMomentum();
	//if(fabs(_track->ty) < 0.01) continue;
	//if(_track->getMomentum() < 30.0) cout << i << " " << _track->getMomentum() << endl;
	if(_track->getMomentum() < 1.0) continue;//cout << i << " " << _track->getMomentum() << endl;
	charge = _track->getCharge();
	cout << "charge: " << charge << endl;
	for(int j = 0; j < nHodos; ++j)
	  {
	    //Expected hit position on one hodo plane
	    hodoID = hodoIDs[j];
	    z_exp = p_geomSvc->getPlanePosition(hodoID);
	    x_exp = _track->getExpPositionX(z_exp);
	    y_exp = _track->getExpPositionY(z_exp);
	    pos_exp = p_geomSvc->getUinStereoPlane(hodoID, x_exp, y_exp);
	    //if(fabs(y_exp) < 3.0) continue;
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
	      flag = 1;
	      tdcTime = hit_exp.tdcTime;
	      pos = hit_exp.pos;
	    }
	    else
	      {
		flag = 2;
		tdcTime = -999.;
		pos = -999.;
	      }
	    if(elementID > 0 && elementID <= p_geomSvc->getPlaneNElements(hodoID)) saveTree->Fill();
/*
	  //If the expected hit position happen to be also on the other paddle
	  if(fabs(pos - pos_exp) > 0.5*p_geomSvc->getPlaneSpacing(hodoID) && fabs(pos - pos_exp) < 0.5*p_geomSvc->getCellWidth(hodoID))
	    {
	      elementID = pos_exp < pos ? elementID - 1 : elementID + 1;
	      if(elementID < 1 || elementID > p_geomSvc->getPlaneNElements(hodoID)) continue;

	      Hit hit_adj = rawEvent->getHit(hodoID, elementID);
	      if(hit_adj.detectorID > 0)
		{
		  flag = 1;
		  tdcTime = hit_adj.tdcTime;
		  pos = hit_adj.pos;
		}
	      else
		{
		  flag = 2;
		  tdcTime = -999.;
		  pos = -999.;
		}
	      saveTree->Fill();
	    }
*/
	}
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
