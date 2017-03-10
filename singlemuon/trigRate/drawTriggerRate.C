
void drawTriggerRate(const char* schema)
{

   gROOT->Reset();
   gBenchmark->Reset();
   gStyle->SetOptStat(1);
   gStyle->SetPalette(1);

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
   int MatrixAvg[4];
   int MatrixStddev[4];
   char querystmt0[5000];
   
   //get the rate dep
   double xval, yval, weight;
   double sumx[4], sumy[4];
   double ratex, ratey;
   double wsum;
   TFile *inFile0 = new TFile("./rateDep.root");
   TH1F *HrateDep = (TH1F*) inFile0 -> Get("hrate");
   //HrateDep -> Rebin(2);
   HrateDep -> Scale(1./HrateDep -> GetEntries());

   //get the graphs and Draw
   TFile *inFile = new TFile("./TriggerData.root");
   TGraphErrors *GMatrix[4];
   TGraphErrors *GMatrixWeight[4];
   char Gname[64];
   TBox *BMatrix[4];
   TLine *LMatrix[4];

   for(int i = 0; i < 4; i++){
     sprintf(querystmt0, "select avg(value), stddev(value) from Scaler where scalerName='RawMATRIX%d' and spillType='EOS' and spillID in (select spillID from Spill where dataQuality=0 and targetPos=6);", i+1);
     TSQLResult* res0 = server->Query(querystmt0);
     int nRows = res0->GetRowCount();
     for(int j = 0; j < nRows; ++j)
       {
	 if(i % 1 == 0)
        {
            cout << "Converting Matrix" << i+1 << "\t";
	}
	 
	 TSQLRow* row0 = res0->Next();
	 MatrixAvg[i] = getInt(row0->GetField(0));
	 MatrixStddev[i] = getInt(row0->GetField(1));
	 delete row0;
	 //cout << "Matrix" << i << " " << "Avg:  " << MatrixAvg[i] << "\t MatrixStddev:  " << MatrixStddev[i] << endl;
       }
     cout << "Matrix" << i+1 << "Avg:  " << MatrixAvg[i] << "\t MatrixStddev:  " << MatrixStddev[i] << endl;
     //Get the TBoxes

     LMatrix[i] = new TLine(0, MatrixAvg[i], 100, MatrixAvg[i]);
     LMatrix[i] -> SetLineWidth(2);
     BMatrix[i] = new TBox(0, MatrixAvg[i] - MatrixStddev[i], 100, MatrixAvg[i] + MatrixStddev[i]);
     //BMatrix1[i] -> SetLineColor(4);
     BMatrix[i] -> SetFillColor(4);
     BMatrix[i] -> SetFillStyle(3003);
     sprintf(Gname, "GdpsimMatrix%d", i+1);
     GMatrix[i] = (TGraphErrors*) inFile -> Get(Gname);
     GMatrix[i] -> SetMinimum(-0.01);
     GMatrix[i] -> SetMarkerStyle(24);
     GMatrix[i] -> SetMarkerColor(1);
     //GMatrix[i] -> GetYaxis() -> SetMaxDigits(5);
     GMatrix[i] -> GetXaxis() -> SetTitle("Trigger Intensity (protons/bucket)");
     sprintf(Gname, "rawMATRIX%d counts/spill", i+1);
     GMatrix[i] -> GetYaxis() -> SetTitle(Gname);
     sprintf(Gname, "Matrix%d trigger", i+1);
     GMatrix[i] -> SetTitle(Gname);
     
     //get the average value
     sumx[i] = 0.; sumy[i] = 0.; wsum = 0;
     for(int j = 0; j < 11; j++){
       if(j == 0) weight = HrateDep -> GetBinContent(3);
       else weight = HrateDep -> GetBinContent(4*j + 1);
       GMatrix[i] -> GetPoint(j, xval, yval);
       sumx[i]+= weight * xval;
       sumy[i] += weight * yval;
       wsum += weight;
       //cout << i << " "<< j << " " << xval << " " << yval << " " << weight << endl;
     }
     ratex =  sumx[i] / wsum[i];
     ratey =  sumy[i] / wsum[i];
     //cout << "\t" << i << " " << ratex << " " << ratey << " " << endl;
     GMatrixWeight[i] = new TGraphErrors();
     GMatrixWeight[i] -> SetPoint(0, ratex, ratey);
     GMatrixWeight[i] -> SetMarkerStyle(21);
     GMatrixWeight[i] -> SetMarkerColor(2);

     //Get the difference real/sim
     cout << "The real/sim ratio for rawMATRIX" << i+1 << " is:  " << MatrixAvg[i] / ratey << " + " <<  (MatrixAvg[i] +  MatrixStddev[i])  / ratey <<  " - " <<  (MatrixAvg[i] -  MatrixStddev[i])  / ratey << endl;   
   }


   
   //TCanvas
   TCanvas *c1 = new TCanvas("c1","c1",600,400);
   c1 -> cd();
   HrateDep -> Draw();
   c1 -> SaveAs("./images/triggerint.gif");
   
   //Draw everything
   TCanvas *c4 = new TCanvas("c4", "c4", 1200, 800);
   c4 -> Divide(2, 2);
   for(int i = 0; i < 4; i++){
     c4 -> cd(i+1);
     //gPad -> SetLogy();
     GMatrix[i] -> Draw("ap");
     GMatrixWeight[i] -> Draw("p");
     GMatrixWeight[i] -> Print();
     BMatrix[i] -> Draw();
     LMatrix[i] -> Draw();
   }
   c4 -> cd();
   c4 -> SaveAs("./images/triggerrate.gif");
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
