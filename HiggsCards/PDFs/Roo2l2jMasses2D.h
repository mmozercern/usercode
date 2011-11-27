#ifndef ROO_2L2JMASSES2D
#define ROO_2L2JMASSES2D

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class Roo2l2jMasses2D : public RooAbsPdf {
public:
  Roo2l2jMasses2D();
  Roo2l2jMasses2D(const char *name, const char *title,
		  RooAbsReal& _mZstar,	     
		  RooAbsReal& _mZ,	     
		  RooAbsReal& _mZZ,	     
		  RooAbsReal& _Gamma,	     
		  RooAbsReal& _p0,	     
		  RooAbsReal& _p1,	     
		  RooAbsReal& _p2,	     
		  RooAbsReal& _p3,	     
		  RooAbsReal& _mean,
		  RooAbsReal& _width
		);
  Roo2l2jMasses2D(const Roo2l2jMasses2D& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new Roo2l2jMasses2D(*this,newname); }
  inline virtual ~Roo2l2jMasses2D() { }

protected:

    RooRealProxy mZstar;	     
    RooRealProxy mZ;     
    RooRealProxy mZZ;     
    RooRealProxy Gamma;     
    RooRealProxy p0;     
    RooRealProxy p1;     
    RooRealProxy p2;
    RooRealProxy p3;     
    RooRealProxy mean;
    RooRealProxy width;

    Double_t evaluate() const ;

private:

  ClassDef(Roo2l2jMasses2D,1) 
};
 
#endif
