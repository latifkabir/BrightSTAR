#include<fstream>
using namespace std;

//calculate asy. for pi0 side band, used as background asymmetries
//using cross-ratio method with sin(phi) fit
  Int_t crjetAn_new( Float_t elow = 48, Float_t ehigh = 53, Float_t mlow = 1.00, Float_t mhigh = 1.00, Int_t fillnum = 18790,char* infile="/star/u/zwzhu/01zwzhu/Run15/byfill/ipi0jetcor_stfms_Fill18790.root" ){
	
	gStyle->SetOptFit(1111);
	gStyle->SetOptDate(0);
	TCanvas* c1 = new TCanvas("c1","c1",700,500);
	c1->SetGridx(0);
	c1->SetGridy(0);	
	
	Float_t fjetalow = 2.9;
    Float_t fjetahigh = 3.8;
	Float_t pizcut = 0.7;
	double ptcut=2;

	Char_t filllist[50];
        Float_t ene = (elow + ehigh)/2.0;//GeV   
	
	Float_t enew = (ehigh-elow)/2.0;   //GeV. energy window
	
//	sprintf(filllist,"newfilllist_stfms.txt");
//        fstream infile(filllist,ios::in);
//        if(!infile){
//                cout<<filllist<<" doesn't exist!!"<<endl;
//                return -1;
//        }
	
	Char_t resultname[400];
	Char_t resultname2[300];
	sprintf(resultname,"jetAnJP_Fill%d_mass%.2fto%.2fGeV_E%.1fto%.1fGeV.txt",fillnum,mlow,mhigh,elow,ehigh);
	sprintf(resultname2,"jetAnyJP_Fill%d_mass%.2fto%.2fGeV_E%.1fto%.1fGeV.txt",fillnum,mlow,mhigh,elow,ehigh);
	cout<<"results stored at "<<resultname<<endl;
	TString outfilename(resultname);
	TString outfilename2(resultname2);
	TString mvCmd = TString::Format("touch %s",outfilename.Data());
	fstream ofile(outfilename,ios::out);
	fstream ofile2(outfilename2,ios::out);

	Char_t cutbup[800];
	Char_t cutbdown[800];
	Char_t cutyup[800];
	Char_t cutydown[800];
	Char_t filename[800];
	
	Char_t* bdown = "(spinbit==5||spinbit==6)";
	Char_t* bup = "(spinbit==9||spinbit==10)";
	Char_t* ydown = "(spinbit==5||spinbit==9)";
	Char_t* yup = "(spinbit==10||spinbit==6)";

	Int_t fillnum,fillnum1;
     double e0    = 95.0223  ;    
    double e1    =-0.01388;
    double e2    =0.030268 ;    
    double e3  =  -2.45823;   
    
     double uee0   =       93.514  ; 
     double uee1   =   -0.0195701  ; 
     double uee2   =    0.0370517  ; 
     double uee3   =    -0.401819  ; 

     double uept0   =     85.5409  ; 
     double uept1   =  -0.0124882  ; 
     double uept2   =   0.0165788  ; 
     double uept3   =    -2.12506  ; 

	//trigger bits
    Int_t JP2=((0x01)<<10);
        Int_t SmlBd2=((0x01)<<4);
        Int_t SmlBd22=((0x01)<<1);
        Int_t LrgBd2=((0x01)<<7);
    Int_t JP1=((0x01)<<9);
        Int_t SmlBd1=((0x01)<<3);
        Int_t LrgBd1=((0x01)<<6);
    Int_t JP0=((0x01)<<8);
        Int_t SmlBd0=((0x01)<<2);
        Int_t LrgBd0=((0x01)<<5);

	Int_t JP=((0x07)<<8);
	Int_t HiTow=(0x03);
	Int_t clbit=((0x03F)<<2);

	Int_t TrigBitsSelect = JP;
//	Int_t TrigBitsSelect = JP+HiTow+clbit;
        Char_t trigselect[400];
        sprintf(trigselect,"(TrigBits&%d&&TMath::Abs((vertexzyuxi+18.4)<93.4))&&fjetntow>0",TrigBitsSelect);
        Char_t ptnew[400];
        sprintf(ptnew,"((%.4f)+(%.8f)*fjE+ TMath::Exp((fjE-(%.4f))*(%.4f)))*(fjpT-((%.4f)+(%.8f)*fjpT+ TMath::Exp((fjpT-(%.4f))*(%.4f))))",e3,e2,e0,e1,uept3,uept2,uept0,uept1);
        Char_t eNew[400];
        sprintf(eNew,"((%.4f)+(%.8f)*fjE+ TMath::Exp((fjE-(%.4f))*(%.4f)))*(fjE-((%.4f)+(%.8f)*fjE+ TMath::Exp((fjE-(%.4f))*(%.4f))))",e3,e2,e0,e1,uee3,uee2,uee0,uee1);

//sprintf(trigselect,"TMath::Abs(vertexzyuxi+36)<90&&((TrigBits&%d&&fjpT>1.84)||(TrigBits&%d&&fjpT>2.765)||(TrigBits&%d&&fjpT>3.68)||(TrigBits&%d&&fjpT>1.26)||(TrigBits&%d&&fjpT>1.84)||(TrigBits&%d&&fjpT>2.2)||(TrigBits&%d&&fjpT>1.26)||(TrigBits&%d&&fjpT>1.84)||(TrigBits&%d&&fjpT>2.76))",JP0,JP1,JP2,SmlBd0,SmlBd1,SmlBd2,LrgBd0,LrgBd1,LrgBd2);
	//cout<<trigselect<<endl;;
	Char_t htitle[200];
	Char_t htitley[200];
	sprintf(htitle,"crRawAsyFill%d_E%d_M%.2fto%.2fGeV_AllTrigger",fillnum,ene,mlow,mhigh);
	sprintf(htitley,"y_crRawAsyFill%d_E%d_M%.2fto%.2fGeV_AllTrigger",fillnum,ene,mlow,mhigh);
	const int nphibin = 10;//must be even
	TGraphErrors* h1asyphi = new TGraphErrors(0); //A_N vs sin(phi);
	TGraphErrors* h2asyphi = new TGraphErrors(0); //A_N vs sin(phi);
	h1asyphi->SetTitle(htitle);
	h2asyphi->SetTitle(htitley);

//	TH1F* h1asyphi = new TH1F(htitle,htitle,nphibin,-TMath::Pi(),TMath::Pi());
	Double_t totbup[nphibin];
	Double_t totbdown[nphibin];
	Double_t totyup[nphibin];
	Double_t totydown[nphibin];
	for(Int_t i = 0; i < nphibin; i++){
		totbup[i] = totbdown[i] = 0;
	}

	TH1F* h1buphi;//hold up to 30 fills
	TH1F* h1bdphi;
	TH1F* h1yuphi;
	TH1F* h1ydphi;

//	Int_t kcnt = 0;
	TChain* t1 = new TChain("tFmsjet");
//	sprintf(filename,"/star/data05/scratch/zwzhu/FMSnew/JetAna/byFill/ipi0jetcor_stfms_Fill%d.root",fillnum,fillnum);
	sprintf(filename,"/star/data05/scratch/zwzhu/FMSnew/Run15/byfill/ipi0jetcor_stfms_Fill%d.root",fillnum,fillnum);
//	sprintf(filename,"/star/u/zwzhu/01zwzhu/Run15/byfill/ipi0jetcor_stfms_Fill%d.root",fillnum,fillnum);
//	sprintf(filename,"/star/u/zwzhu/shenfw/FMS/JetAna/byFill/ipi0jetcor_stfms_Fill%d.root",fillnum,fillnum);
//	while(infile>>filename){
//		cout<<"adding file: "<<filename<<endl;
		t1->Add(infile);
//		kcnt++;
//	}
	
//	cout<<kcnt<<" file added "<<endl;
		
	Char_t hbutitle[100];
	sprintf(hbutitle,"h1_blueup");
	h1buphi = new TH1F(hbutitle,hbutitle,nphibin,-TMath::Pi(),TMath::Pi());//counts per phi bin
	Char_t hbdtitle[100];
	sprintf(hbdtitle,"h1_bluedown");
	h1bdphi = new TH1F(hbdtitle,hbdtitle,nphibin,-TMath::Pi(),TMath::Pi());
	Char_t hyutitle[100];
	sprintf(hyutitle,"h1_yellup");
	h1yuphi = new TH1F(hyutitle,hyutitle,nphibin,-TMath::Pi(),TMath::Pi());//counts per phi bin
	Char_t hydtitle[100];
	sprintf(hydtitle,"h1_yelldown");
	h1ydphi = new TH1F(hydtitle,hydtitle,nphibin,-TMath::Pi(),TMath::Pi());

//all jets
    sprintf(cutbup,  "%s>%.1f&&TMath::Abs(%s-%.1f)<%f&&fjeta>%.1f&&fjeta<%.1f&&%s&&%s",ptnew,ptcut,eNew,ene,enew,fjetalow,fjetahigh,bup,trigselect);
	sprintf(cutbdown,"%s>%.1f&&TMath::Abs(%s-%.1f)<%f&&fjeta>%.1f&&fjeta<%.1f&&%s&&%s",ptnew,ptcut,eNew,ene,enew,fjetalow,fjetahigh,bdown,trigselect);
	sprintf(cutyup,  "%s>%.1f&&TMath::Abs(%s-%.1f)<%f&&fjeta>%.1f&&fjeta<%.1f&&%s&&%s",ptnew,ptcut,eNew,ene,enew,fjetalow,fjetahigh,yup,trigselect);
	sprintf(cutydown,"%s>%.1f&&TMath::Abs(%s-%.1f)<%f&&fjeta>%.1f&&fjeta<%.1f&&%s&&%s",ptnew,ptcut,eNew,ene,enew,fjetalow,fjetahigh,ydown,trigselect);

//pion w/o recoil	
/*	sprintf(cutbup,"abs(pie-%.1f)<%f&&abs(pipt-%.3f)<%.3f&&%s&&(ptrecoil<2&&ephnear<2&&ephaway<2)",ene,enew,pt,ptw,bup);
	sprintf(cutbdown,"abs(pie-%.1f)<%f&&abs(pipt-%.3f)<%.3f&&%s&&ptrecoil==-1",ene,enew,pt,ptw,bdown);
	sprintf(cutyup,"abs(pie-%.1f)<%f&&abs(pipt-%.3f)<%.3f&&%s&&ptrecoil==-1",ene,enew,pt,ptw,yup);
	sprintf(cutydown,"abs(pie-%.1f)<%f&&abs(pipt-%.3f)<%.3f&&%s&&ptrecoil==-1",ene,enew,pt,ptw,ydown);
*/	

//pions with recoil
/*	sprintf(cutbup,"abs(pie-%.1f)<%f&&%s&&(ptrecoil>2||ephnear>2||ephaway>2)",ene,enew,bup);
	sprintf(cutbdown,"abs(pie-%.1f)<%f&&%s&&(ptrecoil>2||ephnear>2||ephaway>2)",ene,enew,bdown);
	sprintf(cutyup,"abs(pie-%.1f)<%f&&%s&&(ptrecoil>2||ephnear>2||ephaway>2)",ene,enew,yup);
	sprintf(cutydown,"abs(pie-%.1f)<%f&&%s&&(ptrecoil>2||ephnear>2||ephaway>2)",ene,enew,ydown);
*/       	

		cout<<"cutbup: "<<cutbup<<endl;
	//	cout<<"cutbdown: "<<cutbdown<<endl;
	//	cout<<"cutyup: "<<cutyup<<endl;
	//	cout<<"cutydown: "<<cutydown<<endl;
	
// generate cos(phi) distribution for blue up/down, for each fill. With fill-dependent mass window
	//	t1->Project(hbutitle,"cos(Phi)",cutyup); //plot yellow beam asy
	//	t1->Project(hbdtitle,"cos(Phi)",cutydown);
//	t1->Project(hbutitle,"piphi",cutbup); //plot yield sorted by blue beam polarizations
//	t1->Project(hbdtitle,"piphi",cutbdown);
	t1->Project(hbutitle,"fjphi",cutbup); //plot yield sorted by blue beam polarizations
	t1->Project(hbdtitle,"fjphi",cutbdown);
	t1->Project(hyutitle,"fjphi",cutyup); //plot yield sorted by blue beam polarizations
	t1->Project(hydtitle,"fjphi",cutydown);

//calculate raw cross-ratio asymmetry vs sin(phi)
	Double_t rawasybin,rawasybinerr;
	Double_t bdemon; //denominator
	for(Int_t ibin = 1; ibin <= nphibin/2; ibin++){
		totbup[ibin-1] = TMath::Max(h1buphi->GetBinContent(ibin),0.);//blue up at phi
		totbdown[ibin-1+nphibin/2] = TMath::Max(h1bdphi->GetBinContent(ibin+nphibin/2),0.);//blue down at phi+pi
		totbup[ibin-1+nphibin/2] = TMath::Max(h1buphi->GetBinContent(ibin+nphibin/2),0.);//blue up at phi+pi		
		totbdown[ibin-1] = TMath::Max(h1bdphi->GetBinContent(ibin),0.);//blue down at phi
		 cout<<" totbup "<< totbup[ibin-1] <<" totbdown "<<totbdown[ibin-1+nphibin/2]<<" totbup "<<totbup[ibin-1+nphibin/2]<<" totbdown "<<totbdown[ibin-1]<<endl;
		//calculate cross-ratio asy. for this particular phi bin
		bdemon = TMath::Sqrt(totbup[ibin-1]*totbdown[ibin-1+nphibin/2])+TMath::Sqrt(totbdown[ibin-1]*totbup[ibin-1+nphibin/2]);
		rawasybin = (TMath::Sqrt(totbup[ibin-1]*totbdown[ibin-1+nphibin/2])-TMath::Sqrt(totbdown[ibin-1]*totbup[ibin-1+nphibin/2]))/bdemon;
		//error cal. from EEMC note (3.38)
		rawasybinerr = TMath::Sqrt((totbdown[ibin-1+nphibin/2]+totbup[ibin-1])*(totbup[ibin-1+nphibin/2]*totbdown[ibin-1])/TMath::Power(bdemon,4) + (totbup[ibin-1+nphibin/2]+totbdown[ibin-1])*(totbdown[ibin-1+nphibin/2]*totbup[ibin-1])/TMath::Power(bdemon,4));
		//rawasybinerr = 1/TMath::Sqrt(totbdown[ibin-1+nphibin/2]+totbup[ibin-1] + totbup[ibin-1+nphibin/2]+totbdown[ibin-1]);//approx.
		//err of A_N  = TMath::Sqrt(1/P)*rawasybinerr
		cout<<"cos_phi = "<<TMath::Cos(h1buphi->GetBinCenter(ibin+nphibin/2))<<", rawasybin = "<<rawasybin<<", rawasybinerr = "<<rawasybinerr<<endl;

		//rawasybin = (totbup[ibin-1] - totbdown[ibin-1]) / (totbup[ibin-1] + totbdown[ibin-1]);
		//rawasybinerr = TMath::Sqrt( 1/(totbup[ibin-1]+totbdown[ibin-1]) + TMath::Power(totbup[ibin-1]-totbdown[ibin-1],2)/TMath::Power(totbup[ibin-1]+totbdown[ibin-1],3 ));
	
		h1asyphi->SetPoint(ibin-1,TMath::Cos(h1buphi->GetBinCenter(ibin+nphibin/2)),-rawasybin); //becase i am only looking at [-pi,0] 
		h1asyphi->SetPointError(ibin-1,0,rawasybinerr);
		h1asyphi->GetXaxis()->SetTitle("cos#phi");
		h1asyphi->GetYaxis()->SetTitle("raw cross-ratio asy.");
		h1asyphi->GetYaxis()->SetTitleOffset(1.2);
	}
	
	h1asyphi->SetMarkerSize(1.5);
	h1asyphi->SetMarkerStyle(20);
	h1asyphi->SetMarkerColor(kBlue);
	h1asyphi->SetLineColor(kBlue);
	h1asyphi->SetLineWidth(1.5);
	h1asyphi->Draw("Ap");
	TF1* fit = new TF1("fit","pol1",-1,1); //2-parameter fit

//	TF1* fit = new TF1("fit","[0]*x-0.0020124",-1,1); //1-parameter fit
//	fit->SetParameter(0,relumi);
        h1asyphi->Fit(fit,"R+");
	Char_t gifname[200];
        sprintf(gifname,"gifs/Anjet/crasyxfblueFill%d_7ebins_E%.1fGeV_M%.2ftoM%.2fGeV_AllTrigger.png",fillnum,ene,mlow,mhigh);
	Char_t coutname[200];
        sprintf(coutname,"coutfile/Anjet/crasyxfblueFill%d_7ebins_E%.1fGeV_M%.2ftoM%.2fGeV_AllTrigger.C",fillnum,ene,mlow,mhigh);
  //      cout<<"C outname: "<<coutname<<endl;
        cout<<"GIF name: "<<gifname<<endl;
	Float_t asy = fit->GetParameter(1);
	Float_t asyerr = fit->GetParError(1);
	Float_t p0 = fit->GetParameter(0);
	Float_t p0err = fit->GetParError(0);
	ofile<<fillnum<<" "<<ene<<" "<<asy<<" "<<asyerr<<" "<<p0<<" "<<p0err<<" "<<mlow<<" "<<mhigh<<endl;
   //     c1->Print(coutname);
   //     c1->Print(gifname);
//	c1->Print("tmp.ps");

	for(Int_t ibin = 1; ibin <= nphibin/2; ibin++){
		totbup[ibin-1] = TMath::Max(h1yuphi->GetBinContent(ibin),0.);//blue up at phi
		totbdown[ibin-1+nphibin/2] = TMath::Max(h1ydphi->GetBinContent(ibin+nphibin/2),0.);//blue down at phi+pi
		totbup[ibin-1+nphibin/2] = TMath::Max(h1yuphi->GetBinContent(ibin+nphibin/2),0.);//blue up at phi+pi		
		totbdown[ibin-1] = TMath::Max(h1ydphi->GetBinContent(ibin),0.);//blue down at phi
		bool invalid=0;
		//calculate cross-ratio asy. for this particular phi bin
		bdemon = TMath::Sqrt(totbup[ibin-1]*totbdown[ibin-1+nphibin/2])+TMath::Sqrt(totbdown[ibin-1]*totbup[ibin-1+nphibin/2]);
		if (bdemon==0) invalid =1;
		rawasybin = (TMath::Sqrt(totbup[ibin-1]*totbdown[ibin-1+nphibin/2])-TMath::Sqrt(totbdown[ibin-1]*totbup[ibin-1+nphibin/2]))/bdemon;
		//error cal. from EEMC note (3.38)
		rawasybinerr = TMath::Sqrt((totbdown[ibin-1+nphibin/2]+totbup[ibin-1])*(totbup[ibin-1+nphibin/2]*totbdown[ibin-1])/TMath::Power(bdemon,4) + (totbup[ibin-1+nphibin/2]+totbdown[ibin-1])*(totbdown[ibin-1+nphibin/2]*totbup[ibin-1])/TMath::Power(bdemon,4));
		//rawasybinerr = 1/TMath::Sqrt(totbdown[ibin-1+nphibin/2]+totbup[ibin-1] + totbup[ibin-1+nphibin/2]+totbdown[ibin-1]);//approx.
		//err of A_N  = TMath::Sqrt(1/P)*rawasybinerr
		cout<<"cos_phi = "<<TMath::Cos(h1yuphi->GetBinCenter(ibin+nphibin/2))<<", rawasybin = "<<rawasybin<<", rawasybinerr = "<<rawasybinerr<<endl;

		//rawasybin = (totbup[ibin-1] - totbdown[ibin-1]) / (totbup[ibin-1] + totbdown[ibin-1]);
		//rawasybinerr = TMath::Sqrt( 1/(totbup[ibin-1]+totbdown[ibin-1]) + TMath::Power(totbup[ibin-1]-totbdown[ibin-1],2)/TMath::Power(totbup[ibin-1]+totbdown[ibin-1],3 ));
	       if (invalid ==0) {	
			   cout<<"invalid yellow denominator"<<endl;}
		h2asyphi->SetPoint(ibin-1,TMath::Cos(h1yuphi->GetBinCenter(ibin+nphibin/2)),-rawasybin); //becase i am only looking at [-pi,0] 
		h2asyphi->SetPointError(ibin-1,0,rawasybinerr);
		h2asyphi->GetXaxis()->SetTitle("cos#phi");
		h2asyphi->GetYaxis()->SetTitle("raw cross-ratio asy.");
		h2asyphi->GetYaxis()->SetTitleOffset(1.2);
	}
	
	h2asyphi->SetMarkerSize(1.5);
	h2asyphi->SetMarkerStyle(20);
	h2asyphi->SetMarkerColor(kBlue);
	h2asyphi->SetLineColor(kBlue);
	h2asyphi->SetLineWidth(1.5);
	h2asyphi->Draw("Ap");

//	TF1* fit = new TF1("fit","[0]*x-0.0020124",-1,1); //1-parameter fit
//	fit->SetParameter(0,relumi);
        h2asyphi->Fit(fit,"R+");
	Char_t gifname2[200];
        sprintf(gifname2,"gifs/Anjety/crasyxfblueFill%d_7ebins_E%.1fGeV_M%.2ftoM%.2fGeV_AllTrigger.png",fillnum,ene,mlow,mhigh);
	Char_t coutname2[200];
        sprintf(coutname2,"coutfile/Anjety/y_crasyxfblueFill%d_7ebins_E%.1fGeV_M%.2ftoM%.2fGeV_AllTrigger.C",fillnum,ene,mlow,mhigh);
    //    cout<<"C outname: "<<coutname2<<endl;
        cout<<"GIF name: "<<gifname2<<endl;
	Float_t asy2 = fit->GetParameter(1);
	Float_t asyerr2 = fit->GetParError(1);
	Float_t p02 = fit->GetParameter(0);
	Float_t p0err2 = fit->GetParError(0);
	ofile2<<fillnum<<" "<<ene<<" "<<asy2<<" "<<asyerr2<<" "<<p02<<" "<<p0err2<<" "<<mlow<<" "<<mhigh<<endl;
 //       c1->Print(coutname2);
 //      c1->Print(gifname2);
//	c1->Print("tmp.ps");
  }

