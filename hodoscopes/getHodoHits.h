#ifndef GETHODOHITS_H__
#define GETHODOHITS_H__

//general stuff
const Float_t PI = 3.14159;
char Fname[128];
char Hname[128];
char Tname[128];

//initialize ktrack tree
Int_t trackID, runID, spillID, eventID, charge, roadID;
Int_t numHits, numHitsSt1, numHitsSt2, numHitsSt3, numHitsSt4H, numHitsSt4V;
Float_t chisq;
Float_t x0, y0, z0;
Float_t x_target, y_target, z_target;
Float_t x_dump, y_dump, z_dump;
Float_t px0, py0, pz0;
Float_t x1, y1, z1;
Float_t px1, py1, pz1;
Float_t x3, y3, z3;
Float_t px3, py3, pz3;
Float_t tx_PT, ty_PT;
Int_t target, dump;
Int_t targetPos;

//declare the hodo z-position constants
Float_t zpos_H1x = 663;
Float_t zpos_H1y = 653;

Float_t zpos_H2x = 1421.13;
Float_t zpos_H2y = 1403.35;

Float_t zpos_H3x = 1958.45;

Float_t zpos_H4y1L = 2129.79;
Float_t zpos_H4y1R = 2146.79;

Float_t zpos_H4y2L = 2199.64;
Float_t zpos_H4y2R = 2216.78;

Float_t zpos_H4xT = 2240.28;
Float_t zpos_H4xB = 2240.28;

//declare the hodo x-position variables;
Float_t xpos_H1x;
Float_t xpos_H1y;

Float_t xpos_H2x;
Float_t xpos_H2y;

Float_t xpos_H3x;

Float_t xpos_H4y1L;
Float_t xpos_H4y1R;

Float_t xpos_H4y2L;
Float_t xpos_H4y2R;

Float_t xpos_H4xT;
Float_t xpos_H4xB;

//declare the hodo y-position variables;
Float_t ypos_H1x;
Float_t ypos_H1y;

Float_t ypos_H2x;
Float_t ypos_H2y;

Float_t ypos_H3x;

Float_t ypos_H4y1L;
Float_t ypos_H4y1R;

Float_t ypos_H4y2L;
Float_t ypos_H4y2R;

Float_t ypos_H4xT;
Float_t ypos_H4xB;

//declare the hodo channels;
Int_t chan_H1x;
Int_t chan_H1y;

Int_t chan_H2x;
Int_t chan_H2y;

Int_t chan_H3x;

Int_t chan_H4y1L;
Int_t chan_H4y1R;

Int_t chan_H4Y2L;
Int_t chan_H4Y2R;

Int_t chan_H4xT;
Int_t chan_H4xB;

#endif // GETHODOHITS_H_
