#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>

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
#include <TH2I.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TClonesArray.h>
#include <TVector3.h>
#include <TArrow.h>
#include <TPaveText.h>
#include <TSystem.h>

#include "getHodoHits.h"

using namespace std;

int main(int argc, char *argv[])
{
  //set search type

  if (!strcmp(argv[1], "-target"))
    target = 1;
  if (!strcmp(argv[1], "-dump"))
    target = 0;
  
  //get the file
  /*
  sprintf(Fname, "./pythiabkg/E906beamProf_ignoreWARN_100M_0.root");
  //sprintf(Fname, "./nDST/longerkaondecay_50M_0.root");
  TFile *inFile = new TFile(Fname);
  TTree *dataTree = (TTree*) inFile -> Get("save");
  */
  
  //get the chain
  //tchain method
  TChain *dataTree = new TChain("save");
  int chainfirst = 0;
  int chainlast = 200;

  for(int i = chainfirst; i <= chainlast; i++){
    sprintf(Fname, "./pythiabkg/E906beamProf_ignoreWARN_100M_%d.root", i);
    //cout << "Getting File " << Fname << endl;
    dataTree -> Add(Fname);
  }
  
  //make the histos
  for(int i = 0; i < nTrigs; ++i){
    for(int k = 0; k < nHodos; ++k){
      sprintf(Hname, "nHits_%s_hodo%d",  hodoTrigs[i].c_str(), k);
      sprintf(Tname, "raw%s hits on %s", hodoTrigs[i].c_str(), hodoNames[k].c_str() );
      if(k<8)hodoHits[i][k] = new TH1I(Hname, Tname, nElements[k], X0_hodoPos[k], XF_hodoPos[k]);
      else hodoHits[i][k] = new TH1I(Hname, Tname, nElements[k], Y0_hodoPos[k], YF_hodoPos[k]);
    }
  }    

  TClonesArray* posArr = new TClonesArray("TVector3");
  TClonesArray* momArr = new TClonesArray("TVector3");
  dataTree -> SetBranchAddress ("n",        &n);
  dataTree -> SetBranchAddress ("zProd",    &zProd);
  dataTree -> SetBranchAddress ("pdg",      pdg);
  dataTree -> SetBranchAddress ("parentID", parentID);
  dataTree -> SetBranchAddress ("pos",      &posArr);
  dataTree -> SetBranchAddress ("mom",      &momArr);
  
  int nentries = dataTree -> GetEntries();
  cout << "The number of track Entries is " << nentries << endl;
  for(int ievent = 0; ievent < nentries; ievent++){
    dataTree -> GetEntry(ievent);
    //target math
    if(ievent%1000000 == 0)cout << "Events: " << ievent << ".  " << float(ievent)/nentries*100. << "% done. " << endl;
    if(target == 0 && zProd < 0.) continue;
    else if(target == 1 && zProd > 0.) continue;

    //set hodo hits to zero
    for(int k = 0; k < nHodos; ++k)nHits[k] = 0;
    for(int j = 0; j < n; j++){
      //get variables
      TVector3* p_pos = (TVector3*)posArr->At(j);
      TVector3* p_mom = (TVector3*)momArr->At(j);
      if(pdg[j] == 13){charge = -1.0;}
      if(pdg[j] == -13){charge = 1.0;}
      x_0 = p_pos->X(); y_0 = p_pos->Y(); z_0 = p_pos->Z();
      px_0 = p_mom->X(); py_0 = p_mom->Y(); pz_0 = p_mom->Z();

      //let's do the x, y, px kicks for target through fmag
      if(z_0 < 0.){
	x_target = px_0 / pz_0 * (0 - z_0);
	x_fmag = px_0 / pz_0 * ZF_FMAG;

	dpx_fmag = charge * 0.3 * FMAG * ZF_FMAG/100.;
	dx_fmag = dpx_fmag / pz_0 * ZF_FMAG / 2;//used for x-hodos

	y_target = py_0 / pz_0 * (0 - z_0);
	y_fmag = py_0 / pz_0 * ZF_FMAG;
      }
      else if( z_0 > 0. &&  z_0 < 500){
	x_target = 0.;
	x_fmag = px_0 / pz_0 * (ZF_FMAG - z_0);

	dpx_fmag = charge * 0.3 * FMAG * (ZF_FMAG - z_0)/100.;
	dx_fmag = dpx_fmag / pz_0 * (ZF_FMAG - z_0) / 2;//used for x-hodos 

	y_target = 0.;
	y_fmag = py_0 / pz_0 * (500. - z_0);
      }
      else continue;
      
      //lets get H1
      x_hodoPos[0][j] = x_0 + x_target + x_fmag + dx_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_hodoPos[0] - ZF_FMAG);
      x_hodoPos[1][j] = x_0 + x_target + x_fmag + dx_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_hodoPos[1] - ZF_FMAG);
      y_hodoPos[0][j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_hodoPos[0] - ZF_FMAG);
      y_hodoPos[1][j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_hodoPos[1] - ZF_FMAG);
      
      x_hodoPos[8][j] = x_0 + x_target + x_fmag +                        px_0 / pz_0 * (Z_hodoPos[8] - ZF_FMAG);
      x_hodoPos[9][j] = x_0 + x_target + x_fmag +                        px_0 / pz_0 * (Z_hodoPos[9] - ZF_FMAG);
      y_hodoPos[8][j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_hodoPos[8] - ZF_FMAG);
      y_hodoPos[9][j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_hodoPos[9] - ZF_FMAG);

      //lets get up to kmag
      x_kmag =  px_0     / pz_0 * (ZM_KMAG - ZF_FMAG);
      dx_kmag = dpx_fmag / pz_0 * (ZM_KMAG - ZF_FMAG);
      y_kmag = (py_0) / pz_0 * (ZM_KMAG - ZF_FMAG);
      dpx_kmag = charge*dpx_kmag;
      
      //les get H2
      x_hodoPos[2][j] = x_0 + x_target + x_fmag + dx_fmag + x_kmag + dx_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_hodoPos[2] - ZM_KMAG);
      x_hodoPos[3][j] = x_0 + x_target + x_fmag + dx_fmag + x_kmag + dx_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_hodoPos[3] - ZM_KMAG);
      y_hodoPos[2][j] = y_0 + y_target + y_fmag +           y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[2] - ZM_KMAG);
      y_hodoPos[3][j] = y_0 + y_target + y_fmag +           y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[3] - ZM_KMAG);
      
      x_hodoPos[10][j] = x_0 + x_target + x_fmag +          x_kmag +                                   px_0 / pz_0 * (Z_hodoPos[10] - ZM_KMAG);
      x_hodoPos[11][j] = x_0 + x_target + x_fmag +          x_kmag +                                   px_0 / pz_0 * (Z_hodoPos[11] - ZM_KMAG);
      y_hodoPos[10][j] = y_0 + y_target + y_fmag +          y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[10] - ZM_KMAG);
      y_hodoPos[11][j] = y_0 + y_target + y_fmag +          y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[11] - ZM_KMAG);

      //lets get H3
      x_hodoPos[4][j] = x_0 + x_target + x_fmag + dx_fmag + x_kmag + dx_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_hodoPos[4] - ZM_KMAG);
      x_hodoPos[5][j] = x_0 + x_target + x_fmag + dx_fmag + x_kmag + dx_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_hodoPos[5] - ZM_KMAG);
      y_hodoPos[4][j] = y_0 + y_target + y_fmag +           y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[4] - ZM_KMAG);
      y_hodoPos[5][j] = y_0 + y_target + y_fmag +           y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[5] - ZM_KMAG);

      //lets get H4
      x_hodoPos[6][j] = x_0 + x_target + x_fmag + dx_fmag + x_kmag + dx_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_hodoPos[6] - ZM_KMAG);
      x_hodoPos[7][j] = x_0 + x_target + x_fmag + dx_fmag + x_kmag + dx_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_hodoPos[7] - ZM_KMAG);
      y_hodoPos[6][j] = y_0 + y_target + y_fmag +           y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[6] - ZM_KMAG);
      y_hodoPos[7][j] = y_0 + y_target + y_fmag +           y_kmag +                                   py_0 / pz_0 * (Z_hodoPos[7] - ZM_KMAG);
      
      x_hodoPos[12][j] = x_0 + x_target + x_fmag +          x_kmag +                                   px_0 / pz_0 * (Z_hodoPos[12] - ZM_KMAG);
      x_hodoPos[13][j] = x_0 + x_target + x_fmag +          x_kmag +                                    px_0 / pz_0 * (Z_hodoPos[13] - ZM_KMAG);
      y_hodoPos[12][j] = y_0 + y_target + y_fmag +          y_kmag +                                    py_0 / pz_0 * (Z_hodoPos[12] - ZM_KMAG);
      y_hodoPos[13][j] = y_0 + y_target + y_fmag +          y_kmag +                                    py_0 / pz_0 * (Z_hodoPos[13] - ZM_KMAG);
      x_hodoPos[14][j] = x_0 + x_target + x_fmag +          x_kmag +                                    px_0 / pz_0 * (Z_hodoPos[14] - ZM_KMAG);
      x_hodoPos[15][j] = x_0 + x_target + x_fmag +          x_kmag +                                    px_0 / pz_0 * (Z_hodoPos[15] - ZM_KMAG);
      y_hodoPos[14][j] = y_0 + y_target + y_fmag +          y_kmag +                                    py_0 / pz_0 * (Z_hodoPos[14] - ZM_KMAG);
      y_hodoPos[15][j] = y_0 + y_target + y_fmag +          y_kmag +                                    py_0 / pz_0 * (Z_hodoPos[15] - ZM_KMAG);
      
      //do acceptance cuts and hit counts
      for(int k = 0; k < nHodos; ++k){
	if(x_hodoPos[k][j] > X0_hodoPos[k] && x_hodoPos[k][j] < XF_hodoPos[k] && y_hodoPos[k][j] > Y0_hodoPos[k] && y_hodoPos[k][j] < YF_hodoPos[k]) nHits[k]++;
	else{
	  x_hodoPos[k][j] = -99999.;
	  y_hodoPos[k][j] = -99999.;
	}
      }  
    }

    //do trigger hits
    for(int trig = 0; trig < nTrigs; trig++) Trig[trig] = 0;//reset trigger
    if(nHits[8]  > 0 || nHits[9]  > 0) Trig[0] = 1;//h1
    if(nHits[10] > 0 || nHits[11] > 0) Trig[1] = 1;//h2
    if(nHits[12] > 0 || nHits[13] > 0) Trig[2] = 1;//h3
    if(nHits[14] > 0 || nHits[15] > 0) Trig[3] = 1;//h4

    if(nHits[8]  > 0 && nHits[10] > 0 || nHits[9]  > 0 && nHits[11] > 0) Trig[4] = 1;//h12
    if(nHits[8]  > 0 && nHits[12] > 0 || nHits[9]  > 0 && nHits[13] > 0) Trig[5] = 1;//h13
    if(nHits[8]  > 0 && nHits[14] > 0 || nHits[9]  > 0 && nHits[15] > 0) Trig[6] = 1;//h14
    if(nHits[10] > 0 && nHits[12] > 0 || nHits[11] > 0 && nHits[13] > 0) Trig[7] = 1;//h23
    if(nHits[10] > 0 && nHits[14] > 0 || nHits[11] > 0 && nHits[15] > 0) Trig[8] = 1;//h24
    if(nHits[12] > 0 && nHits[14] > 0 || nHits[13] > 0 && nHits[15] > 0) Trig[9] = 1;//h34

    if(nHits[8]  > 0 && nHits[10] > 0 && nHits[12] > 0 || nHits[9]  > 0 && nHits[11] > 0 && nHits[13] > 0) Trig[10] = 1;//h123
    if(nHits[8]  > 0 && nHits[10] > 0 && nHits[14] > 0 || nHits[9]  > 0 && nHits[11] > 0 && nHits[15] > 0) Trig[11] = 1;//h124
    if(nHits[10] > 0 && nHits[12] > 0 && nHits[14] > 0 || nHits[11] > 0 && nHits[13] > 0 && nHits[15] > 0) Trig[12] = 1;//h234

    if(nHits[8]  > 0 && nHits[10] > 0 && nHits[12] > 0 && nHits[14] > 0 || nHits[9]  > 0 && nHits[11] > 0 && nHits[13] > 0 && nHits[15] > 0) Trig[13] = 1;//h1234, nim1
    if(nHits[0]  > 0 && nHits[2] > 0 && nHits[4] > 0 && nHits[6] > 0 || nHits[1]  > 0 && nHits[3] > 0 && nHits[5] > 0 && nHits[7] > 0) Trig[14] = 1;//h1234, nim2
    Trig[15] = 1;//all hits

    //fill the histos
    for(int i = 0; i < nTrigs; ++i)
      for(int k = 0; k < nHodos; ++k)
	for(int j = 0; j < n; j++){
	  if(x_hodoPos[k][j] < -1000.) continue;
	  if(Trig[i] == 1){
	    if(k<8) hodoHits[i][k] -> Fill(x_hodoPos[k][j]);
	    else hodoHits[i][k] -> Fill(y_hodoPos[k][j]);
	  }
	}
  }

  //draw plots
  TCanvas *c4m[nTrigs];
  for(int i = 0; i < nTrigs; ++i){
    sprintf(Hname, "c4matrix%d", i+1);
    c4m[i] = new TCanvas(Hname, Hname, 2000, 1000);
    c4m[i] -> Divide(4, 2);
    //xhodos
    for(int k = 0; k < 8; ++k){
      c4m[i] -> cd(k+1);
      hodoHits[i][k] -> Draw();

    }
    sprintf(Hname, "./images/XhodoHits_%s.gif", hodoTrigs[i].c_str());
    c4m[i] -> SaveAs(Hname);

    //yhodos
    for(int k = 0; k < 8; ++k){
      c4m[i] -> cd(k+1);
      hodoHits[i][k+8] -> Draw();

    }
    sprintf(Hname, "./images/YhodoHits_%s.gif", hodoTrigs[i].c_str());
    c4m[i] -> SaveAs(Hname);
  }
  
}

//boneyard
/*
      //x_H1B[j] = x_0 + x_target + dx_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1B - ZF_FMAG);
      //x_H1T[j] = x_0 + x_target + dx_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1T - ZF_FMAG);
      //y_H1B[j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_H1B - ZF_FMAG);
      //y_H1T[j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_H1T - ZF_FMAG);
      //x_H1L[j] = x_0 + x_target + x_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1L - ZF_FMAG);
      //x_H1R[j] = x_0 + x_target + x_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1R - ZF_FMAG);
      //y_H1L[j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_H1L - ZF_FMAG);
      //y_H1R[j] = y_0 + y_target + y_fmag +                        py_0 / pz_0 * (Z_H1R - ZF_FMAG);
      //x_H2B[j] = x_0 + x_target + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2B - ZM_KMAG);
      //x_H2T[j] = x_0 + x_target + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2T - ZM_KMAG);
      //y_H2B[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H2B - ZM_KMAG);
      //y_H2T[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H2T - ZM_KMAG);
      //x_H2L[j] = x_0 + x_target + x_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2L - ZM_KMAG);
      //x_H2R[j] = x_0 + x_target + x_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2R - ZM_KMAG);
      //y_H2L[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H2L - ZM_KMAG);
      //y_H2R[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H2R - ZM_KMAG);
      //x_H3B[j] = x_0 + x_target + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H3B - ZM_KMAG);
      //x_H3T[j] = x_0 + x_target + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H3T - ZM_KMAG);
      //y_H3B[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H3B - ZM_KMAG);
      //y_H3T[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H3T - ZM_KMAG);
      //x_H4B[j] = x_0 + x_target + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4B - ZM_KMAG);
      //x_H4T[j] = x_0 + x_target + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4T - ZM_KMAG);
      //y_H4B[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H4B - ZM_KMAG);
      //y_H4T[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H4T - ZM_KMAG);
      //x_H4Y1L[j] = x_0 + x_target + x_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y1L - ZM_KMAG);
      //x_H4Y1R[j] = x_0 + x_target + x_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y1R - ZM_KMAG);
      //y_H4Y1L[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H4Y1L - ZM_KMAG);
      //y_H4Y1R[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H4Y1R - ZM_KMAG);
      //x_H4Y2L[j] = x_0 + x_target + x_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y2L - ZM_KMAG);
      //x_H4Y2R[j] = x_0 + x_target + x_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y2R - ZM_KMAG);
      //y_H4Y2L[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H4Y2L - ZM_KMAG);
      //y_H4Y2R[j] = y_0 + y_target + y_fmag +           y_kmag +                         py_0 / pz_0 * (Z_H4Y2R - ZM_KMAG);
      
      if(dx_H1B[j] > -L0_H1B/2 && dx_H1B[j] < L0_H1B/2 && dy_H1B[j] > Y0_H1B && dy_H1B[j] < 0.) nH1B++; else dx_H1B[j] = -99999.;
      if(dx_H2B[j] > -L0_H2B/2 && dx_H2B[j] < L0_H2B/2 && dy_H2B[j] > Y0_H2B[j] && dy_H2B[j] < 0.) nH2B++; else dx_H2B[j] = -99999.;
      if(dx_H3B[j] > -L0_H3B/2 && dx_H3B[j] < L0_H3B/2 && dy_H3B[j] > Y0_H3B[j] && dy_H3B[j] < 0.) nH3B++; else dx_H3B[j] = -99999.;
      if(dx_H1B[j] > -L0_H4B/2 && dx_H4B[j] < L0_H4B/2 && dy_H4B[j] > Y0_H4B[j] && dy_H4B[j] < 0.) nH4B++; else dx_H4B[j] = -99999.;
      if(dx_H1T[j] > -L0_H1T/2 && dx_H1T[j] < L0_H1T/2 && dy_H1T[j] < Y0_H1T[j] && dy_H1T[j] > 0.) nH1T++; else dx_H1T[j] = -99999.;
      if(dx_H2T[j] > -L0_H2T/2 && dx_H2T[j] < L0_H2T/2 && dy_H2T[j] < Y0_H2T[j] && dy_H2T[j] > 0.) nH2T++; else dx_H2T[j] = -99999.;
      if(dx_H3T[j] > -L0_H3T/2 && dx_H3T[j] < L0_H3T/2 && dy_H3T[j] < Y0_H3T[j] && dy_H3T[j] > 0.) nH3T++; else dx_H3T[j] = -99999.;
      if(dx_H1T[j] > -L0_H4T/2 && dx_H4T[j] < L0_H4T/2 && dy_H4T[j] < Y0_H4T[j] && dy_H4T[j] > 0.) nH4T++; else dx_H4T[j] = -99999.;	    

      if(dy_H1L[j] > -L0_H1L/2 && dy_H1L[j] < L0_H1L/2 && dx_H1L[j] < Y0_H1L && dy_H1L[j] > 0.) nH1L++; else dx_H1L[j] = -99999.;
      if(dy_H2L[j] > -L0_H2L/2 && dy_H2L[j] < L0_H2L/2 && dx_H2L[j] < Y0_H2L && dy_H2L[j] > 0.) nH2L++; else dx_H2L[j] = -99999.;
      if(dy_H4Y1L[j] > -L0_H4Y1L/2 && dy_H4Y1L[j] < L0_H4Y1L/2 && dx_H4Y1L[j] < Y0_H4Y1L && dy_H4Y1L[j] > 0.) nH4Y1L++; else dy_H4Y1L[j] = -99999.;
      if(dy_H4Y2L[j] > -L0_H4Y2L/2 && dy_H4Y2L[j] < L0_H4Y2L/2 && dx_H4Y2L[j] < Y0_H4Y2L && dy_H4Y2L[j] > 0.) nH4Y2L++; else dy_H4Y2L[j] = -99999.;

      if(dy_H1R[j] > -R0_H1R/2 && dy_H1R[j] < R0_H1R/2 && dx_H1R[j] > Y0_H1R && dy_H1R[j] < 0.) nH1R++; else dx_H1R[j] = -99999.;
      if(dy_H2R[j] > -R0_H2R/2 && dy_H2R[j] < R0_H2R/2 && dx_H2R[j] > Y0_H2R && dy_H2R[j] < 0.) nH2R++; else dx_H2R[j] = -99999.;
      if(dy_H4Y1R[j] > -R0_H4Y1R/2 && dy_H4Y1R[j] < R0_H4Y1R/2 && dx_H4Y1R[j] > Y0_H4Y1R && dy_H4Y1R[j] < 0.) nH4Y1R++; else dy_H4Y1R[j] = -99999.;
      if(dy_H4Y2R[j] > -R0_H4Y2R/2 && dy_H4Y2R[j] < R0_H4Y2R/2 && dx_H4Y2R[j] > Y0_H4Y2R && dy_H4Y2R[j] < 0.) nH4Y2R++; else dy_H4Y2R[j] = -99999.;

*/
