#include <iostream>
#include <math.h>
#include "TMath.h"
#include "RooTheta2Func.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(RooTheta2Func) 

  RooTheta2Func::RooTheta2Func(){}

RooTheta2Func::RooTheta2Func(const char *name, const char *title, 
		       RooAbsReal& _x,
		       RooAbsReal& _a1,
		       RooAbsReal& _a2,
		       RooAbsReal& _a3,
		       RooAbsReal& _m,
		       RooAbsReal& _g
			
				      ) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   a1("a1","a1",this,_a1),
   a2("a2","a2",this,_a2),
   a3("a3","a3",this,_a3),
   m("m","m",this,_m),
   g("g","g",this,_g)
 { 
 } 


 RooTheta2Func::RooTheta2Func(const RooTheta2Func& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   a1("a1",this,other.a1),
   a2("a2",this,other.a2),
   a3("a3",this,other.a3),
   m("m",this,other.m),
   g("g",this,other.g)
 { 
 } 



 double RooTheta2Func::evaluate() const 
 { 
   
   double t = (x-x.min())/(x.max()-x.min());

   return (1+a1*t*t+a2*pow(t,4)+a3*pow(t,6))/(exp((x-m)/g)+1);
 }

