#include <iostream>
#include <math.h>
#include "TMath.h"
#include "RooPosPoly.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(RooPosPoly) 

 RooPosPoly::RooPosPoly(){}

 RooPosPoly::RooPosPoly(const char *name, const char *title, 
			  RooAbsReal& _x,
			  RooAbsReal& _a1,
			  RooAbsReal& _a2,
			  RooAbsReal& _a3
				      ) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   a1("a1","a1",this,_a1),
   a2("a2","a2",this,_a2),
   a3("a3","a3",this,_a3)
 { 
 } 


 RooPosPoly::RooPosPoly(const RooPosPoly& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   a1("a1",this,other.a1),
   a2("a2",this,other.a2),
   a3("a3",this,other.a3)
 { 
 } 



 double RooPosPoly::evaluate() const 
 { 

   double t = x;//(x-x.min())/(x.max()-x.min());

   return 1+a1*t*t+a2*pow(t,4)+a3*pow(t,6);
 }

