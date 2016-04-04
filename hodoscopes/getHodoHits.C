//get the hodoscope positions
#include <TH1.h>
#include <getHodoHits.h>

void getHodoHits(const int roadset = 61)
{
  cout << roadset << endl;
  //general initialization stuff
  gStyle -> SetOptFit(1);
  cout<<"blah"<<endl;

  //get histos
  sprintf(Tname, "position of ktrack, H1x[TB], roadset %d", roadset);
  TH2D *H1x = new TH2D("H1x", Tname, 150, -75, 75, 150, -75, 75);
  sprintf(Tname, "position of ktrack, H1x[LR], roadset %d", roadset);
  TH2D *H1y = new TH2D("H1y", Tname, 150, -75, 75, 150, -75, 75);

  sprintf(Tname, "position of ktrack, H2x[TB], roadset %d", roadset);
  TH2D *H2x = new TH2D("H2x", Tname, 250, -125, 125, 300, -150, 150);
  sprintf(Tname, "position of ktrack, H2x[LR], roadset %d", roadset);
  TH2D *H2y = new TH2D("H2y", Tname, 250, -125, 125, 300, -150, 150);

  sprintf(Tname, "position of ktrack, H3x[TB], roadset %d", roadset);
  TH2D *H3x = new TH2D("H3x", Tname, 250, -125, 125, 300, -150, 150);
  
  sprintf(Tname, "position of ktrack, H4y1L, roadset %d", roadset);
  TH2D *H4y1L = new TH2D("H4y1L", Tname, 320, -160, 160, 400, -200, 200);
  sprintf(Tname, "position of ktrack, H4y1R, roadset %d", roadset);
  TH2D *H4y1R = new TH2D("H4y1R", Tname, 320, -160, 160, 400, -200, 200);
  sprintf(Tname, "position of ktrack, H4y2L, roadset %d", roadset);
  TH2D *H4y2L = new TH2D("H4y2L", Tname, 320, -160, 160, 400, -200, 200);
  sprintf(Tname, "position of ktrack, H4y2R, roadset %d", roadset);
  TH2D *H4y2R = new TH2D("H4y2R", Tname, 320, -160, 160, 400, -200, 200);

  sprintf(Tname, "position of ktrack, H4xT, roadset %d", roadset);
  TH2D *H4xT = new TH2D("H4xT", Tname, 320, -160, 160, 400, -200, 200);
  sprintf(Tname, "position of ktrack, H4xB, roadset %d", roadset);
  TH2D *H4xB = new TH2D("H4xB", Tname, 320, -160, 160, 400, -200, 200);
  
  //get the ktrack tree
  sprintf(Fname, "./nDST/cuts1489v1/Analysis_roadset%d_R005_V001_ktrack.root", roadset);
  if(roadset == 68)  sprintf(Fname, "./nDST/cuts1489v1/Analysis_roadset%d_R005_V001_ktrack.root", 67);
  if(roadset == 69)  sprintf(Fname, "./nDST/cuts1489v1/Analysis_roadset%d_R005_V001_ktrack.root", 67);
  
  TFile *inFile1 = new TFile(Fname);
  TTree *tracktree = (TTree*) inFile1 -> Get("ktrack");
   
  tracktree -> SetBranchAddress ("trackID",   &trackID);
  tracktree -> SetBranchAddress ("runID",     &runID);
  tracktree -> SetBranchAddress ("spillID",   &spillID);
  tracktree -> SetBranchAddress ("eventID",   &eventID);
  tracktree -> SetBranchAddress ("numHits",   &numHits);
  tracktree -> SetBranchAddress ("charge",    &charge);
  tracktree -> SetBranchAddress ("roadID",    &roadID);
  tracktree -> SetBranchAddress ("chisq",     &chisq);
  tracktree -> SetBranchAddress ("x0",        &x0);
  tracktree -> SetBranchAddress ("y0",        &y0);
  tracktree -> SetBranchAddress ("z0",        &z0);
  tracktree -> SetBranchAddress ("x_target",  &x_target);
  tracktree -> SetBranchAddress ("y_target",  &y_target);
  tracktree -> SetBranchAddress ("z_target",  &z_target);
  tracktree -> SetBranchAddress ("x_dump",    &x_dump);
  tracktree -> SetBranchAddress ("y_dump",    &y_dump);
  tracktree -> SetBranchAddress ("z_dump",    &z_dump);
  tracktree -> SetBranchAddress ("x1",        &x1);
  tracktree -> SetBranchAddress ("y1",        &y1);
  tracktree -> SetBranchAddress ("z1",        &z1);
  tracktree -> SetBranchAddress ("px1",       &px1);
  tracktree -> SetBranchAddress ("py1",       &py1);
  tracktree -> SetBranchAddress ("pz1",       &pz1);
  tracktree -> SetBranchAddress ("x3",        &x3);
  tracktree -> SetBranchAddress ("y3",        &y3);
  tracktree -> SetBranchAddress ("z3",        &z3);
  tracktree -> SetBranchAddress ("px3",       &px3);
  tracktree -> SetBranchAddress ("py3",       &py3);
  tracktree -> SetBranchAddress ("pz3",       &pz3);
  tracktree -> SetBranchAddress ("tx_PT",     &tx_PT);
  tracktree -> SetBranchAddress ("ty_PT",     &ty_PT);
  tracktree -> SetBranchAddress ("targetPos", &targetPos);
  tracktree -> SetBranchAddress ("target",    &target);
  tracktree -> SetBranchAddress ("dump",    &dump);

  //begin getting hodo positions
  int nentries = tracktree -> GetEntries();
  for(int i = 0; i < nentries; i++){
    if(i%1000000 == 0) cout << i << endl;
    tracktree -> GetEntry(i);
    
    if(roadset == 68 && spillID<580000) continue;
    else if(roadset == 69 && spillID>580000) continue;
    else if(roadset == 70 && spillID<687000) continue;
    
    //get hodo1 positions
    xpos_H1x = px1/pz1 * (zpos_H1x - z1) + x1;
    ypos_H1x = py1/pz1 * (zpos_H1x - z1) + y1;
    xpos_H1y = px1/pz1 * (zpos_H1y - z1) + x1;
    ypos_H1y = py1/pz1 * (zpos_H1y - z1) + y1;
    
    //get hodo2 positions, careful with this one.
    xpos_H2x = x3 - px3/pz3 * (z3 - zpos_H2x);
    ypos_H2x = y3 - py3/pz3 * (z3 - zpos_H2x);
    xpos_H2y = x3 - px3/pz3 * (z3 - zpos_H2y);
    ypos_H2y = y3 - py3/pz3 * (z3 - zpos_H2y);

    //get hodo3 positions
    xpos_H3x = px3/pz3 * (zpos_H3x - z3) + x3;
    ypos_H3x = py3/pz3 * (zpos_H3x - z3) + y3;

    //get hodo4 positions
    xpos_H4y1L = px3/pz3 * (zpos_H4y1L - z3) + x3;
    ypos_H4y1L = py3/pz3 * (zpos_H4y1L - z3) + y3;
    xpos_H4y1R = px3/pz3 * (zpos_H4y1R - z3) + x3;
    ypos_H4y1R = py3/pz3 * (zpos_H4y1R - z3) + y3;

    xpos_H4y2L = px3/pz3 * (zpos_H4y2L - z3) + x3;
    ypos_H4y2L = py3/pz3 * (zpos_H4y2L - z3) + y3;
    xpos_H4y2R = px3/pz3 * (zpos_H4y2R - z3) + x3;
    ypos_H4y2R = py3/pz3 * (zpos_H4y2R - z3) + y3;

    xpos_H4xT = px3/pz3 * (zpos_H4xT - z3) + x3;
    ypos_H4xT = py3/pz3 * (zpos_H4xT - z3) + y3;
    xpos_H4xB = px3/pz3 * (zpos_H4xB - z3) + x3;
    ypos_H4xB = py3/pz3 * (zpos_H4xB - z3) + y3;

    H1x -> Fill(xpos_H1x, ypos_H1x);
    H1y -> Fill(xpos_H1y, ypos_H1y);
    H2x -> Fill(xpos_H2x, ypos_H2x);
    H2y -> Fill(xpos_H2y, ypos_H2y);
    H3x -> Fill(xpos_H3x, ypos_H3x);
    
    H4y1L -> Fill(xpos_H4y1L, ypos_H4y1L);
    H4y1R -> Fill(xpos_H4y1R, ypos_H4y1R);
    H4y2L -> Fill(xpos_H4y2L, ypos_H4y2L);
    H4y2R -> Fill(xpos_H4y2R, ypos_H4y2R);
    
    H4xT -> Fill(xpos_H4xT, ypos_H4xT);
    H4xB -> Fill(xpos_H4xB, ypos_H4xB);
    
  }
  cout<<"blah"<<endl;
  TCanvas *c12 = new TCanvas("c12", "c12", 1200, 800);
  c12 -> Divide(2, 2);
  c12 -> cd(1);
  H1x -> Draw("colz");
  c12 -> cd(2);
  H1y -> Draw("colz");
  c12 -> cd(3);
  H2x -> Draw("colz");
  c12 -> cd(4);
  H2y -> Draw("colz");

  TCanvas *c3 = new TCanvas("c3", "c3", 600, 400);
  c3 -> cd(1);
  H3x -> Draw("colz");

  TCanvas *c4 = new TCanvas("c4", "c4", 1200, 1200);
  c4 -> Divide(2, 3);
  c4 -> cd(1);
  H4y1L -> Draw("colz");
  c4 -> cd(2);
  H4y1R -> Draw("colz");
  c4 -> cd(3);
  H4y2L -> Draw("colz");
  c4 -> cd(4);
  H4y2R -> Draw("colz");
  c4 -> cd(5);
  H4xT -> Draw("colz");
  c4 -> cd(6);
  H4xB -> Draw("colz");
}
*/
