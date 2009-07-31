#ifndef __Model_h__
#define __Model_h__

#include "RooAbsPdf.h"
#include "RooLinkedList.h"
#include "RooSimultaneous.h"
#include "RooRealVar.h"
#include "RooProduct.h"
#include "RooGaussian.h"
#include "RooVoigtian.h"
#include "RooCategory.h"
#include "RooMuBackground.h"
#include "RooEBackground.h"
#include "RooExtendPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"

#include "TRandom3.h"

class DataSetHelper;

class Model{
 public:
  Model(int,int,bool,bool=false);
  ~Model();

  RooAddPdf* sume,*summu;                                // signal + backgr pdfs
  RooAbsPdf* extbkge,*extbkgmu;                          // background pdfs
  RooSimultaneous* extbkgboth;                           // combined background pdf
  RooSimultaneous* sumboth;                              // combined signal + background
  RooRealVar* mass;                                      // mass: master variable
  RooRealVar* peak;                                      // peak position (directly used for muons)
  RooRealVar* gamma;                                     // peak natural width
  RooRealVar* sigmamu,*sigmarele;                        // mass resolutin XXX make function of mass XXX      
  RooProduct* sigmae;
  RooRealVar* norm,*normbkge,*normbkgmu;                 // singal and background cross sections
  RooRealVar* bkgslopmu,*bkgslope,*bkgshape;             // background shape parameters
  RooRealVar* effratio,*effratiomean,*effratiosigma;     // efficiency ratio and distribution params.
  RooRealVar* massshift,*massshiftmean,*massshiftsigma;  // mass scale and distribution params.
  RooProduct* shiftedpeak,*norme;                        // peak position and cross section corrected
                                                         // for syst. uncertainties (used for e only)
  double truepeak;                                       // peak value for generation
  double trueshift;                                      // mass shift for generation
  double trueeff;                                        // efficiency for generation
  double trueslopemu;
  double truewidth;
  double truerese;                                       // true electron resolution
  
  double fitwithpeak;
  double fitwithshift;
  double fitwitheff;
  double fitwithslopemu;                                 // for the fit, set the value to this
  double fitwithwidth;                                 // for the fit, use this
  double fitwithrese;

  RooCategory sample;                                    // defines e/mu for RooFit

  RooFitResult* fitrese,*fitresmu,*fitresboth;           // fit results for signal +background
  RooFitResult* fitresbe,*fitresbmu,*fitresbboth;        // fit results for background only fits

  // private:
  RooLinkedList options;                                 // fit option
  int effoption;                                         // 1-fixed 2-free 3-contrained 4-integrated, only 1 and 2 fully operational;
  int massoption;                                        // 1-fixed 2-free 3-contrained 4-integrated, only 1 and 2 fully operational;
  bool fitmass;                                          // should the peak value be a fit parameter?
  bool fitwidth;

  TRandom3 rand;                                         // std. root random numbers

  RooGaussian* effratiodist;                             // various indertmediate pdfs
  RooGaussian* massshiftdist;                            // and variables
  RooVoigtian* signale,*signalmu;                        //  
  RooMuBackground* bkgmu;                                //
  RooEBackground* bkge;                                  //
  RooAbsPdf* shiftsige;                                  //
  RooProdPdf* shiftprod;                                  //
  RooAbsPdf* extsige,*extsigmu;

  void fit(DataSetHelper&);                         // fit to given datasets

};

#endif
