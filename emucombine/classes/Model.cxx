#include "Model.h"
#include "RooGlobalFunc.h"
#include "RooNumIntConfig.h"
#include "DataSetHelper.h"
#include "RooRandom.h"

using namespace RooFit;

Model::Model(int effo,int masso,bool fitm,bool fitw): // constructor sets up PDF's to reflect constraints
  //effoption: 1-fixed 2-free 3-contrained 4-integrated, only 1 and 2 fully operational;
  //msasoption: 1-fixed 2-free 3-contrained 4-integrated, only 1 and 2 fully operational;
  //other values default to "fixed";

  //initialize various pointers to zero
  sume(0),summu(0),
  extbkge(0),extbkgmu(0),
  extbkgboth(0),
  sumboth(0),
  mass(0),  
  peak(0),
  gamma(0),
  sigmarelmu(0),sigmarele(0),sigmae(0),sigmamu(0),
  norm(0),normbkge(0),normbkgmu(0),
  bkgslopmu(0),bkgslope(0),bkgshape(0),
  effratio(0),effratiomean(0),effratiosigma(0),
  massshift(0),massshiftmean(0),massshiftsigma(0),
  shiftedpeak(0),norme(0),
  truepeak(1000.),
  trueshift(1.),
  trueeff(1.),
  trueslopemu(-2),
  truewidth(30),
  trueresmu(0.08),
  truerese(0.005),
  fitwithpeak(truepeak),
  fitwithshift(trueshift),
  fitwitheff(trueeff),
  fitwithslopemu(trueslopemu),
  fitwithwidth(truewidth),
  fitwithresmu(trueresmu),fitwithrese(truerese),
  sample("sample","sample"),
  fitrese(0),fitresmu(0),fitresboth(0),
  fitresbe(0),fitresbmu(0),fitresbboth(0),
  options(0),
  effoption(effo),
  massoption(masso),
  fitmass(fitm),
  fitwidth(fitw),
  rand(0),
  effratiodist(0),
  massshiftdist(0),
  signale(0),signalmu(0),
  bkgmu(0),
  bkge(0),
  shiftsige(0),
  shiftprod(0),
  extsige(0),extsigmu(0)
{
  //increase normaisation precision
  RooAbsReal::defaultIntegratorConfig()->setEpsAbs(1e-12) ;
  RooAbsReal::defaultIntegratorConfig()->setEpsRel(1e-12) ;
  RooAbsReal::defaultIntegratorConfig()->method1D().setLabel("RooAdaptiveGaussKronrodIntegrator1D") ;
  RooAbsReal::defaultIntegratorConfig()->method1DOpen().setLabel("RooAdaptiveGaussKronrodIntegrator1D");



  
  //deactivate generation messages
  RooMsgService::instance().setStreamStatus(1,kFALSE);

  //mass: master variable
  mass = new RooRealVar("mass","invariant mass",500,6500,"GeV");
  
  //define data types
  sample.defineType("e");
  sample.defineType("mu");

  //systematics: efficiency ratio
  effratiomean = new RooRealVar("effratiomean","effratiomean",1.);
  effratiosigma = new RooRealVar("effratiosigma","effratiosigma",.1 );
  switch(effoption){
  case 2: //free
    effratio = new RooRealVar("effratio","effratio",1.,0.,10.); break;
  case 3: //contraint
    effratio = new RooRealVar("effratio","effratio",1.,0.,10.);
    effratiodist = new RooGaussian("effratiodist","effratiodist",*effratio,*effratiomean,*effratiosigma);
    break;
  case 4: // integrated
    effratio = new RooRealVar("effratio","effratio",1.,0.,10.);
    effratiodist = new RooGaussian("effratiodist","effratiodist",*effratio,*effratiomean,*effratiosigma);
    break;
  default: //default is fixed
    effratio = new RooRealVar("effratio","effratio",1.);
  }

  //systematics: mass scale
  massshiftmean = new RooRealVar("massshiftmean","massshiftmean",1.);
  massshiftsigma = new RooRealVar("massshiftsigma","massshiftsigma",.1 );
  switch(massoption){
  case 2: //free
    massshift = new RooRealVar("massshift","massshift",1.,0.6,1.4); break;
  case 3: //contraint
    massshift = new RooRealVar("massshift","massshift",1.,0.6,1.4);
    massshiftdist = new RooGaussian("massshiftdist","massshiftdist",*massshift,*massshiftmean,*massshiftsigma);break;
  case 4: // integrated
    massshift = new RooRealVar("massshift","massshift",1.,0.6,1.4);
    massshiftdist = new RooGaussian("massshiftdist","massshiftdist",*massshift,*massshiftmean,*massshiftsigma);break;
  default: //default is fixed
    massshift = new RooRealVar("massshift","massshift",1.);
  }

  //define signal functions
  peak       = new RooRealVar("peak","mass peak",truepeak);
  gamma      = new RooRealVar("gamma","BW gamma",truewidth);
  if(fitwidth) {gamma->setRange(0,50);gamma->setConstant(kFALSE);gamma->setError(1.);}
  sigmarelmu  = new RooRealVar("sigmamural","relative mu reslution",trueresmu);
  sigmamu    = new RooProduct("sigmamu","gaus sigma mu",RooArgSet(*sigmarelmu,*peak));
  sigmarele  = new RooRealVar("sigmaeral","relative e reslution",truerese);
  sigmae     = new RooProduct("sigmae","gaus sigma e",RooArgSet(*sigmarele,*peak));
  if(fitmass) {peak->setRange(100,6500);peak->setConstant(kFALSE);peak->setError(1.);}
  shiftedpeak= new RooProduct("shiftedpeak","shiftedpeak",RooArgSet(*peak,*massshift));
  signale    = new RooVoigtian("signale","signal pdf e",*mass,*shiftedpeak,*gamma,*sigmae);
  signalmu   = new RooVoigtian("signalmu","signal pdf mu",*mass,*peak,*gamma,*sigmamu);

  //define background functions
  bkgslopmu  = new RooRealVar("bkgsig","bkgsig",-2);
  bkgmu      = new RooMuBackground("bckgmu","muon background",*mass,*bkgslopmu);

  
  bkgslope   = new RooRealVar("bckgslope","slope parameter",0.001308);
  bkgshape   = new RooRealVar("bckgshape","shape parameter",3.8729);
  //  bkgslope   = new RooRealVar("bckgslope","slope parameter",0.00225);
  //  bkgshape   = new RooRealVar("bckgshape","shape parameter",3.55);
  bkge       = new RooEBackground("bckg","e background",*mass,*bkgslope,*bkgshape);

  
  //integrate singal pdf over mass-scale uncertainty if necessary
  switch(massoption){
  case 3: // constrained 
    shiftsige= new RooProdPdf("temp product","temp product",RooArgSet(*signale,*massshiftdist));
    options.Add( new RooCmdArg( Constrain(*massshift) ) );break;    
  case 4: // integrated
    shiftprod= new RooProdPdf("temp product","temp product",RooArgSet(*signale,*massshiftdist));
    shiftsige = shiftprod->createProjection(RooArgSet(*massshift));break;
  default: //fixed/free/constrained -> do nothing
    shiftsige = signale;
  }

  //add extended terms
  norm = new RooRealVar("norm","norm",10,0,100);
  norm->setError(1.);
  norme = new RooProduct("norme","norme",RooArgSet(*norm,*effratio));
  normbkge = new RooRealVar("normbkge","normbkge",10,0,100);
  normbkge->setError(1.);
  normbkgmu = new RooRealVar("normbkgmu","normbkgmu",10,0,100);
  normbkgmu->setError(1.);
  extsige = new RooExtendPdf("extsige","ext sig e",*shiftsige,*norme);
  extsigmu = new RooExtendPdf("extsigmu","ext sig mu",*signalmu,*norm);
  extbkge = new RooExtendPdf("extbkge","ext bkg e",*bkge,*normbkge);
  extbkgmu = new RooExtendPdf("extbkgmu","ext bkg mu",*bkgmu,*normbkgmu);

  // add signal and background
  sume = new RooAddPdf("sume","signal + bkg e",RooArgList(*extsige,*extbkge));
  summu = new RooAddPdf("summu","signal + bkg mu",RooArgList(*extsigmu,*extbkgmu));

  //integrate singal pdf over mass-scale uncertainty if necessary
  switch(effoption){
  case 3: // constrained 
    options.Add( new RooCmdArg( ExternalConstraints(*effratiodist) ) );break;    
  }
 

  // Construct a simultaneous pdf using category sample as index
  sumboth = new RooSimultaneous("simPdf","simultaneous pdf",sample) ;
  sumboth->addPdf(*sume,"e") ;
  sumboth->addPdf(*summu,"mu") ;
  
  extbkgboth = new RooSimultaneous("simbPdf","simultaneous background only",sample);
  extbkgboth->addPdf(*extbkge,"e");
  extbkgboth->addPdf(*extbkgmu,"mu");

}

Model::~Model(){
}


void Model::fit(DataSetHelper& data){
  // clear old fitresults
  if( fitrese  ) delete fitrese;
  if( fitresmu  ) delete fitresmu;
  if( fitresboth  ) delete fitresboth;
  if( fitresbe  ) delete fitresbe;
  if( fitresbmu  ) delete fitresbmu;
  if( fitresbboth   ) delete fitresbboth;
  

  if(data.datae){ //electron data exists => fit it
    // set useful starting parameters and errors
    normbkge->setVal(10);//XXXXX 10
    normbkge->setValueDirty();
    normbkge->setError(.1);

    // background only for 
    fitresbe = extbkge->fitTo(*(data.datae),options);

    //reset paramters
    norm->setVal(10);
    norm->setValueDirty();
    norm->setError(.1);
    normbkge->setVal(10);//XXXXXX 10
    normbkge->setValueDirty();
    normbkge->setError(.1);
    gamma->setVal(fitwithwidth);
    gamma->setError(1.);
    gamma->setValueDirty();
    sigmarele->setVal(fitwithrese);
    sigmarele->setError(1.);
    sigmarele->setValueDirty();
    
    double min=0.,max=0.;
    if(!massshift->isConstant()){//start fit with the assumption of normal mass scale
      massshift->setVal(1.);
      massshift->setError(0.5);
      massshift->setValueDirty();
    }else{
      //massshift->setVal(fitwithshift);
      //massshift->setValueDirty();
    }
    if(!peak->isConstant()){ // if the peak value is constant, don't touch it
      data.datae->getRange(*mass,min,max);
      if (min< peak->getMin()) min = peak->getMin();
      peak->setVal(max/massshift->getVal());
      peak->setValueDirty();
      peak->setError(500.);
    }else{
      peak->setVal(fitwithpeak);
      peak->setValueDirty();
    }    
    if(!effratio->isConstant()){//start fit with the assumption of ideal efficiencies
      effratio->setVal(1.);
      effratio->setError(0.5);
      effratio->setValueDirty();
    }else{
      effratio->setVal(fitwitheff);
      effratio->setValueDirty();
    }

    if(!peak->isConstant()){ //finding the true minimum at variable peak-mass is tricky
      //do a scan of likelihood for good intial value
      double maxlike = -1000;
      int maxpoint =-1000;
      if( max > min){
	int nscanpoint= (int)ceil((max-min)/sigmae->getVal());
	std::cout << "scanning from " << min << " to " << max << " in " << nscanpoint << " steps" <<std::endl; 
	//fix width for scanning
	gamma->setConstant(kTRUE);
	for(int scanpoint = 0 ; scanpoint<nscanpoint;scanpoint++ ){
	  peak->setVal(min + scanpoint*(max-min)/nscanpoint);
	  peak->setConstant(kTRUE);
	  norm->setVal(10);
	  norm->setValueDirty();
	  norm->setError(.1);
	  normbkge->setVal(10);//XXXXX
	  normbkge->setValueDirty();
	  normbkge->setError(.1);
	  fitrese = sume->fitTo(*(data.datae),options);
	  double sig = sqrt(2*fabs( fitrese->minNll() - fitresbe->minNll())) ;
	  delete fitrese;
	  if(normbkge->isConstant()) sig=-sig;//XXXXXXX
	//std::cout << "sig: " << sig << " at " << peak->getVal() << std::endl;
	  if (sig>maxlike){
	    maxpoint = scanpoint;
	    maxlike = sig;
	  }
	}
	std::cout << "maximum at " << min + maxpoint*(max-min)/nscanpoint << " with " << maxlike <<std::endl;
	peak->setVal(min + maxpoint*(max-min)/nscanpoint);
	peak->setConstant(kFALSE);
      }
      else{
	peak->setVal(min);
      }
      if(fitwidth) gamma->setConstant(kFALSE);
      norm->setVal(10);
      norm->setValueDirty();
      norm->setError(.1);
      normbkge->setVal(10);//XXXXXX !!! 1.0
      normbkge->setValueDirty();
      normbkge->setError(.1);
    }
    fitrese = sume->fitTo(*(data.datae),options);    
    std::cout <<  fitresbe->minNll() << " " << fitrese->minNll()<< " " << sqrt(2*fabs( fitrese->minNll() - fitresbe->minNll())) << std::endl;
  }

  if(data.datamu){ //muon data exists => fit it
    // set useful starting parameters and errors
    normbkgmu->setVal(10);
    normbkgmu->setValueDirty();
    normbkgmu->setError(.1);
    bkgslopmu->setVal(fitwithslopemu);
    bkgslopmu->setValueDirty();


    // background only
    fitresbmu = extbkgmu->fitTo(*(data.datamu),options);

    //reset paramters
    norm->setVal(10);
    norm->setValueDirty();
    norm->setError(.1);
    normbkgmu->setVal(10);
    normbkgmu->setValueDirty();
    normbkgmu->setError(.1);
    gamma->setVal(fitwithwidth);
    gamma->setError(1.);
    gamma->setValueDirty();
    sigmarelmu->setVal(fitwithresmu);
    sigmarelmu->setError(1.);
    sigmarelmu->setValueDirty();


    double min=0.,max=0.;
    if(!peak->isConstant()){ // if the peak value is constant, don't touch it
      data.datamu->getRange(*mass,min,max);      
      peak->setVal(max);
      peak->setValueDirty();
      peak->setError(500.);
    }else{
      peak->setVal(fitwithpeak);
      peak->setValueDirty();
    }
    //signal and background
    //same comments as above
    if(!peak->isConstant()){
      //do a scan of likelihood for good intial value
      double maxlike = -1;
      int maxpoint =-1;
      int nscanpoint=(int)ceil((max-min)/sigmamu->getVal());
      //fix width for sccanning
      gamma->setConstant(kTRUE);
      for(int scanpoint = 0 ; scanpoint<nscanpoint;scanpoint++ ){
	peak->setVal(min + scanpoint*(max-min)/nscanpoint);
	peak->setConstant(kTRUE);
	norm->setVal(10);
	norm->setValueDirty();
	norm->setError(.1);
	normbkgmu->setVal(10);
	normbkgmu->setValueDirty();
	normbkgmu->setError(.1);
	fitresmu = summu->fitTo(*(data.datamu),options);
	double sig = sqrt(2*fabs( fitresmu->minNll() - fitresbmu->minNll())) ;
	delete fitresmu;
	if (sig>maxlike){
	  maxpoint = scanpoint;
	  maxlike = sig;
	}
      }
      peak->setVal(min + maxpoint*(max-min)/nscanpoint);
      peak->setConstant(kFALSE);
      if(fitwidth) gamma->setConstant(kFALSE);
      norm->setVal(10);
      norm->setValueDirty();
      norm->setError(.1);
      normbkgmu->setVal(10);
      normbkgmu->setValueDirty();
      normbkgmu->setError(.1);
    }      
    fitresmu = summu->fitTo(*(data.datamu),options);    
  }

  if(data.combdata){ //combined data exists => fit it
   // set useful starting parameters and errors
    normbkge->setVal(10);
    normbkge->setValueDirty();
    normbkge->setError(.1);
    normbkgmu->setVal(10);
    normbkgmu->setValueDirty();
    normbkgmu->setError(.1);
    bkgslopmu->setVal(fitwithslopemu);
    bkgslopmu->setValueDirty();

    // background only
    fitresbboth = extbkgboth->fitTo(*(data.combdata),options);
    
    //reset paramters
    norm->setVal(10);
    norm->setValueDirty();
    norm->setError(.1);
    normbkge->setVal(10);
    normbkge->setValueDirty();
    normbkge->setError(.1);
    normbkgmu->setVal(10);
    normbkgmu->setValueDirty();
    normbkgmu->setError(.1);
    gamma->setVal(fitwithwidth);
    gamma->setError(1.);
    gamma->setValueDirty();
    sigmarele->setVal(fitwithrese);
    sigmarele->setError(1.);
    sigmarele->setValueDirty();
    sigmarelmu->setVal(fitwithresmu);
    sigmarelmu->setError(1.);
    sigmarelmu->setValueDirty();

    double maxe=0.,maxmu=0.;

    if(!massshift->isConstant()){//start fit with the assumption of normal mass scale
      massshift->setVal(1.);
      massshift->setError(0.5);
      massshift->setValueDirty();
    }else{
      massshift->setVal(fitwithshift);
      massshift->setValueDirty();
    }
    if(!peak->isConstant()){ // if the peak value is constant, don't touch it
      //use the maximum from the simple fits to start
      //the maximum with the higher significance is used.
      //if the masshift is free we twiddle it to start at the prefered vales for e and mu separately
      maxe= dynamic_cast<RooAbsReal*>(fitrese->floatParsFinal().find("peak"))->getVal();
      maxmu= dynamic_cast<RooAbsReal*>(fitresmu->floatParsFinal().find("peak"))->getVal();      
      if(!massshift->isConstant()){// if the massshift is not constant we twiddle it to have good starting values for e and mu
	maxe *= dynamic_cast<RooAbsReal*>(fitrese->floatParsFinal().find("massshift"))->getVal();
	massshift->setVal(maxe/maxmu);
	massshift->setError(0.5);
	massshift->setValueDirty();
	peak->setVal(maxmu);
	peak->setValueDirty();
	peak->setError(500.);	
      }
      else{
	double sige = sqrt(2*fabs(fitresbe->minNll() - fitrese->minNll()));
	double sigmu= sqrt(2*fabs(fitresbmu->minNll() - fitresmu->minNll()));
	if(sige>sigmu)
	  peak->setVal(maxe*dynamic_cast<RooAbsReal*>(fitrese->constPars().find("massshift"))->getVal());
	else
	  peak->setVal(maxmu);
	peak->setValueDirty();
	peak->setError(500.);
     }
    }else{
      peak->setVal(fitwithpeak);
      peak->setValueDirty();
    }   
    if(!effratio->isConstant()){//start fit with the assumption of ideal efficiencies
      effratio->setVal(1.);
      effratio->setError(0.5);
      effratio->setValueDirty();
    }else{
      effratio->setVal(fitwitheff);
      effratio->setValueDirty();
    }


    //signal and background
    fitresboth = sumboth->fitTo(*(data.combdata),options);
 
  }
}
