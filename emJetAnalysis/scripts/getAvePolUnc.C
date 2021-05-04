const double f2f_blue = 0;
const double f2f_yell = 0.031;

const double n_blue = 49;
const double n_yell = 49;

const  double unc_scale = 6.6; //in percentage
const double dsa_profile = 3.1; //in percentage
int getAvePolUnc(const char* file = "fills.txt")
{
  char *poldir = "/star/u/zchang/2013-10-jets/run_Polarimetry";
  //char *poldir = "/star/u/zchang/2013-10-jets/run_Polarimetry/pol_run";
  char *rdir = "/star/u/zchang/2013-05-jets/dev/rel_files/vpd_x";

  int fill;
  ifstream in(file);

  double pb_sum = 0, py_sum = 0, rate_sum = 0;
  double pb_sum_err = 0, py_sum_err = 0;

  int counter = 0;
  while(in >> fill){
    cout<<fill<<endl;
    double pb = 0, py = 0, rate = 0;
    double dt = 0;
    int run;
    ifstream inrun(Form("%s/fill_runs/%d.txt", poldir, fill));
    while(inrun >> run){
      cout<<run<<endl;
      double pbr, pyr;
      char* polfile = Form("%s/pol_run/%d.pol.dat", poldir, run);
      ifstream inpol(polfile);
      if(!inpol) cout<<"loading polarization failure!!! "<<polfile<<endl;
      //cout<<"polarization: ";
      inpol>>pbr>>pyr;
      //cout<<"blue: "<<pbr<<" yellow: "<<pyr<<endl;
      inpol.close();

      int dtr;
      char* polerrfile = Form("%s/pol_run/%d.pol.err.dat", poldir, run);
      ifstream inpolerr(polerrfile);
      if(!inpolerr) cout<<"loading polarization error failure!!! "<<polerrfile<<endl;
      inpolerr>>dtr;
      //cout<<"dtr: "<<dtr<<endl;
      dtr /= 3600; //in hours
      inpolerr.close();

      //
      double raterun;
      char* rfile = Form("%s/%d.vpdx.dat", rdir, run);
      ifstream inr(rfile);
      if(!inr) cout<<"loading vpdx rate failure!!! "<<rfile<<endl;
      inr>>raterun;
      //cout<<"raterun: "<<raterun<<endl;
      inr.close();
      
      pb += pbr*raterun;
      py += pyr*raterun;
      rate += raterun;
      dt += dtr*raterun;
    }
    inrun.close();

    double pb_err = 0, py_err = 0;
    ifstream inpolerrfill(Form("%s/pol_fill/%d.pol.err.dat", poldir, fill));
    double t0,pba, pb0err,dpbdterr, pya, py0err, dpydterr;
    inpolerrfill >> t0 >> pba >> pb0err >> dpbdterr
		 >>pya >> py0err >> dpydterr;
    inpolerrfill.close();
    pb0err /= 100; dpbdterr /= 100;
    py0err /= 100; dpydterr /= 100;
    cout<<"blue errors: "<<pb0err<<" "<<dpbdterr<<endl;
    cout<<"yell errors: "<<py0err<<" "<<dpydterr<<endl;
    pb_err = getError(1, pb0err, dt/rate, dpbdterr);
    py_err = getError(1, py0err, dt/rate, dpydterr);

    pb_sum += pb;
    py_sum += py;
    rate_sum += rate;

    pb /= rate;
    py /= rate;
    //add fill-to-fill uncertainty;
    pb_err = getError(1, pb_err, 1, f2f_blue*pb);
    cout<<"pb: "<<py<<" pb err: "<<pb_err<<endl;
    pb_sum_err += TMath::Power(pb_err*rate, 2);
    py_err = getError(1, pb_err, 1, f2f_yell*py);
    cout<<"py: "<<py<<" py err: "<<py_err<<endl;
    py_sum_err += TMath::Power(py_err*rate, 2);
    cout<<py_sum_err<<" "<<rate<<endl;

    counter++;
  }
  cout<<"rate_sum: "<<rate_sum<<endl;
  double pb_ave = pb_sum;
  if(rate_sum > 0.) pb_ave /= rate_sum;
  double py_ave = py_sum;
  if(rate_sum > 0.) py_ave /= rate_sum;

  double pb_ave_err = TMath::Sqrt(pb_sum_err);
  if(rate_sum > 0.) pb_ave_err /= rate_sum;
  

  double py_ave_err = TMath::Sqrt(py_sum_err);
  if(rate_sum > 0.) py_ave_err /= rate_sum;

  cout<<"number of fills counted: "<<counter<<endl;

  double factor_blue = TMath::Sqrt(1-counter/n_blue);
  pb_ave_err *= factor_blue;

  double factor_yell = TMath::Sqrt(1-counter/n_yell);
  py_ave_err *= factor_yell;

  double unc_f2f_blue = pb_ave_err/pb_ave*100;

  double unc_f2f_yell = py_ave_err/py_ave*100;

  double unc_f2f = getError(1, unc_f2f_blue, 1, unc_f2f_yell);
  
  double unc_profile = dsa_profile/TMath::Sqrt(counter+1); //in percentage;

  cout<<"average polarization by rate: "<<endl
      <<"blue: "<<pb_ave<<" fill-by-fill error: "<<pb_ave_err<<" uncertainty in percentage: "<<unc_f2f_blue<<endl
      <<"yell: "<<py_ave<<" fill-by-fill error: "<<py_ave_err<<" uncertainty in percentage: "<<unc_f2f_yell<<endl;

  cout<<"scale: "<<unc_scale<<" f2f: "<<unc_f2f<<" profile: "<<unc_profile<<endl;
  double unc = getError(1, unc_scale, 1, getError(1, unc_f2f, 1, unc_profile));

  cout<<"uncertainty: "<<unc<<endl;
  return 1;
}

double getError(double a, double dx, double b, double dy)
{
  return TMath::Sqrt(TMath::Power(a*dx,2)+TMath::Power(b*dy,2));
}
