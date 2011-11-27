#ifndef ROOTHETA2FUNC
#define ROOTHETA2FUNC

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

 
class RooTheta2Func : public RooAbsPdf {
public:
  RooTheta2Func();
  RooTheta2Func(const char *name, const char *title,
	     RooAbsReal& _x,
	     RooAbsReal& _a1, 
	     RooAbsReal& _a2,
	     RooAbsReal& _a3,
	     RooAbsReal& _m,
	     RooAbsReal& _g
		    );
  RooTheta2Func(const RooTheta2Func& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooTheta2Func(*this,newname); }
  inline virtual ~RooTheta2Func() { }

protected:

    RooRealProxy x;
    RooRealProxy a1;
    RooRealProxy a2;
    RooRealProxy a3;
    RooRealProxy m;
    RooRealProxy g;

    Double_t evaluate() const ;

private:

  ClassDef(RooTheta2Func,1) 
};
 
#endif
