#include <iostream>
#include <math.h>
#include "TMath.h"
#include "Roo2l2jMasses2D_DoubleCB.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(Roo2l2jMasses2D_DoubleCB) 

 Roo2l2jMasses2D_DoubleCB::Roo2l2jMasses2D_DoubleCB(){}

Roo2l2jMasses2D_DoubleCB::Roo2l2jMasses2D_DoubleCB(const char *name, const char *title, 
						   RooAbsReal& _mZstar,
						   RooAbsReal& _mZ,
						   RooAbsReal& _mZZ,
						   RooAbsReal& _Gamma,
						   RooAbsReal& _p0,
						   RooAbsReal& _p1,
						   RooAbsReal& _p2,
						   RooAbsReal& _mean,
						   RooAbsReal& _width,
						   RooAbsReal& _a1,
						   RooAbsReal& _n1,
						   RooAbsReal& _a2,
						   RooAbsReal& _n2
						   ) :
   RooAbsPdf(name,title), 
   mZstar("mZstar","mZstar",this,_mZstar),
   mZ("mZ","mZ",this,_mZ),
   mZZ("mZZ","mZZ",this,_mZZ),
   Gamma("Gamma","Gamma",this,_Gamma),
   p0("p0","p0",this,_p0),
   p1("p1","p1",this,_p1),
   p2("p2","p2",this,_p2),
   mean("mean","mean",this,_mean),
   width("width","width",this,_width),
   a1("a1","a1",this,_a1),
   n1("n1","n1",this,_n1),
   a2("a2","a2",this,_a2),
   n2("n2","n2",this,_n2)
{ 
 } 


 Roo2l2jMasses2D_DoubleCB::Roo2l2jMasses2D_DoubleCB(const Roo2l2jMasses2D_DoubleCB& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZstar("mZstar",this,other.mZstar),
   mZ("mZ",this,other.mZ),
   mZZ("mZZ",this,other.mZZ),
   Gamma("Gamma",this,other.Gamma),
   p0("p0",this,other.p0),
   p1("p1",this,other.p1),
   p2("p2",this,other.p2),
   mean("mean"  ,this,other.mean),
   width("width",this,other.width),
   a1("a1",this,other.a1),
   n1("n1",this,other.n1),
   a2("a2",this,other.a2),
   n2("n2",this,other.n2)
 { 
 } 


 double Roo2l2jMasses2D_DoubleCB::evaluate() const 
 { 
   double mZstarDistribution;
   double Numerator,Denominator,beta;
   beta = (1-(mZstar-mZ)*(mZstar-mZ)/(mZZ*mZZ))*(1-(mZstar+mZ)*(mZstar+mZ)/(mZZ*mZZ));

   if(beta<0.0) return 0.00000001;

   Numerator = mZstar*mZstar*sqrt(beta)*(2+(mZstar*mZstar-mZZ*mZZ+mZ*mZ)*(mZstar*mZstar-mZZ*mZZ+mZ*mZ)/(4*mZstar*mZstar*mZ*mZ));
   Denominator = 2*(mZstar*mZstar-mZ*mZ)*(mZstar*mZstar-mZ*mZ)-2*mZ*mZ*Gamma*Gamma;

   mZstarDistribution = Numerator/Denominator;

   //double acceptance = p0+p1*mZstar+p2*mZstar*mZstar;
   
   double x = (mZstar-mZstar.min())/(mZstar.max()-mZstar.min());
   //--- order-2 bernstein polynomial ------
   double acceptance = p0*pow(1-x,2)+p1*2*x*(1-x)+p2*x*x;
   
   double mZZDistribution;

   //========= double crystal ball ========

   double A1 = pow(n1/fabs(a1),n1)*exp(-a1*a1/2);
   double A2 = pow(n2/fabs(a2),n2)*exp(-a2*a2/2);
   double B1 = n1/fabs(a1)-fabs(a1);
   double B2 = n2/fabs(a2)-fabs(a2);

   if((mZZ-mean)/width>-a1 && (mZZ-mean)/width<a2){
     mZZDistribution = exp(-(mZZ-mean)*(mZZ-mean)/(2*width*width));
   }else if((mZZ-mean)/width<-a1){
     mZZDistribution = A1*pow(B1-(mZZ-mean)/width,-n1);
   }else if((mZZ-mean)/width>a2){
     mZZDistribution = A2*pow(B2+(mZZ-mean)/width,-n2);
   }else{
     cout << "ERROR evaluating range..." << endl;
     mZZDistribution = 0.0 ;
   }

   return mZstarDistribution*acceptance*mZZDistribution;
 }

