#ifndef ROOPOSPOLY
#define ROOPOSPOLY

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class RooPosPoly : public RooAbsPdf {
public:
  RooPosPoly();
  RooPosPoly(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _a1, 
	      RooAbsReal& _a2,
	      RooAbsReal& _a3
		    );
  RooPosPoly(const RooPosPoly& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooPosPoly(*this,newname); }
  inline virtual ~RooPosPoly() { }

protected:

    RooRealProxy x;
    RooRealProxy a1;
    RooRealProxy a2;
    RooRealProxy a3;

    Double_t evaluate() const ;

private:

  ClassDef(RooPosPoly,1) 
};
 
#endif
