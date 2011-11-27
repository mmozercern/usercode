#ifndef ROO_4LMASSES2D
#define ROO_2LMASSES2D

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class Roo4lMasses2D : public RooAbsPdf {
public:
  Roo4lMasses2D();
  Roo4lMasses2D(const char *name, const char *title,
		RooAbsReal& _mZstar,	     
		RooAbsReal& _mZ,	     
		RooAbsReal& _mZZ,	     
		RooAbsReal& _Gamma,	     
		RooAbsReal& _p0,	     
		RooAbsReal& _p1,	     
		RooAbsReal& _p2,	     
		RooAbsReal& _a0,	     
		RooAbsReal& _a1,	     
		RooAbsReal& _a2,	     
		RooAbsReal& _CBmean,	     
		RooAbsReal& _CBwidth,	     
		RooAbsReal& _CBalpha,	     
		RooAbsReal& _CBn             
		);
  Roo4lMasses2D(const Roo4lMasses2D& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new Roo4lMasses2D(*this,newname); }
  inline virtual ~Roo4lMasses2D() { }

protected:

    RooRealProxy mZstar;	     
    RooRealProxy mZ;     
    RooRealProxy mZZ;     
    RooRealProxy Gamma;     
    RooRealProxy p0;     
    RooRealProxy p1;     
    RooRealProxy p2;     
    RooRealProxy a0;     
    RooRealProxy a1;     
    RooRealProxy a2;     
    RooRealProxy CBmean;    
    RooRealProxy CBwidth;    
    RooRealProxy CBalpha;	     
    RooRealProxy CBn;             

    Double_t evaluate() const ;

private:

  ClassDef(Roo4lMasses2D,1) 
};
 
#endif
