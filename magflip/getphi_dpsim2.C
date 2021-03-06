#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>

//it's only here to suppress linter complains, comment out when actually running
//or writing your own analysis macro
//#include <DPMCRawEvent.h>

/*
This macro illustrates how to draw the dimuon mass/xF withing acceptance
*/
void getphi_dpsim2(int hodoeff = 0, int trigon = 1)
{
    gSystem->Load("libDPMCRawEvent.so");

    gStyle -> SetOptFit(1);
    const Float_t PI = 3.14159;
    
    char Fname[128];
    char Hname[128];
    char Tname[128];
    
    sprintf(Tname, "#phi of dimuon, hodoeff = %d", hodoeff);
    TH1D *Hdmphi = new TH1D("Hdmphi", Tname, 64, -3.15, 3.15);
    Hdmphi -> SetMinimum(0.1);
    Hdmphi -> Sumw2();
    
    sprintf(Fname, "./nDST/dy_hodo%d_50M.root", hodoeff);
    TFile* dataFile = new TFile(Fname, "READ");
    TTree* dataTree = (TTree*)dataFile->Get("save");

    DPMCRawEvent* rawEvent = new DPMCRawEvent;
    dataTree->SetBranchAddress("rawEvent", &rawEvent);

    //get phi distributions dimuon
    TLorentzVector lordm;// = new TLorentzVector;
    TLorentzVector track1;// = new TLorentzVector;
    TLorentzVector track2;// = new TLorentzVector;
    Float_t phi;
    
    int nentries = dataTree -> GetEntries();
    cout << "The number of dimuon Entries is " << nentries << endl;
    for(Int_t i = 0; i < dataTree->GetEntries(); ++i)
    {
        dataTree->GetEntry(i);
	if(i%10000 == 0) cout << i << " Events. " << endl;
        //Normally there is only one dimuon per event
        //But pile-up of multiple dimuons is possible

	//get trigger conditions
	//if(trigon == 1){
	  DPMCHeader evt = rawEvent->eventHeader();
	  if((evt.fTriggerBit & 1) == 0) continue;
	  //}
	for(Int_t j = 0; j < rawEvent->getNDimuons(); ++j)
        {
	  DPMCDimuon dimuon = rawEvent->getDimuon(j);
	  
	  if(!dimuon.fAccepted) continue;
	  
	  track1.SetPxPyPzE(dimuon.fPosMomentum.Px(), dimuon.fPosMomentum.Py(), dimuon.fPosMomentum.Pz(), dimuon.fPosMomentum.E());
	  track2.SetPxPyPzE(dimuon.fNegMomentum.Px(), dimuon.fNegMomentum.Py(), dimuon.fNegMomentum.Pz(), dimuon.fNegMomentum.E());
	  lordm = track1 + track2;
	  if(lordm.Pt() < 1.)continue;
	  phi = lordm.Phi();
	  double weight = rawEvent->eventHeader().fSigWeight;
	  Hdmphi -> Fill(phi, weight);
	  //rawEvent -> clear();
	}
    }

    TCanvas* c1 = new TCanvas();
    Hdmphi -> Draw();
   
    sprintf(Fname, "./root_files/phidists_dpsim_hodoeff%d_trigon%d.root", hodoeff, trigon);
    TFile *outfile = new TFile(Fname, "RECREATE");
    outfile -> cd();
    Hdmphi -> Write();
    
}

