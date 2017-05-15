#ifndef GETHODOHITS_H__
#define GETHODOHITS_H__

//sprintf, histos, tfiles
int target;
char Fname[128];
char Hname[128];
char Tname[128];
const int nTrigs = 16;
const int nHodos = 16;
TH1I *hodoHits[nTrigs][nHodos];

//tree variables
Int_t all, pions, kaons, others;
Int_t n;
Int_t pdg[1000];
Int_t parentID[1000];
Float_t zProd;
Float_t px_0, py_0, pz_0;
Float_t x_0, y_0, z_0;
Int_t charge;

//Constants
int nElements[nHodos] = {23, 23, 16, 16, 16, 16, 16, 16, 20, 20, 19, 19, 16, 16, 16, 16 };
int hodoIDs[nHodos] = {25, 26, 31, 32, 33, 34, 39, 40, 27, 28, 29, 30, 35, 36, 37, 38 };
std::string hodoNames[nHodos] = {"H1B", "H1T", "H2B", "H2T", "H3B", "H3T", "H4B", "H4T", "H1L", "H1R", "H2L", "H2R", "H4Y1L", "H4Y1R", "H4Y2L", "H4Y2R"};
std::string hodoTrigs[nTrigs] = {"1H1Y", "1H2Y", "1H3Y", "1H4Y", "2H12Y", "2H13Y", "2H14Y", "2H23Y", "2H24Y", "3H123Y", "3H124Y", "3H234Y", "NIM1", "NIM2", "NIM3"};

Float_t L0_hodoPos[nHodos] = {-80.53, -80.53, -101.46, -101.46, -114.16, -114.16, -154.63, -154.63, -70.03, -70.03, -120.48, -120.48, -185.28, -185.28, -185.28, -185.28};
Float_t LF_hodoPos[nHodos] = {80.53, 80.53, 101.46, 101.46, 114.16, 114.16, 154.63, 154.63, 70.03, 70.03, 120.48, 120.48, 185.28, 185.28, 185.28, 185.28};

Float_t W0_hodoPos[nHodos] = {-34.925, 0., -76., 0., -83.82, 0., -91.44, 0.,  0, -39.37, 0., -66, 0., -76.2, 0., -76.2};
Float_t WF_hodoPos[nHodos] = {0.,  34.925, 0.,  76., 0.,  83.82, 0.,  91.44,  39.37, 0.,  66, 0.,  76.2, 0.,  76.2, 0.}; 

Float_t Z_hodoPos[nHodos] = {644., 644., 1420.95, 1421.28, 1958.34, 1958.9, 2251.71, 2234.29, 628., 628., 1405.09, 1404.78, 2130.27, 2146.45, 2200.44, 2216.62};

Float_t FMAG = -2.0;
Float_t KMAG = 0.3;

Float_t Z0_FMAG = 0;
Float_t ZF_FMAG = 502.92;
Float_t ZM_KMAG = 1042.02; 

//hit variables;
Float_t dpx_fmag;
Float_t dpx_kmag = 0.4;
Float_t dx_fmag, dx_kmag;

Float_t x_target, y_target;
Float_t x_fmag, y_fmag;
Float_t x_kmag, y_kmag;

const int nTracks = 100;
Float_t x_hodoPos[nHodos][nTracks];
Float_t y_hodoPos[nHodos][nTracks];

//trigger variables
Int_t nHits[nHodos];
Int_t Trig[nTrigs];

#endif // GETHODOHITS_H_

/*
Float_t L_H1B   = 161.06;
Float_t L_H1T   = 161.06;
Float_t L_H2B   = 202.92;
Float_t L_H2T   = 202.92;
Float_t L_H3B   = 228.32;
Float_t L_H3T   = 228.32;
Float_t L_H4B   = 309.28;
Float_t L_H4T   = 309.28;

Float_t L_H1L   = 140.05;
Float_t L_H1R   = 140.05;
Float_t L_H2L   = 240.97;
Float_t L_H2R   = 240.97;
Float_t L_H4Y1L = 370.56;
Float_t L_H4Y1R = 370.56;
Float_t L_H4Y2L = 370.56;
Float_t L_H4Y2R = 370.56;

Float_t X0_H1B   = 0.;
Float_t X0_H1T   = 0.;
Float_t X0_H2B   = 0.;
Float_t X0_H2T   = 0.;
Float_t X0_H3B   = 0.;
Float_t X0_H3T   = 0.;
Float_t X0_H4B   = 0.;
Float_t X0_H4T   = 0.;
Float_t X0_H1L   = 39.37;
Float_t X0_H1R   = -39.37;
Float_t X0_H2L   = 66.;
Float_t X0_H2R   = -66.;
Float_t X0_H4Y1L = 76.2;
Float_t X0_H4Y1R = -76.2;
Float_t X0_H4Y2L = 76.2;
Float_t X0_H4Y2R = -76.2;

Float_t Y0_H1B   = -34.925;
Float_t Y0_H1T   = 34.925;
Float_t Y0_H2B   = -76.;
Float_t Y0_H2T   = 76.;
Float_t Y0_H3B   = -83.82;
Float_t Y0_H3T   = 83.82;
Float_t Y0_H4B   = -91.44;
Float_t Y0_H4T   = 91.44;
Float_t Y0_H1L   = 0.;
Float_t Y0_H1R   = 0.;
Float_t Y0_H2L   = 0.;
Float_t Y0_H2R   = 0.;
Float_t Y0_H4Y1L = 0.;
Float_t Y0_H4Y1R = 0.;
Float_t Y0_H4Y2L = 0.;
Float_t Y0_H4Y2R = 0.;

Float_t Z_H1B   = 644.;
Float_t Z_H1T   = 644.;
Float_t Z_H2B   = 1420.95;
Float_t Z_H2T   = 1421.28;
Float_t Z_H3B   = 1958.34;
Float_t Z_H3T   = 1958.9;
Float_t Z_H4B   = 2251.71;
Float_t Z_H4T   = 2234.29;
Float_t Z_H1L   = 628.;
Float_t Z_H1R   = 628.;
Float_t Z_H2L   = 1405.09;
Float_t Z_H2R   = 1404.78;
Float_t Z_H4Y1L = 2130.27;
Float_t Z_H4Y1R = 2146.45;
Float_t Z_H4Y2L = 2200.44;
Float_t Z_H4Y2R = 2216.62;

Float_t x_H1T, y_H1T;
Float_t x_H1B, y_H1B;
Float_t x_H1L, y_H1L;
Float_t x_H1R, y_H1R;
Float_t x_H2T, y_H2T;
Float_t x_H2B, y_H2B;
Float_t x_H2L, y_H2L;
Float_t x_H2R, y_H2R;
Float_t x_H3T, y_H3T;
Float_t x_H3B, y_H3B;
Float_t x_H4T, y_H4T;
Float_t x_H4B, y_H4B;
Float_t x_H4Y1L, y_H4Y1R;
Float_t x_H4Y2L, y_H4Y2R;
*/
