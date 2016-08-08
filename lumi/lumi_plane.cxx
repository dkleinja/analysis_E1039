#include <iostream>
#include <stdlib.h>

#include <TMath.h>
#include <TVector3.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>

using namespace std;

class Plane
{
public:
	void intercept(TVector3 l0, TVector3 l, double& d, TVector3& l1);  //l0 is the origin point, l is the direction unit vector, l1 is the final point

public:
	TVector3 s0;    //center of the plane
	TVector3 n;     //normal vector of the plane
};

void Plane::intercept(TVector3 l0, TVector3 l, double& d, TVector3& l1)
{
  d = (s0 - l0)*n/(l*n);
  l1 = l0 + (d*l);

  //couts for debugging purposes
  /*
  Float_t mag = l.X()*l.X() + l.Y()*l.Y() + l.Z()*l.Z()*10;
  cout << "s_0 is (" << s0.X() <<", " << s0.Y() << ", " << s0.Z() << ");" << endl;
  cout << "l_0 is (" << l0.X() <<", " << l0.Y() << ", " << l0.Z() << ");" << endl;
  cout << "l   is (" << l.X() <<", " << l.Y() << ", " << l.Z() << ");" << endl;
  cout <<"||l|| is " << l.Mag() << endl;
  cout << " d is " << d << endl;
  cout << "l_1   is (" << l1.X() <<", " << l1.Y() << ", " << l1.Z() << ");" << endl;
  */
}

const double WIDTH = 2.54/2.;
const double DISTANCE = 270.;

bool accepted(Plane& p1, Plane& p2, TVector3 pos, TVector3 mom, int index)
{
  mom = mom*(1./mom.Mag());// get the momentum unit vector

  double d1, d2;
  TVector3 proj1, proj2;
  p1.intercept(pos, mom, d1, proj1);
  p2.intercept(pos, mom, d2, proj2);

  //cout << d1 << "  " << d2 << endl;
  //cout << proj1.X() << "  " << proj1.Y() << "  " << proj1.Z() << endl;
  //cout << proj2.X() << "  " << proj2.Y() << "  " << proj2.Z() << endl;

  //two fold coincidence
  
  if(d1 < 0 || d2 < 0) return false; //particle going in opposite direction
  if(d1 > 1E4 || d2 > 1E4) return false; //particle nearly parallel to scintillator
  if(fabs(proj1.Z() - p1.s0.Z()) > WIDTH || fabs(proj2.Z() - p2.s0.Z()) > WIDTH) return false;
  if(fabs(proj1[index] - p1.s0[index]) > WIDTH || fabs(proj2[index] - p2.s0[index]) > WIDTH) return false;
  /*
  //just one coincidence
  if(d1 < 0) return false;
  if(d1 > 1E4) return false;
  if(fabs(proj1.Z() - p1.s0.Z()) > WIDTH) return false;
  if(fabs(proj1[index] - p1.s0[index]) > WIDTH) return false;
  */
  return true;
}

bool isMip(TVector3 mom, int pdg)
{
  //if(pdg == 2212 && mom.Mag() < 0.120) return false;
  if(abs(pdg) == 211 && (sqrt(mom.Mag2() + 0.139*0.139) - 0.139) < 0.1) return false;
  else if(abs(pdg) == 13 && (sqrt(mom.Mag2() + 0.205*0.205) - 0.205) < 0.1) return false;
  else if(pdg == 22 && mom.Mag() < .1) return false;
    //if(if(G4UniformRand() > TMath::Exp(-0.003/1.2))
  
  return true;
}

int main(int argc, char* argv[])
//void lumi_plane()
{
 
  int xdrift = strtol(argv[1], NULL, 10);
  int ydrift = strtol(argv[2], NULL, 10);
  cout << xdrift << " " << ydrift << endl;

  //histograms
  //four detectors
  TH1I *Hleft = new TH1I("Hleft", "Hleft", 1, 0, 1);
  TH1I *Hright = new TH1I("Hright", "Hright", 1, 0, 1);
  TH1I *Hup = new TH1I("Hup", "Hup", 1, 0, 1);
  TH1I *Hdown = new TH1I("Hdown", "Hdown", 1, 0, 1);

  //kinetic energies
  TH1F *Henergy0 = new TH1F("Henergy0","Henergy0", 100, 0, 1);
  TH1F *Henergy1 = new TH1F("Henergy1","Henergy1", 100, 0, 1);

  //left detectors
  Plane pl0, pl1;
  pl0.s0.SetXYZ(DISTANCE,     0., -330.); pl0.n.SetXYZ(1, 0, 0);
  pl1.s0.SetXYZ(DISTANCE+10., 0., -330.); pl1.n.SetXYZ(1, 0, 0);
  //pl2.s0.SetXYZ(220., 0., -330.); pl2.n.SetXYZ(1, 0, 0);
  //pl3.s0.SetXYZ(230., 0., -330.); pl3.n.SetXYZ(1, 0, 0);

  //right detectors
  Plane pr0, pr1;
  pr0.s0.SetXYZ(-1*DISTANCE,    0., -330.); pr0.n.SetXYZ(1, 0, 0);
  pr1.s0.SetXYZ(-1*DISTANCE-10., 0., -330.); pr1.n.SetXYZ(1, 0, 0);
  
  //top detectors
  Plane pt0, pt1;
  pt0.s0.SetXYZ(0., DISTANCE,     -330.); pt0.n.SetXYZ(0, 1, 0);
  pt1.s0.SetXYZ(0., DISTANCE+10., -330.); pt1.n.SetXYZ(0, 1, 0);

  //bottom detectors
  Plane pb0, pb1;
  pb0.s0.SetXYZ(0., -1*DISTANCE,     -330.); pb0.n.SetXYZ(0, 1, 0);
  pb1.s0.SetXYZ(0., -1*DISTANCE-10., -330.); pb1.n.SetXYZ(0, 1, 0);

  int n;
  int pdg[1000];
  TClonesArray* posArr = new TClonesArray("TVector3");
  TClonesArray* momArr = new TClonesArray("TVector3");
  
  //TFile* dataFile = new TFile("./nDST/test_lumi_x0y0_50M_0.root", "READ");
  //TFile* dataFile = new TFile(argv[1], "READ");
  //TTree* dataTree = (TTree*)dataFile->Get("save");
  char Fname[128];
  TChain *dataTree = new TChain("save");
  int nchain = 1;
  for(int i = 0; i < nchain; i++){
    sprintf(Fname, "./nDST/lumi_x%dy%d_50M_%d.root", xdrift, ydrift, i);
    cout << "Getting File " << Fname << endl;
    dataTree -> Add(Fname);
  }
  
  dataTree->SetBranchAddress("n", &n);
  dataTree->SetBranchAddress("pdg", pdg);
  dataTree->SetBranchAddress("pos", &posArr);
  dataTree->SetBranchAddress("mom", &momArr);
  
  int nl, nr, nt, nb;
  nl = 0; nr = 0; nt = 0; nb = 0;
  for(int i = 0; i < dataTree->GetEntries(); ++i)
    {
      if(i%1000000 == 0)cout << "Entry " << i << endl;
      dataTree->GetEntry(i);
      for(int j = 0; j < n; ++j)
        {
	  TVector3* p_pos = (TVector3*)posArr->At(j);
	  TVector3* p_mom = (TVector3*)momArr->At(j);

	  //no protons or neutrons
	  //if(pdg[j] > 2000)continue;

	  //kinetic energy cut	  
	  if(!isMip(*p_mom, pdg[j])) continue;

	  //if(abs(p_mom->Y()/p_mom->X()) > 0.176 && abs(p_mom->Y()/p_mom->X()) < 5.67)continue;//#phi = #pm 10 deg at 0, 90, 180, 360
	  //if(abs(p_mom->CosTheta()) < 0.087)//#theta = #pm 10 deg

	  if(accepted(pl0, pl1, *p_pos, *p_mom, 1)){ ++nl; Hleft -> Fill(0);}
	  if(accepted(pr0, pr1, *p_pos, *p_mom, 1)){ ++nr; Hright -> Fill(0);}
	  if(accepted(pt0, pt1, *p_pos, *p_mom, 0)){ ++nt; Hup -> Fill(0);}
	  if(accepted(pb0, pb1, *p_pos, *p_mom, 0)){ ++nb; Hdown -> Fill(0);}
	  
	  //if(accepted(pl0, pl1, *p_pos, *p_mom, 1)) cout << 1 << " " << i << "  " << j << "  " << pdg[j] << "  " << p_mom->X() << "  " << p_mom->Y() << "  " << p_mom->Z() << endl;
	  //if(accepted(pr0, pr1, *p_pos, *p_mom, 1)) cout << 2 << " " << i << "  " << j << "  " << pdg[j] << "  " << p_mom->X() << "  " << p_mom->Y() << "  " << p_mom->Z() << endl;
	  //if(accepted(pt0, pt1, *p_pos, *p_mom, 0)) cout << 3 << " " << i << "  " << j << "  " << pdg[j] << "  " << p_mom->X() << "  " << p_mom->Y() << "  " << p_mom->Z() << endl;
	  //if(accepted(pb0, pb1, *p_pos, *p_mom, 0)) cout << 4 << " " << i << "  " << j << "  " << pdg[j] << "  " << p_mom->X() << "  " << p_mom->Y() << "  " << p_mom->Z() << endl;
	  
        }
    }
  
  cout << nl << "  " << nr << "  " << nt << "   " << nb << endl;

  sprintf(Fname, "./root_files/lumi_x%dy%d_50Mx%d.root", xdrift, ydrift, nchain);
  TFile *outFile = new TFile(Fname, "RECREATE");
  outFile -> cd();
  Hleft -> Write();
  Hright -> Write();
  Hup -> Write();
  Hdown -> Write();
  
  return 0;
}
