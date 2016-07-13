double simple_asym (double n1, double n2)
{
    double the_simple_asym = 0;
    if ((n1 > 0) && (n2 > 0))
	the_simple_asym = (n1 - n2) / (n1 + n2);
    return the_simple_asym;
}

double simple_error (double n1, double n2, double e1, double e2)
{
    double the_simple_error = 0;
    if ((n1 > 0) && (n2 > 0))
	the_simple_error = 2 * sqrt (pow (n2 * e1, 2) + pow(n1 * e2, 2)) / pow (n1 + n2, 2);
    //cout << the_simple_error << " " << n1 << " " << n2 << " " << e1 << " " << e2 << endl;
    return the_simple_error;
}

double weighted_mean(double n1, double n2, double e1, double e2)
{
  double the_weighted_mean;
  double y1 = n1 / pow(e1, 2) + n2 / pow (e2, 2);
  double y2 = 1 / pow(e1, 2) + 1 / pow (e2, 2);
  the_weighted_mean = y1/y2;
  return the_weighted_mean;
}
 
double weighted_error(double e1, double e2)
{
  double the_weighted_error = sqrt(1/(1 / pow(e1, 2) + 1 / pow (e2, 2)));
  return the_weighted_error;
}

double square_asym (double n1, double n2, double n3, double n4)
{
    double the_square_asym = 0.;
    double y1 = sqrt (n1 * n4);
    double y2 = sqrt (n2 * n3);
    if (y1 + y2)
	the_square_asym = (y1 - y2) / (y1 + y2);
    //printf ("%f %f \n", n1+n2+n3+n4, the_square_asym);
    return the_square_asym;
}

double square_error (double n1, double n2, double n3, double n4, double e1, double e2, double e3, double e4)
{
    double the_square_error = 0.;
    double DEN = pow ((sqrt (n1 * n4) + sqrt (n2 * n3)), 2);
    double p1 = 0.;
    double p2 = 0.;
    double p3 = 0.;
    double p4 = 0.;
    if (n1) p1 = n2 * n3 * n4 / n1 * pow (e1, 2);
    if (n2) p2 = n1 * n3 * n4 / n2 * pow (e2, 2);
    if (n3) p3 = n1 * n2 * n4 / n3 * pow (e3, 2);
    if (n4) p4 = n1 * n2 * n3 / n4 * pow (e4, 2);
    if (DEN)
	the_square_error = sqrt (p1 + p2 + p3 + p4) / DEN;

    //double the_square_asym = 0;
    //double y1 = sqrt (n1 * n4);
    //double y2 = sqrt (n2 * n3);
    //if (y1 + y2)
    //  the_square_asym = (y1 - y2) / (y1 + y2);
    //printf ("%7.0f %8.5f %8.5f\n", n1+n2+n3+n4, the_square_asym, the_square_error);

    return the_square_error;
}

double final_an(double a1, double a2, double r)
{
  double the_final_an;
  the_final_an = (a1 - r*a2)/(1-r);
  return the_final_an;
}

double final_err(double a1, double a2, double r)
{
  double the_final_err;
  the_final_err = sqrt(pow(a1, 2) + pow(r, 2) * pow(a2, 2))/(1-r);
  return the_final_err;
}

Double_t pi_mass (Double_t *x, Double_t *par)
{
    Float_t PI = 3.141592654;
    Double_t gauss1, mpol1, myfunc;
    gauss1 = par[0] / sqrt (2 * PI) / par[2] * exp (-pow (x[0] - par[1], 2) / pow (par[2], 2));
    mpol1 = par[3] + par[4] * x[0];
    myfunc = gauss1 + mpol1;
    return myfunc;
}

Double_t an_cos (Double_t *x, Double_t *par)
{
    Double_t mycosine;;
    mycosine = par[0] * cos (x[0]);
    //mycosine = par[0] * cos (x[0] - 0.25);
    //mycosine = par[0] * sin (x[0]);
    return mycosine;
}

Double_t an_sin (Double_t *x, Double_t *par)
{
    Double_t mysine;;
    mysine = par[0] * sin (x[0] + par[1]);
    return mysine;
}

Double_t an_cosP (Double_t *x, Double_t *par)
{
    Double_t mycosine;;
    mycosine = par[0] + par[1] * cos (x[0]);
    //mycosine = par[0] + par[1] * cos (x[0] - 0.25);
    //mycosine = par[0] + par[1] * sin (x[0]);
    return mycosine;
}

Double_t an_sinP (Double_t *x, Double_t *par)
{
    Double_t mysine;;
    mysine = par[0] + par[1] * sin (x[0] + par[2]);
    return mysine;
}

Double_t phase_modulo (Double_t my_phase)
{
    Float_t PI = 3.141592654;
    my_phase /= (2 * PI);
    my_phase -= int (my_phase);
    if (my_phase < 0.) my_phase += 1.;
    my_phase *= (2 * PI);
    return my_phase;
}

void getPolarizations (int this_fill, Double_t &bluePol, Double_t &bluePolError, Double_t &yellowPol, Double_t &yellowPolError)
{
    Double_t poli[4];
    switch (this_fill) {
	case 8014: poli[0] = 0.516; poli[1] = 0.018; poli[2] = 0.529; poli[3] = 0.019; break;
	case 8015: poli[0] = 0.509; poli[1] = 0.014; poli[2] = 0.519; poli[3] = 0.013; break;
	case 8016: poli[0] = 0.483; poli[1] = 0.020; poli[2] = 0.505; poli[3] = 0.017; break;
	case 8020: poli[0] = 0.565; poli[1] = 0.020; poli[2] = 0.468; poli[3] = 0.019; break;
	case 8021: poli[0] = 0.500; poli[1] = 0.018; poli[2] = 0.464; poli[3] = 0.019; break;
	case 8022: poli[0] = 0.482; poli[1] = 0.012; poli[2] = 0.463; poli[3] = 0.011; break;
	case 8023: poli[0] = 0.526; poli[1] = 0.014; poli[2] = 0.541; poli[3] = 0.014; break;
	case 8024: poli[0] = 0.492; poli[1] = 0.014; poli[2] = 0.517; poli[3] = 0.021; break;
	case 8025: poli[0] = 0.513; poli[1] = 0.020; poli[2] = 0.538; poli[3] = 0.025; break;
	default:   poli[0] = 1.000; poli[1] = 1.000; poli[2] = 1.000; poli[3] = 1.000; break;
    }
    bluePol        = poli[0];
    bluePolError   = poli[1];
    yellowPol      = poli[2];
    yellowPolError = poli[3];
    return;
}
