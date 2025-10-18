	// Filename: ChiSquareBkg.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Jul 30 22:39:00 2024 (-0400)
// URL: jlab.org/~latif

void ChiSquareBkg()
{
    ifstream inFileLess("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/background_study/Run15An_blue_less_than_2hrs.txt");
    ifstream inFileMore("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/background_study/Run15An_blue_more_than_2hrs.txt");

    if (!inFileLess || !inFileMore)
    {
	cout << "Unable to open input file" << endl;
	return;
    }

    int idx_l;
    int idx_m;
    double pt_l;
    double pt_m;

    double an_l;
    double an_m;
    double dx_l;
    double dx_m;
    double dy_l;
    double dy_m;

    double sdx_l;
    double sdx_m;
    double sdy_l;
    double sdy_m;

    double chiSq = 0.0;
    TH1D *hist = new TH1D("hist", "hist", 30, 0, 0);
    
    while(!inFileLess.eof() && !inFileMore.eof())
    {
	inFileLess >> idx_l >> pt_l >> an_l >> dx_l >> dy_l >> sdx_l >> sdy_l;	
	inFileMore >> idx_m >> pt_m >> an_m >> dx_m >> dy_m >> sdx_m >> sdy_m;

	if(inFileLess.eof() || inFileMore.eof())
		break;

	cout << idx_l << "\t" << pt_l << "\t" << an_l << "\t" << dx_l << "\t" << dy_l << "\t" << sdx_l << "\t" <<sdy_l << endl;	
	cout << idx_m << "\t" << pt_m << "\t" << an_m << "\t" << dx_m << "\t" << dy_m << "\t" << sdx_m << "\t" <<sdy_m << endl;

	// chiSq += pow((an_l - an_m), 2) / an_m;
	chiSq = pow((an_l - an_m), 2) / ( pow(dy_m,2) + pow(dy_l,2) );

	hist->Fill(chiSq);
	
    }
    cout << "Chi Square: " << chiSq << endl;
    inFileLess.close();
    inFileMore.close();

    hist->Draw();
}
