#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooExtendPdf.h"
#include "RooFitResult.h"
#include "RooGlobalFunc.h"
#include "RooBreitWigner.h"
#include "RooGaussModel.h"
#include "RooGaussian.h"
#include "RooNumConvPdf.h"
#include "RooFFTConvPdf.h"
#include "RooAddPdf.h"
#include "RooProduct.h"
#include "RooFitResult.h"
#include "RooMsgService.h"
#include "RooSimultaneous.h"
#include "RooPlot.h"
#include "RooProdPdf.h"
#include "RooEBackground.h"
#include "RooMuBackground.h"
#include "RooLinkedList.h"
#include "RooCmdArg.h"
#include "RooPolynomial.h"

#include "TRandom3.h"
#include "TH1F.h"
#include "TCanvas.h"

void elsewhere(int it){
  TRandom3 rand(0);

  //deactivate generation messages
  RooMsgService::instance().setStreamStatus(1,kFALSE);

  //setup fit options
  RooLinkedList options;
  RooCmdArg arg1=Minos(kFALSE);  
  options.Add(&arg1);
  RooCmdArg arg2=Hesse(kFALSE);
  options.Add(&arg2);
  RooCmdArg arg3=PrintLevel(-1);
  options.Add(&arg3);
  RooCmdArg arg4=Save(kTRUE);
  options.Add(&arg4);
  RooCmdArg arg5=Extended(kTRUE);
  options.Add(&arg5);


  //master variable
  RooRealVar mass("mass","invariant mass",200,2000,"GeV");

  RooRealVar gamma("gamma","BW gamma",30);
  RooRealVar sigmae("sigmae","gaus sigma e",15);
  RooRealVar peak("peak","mass peak",1000,500,2000);
  RooRealVar origin("origin","zero",0);
  RooBreitWigner bwe("bwe","breit wigner e",mass,peak,gamma);
  RooGaussModel gme("gme","det smearing e",mass,origin,sigmae);
  RooNumConvPdf signale("signale","signal pdf e",mass,bwe,gme);
  signale.setConvolutionWindow(origin,sigmae,3);


  //background
  RooRealVar bckgslope("bckgslope","slope parameter",0.00291045);
  RooRealVar bckgshape("bckgshape","shape parameter",3.177);
  RooEBackground bckg("bckg","e background",mass,bckgslope,bckgshape);

  RooRealVar norm("norm","norm",100,0,200);
  RooRealVar normbkge("normbkge","normbkge",70,0,200);
  RooExtendPdf extsige("extsige","ext sig e",*shiftsige,norme);
  RooExtendPdf extbkge("extbkge","ext bkg e",bckg,normbkge);

  RooAddPdf sume("sume","signal + bkg e",RooArgList(extsige,extbkge));
  for(int i = 0 ; i < it ;i++){
    if(i%10 == 0)      std::cout << i << std::endl;

    massshift->setVal(1.0);massshift->setValueDirty();
    //draw poisson variable for signal+background
    int bckmeane  = rand.Poisson( 70.);
    int sigmeanmu = rand.Poisson( 10.);
    int bckmeanmu = rand.Poisson( 70 );
        
    //generate signal+back sample, each poisson distributed (careful with empty sets)
    RooDataSet *datae,*dataeb,*datamu, *datamub;  
    if(sigmeane == 0)
      datae = new RooDataSet("datae","datae",RooArgSet(mass));
    else
      datae  = signale.generate(RooArgSet(mass),sigmeane) ; //extsige may be integrated over the masshift=> use bare signal
    if(bckmeane == 0)
      dataeb = new RooDataSet("dataeb","dataeb",RooArgSet(mass));
    else
      dataeb  = extbkge.generate(RooArgSet(mass),bckmeane) ;
    if(sigmeanmu ==0 )
      datamu = new RooDataSet("datamu","datamu",RooArgSet(mass));
    else    
      datamu =  extsigmu.generate(RooArgSet(mass),sigmeanmu) ;
    if(bckmeanmu ==0 )
      datamub = new RooDataSet("datamub","datamub",RooArgSet(mass));
    else
      datamub = extbkgmu.generate(RooArgSet(mass),bckmeanmu) ;
