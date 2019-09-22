// Filename: cronJob.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep  8 16:58:40 2019 (-0400)
// URL: jlab.org/~latif

#include<unistd.h> 
void cronJob()
{
    cout << "Submitting first batch of jobs ...." << endl;
    SubmitSumsJob("RunEmcTreeMaker", "/star/u/kabir/GIT/BrightSTAR/resources/jobRunList/temp1.txt");
    cout << "Sleepig for a while ..." << endl;
    sleep(80);

    cout << "Submitting 2nd batch of jobs ...." << endl;
    SubmitSumsJob("RunEmcTreeMaker", "/star/u/kabir/GIT/BrightSTAR/resources/jobRunList/temp2.txt");
    sleep(80);

    // cout << "Submitting 3nd batch of jobs ...." << endl;
    // SubmitSumsJob("TpcTrackQA", "/star/u/kabir/GIT/BrightSTAR/resources/jobRunList/temp.txt");
}
