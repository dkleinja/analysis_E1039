#ifndef DRAWHODO_DPSIM2_H__
#define DRAWHODO_DPSIM2_H__

const Float_t PI = 3.14159;

char Fname[128];
char Hname[128];
char Tname[128];
char onoff[4];

//TTree variables;
Float_t xF, xT, xB, mass;
Float_t cs_costh, cs_phi;
Float_t cs_costh2, cs_phi2, cs_th2;
Float_t theta, phi;
//Float_t dx, dy, dz;
Float_t cs_dpx, cs_dpy, cs_dpz, dpz, cs_energy;
Float_t dpx, dpy, dpz, dpt, energy;
Float_t px1, py1, pz1;
Float_t px2, py2, pz2;
Float_t e1, e2;
Float_t sigWeight;

//get phi distributions dimuon
TLorentzVector lordm;// = new TLorentzVector;
TLorentzVector track1;// = new TLorentzVector;
TLorentzVector track2;// = new TLorentzVector;
TLorentzVector cs_lordm;// = new TLorentzVector;

//binned histos
TH3D *Hallphi[2];
TH1D *Hdmphi[2];
TH1D *Hpmuonphi[2];
TH1D *Hmmuonphi[2];
TH1D *Hdmcsphi[2];
TH1D *Hdmcstheta[2];

TH1D *Hphi[2][4];
TH1D *Hcsphi[2][4];
TH1D *Hcstheta[2][4];
TH1D *Hmass[2][4];
TH1D *HxF[2][4];
TH1D *Hdpz[2][4];

TH2D *Hcsphiphi[4];

TH1D *Hphirat[4];
TH1D *Hcsthetarat[4];
TH1D *Hcsphirat[4];
TH1D *Hmassrat[4];
TH1D *HxFrat[4];
TH1D *Hdpzrat[4];
Float_t ptTitleMin, ptTitleMax;

//cut varibles
Float_t PTMIN = 0.5;
Float_t PTMAX = 5.0;
Float_t MASS = 0.5;
Float_t PZ = 20.;

//Draw variables
TCanvas *c4[6];

#endif // DRAWHODO_DPSIM2_H_
