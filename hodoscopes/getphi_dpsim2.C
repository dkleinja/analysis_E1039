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
void getphi_dpsim2(int batch = 0, int hodoEff = 0, int fileSeed = 0)
{
    gSystem->Load("libDPMCRawEvent.so");

    gStyle -> SetOptFit(1);
    const Float_t PI = 3.14159;
    
    char Fname[128];
    char Hname[128];
    char Tname[128];
    
    sprintf(Tname, "#phi of dimuon, hodoEff = %d", hodoEff);
    TH1D *Hdmphi = new TH1D("Hdmphi", Tname, 64, -3.15, 3.15);
    //Hdmphi -> SetMinimum(0.1);
    Hdmphi -> Sumw2();

    sprintf(Tname, "#phi of #mu^{-}, hodoEff %d", hodoEff);
    TH1D *Hmmuonphi = new TH1D("Hmmuonphi", Tname, 64, -3.15, 3.15);
    //Hmmuonphi -> SetMinimum(0.1);
    Hmmuonphi -> Sumw2();
    sprintf(Tname, "#phi of #mu^{+}, hodoEff %d", hodoEff);
    TH1D *Hpmuonphi = new TH1D("Hpmuonphi", Tname, 64, -3.15, 3.15);
    //Hpmuonphi -> SetMinimum(0.11);
    Hpmuonphi -> Sumw2();
    
    TH1D *Hcsphi = new TH1D("Hcsphi", Tname, 32, -1.6, 1.6);
    Hcsphi -> SetMinimum(0.1);
    Hcsphi -> Sumw2();
    
    TChain *dataTree = new TChain("save");
    int chainFirst = fileSeed*5;
    for(int i = chainFirst; i < chainFirst+5; i++){
      sprintf(Fname, "./batch%d_dy_hodo%d_100K_R005_%d.root", batch, hodoEff, i);
      printf(Fname, "./batch%d_dy_hodo%d_100K_R005_%d.root", batch, hodoEff, i);
      cout <<endl;
      dataTree -> Add(Fname);
    }
  
    Float_t xF, xT, xB, mass;
    Float_t cs_costh, cs_phi;
    Float_t theta, phi;
    Float_t dx, dy, dz;
    Float_t dpx, dpy, dpz;
    Float_t px1, py1, pz1;
    Float_t px2, py2, pz2;
    
    TTree *kdimuon = new TTree("dmHodos","dmHodos");
    //kdimuon -> Branch ("dx",          &dx,              "dx/F");
    //kdimuon -> Branch ("dy",          &dy,              "dy/F");
    //kdimuon -> Branch ("dz",          &dz,              "dz/F");
    kdimuon -> Branch ("dpx",           &dpx,             "dpx/F");
    kdimuon -> Branch ("dpy",           &dpy,             "dpy/F");
    kdimuon -> Branch ("dpz",           &dpz,             "dpz/F");
    kdimuon -> Branch ("mass",          &mass,            "mass/F");
    kdimuon -> Branch ("xF",            &xF,              "xF/F");
    kdimuon -> Branch ("xB",            &xB,              "xB/F");
    kdimuon -> Branch ("xT",            &xT,              "xT/F");
    kdimuon -> Branch ("cs_costh",      &cs_costh,        "cs_costh/F");
    kdimuon -> Branch ("cs_phi",        &cs_phi,          "cs_phi/F");
    kdimuon -> Branch ("cs_phi",        &cs_phi,          "cs_phi/F");
    kdimuon -> Branch ("cs_phi",        &cs_phi,          "cs_phi/F");
    kdimuon -> Branch ("px1",           &px1,             "px1/F");
    kdimuon -> Branch ("py1",           &py1,             "py1/F");
    kdimuon -> Branch ("pz1",           &pz1,             "pz1/F");
    kdimuon -> Branch ("px2",           &px2,             "px2/F");
    kdimuon -> Branch ("py2",           &py2,             "py2/F");
    kdimuon -> Branch ("pz2",           &pz2,             "pz2/F");
      
    DPMCRawEvent* rawEvent = new DPMCRawEvent;
    dataTree->SetBranchAddress("rawEvent", &rawEvent);

    //get phi distributions dimuon
    TLorentzVector lordm;// = new TLorentzVector;
    TLorentzVector track1;// = new TLorentzVector;
    TLorentzVector track2;// = new TLorentzVector;
    Float_t phi;
    
    int nentries = dataTree -> GetEntries();
    cout << "The number of dimuon Entries is " << nentries << endl;
    for(Int_t i = 0; i < nentries; ++i)
      {
        dataTree->GetEntry(i);
	if(i%1000 == 0) cout << i << " Events. " << endl;
        //Normally there is only one dimuon per event
        //But pile-up of multiple dimuons is possible

	//get trigger conditions
	//if(trigon == 1){
	DPMCHeader evt = rawEvent->eventHeader();
	if((evt.fTriggerBit & 1) == 0){
	//if((evt.fTriggerBit%2 == 0)){
	  //cout << i << "fTriggerBit is:  " << evt.fTriggerBit << endl;
	  continue;
	}
	for(Int_t j = 0; j < rawEvent->getNDimuons(); ++j)
        {
	  DPMCDimuon dimuon = rawEvent->getDimuon(j);
	  
	  //if(!dimuon.fAccepted) continue;
	  
	  track1.SetPxPyPzE(dimuon.fPosMomentum.Px(), dimuon.fPosMomentum.Py(), dimuon.fPosMomentum.Pz(), dimuon.fPosMomentum.E());
	  track2.SetPxPyPzE(dimuon.fNegMomentum.Px(), dimuon.fNegMomentum.Py(), dimuon.fNegMomentum.Pz(), dimuon.fNegMomentum.E());
	  lordm = track1 + track2;
	  if(lordm.Pt() < 1.)continue;
	  phi = lordm.Phi();
	  double weight = rawEvent->eventHeader().fSigWeight;
	  Hdmphi -> Fill(phi, weight);
	  Hpmuonphi -> Fill(track1.Phi(), weight);
	  Hmmuonphi -> Fill(track2.Phi(), weight);
	  Hcsphi -> Fill(dimuon.fPhi, weight);
	  //rawEvent -> clear();
	}
    }

    TCanvas* c1 = new TCanvas();
    Hdmphi -> Draw();
   
    sprintf(Fname, "./root_files/phidists_dpsim_batch%d_hodoEff%d_seed%d.root", batch, hodoEff, fileSeed);
    TFile *outfile = new TFile(Fname, "RECREATE");
    outfile -> cd();
    Hdmphi -> Write();
    Hpmuonphi -> Write();
    Hmmuonphi -> Write();
    Hcsphi -> Write();
    
}

