#include <iostream>
#include <math.h>
#include "TMath.h"
#include "Roo2DGauss.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

using namespace RooFit;

ClassImp(Roo2DGauss) 

 Roo2DGauss::Roo2DGauss(){}

Roo2DGauss::Roo2DGauss(const char *name, const char *title, 
		       RooAbsReal& _mZstar,
		       RooAbsReal& _mZZ,
		       RooAbsReal& _mean,
		       RooAbsReal& _mean2,
		       RooAbsReal& _width,
		       RooAbsReal& _width2,
		       RooAbsReal& _width3
		    ) :
   RooAbsPdf(name,title), 
   mZstar("mZstar","mZstar",this,_mZstar),
   mZZ("mZZ","mZZ",this,_mZZ),
   mean("mean","mean",this,_mean),
   mean2("mean2","mean2",this,_mean2),
   width("width","width",this,_width),
   width2("width2","width2",this,_width2),
   width3("width3","width3",this,_width3)
 { 
 } 


 Roo2DGauss::Roo2DGauss(const Roo2DGauss& other, const char* name) :  
   RooAbsPdf(other,name), 
   mZstar("mZstar",this,other.mZstar),
   mZZ("mZZ",this,other.mZZ),
   mean("mean"  ,this,other.mean),
   width("width",this,other.width),
   mean2("mean2"  ,this,other.mean2),
   width2("width2",this,other.width2),
   width3("width3",this,other.width3)
 { 
 } 


 double Roo2DGauss::evaluate() const 
 { 
   double G1, G2, G3;

   G1=exp(-(mZZ-mean)*(mZZ-mean)/(2*width*width));
   //cout << "G1" << G1 << endl;
   G2=exp(-(mZstar-mean2)*(mZstar-mean2)/(2*width2*width2));
   //cout << "G2" << G2 << endl;
   G3=exp(-(mZZ-mean)*(mZstar-mean2)/(width3*width3));
   //cout << "G3" << G3 << endl;

   return G1*G2*G3;
 }

