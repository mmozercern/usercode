#include <iostream>
#include <math.h>
#include "TMath.h"
#include "Roo2l2jMasses2D.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(Roo2l2jMasses2D) 

 Roo2l2jMasses2D::Roo2l2jMasses2D(){}

Roo2l2jMasses2D::Roo2l2jMasses2D(const char *name, const char *title, 
				  RooAbsReal& _mZstar,
				  RooAbsReal& _mZ,
				  RooAbsReal& _mZZ,
				  RooAbsReal& _Gamma,
				  RooAbsReal& _p0,
				  RooAbsReal& _p1,
				  RooAbsReal& _p2,
				  RooAbsReal& _p3,
				  RooAbsReal& _mean,
				  RooAbsReal& _width
			      
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
   mean("mean","mean",this,_mean),
   width("width","width",this,_width)
 { 
 } 


 Roo2l2jMasses2D::Roo2l2jMasses2D(const Roo2l2jMasses2D& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZstar("mZstar",this,other.mZstar),
   mZ("mZ",this,other.mZ),
   mZZ("mZZ",this,other.mZZ),
   Gamma("Gamma",this,other.Gamma),
   p0("p0",this,other.p0),
   p1("p1",this,other.p1),
   p2("p2",this,other.p2),
   p3("p3",this,other.p3),
   mean("mean"  ,this,other.mean),
   width("width",this,other.width)
 { 
 } 


 double Roo2l2jMasses2D::evaluate() const 
 { 
   double mZstarDistribution;
   double Numerator,Denominator,beta;
   beta = (1-(mZstar-mZ)*(mZstar-mZ)/(mZZ*mZZ))*(1-(mZstar+mZ)*(mZstar+mZ)/(mZZ*mZZ));

   if(beta<0.0) return .00000001;

   Numerator = mZstar*mZstar*sqrt(beta)*(2+(mZstar*mZstar-mZZ*mZZ+mZ*mZ)*(mZstar*mZstar-mZZ*mZZ+mZ*mZ)/(4*mZstar*mZstar*mZ*mZ));
   Denominator = 2*(mZstar*mZstar-mZ*mZ)*(mZstar*mZstar-mZ*mZ)-2*mZ*mZ*Gamma*Gamma;

   //mZstarDistribution = Numerator/Denominator;

   // added on November 8, 2011 --- not part of default
   mZstarDistribution = beta;

   //double acceptance = 1+p0*mZstar+p1*mZstar*mZstar+p2*mZstar*mZstar*mZstar;

   // added on November 8, 2011 --- not part of default 
   double x = (mZstar-mZstar.min())/(mZstar.max()-mZstar.min());
   //-------- order-2 bernstein polynomial -----------
   //double acceptance = p0*pow(1-x,2)+p1*2*x*(1-x)+p2*x*x;
   //-------- order-3 bernstein polynomial -----------
   double acceptance = p0*pow(1-x,3)+p1*3*x*pow(1-x,2)+p2*3*x*x*(1-x)+p3*x*x*x;

   double mZZDistribution;

   //========= gaussian =========

   mZZDistribution = exp(-(mZZ-mean)*(mZZ-mean)/(2*width*width))/sqrt(2*3.1415*width*width);

   return mZstarDistribution*acceptance*mZZDistribution;
 }

