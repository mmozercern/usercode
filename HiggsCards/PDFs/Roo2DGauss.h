#ifndef ROO2DGAUSS
#define ROO2DGAUSS

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class Roo2DGauss : public RooAbsPdf {
public:
  Roo2DGauss();
  Roo2DGauss(const char *name, const char *title,
	     RooAbsReal& _mZstar,	     
	     RooAbsReal& _mZZ,	     
	     RooAbsReal& _mean,
	     RooAbsReal& _mean2,
	     RooAbsReal& _width,
	     RooAbsReal& _width2,
	     RooAbsReal& _width3
		);
  Roo2DGauss(const Roo2DGauss& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new Roo2DGauss(*this,newname); }
  inline virtual ~Roo2DGauss() { }

protected:

    RooRealProxy mZstar;	     
    RooRealProxy mZZ;     
    RooRealProxy mean;
    RooRealProxy mean2;
    RooRealProxy width;
    RooRealProxy width2;
    RooRealProxy width3;

    Double_t evaluate() const ;

private:

  ClassDef(Roo2DGauss,1) 
};
 
#endif
