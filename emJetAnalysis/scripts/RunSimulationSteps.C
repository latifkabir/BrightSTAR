// Filename: RunSimulationSteps.C
// Description: 
// Author: Latif Kabir < latiful.kabir@ucr.edu >
// Created: Thu May 14 00:11:49 2026 (-0400)
// URL: latifkabir.github.io

/*
For real analysis, these steps are done by submitting jobs. See these job files under starSim/simJobs
*/


void RunSimulationSteps(TString _step)
{    
    cout << "Running simulation step:" << _step << endl;

    //-------------------- FMS Simulation -------------------------    
    if (_step == "1a")
    {
	//Step 1a:
	// Run PYTHIA using StarSim and then BFC for FMS simulation
	FmsSimRunStarsimAndBfc(1, 10);
    }
    else if (_step == "1b")
    {
	//Step 1b:
	// Reconstruct FMS jet from the created MuDst in the previous step	
	RunFmsJetFinderPro("starSim/pythiaOut.MuDst.root", "testSimJet.root");
    }
    else if (_step == "1c")
    {
	//Step 1c:
	// Generate response matrix 2D plot	
	FmsSimMakeResponseMatrix(-1, "/star/u/kabir/GIT/EmJet-GPC-BrightSTAR/dst/fmsSimData/fmsJet/pass6/FmsJet_Run15_pass6_*.root", "test.root");
    }
    //------------------- EEMC Simulation -----------------------------
    else if (_step == "2a")
    {
	// Step 2a:
	// Run PYTHIA and then BFC
	EEmcSimRunStarsimAndBfc(1, 100);
    }    
    else if (_step == "2b")
    {
	// Step 2b:
	// Reconstruct EEMC Jet with both detector and particle branches from simulated MuDST file
	RunEEmcJetFinderPro("/star/u/kabir/GIT/EmJet-GPC-BrightSTAR/dst/scratch/condor/EEmcSim_Run15_5003_evt1000.MuDst.root", "testEEmcSimJet.root");
    }
    else
    {
	cout << "Invalid step" << endl;
    }
}
