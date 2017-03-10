void getRateDependence(const char *schema)
{
  gROOT->Reset();
  gBenchmark->Reset();
  gStyle->SetOptStat(1);
  gStyle->SetPalette(1);

  // Make histogram
  TH1F *hrate = new TH1F("hrate", "Rate Distribution of Events", 42, -1250., 103750.);
  hrate -> SetXTitle("Live Protons");  
  
  // This it used to time the SQL action of the macro
  gBenchmark->Start("SQL");
  
  // Connect to server
  TSQLServer *server = TSQLServer::Connect("mysql://e906-db3.fnal.gov:3306",
					   "seaguest", "qqbar2mu+mu-");

  // Select which database to use on the server 
  char seedstmt[64];
  sprintf(seedstmt, "use %s;", schema);
  server->Exec(seedstmt);
  
  // Query the values
  const double PEDESTAL = 36.791;
  char querystmt[5000];
  char signstmt[2];
  double weight[27] = {0.000814246430361413, 0.0028662467149288, 0.00597015326639906, 0.0121262946061061,
		     0.0300863195179747, 0.0777262437180552, 0.159446650644417, 0.259932709364831,
		     0.36718876894966, 0.488159093692654, 0.678969311099113, 0.847788074599439, 0.956475273764143,
		     1.,
		     0.989173954042814, 0.897678016090413, 0.767828869998712, 0.647167321489559, 0.533894756174369,
		     0.448848741080746, 0.356435437171761, 0.263693103645649, 0.177964720504253,
		     0.108504562083177, 0.0540099990325891, 0.019218568399343, 0.00308302089003216};
  double intensity[27];
  double unit;
  int nEvents = 700000;
  //int nEvents = 1000;
  
  //sprintf(querystmt, "select QIE.`RF-13`, QIE.`RF-12`, QIE.`RF-11`, QIE.`RF-10`, QIE.`RF-09`, QIE.`RF-08`, QIE.`RF-07`, QIE.`RF-06`, QIE.`RF-05`, QIE.`RF-04`, QIE.`RF-03`, QIE.`RF-02`, QIE.`RF-01`, QIE.`RF+00`, QIE.`RF+01`, QIE.`RF+02`, QIE.`RF+03`, QIE.`RF+04`, QIE.`RF+05`, QIE.`RF+06`, QIE.`RF+07`, QIE.`RF+08, QIE.`RF+09`, QIE.`RF+10`, QIE.`RF+11`, QIE.`RF+12`, QIE.`RF+13`, Beam.value, BeamDAQ.QIEsum FROM QIE, Beam, BeamDAQ WHERE QIE.spillID = BeamDAQ.spillID AND QIE.spillID = Beam.spillID AND Beam.name = 'S:G2SEM' LIMIT %d;", nEvents);
  sprintf(querystmt, "select QIE.`RF-13`, QIE.`RF-12`, QIE.`RF-11`, QIE.`RF-10`, QIE.`RF-09`, QIE.`RF-08`, QIE.`RF-07`, QIE.`RF-06`, QIE.`RF-05`, QIE.`RF-04`, QIE.`RF-03`, QIE.`RF-02`, QIE.`RF-01`, QIE.`RF+00`, QIE.`RF+01`, QIE.`RF+02`, QIE.`RF+03`, QIE.`RF+04`, QIE.`RF+05`, QIE.`RF+06`, QIE.`RF+07`, QIE.`RF+08`, QIE.`RF+09`, QIE.`RF+10`, QIE.`RF+11`, QIE.`RF+12`, QIE.`RF+13`, Beam.value, BeamDAQ.QIEsum, Event.NIM3 FROM QIE, Beam, BeamDAQ, Event WHERE QIE.spillID = BeamDAQ.spillID AND QIE.spillID = Beam.spillID AND Beam.name = 'S:G2SEM' AND QIE.spillID in (select spillID from Spill where dataQuality=0 and targetPos=7) AND QIE.spillID = Event.spillID AND QIE.eventID = Event.eventID AND Event.NIM3=1 LIMIT %d;", nEvents);
  printf(querystmt);
  cout<<endl;
  TSQLResult* res = server->Query(querystmt);
  int nRows = res -> GetRowCount();
  for(int i = 0; i < nRows; i++)
    {
    
      TSQLRow* row = res->Next();
      double sum = 0.;
      double wsum = 0.;
      //for(int j = -13; j <= 13; j++) //uncomment for chamber intensity
      for(int j = 0; j < 1; j++) //uncomment for trigger intensity
	{
	  intensity[j+13] = (getFloat(row->GetField(j+13)) - PEDESTAL) * getFloat(row->GetField(27)) / (getFloat(row->GetField(28)) - PEDESTAL*369000.*588.);
	  //cout << i << "\t" << j << "\t" << weight[j+13] << "\t" << intensity[j+13] << "\t\t" << unit << "\t\t" <<  getFloat(row->GetField(j+13)) << "\t" <<  getFloat(row->GetField(27)) << "\t" << getFloat(row->GetField(28)) << "\t" << getFloat(row->GetField(29)) << endl;
	  sum += weight[j+13]*(intensity[j+13]);
	  wsum += weight[j+13];	  
	}
      
      rate = sum/wsum;
      hrate -> Fill(rate);
      if(i % 1000 == 0)
	{
	  cout << "Converting Event" << i << "\t Rate:  " << rate << endl;
	}
    }
  
  TCanvas *c1 = new TCanvas("c1", "c1", 600, 400);
  c1 -> cd();
  hrate -> Draw();

  TFile *outFile = new TFile("rateDep.root", "RECREATE");
  outFile -> cd();
  hrate -> Write();
  
}

int getInt(const char* row)
{
    if(row == NULL)
    {
        gEvent->log("Integer content is missing.");
        return -9999;
    }
    return atoi(row);
}

float getFloat(const char* row)
{
    if(row == NULL)
    {
        gEvent->log("Floating content is missing.");
        return -9999.;
    }
    return atof(row);
}

