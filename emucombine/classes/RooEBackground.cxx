 /***************************************************************************** 
  * Project: RooFit                                                           * 
  *                                                                           * 
  * This code was autogenerated by RooClassFactory                            * 
  *****************************************************************************/ 

 // Your description goes here... 

 #include "Riostream.h" 

 #include "RooEBackground.h" 
 #include "RooAbsReal.h" 
 #include "RooAbsCategory.h" 

 ClassImp(RooEBackground) 

 RooEBackground::RooEBackground(const char *name, const char *title, 
                        RooAbsReal& _ebx,
                        RooAbsReal& _ebslope,
                        RooAbsReal& _ebshape) :
   RooAbsPdf(name,title), 
   ebx("ebx","ebx",this,_ebx),
   ebslope("ebslope","ebslope",this,_ebslope),
   ebshape("ebshape","ebshape",this,_ebshape)
 { 
 } 


 RooEBackground::RooEBackground(const RooEBackground& other, const char* name) :  
   RooAbsPdf(other,name), 
   ebx("ebx",this,other.ebx),
   ebslope("ebslope",this,other.ebslope),
   ebshape("ebshape",this,other.ebshape)
 { 
 } 



 Double_t RooEBackground::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return exp(-ebx*ebslope)/TMath::Power(ebx,ebshape) ; 
 } 



