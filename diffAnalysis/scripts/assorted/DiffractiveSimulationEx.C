
// Main program to generate diffractive dijet events in ultra-peripheral
// Pb+p collisions.

// Author: Ilkka Helenius

#include "Pythia8/Pythia.h"
#include <algorithm>
#include "Pythia8Plugins/HepMC2.h"
#include "YODA/Histo1D.h"
#include "YODA/WriterYODA.h"
//#include "Hist.h"

using namespace Pythia8;

// Photon flux from leptons, corresponds to internal Lepton2gamma.

class Lepton2gamma2 : public PDF {

public:

  // Constructor.
  Lepton2gamma2(int idBeamIn) : PDF(idBeamIn) {}

  // Update the photon flux.
  void xfUpdate(int , double x, double Q2) {
    xgamma = 0.5 * 0.007297353080 / M_PI * (1. + pow2(1. - x)) / Q2;
  }
};

class Proton2gamma : public PDF {

public:

  // Constructor.
  Proton2gamma(int idBeamIn) : PDF(idBeamIn) {}

  // Update the photon flux.
  void xfUpdate(int , double x, double Q2) {
    double Q20 = 0.71;
    double FQ4 = 1. / pow4( 1 + Q2 / Q20 );
    xgamma = 0.5 * 0.007297353080 / M_PI * (1. + pow2(1. - x)) / Q2 * FQ4;
  }
};

// Photon flux from lead-ions. Integrated over impact parameters > 2*r_Pb.
// Suitable for photo-nuclear processes but not for photon-photon.
// This should be considered as an experimental setup and used with caution.

class Nucleus2gamma : public PDF {

public:

  // Constructor.
  Nucleus2gamma(int idBeamIn) : PDF(idBeamIn) {}

  // Update the photon flux.
  void xfUpdate(int , double x, double ) {

    // Minimum impact parameter (~2*radius) [fm].
    // double bmin = 2 * 6.636;
    double bmin = 6.636 + 0.7;

    // Charge of the nucleus.
    double z = 82.;

    // Per-nucleon mass for lead.
    double m2 = pow2(0.9314);
    double alphaEM = 0.007297353080;
    double hbarc = 0.197;
    double xi = x * sqrt(m2) * bmin / hbarc;
    double bK0 = besselK0(xi);
    double bK1 = besselK1(xi);
    double intB = xi * bK1 * bK0 - 0.5 * pow2(xi) * ( pow2(bK1) - pow2(bK0) );
    xgamma = 2. * alphaEM * pow2(z) / M_PI * intB;
  }


};

// The main program.

int main(int argc, char* argv[]) {

  bool doRivet = false;
  if (argc == 2) {
    doRivet = true;
    // cout << "No output file given!" << endl;
  }

  string outFile = "";
  if (doRivet) outFile = argv[1];

  // Generator.
  Pythia pythia;
  Settings &settings = pythia.settings;


  // Decrease the output.
  pythia.readString("Init:showChangedSettings = off");
  pythia.readString("Init:showChangedParticleData = off");
  pythia.readString("Next:numberCount = 10000");
  pythia.readString("Next:numberShowInfo = 0");
  pythia.readString("Next:numberShowProcess = 1");
  pythia.readString("Next:numberShowEvent = 0");

  int configuration = 4;
  int diffSelection = 3;

  // test configuration.
  if (configuration == 0) {

    // Beam parameters.
    pythia.readString("Beams:frameType = 1");
    pythia.readString("Beams:idA = 22");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 200.");

  // HERA default.
  } else if (configuration == 1) {

    // Beam parameters.
    pythia.readString("Beams:frameType = 2");
    pythia.readString("Beams:idA = -11");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eA = 27.5");
    pythia.readString("Beams:eB = 920.");

    pythia.readString("PDF:lepton2gamma = on");
    pythia.readString("PDF:lepton2gammaSet = 1");

  // HERA external.
  } else if (configuration == 2) {

    // Beam parameters.
    pythia.readString("Beams:frameType = 2");
    pythia.readString("Beams:idA = -11");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eA = 27.5");
    pythia.readString("Beams:eB = 920.");

    pythia.readString("PDF:lepton2gamma = on");
    pythia.readString("PDF:lepton2gammaSet = 2");
    PDFPtr photonFlux = make_shared<Lepton2gamma2>(-11);
    pythia.setPhotonFluxPtr(photonFlux, 0);

    // LHC external.
  } else if ( configuration == 3) {

    pythia.readString("Beams:frameType = 2");
    pythia.readString("Beams:idA = -11");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eA = 2500.");
    pythia.readString("Beams:eB = 2500.");

    pythia.readString("PDF:lepton2gamma = on");
    pythia.readString("PDF:lepton2gammaSet = 2");
    pythia.readString("PDF:lepton2gammaApprox = 2");
    PDFPtr photonFlux = make_shared<Nucleus2gamma>(-11);
    pythia.setPhotonFluxPtr(photonFlux, 0);
    pythia.readString("Photon:SampleQ2 = off");

   // Photoproduction in p+p.
   } else if ( configuration == 4) {

      // Workaround: Use massive muon as a proton proxy.
      // Provides just the flux here so fine for now.
      pythia.readString("13:m0 = 0.938");

      pythia.readString("Beams:frameType = 2");
      pythia.readString("Beams:idA = -13");
      pythia.readString("Beams:idB = 2212");
      pythia.readString("Beams:eA = 6500");
      pythia.readString("Beams:eB = 6500.");

      pythia.readString("PDF:lepton2gamma = on");
      pythia.readString("PDF:lepton2gammaSet = 2");
      PDFPtr photonFlux = make_shared<Proton2gamma>(-13);
      pythia.setPhotonFluxPtr(photonFlux, 0);

  }

  // Photoproduction and relevant hard processes.
  pythia.readString("Photon:ProcessType = 0");
  pythia.readString("HardQCD:all = on");
  pythia.readString("PhotonParton:all = on");

  // pythia.readString("PartonLevel:MPI = off");
  // pythia.readString("PartonLevel:ISR = off");
  // pythia.readString("PartonLevel:FSR = off");
  // pythia.readString("HadronLevel:all = off");
  // pythia.readString("Photon:showUnres = true");

  // Diffractive stuff.
  // pythia.readString("PDF:PomSet = 8");

  pythia.readString("Diffraction:hardDiffSide = 1");
  pythia.readString("SigmaDiffractive:PomFlux = 7");
  pythia.readString("Diffraction:doHard = on");
  settings.mode("Diffraction:sampleType", diffSelection);

  // Limit partonic pThat, leading jet pT > 7.5 for the ZEUS analysis.
  // pythia.readString("Photon:Wmin = 10.0");
  // pythia.readString("Photon:Q2max = 1.0");

  // pythia.readString("PhaseSpace:bias2Selection = on");
  // pythia.readString("PhaseSpace:bias2SelectionPow = 2.");
  // settings.parm("PhaseSpace:bias2SelectionRef", 10.0);

  // Use optimized pT0ref for photon-hadron, a more detailed fit for this?
  pythia.readString("MultipartonInteractions:pT0Ref = 3.0");

  int pTregion = 1;

  double pTjetMin1 = 8.0;
  double pTjetMin2 = 6.0;
  double mJetsCut  = 14.;

  if (pTregion == 1) {
    pythia.readString("PhaseSpace:pTHatMin = 5.0");
  } else if (pTregion == 2) {
    pythia.readString("PhaseSpace:pTHatMin = 15.0");
    pTjetMin1 = 20.0;
    pTjetMin2 = 15.0;
    mJetsCut  = 35.;
    pythia.readString("Photon:Wmin = 30.0");
  }

  // Jet acceptance.
  double etaJetMax = 4.4;
  double radius    = 1.0;
  double pTjetMin  = pTjetMin2 * 0.5;

  // Diffractive cuts.
  double xPomCut = 0.025;

  // Initialize jet finder.
  SlowJet slowJet( -1, radius, pTjetMin, etaJetMax);

  // Initialize the generator.
  pythia.init();

  // Interface for conversion from Pythia8::Event to HepMC event.
  HepMC::Pythia8ToHepMC ToHepMC;
  HepMC::IO_GenEvent ascii_io(outFile.c_str(), std::ios::out);

  double HTbinsTable[] = { 14., 19., 24., 29. , 35. ,42., 50., 59., 70., 84.,
                           100., 119., 141., 168., 200.};
  int nHTbins = 14;
  vector<double> HTbinsVector;
  for( int i = 0; i < nHTbins + 1; ++i) HTbinsVector.push_back(HTbinsTable[i]);
  YODA::Histo1D Hdsigma_HT(HTbinsVector,"/UPC_hard_diffraction/d01-x01-y01",
    "H_T");

  double xbinsTable[] = { 0.0001, 0.000215, 0.000464, 0.001, 0.00215, 0.00464, 
                          0.01, 0.0215, 0.0464,  0.1,    0.215,   0.464, 1.0 };
  int nxBins = 12;
  vector<double> xbinsVector;
  for( int i = 0; i < nxBins + 1; ++i) xbinsVector.push_back(xbinsTable[i]);
  YODA::Histo1D Hdsigma_xA(xbinsVector,"/UPC_hard_diffraction/d02-x01-y01",
    "x_A");

  double zbinsTable[] = { 0.0001    ,  0.00018738,  0.00035112,  0.00065793,
                          0.00123285,  0.00231013,  0.00432876,  0.00811131,
                          0.01519911,  0.02848036,  0.05336699,  0.1};
  int nzBins = 11;
  vector<double> zbinsVector;
  for( int i = 0; i < nzBins + 1; ++i) zbinsVector.push_back(zbinsTable[i]);
  YODA::Histo1D Hdsigma_zG(zbinsVector, "/UPC_hard_diffraction/d03-x01-y01",
    "x_A");

  YODA::Histo1D W(20, 0.0, 2000, "/UPC_hard_diffraction/d04-x01-y01",
                  "Invariant mass of photon-proton system");
  YODA::Histo1D MX(20, 0.0, 1000, "/UPC_hard_diffraction/d05-x01-y01",
                  "Invariant mass of the diffractive system");
  YODA::Histo1D xGm(10, 0.0, 1.0, "/UPC_hard_diffraction/d06-x01-y01",
                  "Momentum fraction of partons inside photon");
  YODA::Histo1D zPom(10, 0.0, 1.0, "/UPC_hard_diffraction/d07-x01-y01",
                  "Momentum fraction partons inside Pomeron");
  YODA::Histo1D ETjet1(9, 10, 100, "/UPC_hard_diffraction/d08-x01-y01",
                  "Transverse energy of jet 1");
  YODA::Histo1D deltaEta(10, 0.0, 5.0, "/UPC_hard_diffraction/d09-x01-y01",
                  "Eta separation of the two jets");

  // Counters.
  int nDiffA = 0;
  int nJetEvents = 0;

  // Number of events.
  int nEvent = 1000;
  // int nEvent = 1000000;

  // Begin event loop. Skip if fails.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    // Generate next event.
    if (!pythia.next()) continue;

    // Calculate number of diffractive events in side B.
    if (pythia.info.isHardDiffractiveA()) ++nDiffA;

    // Write the HepMC event to file when using rivet.
    if (doRivet) {

      // Construct new empty HepMC event and fill it.
      HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
      ToHepMC.fill_next_event( pythia, hepmcevt );

      // Write the HepMC event to file. Done with it.
      ascii_io << hepmcevt;
      delete hepmcevt;
    }

    slowJet.analyze( pythia.event );
    vector<Vec4> pJets;
    for (int i = 0; i < slowJet.sizeJet();++i) {

      // Kinematics of the jet.
      Vec4 pJet     = slowJet.p(i);
      double pTJet  = pJet.pT();
      double etaJet = pJet.eta();

      // Check that jet within acceptance.
      if ( (pTJet > pTjetMin2) && ( abs(etaJet) < etaJetMax) ) {
        pJets.push_back(slowJet.p(i));
      }
    }

    // Sort jets according to E_T and reject events which fail cuts.
    // sort (pJets.begin(), pJets.end(), cmpByEt);

    double weight = pythia.info.weight();

    // Photon and pomeron 4-momenta.
    Vec4 pGam = pythia.process[7].p();
    Vec4 pPom = pythia.process[8].p();
    Vec4 pE   = pythia.process[1].p();
    Vec4 pP   = pythia.process[2].p();

    // Partonic jets for testing.
    // Vec4 p1 = pythia.process[11].p();
    // Vec4 p2 = pythia.process[12].p();

    // if (p2.pT() > p1.pT()) {
    //   Vec4 tmp = p2;
    //   p2 = p1;
    //   p1 = tmp;
    // }

    // double MGamPom = (pGam + pPom).mCalc();
    double y       = (pP * pGam) / (pP * pE);
    double xPom    = (pPom * pGam) / (pP * pGam);

    // Cut on x_Pom to enhance Pomeron contribution (wrt. Reggeon?)
    if (xPom > xPomCut) continue;

    // At least two jets, one with pT > pTcut1.
    if (pJets.size() < 2) continue;
    if (pJets[0].pT() < pTjetMin1) continue;

    /// Analysis here...
    double hT = 0.;
    double ei = 0.;
    double pz = 0.;
    Vec4 pTot;

    double sumJetsPos = 0;
    double sumJetsNeg = 0;

    for (unsigned int i = 0; i < pJets.size(); ++i){
      hT += pJets[i].pT();
      ei += pJets[i].e();
      pz += pJets[i].pz();
      pTot += pJets[i];
      if (i < 2) {
        sumJetsPos += pJets[i].eT()*exp( pJets[i].eta());
        sumJetsNeg += pJets[i].eT()*exp(-pJets[i].eta());
      }
    }

    double sqrts = pythia.info.eCM();

    double pAbs  = pTot.pAbs();
    double mJets = sqrt( pow2(ei) - pow2(pAbs) );
    double yJets = 0.5 * log( (ei + pz) / (ei - pz) );
    double xA    = mJets / sqrts * exp(-yJets);
    double zG    = mJets / sqrts * exp( yJets);

    double xGmObs  = sumJetsPos / (2. * y * pE.e());
    double zPomObs = sumJetsNeg / (2. * xPom * pP.e());

    double dEta = abs(pJets[0].eta()-pJets[1].eta());

    // cout << xA << ' ' << zGamma << ' ' << hT << endl;

    // Cut on jet mass.
    if (mJets < mJetsCut) continue;

    ++nJetEvents;

    double Wgmp = pythia.info.eCMsub();
    W.fill(Wgmp, weight);

    double WgmPom = pythia.process[5].m();
    MX.fill(WgmPom, weight);

    xGm.fill(xGmObs, weight);
    zPom.fill(zPomObs, weight);

    ETjet1.fill(pJets[0].eT(), weight);

    deltaEta.fill(dEta,weight);

    Hdsigma_HT.fill(hT, weight);
    Hdsigma_xA.fill(xA, weight);
    Hdsigma_zG.fill(zG, weight);

  } // End of event loop.

  // Show statistics.
  pythia.stat();

  double sigmaNorm = pythia.info.sigmaGen() / pythia.info.weightSum() * 1.e6;

  Hdsigma_HT.scaleW(sigmaNorm);
  Hdsigma_xA.scaleW(sigmaNorm);
  Hdsigma_zG.scaleW(sigmaNorm);
  W.scaleW(sigmaNorm);
  MX.scaleW(sigmaNorm);
  xGm.scaleW(sigmaNorm);
  zPom.scaleW(sigmaNorm);
  ETjet1.scaleW(sigmaNorm);
  deltaEta.scaleW(sigmaNorm);

  YODA::Writer& writer = YODA::mkWriter ("yoda");

  // vector<YODA::Histo1D> histos;
  vector<YODA::AnalysisObject*> histos;
  histos.push_back(&Hdsigma_HT);
  histos.push_back(&Hdsigma_xA);
  histos.push_back(&Hdsigma_zG);
  histos.push_back(&W);
  histos.push_back(&MX);
  histos.push_back(&xGm);
  histos.push_back(&zPom);
  histos.push_back(&ETjet1);
  histos.push_back(&deltaEta);

  if (configuration == 3) {

    if (diffSelection == 3) writer.write("yodafiles/pPb_UPC_diff_pTref_PDF.yoda", 
      histos.begin(), histos.end());
    else if(diffSelection == 4) writer.write("yodafiles/pPb_UPC_diff_pTref_MPI.yoda", 
      histos.begin(), histos.end());

    // writer.write("yodafiles/pPb_UPC_HT_diff_3.yoda", Hdsigma_HT);
    // writer.write("yodafiles/pPb_UPC_xA_diff_3.yoda", Hdsigma_xA);
    // writer.write("yodafiles/pPb_UPC_zG_diff_3.yoda", Hdsigma_zG);
    
    // writer.write("yodafiles/pPb_UPC_W_diff_3.yoda", W);
    // writer.write("yodafiles/pPb_UPC_MX_diff_3.yoda", MX);
    // writer.write("yodafiles/pPb_UPC_xGm_diff_3.yoda", xGm);

    // writer.write("yodafiles/pPb_UPC_zPom_diff_3.yoda", zPom);
    // writer.write("yodafiles/pPb_UPC_ETjet1diff_3.yoda", ETjet1);
    // writer.write("yodafiles/pPb_UPC_deltaEta_diff_3.yoda", deltaEta);

  } else if ( configuration == 4) {

    if (diffSelection == 3) writer.write("yodafiles/pp_UPC_diff_pTref_PDF.yoda", 
      histos.begin(), histos.end());
    else if (diffSelection == 4) writer.write("yodafiles/pp_UPC_diff_pTref_MPI.yoda", 
      histos.begin(), histos.end());

    // writer.write("yodafiles/pp_UPC_HT_diff_3.yoda", Hdsigma_HT);
    // writer.write("yodafiles/pp_UPC_xA_diff_3.yoda", Hdsigma_xA);
    // writer.write("yodafiles/pp_UPC_zG_diff_3.yoda", Hdsigma_zG);
    
    // writer.write("yodafiles/pp_UPC_W_diff_3.yoda", W);
    // writer.write("yodafiles/pp_UPC_MX_diff_3.yoda", MX);
    // writer.write("yodafiles/pp_UPC_xGm_diff_3.yoda", xGm);
    
    // writer.write("yodafiles/pp_UPC_zPom_diff_3.yoda", zPom);
    // writer.write("yodafiles/pp_UPC_ETjet1diff_3.yoda", ETjet1);
    // writer.write("yodafiles/pp_UPC_deltaEta_diff_3.yoda", deltaEta);
    
  }

  cout << "Number of diffractive events in side A = " << nDiffA << endl;

  // Done.
  return 0;
}
