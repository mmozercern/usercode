#include <iostream>
#include <math.h>
#include "TMath.h"
#include "Roo2l2jMasses2D_Bkg.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(Roo2l2jMasses2D_Bkg) 

 Roo2l2jMasses2D_Bkg::Roo2l2jMasses2D_Bkg(){}

Roo2l2jMasses2D_Bkg::Roo2l2jMasses2D_Bkg(const char *name, const char *title, 
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
					 ) :
   RooAbsPdf(name,title), 
   mZstar("mZstar","mZstar",this,_mZstar),
   mZ("mZ","mZ",this,_mZ),
   mZZ("mZZ","mZZ",this,_mZZ),
   Gamma("Gamma","Gamma",this,_Gamma),
   p0("p0","p0",this,_p0),
   p1("p1","p1",this,_p1),
   p2("p2","p2",this,_p2),
   p3("p3","p3",this,_p3),
   a0("a0","a0",this,_a0),
   a1("a1","a1",this,_a1),
   a2("a2","a2",this,_a2),
   a3("a3","a3",this,_a3)

 { 
 } 


 Roo2l2jMasses2D_Bkg::Roo2l2jMasses2D_Bkg(const Roo2l2jMasses2D_Bkg& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZstar("mZstar",this,other.mZstar),
   mZ("mZ",this,other.mZ),
   mZZ("mZZ",this,other.mZZ),
   Gamma("Gamma",this,other.Gamma),
   p0("p0",this,other.p0),
   p1("p1",this,other.p1),
   p2("p2",this,other.p2),
   p3("p3",this,other.p3),
   a0("a0",this,other.a0),
   a1("a1",this,other.a1),
   a2("a2",this,other.a2),
   a3("a3",this,other.a3)
 { 
 } 


 double Roo2l2jMasses2D_Bkg::evaluate() const 
 { 
   double mZstarDistribution;
   double Numerator,Denominator,beta;
   beta = (1-(mZstar-mZ)*(mZstar-mZ)/(mZZ*mZZ))*(1-(mZstar+mZ)*(mZstar+mZ)/(mZZ*mZZ));

   if(beta<0.0) beta = .00000001;

   //Numerator = mZstar*mZstar*sqrt(beta)*(2+(mZstar*mZstar-mZZ*mZZ+mZ*mZ)*(mZstar*mZstar-mZZ*mZZ+mZ*mZ)/(4*mZstar*mZstar*mZ*mZ));
   //Denominator = 2*(mZstar*mZstar-mZ*mZ)*(mZstar*mZstar-mZ*mZ)-2*mZ*mZ*Gamma*Gamma;

   //-24324.4+336.81*mH + -1.13757*mH*mH
   //1620.46+-22.6879*mH + 0.0775867*mH*mH
   //-32.8534+0.463359*mH + -0.00159783*mH*mH

   double par0=-24324.4+336.81*mZZ  -1.13757*mZZ*mZZ;
   double par1=1620.46-22.6879*mZZ + 0.0775867*mZZ*mZZ;
   double par2=-32.8534+0.463359*mZZ  -0.00159783*mZZ*mZZ;
   double par3=0.;

   //double acceptance = par0+par1*mZstar+par2*mZstar*mZstar+par3*mZstar*mZstar*mZstar;
   
   //=========== 3rd-order bernstein polynomial =====================
   //double y = (mZstar-mZstar.min())/(mZstar.max()-mZstar.min());
   double y = (mZstar-20.)/70.;
   double acceptance = p0*pow(1-y,3)+p1*3*y*pow(1-y,2)+p2*3*y*y*(1-y)+p3*y*y*y;
   //================================================================
   
   //---------- calculate normalization factor of mZstarDistribution
   double upperLimit=mZZ-mZ, lowerLimit=20.;

   double C0=(729*p0-486*p1+108*p2-8*p3)/343;
   double C1=(-243*p0+351*p1-120*p2+120*p3)/3430;
   double C2=(27*p0-60*p1+39*p2-600*p3)/34300;
   double C3=(-p0+3*p1-3*p2+p3)/343000;

   double mZstarNormalization=
     //contribution from mZstar^0
     (C0/pow(mZZ,4))*(
		      (mZZ*mZZ-mZ*mZ)*(mZZ*mZZ-mZ*mZ)*(pow(upperLimit,1)-pow(lowerLimit,1))/(1)
		      -2*(mZZ*mZZ+mZ*mZ)*(pow(upperLimit,3)-pow(lowerLimit,3))/(3)
		      +(pow(upperLimit,5)-pow(lowerLimit,5))/(5)
		      )
     //contribution from mZstar^1
     +(C1/pow(mZZ,4))*(
			      (mZZ*mZZ-mZ*mZ)*(mZZ*mZZ-mZ*mZ)*(pow(upperLimit,2)-pow(lowerLimit,2))/(2)
			      -2*(mZZ*mZZ+mZ*mZ)*(pow(upperLimit,4)-pow(lowerLimit,4))/(4)
			      +(pow(upperLimit,6)-pow(lowerLimit,6))/(6)
			      )
     //contribution from mZstar^2
     +(C2/pow(mZZ,4))*(
				   (mZZ*mZZ-mZ*mZ)*(mZZ*mZZ-mZ*mZ)*(pow(upperLimit,3)-pow(lowerLimit,3))/(3)
				   -2*(mZZ*mZZ+mZ*mZ)*(pow(upperLimit,5)-pow(lowerLimit,5))/(5)
				   +(pow(upperLimit,7)-pow(lowerLimit,7))/(7)
				   )
     //contribution from mZstar^3
     +(C3/pow(mZZ,4))*(
				      (mZZ*mZZ-mZ*mZ)*(mZZ*mZZ-mZ*mZ)*(pow(upperLimit,4)-pow(lowerLimit,4))/(4)
				      -2*(mZZ*mZZ+mZ*mZ)*(pow(upperLimit,6)-pow(lowerLimit,6))/(6)
				      +(pow(upperLimit,8)-pow(lowerLimit,8))/(8)
				      );


   mZstarDistribution =  beta*acceptance/mZstarNormalization;

   //std::cout << "beta: " << beta << " acceptance: " << acceptance << " mZstarNormalization: " << mZstarNormalization << std::endl;

   //bernstein polynomials ...
   //double x = (mZZ-mZZ.min())/(mZZ.max()-mZZ.min());
   double x = (mZZ-120.)/50.;
   // =============== order 2 berstein polynomial =============
   // ---------------------------------------------------------
   //double mZZDistribution = a0*(1-x)*(1-x)+a1*2*x*(1-x)+a2*x*x;
   // ---------------------------------------------------------
   // =========== order 3 berstein polynomial =================
   // ---------------------------------------------------------
   double mZZDistribution = a0*pow(1-x,3)+a1*3*x*(1-x)*(1-x)+a2*3*x*x*(1-x)+a3*pow(x,3);
   // ---------------------------------------------------------
   
   return mZstarDistribution*mZZDistribution;
 }

