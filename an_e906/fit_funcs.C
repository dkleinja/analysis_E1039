Double_t gammadist(Double_t *x, Double_t *par)
{
  Double_t fitvalgamma = par[0] * TMath::GammaDist(x[0],par[1],par[2],par[3]);
  return fitvalgamma;
}

Double_t gammadist2(Double_t *x, Double_t *par)
{
  Double_t fitvalgamma = par[0] * TMath::GammaDist(x[0],par[1],par[2],par[3]);
  //Double_t fitvalgamma = par[0] + par[1] * TMath::GammaDist(x[0],par[2],par[3],par[4]);
  return fitvalgamma;
}


Double_t gausdist(Double_t *x, Double_t *par)
{
  Double_t fitvalgaus = par[0] * TMath::Gaus(x[0],par[1],par[2],par[3]);
  return fitvalgaus;
}

Double_t gausdist2(Double_t *x, Double_t *par)
{
  Double_t fitvalgaus = par[0] * TMath::Gaus(x[0],par[1],par[2],par[3]);
  return fitvalgaus;
}


Double_t nbddist2(Double_t *x, Double_t *par)
{
  Double_t fitvalnbd = par[0] + par[1]*(TMath::Gamma(x+par[2])/(TMath::Gamma(x+1)*TMath::Gamma(par[2])))*(TMath::Power((par[3]/par[2]),x))*(TMath::Power((1+(par[3]/par[2])),-x-par[2]));
}

Double_t bothdist(Double_t *x, Double_t *par)
{
  //Double_t fitval = par[6]*(TMath::GammaDist(x[0],par[1],par[2],par[3])) + TMath::Gaus(x[0],par[3],par[4],par[5]);
  //return fitval;
  return gammadist(x,par) + gausdist(x,&par[4]);
  //return gammadist2(x,par) + gausdist(x,&par[5]);
  //return par[0] + gammadist(x,&par[1]) + gausdist(x,&par[5]);
  //return gammadist(x,par) + gausdist(x,&par[4]) + gausdist2(x,&par[7]);
  
}

Double_t bothdist2(Double_t *x, Double_t *par)
{
  
  return gausdist(x,par) + gammadist(x,&par[4]);
  
}
