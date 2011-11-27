#ifndef ROOBETAFUNC
#define ROOBETAFUNC

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class RooBetaFunc : public RooAbsPdf {
public:
  RooBetaFunc();
  RooBetaFunc(const char *name, const char *title,
	      RooAbsReal& _mZstar,	     
	      RooAbsReal& _mZ,	     
	      RooAbsReal& _mZZ,	     
	      RooAbsReal& _Gamma,
	      RooAbsReal& _a0,  // mZZ distribution vars
	      RooAbsReal& _a1, 
	      RooAbsReal& _a2,
	      RooAbsReal& _a3,
	      RooAbsReal& _f
		    );
  RooBetaFunc(const RooBetaFunc& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooBetaFunc(*this,newname); }
  inline virtual ~RooBetaFunc() { }

protected:

    RooRealProxy mZstar;	     
    RooRealProxy mZ;     
    RooRealProxy mZZ;     
    RooRealProxy Gamma;
    RooRealProxy a0;  // mZZ distribution vars
    RooRealProxy a1;  // mZZ distribution vars
    RooRealProxy a2;
    RooRealProxy a3;
    RooRealProxy f;

    Double_t evaluate() const ;

private:

  ClassDef(RooBetaFunc,1) 
};
 
#endif
