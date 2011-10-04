#ifndef __ROO_Triangle_RDL__
#define __ROO_Triangle_RDL__

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

class Triangle : public RooAbsPdf {
public:
  Triangle();
  Triangle(const char *name, const char *title,                
	   RooAbsReal& _m,
	   RooAbsReal& _start,
	   RooAbsReal& _turn,
	   RooAbsReal& _stop
	   );	
  
  Triangle(const Triangle& other, const char* name = 0);
  virtual TObject* clone(const char* newname) const { 
    return new Triangle(*this,newname); }

  inline virtual ~Triangle() { }
  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;

protected:

  RooRealProxy m;
  RooRealProxy start;
  RooRealProxy turn;
  RooRealProxy stop;
  
  Double_t evaluate() const;

private:
  
  ClassDef(Triangle,1)
};

#endif
