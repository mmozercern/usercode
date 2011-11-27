#ifndef ROO_2L2JMASSES2D_BKG
#define ROO_2L2JMASSES2D_BKG

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class Roo2l2jMasses2D_Bkg : public RooAbsPdf {
public:
  Roo2l2jMasses2D_Bkg();
  Roo2l2jMasses2D_Bkg(const char *name, const char *title,
		      RooAbsReal& _mZstar,	     
		      RooAbsReal& _mZ,	     
		      RooAbsReal& _mZZ,	     
		      RooAbsReal& _Gamma,	     
		      RooAbsReal& _p0,	     
		      RooAbsReal& _p1,	     
		      RooAbsReal& _p2,	     
		      RooAbsReal& _p3,	     
		      RooAbsReal& _a0,	     
		      RooAbsReal& _a1,	     
		      RooAbsReal& _a2,
		      RooAbsReal& _a3	     
		);
  Roo2l2jMasses2D_Bkg(const Roo2l2jMasses2D_Bkg& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new Roo2l2jMasses2D_Bkg(*this,newname); }
  inline virtual ~Roo2l2jMasses2D_Bkg() { }

protected:

    RooRealProxy mZstar;	     
    RooRealProxy mZ;     
    RooRealProxy mZZ;     
    RooRealProxy Gamma;     
    RooRealProxy p0;     
    RooRealProxy p1;     
    RooRealProxy p2;
    RooRealProxy p3;
    RooRealProxy a0;     
    RooRealProxy a1;     
    RooRealProxy a2;
    RooRealProxy a3;     

    Double_t evaluate() const ;

private:

  ClassDef(Roo2l2jMasses2D_Bkg,1) 
};
 
#endif
