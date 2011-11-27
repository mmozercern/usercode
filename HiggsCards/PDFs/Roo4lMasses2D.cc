#include <iostream>
#include <math.h>
#include "TMath.h"
#include "Roo4lMasses2D.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(Roo4lMasses2D) 

 Roo4lMasses2D::Roo4lMasses2D(){}

 Roo4lMasses2D::Roo4lMasses2D(const char *name, const char *title, 
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
			      
		    ) :
   RooAbsPdf(name,title), 
   mZstar("mZstar","mZstar",this,_mZstar),
   mZ("mZ","mZ",this,_mZ),
   mZZ("mZZ","mZZ",this,_mZZ),
   Gamma("Gamma","Gamma",this,_Gamma),
   p0("p0","p0",this,_p0),
   p1("p1","p1",this,_p1),
   p2("p2","p2",this,_p2),
   a0("a0","a0",this,_a0),
   a1("a1","a1",this,_a1),
   a2("a2","a2",this,_a2),
   CBmean("CBmean","CBmean",this,_CBmean),
   CBwidth("CBwidth","CBwidth",this,_CBwidth),
   CBalpha("CBalpha","CBalpha",this,_CBalpha),
   CBn("CBn","CBn",this,_CBn)
   
 { 
 } 


 Roo4lMasses2D::Roo4lMasses2D(const Roo4lMasses2D& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZstar("mZstar",this,other.mZstar),
   mZ("mZ",this,other.mZ),
   mZZ("mZZ",this,other.mZZ),
   Gamma("Gamma",this,other.Gamma),
   p0("p0",this,other.p0),
   p1("p1",this,other.p1),
   p2("p2",this,other.p2),
   a0("a0",this,other.a0),
   a1("a1",this,other.a1),
   a2("a2",this,other.a2),
   CBmean("CBmean"  ,this,other.CBmean),
   CBwidth("CBwidth",this,other.CBwidth),
   CBalpha("CBalpha",this,other.CBalpha),
   CBn("CBn",this,other.CBn)

 { 
 } 



 double Roo4lMasses2D::evaluate() const 
 { 
   double mZstarDistribution;
   double Numerator,Denominator,beta;
   beta = (1-(mZstar-mZ)*(mZstar-mZ)/(mZZ*mZZ))*(1-(mZstar+mZ)*(mZstar+mZ)/(mZZ*mZZ));

   if(beta<0.0) return 0.0;

   Numerator = mZstar*mZstar*sqrt(beta)*(2+(mZstar*mZstar-mZZ*mZZ+mZ*mZ)*(mZstar*mZstar-mZZ*mZZ+mZ*mZ)/(4*mZstar*mZstar*mZ*mZ));
   Denominator = 2*(mZstar*mZstar-mZ*mZ)*(mZstar*mZstar-mZ*mZ)-2*mZ*mZ*Gamma*Gamma;

   mZstarDistribution = Numerator/Denominator;

   double acceptance = p0+p1*mZstar+p2*mZstar*mZstar;

   double mZZDistribution;

   //========= crystal ball =========
   double t = (mZZ-CBmean)/CBwidth;
   //std::cout << "CBwidth: " << CBwidth << " CBmean: " << CBmean << " mZZ: " << mZZ << std::endl;
   if (CBalpha < 0) t = -t;

   double absAlpha = fabs((double)CBalpha);

   if (t >= -absAlpha) {
   
     mZZDistribution = exp(-0.5*t*t);
     //std::cout << "Gaussian!! " << -.5*t*t << " : " << mZZDistribution << std::endl;

   }
   else {
     double a =  TMath::Power(CBn/absAlpha,CBn)*exp(-0.5*absAlpha*absAlpha);
     double b = CBn/absAlpha - absAlpha;

     //std::cout << "power!!! a: " << a << " b: " << b << endl;

     mZZDistribution = a/TMath::Power(b - t, CBn);

   }
   //std::cout << "t: " << t << " absAlpha: " << absAlpha << std::endl;
   
   return mZstarDistribution*acceptance*mZZDistribution*(a0+a1*mZZ+a2*mZZ*mZZ);
 }

