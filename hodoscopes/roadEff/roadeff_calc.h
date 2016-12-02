#ifndef __ROADEFF_CALC__
#define __ROADEFF_CALC__

char Fname[128];
char inName[128];
char outName[128];
const int nroadID_chg0_det0 = 248;
const int nroadID_chg0_det1 = 240;
const int nroadID_chg1_det0 = 247;
const int nroadID_chg1_det1 = 242;

int roadID_chg0_det0[nroadID_chg0_det0], h1_chg0_det0[nroadID_chg0_det0],  h2_chg0_det0[nroadID_chg0_det0], h3_chg0_det0[nroadID_chg0_det0], h4_chg0_det0[nroadID_chg0_det0];
int roadID_chg0_det1[nroadID_chg0_det0], h1_chg0_det1[nroadID_chg0_det0],  h2_chg0_det1[nroadID_chg0_det0], h3_chg0_det1[nroadID_chg0_det0], h4_chg0_det1[nroadID_chg0_det0];
int roadID_chg1_det0[nroadID_chg0_det0], h1_chg1_det0[nroadID_chg0_det0],  h2_chg1_det0[nroadID_chg0_det0], h3_chg1_det0[nroadID_chg0_det0], h4_chg1_det0[nroadID_chg0_det0];
int roadID_chg1_det1[nroadID_chg0_det0], h1_chg1_det1[nroadID_chg0_det0],  h2_chg1_det1[nroadID_chg0_det0], h3_chg1_det1[nroadID_chg0_det0], h4_chg1_det1[nroadID_chg0_det0];

int total_chg0_det0[nroadID_chg0_det0], accept_chg0_det0[nroadID_chg0_det0];
int total_chg0_det1[nroadID_chg0_det1], accept_chg0_det1[nroadID_chg0_det1];
int total_chg1_det0[nroadID_chg1_det0], accept_chg1_det0[nroadID_chg1_det0];
int total_chg1_det1[nroadID_chg1_det1], accept_chg1_det1[nroadID_chg1_det1];

double eff_chg0_det0[nroadID_chg0_det0];
double eff_chg0_det1[nroadID_chg0_det1];
double eff_chg1_det0[nroadID_chg1_det0];
double eff_chg1_det1[nroadID_chg1_det1];

ifstream inFile_chg0_det0;
ifstream inFile_chg0_det1;
ifstream inFile_chg1_det0;
ifstream inFile_chg1_det1;

ofstream outFile_chg0_det0;
ofstream outFile_chg0_det1;
ofstream outFile_chg1_det0;
ofstream outFile_chg1_det1;

//Evaluation tree structure
//road ID variables
int eventID;
int charge;
int matrix1flag;
int matrix1two;
int detHalf;
int roadflag;
int n; //number of planes
int hodoFlags[4];  //flag for elements IDs
int roadElements[4]; //[elementIDs]
double mom_exp;

//histograms
TH1I* hist_all_chg0_det0;
TH1I* hist_acc_chg0_det0;
TH1D* hist_eff_chg0_det0;
TGraphAsymmErrors* graph_eff_chg0_det0;
TH1I* hist_all_chg0_det1;
TH1I* hist_acc_chg0_det1;
TH1D* hist_eff_chg0_det1;
TGraphAsymmErrors* graph_eff_chg0_det1;
TH1I* hist_all_chg1_det0;
TH1I* hist_acc_chg1_det0;
TH1D* hist_eff_chg1_det0;
TGraphAsymmErrors* graph_eff_chg1_det0;
TH1I* hist_all_chg1_det1;
TH1I* hist_acc_chg1_det1;
TH1D* hist_eff_chg1_det1;
TGraphAsymmErrors* graph_eff_chg1_det1;
#endif  // __ROADEFF_CALC__
