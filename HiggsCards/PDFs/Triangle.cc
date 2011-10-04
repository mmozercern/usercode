#include <iostream>
#include <math.h>

#include "Triangle.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"

ClassImp(Triangle)

  Triangle::Triangle(){}

Triangle::Triangle(const char *name, const char *title,                
		   RooAbsReal& _m,
		   RooAbsReal& _start,
		   RooAbsReal& _turn,
		   RooAbsReal& _stop
		   ):
  RooAbsPdf(name, title),
  m("m", "Dependent", this, _m),
  start("start","start",this,_start),
  turn("turn","turn",this,_turn),
  stop("stop","stop",this,_stop)
{
}

Triangle::Triangle(const Triangle& other, const char* name) :
  RooAbsPdf(other, name), m("m", this, other.m),start("start", this, other.start), turn("turn", this, other.turn), stop("stop", this, other.stop)
{
}

Double_t Triangle::evaluate() const 
{

  //std::cout << m << " "<<1.+(start-m)/turn << " " << 1+(turn-m)/stop << std::endl;
  if(m<turn  && m > turn+start)
    return 1.+(turn-m)/start;
  if(m>=turn && m < turn+stop)
    return 1.+(turn-m)/stop;
  
  return 0;
}


Int_t Triangle::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* range) const 
{
  if (matchArgs(allVars,analVars,m)) return 1;
  return 0;
}

Double_t Triangle::analyticalIntegral(Int_t code, const char* rangeName) const 
{

  // WARNING, ASSSUMES TURN TO BE IN INTERVAL
  assert(code==1) ;
  //whole triangle
  Double_t sumleft = sqrt(1+ (turn+start)*(turn+start) ) ;
  Double_t sumright= sqrt(1+ (turn+stop)*(turn+stop) );


  if(m.min(rangeName) < turn+start)// correct for left missing bit
    sumleft -= sumleft*(m.min(rangeName)-(turn+start))/fabs(start);


  if(m.max(rangeName) > turn+stop)// correct for right missing bit
    sumright -= sumright*(turn+stop -m.max(rangeName))/fabs(stop);

  

  return sumleft+sumright;    
}
