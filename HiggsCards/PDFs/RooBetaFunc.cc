#include <iostream>
#include <math.h>
#include "TMath.h"
#include "RooBetaFunc.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(RooBetaFunc) 

 RooBetaFunc::RooBetaFunc(){}

 RooBetaFunc::RooBetaFunc(const char *name, const char *title, 
			  RooAbsReal& _mZstar,	     
			  RooAbsReal& _mZ,	     
			  RooAbsReal& _mZZ,	     
			  RooAbsReal& _Gamma,
			  RooAbsReal& _a0,  // mZZ distribution vars
			  RooAbsReal& _a1,
			  RooAbsReal& _a2,
			  RooAbsReal& _a3,
			  RooAbsReal& _f
			  
				      ) :
   RooAbsPdf(name,title), 
   mZstar("mZstar","mZstar",this,_mZstar),
   mZ("mZ","mZ",this,_mZ),
   mZZ("mZZ","mZZ",this,_mZZ),
   Gamma("Gamma","Gamma",this,_Gamma),
   a0("a0","a0",this,_a0),
   a1("a1","a1",this,_a1),
   a2("a2","a2",this,_a2),
   a3("a3","a3",this,_a3),
   f("f","f",this,_f)
 { 
 } 


 RooBetaFunc::RooBetaFunc(const RooBetaFunc& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZstar("mZstar",this,other.mZstar),
   mZ("mZ",this,other.mZ),
   mZZ("mZZ",this,other.mZZ),
   Gamma("Gamma",this,other.Gamma),
   a0("a0",this,other.a0),
   a1("a1",this,other.a1),
   a2("a2",this,other.a2),
   a3("a3",this,other.a3),
   f("f",this,other.f)
   
 { 
 } 



 double RooBetaFunc::evaluate() const 
 { 

   double mZDistribution, acceptance;

   //================= mZ Distribution =================
   double beta= (1-(mZstar-mZ)*(mZstar-mZ)/(mZZ*mZZ))*(1-(mZstar+mZ)*(mZstar+mZ)/(mZZ*mZZ));
   if(beta<0) return 0.0;
   else mZDistribution = beta;

   //================= acceptance ======================
   acceptance = a0+a1*mZstar+a2*mZstar*mZstar+a3*mZstar*mZstar*mZstar;

   //================= on-shell Z contamination ========

   double onShellZ = exp(-(mZstar-mZ)*(mZstar-mZ)/(2*Gamma*Gamma))/sqrt(2*3.1415*Gamma*Gamma);
   
   //std::cout << mZZDistribution << " : " << mZDistribution << " : " << acceptance << std::endl;
   return mZDistribution*(acceptance+f*onShellZ);
 }

