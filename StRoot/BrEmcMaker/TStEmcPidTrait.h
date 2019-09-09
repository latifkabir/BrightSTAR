// Filename: TStEmcPidTrait.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep  8 12:23:35 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTEMCPIDTRAIT_H
#define TSTEMCPIDTRAIT_H

struct TStEmcPidTrait
{
    Int_t q[100][100];
    Double_t p[100][100];
    Double_t pt[100][1000];
    Double_t dca[100][100];   
    Double_t beta[100][100];
};

#endif
