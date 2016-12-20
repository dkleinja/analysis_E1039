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
    //Float_t dx, dy, dz;
    Float_t dpx, dpy, dpz, dpt;
    Float_t px1, py1, pz1;
    Float_t px2, py2, pz2;
    Float_t e1, e2;
    Float_t sigWeight;
    
    TTree *kdimuon = new TTree("dmHodos","dmHodos");
    //kdimuon -> Branch ("dx",          &dx,              "dx/F");
    //kdimuon -> Branch ("dy",          &dy,              "dy/F");
    //kdimuon -> Branch ("dz",          &dz,              "dz/F");
    kdimuon -> Branch ("sigWeight",     &sigWeight,      "sigWeight/F");
    kdimuon -> Branch ("dpx",           &dpx,             "dpx/F");
    kdimuon -> Branch ("dpy",           &dpy,             "dpy/F");
    kdimuon -> Branch ("dpz",           &dpz,             "dpz/F");
    kdimuon -> Branch ("dpt",           &dpt,             "dpt/F");
    kdimuon -> Branch ("mass",          &mass,            "mass/F");
    kdimuon -> Branch ("xF",            &xF,              "xF/F");
    kdimuon -> Branch ("xB",            &xB,              "xB/F");
    kdimuon -> Branch ("xT",            &xT,              "xT/F");
    kdimuon -> Branch ("cs_costh",      &cs_costh,        "cs_costh/F");
    kdimuon -> Branch ("cs_phi",        &cs_phi,          "cs_phi/F");
    kdimuon -> Branch ("theta",         &theta,           "theta/F");
    kdimuon -> Branch ("phi",           &phi,             "phi/F");
    kdimuon -> Branch ("px1",           &px1,             "px1/F");
    kdimuon -> Branch ("py1",           &py1,             "py1/F");
    kdimuon -> Branch ("pz1",           &pz1,             "pz1/F");
    kdimuon -> Branch ("e1",            &e1,              "e1/F");
    kdimuon -> Branch ("px2",           &px2,             "px2/F");
    kdimuon -> Branch ("py2",           &py2,             "py2/F");
    kdimuon -> Branch ("pz2",           &pz2,             "pz2/F");
    kdimuon -> Branch ("e2",            &e2,              "e2/F");
      
    DPMCRawEvent* rawEvent = new DPMCRawEvent;
    dataTree->SetBranchAddress("rawEvent", &rawEvent);

    //get phi distributions dimuon
    TLorentzVector lordm;// = new TLorentzVector;
    TLorentzVector track1;// = new TLorentzVector;
    TLorentzVector track2;// = new TLorentzVector;
    
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
	  dpt = lordm.Pt();
	  dpx = lordm.Px(), dpy = lordm.Py(), dpz = lordm.Pz();
	  xF = dimuon.fxF;
	  xB = dimuon.fx1;
	  xT = dimuon.fx2;
	  mass = dimuon.fMass;
	  cs_costh = dimuon.fCosTh;
	  cs_phi = dimuon.fPhi;
	  px1 = dimuon.fPosMomentum.Px(); py1 = dimuon.fPosMomentum.Py(); pz1 =  dimuon.fPosMomentum.Pz(); e1 =  dimuon.fPosMomentum.E();
	  px2 = dimuon.fNegMomentum.Px(); py2 = dimuon.fNegMomentum.Py(); pz2 =  dimuon.fNegMomentum.Pz(); e2 =  dimuon.fNegMomentum.E();
	  
	  phi = lordm.Phi();
	  theta = lordm.Theta();
	  sigWeight = rawEvent->eventHeader().fSigWeight;
	  if(lordm.Pt() > 0.5){
	    Hdmphi -> Fill(phi, sigWeight);
	    Hpmuonphi -> Fill(track1.Phi(), sigWeight);
	    Hmmuonphi -> Fill(track2.Phi(), sigWeight);
	    Hcsphi -> Fill(dimuon.fPhi, sigWeight);
	  }
	  kdimuon -> Fill();
	  //rawEvent -> clear();
	}
    }

    TCanvas* c1 = new TCanvas();
    Hdmphi -> Draw();
   
    sprintf(Fname, "./root_files/phidists_dpsim_batch%d_hodoEff%d_seed%d.root", batch, hodoEff, fileSeed);
    TFile *outfile = new TFile(Fname, "RECREATE");
    outfile -> cd();
    kdimuon -> Write();
    Hdmphi -> Write();
    Hpmuonphi -> Write();
    Hmmuonphi -> Write();
    Hcsphi -> Write();
    
    
}

