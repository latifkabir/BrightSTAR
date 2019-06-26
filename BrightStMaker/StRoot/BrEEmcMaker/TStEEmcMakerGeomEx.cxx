// *-- Author : Victor Perevoztchikov
// 

#include "TStEEmcMakerGeomEx.h"

#include "StChain.h"
#include "St_DataSetIter.h"

#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"

#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/StEEmcSmdGeom.h"

#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEfeeRaw/EEname2Index.h"

ClassImp(TStEEmcMakerGeomEx)

TStEEmcMakerGeomEx::TStEEmcMakerGeomEx(const char* self ,const char* muDstMakerName) : StMaker(self)
{
    mMuDstMaker = (StMuDstMaker*)GetMaker(muDstMakerName);
    assert(mMuDstMaker);
    geomTw = new EEmcGeomSimple(); // tower geomtry
    geomSmd =  StEEmcSmdGeom::instance(); //strip geometry, do NOT call new StEEmcSmdGeom()

}


TStEEmcMakerGeomEx::~TStEEmcMakerGeomEx()
{
    //
}


//_____________________________________________________________________________
/// Init - is a first method the top level StChain calls to initialize all its makers 
Int_t TStEEmcMakerGeomEx::Init()
{
    // connect to eemcDB
    eeDb = (StEEmcDb*)this->GetDataSet("StEEmcDb");
    if(eeDb==0)
    {
	printf("eemcDB must be in the chain, fix it or drop %s-maker, JB\n",GetName());
	assert(eeDb); // eemcDB must be in the chain, fix it
    }

    // Create tables
    // Create Histograms    
    return StMaker::Init();
}


//_____________________________________________________________________________
/// Make - this method is called in loop for each event
Int_t TStEEmcMakerGeomEx::Make()
{
    //
    // PrintInfo();
    //
  
    printf("%s::Make() is called .................................\n",GetName());
    
    if(eeDb->valid()<=0)
    {
	printf("%s aborted, due to no eemcDb records\n",GetName());
	return kStErr;
    }
  
    StMuEmcCollection* emc = mMuDstMaker->muDst()->muEmcCollection();
    if (!emc)
    {
	printf(" No EMC data for this event\n");
	return kStOK;
    }

    int sec, eta, sub, strip, adc, pre;
    int isec, ieta, isub, istrip, ipre;
    StMuEmcHit *hit;

    int i, nh;
 
    printf("\nTotal %d hits in Tower\n",emc->getNEndcapTowerADC());
    nh=0;
    for (i=0; i< emc->getNEndcapTowerADC(); i++)
    {
	emc->getEndcapTowerADC(i,adc, sec, sub, eta);
	if (adc<=0) continue; // print only non-zero values
	nh++;
	isec = sec - 1;
	isub = sub - 1;
	ieta = eta - 1;
	// access geometry info
	float etaCenter     = geomTw->getEtaMean(ieta);
	float phiCenter     = geomTw->getPhiMean(isec,isub);
	TVector3 r = geomTw-> getTowerCenter(isec, isub,ieta);

	printf("\nToweR %2.2dT%c%2.2d  phi/deg=%6.1f eta=%5.2f x=%4.1f y=%4.1f z=%5.1f: adc=%4d\n   ",isec+1,isub+'A',ieta+1,phiCenter/3.14*180,etaCenter,r.x(),r.y(),r.z(),adc );
 
#if 0
	// more geometry info for towers, see .h
	float etaHalfWidth  =geomTw->getEtaHalfWidth(ieta);
	float phiHalfWidth  =geomTw->getPhiHalfWidth(isec,isub);
    
	// center of the tower in two ways
	TVector3 r1=geomTw-> getDirection( etaCenter, phiCenter);
#endif

	// ....... Access  DB 
	const EEmcDbItem *dbItem=eeDb->getT(isec+1,isub+'A',ieta+1);
	assert(dbItem); //  fatal error in EEmcDb-maker
	dbItem->print();
    }
    printf("Total %d towers with ADC>0\n",nh);

    //====================== PRE/POST
    nh= emc->getNEndcapPrsHits();
    printf("\nTotal %d hits in pre1+2+post\n",nh);
    for (i=0; i<nh; i++)
    {
	hit=emc->getEndcapPrsHit(i, sec, sub, eta, pre);
	isec = sec - 1;
	isub = sub - 1;
	ieta = eta - 1;
	ipre = pre - 1;
	printf("\n\npre/post(%d) %2.2d%c%c%2.2d : energy=%f  adc=%d\n",ipre+1,isec+1,ipre+'P',isub+'A',ieta+1,hit->getEnergy(),hit->getAdc());
	// ....... Access  DB 
	char name[20];
	sprintf(name,"%2.2d%c%c%2.2d", isec+1, ipre+'P', isub+'A', ieta+1);
	printf("  DB: name='%s'",name);
	int index = EEname2Index(name);
	printf(", index=%d, ", index);
	const EEmcDbItem *dbItem = eeDb->getByIndex(index);
	assert(dbItem); //  fatal error in EEmcDb-maker
	dbItem->print();

    }
  
    //====================== SMD
    char uv='U';
 
    for(uv='U'; uv<='V'; uv++)
    {
	nh= emc->getNEndcapSmdHits(uv);
	printf("\nTotal %d hits in SMD-%c\n",nh,uv);
	for (i=0; i<nh; i++)
	{
	    hit=emc->getEndcapSmdHit(uv,i, sec, strip);
	    isec = sec - 1;
	    istrip = strip - 1;
	    printf("\nSMD-%c  %2.2d%c%3.3d : energy=%f  adc=%d\n",uv,isec+1,uv,istrip+1,hit->getEnergy(),hit->getAdc());
      
	    // ... geometry
	    int iuv=uv-'U';
	    StructEEmcStrip *st=geomSmd->getStripPtr(istrip,iuv,isec);
	    // StEEmcSmdGeom changed, now stores TVector3 in StructEEmcStrip...
	    // besides, you really shouldn't be accessing these directly
	    // anyway.
	    //StThreeVectorD end1 = st->end1;
	    //StThreeVectorD end2 = st->end2;
	    StThreeVectorD end1 = geomSmd -> getstripEnd( *st, 0 );
	    StThreeVectorD end2 = geomSmd -> getstripEnd( *st, 1 );
      
	    printf("   x1=%6.2f y1=%6.2f z1=%6.2f x2=%6.2f y2=%6.2f z2=%6.2f\n",
		   end1.x(),end1.y(),end1.z(),end2.x(),end2.y(),end2.z());
	    //      	     st.end1.x(),
	    //	     st.end1.y(),st.end2.x(),st.end1.z(),st.end2.y(),st.end2.z());
      

	    // ....... Access  DB

	    const EEmcDbItem *dbItem=eeDb-> getByStrip0(isec,iuv,istrip);
	    assert(dbItem); //  fatal error in EEmcDb-maker
	    dbItem->print();
 
	}
    }
  
    //====================== Clusters tw,pre12po,smdu, smdv
    for (int n=eemc ; n<=esmdv; n++)
    {
	int nClusters = emc->getNClusters(n);
	printf("EEMC depth=%d nClust=%d\n",n,nClusters);
	for (int i=0; i<nClusters; i++)
	{
	    StMuEmcCluster* c =  emc->getCluster(i,n);
	    printf("Cluster(%02i,%02i): energy=%f phi=%f eta=%f nHits=%d\n"
		   ,n,i,c->getEnergy(), c->getPhi(), c->getEta(), c->getNHits() ); 
	}
    }

    //==================== points
    int nPoints = emc->getNEndcapPoints();
    printf("Points: %d\n",nPoints);
    for (int n=0; n<nPoints; n++)
    {
	StMuEmcPoint* p =  emc->getPoint(n);
	printf("Point(%02i) : energy=%f phi=%f eta=%f\n",n,p->getEnergy(), p->getPhi(), p->getEta()); 
    }

    return kStOK;
}








