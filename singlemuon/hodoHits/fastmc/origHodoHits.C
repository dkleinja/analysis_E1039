void drawHodos(int target = 2)
{

  //get the file
  char Fname[128];
  sprintf(Fname, "./beamProfile/output.root");
  sprintf(Fname, "./beamProfile/e906beamProf_100M_1.root");
  //sprintf(Fname, "./nDST/longerkaondecay_50M_0.root");
  TFile *inFile = new TFile(Fname);

  //make the histos
  TH1I *HodoH1B = new TH1F("HodoH1B", "Hits On H1B", 23, -L0_H1B/2., L0_H1B/2.);
  TH1I *HodoH1T = new TH1F("HodoH1T", "Hits On H1T", 23, -L0_H1T/2., L0_H1T/2.);

  Int_t all, pions, kaons, others;
  Int_t n;
  Int_t pdg[1000];
  Int_t parentID[1000];
  Float_t zProd;

  TClonesArray* posArr = new TClonesArray("TVector3");
  TClonesArray* momArr = new TClonesArray("TVector3");
  
  TTree *tracktree = (TTree*) inFile -> Get("save");
  tracktree -> SetBranchAddress ("n",        &n);
  tracktree -> SetBranchAddress ("zProd",    &zProd);
  tracktree -> SetBranchAddress ("pdg",      pdg);
  tracktree -> SetBranchAddress ("parentID", parentID);
  tracktree -> SetBranchAddress ("pos",      &posArr);
  tracktree -> SetBranchAddress ("mom",      &momArr);

  int nentries = tracktree -> GetEntries();
  cout << "The number of track Entries is " << nentries << endl;
  for(int i = 0; i < nentries; i++){
    tracktree -> GetEntry(i);
    nH1B = 0; nH2B = 0; nH3B = 0; nH4B = 0;
    //target math
    if(target == 0 && zProd < 0.) continue;
    else if(target == 1 && zProd > 0.) continue;
    
    for(int j = 0; j < n; j++){
      //get variables
      TVector3* p_pos = (TVector3*)posArr->At(j);
      TVector3* p_mom = (TVector3*)momArr->At(j);
      if(pdg == 13){charge = -1.0;}
      if(pdg == -13){charge = 1.0;}
      x_0 = p_pos->X(); y_0 = p_pos->Y(); z_0 = p_pos->Z();
      px_0 = p_mom->X(); py_0 = p_mom->Y(); pz_0 = p_mom->Z();

      //ok, let's do the x, y, px kicks for target through fmag
      dx_target = 0.; dy_target = 0.;
      if(z_0 < 0.){
	dpx_fmag = 0.3*FMAG*ZF_FMAG/100.;
	x_target = px_0 / pz_0 * (0 - z_0);
	x_fmag = (px_0 + dpx_fmag) / pz_0 * ZF_FMAG;
	y_target = py_0 / pz_0 * (0 - z_0);
	y_fmag = py_0 / pz_0 * ZF_FMAG.;
      }
      else if( z_0 > 0. &&  z_0 < 500){
	dpx_fmag = 0.3*FMAG*(Z_FMAG - z_0)/100.;
	x_target = 0.;
	x_fmag = (px_0 + dpx_fmag) / pz_0 * (Z_FMAG - z_0) / 2;
	y_target = 0.;
	y_fmag = px_0 / pz_0 * (500. - z_0);
      }
      else continue;
      
      //lets get H1
      x_H1T[j] = x_0 + x_target + x_dump + x_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1T - Z_FMAG);
      x_H1B[j] = x_0 + x_target + x_dump + x_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1B - Z_FMAG);
      y_H1T[j] = y_0 + y_target + y_dump +                        py_0 / pz_0 * (Z_H1T - Z_FMAG);
      y_H1B[j] = y_0 + y_target + y_dump +                        py_0 / pz_0 * (Z_H1B - Z_FMAG);
      x_H1L[j] = x_0 + x_target + x_dump + x_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1L - Z_FMAG);
      x_H1R[j] = x_0 + x_target + x_dump + x_fmag + (px_0 + dpx_fmag) / pz_0 * (Z_H1R - Z_FMAG);
      y_H1L[j] = y_0 + y_target + y_dump +                        py_0 / pz_0 * (Z_H1L - Z_FMAG);
      y_H1R[j] = y_0 + y_target + y_dump +                        py_0 / pz_0 * (Z_H1R - Z_FMAG);

      //lets get up to kmag
      x_kmag = (px_0 + dpx_fmag) / pz_0 * (ZM_KMAG - ZF_FMAG);
      y_kmag = (py_0) / pz_0 * (ZM_KMAG - ZF_FMAG);

      //les get H2
      x_H2T[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2T - Z_KMAG);
      x_H2B[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2B - Z_KMAG);
      y_H2T[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H2T - Z_KMAG);
      y_H2B[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H2B - Z_KMAG);
      x_H2L[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2L - Z_KMAG);
      x_H2R[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H2R - Z_KMAG);
      y_H2L[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H2L - Z_KMAG);
      y_H2R[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H2R - Z_KMAG);

      //lets get H3
      x_H3T[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H3T - Z_KMAG);
      x_H3B[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H3B - Z_KMAG);
      y_H3T[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H3T - Z_KMAG);
      y_H3B[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H3B - Z_KMAG);

      //lets get H4
      x_H4T[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4T - Z_KMAG);
      x_H4B[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4B - Z_KMAG);
      y_H4T[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H4T - Z_KMAG);
      y_H4B[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H4B - Z_KMAG);
      x_H4Y1L[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y1L - Z_KMAG);
      x_H4Y1R[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y1R - Z_KMAG);
      y_H4Y1L[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H4Y1L - Z_KMAG);
      y_H4Y1R[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H4Y1R - Z_KMAG);
      x_H4Y2L[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y2L - Z_KMAG);
      x_H4Y2R[j] = x_0 + x_target + x_dump + dx_fmag + x_kmag + (px_0 + dpx_fmag + dpx_kmag) / pz_0 * (Z_H4Y2R - Z_KMAG);
      y_H4Y2L[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H4Y2L - Z_KMAG);
      y_H4Y2R[j] = y_0 + y_target + y_dump +           y_kmag +                         py_0 / pz_0 * (Z_H4Y2R - Z_KMAG);

      //do acceptance cuts and hit counts
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

    }

    //do trigger hits
    nim0=0; nim1=0; nim2=0; nim3=0;
    h1=0; h2=0; h3=0; h4=0;
    h12=0; h13=0; h14=0; h23=0; h24=0;
    h123=0; h124=0; h234=0;
    if(nH1L > 0 || nNH1R > 0) h1 = 1;
    if(nH2L > 0 || nNH2R > 0) h2 = 1;
    if(nH4Y1L > 0 || nNH4Y1R > 0) h3 = 1;
    if(nH4Y1L > 0 || nNH4Y2R > 0) h4 = 1;
     
    if(nH1L   > 0 && nH2L   > 0 || nH1R   > 0 && nH2R     > 0) h12 = 1;
    if(nH1L   > 0 && nH4Y1L > 0 || nH1R   > 0 && nH4Y1R   > 0) h13 = 1;
    if(nH1L   > 0 && nH4Y2L > 0 || nH1R   > 0 && nH4Y2R   > 0) h14 = 1;
    if(nH2L   > 0 && nH4Y1L > 0 || nH2R   > 0 && nH4Y1R   > 0) h23 = 1;
    if(nH2L   > 0 && nH4Y2L > 0 || nH2R   > 0 && nH4Y2R   > 0) h24 = 1;
    if(nH4Y1L > 0 && nH4Y2L > 0 || nH4Y1R > 0 && nH4Y2R   > 0) h34 = 1;
    if(nH1L   > 0 && nH2L   > 0 && nH4Y1L > 0 || nH1R   > 0 && nH2R   > 0 && nH4Y1R > 0) h123 = 1;
    if(nH1L   > 0 && nH2L   > 0 && nH4Y2L > 0 || nH1R   > 0 && nH2R   > 0 && nH4Y2R > 0) h124 = 1;
    if(nH2L   > 0 && nH4Y1L > 0 && nH4Y2L > 0 || nH2R   > 0 && nH4Y1R > 0 && nH4Y2R > 0) h234 = 1;
    if(nH1L   > 0 && nH2L   > 0 && nH4Y1L > 0 && nH4Y2L > 0 || nH1R   > 0 && nH2R   > 0 && nH4Y1R > 0 && nH4Y2R > 0) nim1 = 1;

    
      deltay = (py0 / pz0) * ( -1*fabs(magpos - maglength/2.0) -  dz);
       
      ptkickx = -0.3 * charge * tesla;
      //cout << i << " charge, tesla, particleID, ptkick " << charge << " " << tesla <<  " " << ptkickx << endl;
      deltax0 = (px0 / pz0) * ( -1*magpos - dz);
      deltax1 = ((px0 + ptkickx) / pz0) * fabs(maglength/2.0);
       deltax = deltax0 + deltax1;
       deltar = sqrt ( pow(deltax,2) + pow(deltay,2));
       


       
      
      hx_all -> Fill(p_pos->X());
      hy_all -> Fill(p_pos->Y());
      hz_all -> Fill(p_pos->Z());
      all++;
      //get the particles
      if(abs(parentID[j]) == 211){
	pions++;
	hx_pions -> Fill(p_pos->X());
	hy_pions -> Fill(p_pos->Y());
	hz_pions -> Fill(p_pos->Z());
      }
      if(abs(parentID[j]) == 321 || parentID[j] == 130){
	kaons++;
	hx_kaons -> Fill(p_pos->X());
     	hy_kaons -> Fill(p_pos->Y());
	hz_kaons -> Fill(p_pos->Z());
      }
      if(abs(parentID[j]) != 211 && abs(parentID[j])!=321 && parentID[j] != 130){
	others++;
	hx_short -> Fill(p_pos->X());
	hy_short -> Fill(p_pos->Y());
	hz_short -> Fill(p_pos->Z());
      }
    }
  }

  TCanvas *c4x = new TCanvas("c4x", "c4x", 1200, 1000);
  c4x -> Divide(2, 2);
  c4x -> cd(1);
  hx_all -> Draw();
  c4x -> cd(2);
  hx_pions -> Draw();
  c4x -> cd(3);
  hx_kaons -> Draw();
  c4x -> cd(4);
  hx_short -> Draw();

  cout << "The total number of particles is " << all << " " << 1.*all/all*100 << "%" << endl;
  cout << "The total number of pions is " << pions << " " << 1.*pions/all*100. << "%" << endl;
  cout << "The total number of kaons is " << kaons << " " << 1.*kaons/all*100. << "%" << endl;
  cout << "The total number of ohters is " << others << " " << 1.*others/all*100. << "%" << endl;
}
