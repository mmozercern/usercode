#ifndef ROO_2L2JMASSES2D_DOUBLECB
#define ROO_2L2JMASSES2D_DOUBLECB

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class Roo2l2jMasses2D_DoubleCB : public RooAbsPdf {
public:
  Roo2l2jMasses2D_DoubleCB();
  Roo2l2jMasses2D_DoubleCB(const char *name, const char *title,
			   RooAbsReal& _mZstar,	     
			   RooAbsReal& _mZ,	     
			   RooAbsReal& _mZZ,	     
			   RooAbsReal& _Gamma,	     
			   RooAbsReal& _p0,	     
			   RooAbsReal& _p1,	     
			   RooAbsReal& _p2,	     
			   RooAbsReal& _mean,
			   RooAbsReal& _width,
			   RooAbsReal& _a1,
			   RooAbsReal& _n1,
			   RooAbsReal& _a2,
			   RooAbsReal& _n2

		);
  Roo2l2jMasses2D_DoubleCB(const Roo2l2jMasses2D_DoubleCB& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new Roo2l2jMasses2D_DoubleCB(*this,newname); }
  inline virtual ~Roo2l2jMasses2D_DoubleCB() { }

protected:

    RooRealProxy mZstar;	     
    RooRealProxy mZ;     
    RooRealProxy mZZ;     
    RooRealProxy Gamma;     
    RooRealProxy p0;     
    RooRealProxy p1;     
    RooRealProxy p2;     
    RooRealProxy mean;
    RooRealProxy width;
    RooRealProxy a1;
    RooRealProxy n1;
    RooRealProxy a2;
    RooRealProxy n2;

    Double_t evaluate() const ;

private:

  ClassDef(Roo2l2jMasses2D_DoubleCB,1) 
};
 
#endif
