getTriggerRate()
{

  //Get the Files and triggerbit values
  char Fname[128];
  const Int_t nRates = 11;
  TFile *inFile[nRates];
  TTree *saveTree[nRates];
  Int_t rate[nRates] = {5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  Int_t nJobs[nRates] = {992, 987, 988, 982, 991, 978, 992, 992, 992, 982, 992};
  Int_t dpsimMatrix1[nRates];
  Int_t dpsimMatrix2[nRates];
  Int_t dpsimMatrix3[nRates];
  Int_t dpsimMatrix4[nRates];
  Int_t dpsimMatrix5[nRates];
  Int_t dpsimNim1[nRates];
  Int_t dpsimNim2[nRates];
  Int_t dpsimNim3[nRates];

  for(int i = 0; i < nRates; i++){
    sprintf(Fname, "./dpsim/%dK/output/MATRIXcheck_%dK_%dx50M.root", rate[i], rate[i], nJobs[i]);
    inFile[i] = new TFile(Fname);
    saveTree[i] = (TTree*)inFile[i] -> Get("save");
    dpsimMatrix1[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>0 & 1 != 0");
    dpsimMatrix2[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>1 & 1 != 0");
    dpsimMatrix3[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>2 & 1 != 0");
    dpsimMatrix4[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>3 & 1 != 0");
    /*dpsimMatrix5[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>4 & 1 != 0");
    dpsimNim1[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>5 & 1 != 0");
    dpsimNim2[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>6 & 1 != 0");
    dpsimNim3[i] = saveTree[i] -> GetEntries("fEvtHeader.fTriggerBit>>7 & 1 != 0");
    */
    cout << "The " << rate[i] << "000 Matrix1 trigger rate is " << dpsimMatrix1[i] << endl;
    cout << "The " << rate[i] << "000 Matrix2 trigger rate is " << dpsimMatrix2[i] << endl;
    cout << "The " << rate[i] << "000 Matrix3 trigger rate is " << dpsimMatrix3[i] << endl;
    cout << "The " << rate[i] << "000 Matrix4 trigger rate is " << dpsimMatrix4[i] << endl;
    inFile[i] -> Close();
    inFile[i] -> Delete();
  }

  //get the graphs
  TGraphErrors *GdpsimMatrix1 = new TGraphErrors();
  TGraphErrors *GdpsimMatrix2 = new TGraphErrors();
  TGraphErrors *GdpsimMatrix3 = new TGraphErrors();
  TGraphErrors *GdpsimMatrix4 = new TGraphErrors();
  
  //get the rates
  Float_t nCollfile = 5e7.;
  Float_t spillLength = 4.2;
  Float_t Freq = 5.3e7;
  Float_t nTrigsMatrix1[nRates];
  Float_t nTrigsMatrix1_err[nRates];
  Float_t nTrigsMatrix2[nRates];
  Float_t nTrigsMatrix2_err[nRates];
  Float_t nTrigsMatrix3[nRates];
  Float_t nTrigsMatrix3_err[nRates];
  Float_t nTrigsMatrix4[nRates];
  Float_t nTrigsMatrix4_err[nRates];
  
  for(int i = 0; i < nRates; i++){
    nTrigsMatrix1[i] = float(dpsimMatrix1[i]) / ( nJobs[i]*nCollfile/(rate[i]*1000)/(spillLength*Freq) );
    nTrigsMatrix1_err[i] = sqrt(dpsimMatrix1[i])/dpsimMatrix1[i] * nTrigsMatrix1[i];
    nTrigsMatrix2[i] = float(dpsimMatrix2[i]) / ( nJobs[i]*nCollfile/(rate[i]*1000)/(spillLength*Freq) );
    nTrigsMatrix2_err[i] = sqrt(dpsimMatrix2[i])/dpsimMatrix2[i] * nTrigsMatrix2[i];
    nTrigsMatrix3[i] = float(dpsimMatrix3[i]) / ( nJobs[i]*nCollfile/(rate[i]*1000)/(spillLength*Freq) );
    nTrigsMatrix3_err[i] = sqrt(dpsimMatrix3[i])/dpsimMatrix3[i] * nTrigsMatrix3[i];
    nTrigsMatrix4[i] = float(dpsimMatrix4[i]) / ( nJobs[i]*nCollfile/(rate[i]*1000)/(spillLength*Freq) );
    nTrigsMatrix4_err[i] = sqrt(dpsimMatrix4[i])/dpsimMatrix4[i] * nTrigsMatrix4[i];

    cout << dpsimMatrix1[i] << " " << rate[i]*1000 << " " << nTrigsMatrix1[i] << " " << nTrigsMatrix1_err[i] << endl;
    GdpsimMatrix1 -> SetPoint(i, rate[i], nTrigsMatrix1[i]);
    GdpsimMatrix1 -> SetPointError(i, 0., nTrigsMatrix1_err[i]);
    GdpsimMatrix2 -> SetPoint(i, rate[i], nTrigsMatrix2[i]);
    GdpsimMatrix2 -> SetPointError(i, 0., nTrigsMatrix2_err[i]);
    GdpsimMatrix3 -> SetPoint(i, rate[i], nTrigsMatrix3[i]);
    GdpsimMatrix3 -> SetPointError(i, 0., nTrigsMatrix3_err[i]);
    GdpsimMatrix4 -> SetPoint(i, rate[i], nTrigsMatrix4[i]);
    GdpsimMatrix4 -> SetPointError(i, 0., nTrigsMatrix4_err[i]);
  }
  GdpsimMatrix1 -> SetName("GdpsimMatrix1");
  GdpsimMatrix2 -> SetName("GdpsimMatrix2");
  GdpsimMatrix3 -> SetName("GdpsimMatrix3");
  GdpsimMatrix4 -> SetName("GdpsimMatrix4"); 
  TFile *outFile = new TFile("./TriggerData.root", "RECREATE");
  GdpsimMatrix1 -> Write();
  GdpsimMatrix2 -> Write();
  GdpsimMatrix3 -> Write();
  GdpsimMatrix4 -> Write();
}
