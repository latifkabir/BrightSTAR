// $Id: St_TLA_Maker.h,v 1.16 2014/08/06 11:43:53 jeromel Exp $

#ifndef STAR_St_TLA_Maker
#define STAR_St_TLA_Maker


#ifndef StMaker_H
#include "StMaker.h"
#endif


// You may forward declare other classes if you have data-members
// used in pointer-only context by using declaration like
// class St_SomeExternClass;
//
// You do need in such simple case to add the include file
// (and compilation is much faster).

class St_TLA_Maker : public StMaker
{
 private:
  // Private method declaration if any
 
 protected:
  // Protected method if any

 public: 
  St_TLA_Maker(const char *name="TLA");
  virtual       ~St_TLA_Maker();
  virtual Int_t Init();
  virtual Int_t  Make();

  // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
  // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 

  /// Displayed on session exit, leave it as-is please ...
  virtual const char *GetCVS() const
  {
      static const char cvs[]="Tag $Name:  $ $Id: St_TLA_Maker.h,v 1.16 2014/08/06 11:43:53 jeromel Exp $ built " __DATE__ " " __TIME__ ; 
      return cvs;
  }

  ClassDef(St_TLA_Maker,0)   //StAF chain virtual base class for Makers
};

#endif

