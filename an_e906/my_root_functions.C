void SetHistoStyle (TH1 *histo, int marker = 20, int color = 1, int linewidth = 1)
{
    histo -> SetMarkerStyle (marker);
    histo -> SetMarkerColor (color);
    histo -> SetLineColor (color);
    histo -> SetLineWidth (linewidth);
}

void SetGraphStyle (TGraph *graph, int marker = 20, int color = 1, int linewidth = 1)
{
    graph -> SetMarkerStyle (marker);
    graph -> SetMarkerColor (color);
    graph -> SetMarkerSize (linewidth);
    graph -> SetLineColor (color);
    graph -> SetLineWidth (linewidth);
}

void SetGraphErrorStyle (TGraph *graph, int marker = 20, int color = 1, int linewidth = 1)
{
    graph -> SetMarkerStyle (marker);
    graph -> SetMarkerColor (color);
    graph -> SetMarkerSize (linewidth);
    graph -> SetLineColor (color);
    graph -> SetLineWidth (linewidth);
}

void SetHistoAxes (TH1 *histo, double x_min, double x_max, double y_min, double y_max)
{
    int x_bin = histo -> GetXaxis() -> FindBin (x_min);
    int x_bax = histo -> GetXaxis() -> FindBin (x_max);
    histo -> GetXaxis() -> SetRange (x_bin, x_bax);
    histo -> SetMinimum (y_min);
    histo -> SetMaximum (y_max);
}

void PadMargins (double my_bottom, double my_left, double my_right, double my_top)
{
    gPad -> SetBottomMargin (my_bottom);
    gPad -> SetLeftMargin (my_left);
    gPad -> SetRightMargin (my_right);
    gPad -> SetTopMargin (my_top);
}
